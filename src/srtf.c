#include "../include/scheduler.h"

// SRTF - Shortest Remaining Time First
void srtf(Process proc[], int n) {
    printf("\n🔹 SRTF (Shortest Remaining Time First)\n");
    
    int current_time = 0;
    int completed = 0;
    int prev_proc = -1;
    
    // Reset remaining time
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
    }
    
    printf("\nGantt Chart:\n");
    
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
            if (prev_proc != -1) printf("| ");
            printf("P%d ", proc[shortest].pid);
        }
        
        proc[shortest].remaining_time--;
        current_time++;
        prev_proc = shortest;
        
        if (proc[shortest].remaining_time == 0) {
            proc[shortest].completion_time = current_time;
            completed++;
        }
    }
    printf("|\n");
    
    calculate_metrics(proc, n);
}
