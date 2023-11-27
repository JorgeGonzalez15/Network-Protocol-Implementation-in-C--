//RESETEO DE TODAS LAS COLAS, MEMORIA COMPARTIDA Y SEMÄFOROS
//IMPORTANTE EJECUTAR DESPUÉS DE ACABAR CON LOS OTROS PROCESOS

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <limits>
#include <unistd.h>
#include "include/primitivas.h"
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/entidad.h"
#include "include/primitivas.h"
#include "include/semaforo.h"
#include <limits>

using namespace std;



int main() { 
    mqd_t mq1;
    mqd_t mq2;
    Transfer transferC("/ColaC",mq1);  // cola para recibir mensajes el cliente
    Transfer transferEA("/ColaEA",mq2);  // cola para recibir mensajes la entidad A
     Transfer transferS("/ColaS", mq1);   // cola para recibir mensajes el Servidor
    Transfer transferEB("/ColaEB", mq2); // cola donde recibe los mensajes la entidad B
    memocomp memo("mimemo", CREAT, RDWR, 10000); // Creamos la memoria compartida
    memo.getPointer();
    std::string semaforoNombre = "memoriacompartida"; // creo el semaforo para acceder a la memoria compartida
    semaforo sem(semaforoNombre);
    //Inicialización del cliente

    transferS.cerrarConexion();
    transferS.desvincular();
    transferEB.cerrarConexion();
    transferEB.desvincular();
    transferEA.cerrarConexion();
    transferEA.desvincular();
    transferC.cerrarConexion();
    transferC.desvincular();
    std::cout << "Se ha reseteado el servidor" << endl; // se cierran las colas de mensajes de los procesos
    sem.close();
    sem.unlink();
    memo.cerrar();
    memo.unlink();
}