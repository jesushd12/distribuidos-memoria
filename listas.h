#ifndef _LISTAS_H
#define _LISTAS_H
#include <pthread.h>
typedef struct campos{
	int id;
	int tamano;
	unsigned char *contenido;
}Campos; 

typedef struct nodo {
	int id;
	int tamano;
	unsigned char *contenido;
	pthread_mutex_t *mutex;
	struct nodo *prox;
} Nodo;

typedef struct {
	Nodo *ini, *fin;
} Lista;

Lista *nueva_lista(void);
int lista_vacia(Lista *lp);
int buscarIdUltimo(Lista *lp);
int agregar_final(Lista *lp, int elemento);
int eliminar_elemento(Lista *lp, int elemento);
char *buscar_elemento(Lista *lp, int id);
void imprime_lista(Lista *lp);
void eliminar_lista(Lista *lp);
void modificar_elemento(Lista *lp, int id,char *contenido);
int buscar_tamano_elemento(Lista *lp, int id);
int modificar_semaforo_nodo(Lista *lp, int id,int operacion);

#endif

