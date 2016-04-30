/*
* Javier Abellan, 20 Jun 2000
*
* Programa Cliente de un socket INET, como ejemplo de utilizacion
* de las funciones de sockets
*/  
#include <stdio.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <sys/un.h>
#include <unistd.h>



int PROTOCOLO_UDP = 0;
int PROTOCOLO_TCP = 1;

char SERVIDOR_LOCAL[] = "127.0.0.1";

int primitivaCreate = 1;
int  primitivaUpdate = 2;
int  primitivaGet  = 3;
int  primitivaDelete  = 4;

typedef struct bufferUDP{
	int primitiva;  
	int tamano;
	int idSegmento;
	char dato[100];
}BufferUDP;

int PUERTO_UDP = 15558;

typedef struct
  {
	  char ip[15];
	  char servicio[15];
      int id_referencia;
  } datos;
  

  
void primitiva(int Socket_Con_Servidor, int primitiva){
	int *prueba = primitiva;
	char *buffer[100];
	memset(buffer,0,100);
	snprintf(buffer, 10, "%d", prueba);
	write(Socket_Con_Servidor,buffer,100);
}

int create(char *servidor, int tamanoSegmento, int protocolo){
	if(protocolo ==PROTOCOLO_TCP){
		int Socket_Con_Servidor;
		Socket_Con_Servidor = Abre_Conexion_Inet (servidor, "cpp_java");
		if (Socket_Con_Servidor == 1)
		{
			printf ("No puedo establecer conexion con el servidor\n");
			exit (-1);
		}
		// ---
		primitiva(Socket_Con_Servidor,primitivaCreate);
		// ---
		char *buffer[100];
		memset(buffer,0,100);
		snprintf(buffer, 10, "%d", tamanoSegmento);
		write(Socket_Con_Servidor,buffer,100);
		// ---
		memset(buffer,0,100);
		int ret,val;
		char *ptr;
		
		val = read(Socket_Con_Servidor, buffer, 100);
		ret = strtol(buffer, &ptr, 10);
		close (Socket_Con_Servidor);
		return ret;
		
	}else if(protocolo==PROTOCOLO_UDP){
		
		struct sockaddr_in si_other = { 0 };
		socklen_t si_len = sizeof(struct sockaddr_in);
		int s, i, slen=sizeof(si_other);
		if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			printf("Error");
		}  
		memset((char *) &si_other, 0, sizeof(si_other));
		si_other.sin_family = AF_INET;
		si_other.sin_port = htons(PUERTO_UDP);
		if (inet_aton(SERVIDOR_LOCAL, &si_other.sin_addr) == 0)
		{
			fprintf(stderr, "inet_aton() failed\n");
			exit(1);
		}   
	   int Aux ; 
	   char bufferNumero[100];
	   BufferUDP * temp = (BufferUDP *)malloc(sizeof(BufferUDP));
	   memset(temp, 0, sizeof(BufferUDP));
	   temp->primitiva = primitivaCreate;
	   temp->tamano = tamanoSegmento;
	   Aux = sendto(s,(BufferUDP *)temp, (sizeof(*temp)), 0, (struct sockaddr *) &si_other, slen);
	   free(temp);
	   memset(bufferNumero,0,100);
	   socklen_t fromlen;
	   struct sockaddr_storage addr;
	   fromlen = sizeof addr;
	   Aux = recvfrom (s,&bufferNumero, (1024+sizeof(bufferNumero)), 0, &addr, &fromlen);
	   int idSegmento;
	   char *ptr;
	   idSegmento = strtol(bufferNumero, &ptr, 10);
	   close(s);
	   return idSegmento;
		
	}
} 




