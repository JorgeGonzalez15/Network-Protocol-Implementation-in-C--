
#include <stdio.h>
#include <stdlib.h> //exit()
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> //close()
#include <string>
#include <iostream>

#ifndef MEMOCOMP
#define MEMOCOMP
#define OPEN 0
#define CREAT 1
#define RDONLY 0 // para lectura solo
#define RDWR 2   // para lectura y escritura

class memocomp
{
    int fd;          // descriptor fichero memo
    void *puntero;   // puntero a memo compartida
    char nombre[30]; // nombre del fichero
public:
    // Constructor para crear o abrir
    memocomp(std::string strname, int crear_o_abrir, int lectura_o_escritura, int msg_len);
    void *getPointer(); // Devuelve el puntero a la zona de memocompartida
    int cerrar();
    int unlink();
};
#endif