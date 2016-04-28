#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listas.h"

Lista *nueva_lista(void) {
	Lista *lp;
	lp = (Lista *) malloc(sizeof(Lista));
	lp->ini = lp->fin = NULL;
	return lp;
}
int lista_vacia(Lista *lp) {
	return lp->ini == NULL;
}
 
int buscarIdUltimo(Lista *lp) {
	Nodo *np = lp->ini;
	int c = 1;
	if (lp->ini == NULL)
		return 0;
	for (np = lp->ini; np; np = np->prox){
		if(np->prox ==NULL)
			return np->id;
	}
	return 0;
}

int agregar_final(Lista *lp, int elemento) {
	Nodo *np;
	int idNuevo;
	np = (Nodo *) malloc(sizeof(Nodo));
	np->tamano = elemento;
	idNuevo = buscarIdUltimo(lp);
	np->id = ++idNuevo;
	
	printf("El nuevo ID: %d \n",np->id);
	np->contenido = (char *)malloc(sizeof(elemento));
	np->prox = NULL;
	if (lp->ini == NULL)
		lp->ini = lp->fin = np;
		
	else {
		lp->fin->prox = np;
		lp->fin = np;
	}
	return idNuevo;
}
int eliminar_elemento(Lista *lp, int id) {
	Nodo *ap, *cp;
	for (ap = NULL, cp = lp->ini; cp; ap = cp, cp = cp->prox)
		if (id == cp->id) {
			if (ap)
				ap->prox = cp->prox;
			else
				lp->ini = cp->prox;
			if (lp->fin == cp)
				lp->fin = ap;
			free(cp);
			return 1;
		}
	return 0;
}

char *buscar_elemento(Lista *lp, int id) {
	Nodo *np = lp->ini;
	int c = 1;
	while (np) {
		if (id == np->id)
			return np->contenido;
		np = np->prox;
		c++;
	}
	return NULL;
}
 

int buscar_tamano_elemento(Lista *lp, int id) {
	Nodo *np = lp->ini;
	int c = 1;
	while (np) {
		if (id == np->id){
			return (np->tamano);
		}
		np = np->prox;
		c++;
	}
	return 0;
}
void modificar_elemento(Lista *lp, int id,char contenido[]) {
	Nodo *np = lp->ini;
	int c = 1;
	while (np) {
		if (id == np->id){
			printf("Valor actual: %s",np->contenido);
			//memccpy(np->contenido,contenido,buscar_tamano_elemento(lp,id));
			
			// ----- PRUEBA funciona con INTEGER 
			 //memcpy(np->contenido,contenido,sizeof(contenido));
			// ------
			
			// --- Prueba con tamano
			//memccpy(np->contenido,contenido,buscar_tamano_elemento(lp,id));
			// ---- 
			
			// --- Prueba con memmve
			memmove(np->contenido,contenido,buscar_tamano_elemento(lp,id));
			// ---- 
			
			
			//*np->contenido = contenido;
			//snprintf(np->contenido,np->tamano,contenido);
			//strcpy((np->contenido),contenido);
			printf("Valor luego de la modificacion %s",np->contenido);
			//(np->contenido) = contenido;
		}
		np = np->prox;
		c++;
	}
}


void imprime_lista(Lista *lp) {
	Nodo *np;
	for (np = lp->ini; np; np = np->prox)
		printf(" Tamano: %d , id: %d ,valor: %s \n", np->tamano,np->id,np->contenido);
	printf("\n");
}
void eliminar_lista(Lista *lp) {
	Nodo *tp, *np = lp->ini;
	while (np) {
		tp = np->prox;
		free(np);
		np = tp;
	}
	free(lp);
}

int modificar_semaforo_nodo(Lista *lp, int id,int operacion) {
printf(" \n ENTRE BIEN %d \n",operacion);
	Nodo *np = lp->ini;
	int c = 1;
	int error;
	while (np) {

		if (id == np->id){//Cuando consiga el segmento se detiene

			if(operacion==1){////LOCK SEM_P
				printf("\n **Bloqueado en el segmento**: %d \n",np->id);
   				if (error = pthread_mutex_lock(np->mutex))
   					return error; 
			}

			if(operacion==0){////UNTLOCK SEM_V
				printf("\n **Continuo en el segmento**: %d \n",np->id);
				if (error = pthread_mutex_unlock(np->mutex))
      				return error;
			}
		}
		np = np->prox;
		c++;
	}
printf(" \n SALI BIEN %d \n",operacion);
}



