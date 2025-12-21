#include "../include/scheduler.h"

// Priority Scheduling (Non-preemptive)
void priority_non_preemptive(Process proc[], int n) {
    export_header("ALGORITHM: Priority Scheduling (Non-preemptive)");
    
    int current_time = 0;
    int completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    char details[200];
    
    export_printf("========================== ACTIVITY LOG ====================================\n\n");
    
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
            sprintf(details, "IDLE | CPU waiting for next process");
            log_event(current_time, "CPU", 0, details);
            simulate_time_unit();
            current_time++;
            continue;
        }
        
        // Process arrives
        if (proc[highest].arrival_time == current_time) {
            sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Added to ready queue",
                    proc[highest].arrival_time, proc[highest].burst_time, proc[highest].priority);
            log_event(current_time, "ARR", proc[highest].pid, details);
        }
        
        // Process starts
        proc[highest].response_time = current_time - proc[highest].arrival_time;
        sprintf(details, "START    | Response Time=%d, Priority=%d | Beginning execution",
                proc[highest].response_time, proc[highest].priority);
        log_event(current_time, "RUN", proc[highest].pid, details);
        
        // Process running
        for (int t = 1; t <= proc[highest].burst_time; t++) {
            simulate_time_unit();
            current_time++;
            
            if (t == proc[highest].burst_time) {
                sprintf(details, "COMPLETE | CT=%d, TAT=%d | Finished execution",
                        current_time, current_time - proc[highest].arrival_time);
                log_event(current_time, "FIN", proc[highest].pid, details);
            } else if (t % 2 == 0 && proc[highest].burst_time > 3) {
                sprintf(details, "RUNNING  | Progress: %d/%d | Still executing",
                        t, proc[highest].burst_time);
                log_event(current_time, "RUN", proc[highest].pid, details);
            }
        }
        
        proc[highest].completion_time = current_time;
        is_completed[highest] = 1;
        completed++;
        
        // Show ready queue
        if (completed < n) {
            char queue_str[200] = "Ready Queue: [";
            int queue_count = 0;
            for (int j = 0; j < n; j++) {
                if (!is_completed[j] && proc[j].arrival_time <= current_time) {
                    char temp[30];
                    sprintf(temp, "P%d(Pri=%d) ", proc[j].pid, proc[j].priority);
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
    
    export_printf("\n");
    export_printf("============================= GANTT CHART ==================================\n\n");
    export_printf("Timeline: ");
    
    // Rebuild gantt
    current_time = 0;
    completed = 0;
    for (int i = 0; i < n; i++) is_completed[i] = 0;
    
    while (completed < n) {
        int highest = -1;
        int min_priority = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && 
                proc[i].arrival_time <= current_time &&
                proc[i].priority < min_priority) {
                highest = i;
                min_priority = proc[i].priority;
            }
        }
        
        if (highest == -1) {
            export_printf("| IDLE ");
            current_time++;
            continue;
        }
        
        export_printf("| P%d ", proc[highest].pid);
        current_time = proc[highest].completion_time;
        is_completed[highest] = 1;
        completed++;
    }
    export_printf("|\n\n");

    export_printf("Time:  ");
    
    current_time = 0;
    completed = 0;
    for (int i = 0; i < n; i++) is_completed[i] = 0;
    
    export_printf(" %3d ", current_time);
    
    while (completed < n) {
        int highest = -1;
        int min_priority = INT_MAX;
        
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
        
        current_time = proc[highest].completion_time;
        export_printf(" %3d ", current_time);
        is_completed[highest] = 1;
        completed++;
    }
    export_printf("\n\n");
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("Priority (Non-preemptive)", proc, n, &metrics);
}

