#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <CL/opencl.h>

#include "deployer.h"
#include "cl_reader.h"
#include "debug.h"

char message[4096];


//******************************************************************************
int set_device(){
	cl_int error;
	//selecciona la primera plataforma
	error = clGetPlatformIDs(1, &platform,NULL);
	if ( error != CL_SUCCESS ){
		sprintf (message, "set_device(): clGetPlatformsIDs() retornó un error número %i", error);
		error_msg(message);
		return DEPLOYER_ERROR;
	}
	//selecciona el primer dispositivo de la primera plataforma
	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (error != CL_SUCCESS)
	{
		sprintf(message, "set_device(): clGetDeviceIDs() retornó un error número %i", error);
		error_msg(message);
		return DEPLOYER_ERROR;
	}
	//crea un contexto con el dispositivo anterior
	context = clCreateContext(0, 1, &device, NULL, NULL, &error);
	if ( ! context ){
		sprintf(message, "set_device(): clCreateContext() retornó NULL error= %i", error);
		error_msg(message);
		return DEPLOYER_ERROR;
	}
	if (error != CL_SUCCESS){
		sprintf(message, "set_device(): clCreateContext() retornó un error número %i", error);
		error_msg(message);
		return DEPLOYER_ERROR;
	}
	//crear una cola para enviar los kernels
	commands = clCreateCommandQueue(context, device, 0, &error);
    if (!commands){
        sprintf(message, "set_device(): clCreateComandQueue retornó un error n° %i", error);
        error_msg(message);
        return DEPLOYER_ERROR;
    }
	return DEPLOYER_SUCCESS;
}

//******************************************************************************
char * platform_and_device_info(){
	char plat_name[128], plat_vendor[128];
	char dev_name[128];
	char * printable_info;
	clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(plat_name), plat_name, NULL);
	clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, sizeof(plat_vendor), plat_vendor, NULL);
	clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(dev_name), dev_name, NULL);
	printable_info = (char *) malloc ( sizeof(char) * 2048 );
	if (printable_info == NULL){
		error_msg("platform_and_device_info(): malloc devolvió NULL para printable_info");
		return NULL;
	}else{
		sprintf(printable_info, "\tNombre palataforma: %s\n\tFabricante: %s\n\tNombre dispositivo: %s\n", plat_name, plat_vendor, dev_name);
	}
	return printable_info;
}

//******************************************************************************
cl_int deploy_script(char *path, int img_size_x, int img_size_y, int img_size_z, 
						float delta_x, float delta_y, float delta_z, float voxel0_x, 
						float voxel0_y, float voxel0_z) {
	cl_int error;
	read_kernel * target_kernel;
	char compiler_options[1024];
	//asignar los valores a las variables globales
	IMG_SIZE_X = img_size_x;
	IMG_SIZE_Y = img_size_y;
	IMG_SIZE_Z = img_size_z;
	VOXEL_SIZE_X = delta_x;
	VOXEL_SIZE_Y = delta_y;
	VOXEL_SIZE_Z = delta_z;
	NUMBER_OF_VOXELS = IMG_SIZE_X * IMG_SIZE_Y* IMG_SIZE_Z;
	//asignar el tamaño de la imagen y voxeles a las opciones del compilador
	sprintf(compiler_options, "-DVOXEL_INDEX_X_MAX=%i -DVOXEL_INDEX_Y_MAX=%i -DVOXEL_INDEX_Z_MAX=%i -DVOXEL_SIZE_X=%f -DVOXEL_SIZE_Y=%f -DVOXEL_SIZE_Z=%f -DVOXEL_REF_X=%f -DVOXEL_REF_Y=%f -DVOXEL_REF_Z=%f",
								img_size_x, img_size_y,	img_size_z, 
								delta_x, delta_y, delta_z, 
								voxel0_x, voxel0_y, voxel0_z);
	//leer el script desde el archivo
	target_kernel = read_kernel_from_file( path );
	if (target_kernel == NULL){
		sprintf (message, "deploy_script(path= %s): ha ocurrido un error al intentar leer el script", path);
		error_msg(message);
		return DEPLOYER_ERROR;
	}
	//cargar el contenido del archivo a un programa
	program = clCreateProgramWithSource(context, 1,(const char**) & (target_kernel->script), NULL, &error);
	if ( (error < 0) || (program == NULL) ){
		sprintf(message, "deploy_script(path= %s): clCreateProgramWithSource() ha devuelto un error número %i", path, error);
		error_msg(message);
		return DEPLOYER_ERROR;
	}
	//compilar el programa
	error = clBuildProgram(program, 1, &device, compiler_options, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		size_t len;
		char buffer[4096];
		//si hay errores de compilación, se imprimen por pantalla.
		sprintf(message, "deploy_script(path= %s): clBuildProgram() ha devuelto un error número %i", path, error);
		error_msg(message);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		sprintf(message, "deploy_script(path= %s): este es el log del compilador:\n%s\n", path, buffer);
		info_msg(message);
		return DEPLOYER_ERROR;
	}
	//obtener la función (kernel) que se ejecutará del programa.
	kernel_square = clCreateKernel(program, "getSegment", &error);
	if (error != CL_SUCCESS){
		sprintf(message, "deployt_script(path= %s): clCreateKernel() ha devuelto un error número %i", path, error);
		error_msg(message);
		return DEPLOYER_ERROR;
	}
	free(target_kernel);
	return DEPLOYER_SUCCESS;
}

