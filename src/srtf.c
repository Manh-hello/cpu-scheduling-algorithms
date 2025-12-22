#include "../include/scheduler.h"

void srtf(Process proc[], int n) {
    export_header("ALGORITHM: SRTF (Shortest Remaining Time First - Preemptive)");
    
    int current_time = 0;
    int completed = 0;
    int prev_proc = -1;
    int context_switches = 0;
    int logged_arrival[MAX_PROCESSES] = {0};
    char details[200];
    
    // Reset remaining time
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].first_run = 0;
    }
    
    export_printf("========================== ACTIVITY LOG ====================================\n\n");
    
    // Log processes arrive lúc time 0
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival_time == 0) {
            sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Entered ready queue",
                    proc[i].arrival_time, proc[i].burst_time, proc[i].priority);
            log_event_with_sim_time(0, "ARR", proc[i].pid, details);
            logged_arrival[i] = 1;
        }
    }
    
    while (completed < n) {
        int had_arrival = 0;  // Track nếu có arrival mới
        
        // Log new arrivals tại current_time
        for (int i = 0; i < n; i++) {
            if (!logged_arrival[i] && proc[i].arrival_time == current_time) {
                sprintf(details, "ARRIVED  | AT=%d, BT=%d, Priority=%d | Entered ready queue",
                        proc[i].arrival_time, proc[i].burst_time, proc[i].priority);
                log_event_with_sim_time(current_time, "ARR", proc[i].pid, details);
                logged_arrival[i] = 1;
                had_arrival = 1;
            }
        }
        
        // In ready queue ngay sau khi có arrival
        if (had_arrival) {
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
            log_queue_with_sim_time(current_time, queue_str);
        }
        
        // Tìm process có remaining time ngắn nhất
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
        
        // Nếu không có process nào ready
        if (shortest == -1) {
            sprintf(details, "IDLE | Waiting for processes to arrive");
            log_event_with_sim_time(current_time, "CPU", 0, details);
            current_time++;
            simulate_time_unit();
            continue;
        }
        
        // Context switch nếu đổi process
        if (prev_proc != shortest && prev_proc != -1) {
            context_switches++;
            sprintf(details, "CONTEXT SWITCH #%d | From P%d to P%d",
                    context_switches, proc[prev_proc].pid, proc[shortest].pid);
            log_event_with_sim_time(current_time, "CTX", proc[shortest].pid, details);
        }
        
        // Lần đầu chạy
        if (!proc[shortest].first_run) {
            proc[shortest].response_time = current_time - proc[shortest].arrival_time;
            sprintf(details, "START    | Response Time=%d, Remaining=%d | First time running",
                    proc[shortest].response_time, proc[shortest].remaining_time);
            log_event_with_sim_time(current_time, "RUN", proc[shortest].pid, details);
            proc[shortest].first_run = 1;
        } else if (prev_proc != shortest) {
            // Resume sau khi bị preempt
            sprintf(details, "RESUME   | Remaining Time=%d | Continuing execution",
                    proc[shortest].remaining_time);
            log_event_with_sim_time(current_time, "RUN", proc[shortest].pid, details);
        }
        
        // Execute one time unit
        proc[shortest].remaining_time--;
        current_time++;
        
        // Log completion hoặc progress
        if (proc[shortest].remaining_time == 0) {
            proc[shortest].completion_time = current_time;
            completed++;
            sprintf(details, "COMPLETE | CT=%d, TAT=%d, WT=%d | Finished execution",
                    proc[shortest].completion_time,
                    proc[shortest].completion_time - proc[shortest].arrival_time,
                    proc[shortest].completion_time - proc[shortest].arrival_time - proc[shortest].burst_time);
            log_event_with_sim_time(current_time, "FIN", proc[shortest].pid, details);
            
            // Show ready queue sau khi complete
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
            log_queue_with_sim_time(current_time, queue_str);
        } else {
            // Log progress occasionally
            if (proc[shortest].remaining_time % 3 == 1 || 
                proc[shortest].remaining_time == proc[shortest].burst_time - 1) {
                sprintf(details, "RUNNING  | Remaining Time=%d | Still executing",
                        proc[shortest].remaining_time);
                log_event_with_sim_time(current_time, "RUN", proc[shortest].pid, details);
            }
        }
        
        prev_proc = shortest;
        simulate_time_unit();  // Sleep sau khi log
    }
    
    export_printf("\n[✓] Total Context Switches: %d\n", context_switches);
    export_printf("[✓] All processes completed\n\n");
    
    export_printf("============================= GANTT CHART ==================================\n\n");
    export_printf("Timeline: | ");
    
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

    export_printf("Time:  ");

    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].first_run = 0;
    }
    
    current_time = 0;
    completed = 0;
    prev_proc = -1;
    
    export_printf(" %3d ", current_time);  // In thời điểm đầu
    
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
            current_time++;
            continue;
        }
        
        // CHỈ ĐẶT RESPONSE TIME, KHÔNG IN TIME Ở ĐÂY
        if (!proc[shortest].first_run) {
            proc[shortest].response_time = current_time - proc[shortest].arrival_time;
            proc[shortest].first_run = 1;
        }
        
        proc[shortest].remaining_time--;
        current_time++;
        
        // Process complete HOẶC (2) Sắp switch sang process khác
        if (proc[shortest].remaining_time == 0) {
            proc[shortest].completion_time = current_time;
            export_printf(" %3d ", current_time);  // In khi complete
            completed++;
        } else {
            // Kiểm tra xem time unit tiếp theo có bị preempt không
            int next_shortest = -1;
            int next_min_remaining = INT_MAX;
            
            for (int i = 0; i < n; i++) {
                if (proc[i].arrival_time <= current_time &&
                    proc[i].remaining_time > 0 &&
                    proc[i].remaining_time < next_min_remaining) {
                    next_shortest = i;
                    next_min_remaining = proc[i].remaining_time;
                }
            }
            
            // Nếu process khác sẽ chạy tiếp, in time hiện tại
            if (next_shortest != shortest) {
                export_printf(" %3d ", current_time);
            }
        }
        
        prev_proc = shortest;
    }
    export_printf("\n\n");
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("SRTF", proc, n, &metrics);
}
