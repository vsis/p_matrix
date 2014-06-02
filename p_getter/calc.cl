/*
 * 	script que retorna el tamaño de los segmentos de 
 * 	intersección entre un LOR y los voxeles de la imágen.
 * 	retorna una fila de la matriz 
 */


#define MIN_DIST 0.1	//distancia mínima para considerar dos puntos iguales

/*
 *	obtiene el centro del voxel a partir del índice
 */
float3 getVoxel (private int index)
{
	float3 img_size, vox_index, delta, voxel0;
	//definir delta, voxel0 y tamaño de la imagen
	delta = (float3) (VOXEL_SIZE_X, VOXEL_SIZE_Y, VOXEL_SIZE_Z);
	voxel0 = (float3) (VOXEL_REF_X, VOXEL_REF_Y, VOXEL_REF_Z);
	img_size = (float3) (VOXEL_INDEX_X_MAX, VOXEL_INDEX_Y_MAX, VOXEL_INDEX_Z_MAX);
	//calcular índice del voxel
	float x_index = trunc(index / img_size.x);
	vox_index.x = fmod(index, img_size.x);
	vox_index.y = fmod(x_index, img_size.y );
	vox_index.z = trunc( x_index / img_size.y );
	//calcular la posición
	return mad(vox_index, delta, voxel0); 
}

/*
 *	Obtiene la intersección de una recta (definida por origin y direction)
 *	y un plano perpendicular a Z (definido por pPlane)
 */
float3 getInterLinePlaneZ(private float pPlane, private float3 origin, private float3 direction){
	float3 inter;
	inter.x = mad (pPlane - origin.z, direction.x/direction.z, origin.x);
	inter.y = mad (pPlane - origin.z, direction.y/direction.z, origin.y);
	inter.z = pPlane;
	return inter;
}

/*
 *	Obtiene la intersección de una recta (definida por origin y direction)
 *	y un plano perpendicular a Y (definido por pPlane)
 */
float3 getInterLinePlaneY(private float pPlane,private float3 origin, private float3 direction){
	float3 inter;
	inter.x = mad (pPlane - origin.y, direction.x/direction.y, origin.x);
	inter.z = mad (pPlane - origin.y, direction.z/direction.y, origin.z);
	inter.y = pPlane;
	return inter;
}

/*
 *	Obtiene la intersección de una recta (definida por origin y direction)
 *	y un plano perpendicular a X (definido por pPlane)
 */
float3 getInterLinePlaneX(private float pPlane, private float3 origin, private float3 direction){
	float3 inter;
	inter.z = mad (pPlane - origin.x, direction.z/direction.x, origin.z);
	inter.y = mad (pPlane - origin.x, direction.y/direction.x, origin.y);
	inter.x = pPlane;
	return inter;
}

/*
 *	Retorna 1 si un punto (point) está dentro de un voxel (ubicado en
 *	voxelPosition). Retorna 0 en caso contrario.
 */
int insideVoxel(private float3 voxelPosition, private float3 point){
	float3 delta = (float3) (VOXEL_SIZE_X, VOXEL_SIZE_Y, VOXEL_SIZE_Z);
	float3 dist = fabs(voxelPosition - point);
	delta += (float3) (MIN_DIST, MIN_DIST, MIN_DIST);
	if ((dist.x <= (delta.x / 2)) && (dist.y <= (delta.y / 2)) && (dist.z <= (delta.z / 2))){
		return 1;
	} else {
		return 0;
	}
}

/*
 *	Determina el tamaño del segmento que atravieza un voxel.
 * 	index: es el índice del voxel
 * 	origin: es el origen desde donse de mide la línea que atravieza el voxel (un cristal del LOR)
 * 	direction: es la dirección, a partir del origen, que define el LOR
 */
float getInterLineVoxel(private int index, private float3 origin, private float3 direction){
	float3 voxelPosition = getVoxel(index);
	float3 delta = (float3) (VOXEL_SIZE_X, VOXEL_SIZE_Y, VOXEL_SIZE_Z);
	float3 intersec[6], aux[2];
	float dist=0;
	int valid[6], number_of_intersections = 0, i;
	/*	se calculan las intersecciones del LOR con los planos del voxel	*/
	if ( direction.x != 0 ){ /* si el LOR atravieza el plano ZY */
		intersec[0] = getInterLinePlaneX(voxelPosition.x + (delta.x / 2), origin, direction);
		intersec[1] = getInterLinePlaneX(voxelPosition.x - (delta.x / 2), origin, direction);
		/*	las intersecciones se consideran válidas si están dentro del voxel	*/
		valid[0] = insideVoxel(voxelPosition, intersec[0]);
		valid[1] = insideVoxel(voxelPosition, intersec[1]);
	} else {	/*	si el LOR no atravieza el plano ZY, las intersecciones son inválidas	*/
		valid[0] = 0;
		valid[1] = 0;
	}
	if ( direction.y != 0 ){ /*	si el LOR atravieza el plano ZX	*/
		intersec[2] = getInterLinePlaneY(voxelPosition.y + (delta.y / 2), origin, direction);
		intersec[3] = getInterLinePlaneY(voxelPosition.y - (delta.y / 2), origin, direction);
		valid[2] = insideVoxel(voxelPosition, intersec[2]);
		valid[3] = insideVoxel(voxelPosition, intersec[3]);
	} else {
		valid[2] = 0;
		valid[3] = 0;
	}
	if ( direction.z != 0 ){ /*	si el LOR atravieza el plano XY	*/
		intersec[4] = getInterLinePlaneZ(voxelPosition.z + (delta.z / 2), origin, direction);
		intersec[5] = getInterLinePlaneZ(voxelPosition.z - (delta.z / 2), origin, direction);
		valid[4] = insideVoxel(voxelPosition, intersec[4]);
		valid[5] = insideVoxel(voxelPosition, intersec[5]);
	} else {
		valid[4] = 0;
		valid[5] = 0;
	}
	/*	se buscan dos intersecciones distintas, dentro del voxel,
	 *  para calcular su distancia.
	 * 	si no hay, se retorna cero.
	 */
	for (i=0; i<6; i++){
		if(	valid[i] == 1 ){
			aux[ number_of_intersections ] = intersec[i];
			number_of_intersections++;
			if ( number_of_intersections == 2 ) {
				dist = distance(aux[0],aux[1]);
				if( dist < MIN_DIST ){
					number_of_intersections = 1;
				} else {
					return dist;
				}
			}
		}
	}
	return 0;
}

/*
 *
 */
__kernel void getSegment (__global float * crystal0, __global float * crystal1,
						__global float * segments){
	int index = get_global_id(0);	/* obtener el índice del voxel */
	float3 origin, destiny, direction;
	float segment_size;
	/* convertir los datos
	 * obtener el cristal de origin y destino, para definir el LOR
	 */
	origin = (float3) (crystal0[0], crystal0[1], crystal0[2]);
	destiny = (float3) (crystal1[0], crystal1[1], crystal1[2]);
	direction = destiny - origin;
	/* calcular el segmento que atravieza el voxel */
	segment_size = getInterLineVoxel(index, origin, direction);
	/* almacenarlo en el arreglo de segmentos */
	segments[ index ] = segment_size;
 }
