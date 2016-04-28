/*
* Javier Abellan, 20 Jun 2000
*
* Funciones de lectura y escritura en sockets
*/
#include <sys/types.h>
#include <listas.h>
#include <sys/socket.h>
#include <Socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


#define MENSAJE  "Crear segmento memoria";

/*
* Lee datos del socket. Supone que se le pasa un buffer con hueco 
*	suficiente para los datos. Devuelve el numero de bytes leidos o
* 0 si se cierra fichero o -1 si hay error.
*/ 
int Lee_Socket (int fd, char *Datos, int Longitud)
{
	int Leido = 0;
	int Aux = 0;

	/*
	* Comprobacion de que los parametros de entrada son correctos
	*/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	/*
	* Mientras no hayamos leido todos los datos solicitados
	*/
	while (Leido < Longitud)
	{
		Aux = read (fd, Datos + Leido, Longitud - Leido);
		if (Aux > 0)
		{
			/* 
			* Si hemos conseguido leer datos, incrementamos la variable
			* que contiene los datos leidos hasta el momento
			*/
			Leido = Leido + Aux;
		}
		else
		{
			/*
			* Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
			* los caracteres leidos hasta ese momento
			*/
			if (Aux == 0) 
				return Leido;
			if (Aux == -1)
			{
				/*
				* En caso de error, la variable errno nos indica el tipo
				* de error. 
				* El error EINTR se produce si ha habido alguna
				* interrupcion del sistema antes de leer ningun dato. No
				* es un error realmente.
				* El error EGAIN significa que el socket no esta disponible
				* de momento, que lo intentemos dentro de un rato.
				* Ambos errores se tratan con una espera de 100 microsegundos
				* y se vuelve a intentar.
				* El resto de los posibles errores provocan que salgamos de 
				* la funcion con error.
				*/
				switch (errno)
				{
					case EINTR:
					case EAGAIN:
						usleep (100);
						break;
					default:
						return -1;
				}
			}
		}
	}

	/*
	* Se devuelve el total de los caracteres leidos
	*/
	return Leido;
}

/*
* Escribe dato en el socket cliente. Devuelve numero de bytes escritos,
* o -1 si hay error.
*/
int Escribe_Socket (int fd, char *Datos, int Longitud)
{
	int Escrito = 0;
	int Aux = 0;

	/*
	* Comprobacion de los parametros de entrada
	*/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	/*
	* Bucle hasta que hayamos escrito todos los caracteres que nos han
	* indicado.
	*/
	while (Escrito < Longitud)
	{
		Aux = write (fd, Datos + Escrito, Longitud - Escrito);
		if (Aux > 0)
		{
			/*
			* Si hemos conseguido escribir caracteres, se actualiza la
			* variable Escrito
			*/
			Escrito = Escrito + Aux;
		}
		else
		{
			/*
			* Si se ha cerrado el socket, devolvemos el numero de caracteres
			* leidos.
			* Si ha habido error, devolvemos -1
			*/
			if (Aux == 0)
				return Escrito;
			else
				return -1;
		}
	}

	/*
	* Devolvemos el total de caracteres leidos
	*/
	return Escrito;
}

/*
int create(char *direccion, long bytes , int protocolo){
	char *buffer = NULL;
	int descriptor;
	
	if(protocolo ==0) {
	
		descriptor = abrir_conexion_tcp(servidor);
		
		//Avisar que funcion llamo el usuario
		memset(buffer,0,2);
		memcpy(buffer,"1",2);
		write(descriptor,buffer,2);
		
		
		memset(buffer,bytes,sizeof(bytes));
		memcpy(buffer,buffer,sizeof(buffer));
		write(descriptor,buffer,sizeof(buffer));
		memset(buffer,0,sizeof(int));
		read(descriptor,buffer,sizeof(int));
		close(descriptor);
		
	}
	if(protocolo==1){
		abrir_conexion_tcp(servidor){
		descriptor = abrir_conexion_udp(servidor);
		}
	}
}

int master(int descriptor_cliente){
val = read(descriptor_cliente, buffer, 2);
if(buffer==1)
	readCreate(int descriptor_cliente);
	

}

*/
 

typedef struct
  { 
      int a;
      int b;
      char name[30]; 
  } variable;
  
