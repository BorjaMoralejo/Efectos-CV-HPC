#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

int main(int argc, char *argv[]) {
    int cantidad_elementos = 0;
    char *elemento;
    int *max;
    int iter;
    int elementP;
    double scaled;
    char hasIPC = 0;


    // Verificar argumentos de entrada
    if (argc != 3) {
        printf("Uso: %s archivo_entrada archivo_salida\n", argv[0]);
        return 1;
    }

    // Abrir archivo de entrada
    FILE *archivo_entrada = fopen(argv[1], "r");
    if (archivo_entrada == NULL) {
        printf("No se pudo abrir el archivo de entrada\n");
        return 1;
    }

    // Abrir archivo de salida
    FILE *archivo_salida = fopen(argv[2], "w");
    if (archivo_salida == NULL) {
        printf("No se pudo abrir el archivo de salida\n");
        return 1;
    }

    char linea[MAX_LINE_LENGTH];

    // contar cuantos elementos tiene separados por : el archivo de entrada y comprobar que el ultimo elemento es ipc
    fgets(linea, MAX_LINE_LENGTH, archivo_entrada);
    



    // preparando strtok para separar por :
    elemento = strtok(linea, ":");

    // usando strtok se separan los nombres de los elementos y se cuentan
    while (elemento != NULL) {
        
            // si el elemento es ipc se resta uno al numero de elementos
        if (strcmp(elemento, "ipc\n") == 0) {
            hasIPC = 1;
        }
        cantidad_elementos++;
        elemento = strtok(NULL, ":");
        
    }
    
    max = malloc (cantidad_elementos * sizeof(int));
    for (int i = 0; i < cantidad_elementos; i++) 
        max[i] = 0;
    

    while (fgets(linea, MAX_LINE_LENGTH, archivo_entrada) != NULL) {
        
        // para saber que elemento es, iter 0
        iter = 0;

        // separa linea de varios :
        elemento = strtok(linea, ":");
        while (elemento != NULL){
            

            // parsear eleemnto y si es mayor que el maximo de su posicion, reemplazar
            elementP = atoi(elemento);
            if (elementP > max[iter])
                max[iter] = elementP;
            // quedarse con maximo de cada iteracion del while
            elemento = strtok(NULL, ":");
            iter++;
        }
    }
    
    // ahora escribir en fichero de salida

    // volver a primera linea y leer header
    fseek(archivo_entrada, 0, SEEK_SET);
    fgets(linea, MAX_LINE_LENGTH, archivo_entrada);

    // escribiendo header en fichero
    fprintf(archivo_salida, "%s",linea);


    // empezar bucle de escalado
    while (fgets(linea, MAX_LINE_LENGTH, archivo_entrada) != NULL) {
        // para saber que elemento es, iter 0
        iter = 0;

        // separa linea de varios :
        elemento = strtok(linea, ":");
        while (elemento != NULL){
            if (iter == 0 || (hasIPC && iter == cantidad_elementos - 1)) {
                
                
                if(iter == cantidad_elementos - 1){
                    fprintf(archivo_salida, "%s",elemento);
                }else {
                    fprintf(archivo_salida, "%s:", elemento);
                }
                
            }
            else {
                // parsear elemento y si es mayor que el maximo de su posicion, reemplazar
                elementP = atoi(elemento);
                scaled = (((double)elementP)) / ((double)max[iter]);
                if (!hasIPC && iter == cantidad_elementos - 1)
                    fprintf(archivo_salida, "%.4f\n", scaled);
                else 
                    fprintf(archivo_salida, "%.4f:", scaled);
                
            }

            // quedarse con maximo de cada iteracion del while
            elemento = strtok(NULL, ":");
            iter++;
        }
    }

    fclose(archivo_entrada);
    fclose(archivo_salida);        


    
    return 0;
}
