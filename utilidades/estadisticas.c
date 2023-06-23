#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[]) {


    // argumentos de entrada: fichero_entrada fichero_salida
    if (argc != 3) {
        printf("Uso: %s archivo_entrada archivo_salida\n", argv[0]);
        return 1;
    }


    // abrir fichero de entrada
    FILE *archivo_entrada = fopen(argv[1], "r");
    if (archivo_entrada == NULL) {
        printf("No se pudo abrir el archivo de entrada\n");
        return 1;
    }


    int n;
    double suma;
    double media;
    double varianza;
    double desviacion_tipica;

    // calcular media y varianza de los datos, cada linea es un dato
    char linea[1000];
    double dato;
    while (fgets(linea, 1000, archivo_entrada) != NULL) {
        n++;
        dato = atof(linea);
        suma += dato;
        varianza += dato * dato;
    }
  

    // cerrar fichero de entrada
    fclose(archivo_entrada);

    // calcular media y varianza
    media = suma / n;
    varianza = (varianza / n) - (media * media);

    // calcular desviacion tipica
    desviacion_tipica = sqrt(varianza);


    // abrir fichero de salida reemplazando si hace falta
    FILE *archivo_salida = fopen(argv[2], "w");
    if (archivo_salida == NULL) {
        printf("No se pudo abrir el archivo de salida\n");
        return 1;
    }

    
    // escribir media y desviacion tipica en el fichero de salida
    fprintf(archivo_salida, "%f\n", media);
    fprintf(archivo_salida, "%f\n", desviacion_tipica);

    // cerrar fichero de salida
    fclose(archivo_salida);

    return 0;
}