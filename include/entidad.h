
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include "memocomp.h"

using namespace std;
#ifndef ENTIDAD_H
#define ENTIDAD_H

struct PDU{
    public:
    uint8_t patron;
    uint16_t destino;
    uint8_t origen;
    uint16_t longitud;
    uint8_t tipo;
    char datos[1025];

    

};
class Entidad {
    
public:
    Entidad(uint8_t x=1); 
    void recibirpdu(vector<PDU> pdu); //recibir una PDU desfragmentarla, comprobarla y pasarsela
    void enviarpdu(PDU pdu);
    void punt_a_memo(memocomp memo);
    void cargar_a_memo(PDU pdu_lado);
    void cargar_de_memo();


    
    std::vector<PDU> crearPDU(int tipo, std::string informacion, uint8_t pas, uint8_t idorigen, uint8_t iddestino);

private:
    static const uint8_t e_patron=170;
    std::vector<uint8_t> e_iddestinos;
    std::string e_informacion;
    uint8_t e_pas;
    uint8_t e_idorigen;
    uint8_t e_iddestino;
    PDU e_pdu;
    bool e_mode; //modo de memoria 1-mem compartida 0 socket
    bool e_fragment; //entra en modo fragmentación, tanto para recibir cómo para crear PDUS
    PDU *e_puntero = NULL;
    
};
#endif