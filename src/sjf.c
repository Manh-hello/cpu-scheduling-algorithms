#include "../include/scheduler.h"

void sjf(Process proc[], int n) {
    export_header("ALGORITHM: SJF (Shortest Job First - Non-preemptive)");
    
    int current_time = 0;
    int completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    char details[200];
    
    export_printf("========================== ACTIVITY LOG ====================================\n\n");
    
    while (completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && 
                proc[i].arrival_time <= current_time &&
                proc[i].burst_time < min_burst) {
                shortest = i;
                min_burst = proc[i].burst_time;
            }
        }
        
        if (shortest == -1) {
            sprintf(details, "IDLE | CPU waiting for next process");
            log_event(current_time, "CPU", 0, details);
            simulate_time_unit();
            current_time++;
            continue;
        }
        
        if (proc[shortest].arrival_time == current_time) {
            sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Added to ready queue",
                    proc[shortest].arrival_time, proc[shortest].burst_time, proc[shortest].priority);
            log_event(current_time, "ARR", proc[shortest].pid, details);
        }
        
        proc[shortest].response_time = current_time - proc[shortest].arrival_time;
        sprintf(details, "START    | Response Time=%d, BT=%d | Beginning execution",
                proc[shortest].response_time, proc[shortest].burst_time);
        log_event(current_time, "RUN", proc[shortest].pid, details);
        
        for (int t = 1; t <= proc[shortest].burst_time; t++) {
            simulate_time_unit();
            current_time++;
            
            if (t == proc[shortest].burst_time) {
                sprintf(details, "COMPLETE | CT=%d, TAT=%d | Finished execution",
                        current_time, current_time - proc[shortest].arrival_time);
                log_event(current_time, "FIN", proc[shortest].pid, details);
            } else if (t % 2 == 0 && proc[shortest].burst_time > 3) {
                sprintf(details, "RUNNING  | Progress: %d/%d | Still executing",
                        t, proc[shortest].burst_time);
                log_event(current_time, "RUN", proc[shortest].pid, details);
            }
        }
        
        proc[shortest].completion_time = current_time;
        is_completed[shortest] = 1;
        completed++;
        
        if (completed < n) {
            char queue_str[200] = "Ready Queue: [";
            int queue_count = 0;
            for (int j = 0; j < n; j++) {
                if (!is_completed[j] && proc[j].arrival_time <= current_time) {
                    char temp[20];
                    sprintf(temp, "P%d(BT=%d) ", proc[j].pid, proc[j].burst_time);
                    strcat(queue_str, temp);
                    queue_count++;
                }
            }
            if (queue_count > 0) {
                strcat(queue_str, "]");
            } else {
                strcat(queue_str, "EMPTY]");
            }
            log_queue(current_time, queue_str);
        }
    }
    
    export_printf("\n============================= GANTT CHART ==================================\n\n");
    export_printf("Timeline: ");
    
    current_time = 0;
    completed = 0;
    for (int i = 0; i < n; i++) is_completed[i] = 0;
    
    while (completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && 
                proc[i].arrival_time <= current_time &&
                proc[i].burst_time < min_burst) {
                shortest = i;
                min_burst = proc[i].burst_time;
            }
        }
        
        if (shortest == -1) {
            export_printf("| IDLE ");
            current_time++;
            continue;
        }
        
        export_printf("| P%d ", proc[shortest].pid);
        current_time = proc[shortest].completion_time;
        is_completed[shortest] = 1;
        completed++;
    }
    export_printf("|\n\n");

    export_printf("Time:  ");
    current_time = 0;
    completed = 0;
    for (int i = 0; i < n; i++) is_completed[i] = 0;
    
    export_printf(" %3d ", current_time);
    
    while (completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;
        
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
            export_printf(" %3d  ", current_time);
            continue;
        }
        
        current_time = proc[shortest].completion_time;
        export_printf(" %3d ", current_time);
        is_completed[shortest] = 1;
        completed++;
    }
    export_printf("\n\n");
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("SJF", proc, n, &metrics);
}