int readUpdate(int descriptor_cliente, Lista *lista){
	char buffer[100];
	int val;
	int *leidos = 0;
	int ret;  
	char *ptr;  
	int id;
	int respuesta;
	
	
	// -- Recibo el ID del segmento que el cliente quiere modificar
	memset(buffer,0,100);
	val = read(descriptor_cliente, buffer, 100);
	id = strtol(buffer, &ptr, 10);
	printf("id del segmento recibido %s",buffer);
	
	
	
	// --   Recibo el tamano del dato que el cliente quiere almacenar
	memset(buffer,0,100);   
	//printf("LEYENDOTamano");     
	val = read(descriptor_cliente, buffer, 100);
	//printf("Buffer recibido con tamano %s",buffer);
	ret = strtol(buffer, &ptr, 10);    
	//printf("Tamano del BUFFER que me enviara: %d",ret);
	
	         
	
	// -- Debo verificar que el tamano del dato que el cliente quiere almacenar sea del mismo tamano del cual esta destinado el segmento
	int tamanoSegmento = buscar_tamano_elemento(lista,id);
	//printf("TAMANO DEL SEGMENTO RESERVADO %d",tamanoSegmento);
	if(tamanoSegmento == 0) 
		respuesta = 0;  
	else if(tamanoSegmento!=ret)
		respuesta = -1;
	else if(tamanoSegmento==ret)
		respuesta = 1;  
	memset(buffer,0,100);
	snprintf(buffer, 10, "%d", respuesta);
	write(descriptor_cliente,buffer,100);
	
	if(tamanoSegmento<=0) 
		return -1;
	//-- Recibo  el dato que el cliente me envio 
	unsigned char buffer2[ret];
	memset(buffer2,0,ret);
	val = read(descriptor_cliente, buffer2, ret);
	modificar_semaforo_nodo(lista,id,1);//SOLICITA PERMISO DE ACCESO AL MUTEX
	modificar_elemento(lista,id,buffer2);
	modificar_semaforo_nodo(lista,id,0);//SUELTA EL MUTEX
	//-- Envio de vuelta el dato que el cliente me envio ( DEBUG )
	//printf("BUFFER Servidor: a %d",(*(variable *)buffer2).a);
	//printf("BUFFER: Servidor name %s",(*(variable *)buffer2).name);
	write(descriptor_cliente,buffer2,ret);
	//Si todo salio bien retorno 1
	return 1;
	
} 
int readCreate(int descriptor_cliente, Lista *lista){
	char buffer[100];
	int val;
	int *leidos = 0;
	memset(buffer,0,100);
	void *numerobytes=(void *)malloc(sizeof(int));
	void * mensajesocket =(void *)malloc(sizeof(int));
	int *entero =0;
	char *ptr;
    long ret;
	val = read(descriptor_cliente, buffer, 100);
	ret = strtol(buffer, &ptr, 10);
	//printf("The number(unsigned long integer) is %ld\n", ret);
	//printf("\n recibido %s \n ",buffer);
	int idSegmentoNuevo = agregar_final(lista,ret);
	return idSegmentoNuevo;
	 
}


int readCreateUDP(void *req, Lista *lista,int tamano){
	RequestUDP *request = (RequestUDP *)req;
	socklen_t si_len = sizeof(struct sockaddr);
	int sock = request->socketDescriptor;
	char buffer[100]; 
	memset(buffer,0,100);
	int val;
	char *ptr;
    int ret;
    //Recibo el tamano del segmento
    //val = recvfrom(sock, buffer, 100, 0, (const struct sockaddr *)&request->from, &request->fromlen);
	//ret = strtol(buffer, &ptr, 10);
	//printf("The number(unsigned long integer) is %ld\n", ret);
	//printf("\n recibido %s \n ",buffer);
	int idSegmentoNuevo = agregar_final(lista,tamano);
	// Envio el ID del nuevo segmento
	memset(buffer,0,100);
	snprintf(buffer, 10,"%d",idSegmentoNuevo);
    val = sendto(request->socketDescriptor, buffer, 100, 0, (const struct sockaddr *)&request->from, si_len);
	return idSegmentoNuevo;
	
}

int readUpdateUDP(void *req, Lista *lista ,unsigned char datos[],int tamano,int id){
	RequestUDP *request = (RequestUDP *)req;
	// -- Debo verificar que el tamano del dato que el cliente quiere almacenar sea del mismo tamano del cual esta destinado el segmento
	int tamanoSegmento = buscar_tamano_elemento(lista,id);
	//printf("TAMANO DEL SEGMENTO RESERVADO %d",tamanoSegmento);
	int respuesta;
	if(tamanoSegmento == 0) 
		respuesta = 0;  
	else if(tamanoSegmento!=tamano)
		respuesta = -1;
	else if(tamanoSegmento==tamano)
		respuesta = 1; 
	modificar_semaforo_nodo(lista,id,1);//SOLICITA PERMISO DE ACCESO AL MUTEX
	modificar_elemento(lista,id,datos);
	modificar_semaforo_nodo(lista,id,0);//SUELTA EL MUTEX
	return respuesta;
}

