

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

struct timespec tempt;





int record_data (struct event_count_s *data){
	int i,j;
	double tej;


	// primero guardar timestamp
	for (j=0; j<total_packages; j++)
	{
        clock_gettime(CLOCK_MONOTONIC, &tempt);

		tej = (tempt.tv_sec - data->start_time.tv_sec)+ (tempt.tv_nsec - data->start_time.tv_nsec)/(double)1e9;
		sprintf (str_buff, "%.4f;", tej);


		// escribiendo datos de los eventos
		for (i=0; i<N_EVENTS; i++)
		{
			if (i != N_EVENTS-1)
				sprintf (str_buff, "%lld;",data->events[j*N_EVENTS+i]);
			else 
				sprintf (str_buff, "%lld\n",data->events[j*N_EVENTS+i]);
		
			fwrite (str_buff, sizeof(char), strlen(str_buff), data->fd_out[j]);
		} 

	}//total_packages
	return 0;
}


void start_record (struct event_count_s *data, char *filename,  int iter){
	int j;

	sprintf (format_buff, "./tiempos/%s", filename);
	mkdir (format_buff, 0700);
	sprintf (format_buff, "./tiempos/%s/", filename);
	mkdir (format_buff, 0700);

			
	for (j=0; j<total_packages; j++)
	{
		sprintf (str_buff, "%srun_c%d_%d.dat",format_buff, j, iter);
		data->fd_out[j] = fopen (str_buff, "w");
		printf ("%s\n", str_buff);
	}


		
}


int end_record (struct event_count_s *data){
	int i,j;

	for (j=0; j<total_packages; j++)
	{
  	 
	  for (i=0; i<N_EVENTS; i++)
	  {
		data->events[j*NUM_RAPL_DOMAINS+i] = 0;
		data->event_sum[j*NUM_RAPL_DOMAINS+i] = 0;
	  }
	  fclose (data->fd_out[j]);
	}
	return 0;
}

int terminate_record (struct event_count_s *data){

	
	free (data->events);
	free (data->event_sum);
	
	
	return 0;
}

