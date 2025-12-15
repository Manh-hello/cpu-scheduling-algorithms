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
#define MAX_GANTT_LENGTH 1000

// Process structure
typedef struct {
    int pid;              // Process ID
    int arrival_time;     // Thời điểm đến
    int burst_time;       // Thời gian CPU cần
    int remaining_time;   // Thời gian còn lại (cho preemptive)
    int priority;         // Độ ưu tiên (số nhỏ = ưu tiên cao)
    int completion_time;  // Thời điểm hoàn thành
    int waiting_time;     // Thời gian chờ
    int turnaround_time;  // Thời gian hoàn thành
    int response_time;    // Thời gian phản hồi
    int first_run;        // Đánh dấu lần đầu chạy
} Process;

// Gantt chart structure
typedef struct {
    char chart[MAX_GANTT_LENGTH];
    int length;
} GanttChart;

// Metrics structure
typedef struct {
    float avg_turnaround;
    float avg_waiting;
    float avg_response;
    float cpu_utilization;
    int total_time;
} Metrics;

// Global variable for output file
extern FILE *output_file;
extern int export_enabled;

// Function prototypes - Utils
int read_from_file(Process proc[], int *n, const char *filename);
void input_processes(Process proc[], int *n);
void use_sample_data(Process proc[], int *n);
void calculate_metrics(Process proc[], int n, Metrics *metrics);
void print_menu();
void print_table(Process proc[], int n);

// Export functions
void enable_export(const char *filename);
void disable_export();
void export_printf(const char *format, ...);
void export_header(const char *algorithm_name);
void export_metrics(const char *algorithm_name, Process proc[], int n, Metrics *metrics);
void export_gantt_chart(const char *chart);

// Function prototypes - Algorithms
void fcfs(Process proc[], int n);
void sjf(Process proc[], int n);
void srtf(Process proc[], int n);
void round_robin(Process proc[], int n);
void priority_non_preemptive(Process proc[], int n);
void priority_preemptive(Process proc[], int n);

#endif
