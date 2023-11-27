/// Clase1.h
#include "include/entidad.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
#include "include/memocomp.h"

using namespace std;

Entidad::Entidad(uint8_t idorigeninicial)
{
    e_idorigen = idorigeninicial;
}
void Entidad::punt_a_memo(memocomp memo)
{
    e_puntero = (PDU *)memo.getPointer();
}

void Entidad::cargar_a_memo(PDU pdu_lado)
{
    *e_puntero = pdu_lado;
}
PDU Entidad::cargar_de_memo()
{
    PDU e_pdu = *e_puntero;
    return e_pdu;
}
bool Entidad::comprobarpdu( PDU pdu, int &tipo ){
    if(pdu.patron==170){
        
        uint8_t destino=(pdu.destino) & 0xFF;
        
        if(destino==e_idorigen){
            
            uint8_t mascara = 0xC0; 
            if(((pdu.tipo & mascara)>>6)==1){
                tipo=1;
                return 1;
                cout<<"Tipo1"<<endl;
            }
            else if(((pdu.tipo & mascara)>>6)==2){
                tipo=2;
                return 1;
            }
            else if(((pdu.tipo & mascara)>>6)==0){
                tipo=0;
                if(pdu.longitud==strlen(pdu.datos)){
                    return 1;
                }

            }
        }
    }
return 0;
}
std::vector<PDU> Entidad::crearPDU(int tipo, std::string informacion, uint8_t pas, uint8_t idorigen, uint8_t iddestino)
{
    std::vector<PDU> pduv(7);
    uint16_t longitud = informacion.length();
    uint16_t paspid = (static_cast<uint16_t>(pas) << 8) | iddestino;
    if (tipo == 0)
    {
        if (longitud <= 1024)
        {

            strncpy(pduv[0].datos, informacion.c_str(), sizeof(pduv[0].datos));
            pduv[0].datos[1024] = '\0';
            pduv[0].patron = e_patron;
            pduv[0].destino = paspid;
            pduv[0].origen = idorigen;
            pduv[0].longitud = longitud;
            pduv[0].tipo = 0;

            return pduv;
        }

        else if ((longitud >= 1024) && (longitud < 7168))
        {
            e_fragment = true;
            uint8_t fragmentos = 0;
            if (longitud % 1024 == 0)
            {
                fragmentos = int(longitud) / 1024;
            }
            else
            {
                fragmentos = (int(longitud) / 1024) + 1;
            }
            for (uint8_t i = 1; i <= fragmentos; i++)
            {
                if (i == fragmentos)
                {
                    strncpy(pduv[i].datos, informacion.c_str() + 1024 * (i - 1), (longitud - (fragmentos - 1) * 1024));
                    pduv[i].datos[longitud - 1024 * (i - 1)] = '\0';
                    pduv[i].longitud = (longitud - (fragmentos - 1) * 1024);
                }
                else if (i < fragmentos)
                {
                    strncpy(pduv[i].datos, informacion.c_str() + 1024 * (i - 1), 1024);
                    pduv[i].datos[1024] = '\0';
                    pduv[i].longitud = 1024;
                }

                pduv[i].patron = e_patron;
                pduv[i].destino = paspid;
                pduv[i].origen = idorigen;

                uint8_t tipoi = ((i << 3) | fragmentos);
                pduv[i].tipo = 0,         // me aseguro de que tenga el valor 0 de inicio
                    pduv[i].tipo = tipoi; // valor 00000000 (tipo 0 sin fragmentar)
            }
            return pduv;
        }
        else
        {
            pduv[0].patron = 0; // si es demasiado largo el mensaje no lo mando, al crear la pdu la devuelve la entidad con patron
                                //  0 y luego la comprueba para ver si he excedido la longitud máxima de mensaje
            return pduv;
        }
    }
    else if (tipo == 1) // para crear tipo 1 ACK
    {
        pduv[0].patron = e_patron;
        pduv[0].destino = paspid;
        pduv[0].origen = idorigen;
        pduv[0].longitud = 0;
        pduv[0].tipo = 64; // valor 01000000 (tipo 1 ACK)
    }
    else if (tipo == 2) // para crear tipo 2 NAK
    {
        pduv[0].patron = e_patron;
        pduv[0].destino = paspid;
        pduv[0].origen = idorigen;
        pduv[0].longitud = 0;
        pduv[0].tipo = 64; // valor 01000000 (tipo 1 ACK)
    }
    return pduv;
}

void Entidad::recibirpdu(vector<PDU> pdu)
{   
    cout <<"Recompongo PDU:" << endl;
    //vector<PDU> pduej = crearPDU(0, "In the heart of a bustling city, where skyscrapers touched the sky and the streets pulsed with life, there existed a small, hidden park. This green oasis, known as Serenity Park, was a sanctuary of calm and tranquility amidst the chaos of the urban jungle. It was a place where people sought refuge from the demands of their fast-paced lives.Serenity Park was not a large park by any means, but its size was decptive. The moment you entered, you felt like you'd stepped into another world. Tall trees formed a natural canopy that provided shade and a sense of seclusion. The gentle rustling of leaves and the soft chirping of birds created a soothing symphony that drowned out the city's noise.A cobblestone path wound its way through the park, meandering alongside a crystal-clear stream that gurgled as it flowed over smooth pebbles. Park benches were strategically placed along the path, inviting visitors to sit, read, or simply observe the world around them.The park was a haven for people from all walks of life. The young and old, families, couples, and individuals seeking solace all found their way to Serenity Park. Children giggled as they fed the ducks that swam in the stream, while elderly folks played chess beneath the shade of the trees. Runners and yogis sought the park's serenity to exercise, while artists captured its beauty on their canvases.At the heart of the park stood a magnificent fountain, its centerpiece a statue of a graceful muse. Water danced from the fingertips of the statue, creating an ethereal display. It was here that many found a place to reflect and gather their thoughts.Serenity Park was also known for its seasonal beauty. In spring, the cherry blossoms adorned the trees with delicate pink petals, creating a breathtaking spectacle. In summer, the park was a lush, green paradise, offering respite from the heat. Autumn brought a kaleidoscope of colors as the leaves changed, and winter transformed the park into a serene wonderland, with a light dusting of snow.As the day turned to evening, the park's atmosphere shifted. The lampposts lining the path came to life, casting a warm, inviting glow. Couples strolled hand in hand, their laughter mixing with the soft strumming of a guitar played by a street musician. The park's magic extended into the night, creating a romantic ambiance that made it a favorite spot for proposals and first dates.Serenity Park was a testament to the power of nature in an urban environment. It reminded people that, amidst the hustle and bustle of city life, there was a place where peace and serenity could be found. It served as a reminder that even in the busiest of worlds, one could always seek refuge in the quiet beauty of nature. In Serenity Park, the city's chaos faded away, and for a while, visitors could find a sense of serenity that rejuvenated their souls.", 20, 37, 42);
    for (auto i : pdu)
    {
        cout << i.datos << endl;
    }

} // recibir una PDU desfragmentarla, comprobarla y pasarsela
