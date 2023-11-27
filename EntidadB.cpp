#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/entidad.h"
#include "include/primitivas.h"

void leermensaje(mensaje &mensajealeer)
{
    std::cout << "Información: " << mensajealeer.informacion << std::endl;
    std::cout << "PAS: " << static_cast<int>(mensajealeer.pas) << std::endl;
    std::cout << "ID Destino: " << static_cast<int>(mensajealeer.id_dest) << std::endl;
    std::cout << "ID Origen: " << static_cast<int>(mensajealeer.id_orig) << std::endl;
}


int main() {
    Entidad entidadA(10);
    memocomp memo("mimemo", CREAT, RDWR, 10000);
        memo.getPointer();
     entidadA.punt_a_memo(memo);
        
    mqd_t mq1;
   
    mqd_t mq2;
    Transfer transferS("/ColaS", mq1);          // cola para recibir mensajes el Servidor
    Transfer transferEB("/ColaEB", mq2);        // cola donde recibe los mensajes la entidad B
    mensaje mensajerecibido;
    

    bool on = true;
    while (on)
    {
        int x = 1;
        std::cout << "0 para salir";
        std::cin >> x;
        if (x == 0)
        {
            on = false;
        }
        else
        {
            mensajerecibido = transferEB.recibirMensaje();
            entidadA.cargar_de_memo();
                if (mensajerecibido.id_orig == 0)
            {
                
            }
            else
            {
                
            }
            // Enviar un mensaje


            bool ok = transferS.pet("Mensaje desde EA", 1, 2, 1);
            if (ok == 0)
            {
                std::cout << "Hubo problemas para enviar el mensaje";
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