#include "../include/scheduler.h"

// Round Robin
void round_robin(Process proc[], int n) {
    printf("\n🔹 Round Robin (Time Quantum = %d)\n", TIME_QUANTUM);
    
    int current_time = 0;
    int completed = 0;
    int queue[MAX_PROCESSES * 10];
    int front = 0, rear = 0;
    int in_queue[MAX_PROCESSES] = {0};
    
    // Reset remaining time
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
    }
    
    // Thêm process đầu tiên vào queue
    queue[rear++] = 0;
    in_queue[0] = 1;
    
    printf("\nGantt Chart:\n");
    
    while (completed < n) {
        if (front == rear) {
            // Queue rỗng, tìm process tiếp theo
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
        
        // Lần đầu chạy
        if (!proc[idx].first_run) {
            proc[idx].response_time = current_time - proc[idx].arrival_time;
            proc[idx].first_run = 1;
        }
        
        int exec_time = (proc[idx].remaining_time > TIME_QUANTUM) ? 
                        TIME_QUANTUM : proc[idx].remaining_time;
        
        proc[idx].remaining_time -= exec_time;
        current_time += exec_time;
        
        printf("| P%d ", proc[idx].pid);
        
        // Thêm các process mới đến vào queue
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && proc[i].remaining_time > 0 &&
                proc[i].arrival_time <= current_time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }
        
        // Nếu chưa xong, đưa lại vào queue
        if (proc[idx].remaining_time > 0) {
            queue[rear++] = idx;
        } else {
            proc[idx].completion_time = current_time;
            in_queue[idx] = 0;
            completed++;
        }
    }
    printf("|\n");
    
    calculate_metrics(proc, n);
}
