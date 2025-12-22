#include "../include/scheduler.h"

int main() {
    setenv("TZ", "Asia/Ho_Chi_Minh", 1);
    tzset();
    
    Process proc[MAX_PROCESSES];
    Process proc_copy[MAX_PROCESSES];
    int n = 0;
    char output_filename[256];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    // Tạo header đẹp mắt
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                               ║\n");
    printf("║           CPU SCHEDULING ALGORITHMS SIMULATOR                 ║\n");
    printf("║                                                               ║\n");
    printf("║            NetBSD - System Programming Project                ║\n");
    printf("║                    Auto-Run Mode                              ║\n");
    printf("║                                                               ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Đọc file processes.txt
    printf("Loading data from: data/processes.txt\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    if (!read_from_file(proc, &n, "data/processes.txt")) {
        printf("\nERROR: Cannot read data/processes.txt\n");
        printf("   Please make sure the file exists in data/ directory.\n\n");
        return 1;
    }
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("Data loaded successfully! %d processes ready.\n\n", n);
    
    // Tạo tên file output với timestamp
    sprintf(output_filename, "results/CPU_Scheduling_Report_%04d%02d%02d_%02d%02d%02d.txt",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    
    printf("Output will be saved to:\n");
    printf("   %s\n\n", output_filename);
    
    // Enable export
    enable_export(output_filename);
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf(" Starting simulation with all 6 algorithms...\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
    
    // Hiển thị input data
    print_input_table(proc, n);
    
    // Chạy từng thuật toán
    printf("\n[1/6] Running FCFS(O(n))...\n");
    memcpy(proc_copy, proc, sizeof(Process) * n);
    fcfs(proc_copy, n);
    printf("       FCFS completed\n");
    
    printf("\n[2/6] Running SJF(O(n))...\n");
    memcpy(proc_copy, proc, sizeof(Process) * n);
    sjf(proc_copy, n);
    printf("       SJF completed\n");
    
    printf("\n[3/6] Running SRTF(O(n^2))...\n");
    memcpy(proc_copy, proc, sizeof(Process) * n);
    srtf(proc_copy, n);
    printf("       SRTF completed\n");
    
    printf("\n[4/6] Running Round Robin(O(n * k))...\n");
    memcpy(proc_copy, proc, sizeof(Process) * n);
    round_robin(proc_copy, n);
    printf("       Round Robin completed\n");
    
    printf("\n[5/6] Running Priority (Non-preemptive)(O(n))...\n");
    memcpy(proc_copy, proc, sizeof(Process) * n);
    priority_non_preemptive(proc_copy, n);
    printf("       Priority (Non-preemptive) completed\n");
    
    printf("\n[6/6] Running Priority (Preemptive)(O(n^2))...\n");
    memcpy(proc_copy, proc, sizeof(Process) * n);
    priority_preemptive(proc_copy, n);
    printf("       Priority (Preemptive) completed\n");
    
    // Export comparison summary
    export_comparison_summary();
    
    // Đóng export
    disable_export();
    
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ALL SIMULATIONS COMPLETED SUCCESSFULLY!\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("\nResults saved to:\n");
    printf("   %s\n", output_filename);
    printf("\nView results with:\n");
    printf("   cat %s\n", output_filename);
    printf("   less %s\n", output_filename);
    
    return 0;
}
