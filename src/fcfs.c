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
    int next_arrival_idx = 0; // Track processes chưa log arrival
    int logged_arrival[MAX_PROCESSES] = {0}; // Đánh dấu đã log arrival
    
    // Log processes arrive lúc time 0
    for (int i = 0; i < n && proc[i].arrival_time == 0; i++) {
        sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Entering ready queue", 
                proc[i].arrival_time, proc[i].burst_time, proc[i].priority);
        log_event_with_sim_time(current_time, "ARR", proc[i].pid, details);
        logged_arrival[i] = 1;
        next_arrival_idx = i + 1;
    }
    
    for (int i = 0; i < n; i++) {
        // Handle idle time
        if (current_time < proc[i].arrival_time) {
            sprintf(details, "IDLE | CPU waiting for next process");
            log_event_with_sim_time(current_time, "CPU", 0, details);
            
            // Simulate idle time và log arrivals trong thời gian idle
            for (int t = current_time; t < proc[i].arrival_time; t++) {
                simulate_time_unit();
                
                // Check và log processes arrive tại thời điểm này
                while (next_arrival_idx < n && proc[next_arrival_idx].arrival_time == t + 1) {
                    sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Entering ready queue", 
                            proc[next_arrival_idx].arrival_time, 
                            proc[next_arrival_idx].burst_time, 
                            proc[next_arrival_idx].priority);
                    log_event_with_sim_time(t + 1, "ARR", proc[next_arrival_idx].pid, details);
                    logged_arrival[next_arrival_idx] = 1;
                    next_arrival_idx++;
                }
            }
            current_time = proc[i].arrival_time;
        }
        
        // Log arrival nếu chưa log (process này vừa mới arrive)
        if (!logged_arrival[i]) {
            sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Entering ready queue", 
                    proc[i].arrival_time, proc[i].burst_time, proc[i].priority);
            log_event_with_sim_time(current_time, "ARR", proc[i].pid, details);
            logged_arrival[i] = 1;
        }
        
        // Process starts
        proc[i].response_time = current_time - proc[i].arrival_time;
        sprintf(details, "START    | Response Time=%d | Beginning execution", 
                proc[i].response_time);
        log_event_with_sim_time(current_time, "RUN", proc[i].pid, details);
        
        // Process running - simulate actual execution
        for (int t = 1; t <= proc[i].burst_time; t++) {
            current_time++;
            
            // Log progress trước khi sleep
            if (t == proc[i].burst_time) {
                // Process completes
                sprintf(details, "COMPLETE | CT=%d, TAT=%d | Finished execution", 
                        current_time, current_time - proc[i].arrival_time);
                log_event_with_sim_time(current_time, "FIN", proc[i].pid, details);
            } else if (t % 2 == 0 && proc[i].burst_time > 3) {
                // Log progress
                sprintf(details, "RUNNING  | Progress: %d/%d | Still executing", 
                        t, proc[i].burst_time);
                log_event_with_sim_time(current_time, "RUN", proc[i].pid, details);
            }
            
            // Check và log processes arrive trong khi đang chạy
            while (next_arrival_idx < n && proc[next_arrival_idx].arrival_time == current_time) {
                sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Entering ready queue", 
                        proc[next_arrival_idx].arrival_time, 
                        proc[next_arrival_idx].burst_time, 
                        proc[next_arrival_idx].priority);
                log_event_with_sim_time(current_time, "ARR", proc[next_arrival_idx].pid, details);
                logged_arrival[next_arrival_idx] = 1;
                next_arrival_idx++;
            }
            
            simulate_time_unit();  // Sleep sau khi log
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
            log_queue_with_sim_time(current_time, queue_str);
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
    export_printf(" %3d ", current_time);
    for (int i = 0; i < n; i++) {
        if (current_time < proc[i].arrival_time) {
            current_time = proc[i].arrival_time;
            export_printf(" %3d  ", current_time);
        }
        current_time = proc[i].completion_time;
        export_printf(" %3d ", current_time);
    }
    export_printf("\n");
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("FCFS", proc, n, &metrics);
}
