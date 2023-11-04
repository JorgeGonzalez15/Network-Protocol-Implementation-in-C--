// Clase1.h
#include <iostream>
#include <cstdint>
#include <vector>
#include <mqueue.h>
#include <cstring>
#include <cstdlib>
#include <string>


#ifndef PRIMITIVAS_H
#define PRIMITIVAS_H
using namespace std;


struct mensaje {
    public:
    char informacion[7171];
    uint8_t pas;
    uint8_t id_dest;
    uint8_t id_orig;
    friend class Transfer;
};



class Transfer{
const char* COLA_MENSAJES;
mqd_t mq;
public:
Transfer(const char* colaNombre, mqd_t mq1);
bool pet(const std::string& informacion, uint8_t pas, uint8_t id_dest, uint8_t id_orig);
bool ind(const std::string& informacion,uint8_t pas);
bool res(const std::string& informacionR,uint8_t pas);
bool con(const std::string& informacionR,uint8_t pas);
struct mensaje recibirMensaje();
void cerrarConexion();
void desvincular();
}

;

#endif