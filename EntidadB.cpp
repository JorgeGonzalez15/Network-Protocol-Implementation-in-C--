#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/primitivas.h"




int main() {
    Transfer transferS("/ColaS");  // cola para recibir mensajes el Servidor
    Transfer transferEB("/ColaEB"); // cola donde recibe los mensajes la entidad B

    bool on = true;
    while (on) {
        int x = 1;
        std::cout << "0 para salir";
        std::cin >> x;
        if (x == 0) {
            on = false;
        } else {
            transferEB.recibirMensaje();
            mensaje msg1;
            // Enviar un mensaje
            bool ok=transferS.pet("Mensaje desde EB",1,2,1);
            if (ok==0){
                std::cout<<"Hubo problemas para enviar el mensaje";
            }

            sleep(2); // Esperar 2 segundos antes de la próxima iteración
        }
    }
    transferS.cerrarConexion();
    transferS.desvincular();
    transferEB.cerrarConexion();
    transferEB.desvincular();
    std::cout << "Se ha cerrado el proceso ENTIDAD B"<<endl;

    return 0;
}