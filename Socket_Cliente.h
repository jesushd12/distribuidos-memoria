#ifndef _SOCKET_CLIENTE_H
#define _SOCKET_CLIENTE_H

typedef struct bufferUDP{
	int primitiva;  
	int tamano;
	int idSegmento;
	char dato[100];
}BufferUDP;

int Abre_Conexion_Unix (char *Servicio);
int Abre_Conexion_Inet (char *Host_Servidor, char *Servicio);
int Dame_Direccion_Udp (char *Host, char *Servicio, struct sockaddr_in *Servidor,int *Longitud_Servidor);

#endif
