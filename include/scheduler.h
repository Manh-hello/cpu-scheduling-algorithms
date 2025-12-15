#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// Constants
#define MAX_PROCESSES 10
#define TIME_QUANTUM 4
#define MAX_GANTT_LENGTH 2000  // Increased for safety

// Process structure
typedef struct {
    int pid;              // Process ID
    int arrival_time;     // Arrival time
    int burst_time;       // CPU burst time
    int remaining_time;   // Remaining time (for preemptive)
    int priority;         // Priority (lower number = higher priority)
    int completion_time;  // Completion time
    int waiting_time;     // Waiting time
    int turnaround_time;  // Turnaround time
    int response_time;    // Response time
    int first_run;        // First run flag
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

// Algorithm functions
void fcfs(Process proc[], int n);
void sjf(Process proc[], int n);
void srtf(Process proc[], int n);
void round_robin(Process proc[], int n);
void priority_non_preemptive(Process proc[], int n);
void priority_preemptive(Process proc[], int n);

#endif
