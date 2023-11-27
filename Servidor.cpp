#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/primitivas.h"
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/entidad.h"
#include "include/primitivas.h"
#include "include/semaforo.h"
#include <limits>

int main()
{
    mqd_t mq1;
    mqd_t mq2;
    Transfer transferS("/ColaS", mq1);   // cola para recibir mensajes el Servidor
    Transfer transferEB("/ColaEB", mq2); // cola donde recibe los mensajes la entidad B

    bool on = true;
    mensaje mensajerecibido;
    while (on)
    {
        bool recibiendo = 1;
        while (recibiendo)
        {
            mensajerecibido = transferS.recibirMensaje();
            if (mensajerecibido.id_orig == 0)
            {
                sleep(2); // Esperar 2 segundos antes de la próxima iteración y seguir a la espera
            }

            else
            {
                recibiendo = 0;
                cout << mensajerecibido.informacion << endl;
            }
        }// Enviar un mensaje
            cout<<mensajerecibido.informacion<<endl;
            bool ok = transferEB.con(mensajerecibido.informacion, mensajerecibido.pas);
            if (ok == 0)
            {
                std::cout << "Hubo problemas para enviar el mensaje";
            }
            sleep(2); // Esperar 2 segundos antes de la próxima iteración
            
    }

    transferS.cerrarConexion();
    transferS.desvincular();
    transferEB.cerrarConexion();
    transferEB.desvincular();
    
    std::cout << "Se ha cerrado el proceso SERVIDOR" << endl;

    return 0;
}