/*
* Javier Abellan, 20 Jun 2000
*       
* Programa Servidor de socket INET, como ejemplo de utilizacion de las
* funciones de sockets. 
*/                    
   
#include <Socket_Servidor.h>
#include <listas.h>   
#include <Socket.h> 
#include <string.h> 
#include <stdio.h>   
#include <stdlib.h>    
#include <pthread.h>    
                           
Lista *lista;      
const char protocoloUDP[] = "udp";
const char protocoloTCP[] = "tcp";

void *tcp_handler(void *socket_desc)
{           
	char Cadena[1024];
	int Socket_Cliente = *(int*)socket_desc;
	int resultado;
	printf("CONEXION");
	int funcion = readPrimitiva(Socket_Cliente);
	 
	 


	switch(funcion){
		case 1:
			resultado = readCreate(Socket_Cliente,lista);
			writeIdSegmentoNuevo(Socket_Cliente,resultado);
			lista->fin->mutex= (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(lista->fin->mutex,NULL);
		 	break;
		case 2:
			readUpdate(Socket_Cliente,lista);
			break;
		case 3:  
			readGet(Socket_Cliente,lista);
			break;
		case 4:
			readDelete(Socket_Cliente,lista);
			break; 
	}
	printf("Lista de segmentos: ");
	imprime_lista(lista);
	close (Socket_Cliente);
return 0;
}     
   
void *udp_handler(void *socket_desc){
	RequestUDP *request = (RequestUDP*)socket_desc;
	//printf("Hola");
	BufferUDP *bufferDeDatos = ((BufferUDP *)request->buf) ;
	//printf("Buffer: %s",request->buf );
	char *ptr;
	int val;
    int ret; 
	char bufferNumero[100];
	//ret = strtol(bufferDeDatos->primitiva, &ptr, 10);
	printf("\nPrimitiva: %d",(*(BufferUDP *)request->buf).primitiva);
	printf("Tamano: %d\n",(*(BufferUDP *)request->buf).tamano);
	 printf("id: %d\n",(*(BufferUDP *)request->buf).idSegmento);
	 printf("dato: %s\n",(*(BufferUDP *)request->buf).dato);
	//printf("Primitiva buffer: %d",bufferDeDatos->primitiva);
	

	switch((*(BufferUDP *)request->buf).primitiva){
		case 1:
			 readCreateUDP(request,lista,bufferDeDatos->tamano);
			lista->fin->mutex= (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(lista->fin->mutex,NULL);
			break;
		case 2:
			readUpdateUDP(request,lista,(*(BufferUDP *)request->buf).dato,(*(BufferUDP *)request->buf).tamano,(*(BufferUDP *)request->buf).idSegmento);
			break;
		case 3:
			readGetUDP(request,lista,(*(BufferUDP *)request->buf).idSegmento);
			break;
		case 4:
			readDeleteUDP(request,lista,(*(BufferUDP *)request->buf).idSegmento);
			break;
	}
	printf("Lista de segmentos: ");
	imprime_lista(lista);
	
}
  
int main (int argc, char *argv[])
{ 
	 
	/*
	* Descriptores de socket servidor y de socket con el cliente
	*/
	int Socket_Servidor, n;
	int Socket_Cliente;
	char Cadena[100];
	pthread_t thread_id;
	lista = nueva_lista();
      
   
    
    if(strcasecmp(protocoloTCP,argv[1]) == 0)
     {
		printf("Ha iniciado el servidor TCP");
		/*
		* Se abre el socket servidor, con el servicio "cpp_java" dado de
		* alta en /etc/services.
		*/
		Socket_Servidor = Abre_Socket_Inet ("cpp_java");
		if (Socket_Servidor == -1)
		{
			printf ("No se puede abrir socket servidor\n");
			exit (-1);
		}
		    
		 
		while(Socket_Cliente = Acepta_Conexion_Cliente (Socket_Servidor))
			{
				
				printf ("Conexion aceptada...\n");
				if( pthread_create( &thread_id , NULL , tcp_handler , (void*) &Socket_Cliente) < 0)
				{
					printf ("No se pudo crear el hilo\n");
					return 1;
				}
			
			}
			
		if (Socket_Cliente == -1)
		{
			printf ("No se puede abrir socket de cliente\n");
			exit (-1);
		}
		close (Socket_Servidor);
	 } 
	   


	if(strcasecmp(protocoloUDP,argv[1])==0)
	{ 
	struct sockaddr_in Cliente;
	socklen_t Longitud_Cliente;
		printf("Ha iniciado el servidor UDP");
		Socket_Servidor = Abre_Socket_Udp ("cpp_java2");
		if (Socket_Servidor == -1)
		{
			printf ("No se puede abrir socket servidor\n");
			exit (-1);
		}
		 /* Preparación de datos para la lectura de mensajes */
	Longitud_Cliente = sizeof (Cliente);
	char buffer[100];
		while(1)
			{

				RequestUDP *request = (RequestUDP*)malloc(sizeof(RequestUDP));
				BufferUDP * temp = malloc(sizeof(BufferUDP));
				char buffer[sizeof(BufferUDP)];
				memset(buffer,0,sizeof(BufferUDP));
				request->fromlen = sizeof(struct sockaddr_in);
				request->socketDescriptor = Socket_Servidor;
				n = recvfrom(Socket_Servidor, request->buf, sizeof(BufferUDP), 0, (struct sockaddr *)&request->from, &request->fromlen);
				//n = recvfrom(Socket_Servidor, buffer, sizeof(BufferUDP), 0, (struct sockaddr *)&request->from, &request->fromlen);
				//n = recvfrom(Socket_Servidor, temp, sizeof(*temp), 0, (struct sockaddr *)&request->from, &request->fromlen);
				//printf("Buffer recibido %s",((BufferUDP *)buffer)->dato);
				//printf("Cantidad de datos recibidos %d",n);
				//printf("primitiva Received: %d\n", temp->primitiva);
				//printf("tamano Received: %d\n", temp->tamano);
				//printf("Data Received: %s\n", temp->dato);
				
				//printf("Id Received: %d\n", temp->idSegmento);
				
				//printf("Buffer recibido %s",((BufferUDP *)&request->buf)->dato);
				//n = recvfrom(Socket_Servidor, buffer, 100, 0, (struct sockaddr *)&Cliente, &Longitud_Cliente);
				if(n<0)
					printf(" Error recvfrom");
				printf ("Conexion UDP aceptada...\n");
				if( pthread_create( &thread_id , NULL , udp_handler , (void*)request) < 0)
				{
					printf ("No se pudo crear el hilo\n");
					return 1;
				}
				free(temp);
			
			}
	}
	
	return EXIT_SUCCESS;
	
}
