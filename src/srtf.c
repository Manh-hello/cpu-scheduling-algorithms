#include "../include/scheduler.h"

void srtf(Process proc[], int n) {
    export_header("ALGORITHM: SRTF (Shortest Remaining Time First - Preemptive)");
    
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
        int shortest = -1;
        int min_remaining = INT_MAX;
        
        // Tìm process có remaining time ngắn nhất
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time &&
                proc[i].remaining_time > 0 &&
                proc[i].remaining_time < min_remaining) {
                shortest = i;
                min_remaining = proc[i].remaining_time;
            }
        }
        
        if (shortest == -1) {
            sprintf(details, "IDLE | Waiting for processes to arrive");
            log_event(current_time, "CPU", 0, details);
            current_time++;
            continue;
        }
        
        // New arrival
        if (proc[shortest].arrival_time == current_time && !proc[shortest].first_run) {
            sprintf(details, "ARRIVED | AT=%d, BT=%d, Priority=%d | Entered ready queue",
                    proc[shortest].arrival_time, proc[shortest].burst_time, proc[shortest].priority);
            log_event(current_time, "ARR", proc[shortest].pid, details);
        }
        
        // Context switch
        if (prev_proc != shortest && prev_proc != -1) {
            context_switches++;
            sprintf(details, "CONTEXT SWITCH #%d | From P%d to P%d",
                    context_switches, prev_proc + 1, proc[shortest].pid);
            log_event(current_time, "CTX", proc[shortest].pid, details);
        }
        
        // Lần đầu chạy
        if (!proc[shortest].first_run) {
            proc[shortest].response_time = current_time - proc[shortest].arrival_time;
            sprintf(details, "START   | Response Time=%d, Remaining=%d | First time running",
                    proc[shortest].response_time, proc[shortest].remaining_time);
            log_event(current_time, "RUN", proc[shortest].pid, details);
            proc[shortest].first_run = 1;
        } else if (prev_proc != shortest) {
            sprintf(details, "RESUME  | Remaining Time=%d | Continuing execution",
                    proc[shortest].remaining_time);
            log_event(current_time, "RUN", proc[shortest].pid, details);
        }
        
        // Execute one time unit
        proc[shortest].remaining_time--;
        current_time++;
        
        // Check if completed
        if (proc[shortest].remaining_time == 0) {
            proc[shortest].completion_time = current_time;
            completed++;
            sprintf(details, "COMPLETE | CT=%d, TAT=%d, WT=%d | Finished execution",
                    proc[shortest].completion_time,
                    proc[shortest].completion_time - proc[shortest].arrival_time,
                    proc[shortest].completion_time - proc[shortest].arrival_time - proc[shortest].burst_time);
            log_event(current_time, "FIN", proc[shortest].pid, details);
        } else if (current_time % 3 == 0) {
            // Log progress every 3 time units
            sprintf(details, "RUNNING  | Remaining Time=%d | Still executing",
                    proc[shortest].remaining_time);
            log_event(current_time, "RUN", proc[shortest].pid, details);
        }
        
        // Show ready queue every 5 time units or on completion
        if (current_time % 5 == 0 || proc[shortest].remaining_time == 0) {
            char queue_str[200] = "Ready Queue: [";
            int queue_count = 0;
            for (int j = 0; j < n; j++) {
                if (proc[j].remaining_time > 0 && proc[j].arrival_time <= current_time) {
                    char temp[30];
                    sprintf(temp, "P%d(RT=%d) ", proc[j].pid, proc[j].remaining_time);
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
        
        prev_proc = shortest;
    }
    
    export_printf("\n[✓] Total Context Switches: %d\n", context_switches);
    export_printf("[✓] All processes completed\n\n");
    
    export_printf("============================= GANTT CHART ==================================\n\n");
    export_printf("Timeline: ");
    
    // Rebuild gantt chart
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].first_run = 0;
    }
    
    current_time = 0;
    completed = 0;
    prev_proc = -1;
    
    while (completed < n) {
        int shortest = -1;
        int min_remaining = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time &&
                proc[i].remaining_time > 0 &&
                proc[i].remaining_time < min_remaining) {
                shortest = i;
                min_remaining = proc[i].remaining_time;
            }
        }
        
        if (shortest == -1) {
            sprintf(details, "IDLE | Waiting for processes to arrive");
            log_event(current_time, "CPU", 0, details);
            simulate_time_unit();  // ADD THIS
            current_time++;
            continue;
        }
        
        if (prev_proc != shortest) {
            if (prev_proc != -1) export_printf("| ");
            export_printf("P%d ", proc[shortest].pid);
        }
        
        if (!proc[shortest].first_run) {
            proc[shortest].response_time = current_time - proc[shortest].arrival_time;
            proc[shortest].first_run = 1;
        }
        
        simulate_time_unit();
        proc[shortest].remaining_time--;
        current_time++;
        prev_proc = shortest;
        
        if (proc[shortest].remaining_time == 0) {
            proc[shortest].completion_time = current_time;
            completed++;
        }
    }
    export_printf("|\n\n");
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("SRTF", proc, n, &metrics);
}
