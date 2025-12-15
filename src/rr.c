#include "../include/scheduler.h"

// Round Robin - FIXED queue logic
void round_robin(Process proc[], int n) {
    export_header("🔹 Round Robin (Time Quantum = 4)");
    
    int current_time = 0;
    int completed = 0;
    int queue[MAX_PROCESSES * 10];
    int front = 0, rear = 0;
    int in_queue[MAX_PROCESSES] = {0};
    char gantt[MAX_GANTT_LENGTH] = "";
    char buffer[50];
    
    // Reset remaining time
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
    }
    
    // Find first process
    int first_idx = -1;
    int min_arrival = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival_time < min_arrival) {
            min_arrival = proc[i].arrival_time;
            first_idx = i;
        }
    }
    
    if (first_idx != -1) {
        queue[rear++] = first_idx;
        in_queue[first_idx] = 1;
        current_time = proc[first_idx].arrival_time;
    }
    
    export_printf("\n📊 Gantt Chart:\n");
    export_printf("   ");
    
    while (completed < n) {
        if (front == rear) {
            // Queue empty - find next process
            int found = 0;
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && proc[i].remaining_time > 0 &&
                    proc[i].arrival_time <= current_time) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                current_time++;
                continue;
            }
        }
        
        int idx = queue[front++];
        
        // First run
        if (!proc[idx].first_run) {
            proc[idx].response_time = current_time - proc[idx].arrival_time;
            proc[idx].first_run = 1;
        }
        
        int exec_time = (proc[idx].remaining_time > TIME_QUANTUM) ? 
                        TIME_QUANTUM : proc[idx].remaining_time;
        
        proc[idx].remaining_time -= exec_time;
        current_time += exec_time;
        
        sprintf(buffer, "| P%d ", proc[idx].pid);
        if (strlen(gantt) + strlen(buffer) < MAX_GANTT_LENGTH - 1) {
            strcat(gantt, buffer);
        }
        
        // Add newly arrived processes to queue
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && proc[i].remaining_time > 0 &&
                proc[i].arrival_time <= current_time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }
        
        // If not finished, add back to queue
        if (proc[idx].remaining_time > 0) {
            queue[rear++] = idx;
        } else {
            proc[idx].completion_time = current_time;
            in_queue[idx] = 0;
            completed++;
        }
    }
    
    strcat(gantt, "|");
    export_printf("%s\n", gantt);
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("Round Robin", proc, n, &metrics);
}
