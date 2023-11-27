#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <unistd.h>
#include "include/entidad.h"
#include "include/primitivas.h"
#include "include/semaforo.h"
#include <limits>

const u_int8_t origen = 15;
void leermensaje(mensaje &mensajealeer)
{
    std::cout << "Información: " << mensajealeer.informacion << std::endl;
    std::cout << "PAS: " << static_cast<int>(mensajealeer.pas) << std::endl;
    std::cout << "ID Destino: " << static_cast<int>(mensajealeer.id_dest) << std::endl;
    std::cout << "ID Origen: " << static_cast<int>(mensajealeer.id_orig) << std::endl;
}

int main()
{
    Entidad entidadA(15);
    memocomp memo("mimemo", CREAT, RDWR, 10000);
    memo.getPointer();
    entidadA.punt_a_memo(memo);

    mqd_t mq1;

    mqd_t mq2;
    Transfer transferS("/ColaS", mq1);   // cola para recibir mensajes el Servidor
    Transfer transferEB("/ColaEB", mq2); // cola donde recibe los mensajes la entidad B
    mensaje mensajerecibido;

    std::string semaforoNombre = "memoriacompartida"; // creo el semaforo para acceder a la memoria compartida
    semaforo sem(semaforoNombre);

    PDU pdurecibir;
    uint8_t pasRecibido;
    uint8_t destino;
    bool on = true;
    while (on)
    {
        
        
            int tipo = 4;
            sem.down();
            PDU pdurecibir = entidadA.cargar_de_memo();
            if (entidadA.comprobarpdu(pdurecibir, tipo) == 1)
            {
                 pasRecibido = (pdurecibir.destino >> 8) & 0xFF;
                 destino = pdurecibir.origen; // guardo el pas y el origen
                if (tipo == 0)
                {

                    vector<PDU> pduenviar = entidadA.crearPDU(1, "", pasRecibido, origen, destino);
                    entidadA.cargar_a_memo(pduenviar[0]);
                    std::cout << "PDU RECIBIDA:" << pdurecibir.datos << endl;
                    std::string str = pdurecibir.datos;
                    transferS.ind(str, pasRecibido);
                    bool recibiendo = 1;
                    while (recibiendo)
                    {

                        mensajerecibido = transferEB.recibirMensaje();
                        if (mensajerecibido.id_orig == 0)
                        {
                            sleep(2); // Esperar 2 segundos antes de la próxima iteración y seguir a la espera
                        }

                        else
                        {
                            recibiendo = 0;
                            cout << mensajerecibido.informacion << endl;
                            sem.up();
                            leermensaje(mensajerecibido);
                            string infostr(mensajerecibido.informacion); // Lo paso a string
                            vector<PDU> pduEnviar;
                            pduEnviar = entidadA.crearPDU(0, infostr, pasRecibido , mensajerecibido.id_orig, destino);
                            std::cout << "Creando pdu tipo 0" << endl;
                            sem.down();
                            std::cout << "cargando a memoria: " << pduEnviar[0].datos << endl;
                            entidadA.cargar_a_memo(pduEnviar[0]);
                            sem.up();

                            bool recibirACK = 1;
                            int tipo = 4;
                            
                            while (recibirACK)
                            {
                                sem.down();
                                PDU pdurecibir = entidadA.cargar_de_memo();
                                if (entidadA.comprobarpdu(pdurecibir, tipo) == 1)
                                {
                                    if (tipo == 1)
                                    {
                                        recibirACK = 0;
                                        std::cout << "Recibido ack" << endl;
                                    }
                                    else if (tipo == 2)
                                    {
                                        entidadA.cargar_a_memo(pduEnviar[0]);
                                        std::cout << "Recibido no ack retransmitiendo" << endl;
                                    }
                                    else
                                    {
                                        std::cout << "tipo:" << tipo << endl;
                                        sleep(2);
                                    }
                                }
                                sem.up();
                            }
                        }
                    }
                }
                else
                {
                    vector<PDU> pduenviar = entidadA.crearPDU(2, "", pasRecibido, origen, destino);
                    entidadA.cargar_a_memo(pduenviar[0]);
                }
            }
            sem.up();
            sleep(0.1);
        
    }

    mensajerecibido = transferEB.recibirMensaje();
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

    transferS.cerrarConexion();
    transferS.desvincular();
    transferEB.cerrarConexion();
    transferEB.desvincular();
    std::cout << "Se ha cerrado el proceso ENTIDAD B" << endl;
    memo.unlink();
    return 0;
}