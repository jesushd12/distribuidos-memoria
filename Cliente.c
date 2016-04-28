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
#include <Socket_Cliente.h>
#include <netdb.h> 
#include <sys/un.h>
#include <unistd.h>
#include <listas.h>
//#include <Socket.h>
#include "Cliente.h"


typedef struct
  {
      int a;
      int b;
      char name[30]; 
  } variable;
  
  
void primitiva(int Socket_Con_Servidor, int primitiva){
	int *prueba = primitiva;
	char *buffer[100];
	memset(buffer,0,100);
	snprintf(buffer, 10, "%d", prueba);
	write(Socket_Con_Servidor,buffer,100);
}

int create(char *servidor, int tamanoSegmento){
	int Socket_Con_Servidor;
	Socket_Con_Servidor = Abre_Conexion_Inet (servidor, SERVICIO);
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
} 


int createUDP2(char *servidor, int tamanoSegmento){
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
   int Aux ; 
   char bufferNumero[100];
 //  char buffer[sizeof(BufferUDP)];
  // memset(buffer,0,sizeof(BufferUDP));
   BufferUDP * temp = (BufferUDP *)malloc(sizeof(BufferUDP));
   memset(temp, 0, sizeof(BufferUDP));
   //BufferUDP bufferDeDatos;
   temp->primitiva = primitivaCreate;
   temp->tamano = tamanoSegmento;
   // temp->idSegmento = 0;
    //strcpy(temp->dato,"aaaaaaaaaaaaa");
   //bufferDeDatos.primitiva = primitivaCreate;
    //bufferDeDatos.tamano = tamanoSegmento;
    //bufferDeDatos.idSegmento = 0;
    //strcpy(bufferDeDatos.dato,"aaaaaaaaaaaaa");
   //memccpy(buffer,(BufferUDP *)&bufferDeDatos,sizeof(BufferUDP));
   Aux = sendto(s,(BufferUDP *)temp, (sizeof(*temp)), 0, (struct sockaddr *) &si_other, slen);
  // printf("Estoy enviando: %s",((BufferUDP *)buffer)->dato);
free(temp);
	// -- Recibo el id del segmento  
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


int updateUDP(char *servidor, int idSegmento, void *dato,int tamanoDato){
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
     BufferUDP * temp = malloc(sizeof(BufferUDP));
     temp->primitiva = primitivaUpdate;
     temp->tamano = tamanoDato;
     temp->idSegmento = idSegmento;
     memset(temp->dato,0,100);
     memmove(temp->dato,dato,tamanoDato);
     Aux = sendto(s,(BufferUDP *)temp, (1024+sizeof(*temp)), 0, (struct sockaddr *) &si_other, slen);
     
     unsigned char buffer2[tamanoDato];
	 memset(buffer2,0,tamanoDato);
     close(s);
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
     //memset(temp->dato,0,100);
     //memmove(temp->dato,dato,tamanoDato);
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


int update(char *servidor, int idSegmento, void *dato,int tamanoDato){
	int Socket_Con_Servidor;
	char bufferNumero[100];
	memset(bufferNumero,0,100);
	char *ptr;
	int readvalue;
	int tamano =tamanoDato;
	int resultado;
	Socket_Con_Servidor = Abre_Conexion_Inet (servidor, SERVICIO);
	if (Socket_Con_Servidor == 1)
	{
		printf ("No puedo establecer conexion con el servidor\n");
		exit (-1);
	}
	//-- Aviso al servidor que funcion estoy invocando
	primitiva(Socket_Con_Servidor,primitivaUpdate);
	
	
	// -- Envio el ID del segmento que quiero modificar
	snprintf(bufferNumero, 10, "%d", idSegmento);
	printf("Tamano en string:  %s",bufferNumero);
	write(Socket_Con_Servidor,bufferNumero,100);
	
	
	//-- Envio el tamano del dato el cual quiero almacenar
	memset(bufferNumero,0,100);
	printf("Tamano estructura:  %d",tamano);
	 
	unsigned char buffer2[tamano];
	memset(buffer2,0,tamano);
	
	snprintf(bufferNumero, 10, "%d", tamano);
	printf("Tamano en string:  %s",bufferNumero);
	write(Socket_Con_Servidor,bufferNumero,100);
	
	
	// -- Recibo respuesta del servidor
	memset(bufferNumero,0,100); 
	readvalue = read(Socket_Con_Servidor, bufferNumero, 100);
	resultado = strtol(bufferNumero, &ptr, 10);
	
	
	if(resultado>0){
		//-- Envio el dato que quiero almacenar
		memcpy(buffer2,dato,tamano);
		//printf("BUFFER: a %d",(*(variable *)buffer2).a);
		//printf("BUFFER: b %d",(*(variable *)buffer2).b);
		//printf("BUFFER: name %s",(*(variable *)buffer2).name);
		//printf("BUFFER: b %d",(*(variable *)buffer2).b);
		write(Socket_Con_Servidor,buffer2,tamano);
		//-- DEBUGGING
		bzero(buffer2,tamano);
		readvalue= read(Socket_Con_Servidor,buffer2,tamano);
		//printf("Contenido readvalue: %d",readvalue);
		//printf("Mi variable a de regreso: %d",(*(variable *)buffer2).a);
	}else if(resultado == 0){
		printf("El Segmento ha sido eliminado");
		}else if(resultado == -1){
			printf("El tamano del segmento no concuerda con el tamano del dato");
		}
	close (Socket_Con_Servidor);
	return resultado;
}  
  
 void *get(char *servidor, int idSegmento){

	int Socket_Con_Servidor,tamanoSegmento,val;
	char *ptr;
	Socket_Con_Servidor = Abre_Conexion_Inet (servidor, SERVICIO);
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
 
 int delete(char *servidor, int idSegmento){
	int Socket_Con_Servidor,tamanoSegmento,val,resultado;
	char *ptr;
	Socket_Con_Servidor = Abre_Conexion_Inet (servidor, SERVICIO);
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
	return resultado; //Devuelve 1 si el borrado fue exitoso, 0 si el elemento no existia
	close (Socket_Con_Servidor);
 }
       
    
main () 
{ 
	variable pruebaStruct;
	pruebaStruct.a =82; 
	pruebaStruct.b =85;  
	strcpy(pruebaStruct.name, "Old Man"); 
	int merwebo = 82;
	int merwebo2 = 80; 
	int merwebo3 = 75;   
	



	// ------- Prueba int
	//int idSegmentoInt = create(SERVIDOR_1,sizeof(int));
	//int result2 = update(SERVIDOR_1,3,&merwebo2,sizeof(int));
	//printf(" \n El valor de MI VARIABLE Merwebo: %d \n",(*(int *)get(SERVIDOR_1,3)));
	// ------
	// ------------------Prueba char
	char mensaje[] = "Esta es una prueba dos";
	int idSegmento = create(SERVIDOR_1,sizeof(mensaje));
	//int result = update(SERVIDOR_1,2,&mensaje,sizeof(mensaje));
	//printf("\nEl valor de MI VARIABLE Mensaje: %s \n",((char *)get(SERVIDOR_1,idSegmento)));
	
	// ---------------- FIN PRUEBA CHAR
	
	//- -----Prueba estructuras
	//int idSegmentoStruct = create(SERVIDOR_1,sizeof(variable));
	//int result = update(SERVIDOR_1,idSegmentoStruct,&pruebaStruct,sizeof(variable));
	//printf("\nEl valor de MI VARIABLE a: %d",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).a);
	//printf("\nEl valor de MI VARIABLE b: %d",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).b);
	//printf("\nEl valor de MI VARIABLE name: %s",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).name);
	//int idSegmento2 = create(SERVIDOR_1,sizeof(int));
	//printf("El id de mi nuevo segmento es : %d",idSegmento);
	
	// --- FIN prueba estructuras
	
	// -- PRUEBA UDP -----
	




	//printf("tamano bufferUDP: %d",sizeof(BufferUDP));
	//int idSegmentoUDP = createUDP2(SERVIDOR_1,sizeof(BufferUDP));
	//printf("Id segmento: %d",idSegmentoUDP);
	//int idSegmentoUDP = createUDP2(SERVIDOR_1,sizeof(mensaje));
	//updateUDP(SERVIDOR_1,3,&mensaje,sizeof(mensaje));
	 //printf("\nEl valor de MI VARIABLE Mensaje: %s \n",((char *)getUDP(SERVIDOR_1,3)));
	 //int idSegmentoUDP = createUDP2(SERVIDOR_1,sizeof(int));
	 //printf("Id del segmento nuevo %d",idSegmentoUDP);
	 //updateUDP(SERVIDOR_1,6,&merwebo2,sizeof(int));
	  //int idSegmentoUDP = createUDP2(SERVIDOR_1,sizeof(variable));
	 //updateUDP(SERVIDOR_1,8,&pruebaStruct,sizeof(variable));
	 //int result = update(SERVIDOR_1,idSegmento,&mensaje,sizeof(mensaje));
	 //printf("\nEl valor de MI VARIABLE Mensaje: %s \n",((char *)getUDP(SERVIDOR_1,5)));
	//deleteUDP(SERVIDOR_1,2);
	// FINAL PRUEBA UDP ----- 
	
	
	
	//delete(SERVIDOR_1,4);
	
	//int result2 = update(SERVIDOR_1,2,&merwebo,sizeof(int));
	 
	//int result3 = update(SERVIDOR_1,1,&merwebo2,sizeof(int));
	//int result4 = update(SERVIDOR_1,3,&merwebo3,sizeof(int));
	
	//printf(" \n El valor de MI VARIABLE Merwebo: %d \n",(*(int *)get(SERVIDOR_1,2)));
	//printf("El valor de MI VARIABLE Merwebo2: %d \n",(*(int *)get(SERVIDOR_1,1)));
	
	 /*
	if(result == 1)
		printf("Modificacion exitosa %d",result);
	else if(result == 0)
		printf("El segmento ha sido eliminado");
	else if(result == -1)
		printf("Intentas almacenar un tamano mayor al reservado");
		*/  
	
	variable miVariable;
	//miVariable = *(variable *)get(SERVIDOR_1,1);
	//printf("El valor de MI VARIABLE a: %d",(*(variable *)get(SERVIDOR_1,1)).a);
	//printf("\nEl valor de MI VARIABLE b: %d \n",(*(variable *)get(SERVIDOR_1,7)).b);
	
	//printf("\nEl valor de MI VARIABLE name: %s \n",(*(variable *)get(SERVIDOR_1,7)).name);
	
		
	/*
	* Descriptor del socket y buffer para datos
	*/
	
	
	/*
	int Socket_Con_Servidor;
	char Cadena[100];

	Socket_Con_Servidor = Abre_Conexion_Inet ("localhost", "cpp_java");
	if (Socket_Con_Servidor == 1)
	{
		printf ("No puedo establecer conexion con el servidor\n");
		exit (-1);
	}



*/


/*
	strcpy (Cadena, "48");
	Escribe_Socket (Socket_Con_Servidor, Cadena, 5);
	printf ("Soy cliente, He recibido : %s\n", Cadena);
*/


//Prueba de CREATE FUNCIONA
	//int pruebaNumero = 123;
	//update(&pruebaNumero);
	
	/*
	int *prueba = 25;
	char *buffer[100];
	memset(buffer,0,100);
	snprintf(buffer, 10, "%d", prueba);
	write(Socket_Con_Servidor,buffer,100);
	
	*/
	//PRueba
	//write(Socket_Con_Servidor,prueba,sizeof(int));
	
	// ----------------------------------------------------------------
	//prueba2  - update
	//prueba2  - update
	/*
	variable pruebaStruct;
	pruebaStruct.a =20; 
	pruebaStruct.b =25; 
	strcpy(pruebaStruct.name, "Old Man");
	 
	 
	char bufferNumero[100];
	memset(bufferNumero,0,100);
	
	int tamano =sizeof(pruebaStruct);
	
	printf("Tamano estructura:  %d",tamano);
	unsigned char *buffer2[tamano];
	 
	unsigned char *buffer3[tamano];
	memset(buffer3,0,tamano);
	snprintf(bufferNumero, 10, "%d", tamano);
	
	printf("Tamano en string:  %s",bufferNumero);
	write(Socket_Con_Servidor,bufferNumero,100);
	
	memset(buffer2,0,tamano);
	memcpy(buffer2,&pruebaStruct,tamano);
	printf("BUFFER: a %d",(*(variable *)buffer2).a);
	
	printf("BUFFER: b %d",(*(variable *)buffer2).b);
	
	printf("BUFFER: name %s",(*(variable *)buffer2).name);
	
	printf("BUFFER: b %d",(*(variable *)buffer2).b);
	write(Socket_Con_Servidor,buffer2,tamano);
	bzero(buffer2,tamano);
	int readvalue;
	readvalue= read(Socket_Con_Servidor,buffer2,tamano);
	printf("Contenido readvalue: %d",readvalue);
	printf("Mi variable a de regreso: %d",(*(variable *)buffer2).a);
	//printf("Mi variable b de regreso: %s",(*(variable *)buffer2).b);
	*/
	//------------------------------------------------------------------
	
	
	/*
	close (Socket_Con_Servidor);
	*/
	
}
