/*
	estructura utilizada para representar un vector o un punto en el espacio carteciano tridimensional.
*/
typedef struct VECTOR{
	float x;
	float y;
	float z;
} vector;

//******************************************************************************
//recibe tres valores float y retorna un vector con ellos
vector *new_vector(float x0, float y0, float z0);

//******************************************************************************
//recibe un vector y retorna su módulo o tamaño
float module (vector *entrada);

//******************************************************************************
//recibe dos vectores y retorna un vector diferencia entre ambos
vector *diff (vector *v0, vector *v1);

//******************************************************************************
//recibe dos vectores y retorna un vector suma entre ambos
vector *sum (vector *v0, vector *v1);

//******************************************************************************
//recibe dos vectores y retorna un vector que consiste en una multiplicación
//entre sus componentes 
vector *mult (vector *v0, vector *v1);

//******************************************************************************
//recibe dos voxeles y determina si son iguales o no.
int equal (vector *v0, vector *v1);

//******************************************************************************
//multiplica un escalar por un vector
vector * mult_esc (float esc, vector *v1);
