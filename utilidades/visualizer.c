#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1000

int main(int argc, char *argv[]) {
    char script[4096];
    if (argc != 4) {
        printf("Uso: %s datos_a_leer titulo subtitulo\n", argv[0]);
        return 1;
    }

    // Nombre del archivo de datos desde argumentos de entrada
    char *archivo_datos  = argv[1];
    char *titulo        = argv[2];
    char *subtitulo     = argv[3];
    char sample[1000];




    char linea[MAX_LINE_LENGTH];
    char *elemento;
    int cantidad_elementos = -1; // -1 porque tiempo no se cuenta
    FILE *archivo_entrada = fopen(argv[1], "r");
    if (archivo_entrada == NULL) {
        printf("No se pudo abrir el archivo de entrada\n");
        return 1;
    }

    // contar cuantos elementos tiene separados por : el archivo de entrada y comprobar que el ultimo elemento es ipc
    fgets(linea, MAX_LINE_LENGTH, archivo_entrada);

    // preparando strtok para separar por :
    elemento = strtok(linea, ":");

    // usando strtok se separan los nombres de los elementos y se cuentan
    while (elemento != NULL) {        
        cantidad_elementos++;
        elemento = strtok(NULL, ":");
    }

    int n = cantidad_elementos;
    // Script de GNU Plot para generar la gráfica
    char* script_plot = "set datafile separator \":\"\n"
                        "set decimalsign \".\"\n"
                        "set key autotitle columnhead\n"
                        // colocar la escala del eje x de 0 a 2
                        //"set yrange [0:3]\n"
                        //"set xrange [0:]\n"
                        "set xlabel \"Tiempo\"\n"
                        
                        "set format x \"%.1f\"\n";

    sprintf(sample, "set title \"%s\"\n", argv[2]);
    sprintf(script, "%s%s", script_plot, sample);


    sprintf(sample, "set label \"%s\" at screen 0.5, 0.92 font \"Arial,8\"\n", argv[3]);
    sprintf(script, "%s%s", script, sample);

                        

    // Número de valores variables (n)
    
    sprintf(sample, "plot for [i=2:%d+1] \"%s\" using 1:i with lines\n", n,archivo_datos);
    sprintf(script, "%s%s", script, sample);
    
    printf("%s\n", script);
    // Ejecutar GNU Plot con el script
    FILE* gnuplot = popen("gnuplot -persist", "w");
    if (gnuplot == NULL) {
        printf("No se pudo iniciar GNU Plot\n");
        return 1;
    }
    fputs(script, gnuplot);
    
    pclose(gnuplot);

    

    return 0;
}
