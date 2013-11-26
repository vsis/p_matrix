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
	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
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
	return DEPLOYER_SUCCESS;
}

//******************************************************************************
char * platform_and_device_info(){
	char plat_name[64], plat_vendor[64];
	char dev_name[64];
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
cl_int deploy_script(char *path){
	cl_int error;
	read_kernel * target_kernel;
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
	error = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
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
	kernel_square = clCreateKernel(program, "examplePos", &error);
	if (error != CL_SUCCESS){
		sprintf(message, "deployt_script(path= %s): clCreateKernel() ha devuelto un error número %i", path, error);
		error_msg(message);
		return DEPLOYER_ERROR;
	}
	return DEPLOYER_SUCCESS;
}

