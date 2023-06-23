#pragma once
#include <time.h>
#include <stdio.h>

struct event_count_s{
	struct timespec start_time;
	long long *events;
	long long *event_sum;

	
	
	FILE *fd_out[2]; // 2 al ser 2 CPU

};


extern int record_data (struct event_count_s *data);
extern void start_record (struct event_count_s *data, char *filename, int iter);
extern int end_record (struct event_count_s *data);
extern int terminate_record (struct event_count_s *data);
