
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include "defines.h"
#include "aux_func.h"
#include "recorder.h"
#include "counters.h"
#include "comparation.h"
#include "profiler.h"








struct event_count_s record;


char *exec_p[16];
char *exec_p2[16];

int main(int argc, char **argv) {

	if (argc < 2)
	{
	 printf("Usage: %s <full-command-between-\"> [-c core] [-h] [-m]\n\n",argv[0]);
	 exit (-1);
	}

	int i,j;
	int periodo = 200000;
	int affinity = 15;
	int coreA=1, coreB =2;
	int n_cores = 1;
	char *datafile_name = "unnamed";
	char comparative_exec = 0;
	char *extra;


	// Check paranoid, sin esto, no se puede ejecutar
	i = check_paranoid();
	if (i!=-1)
	{
	  printf("El paranoid %d! Asignalo 0 o -1\n",i);
	  exit(-1);
	}

	// detecta estructura de la arquitectura
	detect_packages();
	

	// Mallocs para la generacion del comando
	for (i=0;i<16;i++) exec_p[i]=malloc(sizeof(char)*128);
	generate_command (argv[1], exec_p+2);

	// Inicialización de memoria para el almacenador de datos
	record.events = malloc (sizeof(long long)*N_EVENTS*total_packages);
	record.event_sum = malloc (sizeof(long long)*N_EVENTS*total_packages);
	for (j=0; j<total_packages; j++ )
	for (i=0; i<N_EVENTS; i++)
	{
	  record.events[j*N_EVENTS+i] = 0;
	  record.event_sum[j*N_EVENTS+i] =0;
	}



	printf("\n");
	// tratando argumentos
	opterr=0;
        int c;
	while ((c = getopt (argc, argv, "a:b:e:p:n:c:hmps")) != -1) {
		switch (c) {
		case 'a':
			coreA = atoi (optarg);
			affinity = coreA;
			break;
		case 'b':
			coreB = atoi (optarg);
			break;
		case 'e':
			extra = optarg;
			comparative_exec = 1;
			break;
		case 'n':
			datafile_name = optarg;
			break;
		case 'c':
			n_cores = atoi (optarg);
			break;
		case 'p':
			periodo = atoi(optarg);
			break;
		case 'h':
			printf("Usage: %s <full-command-between-\"> [-c core] [-h] [-m]\n\n",argv[0]);
			printf("\t-c core : affinity that will be used, def 1 in comp, 15 in indididual\n");
			printf("\t-h      : displays this help\n");
			printf("\t-p periodo : periodo del sondeo\n");
			exit(0);
		default:
			fprintf(stderr,"Unknown option %c\n",c);
			exit(-1);
		}
	}


	

	//***************
	// Inicio de perfilado
	//***************
	
	

	printf ("MAIN: %s\n", exec_p[0]);
	//rapl_init(&record);


	

	if (comparative_exec)
	{
		for (i=0;i<16;i++) exec_p2[i]=malloc(sizeof(char)*128);
		generate_command (extra, exec_p2+2);
		if (coreA == coreB)
		{
			fprintf (stderr, "Cores must be different\n");
			exit (-1);
		}


		comparation (datafile_name, coreA, coreB, periodo);

	}
	else
		profile (datafile_name, n_cores, affinity, periodo);

	// finalizacion de analisis
	terminate_record (&record);

	return 0;
}
