#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/primitivas.h"




int main() {
    Transfer transfer("/ColaA");  
  

    bool on = true;
    while (on) {
        int x = 1;
        std::cout << "0 para salir"<<endl;
        std::cin >> x;
        if (x == 0) {
            on = false;
        } else {
            transfer.recibirMensaje();
            mensaje msg1;
            // Enviar un mensaje
            bool ok=transfer.pet("Mensaje desde Proceso1",1,2,1);
            if (ok==0){
                std::cout<<"Hubo problemas para enviar el mensaje";
            }

            sleep(2); // Esperar 2 segundos antes de la próxima iteración
        }
    }
    transfer.cerrarConexion();
    transfer.desvincular();
    std::cout << "Se ha cerrado el proceso CLIENTE"<<endl;


    return 0;
}