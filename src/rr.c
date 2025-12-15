#include "../include/scheduler.h"

void round_robin(Process proc[], int n) {
    export_header("ALGORITHM: Round Robin (Time Quantum = 4)");
    
    int current_time = 0;
    int completed = 0;
    int queue[MAX_PROCESSES * 10];
    int front = 0, rear = 0;
    int in_queue[MAX_PROCESSES] = {0};
    int context_switches = 0;
    char details[200];
    
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
    
    export_printf("========================== ACTIVITY LOG ====================================\n\n");
    
    // Log first arrival
    sprintf(details, "ARRIVED | AT=%d, BT=%d, Priority=%d | Added to ready queue", 
            proc[first_idx].arrival_time, proc[first_idx].burst_time, proc[first_idx].priority);
    log_event(current_time, "ARR", proc[first_idx].pid, details);
    
    int prev_proc = -1;
    
    while (completed < n) {
        if (front == rear) {
            // Queue empty
            int found = 0;
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && proc[i].remaining_time > 0 &&
                    proc[i].arrival_time <= current_time) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                    found = 1;
                    
                    sprintf(details, "ARRIVED | AT=%d, BT=%d, Priority=%d | Added to ready queue",
                            proc[i].arrival_time, proc[i].burst_time, proc[i].priority);
                    log_event(current_time, "ARR", proc[i].pid, details);
                    break;
                }
            }
            if (!found) {
                sprintf(details, "IDLE | Waiting for processes to arrive");
                log_event(current_time, "CPU", 0, details);
                simulate_time_unit();
                current_time++;
                continue;
            }
        }
        
        int idx = queue[front++];
        
        // Context switch
        if (prev_proc != idx && prev_proc != -1) {
            context_switches++;
            sprintf(details, "CONTEXT SWITCH #%d | From P%d to P%d", 
                    context_switches, prev_proc + 1, proc[idx].pid);
            log_event(current_time, "CTX", proc[idx].pid, details);
        }
        
        // First run
        if (!proc[idx].first_run) {
            proc[idx].response_time = current_time - proc[idx].arrival_time;
            sprintf(details, "START   | Response Time=%d, Remaining=%d | First time running",
                    proc[idx].response_time, proc[idx].remaining_time);
            log_event(current_time, "RUN", proc[idx].pid, details);
            proc[idx].first_run = 1;
        } else {
            sprintf(details, "RESUME  | Remaining Time=%d | Continuing execution",
                    proc[idx].remaining_time);
            log_event(current_time, "RUN", proc[idx].pid, details);
        }
        
        int exec_time = (proc[idx].remaining_time > TIME_QUANTUM) ? 
                        TIME_QUANTUM : proc[idx].remaining_time;
        
        int start_time = current_time;
        
        // Execute and check for arrivals
        for (int t = 0; t < exec_time; t++) {
            simulate_time_unit();
            current_time++;
            proc[idx].remaining_time--;
            
            // Log progress in middle of quantum
            if (t == exec_time / 2 && exec_time > 2) {
                sprintf(details, "RUNNING  | Progress: %d/%d of quantum | Still executing",
                        t + 1, exec_time);
                log_event(current_time, "RUN", proc[idx].pid, details);
            }
            
            // Check for new arrivals during execution
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && proc[i].remaining_time > 0 &&
                    proc[i].arrival_time == current_time) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                    sprintf(details, "ARRIVED | AT=%d, BT=%d, Priority=%d | Added to ready queue",
                            proc[i].arrival_time, proc[i].burst_time, proc[i].priority);
                    log_event(current_time, "ARR", proc[i].pid, details);
                }
            }
        }
        
        // Show queue status
        char queue_str[200] = "Ready Queue: [";
        int queue_count = 0;
        for (int i = front; i < rear; i++) {
            char temp[20];
            sprintf(temp, "P%d ", proc[queue[i]].pid);
            strcat(queue_str, temp);
            queue_count++;
        }
        if (queue_count > 0) {
            strcat(queue_str, "]");
        } else {
            strcat(queue_str, "EMPTY]");
        }
        log_queue(current_time, queue_str);
        
        // Check if process finished or preempted
        if (proc[idx].remaining_time > 0) {
            sprintf(details, "PREEMPT | Remaining Time=%d | Time quantum expired, back to queue",
                    proc[idx].remaining_time);
            log_event(current_time, "PRE", proc[idx].pid, details);
            queue[rear++] = idx;
        } else {
            proc[idx].completion_time = current_time;
            in_queue[idx] = 0;
            completed++;
            sprintf(details, "COMPLETE | CT=%d, TAT=%d, WT=%d | Finished execution",
                    proc[idx].completion_time,
                    proc[idx].completion_time - proc[idx].arrival_time,
                    proc[idx].completion_time - proc[idx].arrival_time - proc[idx].burst_time);
            log_event(current_time, "FIN", proc[idx].pid, details);
        }
        
        prev_proc = idx;
    }
    
    export_printf("\n[✓] Total Context Switches: %d\n", context_switches);
    export_printf("[✓] All processes completed\n\n");
    
    export_printf("============================= GANTT CHART ==================================\n\n");
    export_printf("Timeline: ");
    
    // Rebuild gantt for display
    current_time = 0;
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].first_run = 0;
    }
    front = rear = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) in_queue[i] = 0;
    completed = 0;
    
    if (first_idx != -1) {
        queue[rear++] = first_idx;
        in_queue[first_idx] = 1;
        current_time = proc[first_idx].arrival_time;
    }
    
    while (completed < n) {
        if (front == rear) {
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && proc[i].remaining_time > 0 &&
                    proc[i].arrival_time <= current_time) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                    break;
                }
            }
            if (front == rear) {
                current_time++;
                continue;
            }
        }
        
        int idx = queue[front++];
        
        if (!proc[idx].first_run) {
            proc[idx].response_time = current_time - proc[idx].arrival_time;
            proc[idx].first_run = 1;
        }
        
        int exec_time = (proc[idx].remaining_time > TIME_QUANTUM) ? 
                        TIME_QUANTUM : proc[idx].remaining_time;
        
        export_printf("| P%d ", proc[idx].pid);
        
        proc[idx].remaining_time -= exec_time;
        current_time += exec_time;
        
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && proc[i].remaining_time > 0 &&
                proc[i].arrival_time <= current_time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }
        
        if (proc[idx].remaining_time > 0) {
            queue[rear++] = idx;
        } else {
            proc[idx].completion_time = current_time;
            in_queue[idx] = 0;
            completed++;
        }
    }
    export_printf("|\n\n");
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("Round Robin", proc, n, &metrics);
}
