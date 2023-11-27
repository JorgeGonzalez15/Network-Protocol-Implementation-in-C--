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

const u_int8_t origen = 10;
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
    memocomp memo("mimemo", CREAT, RDWR, 10000); // Creamos la memoria compartida
    memo.getPointer();
    entidadA.punt_a_memo(memo);

    mqd_t mq1;
    mqd_t mq2;
    Transfer transferC("/ColaC", mq1);   // cola para recibir mensajes el cliente
    Transfer transferEA("/ColaEA", mq2); // cola para recibir mensajes la entidad A //primero se abren las colas de mensaje que permiten comunicarse a los procesos
    mensaje mensajerecibido;

    std::string semaforoNombre = "memoriacompartida"; // creo el semaforo para acceder a la memoria compartida
    semaforo sem(semaforoNombre, 0);
    entidadA.e_fragment = 0;
    bool on = true;
    while (on)
    {
        
            mensajerecibido = transferEA.recibirMensaje();

            if (mensajerecibido.id_orig == 0)
            {
                sleep(2);
            }
            else
            {
                sem.up();
                leermensaje(mensajerecibido);
                string infostr(mensajerecibido.informacion); // Lo paso a string
                vector<PDU> pduEnviar;
                pduEnviar = entidadA.crearPDU(0, infostr, mensajerecibido.pas, mensajerecibido.id_orig, mensajerecibido.id_dest);
                std::cout << "Creando pdu tipo 0" << endl;
                if (false)
                { // comprueblo si va fragmentada la pdu o no+
                    for (auto i : pduEnviar)
                    {
                        sem.down();
                        entidadA.cargar_a_memo(i);
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
                                }
                                else if (tipo == 2)
                                {
                                    entidadA.cargar_a_memo(i);
                                }
                            }
                            sem.up();
                            sleep(0.1);
                        }
                        entidadA.e_fragment = 0; // salgo del modo fragmentación
                    }
                }
                else if (true) // entidadA.e_fragment == 0
                {

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
                int tipo = 4;
                sem.down();
                PDU pdurecibir = entidadA.cargar_de_memo();
                sem.up();
                bool comprobandoaCanal = 1;
                while (comprobandoaCanal)
                {
                    sem.down();
                    PDU pdurecibir = entidadA.cargar_de_memo();
                    if (entidadA.comprobarpdu(pdurecibir, tipo) == 1)
                    {
                        uint8_t pasrec = (pdurecibir.destino >> 8) & 0xFF;
                        uint8_t destino = pdurecibir.origen; // guardo el pas y el origen
                        
                        if (tipo == 0)
                        {
                            comprobandoaCanal = 0;
                            
                            vector<PDU> pduenviar = entidadA.crearPDU(1, "", pasrec, origen, mensajerecibido.id_dest);
                            cout<<"ORIGEN "<<static_cast<int>(pduenviar[0].origen)<<" DESTINO "<< static_cast<int>(pduenviar[0].destino);
                            entidadA.cargar_a_memo(pduenviar[0]);
                            std::cout << "PDU RECIBIDA:" << pdurecibir.datos << endl;
                            std::string str = pdurecibir.datos;
                            bool ok = transferC.con(str, pasrec);
                            if (ok == 0)
                            {
                                std::cout << "Hubo problemas para enviar el mensaje";
                            }

                            sleep(2); // Esperar 2 segundos antes de la próxima iteración
                        }
                        
                    }
                    sem.up();
                }
            
        }
    }

    transferEA.cerrarConexion();
    transferEA.desvincular();
    transferC.cerrarConexion();
    transferC.desvincular();
    std::cout << "Se ha cerrado el proceso ENTIDAD A" << endl; // se cierran las colas de mensajes de los procesos
    memo.unlink();
    return 0;
}
