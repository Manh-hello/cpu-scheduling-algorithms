#include "../include/scheduler.h"
#include <stdarg.h>

FILE *output_file = NULL;
int export_enabled = 0;
char export_filename[256] = "";

// Lưu metrics của từng thuật toán để so sánh
typedef struct {
    char name[50];
    Metrics metrics;
} AlgorithmResult;

AlgorithmResult results[6];
int result_count = 0;

// Mở file để export
void enable_export(const char *filename) {
    // Đóng file cũ nếu có
    if (output_file != NULL) {
        fclose(output_file);
        output_file = NULL;
    }
    
    output_file = fopen(filename, "w");
    if (output_file == NULL) {
        printf("❌ Cannot create export file: %s\n", filename);
        export_enabled = 0;
        return;
    }
    export_enabled = 1;
    strcpy(export_filename, filename);
    result_count = 0;
    
    // Ghi header siêu đẹp
    time_t now = time(NULL);
    fprintf(output_file, "╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                                                                               ║\n");
    fprintf(output_file, "║               🖥️  CPU SCHEDULING ALGORITHMS - ANALYSIS REPORT  🖥️              ║\n");
    fprintf(output_file, "║                                                                               ║\n");
    fprintf(output_file, "║                         NetBSD System Programming Project                     ║\n");
    fprintf(output_file, "║                                                                               ║\n");
    fprintf(output_file, "╚═══════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
    fprintf(output_file, "📅 Report Generated: %s", ctime(&now));
    fprintf(output_file, "📂 Source File: data/processes.txt\n");
    fprintf(output_file, "⚙️  Time Quantum (Round Robin): %d\n", TIME_QUANTUM);
    fprintf(output_file, "\n");
    fprintf(output_file, "═══════════════════════════════════════════════════════════════════════════════\n");
    fprintf(output_file, "\n");
}

// Đóng file export
void disable_export() {
    if (output_file != NULL) {
        fprintf(output_file, "\n\n");
        fprintf(output_file, "╔═══════════════════════════════════════════════════════════════════════════════╗\n");
        fprintf(output_file, "║                                                                               ║\n");
        fprintf(output_file, "║                          🎉 END OF REPORT 🎉                                  ║\n");
        fprintf(output_file, "║                                                                               ║\n");
        fprintf(output_file, "║              Thank you for using CPU Scheduling Simulator!                   ║\n");
        fprintf(output_file, "║                                                                               ║\n");
        fprintf(output_file, "╚═══════════════════════════════════════════════════════════════════════════════╝\n");
        fclose(output_file);
        output_file = NULL;
    }
    export_enabled = 0;
}

// Printf đồng thời ra console và file
void export_printf(const char *format, ...) {
    va_list args1, args2;
    
    va_start(args1, format);
    vprintf(format, args1);
    va_end(args1);
    
    if (export_enabled && output_file != NULL) {
        va_start(args2, format);
        vfprintf(output_file, format, args2);
        va_end(args2);
    }
}

// Export header của thuật toán
void export_header(const char *algorithm_name) {
    export_printf("\n\n");
    export_printf("╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    export_printf("║                                                                               ║\n");
    export_printf("║   %-75s ║\n", algorithm_name);
    export_printf("║                                                                               ║\n");
    export_printf("╚═══════════════════════════════════════════════════════════════════════════════╝\n");
}

// Lưu metrics để so sánh
void export_metrics(const char *algorithm_name, Process proc[], int n, Metrics *metrics) {
    if (result_count < 6) {
        strcpy(results[result_count].name, algorithm_name);
        results[result_count].metrics = *metrics;
        result_count++;
    }
}

// Export comparison summary
void export_comparison_summary() {
    if (!export_enabled || output_file == NULL || result_count == 0) return;
    
    fprintf(output_file, "\n\n\n");
    fprintf(output_file, "╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                                                                               ║\n");
    fprintf(output_file, "║                    📊 COMPARATIVE ANALYSIS OF ALGORITHMS 📊                   ║\n");
    fprintf(output_file, "║                                                                               ║\n");
    fprintf(output_file, "╚═══════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
    
    // Bảng so sánh
    fprintf(output_file, "┌───────────────────────────────┬──────────┬──────────┬──────────┬──────────┬──────────┐\n");
    fprintf(output_file, "│         Algorithm             │   TAT    │    WT    │    RT    │   Time   │  CPU %%   │\n");
    fprintf(output_file, "├───────────────────────────────┼──────────┼──────────┼──────────┼──────────┼──────────┤\n");
    
    for (int i = 0; i < result_count; i++) {
        fprintf(output_file, "│ %-29s │  %6.2f  │  %6.2f  │  %6.2f  │    %3d   │  %6.2f  │\n",
                results[i].name,
                results[i].metrics.avg_turnaround,
                results[i].metrics.avg_waiting,
                results[i].metrics.avg_response,
                results[i].metrics.total_time,
                results[i].metrics.cpu_utilization);
    }
    
    fprintf(output_file, "└───────────────────────────────┴──────────┴──────────┴──────────┴──────────┴──────────┘\n");
    
    // Tìm thuật toán tốt nhất
    fprintf(output_file, "\n");
    fprintf(output_file, "┌───────────────────────────────────────────────────────────────────────────────┐\n");
    fprintf(output_file, "│                         🏆 BEST ALGORITHMS 🏆                                 │\n");
    fprintf(output_file, "├───────────────────────────────────────────────────────────────────────────────┤\n");
    
    // Best TAT
    int best_tat_idx = 0;
    for (int i = 1; i < result_count; i++) {
        if (results[i].metrics.avg_turnaround < results[best_tat_idx].metrics.avg_turnaround) {
            best_tat_idx = i;
        }
    }
    fprintf(output_file, "│  🥇 Lowest Avg Turnaround Time : %-30s (%.2f)      │\n",
            results[best_tat_idx].name, results[best_tat_idx].metrics.avg_turnaround);
    
    // Best WT
    int best_wt_idx = 0;
    for (int i = 1; i < result_count; i++) {
        if (results[i].metrics.avg_waiting < results[best_wt_idx].metrics.avg_waiting) {
            best_wt_idx = i;
        }
    }
    fprintf(output_file, "│  🥇 Lowest Avg Waiting Time    : %-30s (%.2f)      │\n",
            results[best_wt_idx].name, results[best_wt_idx].metrics.avg_waiting);
    
    // Best RT
    int best_rt_idx = 0;
    for (int i = 1; i < result_count; i++) {
        if (results[i].metrics.avg_response < results[best_rt_idx].metrics.avg_response) {
            best_rt_idx = i;
        }
    }
    fprintf(output_file, "│  🥇 Lowest Avg Response Time   : %-30s (%.2f)      │\n",
            results[best_rt_idx].name, results[best_rt_idx].metrics.avg_response);
    
    // Best CPU utilization
    int best_cpu_idx = 0;
    for (int i = 1; i < result_count; i++) {
        if (results[i].metrics.cpu_utilization > results[best_cpu_idx].metrics.cpu_utilization) {
            best_cpu_idx = i;
        }
    }
    fprintf(output_file, "│  🥇 Highest CPU Utilization    : %-30s (%.2f%%)    │\n",
            results[best_cpu_idx].name, results[best_cpu_idx].metrics.cpu_utilization);
    
    fprintf(output_file, "└───────────────────────────────────────────────────────────────────────────────┘\n");
    
    // Recommendations
    fprintf(output_file, "\n");
    fprintf(output_file, "┌───────────────────────────────────────────────────────────────────────────────┐\n");
    fprintf(output_file, "│                            💡 RECOMMENDATIONS 💡                              │\n");
    fprintf(output_file, "├───────────────────────────────────────────────────────────────────────────────┤\n");
    fprintf(output_file, "│                                                                               │\n");
    fprintf(output_file, "│  • For minimum average waiting time → Use: %-34s │\n", results[best_wt_idx].name);
    fprintf(output_file, "│  • For best response time        → Use: %-34s │\n", results[best_rt_idx].name);
    fprintf(output_file, "│  • For fairness & preventing starvation → Consider: Round Robin              │\n");
    fprintf(output_file, "│  • For short processes priority    → Consider: SJF or SRTF                   │\n");
    fprintf(output_file, "│  • For system with priorities      → Use: Priority Scheduling                │\n");
    fprintf(output_file, "│                                                                               │\n");
    fprintf(output_file, "└───────────────────────────────────────────────────────────────────────────────┘\n");
    
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("📊 COMPARISON SUMMARY\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("🥇 Best Avg Turnaround: %s (%.2f)\n", results[best_tat_idx].name, results[best_tat_idx].metrics.avg_turnaround);
    printf("🥇 Best Avg Waiting   : %s (%.2f)\n", results[best_wt_idx].name, results[best_wt_idx].metrics.avg_waiting);
    printf("🥇 Best Avg Response  : %s (%.2f)\n", results[best_rt_idx].name, results[best_rt_idx].metrics.avg_response);
    printf("🥇 Best CPU Usage     : %s (%.2f%%)\n", results[best_cpu_idx].name, results[best_cpu_idx].metrics.cpu_utilization);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}
