#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/primitivas.h"




int main() {
    Transfer transferC("/ColaC");  // cola para recibir mensajes el cliente
    Transfer transferEA("/ColaEA");  // cola para recibir mensajes la entidad A

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
    std::cout << "Se ha cerrado el proceso ENTIDAD A"<<endl;

    return 0;
}