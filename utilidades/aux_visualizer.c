#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024



/*
Lee el fichero principal línea a línea y extrae el tiempo de la primera columna. 
Luego, recorre los demás ficheros y extrae los datos de la columna indicada en cada línea correspondiente. 
Finalmente, se escribe cada línea en el fichero temporal con el formato "tiempo:fichero1:fichero2:...:ficheroN".

*/


int main(int argc, char *argv[]) {
    // Verificar que se hayan proporcionado al menos 3 argumentos
    if (argc < 3) {
        printf("Uso: %s columna fichero1 fichero2 ... ficheroN\n", argv[0]);
        return 1;
    }

    int columna = atoi(argv[1]);

    // Obtener la lista de ficheros
    char **ficheros = malloc((argc - 2) * sizeof(char *));
    for (int i = 0; i < argc - 2; i++) {
        ficheros[i] = argv[i + 2];
    }

    FILE *fichero_principal = NULL;
    int num_lineas_principal = 0;

    // Obtener el fichero principal con más líneas
    for (int i = 0; i < argc - 2; i++) {
        FILE *fichero = fopen(ficheros[i], "r");
        if (fichero == NULL) {
            printf("Error al abrir el fichero %s\n", ficheros[i]);
            return 1;
        }

        int num_lineas = 0;
        char linea[MAX_LINE_LENGTH];

        while (fgets(linea, MAX_LINE_LENGTH, fichero) != NULL) {
            num_lineas++;
        }

        if (num_lineas > num_lineas_principal) {
            if (fichero_principal != NULL) {
                fclose(fichero_principal);
            }
            fichero_principal = fichero;
            num_lineas_principal = num_lineas;
        } else {
            fclose(fichero);
        }
    }

    // Obtener el encabezado del fichero temporal
    char encabezado[MAX_LINE_LENGTH];
    snprintf(encabezado, MAX_LINE_LENGTH, "tiempo");
    for (int i = 0; i < argc - 2; i++) {
        strncat(encabezado, ":", MAX_LINE_LENGTH - strlen(encabezado) - 1);
        strncat(encabezado, ficheros[i], MAX_LINE_LENGTH - strlen(encabezado) - 1);
    }

    FILE *temporal = fopen("temporal.txt", "w");
    if (temporal == NULL) {
        printf("Error al crear el fichero temporal\n");
        return 1;
    }
    fprintf(temporal, "%s\n", encabezado);

    // Leer el fichero principal línea a línea
    rewind(fichero_principal);
    int j = 0;
    char linea_principal[MAX_LINE_LENGTH];
    char escrito = 0;

    while (fgets(linea_principal, MAX_LINE_LENGTH, fichero_principal) != NULL) {
        char *tiempo = strtok(linea_principal, ":");
        
        char datos_linea[MAX_LINE_LENGTH];
        snprintf(datos_linea, MAX_LINE_LENGTH, "%s", tiempo);

        // Leer los demás ficheros y extraer los datos de la columna indicada
        for (int i = 0; i < argc - 2; i++) {
            FILE *fichero = fopen(ficheros[i], "r");
            if (fichero == NULL) {
                printf("Error al abrir el fichero %s\n", ficheros[i]);
                return 1;
            }
            escrito=0;
            int k = 0;
            char linea_fichero[MAX_LINE_LENGTH];

            while (fgets(linea_fichero, MAX_LINE_LENGTH, fichero) != NULL) {
                if (k == j) {
                    char *token = strtok(linea_fichero, ":");
                    // quitarle el salto de línea si tiene
                    for (int c = 1; c < columna; c++) {
                        token = strtok(NULL, ":");
                    }
                    if (token[strlen(token) - 1] == '\n') {
                        token[strlen(token) - 1] = '\0';
                    }
                    strncat(datos_linea, ":", MAX_LINE_LENGTH - strlen(datos_linea) - 1);
                    strncat(datos_linea, token, MAX_LINE_LENGTH - strlen(datos_linea) - 1);
                    escrito=1;
                    break;
                }
                k++;
            }

            // si no ha escrito nada, escribe  : vacio
            if (!escrito) {
                strncat(datos_linea, ":", MAX_LINE_LENGTH - strlen(datos_linea) - 1);
            }

            fclose(fichero);
        }

        fprintf(temporal, "%s\n", datos_linea);
        j++;
    }

    fclose(fichero_principal);
    fclose(temporal);

    printf("Datos de la columna %d extraídos y guardados en temporal.txt\n", columna);

    // Liberar memoria
    free(ficheros);

    return 0;
}
