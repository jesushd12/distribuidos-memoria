#ifndef _SOCKET_H
#define _SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
 
typedef struct bufferUDP{
	int primitiva; 
	int tamano; 
	int idSegmento;
	char dato[100];
}BufferUDP;

typedef struct requestUDP{ 
	socklen_t fromlen;
	struct sockaddr_in from;
	unsigned char buf[sizeof(BufferUDP)];
	int socketDescriptor; 
} RequestUDP;




int Lee_Socket (int fd, char *Datos, int Longitud);
int Escribe_Socket (int fd, char *Datos, int Longitud);
//int Create(char *servidor, long longitud_byte);
int readCreate(int descriptor,Lista *lista);
void writeIdSegmentoNuevo(int descriptor_cliente, int idSegmento);
int readUpdate(int descriptor_cliente,Lista *lista);
void readGet(int descriptor_cliente, Lista *lista);
void readDelete(int descriptor_cliente, Lista *lista);
//int update(void *dato);
int readCreateUDP(void *req, Lista *lista,int tamano);
int readUpdateUDP(void *req, Lista *lista ,unsigned char datos[],int tamano,int id);
void readGetUDP(void *req, Lista *lista, int id);
void readDeleteUDP(void *req, Lista *lista, int idSegmento);

#endif
 
