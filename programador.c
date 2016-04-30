#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Cliente.h"



typedef struct
  {
      char nombre[20];
	  char ci[15];
	  char edad[15];
	  char genero[2];
      char ip[20];
	  char servicio[10];
      int id_referencia;
  } variable;
  


char SERVIDOR_1[] = "localhost";
char SERVIDOR_2[] = "192.168.1.108";
char SERVIDOR_LINO[] = "192.168.43.190";
char SERVIDOR_UNO[] = "192.168.43.190";
char SERVICIO[] = "cpp_java"; //Para TCP


	    
main () 
{ 


	variable pruebaStruct2;
	strcpy(pruebaStruct2.nombre, "Pedro"); 
	strcpy(pruebaStruct2.ci, "20.503.792."); 
	strcpy(pruebaStruct2.edad, "21"); 
	strcpy(pruebaStruct2.genero, "m"); 

	variable pruebaStruct;
	strcpy(pruebaStruct.nombre, "Maria"); 
	strcpy(pruebaStruct.ci, "15.503.792."); 
	strcpy(pruebaStruct.edad, "66"); 
	strcpy(pruebaStruct.genero, "f"); 

	int pruebaInt1 = 82;
	int pruebaInt2 = 80; 
	int pruebaInt3 = 75;   
	




	// ------- Prueba int-------------------------------------------------------
	//int idSegmentoInt = create(SERVIDOR_1,sizeof(int));
	//int result2 = update(SERVIDOR_1,3,&pruebaInt2,sizeof(int));
	//printf(" \n El valor de MI VARIABLE pruebaInt2: %d \n",(*(int *)get(SERVIDOR_1,3)));
	// ----------------------------------------------------------------
	// ------------------Prueba char----------------------------------
	//char mensaje[] = "Esta es una prueba doskjhjkjjjjjjuuuu";
	//int idSegmento = create(SERVIDOR_1,sizeof(mensaje));
	//int result = update(SERVIDOR_1,idSegmento,&mensaje,sizeof(mensaje));
	//printf("\nEl valor de MI VARIABLE Mensaje: %s \n",((char *)get(SERVIDOR_1,idSegmento)));
	// ---------------- -------------------------------------------------------------
	
	
	//- -----Ejercicio - Proyecto Cliente UNO - LINO
	/*
	int idSegmentoStruct = create(SERVIDOR_1,sizeof(variable));
	int result = update(SERVIDOR_1,idSegmentoStruct,&pruebaStruct,sizeof(variable));
	printf("\nEl valor de MI VARIABLE nombre: %s",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).nombre); 
	printf("\nEl valor de MI VARIABLE edad: %s",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).edad); 
	printf("\nEl valor de MI VARIABLE ci: %s",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).ci); 
	printf("\nEl valor de MI VARIABLE genero: %s \n",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).genero); 

	*/
	
	//- ------Ejercicio - Proyecto Cliente DOS -- JESUS
	
	
	int idSegmentoStruct2 = create(SERVIDOR_1,sizeof(variable),PROTOCOLO_TCP);
	int result2 = update(SERVIDOR_1,idSegmentoStruct2,&pruebaStruct2,sizeof(variable),PROTOCOLO_TCP);
	printf("\nEl valor de MI VARIABLE nombre: %s",(*(variable *)get(SERVIDOR_1,idSegmentoStruct2,PROTOCOLO_TCP)).nombre); 
	printf("\nEl valor de MI VARIABLE edad: %s",(*(variable *)get(SERVIDOR_1,idSegmentoStruct2,PROTOCOLO_TCP)).edad); 
	printf("\nEl valor de MI VARIABLE ci: %s",(*(variable *)get(SERVIDOR_1,idSegmentoStruct2,PROTOCOLO_TCP)).ci); 
	printf("\nEl valor de MI VARIABLE genero: %s \n",(*(variable *)get(SERVIDOR_1,idSegmentoStruct2,PROTOCOLO_TCP)).genero); 

	
	/* Codigo para setear el ID de referencia en el Servidor 1
	variable pruebaStructAux;
	int idSegmentoServidor1 = 1; // Este ID es suministrado por el cliente 1
	pruebaStructAux=(*(variable *)get(SERVIDOR_LINO,idSegmentoServidor1));
	pruebaStructAux.id_referencia=idSegmentoStruct2;
	strcpy(pruebaStructAux.ip,SERVIDOR_UNO);
	strcpy(pruebaStructAux.servicio,SERVICIO);
	int result3 = update(SERVIDOR_LINO,1,&pruebaStructAux,sizeof(variable)); //SERVIDOR DOS TIENE QUE SER SERVIDOR 1

	
	printf("\nEl valor de MI VARIABLE nombre: %s",(*(variable *)get(SERVIDOR_LINO,idSegmentoServidor1)).nombre); 
	printf("\nEl valor de MI VARIABLE edad: %s",(*(variable *)get(SERVIDOR_LINO,idSegmentoServidor1)).edad); 
	printf("\nEl valor de MI VARIABLE ci: %s",(*(variable *)get(SERVIDOR_LINO,idSegmentoServidor1)).ci); 
	printf("\nEl valor de MI VARIABLE genero: %s \n",(*(variable *)get(SERVIDOR_LINO,idSegmentoServidor1)).genero); 
	*/
	
	//------------------------------------------------------------------
	
	

	// -------- Prueba con Estructuras-----------------------------------
	//printf("\nEl valor de MI VARIABLE i: %d",(*(variable *)get(SERVIDOR_1,1)).nombre); 
	//int idSegmentoStruct = create(SERVIDOR_1,sizeof(variable));
	//int result = update(SERVIDOR_1,idSegmentoStruct,&pruebaStruct,sizeof(variable));
	//printf("\nEl valor de MI VARIABLE a: %d",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).a);
	//printf("\nEl valor de MI VARIABLE b: %d",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).b);
	//printf("\nEl valor de MI VARIABLE name: %s",(*(variable *)get(SERVIDOR_1,idSegmentoStruct)).name);
	//int idSegmento2 = create(SERVIDOR_1,sizeof(int));
	//printf("El id de mi nuevo segmento es : %d",idSegmento);
	
	// -------------------------------------------------------------------
	
	
	// -- PRUEBA UDP ---------------------------------------------------------------

	//printf("tamano bufferUDP: %d",sizeof(BufferUDP));
	//int idSegmentoUDP = createUDP2(SERVIDOR_1,sizeof(BufferUDP));
	//printf("Id segmento: %d",idSegmentoUDP);
	//int idSegmentoUDP = createUDP2(SERVIDOR_1,sizeof(mensaje));
	//updateUDP(SERVIDOR_1,3,&mensaje,sizeof(mensaje));
	 //printf("\nEl valor de MI VARIABLE Mensaje: %s \n",((char *)getUDP(SERVIDOR_1,3)));
	 //int idSegmentoUDP = createUDP2(SERVIDOR_1,sizeof(int));
	 //printf("Id del segmento nuevo %d",idSegmentoUDP);
	 //updateUDP(SERVIDOR_1,6,&pruebaInt2,sizeof(int));
	  //int idSegmentoUDP = createUDP2(SERVIDOR_1,sizeof(variable));
	 //updateUDP(SERVIDOR_1,8,&pruebaStruct,sizeof(variable));
	 //int result = update(SERVIDOR_1,idSegmento,&mensaje,sizeof(mensaje));
	 //printf("\nEl valor de MI VARIABLE Mensaje: %s \n",((char *)getUDP(SERVIDOR_1,5)));
	//deleteUDP(SERVIDOR_1,2);
	// FINAL PRUEBA UDP ----- 
	
	
	
}
