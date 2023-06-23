#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    // argumentos de entrada: fichero_entrada fichero_salida
    if (argc != 4) {
        printf("Uso: %s fichero_entrada fichero_salida generaIPC\n", argv[0]);
        return 1;
    }

    // comprobar que el fichero de entrada existe
    FILE *fichero_entrada = fopen(argv[1], "r");
    if (fichero_entrada == NULL) {
        printf("No se pudo abrir el archivo de entrada\n");
        return 1;
    }

    // reemplazar fichero de salida si existe
    FILE *fichero_salida = fopen(argv[2], "w");

    // el tercer argumento indica si se quiere generar el ipc
    char generaIPC = atoi(argv[3]);


    // primero crea el header con los eventos que haya en el fichero de entrada
    char linea[1000];
    char *elemento;
    fgets(linea, 1000, fichero_entrada); // primera linea la ignora
    fgets(linea, 1000, fichero_entrada); // segunda linea tambien la ignora
    fprintf(fichero_salida, "time");
    int doubleP=0;
    double tiempo=0, tiempoViejo = 0;
    int i;
    while (fgets(linea, 1000, fichero_entrada) != NULL) {


        // encontrar indice del primer : y dejarlo en doubleP
        for(i=0; i<strlen(linea); i++){
            if(linea[i] == ':'){
                doubleP = i;
                break;
            }
        }
        if(linea[doubleP+1]==':') // stalled mete una linea tonta despues de instrucciones
            continue;

        // quedarse con primer elemento y cuarto elemento separado por :
        elemento = strtok(linea, ":");
        
        tiempo = atof(elemento);
        if(tiempoViejo == 0){
            tiempoViejo = tiempo;
            fprintf(fichero_salida, "%0.6f", tiempoViejo);
        }

        if(tiempo != tiempoViejo){
            break;
        }
        // obtener cuarto elemento, nombre del evento

        elemento = strtok(NULL, ":");
        
        elemento = strtok(NULL, ":");
        
        
        fprintf(fichero_salida, ":%s", elemento);        
    }

    if (generaIPC) {
        fprintf(fichero_salida, ":ipc\n");
    }else 
        fprintf(fichero_salida, "\n");


    // ahora que ya ha generado el header, vuelve a empezar a leer el fichero para quedarse con los numeros
    fseek(fichero_entrada, 0, SEEK_SET);
    fgets(linea, 1000, fichero_entrada); // primera linea la ignora
    fgets(linea, 1000, fichero_entrada); // segunda linea tambien la ignora

    tiempo=0;     tiempoViejo = 0;

    double instrucciones = 0, ciclos = 0, ipc = 0;
    int cuenta = 0;
    while (fgets(linea, 1000, fichero_entrada) != NULL) {

        // encontrar indice del primer : y dejarlo en doubleP
        for(i=0; i<strlen(linea); i++){
            if(linea[i] == ':'){
                doubleP = i;
                break;
            }
        }
        if(linea[doubleP+1]==':') // stalled mete una linea tonta despues de instrucciones
            continue;

        // quedarse con primer elemento y cuarto elemento separado por :
        elemento = strtok(linea, ":");
        tiempo = atof(elemento);
        if(tiempoViejo == 0)
            tiempoViejo = tiempo;

        if(tiempo != tiempoViejo){
            cuenta = 0;
            
            
            if (generaIPC) {
                ipc = instrucciones / ciclos;
                fprintf(fichero_salida, ":%0.4f\n", ipc);
            }else
                fprintf(fichero_salida, "\n");
            fprintf(fichero_salida, "%0.6f", tiempoViejo);
            tiempoViejo = tiempo;
        }
        // segundo elemento
        elemento = strtok(NULL, ":");        
            
        fprintf(fichero_salida, ":%s", elemento);        
        if(cuenta == 0){
            instrucciones = atof(elemento);
        }else if(cuenta == 1){
            ciclos = atof(elemento);
        }
        cuenta++;
    }

    if (generaIPC) {
        ipc = instrucciones / ciclos;
        fprintf(fichero_salida, ":%0.4f\n", ipc);
    }else
        fprintf(fichero_salida, "\n");

    fclose(fichero_entrada);
    fclose(fichero_salida);


    return 0;
}