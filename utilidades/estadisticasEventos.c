#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_ELEMENTOS 30


int main(int argc, char *argv[]) {

    // argumentos de entrada: fichero_entrada
    if (argc != 2) {
        printf("Uso: %s archivo_entrada\n", argv[0]);
        return 1;
    }

    
    // abrir fichero de entrada
    FILE *archivo_entrada = fopen(argv[1], "r");
    if (archivo_entrada == NULL) {
        printf("No se pudo abrir el archivo de entrada\n");
        return 1;
    }


    // contar cuantos elementos separados por : tiene el archivo de entrada y comprobar que el ultimo elemento es ipc
    int cantidad_elementos = 0;
    char linea[1000];
    double dato;
    char *elemento;
    // leer primera linea
    fgets(linea, 1000, archivo_entrada);

    // separar linea por :
    elemento = strtok(linea, ":");
    
    // usando strtok se separan los nombres de los elementos y se cuentan
    while (elemento != NULL) {
        cantidad_elementos++;
        elemento = strtok(NULL, ":");            
    }

    
    // inicializar variables buffer que van a contener la suma, media, varianza y desviacion tipica
    // como esta vez no sabemos cuantos elementos hay, usamos un array dinamico con cantidad_elementos elementos
    int cuenta = 0;
    double suma[MAX_ELEMENTOS];
    double media[MAX_ELEMENTOS];
    double varianza[MAX_ELEMENTOS];
    double desviacion_tipica[MAX_ELEMENTOS];
    int cuenta2 = 0;

    for (int i=0;i< cantidad_elementos;i++){
        suma[i] = 0;
        media[i] = 0;
        varianza[i] = 0;
        desviacion_tipica[i] = 0;
    }

    
    // calcular media y varianza de los datos, los elementos vienen separados por :
    while (fgets(linea, 1000, archivo_entrada) != NULL) {
        
        cuenta2=0;
        // separar linea por :
        elemento = strtok(linea, ":");
        
        // usando strtok se separan los nombres de los elementos y se cuentan
        while (elemento != NULL) {
            dato = strtod(elemento, NULL);
            suma[cuenta2] += dato;
            varianza[cuenta2] += dato * dato;
            cuenta2++;
            elemento = strtok(NULL, ":");       
           
        }
        cuenta++;
    }

    // calcular media
    for (int i = 0; i < cantidad_elementos; i++) {
        media[i] = suma[i] / cuenta;
    }

    for (int i = 0; i < cantidad_elementos; i++) {
     varianza[i] = (varianza[i]/cuenta) - (media[i] * media[i]);
     desviacion_tipica[i] = sqrt(varianza[i]);
    }
    
    // imprimir resultados
    for (int i = 0; i < cantidad_elementos; i++) {
        if (i == cantidad_elementos - 1) {
            printf("%lf\n", media[i] );
            break;
        }else printf("%lf:", media[i] );
    }   

    // imprimir desviacion tipica
    for (int i = 0; i < cantidad_elementos; i++) {
        if (i == cantidad_elementos - 1) {
            printf("%lf\n", desviacion_tipica[i] );
            break;
        }else printf("%lf:", desviacion_tipica[i] );
    }


    // cerrar fichero de entrada
    fclose(archivo_entrada);
    


}