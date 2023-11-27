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

int main()
{
    Entidad entidadA(10);
    memocomp memo("mimemo", CREAT, RDWR, 10000);
        memo.getPointer();
        entidadA.punt_a_memo(memo);
        
    mqd_t mq1;
    mqd_t mq2;
    Transfer transferC("/ColaC", mq1);   // cola para recibir mensajes el cliente
    Transfer transferEA("/ColaEA", mq2); // cola para recibir mensajes la entidad A //primero se abren las colas de mensaje que permiten comunicarse a los procesos
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
            mensajerecibido = transferEA.recibirMensaje();

                if (mensajerecibido.id_orig == 0)
            {
                sleep(2);
            }
            else
            {
                leermensaje(mensajerecibido);
                string infostr(mensajerecibido.informacion); //Lo paso a string
                vector<PDU> pduEnviar;
                pduEnviar=entidadA.crearPDU(0, infostr, mensajerecibido.pas, mensajerecibido.id_orig, mensajerecibido.id_dest);
                entidadA.cargar_a_memo(pduEnviar[0]);
                entidadA.recibirpdu(pduEnviar);
            }
            // Enviar un mensaje


            bool ok = transferC.pet("Mensaje desde EA", 1, 2, 1);
            if (ok == 0)
            {
                std::cout << "Hubo problemas para enviar el mensaje";
            }

            sleep(2); // Esperar 2 segundos antes de la próxima iteración
        }
    }

    transferEA.cerrarConexion();
    transferEA.desvincular();
    transferC.cerrarConexion();
    transferC.desvincular();
    std::cout << "Se ha cerrado el proceso ENTIDAD A" << endl; // se cierran las colas de mensajes de los procesos

    return 0;
}
