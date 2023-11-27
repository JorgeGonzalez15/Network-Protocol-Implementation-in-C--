
//Esta clase la utilizaré para crear, abrir y gestionar la memoria compartida
//El constructor lo usaré para crear o abrir la memoria compartida con un uid unico para cada entidad para que no haya problemas de acceso.
//getPointer() devuelve el puntero a la memoria compartida
//cerrar() cierra la memoria compartida
//unlink() desvincula la memoria compartida
//Recalcar que el tamaño máximo de la memoria compartida será 10000 bytes para asegurarme que no haya problemas de capacidad.

#include "include/memocomp.h"
#include <stdio.h>
#include <stdlib.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <string.h>



using namespace std;


// Constructor para crear o abrir
memocomp::memocomp(string strname, int crear_o_abrir, int lectura_o_escritura, int msg_len)
{ // abrir 0, crear
    char name[30];
    char nombreConUID[30];
    strcpy(name, strname.c_str()); 
    uid_t uid = getuid();
    sprintf(nombreConUID, "/%d", uid);

    strcat(nombreConUID, name);
    strcpy(nombre, nombreConUID);


    if (crear_o_abrir == CREAT) 
    {
        if (lectura_o_escritura == RDONLY)
            fd = shm_open(nombre,
                          O_CREAT | O_RDONLY, 0644);
        else
            fd = shm_open(nombre, O_CREAT | O_RDWR, 0644);
    }
    else 
    {
        if (lectura_o_escritura == RDONLY)
            fd = shm_open(nombre, O_RDONLY,
                          0);
        else
            fd = shm_open(nombre, O_RDWR, 0);
    }
    if (fd == -1) 
    {
        cout << "Error al crear la memo" << endl;
        exit(-1);
    }
    
    if (ftruncate(fd, msg_len) == -1)
    {
        cout << "Error en ftruncate" << endl;
        exit(-1);
    }
    

    if (lectura_o_escritura == RDONLY)
        puntero = mmap(NULL, msg_len, PROT_READ, MAP_SHARED, fd, 0);
    
    else
        puntero = mmap(NULL, msg_len, PROT_READ | PROT_WRITE,
                       MAP_SHARED, fd, 0); 
    if (puntero == MAP_FAILED)
    {
        cout << "Error en vinculacion de memoria mmap" << endl;
        exit(-1);
    }
}
void *memocomp::getPointer() 
{
    return (puntero);
}
int memocomp::cerrar()
{
    int error = close(fd);
    return (error); 
}

int memocomp::unlink()
{
    int error = shm_unlink(nombre);
    return (error); 
}