/*
 *
 */

float3 getVoxel (private int index)
{
	int3 vox_index, img_size;
	float3 delta, voxel0;
	//definir delta y voxel0
	delta = (float3) (VOXEL_SIZE_X, VOXEL_SIZE_Y, VOXEL_SIZE_Z);
	voxel0 = (float3) (VOXEL_REF_X, VOXEL_REF_Y, VOXEL_REF_Z);
	//obtener el tamaño de la imagen
	img_size = (int3) (VOXEL_INDEX_X_MAX, VOXEL_INDEX_Y_MAX, VOXEL_INDEX_Z_MAX);
	//calcular índice del voxel
	vox_index.z = index % img_size.z;
	vox_index.y = (index / img_size.z) % img_size.y;
	vox_index.x = (index / img_size.z) / img_size.y;
	//calcular la pocición
	return fma ( convert_float3(vox_index), delta, voxel0); 
}

__kernel void getVoxels(global float3 * positions){
	int index = get_global_id(0);
	float3 localResult;
	localResult = getVoxel(index);
	positions[index] = localResult;
}
