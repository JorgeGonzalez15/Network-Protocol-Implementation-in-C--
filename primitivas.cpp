//INTERFAZ, biblioteca de primitivas, aquí programo las primitivas que permitirán obtener servicios a través de la interfaz.
//Clase Transfer con funciones que representan los 4 modos de invocación: pet(), ind(), res(), con()
// El constructor lo uso para abrir colas de mensajes, habrá 4 colas de mensaje, 2 para los mensajes que llegan a las entidades y dos para los que llegan al cliente y servidor.
//Añado también en esta clase funciones que no son primitivas pero sirven para gestionar las colas de mensaje o desvincularlas por motivos de comodidad ya que esta biblioteca estará inclida en el código de todos los procesos.


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include "include/primitivas.h"


using namespace std;


const int MAX_SIZE = 7171;



Transfer::Transfer(const char* colaNombre, mqd_t mq1){
    mq=mq1;
    COLA_MENSAJES = colaNombre;
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct mensaje);  // Tamaño de la estructura

    mq = mq_open(COLA_MENSAJES, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }
    cout<<"Se ha abierto la "<<colaNombre<<endl;
}

bool Transfer::pet(const std::string& informacion, uint8_t pas, uint8_t id_dest, uint8_t id_orig){
    
    mensaje msg;
    strncpy(msg.informacion, informacion.c_str(), MAX_SIZE);
    msg.pas = pas;
    msg.id_dest = id_dest;
    msg.id_orig = id_orig;
   

     if (mq_send(mq, reinterpret_cast<const char*>(&msg), sizeof(struct mensaje), 0) == -1) {
        perror("mq_send");
        return 0;
    }
     return 1;
};

bool Transfer::ind(const std::string& informacion, uint8_t pas){
    mensaje msg;
    strncpy(msg.informacion, informacion.c_str(), MAX_SIZE);
    msg.pas = pas;
    msg.id_dest = 0;
    msg.id_orig = 1;
   

     if (mq_send(mq, reinterpret_cast<const char*>(&msg), sizeof(struct mensaje), 0) == -1) {
        perror("mq_send");
        return 0;
    }
     return 1;
};

bool Transfer::res(const std::string& informacionR, uint8_t pas){
    
    mensaje msg;
    strncpy(msg.informacion, informacionR.c_str(), MAX_SIZE);
    msg.pas = pas;
    msg.id_dest = 0;
    msg.id_orig = 1;
   

     if (mq_send(mq, reinterpret_cast<const char*>(&msg), sizeof(struct mensaje), 0) == -1) {
        perror("mq_send");
        return 0;
    }
     return 1;
};


bool Transfer::con(const std::string&informacionR, uint8_t pas){
    mensaje msg;
    strncpy(msg.informacion, informacionR.c_str(), MAX_SIZE);
    msg.pas = pas;
    msg.id_dest = 0;
    msg.id_orig = 1;
   

     if (mq_send(mq, reinterpret_cast<const char*>(&msg), sizeof(struct mensaje), 0) == -1) {
        perror("mq_send");
        return 0;
    }
     return 1;
    
};


struct mensaje Transfer::recibirMensaje() {
    struct mensaje msg;
            struct timespec timeout;
            timeout.tv_sec = 5;  // Tiempo de espera en segundos
            timeout.tv_nsec = 0;
            unsigned int prio;
            int bytes_received = mq_timedreceive(mq, reinterpret_cast<char*>(&msg), sizeof(struct mensaje), &prio, &timeout);

            if (bytes_received != -1) {
                //std::cout << "Mensaje recibido:" << std::endl;
               // std::cout << "Información: " << msg.informacion << std::endl;
                //std::cout << "PAS: " << static_cast<int>(msg.pas) << std::endl;
                //std::cout << "ID Destino: " << static_cast<int>(msg.id_dest) << std::endl;
                //std::cout << "ID Origen: " << static_cast<int>(msg.id_orig) << std::endl;
            } else if (errno == ETIMEDOUT) {
                std::cout << ".";
                std::cout.flush();
                msg.id_orig=0;
                return msg;
            } else {
                perror("mq_timedreceive");
            }
            return msg;
}



void Transfer::cerrarConexion() {
    mq_close(mq);
    
}
void Transfer::desvincular(){
    mq_unlink(COLA_MENSAJES);
}
