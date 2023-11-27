//En esta clase programo las diferentes funciones de el semáforo que usaré para acceder de forma segura a la memoria compartida

#ifndef SEMAFOROS
#define SEMAFOROS
#include <semaphore.h>
#include <string>



#define OPEN 0
#define CREAT 1
class semaforo
{
sem_t* id;
char nombre[30];
public:
semaforo(std::string strname, int val); //Dos constructores, uno para crear el semáforo y otro para abrirlo
semaforo(std::string strname); 
int down(); //BAJAR
int up(); //SUBIR
int get(); //VER valor
int close(); 
int unlink(); //Para cerrar y destruir el semáforo he decidido hacer otro .cpp para que se encargue de primero abrir todos los semáforos colas y memoria compartida usadas y luego cerrarlos.
};
#endif
