
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
#include <time.h>

#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include "defines.h"
#include "aux_func.h"
#include "recorder.h"
#include "counters.h"


extern struct event_count_s record;

extern char *exec_p[16];
extern char *exec_p2[16];

extern struct event_count_s record;









void comparation (char * datafile_name, int coreA, int coreB, int periodo){

	struct timespec t_start, t_temp, t_end, tA_0, tA_1, tB_0, tB_1;
	int pid_A, pid_B;
	int watch_time = 30;
	int iterA=0, iterB=0;
	int ret_pid, status = 0;
	double tej, tinicio;
	
	char ** exec_pA, **exec_pB;
	char finished = 0;
	char B_going=0, A_going=0;
	FILE *summ;
	exec_pA = exec_p;
	exec_pB = exec_p2;




	//***************
	// Modo comparacion
	//***************
	
	printf ("ExecA = %s - %s - %s\n", exec_pA[0], exec_pA[1] , exec_pA[2]);
	printf ("ExecB = %s - %s - %s\n", exec_pB[0], exec_pB[1], exec_pB[2]);
	
	//Se crea fichero de resultados
	sprintf (format_buff, "./comp/%s", datafile_name);
	mkdir (format_buff, 0700);
	sprintf (str_buff, "%s/run.dat",format_buff);


	printf ("ESTO?%s\n",str_buff);


	record.fd_out[0] = fopen (str_buff, "w");

	sprintf (str_buff, "%s/data.dat",format_buff);

	printf ("Fichero de resumen: %s\n", str_buff);
	summ = fopen (str_buff, "w");
	if(record.fd_out == NULL) {
		printf ("no puede ser\n");
		exit (-1);
	}

	if (summ == NULL){
	 	printf("Error, no se ha conseguido crear el fichero de resumen\n");
		exit (-1);
	}

	//*******************
	// Lanzamiento
	//*******************



	// empezar a contar
	init_counters (&record);

	
	//Se tiene que lanzar durante X tiempo, definido en watch_time
	clock_gettime (CLOCK_MONOTONIC, &t_start);
	

	// Se lanzan las dos aplicaciones en diferentes cores
	// LANZANDO A
	pid_A = launch_program_mult (coreA, exec_pA, iterA);
	clock_gettime (CLOCK_MONOTONIC, &tA_0);
	
	/*
	// midiendo tiempos de lanzamiento
	tej = (tA_0.tv_sec - t_start.tv_sec )+ (tA_0.tv_nsec- t_start.tv_nsec )/(double)1e9;
	sprintf (str_buff, "%.4f;%c;%d\n", tej, 'a', iterA  );
	fwrite (str_buff, sizeof(char), strlen(str_buff), summ);
*/

	// LANZANDO B
	pid_B = launch_program_mult (coreB, exec_pB, iterB);
	clock_gettime (CLOCK_MONOTONIC, &tB_0);

	/*
	// obteniendo tiempos de lanzamiento
	tej = (tB_0.tv_sec - t_start.tv_sec )+ (tB_0.tv_nsec- t_start.tv_nsec )/(double)1e9;
	sprintf (str_buff, "%.4f;%c;%d\n", tej, 'b', iterB  );
	fwrite (str_buff, sizeof(char), strlen(str_buff), summ);
	*/


	clock_gettime (CLOCK_MONOTONIC, &t_temp);
	


	

	printf("Okay, loop principal \n");

	//En ese X tiempo se lanzan tantas veces como se puedan las dos aplicaciones
	//Se mide el tiempo de ejecucion de las dos aplicaciones, con el tiempo de inicio y de fin

	while (finished == 0)
	{
	
		//Mirar si la aplicacion esta en ejecucion o no
		ret_pid = waitpid(pid_A, &status, WNOHANG);
		if (A_going == 0 && ret_pid!=0)
		{
			// obtiene tiempo de finalizacion
			clock_gettime (CLOCK_MONOTONIC, &tA_1);
			tej = (tA_1.tv_sec - tA_0.tv_sec)+ (tA_1.tv_nsec - tA_0.tv_nsec)/(double)1e9;
			tinicio = (tA_0.tv_sec - t_start.tv_sec )+ (tA_0.tv_nsec- t_start.tv_nsec )/(double)1e9;

			// registra los datos de la ejecucion
			sprintf (str_buff, "%.4f;%c;%d;%.4f\n", tinicio, 'a', iterA, tej);
			fwrite (str_buff, sizeof(char), strlen(str_buff), summ);

			// lanza otro proceso
			pid_A = launch_program_mult (coreA, exec_pA, iterA);
			clock_gettime (CLOCK_MONOTONIC, &tA_0);
			iterA++;      	
	    }

	    ret_pid = waitpid(pid_B, &status, WNOHANG);
		if (B_going == 0 && ret_pid != 0)
	    {
	      	// Se pilla tiempo de ejec y se vuelve a lanzar
			clock_gettime (CLOCK_MONOTONIC, &tB_1);
	      	tej = (tB_1.tv_sec - tB_0.tv_sec)+ (tB_1.tv_nsec - tB_0.tv_nsec)/(double)1e9;
			tinicio = (tB_0.tv_sec - t_start.tv_sec )+ (tB_0.tv_nsec- t_start.tv_nsec )/(double)1e9;

			// registra los datos de la ejecucion
			sprintf (str_buff, "%.4f;%c;%d;%.4f\n", tinicio, 'b', iterB, tej);
			fwrite (str_buff, sizeof(char), strlen(str_buff), summ);

			// lanza otro proceso
			pid_B = launch_program_mult (coreB, exec_pB, iterB);
			clock_gettime (CLOCK_MONOTONIC, &tB_0);
			iterB++;
	    }

	 
		
		reset_event_counters ();
		usleep (periodo);

		get_counter_data (&record);
		record_data (&record);
	

		

		clock_gettime (CLOCK_MONOTONIC, &t_temp);
		if ((t_temp.tv_sec - t_start.tv_sec) >= watch_time) {
			finished = 1;
		}
	      

	}// while
	stop_counter();
	printf ("fin\n");
	
	//Una vez se pasa el tiempo, se matan los procesos
	kill (pid_A, SIGKILL);
	kill (pid_B, SIGKILL);
	  

	clock_gettime (CLOCK_MONOTONIC, &t_end);
	tej = (t_end.tv_sec - t_start.tv_sec)+ (t_end.tv_nsec - t_start.tv_nsec)/(double)1e9;

	// guardando tiempo de ejec total, tiempo lim, freq, num iter A, num iter B
	sprintf (str_buff, "tej;watchtime;periodo;iterA;affinityA;iterB;affinityB\n" );
	fwrite (str_buff, sizeof(char), strlen(str_buff), summ);

	sprintf (str_buff, "%.4f;%d;%d;%d;%d;%d;%d\n", tej, watch_time, periodo, iterA+1, coreA, iterB+1, coreB );
	fwrite (str_buff, sizeof(char), strlen(str_buff), summ);

	fclose (summ);

	end_record (&record);

	
	

}