void *getUDP(char *servidor, int idSegmento){
	struct sockaddr_in si_other = { 0 };
	socklen_t si_len = sizeof(struct sockaddr_in);
    int s, i, slen=sizeof(si_other);
    if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
		printf("Error");
    }  
    
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(15558);
     
    if (inet_aton("127.0.0.1" , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }   
   
     int Aux,tamanoSegmento;
     //Envio la solicitud
	 BufferUDP * temp = malloc(sizeof(BufferUDP));
     memset(temp, 0, sizeof(BufferUDP));
     char *ptr;
     temp->primitiva = primitivaGet;
     temp->idSegmento = idSegmento;
     Aux = sendto(s,(BufferUDP *)temp, (sizeof(*temp)), 0, (struct sockaddr *) &si_other, slen);
     //Tamano de lo que debo leer
     char bufferNumero[100];
	 memset(bufferNumero,0,100);
	 socklen_t fromlen;
	struct sockaddr_storage addr;
	fromlen = sizeof addr;
     Aux = recvfrom (s,&bufferNumero, 100, 0,  &addr, &fromlen);
     tamanoSegmento = strtol(bufferNumero, &ptr, 10);
	 if(tamanoSegmento==0){
		printf("El segmento de memoria ha sido eliminado");
		return NULL;
	}
	//-- Recibo el dato
	unsigned char buffer2[tamanoSegmento];
	memset(buffer2,0,tamanoSegmento);
	Aux = recvfrom (s,&buffer2, tamanoSegmento, 0, &addr, &fromlen);
     
     close(s);
     return buffer2;
}

int deleteUDP(char *servidor, int idSegmento){
	struct sockaddr_in si_other = { 0 };
	socklen_t si_len = sizeof(struct sockaddr_in);
    int s, i, slen=sizeof(si_other);
    if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
		printf("Error");
    }  
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(15558);
     
    if (inet_aton("127.0.0.1" , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }   
   
     int Aux;
     //Envio la solicitud
     BufferUDP * temp = malloc(sizeof(BufferUDP));
     char *ptr;
     temp->primitiva = primitivaDelete;
     temp->idSegmento = idSegmento;
     Aux = sendto(s,(BufferUDP *)temp, (1024+sizeof(*temp)), 0, (struct sockaddr *) &si_other, slen);
     char bufferNumero[100];
	 memset(bufferNumero,0,100);
     Aux = recvfrom (s,&bufferNumero, 100, 0, (struct sockaddr *) &si_other, si_len);
}


