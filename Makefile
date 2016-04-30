all : listas.o Socket.o Socket_Servidor.o Servidor

CPPFLAGS = -g -I.

Servidor : Servidor.c
	cc -g -I. listas.o Socket.o Socket_Servidor.o Servidor.c -lpthread -o Servidor


clean :
	rm *.o Servidor