//******************************************************************************
float * get_segments(float * crystal0, float * crystal1){
	size_t local, global = (size_t) NUMBER_OF_VOXELS; //número de unidades de trabajo openCL
	int error;  //errores devueltos por openCL
	float * segments = (float *) calloc (NUMBER_OF_VOXELS, sizeof(float));
	char message[256];
	if (segments != NULL){
		//crear buffers de entrada que contienen las posiciones de los cristales del LOR
		input_crystal0 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * 3, NULL, NULL);
		input_crystal1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * 3, NULL, NULL);
		//crear buffer de salida, que contendrá los tamaños de segmento
		output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * NUMBER_OF_VOXELS, NULL, NULL);
		if ((!output) || (!input_crystal0) || (!input_crystal1)){ //si no se construyó uno de los buffers
			error_msg("get_voxel_positions(): clCreateBuffer() no asignó memoria de salida");
			return NULL;
		}
		//asignar posiciones de los cristales en los argumentos del kernel
		error = clEnqueueWriteBuffer(commands, input_crystal0, CL_TRUE, 0, sizeof(float) * 3, crystal0, 0, NULL, NULL);
		if (error != CL_SUCCESS){
			sprintf(message, "get_voxel_positions(): clEnqueueWriteBuffer retornó un error %i para el cristal 0", error);
			error_msg(message);
			return NULL;
		}
		
		error = clEnqueueWriteBuffer(commands, input_crystal1, CL_TRUE, 0, sizeof(float) * 3, crystal1, 0, NULL, NULL);
		if (error != CL_SUCCESS){
			sprintf(message, "get_voxel_positions(): clEnqueueWriteBuffer retornó un error %i para el cristal 1", error);
			error_msg(message);
			return NULL;
		}
		//asignar el buffer de salida como argumento del kernel
		error  = clSetKernelArg(kernel_square, 0, sizeof(cl_mem), &input_crystal0);
		if (error != CL_SUCCESS){
			sprintf(message, "get_voxel_positions(): clSetKernelArg retornó un error n° %i para input_crystal0", error);
			error_msg(message);
			return NULL;
		}
		error  = clSetKernelArg(kernel_square, 1, sizeof(cl_mem), &input_crystal1);
		if (error != CL_SUCCESS){
			sprintf(message, "get_voxel_positions(): clSetKernelArg retornó un error n° %i para input_crystal1", error);
			error_msg(message);
			return NULL;
		}
		error  = clSetKernelArg(kernel_square, 2, sizeof(cl_mem), &output);
		if (error != CL_SUCCESS){
			sprintf(message, "get_voxel_positions(): clSetKernelArg retornó un error n° %i para output", error);
			error_msg(message);
			return NULL;
		}	
		//obtener el tamaño del local workgroup
		error = clGetKernelWorkGroupInfo(kernel_square, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
		if (error != CL_SUCCESS){
			sprintf(message, "get_voxel_positions(): clGetKernelWorkGroupInfo retornó un error n° %i", error);
			error_msg(message);
			return NULL;	
		}
		if (local > global){
			local = global;
		}
		//enviar el kernel a la cola de ejecución
		error = clEnqueueNDRangeKernel(commands, kernel_square, 1, NULL, &global, &local, 0, NULL, NULL);
		if (error != CL_SUCCESS){
			sprintf(message, "get_voxel_positions(): clEnqueueNDRangeKernel retornó un error n° %i", error);
			error_msg(message);
			sprintf(message, "get_voxel_positions(): global= %i local= %i", (int)global, (int)local);
			info_msg(message);
			return NULL;	
		}
		//esperar hasta que termine su ejecución
		clFinish(commands);
		//leer los resultados
		error = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float) * NUMBER_OF_VOXELS, segments, 0, NULL, NULL );
		if (error != CL_SUCCESS){
			sprintf(message, "get_voxel_positions(): clEnqueueReadBuffer retornó un error n° %i", error);
			error_msg(message);
			return NULL;	
		}
		//liberar los objetos ocupados y retornar el resultado
		clReleaseMemObject(input_crystal0);
		clReleaseMemObject(input_crystal1);
		clReleaseMemObject(output);
		return segments;
	} else {
		error_msg("get_segments(): calloc retornó NULL");
	}
	return NULL;
}

void release_script(){
	clReleaseKernel(kernel_square);
	clReleaseProgram(program);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
}