int update(char *servidor, int idSegmento, void *dato,int tamanoDato, int protocolo){
	if(protocolo == PROTOCOLO_TCP)
	{
		
		int Socket_Con_Servidor;
		char bufferNumero[100];
		memset(bufferNumero,0,100);
		char *ptr;
		int readvalue;
		int tamano =tamanoDato;
		int resultado;
		Socket_Con_Servidor = Abre_Conexion_Inet (servidor, "cpp_java");
		if (Socket_Con_Servidor == 1)
		{
			printf ("No puedo establecer conexion con el servidor\n");
			exit (-1);
		}
		//-- Aviso al servidor que funcion estoy invocando
		primitiva(Socket_Con_Servidor,primitivaUpdate);
		// -- Envio el ID del segmento que quiero modificar
		snprintf(bufferNumero, 10, "%d", idSegmento);
		write(Socket_Con_Servidor,bufferNumero,100);
		//-- Envio el tamano del dato el cual quiero almacenar
		memset(bufferNumero,0,100);
		unsigned char buffer2[tamano];
		memset(buffer2,0,tamano);
		snprintf(bufferNumero, 10, "%d", tamano);
		write(Socket_Con_Servidor,bufferNumero,100);
		// -- Recibo respuesta del servidor
		memset(bufferNumero,0,100); 
		readvalue = read(Socket_Con_Servidor, bufferNumero, 100);
		resultado = strtol(bufferNumero, &ptr, 10);
		if(resultado>0){
			//-- Envio el dato que quiero almacenar
			memcpy(buffer2,dato,tamano);
			write(Socket_Con_Servidor,buffer2,tamano);
			//-- DEBUGGING
			bzero(buffer2,tamano);
			readvalue= read(Socket_Con_Servidor,buffer2,tamano);
		}else if(resultado == 0){
			printf("El Segmento ha sido eliminado");
			}else if(resultado == -1){
				printf("El tamano del segmento no concuerda con el tamano del dato");
			}
		close (Socket_Con_Servidor);
		return resultado;
	}else if(protocolo == PROTOCOLO_UDP){
		
		struct sockaddr_in si_other = { 0 };
		socklen_t si_len = sizeof(struct sockaddr_in);
		int s, i, slen=sizeof(si_other);
		if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			printf("Error");
		}  
		
		memset((char *) &si_other, 0, sizeof(si_other));
		si_other.sin_family = AF_INET;
		si_other.sin_port = htons(15558);
		 
		if (inet_aton(SERVIDOR_LOCAL , &si_other.sin_addr) == 0)
		{
			fprintf(stderr, "inet_aton() failed\n");
			exit(1);
		}   
		int Aux;
		 BufferUDP * temp = malloc(sizeof(BufferUDP));
		 temp->primitiva = primitivaUpdate;
		 temp->tamano = tamanoDato;
		 temp->idSegmento = idSegmento;
		 memset(temp->dato,0,100);
		 memmove(temp->dato,dato,tamanoDato);
		 Aux = sendto(s,(BufferUDP *)temp, (1024+sizeof(*temp)), 0, (struct sockaddr *) &si_other, slen);
		 
		 // Debo recibir respuesta del servidor acerca del servicio pedido
		 unsigned char buffer2[tamanoDato];
		 memset(buffer2,0,tamanoDato);
		 close(s);
		
	}
}  
  
 void *get(char *servidor, int idSegmento, int protocolo){

	if(protocolo == PROTOCOLO_TCP)
	{
		int Socket_Con_Servidor,tamanoSegmento,val;
		char *ptr;
		Socket_Con_Servidor = Abre_Conexion_Inet (servidor, "cpp_java");
		if (Socket_Con_Servidor == 1)
		{
			printf ("No puedo establecer conexion con el servidor\n");
			exit (-1);
		}
		char bufferNumero[100];
		memset(bufferNumero,0,100);
		//-- Aviso al servidor que funcion estoy invocando
		primitiva(Socket_Con_Servidor,primitivaGet);
		//--Envio el id del segmento que quiero obtener
		snprintf(bufferNumero, 10, "%d", idSegmento);
		write(Socket_Con_Servidor,bufferNumero,100);
		//--Recibo el tamano del elemento que debo leer
		memset(bufferNumero,0,100);
		val = read(Socket_Con_Servidor, bufferNumero, 100);
		tamanoSegmento = strtol(bufferNumero, &ptr, 10);
		if(tamanoSegmento==0){
			printf("El segmento de memoria ha sido eliminado");
			return NULL;
		}
		//-- Recibo el dato
		unsigned char buffer2[tamanoSegmento];
		memset(buffer2,0,tamanoSegmento);
		val = read(Socket_Con_Servidor, buffer2, tamanoSegmento);
		close (Socket_Con_Servidor);
		return buffer2;
	}
	else if(protocolo==PROTOCOLO_UDP)
	{
		struct sockaddr_in si_other = { 0 };
		socklen_t si_len = sizeof(struct sockaddr_in);
		int s, i, slen=sizeof(si_other);
		if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			printf("Error");
		}  
		
		memset((char *) &si_other, 0, sizeof(si_other));
		si_other.sin_family = AF_INET;
		si_other.sin_port = htons(15558);
		 
		if (inet_aton(SERVIDOR_LOCAL, &si_other.sin_addr) == 0)
		{
			fprintf(stderr, "inet_aton() failed\n");
			exit(1);
		}   
	   
		 int Aux,tamanoSegmento;
		 //Envio la solicitud
		 BufferUDP * temp = malloc(sizeof(BufferUDP));
		 memset(temp, 0, sizeof(BufferUDP));
		 char *ptr;
		 temp->primitiva = primitivaGet;
		 temp->idSegmento = idSegmento;
		 Aux = sendto(s,(BufferUDP *)temp, (sizeof(*temp)), 0, (struct sockaddr *) &si_other, slen);
		 //Tamano de lo que debo leer
		 char bufferNumero[100];
		 memset(bufferNumero,0,100);
		 socklen_t fromlen;
		struct sockaddr_storage addr;
		fromlen = sizeof addr;
		 Aux = recvfrom (s,&bufferNumero, 100, 0,  &addr, &fromlen);
		 tamanoSegmento = strtol(bufferNumero, &ptr, 10);
		 if(tamanoSegmento==0){
			printf("El segmento de memoria ha sido eliminado");
			return NULL;
		}
		//-- Recibo el dato
		unsigned char buffer2[tamanoSegmento];
		memset(buffer2,0,tamanoSegmento);
		Aux = recvfrom (s,&buffer2, tamanoSegmento, 0, &addr, &fromlen);
		 
		 close(s);
		 return buffer2;
	}

 }   
 
 int delete(char *servidor, int idSegmento, int protocolo){
	 int resultado;
	if(protocolo ==PROTOCOLO_TCP){
		int Socket_Con_Servidor,tamanoSegmento,val;
		char *ptr;
		Socket_Con_Servidor = Abre_Conexion_Inet (servidor, "cpp_java");
		if (Socket_Con_Servidor == 1)
		{
			printf ("No puedo establecer conexion con el servidor\n");
			exit (-1);
		}
		char bufferNumero[100];
		memset(bufferNumero,0,100);
		//-- Aviso al servidor que funcion estoy invocando
		primitiva(Socket_Con_Servidor,primitivaDelete);
		//-- Envio el id del segmento que quiero eliminar
		snprintf(bufferNumero, 10, "%d", idSegmento);
		write(Socket_Con_Servidor,bufferNumero,100);
		// -- Recibo respuesta del borrado
		memset(bufferNumero,0,100);
		val = read(Socket_Con_Servidor, bufferNumero, 100);
		resultado = strtol(bufferNumero, &ptr, 10);
		close (Socket_Con_Servidor);
		return resultado; //Devuelve 1 si el borrado fue exitoso, 0 si el elemento no existia
	}else if(protocolo==PROTOCOLO_UDP){
		struct sockaddr_in si_other = { 0 };
		socklen_t si_len = sizeof(struct sockaddr_in);
		int s, i, slen=sizeof(si_other);
		if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			printf("Error");
		}  
		memset((char *) &si_other, 0, sizeof(si_other));
		si_other.sin_family = AF_INET;
		si_other.sin_port = htons(15558);
		 
		if (inet_aton(SERVIDOR_LOCAL , &si_other.sin_addr) == 0)
		{
			fprintf(stderr, "inet_aton() failed\n");
			exit(1);
		}   
	   
		 int Aux;
		 //Envio la solicitud
		 BufferUDP * temp = malloc(sizeof(BufferUDP));
		 char *ptr;
		 temp->primitiva = primitivaDelete;
		 temp->idSegmento = idSegmento;
		 Aux = sendto(s,(BufferUDP *)temp, (1024+sizeof(*temp)), 0, (struct sockaddr *) &si_other, slen);
		 char bufferNumero[100];
		 memset(bufferNumero,0,100);
		 Aux = recvfrom (s,&bufferNumero, 100, 0, (struct sockaddr *) &si_other, si_len);
		 resultado = strtol(bufferNumero, &ptr, 10);
		 free(temp);
		 return resultado;
	}
	
 }
       
