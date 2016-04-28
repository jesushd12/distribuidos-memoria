#ifndef _Cliente_H
#define _Cliente_H
 
char SERVIDOR_1[] = "localhost";
char SERVIDOR_2[] = "192.168.1.108";
char SERVICIO[] = "cpp_java";
 
int primitivaCreate = 1;
int  primitivaUpdate = 2;
int  primitivaGet  = 3;
int  primitivaDelete  = 4;

void primitiva(int Socket_Con_Servidor, int primitiva);
int create(char *servidor, int tamanoSegmento);
int update(char *servidor, int idSegmento, void *dato,int tamanoDato);
void *get(char *servidor, int idSegmento);
int delete(char *servidor, int idSegmento);

int createUDP(char *servidor, int tamanoSegmento);

#endif

