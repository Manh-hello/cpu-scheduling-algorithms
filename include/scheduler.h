#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

// Constants
#define MAX_PROCESSES 10
#define TIME_QUANTUM 4
#define MAX_GANTT_LENGTH 2000

// Process structure
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int first_run;
} Process;

// Metrics structure
typedef struct {
    float avg_turnaround;
    float avg_waiting;
    float avg_response;
    float cpu_utilization;
    int total_time;
} Metrics;

// Global variables for export
extern FILE *output_file;
extern int export_enabled;
extern char export_filename[256];
extern struct timeval simulation_start_time;
extern int time_scale_ms;  // Milliseconds per time unit

// Function prototypes - Utils
int read_from_file(Process proc[], int *n, const char *filename);
void calculate_metrics(Process proc[], int n, Metrics *metrics);
void print_table(Process proc[], int n);
void print_input_table(Process proc[], int n);

// Export functions
void enable_export(const char *filename);
void disable_export();
void export_printf(const char *format, ...);
void export_header(const char *algorithm_name);
void export_metrics(const char *algorithm_name, Process proc[], int n, Metrics *metrics);
void export_comparison_summary();

// Simulation timing
void simulate_time_unit();  // Sleep for one time unit
double get_elapsed_time();  // Get elapsed real time
void format_elapsed_time(char *buffer, double elapsed);

// Logging functions with real elapsed time (no sim_time needed)
void log_event(int sim_time, const char *event_type, int pid, const char *details);
void log_queue(int sim_time, const char *queue_content);

// Algorithm functions
void fcfs(Process proc[], int n);
void sjf(Process proc[], int n);
void srtf(Process proc[], int n);
void round_robin(Process proc[], int n);
void priority_non_preemptive(Process proc[], int n);
void priority_preemptive(Process proc[], int n);

#endif