//////////////////////////////// Socket_Cliente



/*
/ Conecta con un servidor remoto a traves de socket INET
*/
int Abre_Conexion_Inet (
	char *Host_Servidor, 
	char *Servicio)
{
	struct sockaddr_in Direccion;
	struct servent *Puerto;
	struct hostent *Host;
	int Descriptor;

	Puerto = getservbyname (Servicio, "tcp");
	if (Puerto == NULL)
		return -1;

	Host = gethostbyname (Host_Servidor);
	if (Host == NULL)
		return -1;

	Direccion.sin_family = AF_INET;
	Direccion.sin_addr.s_addr = ((struct in_addr *)(Host->h_addr))->s_addr;
	Direccion.sin_port = Puerto->s_port;
	
	Descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (Descriptor == -1)
		return -1;

	if (connect (
			Descriptor, 
			(struct sockaddr *)&Direccion, 
			sizeof (Direccion)) == -1)
	{
		return -1;
	}

	return Descriptor;
}

/*
 * Prepara un socket para un cliente UDP.
 * Asocia un socket a un cliente UDP en un servicio cualquiera elegido por el sistema,
 * de forma que el cliente tenga un sitio por el que enviar y recibir mensajes.
 * Devuelve el descriptor del socket que debe usar o -1 si ha habido algún error.
 */
