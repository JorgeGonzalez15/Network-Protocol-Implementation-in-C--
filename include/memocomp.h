
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <string>
#include <unistd.h> 
#include <iostream>

#ifndef MEMOCOMP
#define MEMOCOMP
#define OPEN 0
#define CREAT 1
#define RDONLY 0 
#define RDWR 2   

class memocomp
{
    int fd;          // descriptor fichero memo
    void *puntero;   // puntero a memo compartida
    char nombre[30]; // nombre del fichero

public:


    
    memocomp(std::string strname, int crear_o_abrir, int lectura_o_escritura, int msg_len); // Constructor para crear o abrir la memoria compartida

    void *getPointer(); 
    int cerrar(); //Para cerrar y desvincular 
    int unlink();
};
#endif