#include "../include/scheduler.h"

// Đọc dữ liệu từ file
int read_from_file(Process proc[], int *n, const char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("\n❌ Cannot open file '%s'!\n", filename);
        return 0;
    }
    
    if (fscanf(file, "%d", n) != 1) {
        printf("❌ Format error: Cannot read number of processes!\n");
        fclose(file);
        return 0;
    }
    
    if (*n < 1 || *n > MAX_PROCESSES) {
        printf("❌ Invalid number of processes: %d (must be 1-%d)\n", 
               *n, MAX_PROCESSES);
        fclose(file);
        return 0;
    }
    
    printf("   ├─ Number of processes: %d\n", *n);
    printf("   ├─ Reading process data...\n");
    
    for (int i = 0; i < *n; i++) {
        int pid, at, bt, pr;
        
        if (fscanf(file, "%d %d %d %d", &pid, &at, &bt, &pr) != 4) {
            printf("❌ Format error at line %d!\n", i + 2);
            fclose(file);
            return 0;
        }
        
        // Validate data
        if (bt <= 0) {
            printf("❌ Invalid burst time at P%d: %d (must be > 0)\n", pid, bt);
            fclose(file);
            return 0;
        }
        if (at < 0) {
            printf("❌ Invalid arrival time at P%d: %d (must be >= 0)\n", pid, at);
            fclose(file);
            return 0;
        }
        
        proc[i].pid = pid;
        proc[i].arrival_time = at;
        proc[i].burst_time = bt;
        proc[i].priority = pr;
        proc[i].remaining_time = bt;
        proc[i].first_run = 0;
        
        printf("   │  └─ P%d: AT=%d, BT=%d, Priority=%d\n", pid, at, bt, pr);
    }
    
    fclose(file);
    return 1;
}

// In bảng input data
void print_input_table(Process proc[], int n) {
    export_printf("\n");
    export_printf("╔═══════════════════════════════════════════════════════════════╗\n");
    export_printf("║                     INPUT PROCESS DATA                        ║\n");
    export_printf("╚═══════════════════════════════════════════════════════════════╝\n");
    export_printf("\n");
    export_printf("┌─────────┬─────────────────┬─────────────┬──────────────┐\n");
    export_printf("│ Process │  Arrival Time   │ Burst Time  │   Priority   │\n");
    export_printf("├─────────┼─────────────────┼─────────────┼──────────────┤\n");
    
    for (int i = 0; i < n; i++) {
        export_printf("│   P%-4d │       %-3d       │      %-3d    │      %-3d     │\n",
               proc[i].pid,
               proc[i].arrival_time,
               proc[i].burst_time,
               proc[i].priority);
    }
    
    export_printf("└─────────┴─────────────────┴─────────────┴──────────────┘\n");
}

// In bảng kết quả
void print_table(Process proc[], int n) {
    export_printf("\n┌─────────┬─────┬─────┬─────┬──────┬──────┬──────┐\n");
    export_printf("│ Process │ AT  │ BT  │ CT  │ TAT  │  WT  │  RT  │\n");
    export_printf("├─────────┼─────┼─────┼─────┼──────┼──────┼──────┤\n");
    
    for (int i = 0; i < n; i++) {
        export_printf("│  P%-5d │ %-3d │ %-3d │ %-3d │ %-4d │ %-4d │ %-4d │\n",
               proc[i].pid,
               proc[i].arrival_time,
               proc[i].burst_time,
               proc[i].completion_time,
               proc[i].turnaround_time,
               proc[i].waiting_time,
               proc[i].response_time);
    }
    
    export_printf("└─────────┴─────┴─────┴─────┴──────┴──────┴──────┘\n");
}

// Tính toán metrics - FIXED: Tính trước khi in
void calculate_metrics(Process proc[], int n, Metrics *metrics) {
    float total_wt = 0, total_tat = 0, total_rt = 0;
    int max_completion = 0;
    int total_burst = 0;
    
    // TÍNH TOÁN TRƯỚC
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
        
        total_tat += proc[i].turnaround_time;
        total_wt += proc[i].waiting_time;
        total_rt += proc[i].response_time;
        total_burst += proc[i].burst_time;
        
        if (proc[i].completion_time > max_completion) {
            max_completion = proc[i].completion_time;
        }
    }
    
    // IN BẢNG SAU KHI TÍNH
    print_table(proc, n);
    
    // Tính metrics
    metrics->avg_turnaround = total_tat / n;
    metrics->avg_waiting = total_wt / n;
    metrics->avg_response = total_rt / n;
    metrics->total_time = max_completion;
    metrics->cpu_utilization = (float)total_burst / max_completion * 100;
    
    // In kết quả
    export_printf("\n╭─────────────────────────────────────────────────────╮\n");
    export_printf("│              📊 PERFORMANCE METRICS                 │\n");
    export_printf("├─────────────────────────────────────────────────────┤\n");
    export_printf("│  Total Execution Time    : %-6d time units       │\n", metrics->total_time);
    export_printf("│  CPU Utilization         : %-6.2f %%               │\n", metrics->cpu_utilization);
    export_printf("│  Avg Turnaround Time     : %-6.2f time units      │\n", metrics->avg_turnaround);
    export_printf("│  Avg Waiting Time        : %-6.2f time units      │\n", metrics->avg_waiting);
    export_printf("│  Avg Response Time       : %-6.2f time units      │\n", metrics->avg_response);
    export_printf("╰─────────────────────────────────────────────────────╯\n");
}
