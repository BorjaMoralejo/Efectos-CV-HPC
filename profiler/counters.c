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

struct datos_lectura {
  uint64_t nr;
  struct {
    uint64_t value;
    uint64_t id;
  } values[];
};

struct counter_s{
  int fd_master[2];
  uint64_t ids[N_EVENTS*2];
  int event_fd[N_EVENTS*2];
};

/*
static int perf_event_open(struct perf_event_attr *hw_event_uptr,
                    pid_t pid, int cpu, int group_fd, unsigned long flags) {

        return syscall(__NR_perf_event_open,hw_event_uptr, pid, cpu,
                        group_fd, flags);
}
*/struct perf_event_attr attr;
struct counter_s contadores;
char buf[9096];



int event_config[N_EVENTS] = {PERF_COUNT_HW_CPU_CYCLES, 		//0
			      PERF_COUNT_HW_REF_CPU_CYCLES,		//7
	                      PERF_COUNT_HW_INSTRUCTIONS,		//1
			      
	                      PERF_COUNT_HW_CACHE_REFERENCES,		//2 
			      /*PERF_COUNT_HW_CACHE_L1D | 			//9
				      (PERF_COUNT_HW_CACHE_OP_PREFETCH << 8) | 
				      (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16),*/
			      PERF_COUNT_HW_CACHE_LL |			//10
			              (PERF_COUNT_HW_CACHE_OP_WRITE << 8) |
				      (PERF_COUNT_HW_CACHE_RESULT_MISS << 16),
			      //PERF_COUNT_HW_CACHE_MISSES,  		//3
			      //PERF_COUNT_HW_BUS_CYCLES,			//4
                              //PERF_COUNT_HW_STALLED_CYCLES_FRONTEND,	//5
                              //PERF_COUNT_HW_STALLED_CYCLES_BACKEND,	//6
			      PERF_COUNT_SW_PAGE_FAULTS,
				  PERF_COUNT_SW_CONTEXT_SWITCHES,

			      PERF_COUNT_SW_CPU_CLOCK,
			      PERF_COUNT_SW_TASK_CLOCK

			      //PERF_COUNT_SW_CONTEXT_SWITCHES,		//8
			     };		

int event_type[N_EVENTS] =  {PERF_TYPE_HARDWARE,			//0

			      PERF_TYPE_HARDWARE,			//1
			      PERF_TYPE_HARDWARE,			//2


			      PERF_TYPE_HARDWARE,			//3
//			      PERF_TYPE_HW_CACHE,			//9
			      PERF_TYPE_HW_CACHE,			//10

			      //PERF_TYPE_HARDWARE,			//5
			      //PERF_TYPE_HARDWARE,			//6
			      PERF_TYPE_SOFTWARE,			//8
			      PERF_TYPE_SOFTWARE,			//8
			      PERF_TYPE_SOFTWARE,			//8
			      PERF_TYPE_SOFTWARE			//8
                             };



void init_counters (struct event_count_s *suma, char comp){

  	struct perf_event_attr att;
  	int fd1,fd2;
	uint64_t id1,id2;
	int i;
	
  

	// creando contador maestro
	memset(&att, 0, sizeof(struct perf_event_attr));
	att.type = PERF_TYPE_HARDWARE;
	att.size = sizeof(struct perf_event_attr);
	att.config = PERF_COUNT_HW_CPU_CYCLES;
	att.disabled = 1;
	att.exclude_kernel = 1;
	att.exclude_hv = 1;
	att.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
	fd1 = syscall(__NR_perf_event_open, &att, -1, 0, -1, 0);
	if(fd1 == -1){
		printf("Error error!\n");
		
	}
	ioctl(fd1, PERF_EVENT_IOC_ID, &id1);

	contadores.fd_master[0] = fd1;
	contadores.ids[0] = id1;


	
	for (i = 0; i < N_EVENTS-1; i++){
		// creando contadores de eventos
		memset(&att, 0, sizeof(struct perf_event_attr));
		att.type = event_type[i+1];
		att.size = sizeof(struct perf_event_attr);
		att.config = event_config[i+1];
		att.disabled = 1;
		att.exclude_kernel = 1;
		att.exclude_hv = 1;
		att.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
		fd2 = syscall(__NR_perf_event_open, &att, -1, 0, fd1 , 0);
		if(fd2 == -1){
			printf("Error en contador %d\n", i+1);
			
		}
		ioctl(fd2, PERF_EVENT_IOC_ID, &id2);
		contadores.ids[i+1] = id2;
		contadores.event_fd[i+1] = fd2;
	}

	

	

	
}

void reset_event_counters (){
	
	

	ioctl(contadores.fd_master[0], PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
	ioctl(contadores.fd_master[0], PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

}


void stop_counter(char comp){
	int i;


	for (i = 1; i < N_EVENTS; i++)
		close (contadores.event_fd[i]);
	close (contadores.fd_master[0]);
	
}

void get_counter_data (struct event_count_s *suma){

	struct datos_lectura* rf = (struct datos_lectura*) buf;
	
	int i, j;
	char found;


	// pausando contadores
	ioctl(contadores.fd_master[0], PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

	// leyendo datos del maestro 1
	read(contadores.fd_master[0], buf, sizeof(buf));

	// leyendo datos de los contadores de eventos
	for (i = 0; i < rf->nr; i++) {
		
		found = 0;
		j = 0;
		// buscando el identificador del contador en el array de identificadores y guardando valor
		while (j < rf->nr && !found) {
			if (rf->values[i].id == contadores.ids[j]) {
				suma->events[i] = rf->values[i].value;;
				suma->event_sum[i] +=  rf->values[i].value;;
			//printf("XD %ld %ld\n", rf->values[i].id, rf->values[i].value);
				found = 1;
			}
			j++;
		}	
	}

}
