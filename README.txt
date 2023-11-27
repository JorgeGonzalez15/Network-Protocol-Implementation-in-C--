INFORMACIÓN PROGRAMACIÓN 

Para ejecutar el programa:

Por razones de comodidad he decidido programar en C++ debido a que creo que simplifica el código la programación orientada a objetos. También he decidido programar en una distribución de Linux, Ubuntu 22.04.3 debido a que tenía la posibilidad al tener un ordenador con este sistema operativo y tiene una interfaz de usuario más amigable a la vez que tiene las funciones a través de terminal de los sistemas Linux. 
Para ejecutar el programa he creado varios archivos .sh para que compilen y ejecuten cada proceso (hay 4 procesos, uno por cada usuario y entidad). Hay varias posibilidades para ejecutarlos:

Opción 1.- Ejecutar ./exe.sh en una terminal bash referida a la carpeta que contiene el programa si se dispone de xterm y se le da permisos de administrador al archivo. Esto compilará y ejecutará en cuatro terminales distintas cada ejecutable y creará los cuatro procesos intercomunicados. Realmente de los procesos solo se interactuará con Usuario 1, es decir cliente ya que será por el que podamos escribir los comandos. Debe saberse que si se usa esta opción después de hacer un ./reset.sh normalmente el proceso ENTIDAD B no se abrirá correctamente por lo que se cerrarán los otros 3 procesos y se volverá a ejecutar ./exe.sh


Opción 2.- Ejecutar cada archivo ejecutable .sh en varias terminales bash referidas a la carpeta por separado en este orden : 1 ./Cliente.sh 2 ./EntidadA.sh 3 ./EntidadB.sh 4  ./Servidor.sh .
Esto compilará los 4 procesos con sus archivos asociados y los ejecutará. Una vez estén todos abiertos se deberá interactuar con el cliente. 


Opción 3.- Compilar de forma manual los archivos según las indicaciones contenidas en  Cliente.sh , EntidadA.sh,  EntidadB.sh,   Servidor.sh, y ejecutar en el orden especificado en opción 2.

Una vez se cierren los procesos simplemente cerrando la terminal se deberá SIEMPRE ejecutar ./reset.sh lo que abrirá todos los mecanismos IPC creados y los desvinculará y cerrará. Es muy importante hacer esto para reiniciar las colas de mensajes, memoria compartida y semáforos.

