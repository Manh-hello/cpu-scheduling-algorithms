#include "../include/scheduler.h"

void fcfs(Process proc[], int n) {
    export_header("ALGORITHM: FCFS (First Come First Served)");
    
    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (proc[i].arrival_time > proc[j].arrival_time) {
                Process temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }
    
    export_printf("========================== ACTIVITY LOG ====================================\n\n");
    
    int current_time = 0;
    char details[150];
    
    for (int i = 0; i < n; i++) {
        // Handle idle time
        if (current_time < proc[i].arrival_time) {
            sprintf(details, "IDLE | CPU waiting for next process");
            log_event(current_time, "CPU", 0, details);
            
            // Simulate idle time passing
            for (int t = current_time; t < proc[i].arrival_time; t++) {
                simulate_time_unit();
            }
            current_time = proc[i].arrival_time;
        }
        
        // Process arrives
        sprintf(details, "ARRIVED | AT=%d, BT=%d, Priority=%d | Entering ready queue", 
                proc[i].arrival_time, proc[i].burst_time, proc[i].priority);
        log_event(current_time, "ARR", proc[i].pid, details);
        
        // Process starts
        proc[i].response_time = current_time - proc[i].arrival_time;
        sprintf(details, "START   | Response Time=%d | Beginning execution", 
                proc[i].response_time);
        log_event(current_time, "RUN", proc[i].pid, details);
        
        // Process running - simulate actual execution
        for (int t = 1; t <= proc[i].burst_time; t++) {
            simulate_time_unit();  // Wait for 1 time unit
            current_time++;
            
            if (t == proc[i].burst_time) {
                // Process completes
                sprintf(details, "COMPLETE | CT=%d, TAT=%d | Finished execution", 
                        current_time, current_time - proc[i].arrival_time);
                log_event(current_time, "FIN", proc[i].pid, details);
            } else if (t % 2 == 0 && proc[i].burst_time > 3) {
                // Log progress
                sprintf(details, "RUNNING  | Progress: %d/%d | Still executing", 
                        t, proc[i].burst_time);
                log_event(current_time, "RUN", proc[i].pid, details);
            }
        }
        
        proc[i].completion_time = current_time;
        
        // Show ready queue
        if (i < n - 1) {
            char queue_str[200] = "Ready Queue: [";
            int queue_count = 0;
            for (int j = i + 1; j < n && proc[j].arrival_time <= current_time; j++) {
                char temp[20];
                sprintf(temp, "P%d ", proc[j].pid);
                strcat(queue_str, temp);
                queue_count++;
            }
            if (queue_count > 0) {
                strcat(queue_str, "]");
            } else {
                strcat(queue_str, "EMPTY]");
            }
            log_queue(current_time, queue_str);
        }
    }
    
    export_printf("\n");
    export_printf("============================= GANTT CHART ==================================\n\n");
    export_printf("Timeline: ");
    
    current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < proc[i].arrival_time) {
            export_printf("| IDLE ");
            current_time = proc[i].arrival_time;
        }
        export_printf("| P%d ", proc[i].pid);
        current_time = proc[i].completion_time;
    }
    export_printf("|\n");
    
    export_printf("Time:  ");
    current_time = 0;
    export_printf(" %3d  ", current_time);
    for (int i = 0; i < n; i++) {
        if (current_time < proc[i].arrival_time) {
            current_time = proc[i].arrival_time;
            export_printf(" %3d  ", current_time);
        }
        current_time = proc[i].completion_time;
        export_printf(" %3d  ", current_time);
    }
    export_printf("\n");
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("FCFS", proc, n, &metrics);
}
