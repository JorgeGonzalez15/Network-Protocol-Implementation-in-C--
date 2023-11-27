#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <mqueue.h>
#include <limits>
#include <unistd.h>
#include "include/primitivas.h"

using namespace std;

const uint8_t ID_origen=10;
void leermensaje(mensaje &mensajealeer){
std::cout << "Información: " << mensajealeer.informacion << std::endl;
                std::cout << "PAS: " << static_cast<int>(mensajealeer.pas) << std::endl;
                std::cout << "ID Destino: " << static_cast<int>(mensajealeer.id_dest) << std::endl;
                std::cout << "ID Origen: " << static_cast<int>(mensajealeer.id_orig) << std::endl;
}

int main() { 
    mqd_t mq1;
    mqd_t mq2;
    Transfer transferC("/ColaC",mq1);  // cola para recibir mensajes el cliente
    Transfer transferEA("/ColaEA",mq2);  // cola para recibir mensajes la entidad A
    //Inicialización del cliente
    mensaje mensajerecibido;

    bool modo=0;

    std::cout<< "Está inicializando el proceso cliente, por favor indique el ID_destino y el PAS a usar, su ID es 10"<<endl;
    int x;
    int y;
    cin>>x;
     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin>>y;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    uint8_t Pas=y;
    uint8_t IDdestino=x;
    std::cout<< "Escriba 1 para el cliente de eco o 0 para el modo de transferencia de ficheros"<<endl;
    cin>>modo;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (modo==1){
        cout<<"ID DESTINO "<<static_cast<int>(IDdestino)<<endl;
        cout<<"PAS "<<static_cast<int>(Pas)<<endl;

        bool on = true;
        std::cout<< "Cliente ECO iniciado, escriba el mensaje para servicio eco a coninuacion, 0 para salir:"<<endl;
        while (on) {
            
                string texto;
                getline(cin, texto);
                if (texto!="0"){
                    bool ok=transferEA.pet(texto,Pas,IDdestino,ID_origen);

                    if (ok==0){
                        std::cout<<"Hubo problemas para enviar el mensaje";
                    }
                    else{
                        bool recibiendo=1;
                        while(recibiendo){

                            mensajerecibido=transferC.recibirMensaje();
                            if (mensajerecibido.id_orig==0){
                                sleep(2);       // Esperar 2 segundos antes de la próxima iteración y seguir a la espera
                            }
                        
                            else{
                                recibiendo=0;
                                cout<<mensajerecibido.informacion<<endl;
                            }
                        } 
                    }
                }
                else{
                on = false;
                }
            
            
        
            }
        }
    
    else{
    std::cout << "No implementado, se procede a cerrar el proceso"<<endl;    
    }
    



    transferEA.cerrarConexion();
    transferEA.desvincular();
    transferC.cerrarConexion();
    transferC.desvincular();
    std::cout << "Se ha cerrado el proceso CLIENTE"<<endl;


    return 0;
}