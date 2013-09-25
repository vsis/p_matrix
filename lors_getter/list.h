/*
 *	Esta cabezera es usada para definir una lista enlazada de cristales
 *	usada en la iteración doble, necesaria para generar los pares de cristales
 */

//Nodo de la lista enlzada
typedef struct NODE{
	crystal *element;	//el cristal que contiene el nodo
	struct NODE *next;	//el siguiente elemento de la lista
}node;

//Definición de la lista enlazada
typedef struct LIST{
	node *head;	//el primer nodo de la lista
	node *tail;	//el último elemento de la lista
	node *i, *j;	//puntero i y j necesarios para la dobre iteración
	int size;	//tamaño de la lista
}list;


//******************************************************************************
//crear un nuevo nodo que contenga un cristal apuntado a new_element
node * new_node(crystal *new_element);

//******************************************************************************
//crear una nueva lista
list * new_list();

//******************************************************************************
//agregar un cristal a una lista enlazada
void add_element (crystal *new_elementm, list *linked_list);

//******************************************************************************
//el siguiente elemento i de la lista (para la primera iteración)
crystal * get_next_i_element(list *linked_list);

//******************************************************************************
//el siguiente elemento j de la lista (para la segunda iteración)
crystal * get_next_j_element(list *linked_list);

//******************************************************************************
//hacer que el puntero j apunte al mismo lugar que i
void set_ij(list *linked_list);
