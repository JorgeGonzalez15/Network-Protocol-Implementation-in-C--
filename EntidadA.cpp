#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/primitivas.h"




int main() {
    mqd_t mq1;
    mqd_t mq2;
    Transfer transferC("/ColaC",mq1);  // cola para recibir mensajes el cliente
    Transfer transferEA("/ColaEA",mq2);  // cola para recibir mensajes la entidad A //primero se abren las colas de mensaje que permiten comunicarse a los procesos



    bool on = true;
    while (on) {
        int x = 1;
        std::cout << "0 para salir";
        std::cin >> x;
        if (x == 0) {
            on = false;
        } else {
            transferEA.recibirMensaje();
            mensaje msg1;
            // Enviar un mensaje
            bool ok=transferC.pet("Mensaje desde EA",1,2,1);
            if (ok==0){
                std::cout<<"Hubo problemas para enviar el mensaje";
            }

            sleep(2); // Esperar 2 segundos antes de la próxima iteración
        }
    }




    transferEA.cerrarConexion();
    transferEA.desvincular();
    transferC.cerrarConexion();
    transferC.desvincular();
    std::cout << "Se ha cerrado el proceso ENTIDAD A"<<endl; //se cierran las colas de mensajes de los procesos

    return 0;
}