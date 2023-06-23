#pragma once
extern int detect_cpu();
extern int detect_packages(void);
extern int check_paranoid(void);
extern int launch_program (int core, char ** exec_p, int iter);
extern int launch_program_mult (int core, char ** exec_p, int iter);
extern int generate_command (char * input, char ** output);

#define MAX_CPUS	1024
#define MAX_PACKAGES	16
#define STR_BUFF_SIZE 	256
//#define STR2_BUFF_SIZE  128

extern char str_buff[STR_BUFF_SIZE+32];
extern char format_buff[STR_BUFF_SIZE];
//extern char str_buff2[STR2_BUFF_SIZE];
extern int total_cores,total_packages;
extern int package_map[MAX_PACKAGES];
