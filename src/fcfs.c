#include "../include/scheduler.h"

// FCFS - First Come First Served
void fcfs(Process proc[], int n) {
    export_header("🔹 FCFS (First Come First Served)");
    
    // Sắp xếp theo arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (proc[i].arrival_time > proc[j].arrival_time) {
                Process temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }
    
    int current_time = 0;
    char gantt[MAX_GANTT_LENGTH] = "";
    char buffer[50];
    
    export_printf("\nGantt Chart:\n");
    
    for (int i = 0; i < n; i++) {
        if (current_time < proc[i].arrival_time) {
            current_time = proc[i].arrival_time;
        }
        
        proc[i].response_time = current_time - proc[i].arrival_time;
        current_time += proc[i].burst_time;
        proc[i].completion_time = current_time;
        
        sprintf(buffer, "| P%d ", proc[i].pid);
        strcat(gantt, buffer);
    }
    strcat(gantt, "|");
    
    export_printf("%s\n", gantt);
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("FCFS", proc, n, &metrics);
}
