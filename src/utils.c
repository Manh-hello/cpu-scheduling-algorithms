#include "../include/scheduler.h"

int read_from_file(Process proc[], int *n, const char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("\nCannot open file '%s'!\n", filename);
        return 0;
    }
    
    if (fscanf(file, "%d", n) != 1) {
        printf("Format error: Cannot read number of processes!\n");
        fclose(file);
        return 0;
    }
    
    if (*n < 1 || *n > MAX_PROCESSES) {
        printf("Invalid number of processes: %d (must be 1-%d)\n", 
               *n, MAX_PROCESSES);
        fclose(file);
        return 0;
    }
    
    printf("   ├─ Number of processes: %d\n", *n);
    printf("   ├─ Reading process data...\n");
    
    for (int i = 0; i < *n; i++) {
        int pid, at, bt, pr;
        
        if (fscanf(file, "%d %d %d %d", &pid, &at, &bt, &pr) != 4) {
            printf("Format error at line %d!\n", i + 2);
            fclose(file);
            return 0;
        }
        
        if (bt <= 0) {
            printf("Invalid burst time at P%d: %d\n", pid, bt);
            fclose(file);
            return 0;
        }
        if (at < 0) {
            printf("Invalid arrival time at P%d: %d\n", pid, at);
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

void print_input_table(Process proc[], int n) {
    export_printf("\n");
    export_printf("+----------------------- INPUT PROCESS DATA -------------------+\n");
    export_printf("| PID | Arrival Time | Burst Time | Priority | State           |\n");
    export_printf("+-----+--------------+------------+----------+-----------------+\n");
    
    for (int i = 0; i < n; i++) {
        export_printf("| P%-3d|      %-3d     |     %-3d    |    %-2d    | READY           |\n",
               proc[i].pid,
               proc[i].arrival_time,
               proc[i].burst_time,
               proc[i].priority);
    }
    
    export_printf("+-----+--------------+------------+----------+-----------------+\n");
    export_printf("\n");
}

void print_table(Process proc[], int n) {
    export_printf("\n");
    export_printf("+-------------------------------------------------------------+\n");
    export_printf("| PID | AT  | BT  | CT  | TAT | WT  | RT  | Status            |\n");
    export_printf("+-----+-----+-----+-----+-----+-----+-----+-------------------+\n");
    
    for (int i = 0; i < n; i++) {
        export_printf("| P%-3d| %-4d| %-4d| %-4d| %-4d| %-4d| %-4d| COMPLETED         |\n",
               proc[i].pid,
               proc[i].arrival_time,
               proc[i].burst_time,
               proc[i].completion_time,
               proc[i].turnaround_time,
               proc[i].waiting_time,
               proc[i].response_time);
    }
    
    export_printf("+-----+-----+-----+-----+-----+-----+-----+-------------------+\n");
}

void calculate_metrics(Process proc[], int n, Metrics *metrics) {
    float total_wt = 0, total_tat = 0, total_rt = 0;
    int max_completion = 0;
    int total_burst = 0;
    
    // Calculate first
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
    
    // Print table
    print_table(proc, n);
    
    // Calculate metrics
    metrics->avg_turnaround = total_tat / n;
    metrics->avg_waiting = total_wt / n;
    metrics->avg_response = total_rt / n;
    metrics->total_time = max_completion;
    metrics->cpu_utilization = (float)total_burst / max_completion * 100;
    
    // Print metrics
    export_printf("\n");
    export_printf("╭─────────────────────────────────────────────────────╮\n");
    export_printf("│                 PERFORMANCE METRICS                 │\n");
    export_printf("├─────────────────────────────────────────────────────┤\n");
    export_printf("│  Total Execution Time    : %-6d time units        │\n", metrics->total_time);
    export_printf("│  CPU Utilization         : %-6.2f %%                 │\n", metrics->cpu_utilization);
    export_printf("│  Avg Turnaround Time     : %-6.2f time units        │\n", metrics->avg_turnaround);
    export_printf("│  Avg Waiting Time        : %-6.2f time units        │\n", metrics->avg_waiting);
    export_printf("│  Avg Response Time       : %-6.2f time units        │\n", metrics->avg_response);
    export_printf("╰─────────────────────────────────────────────────────╯\n");
}

void sim_time_to_string(int sim_time, char *buffer) {
    int hours = sim_time / 3600;
    int minutes = (sim_time % 3600) / 60;
    int seconds = sim_time % 60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
}

// Log event với simulation time as HH:MM:SS
void log_event_with_sim_time(int sim_time, const char *event_type, int pid, const char *details) {
    char time_str[16];
    sim_time_to_string(sim_time, time_str);
    
    if (pid > 0) {
        export_printf("[%s] [%s] P%-2d | %s\n", 
                     time_str, event_type, pid, details);
    } else {
        export_printf("[%s] [%s]     | %s\n",
                     time_str, event_type, details);
    }
}

// Log queue với simulation time as HH:MM:SS
void log_queue_with_sim_time(int sim_time, const char *queue_content) {
    char time_str[16];
    sim_time_to_string(sim_time, time_str);
    
    export_printf("[%s] [Queue] %s\n", time_str, queue_content);
}