void readGetUDP(void *req, Lista *lista, int id){
	RequestUDP *request = (RequestUDP *)req;
	//socklen_t si_len = sizeof(struct sockaddr_in);
	
	socklen_t si_len = sizeof(struct sockaddr);
	int tamanoSegmento,val;
	char bufferNumero[100];
	memset(bufferNumero,0,100);
	tamanoSegmento = buscar_tamano_elemento(lista,id);
	//printf("Tam :%d",tamanoSegmento);
	unsigned char buffer2[tamanoSegmento];
	memset(buffer2,0,tamanoSegmento);
	snprintf(bufferNumero,10,"%d",tamanoSegmento);
	val = sendto(request->socketDescriptor, bufferNumero, 100, 0, (const struct sockaddr *)&request->from, si_len);
	if(tamanoSegmento ==0){
		printf("No pude enviar el dato");
		return;  
	} 
	memset(buffer2,0,tamanoSegmento);
	memcpy(buffer2,buscar_elemento(lista,id),tamanoSegmento);
	val = sendto(request->socketDescriptor, buffer2, tamanoSegmento, 0, (const struct sockaddr *)&request->from, si_len);
}

void readDeleteUDP(void *req, Lista *lista, int idSegmento){
	RequestUDP *request = (RequestUDP *)req;
	char buffer[100];
	int ret,id,resultado,val; 
	char *ptr;
	modificar_semaforo_nodo(lista,id,1);//SOLICITA PERMISO DE ACCESO AL MUTEX
	resultado = eliminar_elemento(lista,idSegmento);
	modificar_semaforo_nodo(lista,id,0);//SUELTA EL MUTEX
	// Envio el resultado de eliminar
	memset(buffer,0,100);
	snprintf(buffer, 10, "%d", resultado);
	val = sendto(request->socketDescriptor, buffer, 100, 0, (const struct sockaddr *)&request->from, &request->fromlen);
}
  
    
void readGet(int descriptor_cliente, Lista *lista){
	char buffer[100];
	int ret,id,tamanoSegmento,val; 
	char *ptr;
	//Recibo el ID del segmento que el cliente quiere modificar
	memset(buffer,0,100);
	val = read(descriptor_cliente, buffer, 100);
	id = strtol(buffer, &ptr, 10);
	// Envio el tamano del segmento pedido
	memset(buffer,0,100);
	tamanoSegmento = buscar_tamano_elemento(lista,id);
	snprintf(buffer, 10, "%d", tamanoSegmento);
	write(descriptor_cliente,buffer,100);
	if(tamanoSegmento ==0){
		printf("No pude enviar el dato");
		return;  
	} 
	// Envio el dato
	unsigned char buffer2[tamanoSegmento];
	memset(buffer2,0,tamanoSegmento);
	memcpy(buffer2,buscar_elemento(lista,id),tamanoSegmento);
	//strcpy(buffer2, buscar_elemento(lista,id));
	//buffer2 = buscar_elemento(lista,id);
	write(descriptor_cliente,buffer2,tamanoSegmento);
	//printf("ENVIE EL DATO con a: %d",(*(variable *)buffer2).a);
	//printf("ENVIE EL DATO con a: %d",(*(variable *)buffer2).b);
	//printf("ENVIE EL DATO CON: name %s",(*(variable *)buffer2).name);
	 
}
  
void readDelete(int descriptor_cliente, Lista *lista){
	char buffer[100];
	int ret,id,resultado,val; 
	char *ptr;
	//Recibo el ID del segmento que el cliente quiere modificar
	memset(buffer,0,100);
	val = read(descriptor_cliente, buffer, 100);
	id = strtol(buffer, &ptr, 10);
	modificar_semaforo_nodo(lista,id,1);//SOLICITA PERMISO DE ACCESO AL MUTEX
	resultado = eliminar_elemento(lista,id);
	modificar_semaforo_nodo(lista,id,0);//SUELTA EL MUTEX
	// Envio el resultado de eliminar
	memset(buffer,0,100);
	snprintf(buffer, 10, "%d", resultado);
	write(descriptor_cliente,buffer,100);
	
}
void writeIdSegmentoNuevo(int descriptor_cliente, int idSegmento){
	char *buffer[100];
	memset(buffer,0,100);
	snprintf(buffer, 10, "%d", idSegmento);
	write(descriptor_cliente,buffer,100);
}

void writeRespuestaUpdate(int descriptor_cliente, int respuesta){
	char *buffer[100];
	memset(buffer,0,100);
	snprintf(buffer, 10, "%d", respuesta);
	write(descriptor_cliente,buffer,100);
}

int bufferAEntero(char buffer[]){
	char *ptr;
    int ret;
    ret = strtol(buffer, &ptr, 10);
    return ret;
}

// Funciones para que avisarle al servidor que es lo que va hacer
int readPrimitiva(int descriptor_cliente){
	char buffer[100];
	memset(buffer,0,100);
	char *ptr;
    int ret;
    int val;
	val = read(descriptor_cliente, buffer, 100);
	ret = strtol(buffer, &ptr, 10);
	return ret;
}
