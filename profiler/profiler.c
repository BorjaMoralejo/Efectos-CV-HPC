
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


double media_sum[NUM_RAPL_DOMAINS];
//long long event_media_sum[N_EVENTS];

extern struct event_count_s record;
extern char *exec_p[16];




/// @brief 
/// @param datafile_name 
/// @param n_cores 
/// @param affinity 
void profile (char *datafile_name, int n_cores, int affinity, int periodo){




	int i,c,j;
	//int core;
	int pid;
	int times = 16;
	int ret_pid =0, waitstatus = 0;

	




	if (n_cores != 1)
		total_cores = 1;



	
	// Ejecutando times veces
	for(c=0;c<times;c++)
	{

		// Creando fichero donde se van a guardar los datos
		start_record (&record, datafile_name, c);

		// Reseteando variables de control
		ret_pid = 0;
		waitstatus = 0;

		// Reseteando contadores		
		for (j=0; j<total_packages; j++)
		for (i=0; i<N_EVENTS; i++)
		{
			record.events[j*NUM_RAPL_DOMAINS+i] = 0;
			record.event_sum[j*NUM_RAPL_DOMAINS+i] =0;
		}
		
		// parte 1, ejecutar programa mirando si es multithread o no
		if (n_cores != 1)
		pid = launch_program_mult (affinity, exec_p, c);
		else 
		pid = launch_program (1, exec_p, c);

		// Inicializando contadores
		init_counters (&record);


		// Bucle de sondeo
		while (ret_pid == 0){
	
			reset_event_counters ();
			usleep (periodo);

			// comprobar que pid siga vivo
			ret_pid = waitpid (pid, &waitstatus, WNOHANG);
			if (ret_pid == 0) // si sigue vivo, se recoge los datos
			{ 
				get_counter_data (&record);
				record_data (&record);
			}
		}
		

		// El programa a ser perfilado ha terminado
		// Se paran los contadores
		stop_counter();

		// Calculando media de todas las ejecuciones para comparar coste del profiler segun la frecuencia
		// for (i=0; i< N_EVENTS; i++)
		// 	event_media_sum[i] += record.event_sum[i];

		// cerrando fichero de una sola ejecución
		end_record (&record);
	}

	
}

