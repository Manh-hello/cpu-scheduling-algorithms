#include "../include/scheduler.h"

// Hàm đọc dữ liệu từ file
int read_from_file(Process proc[], int *n, const char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("\n❌ Không thể mở file '%s'!\n", filename);
        return 0;
    }
    
    printf("\n📁 Đang đọc file '%s'...\n", filename);
    
    if (fscanf(file, "%d", n) != 1) {
        printf("❌ Lỗi định dạng: Không đọc được số lượng processes!\n");
        fclose(file);
        return 0;
    }
    
    if (*n < 1 || *n > MAX_PROCESSES) {
        printf("❌ Số lượng processes không hợp lệ: %d (phải từ 1-%d)\n", 
               *n, MAX_PROCESSES);
        fclose(file);
        return 0;
    }
    
    printf("   Số processes: %d\n", *n);
    
    for (int i = 0; i < *n; i++) {
        int pid, at, bt, pr;
        
        if (fscanf(file, "%d %d %d %d", &pid, &at, &bt, &pr) != 4) {
            printf("❌ Lỗi định dạng tại dòng %d!\n", i + 2);
            fclose(file);
            return 0;
        }
        
        proc[i].pid = pid;
        proc[i].arrival_time = at;
        proc[i].burst_time = bt;
        proc[i].priority = pr;
        proc[i].remaining_time = bt;
        proc[i].first_run = 0;
        
        printf("   P%d: AT=%d, BT=%d, Priority=%d\n", pid, at, bt, pr);
    }
    
    fclose(file);
    printf("✓ Đọc file thành công!\n");
    return 1;
}

// Hàm nhập dữ liệu thủ công
void input_processes(Process proc[], int *n) {
    printf("\n=== NHẬP THÔNG TIN PROCESSES ===\n");
    printf("Nhập số lượng processes (1-%d): ", MAX_PROCESSES);
    scanf("%d", n);
    
    if (*n < 1 || *n > MAX_PROCESSES) {
        printf("Số lượng không hợp lệ! Đặt mặc định = 5\n");
        *n = 5;
    }
    
    for (int i = 0; i < *n; i++) {
        proc[i].pid = i + 1;
        printf("\nProcess P%d:\n", proc[i].pid);
        printf("  Arrival Time: ");
        scanf("%d", &proc[i].arrival_time);
        printf("  Burst Time: ");
        scanf("%d", &proc[i].burst_time);
        printf("  Priority (1=cao): ");
        scanf("%d", &proc[i].priority);
        
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].first_run = 0;
    }
}

// Hàm sử dụng dữ liệu mẫu
void use_sample_data(Process proc[], int *n) {
    *n = 5;
    
    int at[] = {0, 2, 4, 6, 8};
    int bt[] = {6, 2, 8, 3, 4};
    int pr[] = {3, 1, 4, 2, 5};
    
    for (int i = 0; i < *n; i++) {
        proc[i].pid = i + 1;
        proc[i].arrival_time = at[i];
        proc[i].burst_time = bt[i];
        proc[i].priority = pr[i];
        proc[i].remaining_time = bt[i];
        proc[i].first_run = 0;
    }
    
    printf("\n✓ Đã load dữ liệu mẫu từ tài liệu!\n");
}

// In bảng process
void print_table(Process proc[], int n) {
    export_printf("\n╔════════╦════╦════╦════╦═════╦═════╦═════╗\n");
    export_printf("║ Proc   ║ AT ║ BT ║ CT ║ TAT ║ WT  ║ RT  ║\n");
    export_printf("╠════════╬════╬════╬════╬═════╬═════╬═════╣\n");
    
    for (int i = 0; i < n; i++) {
        export_printf("║ P%-6d║ %-3d║ %-3d║ %-3d║ %-4d║ %-4d║ %-4d║\n",
               proc[i].pid,
               proc[i].arrival_time,
               proc[i].burst_time,
               proc[i].completion_time,
               proc[i].turnaround_time,
               proc[i].waiting_time,
               proc[i].response_time);
    }
    
    export_printf("╚════════╩════╩════╩════╩═════╩═════╩═════╝\n");
}

// Hàm tính toán và hiển thị metrics
void calculate_metrics(Process proc[], int n, Metrics *metrics) {
    float total_wt = 0, total_tat = 0, total_rt = 0;
    int max_completion = 0;
    int total_burst = 0;
    
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
    
    print_table(proc, n);
    
    metrics->avg_turnaround = total_tat / n;
    metrics->avg_waiting = total_wt / n;
    metrics->avg_response = total_rt / n;
    metrics->total_time = max_completion;
    metrics->cpu_utilization = (float)total_burst / max_completion * 100;
    
    export_printf("\n📊 KẾT QUẢ TRUNG BÌNH:\n");
    export_printf("   Total Execution Time:    %d\n", metrics->total_time);
    export_printf("   CPU Utilization:         %.2f%%\n", metrics->cpu_utilization);
    export_printf("   Average Turnaround Time: %.2f\n", metrics->avg_turnaround);
    export_printf("   Average Waiting Time:    %.2f\n", metrics->avg_waiting);
    export_printf("   Average Response Time:   %.2f\n", metrics->avg_response);
}

// Menu chính
void print_menu() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║   CPU SCHEDULING ALGORITHMS SIMULATOR        ║\n");
    printf("║   NetBSD - Lập trình Hệ thống               ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    printf("\n");
    printf("1. FCFS (First Come First Served)\n");
    printf("2. SJF (Shortest Job First)\n");
    printf("3. SRTF (Shortest Remaining Time First)\n");
    printf("4. Round Robin\n");
    printf("5. Priority Scheduling (Non-preemptive)\n");
    printf("6. Priority Scheduling (Preemptive)\n");
    printf("7. Chạy tất cả để so sánh\n");
    printf("8. Bật/Tắt Export to File\n");
    printf("0. Thoát\n");
    printf("\n");
    if (export_enabled) {
        printf("📝 Export: BẬT\n");
    }
}
