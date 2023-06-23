#pragma once
#include "recorder.h"
#include "defines.h"






extern void init_counters (struct event_count_s *suma);

extern void reset_event_counters ();

extern void stop_counter();

extern void get_counter_data (struct event_count_s *data);