int Abre_Conexion_Udp ()
{
	struct sockaddr_in Direccion;
	int Descriptor;

	/* Se abre el socket UDP (DataGRAM) */
	Descriptor = socket (AF_INET, SOCK_DGRAM, 0);
	if (Descriptor == -1)
	{
		return -1;
	}

	/* Se rellena la estructura de datos necesaria para hacer el bind() */
	Direccion.sin_family = AF_INET;            /* Socket inet */
	Direccion.sin_addr.s_addr = htonl(INADDR_ANY);    /* Cualquier dirección IP */
	Direccion.sin_port = htons(0);                    /* Dejamos que linux eliga el servicio */

	/* Se hace el bind() */
	if (bind (
			Descriptor, 
			(struct sockaddr *)&Direccion, 
			sizeof (Direccion)) == -1)
	{
		close (Descriptor);
		return -1;
	}

	/* Se devuelve el Descriptor */
	return Descriptor;
}

/**
 * Rellena una estructura sockaddr_in con los datos que se le pasan. Esta estrutura es
 * útil para el envio o recepción de mensajes por sockets Udp o para abrir conexiones.
 * Se le pasa el host. Puede ser NULL (para abrir socket servidor Udp o para recepción de
 * mensajes de cualquier host).
 * Se le pasa el servicio. Puede ser NULL (para abrir socket cliente Udp).
 * Se le pasa una estructura sockaddr_in que devolverá rellena.
 * Se le pasa una Longitud. Debe contener el tamaño de la estructura sockaddr_in y
 * devolverá el tamaño de la estructura una vez rellena.
 * Devuelve -1 en caso de error.
 */
int Dame_Direccion_Udp (char *Host, char *Servicio, struct sockaddr_in *Servidor,int *Longitud_Servidor)
{
   struct servent *Puerto;
   struct hostent *Maquina;
  
   /* Comprobación de parámetros */
   if (Servidor == NULL) return -1;

   /* Relleno del primer campo de la estructura */
   Servidor->sin_family = AF_INET;

   /* Si nos han pasado un host ... */
   if (Host != NULL)
   {
      /* ... obtenemos la dirección del host y la ponemos en la estructura */
      Maquina = gethostbyname (Host);
      if (Maquina == NULL)
         return -1;

      Servidor->sin_addr.s_addr = ((struct in_addr *)(Maquina->h_addr))->s_addr;
   }
   else
      /* Si no nos han pasado un host, ponemos cualquier host. */
      Servidor->sin_addr.s_addr = INADDR_ANY;

   /* Si servicio en NULL, hacemos que el puerto lo eliga el sistema operativo
    libremente.*/
   if (Servicio == NULL)
      Servidor->sin_port = 0;
   else
   {
      /* Si el servicio no es NULL, lo obtenemos. */
      Puerto = getservbyname (Servicio, "udp");
      if (Puerto == NULL)
         return -1;
      Servidor->sin_port = Puerto->s_port;
   }

   return 0;
}

