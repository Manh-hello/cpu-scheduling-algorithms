#include "../include/scheduler.h"

// SJF - Shortest Job First (Non-preemptive)
void sjf(Process proc[], int n) {
    export_header("🔹 SJF (Shortest Job First - Non-preemptive)");
    
    int current_time = 0;
    int completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    char gantt[MAX_GANTT_LENGTH] = "";
    char buffer[50];
    
    export_printf("\nGantt Chart:\n");
    
    while (completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;
        
        // Tìm process có burst time ngắn nhất đã đến
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && 
                proc[i].arrival_time <= current_time &&
                proc[i].burst_time < min_burst) {
                shortest = i;
                min_burst = proc[i].burst_time;
            }
        }
        
        if (shortest == -1) {
            current_time++;
            continue;
        }
        
        proc[shortest].response_time = current_time - proc[shortest].arrival_time;
        current_time += proc[shortest].burst_time;
        proc[shortest].completion_time = current_time;
        is_completed[shortest] = 1;
        completed++;
        
        sprintf(buffer, "| P%d ", proc[shortest].pid);
        strcat(gantt, buffer);
    }
    strcat(gantt, "|");
    
    export_printf("%s\n", gantt);
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("SJF", proc, n, &metrics);
}
