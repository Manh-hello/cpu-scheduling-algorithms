
#include "../include/scheduler.h"

// FCFS - First Come First Served
void fcfs(Process proc[], int n) {
    printf("\n🔹 FCFS (First Come First Served)\n");
    
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
    printf("\nGantt Chart:\n");
    
    for (int i = 0; i < n; i++) {
        if (current_time < proc[i].arrival_time) {
            current_time = proc[i].arrival_time;
        }
        
        proc[i].response_time = current_time - proc[i].arrival_time;
        current_time += proc[i].burst_time;
        proc[i].completion_time = current_time;
        
        printf("| P%d ", proc[i].pid);
    }
    printf("|\n");
    
    calculate_metrics(proc, n);
}
