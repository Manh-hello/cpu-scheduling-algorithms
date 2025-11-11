#ifndef CHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Constants
#define MAX_PROCESSES 10
#define TIME_QUANTUM 4

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

// Function prototypes - Utils
int read_from_file(Process proc[], int *n, const char *filename);
void input_processes(Process proc[], int *n);
void use_sample_data(Process proc[], int *n);
void calculate_metrics(Process proc[], int n);
void print_menu();

// Function prototypes - Algorithms
void fcfs(Process proc[], int n);
void sjf(Process proc[], int n);
void srtf(Process proc[], int n);
void round_robin(Process proc[], int n);
void priority_non_preemptive(Process proc[], int n);
void priority_preemptive(Process proc[], int n);

#endif
