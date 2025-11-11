#include "../include/scheduler.h"

int main() {
    Process proc[MAX_PROCESSES];
    Process proc_copy[MAX_PROCESSES];
    int n = 0;
    int choice;
    int data_loaded = 0;
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  CPU SCHEDULING ALGORITHMS - NETBSD       ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    // Chọn nguồn dữ liệu
    printf("\nChọn nguồn dữ liệu:\n");
    printf("1. Sử dụng dữ liệu mẫu (hardcoded)\n");
    printf("2. Nhập dữ liệu thủ công\n");
    printf("3. Đọc từ file\n");
    printf("Lựa chọn: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        use_sample_data(proc, &n);
        data_loaded = 1;
    } else if (choice == 2) {
        input_processes(proc, &n);
        data_loaded = 1;
    } else if (choice == 3) {
        char filename[256];
        printf("\nNhập tên file (VD: ../data/processes.txt): ");
        scanf("%s", filename);
        
        if (read_from_file(proc, &n, filename)) {
            data_loaded = 1;
        } else {
            printf("❌ Không thể đọc file. Thoát chương trình.\n");
            return 1;
        }
    }
    
    while (data_loaded) {
        print_menu();
        printf("Lựa chọn: ");
        scanf("%d", &choice);
        
        // Copy dữ liệu gốc
        memcpy(proc_copy, proc, sizeof(Process) * n);
        
        switch (choice) {
            case 1:
                fcfs(proc_copy, n);
                break;
            case 2:
                sjf(proc_copy, n);
                break;
            case 3:
                srtf(proc_copy, n);
                break;
            case 4:
                round_robin(proc_copy, n);
                break;
            case 5:
                priority_non_preemptive(proc_copy, n);
                break;
            case 6:
                priority_preemptive(proc_copy, n);
                break;
            case 7:
                printf("\n════════════════════════════════════════\n");
                printf("         SO SÁNH TẤT CẢ THUẬT TOÁN\n");
                printf("════════════════════════════════════════\n");
                
                memcpy(proc_copy, proc, sizeof(Process) * n);
                fcfs(proc_copy, n);
                
                memcpy(proc_copy, proc, sizeof(Process) * n);
                sjf(proc_copy, n);
                
                memcpy(proc_copy, proc, sizeof(Process) * n);
                srtf(proc_copy, n);
                
                memcpy(proc_copy, proc, sizeof(Process) * n);
                round_robin(proc_copy, n);
                
                memcpy(proc_copy, proc, sizeof(Process) * n);
                priority_non_preemptive(proc_copy, n);
                
                memcpy(proc_copy, proc, sizeof(Process) * n);
                priority_preemptive(proc_copy, n);
                break;
            case 0:
                printf("\n👋 Cảm ơn đã sử dụng chương trình!\n\n");
                return 0;
            default:
                printf("\n❌ Lựa chọn không hợp lệ!\n");
        }
        
        printf("\n▶ Nhấn Enter để tiếp tục...");
        getchar();
        getchar();
    }
    
    return 0;
}
