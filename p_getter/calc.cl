/*__kernel void getSegments (__global float * crystals, __global int * voxels_segments_size, __global int * voxels_indexes, __global float * voxels_segments)
{
	int i = get_global_id(0);
}
*/
__kernel void getVoxelPos (int voxelIndex, float4 voxelPosition)
{
	voxelPosition.x = fma((voxelIndex % VOXEL_INDEX_X_MAX), VOXEL_SIZE_X, VOXEL_REF_X);
	voxelPosition.y = fma(((voxelIndex / VOXEL_INDEX_X_MAX) % VOXEL_INDEX_Y_MAX), VOXEL_SIZE_Y, VOXEL_REF_Y);
	voxelPosition.z = fma((voxelIndex / (VOXEL_INDEX_X_MAX * VOXEL_INDEX_Y_MAX)), VOXEL_SIZE_Z, VOXEL_REF_Z);
}

__kernel void examplePos (__global float * xPos, __global float * yPos, __global float *zPos){
	int index = get_global_id(0);
	float3 position;
	getVoxelPos(index, &position);
	xPos[index] = position.x;
	yPos[index] = position.y;
	zPos[index] = position.z;
}

