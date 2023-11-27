#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> //getuid()
#include <stdlib.h> //exit()
#include <string>
#include "include/semaforo.h"


using namespace std;
semaforo::semaforo(string strname, int val)//Para CREAR
{
char name[30];
char nombreConUID[30];
strcpy(name,strname.c_str());
//para no tener conflicto con otros usarios crea nombre con el uid
uid_t uid=getuid();
sprintf(nombreConUID, "/%d",uid);
strcat(nombreConUID,name);
strcpy(nombre, nombreConUID);
id = sem_open(nombre, O_CREAT, 0644, val);//con perm 0644
}
semaforo::semaforo(string strname)//para ABRIR
{
char name[30];
char nombreConUID[30];
strcpy(name,strname.c_str());
//para no tener conflicto con otros usarios crea nombre con el uid
uid_t uid=getuid();
sprintf(nombreConUID, "/%d",uid);
strcat(nombreConUID,name);
strcpy(nombre, nombreConUID);
id = sem_open(nombre, 0);
if(id==SEM_FAILED)
{
printf("ERROR al abrir semaforo... quizas no este creado previamente\n");
exit(-1);
}
}
int semaforo::down()
{
int error=sem_wait(id);
return(error); //devuelve 0 si ok
}
int semaforo::up()
{
int error=sem_post(id);
return(error); //devuelve 0 si ok
}
int semaforo::get()
{
int val;
int error=sem_getvalue (id, &val);
if(!error) return val;
else printf("ERRoR\n");
return 0;
}
int semaforo::close()
{
int error=sem_close(id);
return(error); //devuelve 0 si ok
}
int semaforo::unlink()
{
int error=sem_unlink(nombre);;
return(error); //devuelve 0 si ok
}