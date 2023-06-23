
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
//#include "rapl_recorder.h"


int total_cores=0,total_packages=0;
int package_map[MAX_PACKAGES];
char str_buff[STR_BUFF_SIZE+32];
char format_buff[STR_BUFF_SIZE];


int detect_packages(void) {

	char filename[BUFSIZ];
	FILE *fff;
	int package;
	int i;

	for(i=0;i<MAX_PACKAGES;i++) package_map[i]=-1;

	printf("\t");
	for(i=0;i<MAX_CPUS;i++) {
		sprintf(filename,"/sys/devices/system/cpu/cpu%d/topology/physical_package_id",i);
		fff=fopen(filename,"r");
		if (fff==NULL) break;
		fscanf(fff,"%d",&package);
		printf("%d (%d)",i,package);
		if (i%8==7) printf("\n\t"); else printf(", ");
		fclose(fff);

		if (package_map[package]==-1) {
			total_packages++;
			package_map[package]=i;
		}

	}

	printf("\n");

	total_cores=i;

	printf("\tDetected %d cores in %d packages\n\n",
		total_cores,total_packages);

	return 0;
}


int check_paranoid(void) {

	int paranoid_value;
	FILE *fff;

	fff=fopen("/proc/sys/kernel/perf_event_paranoid","r");
	if (fff==NULL) {
		fprintf(stderr,"Error! could not open /proc/sys/kernel/perf_event_paranoid %s\n",
			strerror(errno));
		printf ("El paranoid!\n");
		/* We can't return a negative value as that implies no paranoia */
		return 500;
	}

	fscanf(fff,"%d",&paranoid_value);
	fclose(fff);

	return paranoid_value;

}


char hexa[32];

int launch_program (int core, char ** exec_p, int iter){
	int ret = -1;
	  ret = fork ();
	  if (ret == 0)
	  {
		// affinity con taskset, el comando se coloca en generate command
		sprintf (hexa, "%04x",1 << core);
		exec_p[1] = hexa;
		execv (exec_p[0], exec_p);


		printf ("Algo ha ido mal al lanzar el proceso del programa!\n");
		exit (-1);
	  }else printf ("Soy padre y mi hijo tiene pid %d, iter %d\n", ret, iter);
	return ret;
}

int launch_program_mult (int affinity, char ** exec_p, int iter){
	int ret = -1;
	ret = fork ();
	if (ret == 0)
	{
		// affinity con taskset, el comando se coloca en generate command
		sprintf (hexa, "%04x", affinity);
		exec_p[1] = hexa;
		printf ("%s %s %s\n", exec_p[0], exec_p[1], exec_p[2]);
		execv (exec_p[0], exec_p);


		printf ("Algo ha ido mal al lanzar el proceso del programa!\n");
		exit (-1);
	}else printf ("Soy padre y mi hijo tiene pid %d, iter %d\n", ret, iter);
	return ret;
}

int generate_command (char * input, char ** output){
	int i, len, start = 0, end = 0, cont = 0;	


	printf ("El comando es %s\n", input);

	len = strlen (input);
	end = len;
	for (i=0; i<len; i++) 
	{ 
		if (input[i] == ' ')
		{
			end = i;
			// grab substring
			strncpy (output[cont++], &input[start], end-start);
	//		printf ("start: %c;%d; end %c;%d; word: %s\n",input[start],start, input[end], end, output[cont-1]);
			output[cont-1][end-start] = '\0';
			start = i+1;
		}
	}

	strncpy (output[cont++], &input[start], len-start);
	output[cont-1][len-start] = '\0';

//	printf ("Se han encontrado %d substrings!\n", cont);
	//for (i=0; i<cont;i++) printf ("%d: %s\n", i, output[i]);

	output[-2] ="/usr/bin/taskset";


	// ultimo elemento del vector tiene que ser null para execv
	output[cont] = (char *) NULL;

	return cont;
}