// Priority Scheduling (Preemptive)
void priority_preemptive(Process proc[], int n) {
    export_header("ALGORITHM: Priority Scheduling (Preemptive)");
    
    int current_time = 0;
    int completed = 0;
    int prev_proc = -1;
    int context_switches = 0;
    char details[200];
    
    // Reset remaining time
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
    }
    
    export_printf("========================== ACTIVITY LOG ====================================\n\n");
    
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
            sprintf(details, "IDLE | Waiting for processes to arrive");
            log_event(current_time, "CPU", 0, details);
            simulate_time_unit();
            current_time++;
            continue;
        }
        
        // New arrival
        if (proc[highest].arrival_time == current_time && !proc[highest].first_run) {
            sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Entered ready queue",
                    proc[highest].arrival_time, proc[highest].burst_time, proc[highest].priority);
            log_event(current_time, "ARR", proc[highest].pid, details);
        }
        
        // Context switch
        if (prev_proc != highest && prev_proc != -1) {
            context_switches++;
            sprintf(details, "CONTEXT SWITCH #%d | From P%d to P%d (Pri: %d->%d)",
                    context_switches, prev_proc + 1, proc[highest].pid,
                    proc[prev_proc].priority, proc[highest].priority);
            log_event(current_time, "CTX", proc[highest].pid, details);
        }
        
        // Lần đầu chạy
        if (!proc[highest].first_run) {
            proc[highest].response_time = current_time - proc[highest].arrival_time;
            sprintf(details, "START    | Response Time=%d, Priority=%d, Remaining=%d | First run",
                    proc[highest].response_time, proc[highest].priority, proc[highest].remaining_time);
            log_event(current_time, "RUN", proc[highest].pid, details);
            proc[highest].first_run = 1;
        } else if (prev_proc != highest) {
            sprintf(details, "RESUME   | Priority=%d, Remaining Time=%d | Continuing execution",
                    proc[highest].priority, proc[highest].remaining_time);
            log_event(current_time, "RUN", proc[highest].pid, details);
        }

        simulate_time_unit();
        proc[highest].remaining_time--;
        current_time++;
        
        if (proc[highest].remaining_time == 0) {
            proc[highest].completion_time = current_time;
            completed++;
            sprintf(details, "COMPLETE | CT=%d, TAT=%d, WT=%d | Finished execution",
                    proc[highest].completion_time,
                    proc[highest].completion_time - proc[highest].arrival_time,
                    proc[highest].completion_time - proc[highest].arrival_time - proc[highest].burst_time);
            log_event(current_time, "FIN", proc[highest].pid, details);
        } else if (current_time % 3 == 0) {
            sprintf(details, "RUNNING  | Priority=%d, Remaining=%d | Still executing",
                    proc[highest].priority, proc[highest].remaining_time);
            log_event(current_time, "RUN", proc[highest].pid, details);
        }
        
        // Show ready queue
        if (current_time % 5 == 0 || proc[highest].remaining_time == 0) {
            char queue_str[200] = "Ready Queue: [";
            int queue_count = 0;
            for (int j = 0; j < n; j++) {
                if (proc[j].remaining_time > 0 && proc[j].arrival_time <= current_time) {
                    char temp[30];
                    sprintf(temp, "P%d(Pri=%d,RT=%d) ", proc[j].pid, proc[j].priority, proc[j].remaining_time);
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
        
        prev_proc = highest;
    }
    
    export_printf("\n[✓] Total Context Switches: %d\n", context_switches);
    export_printf("[✓] All processes completed\n\n");
    
    export_printf("============================= GANTT CHART ==================================\n\n");
    export_printf("Timeline: | ");
    
    // Rebuild gantt
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].first_run = 0;
    }
    
    current_time = 0;
    completed = 0;
    prev_proc = -1;
    
    while (completed < n) {
        int highest = -1;
        int min_priority = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time &&
                proc[i].remaining_time > 0 &&
                proc[i].priority < min_priority) {
                highest = i;
                min_priority = proc[i].priority;
            }
        }
        
        if (highest == -1) {
            export_printf("| IDLE ");
            current_time++;
            continue;
        }
        
        if (prev_proc != highest) {
            if (prev_proc != -1) export_printf("| ");
            export_printf("P%d ", proc[highest].pid);
        }
        
        if (!proc[highest].first_run) {
            proc[highest].response_time = current_time - proc[highest].arrival_time;
            proc[highest].first_run = 1;
        }
        
        proc[highest].remaining_time--;
        current_time++;
        prev_proc = highest;
        
        if (proc[highest].remaining_time == 0) {
            proc[highest].completion_time = current_time;
            completed++;
        }
    }
    export_printf("|\n\n");

    export_printf("Time:  ");
    
    // Reset
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].first_run = 0;
    }
    
    current_time = 0;
    completed = 0;
    prev_proc = -1;
    
    export_printf(" %3d ", current_time);  // In thời điểm đầu
    
    while (completed < n) {
        int highest = -1;
        int min_priority = INT_MAX;
        
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
        
        // CHỈ ĐẶT RESPONSE TIME, KHÔNG IN TIME Ở ĐÂY
        if (!proc[highest].first_run) {
            proc[highest].response_time = current_time - proc[highest].arrival_time;
            proc[highest].first_run = 1;
        }
        
        proc[highest].remaining_time--;
        current_time++;
        
        // Process complete HOẶC (2) Sắp switch sang process khác
        if (proc[highest].remaining_time == 0) {
            proc[highest].completion_time = current_time;
            export_printf(" %3d ", current_time);  // In khi complete
            completed++;
        } else {
            // Kiểm tra xem time unit tiếp theo có bị preempt không
            int next_highest = -1;
            int next_min_priority = INT_MAX;
            
            for (int i = 0; i < n; i++) {
                if (proc[i].arrival_time <= current_time &&
                    proc[i].remaining_time > 0 &&
                    proc[i].priority < next_min_priority) {
                    next_highest = i;
                    next_min_priority = proc[i].priority;
                }
            }
            
            // Nếu process khác sẽ chạy tiếp, in time hiện tại
            if (next_highest != highest) {
                export_printf(" %3d ", current_time);
            }
        }
        
        prev_proc = highest;
    }
    export_printf("\n\n");
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("Priority (Preemptive)", proc, n, &metrics);
}
