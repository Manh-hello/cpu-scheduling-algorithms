#include "../include/scheduler.h"

// SRTF - Shortest Remaining Time First
void srtf(Process proc[], int n) {
    export_header("🔹 SRTF (Shortest Remaining Time First)");
    
    int current_time = 0;
    int completed = 0;
    int prev_proc = -1;
    char gantt[MAX_GANTT_LENGTH] = "";
    char buffer[50];
    
    // Reset remaining time
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
    }
    
    export_printf("\nGantt Chart:\n");
    
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
            current_time++;
            continue;
        }
        
        // Lần đầu chạy
        if (!proc[shortest].first_run) {
            proc[shortest].response_time = current_time - proc[shortest].arrival_time;
            proc[shortest].first_run = 1;
        }
        
        // In Gantt chart khi đổi process
        if (prev_proc != shortest) {
            if (prev_proc != -1) {
                strcat(gantt, "| ");
            }
            sprintf(buffer, "P%d ", proc[shortest].pid);
            strcat(gantt, buffer);
        }
        
        proc[shortest].remaining_time--;
        current_time++;
        prev_proc = shortest;
        
        if (proc[shortest].remaining_time == 0) {
            proc[shortest].completion_time = current_time;
            completed++;
        }
    }
    
    strcat(gantt, "|");
    export_printf("%s\n", gantt);
    
    Metrics metrics;
    calculate_metrics(proc, n, &metrics);
    export_metrics("SRTF", proc, n, &metrics);
}
