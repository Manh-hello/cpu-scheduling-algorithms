#include "../include/scheduler.h"

// Priority Scheduling (Non-preemptive)
void priority_non_preemptive(Process proc[], int n) {
    printf("\n🔹 Priority Scheduling (Non-preemptive)\n");
    
    int current_time = 0;
    int completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    
    printf("\nGantt Chart:\n");
    
    while (completed < n) {
        int highest = -1;
        int min_priority = INT_MAX;
        
        // Tìm process có priority cao nhất (số nhỏ nhất)
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && 
                proc[i].arrival_time <= current_time &&
                proc[i].priority < min_priority) {
                highest = i;
                min_priority = proc[i].priority;
            }
        }
        
        if (highest == -1) {
            current_time++;
            continue;
        }
        
        proc[highest].response_time = current_time - proc[highest].arrival_time;
        current_time += proc[highest].burst_time;
        proc[highest].completion_time = current_time;
        is_completed[highest] = 1;
        completed++;
        
        printf("| P%d ", proc[highest].pid);
    }
    printf("|\n");
    
    calculate_metrics(proc, n);
}

// Priority Scheduling (Preemptive)
void priority_preemptive(Process proc[], int n) {
    printf("\n🔹 Priority Scheduling (Preemptive)\n");
    
    int current_time = 0;
    int completed = 0;
    int prev_proc = -1;
    
    // Reset remaining time
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
    }
    
    printf("\nGantt Chart:\n");
    
    while (completed < n) {
        int highest = -1;
        int min_priority = INT_MAX;
        
        // Tìm process có priority cao nhất
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time &&
                proc[i].remaining_time > 0 &&
                proc[i].priority < min_priority) {
                highest = i;
                min_priority = proc[i].priority;
            }
        }
        
        if (highest == -1) {
            current_time++;
            continue;
        }
        
        // Lần đầu chạy
        if (!proc[highest].first_run) {
            proc[highest].response_time = current_time - proc[highest].arrival_time;
            proc[highest].first_run = 1;
        }
        
        // In Gantt chart khi đổi process
        if (prev_proc != highest) {
            if (prev_proc != -1) printf("| ");
            printf("P%d ", proc[highest].pid);
        }
        
        proc[highest].remaining_time--;
        current_time++;
        prev_proc = highest;
        
        if (proc[highest].remaining_time == 0) {
            proc[highest].completion_time = current_time;
            completed++;
        }
    }
    printf("|\n");
    
    calculate_metrics(proc, n);
}
