#include "../include/scheduler.h"
#include <stdarg.h>

FILE *output_file = NULL;
int export_enabled = 0;

// Mở file để export
void enable_export(const char *filename) {
    if (output_file != NULL) {
        fclose(output_file);
        output_file = NULL;
    }

    output_file = fopen(filename, "w");
    if (output_file == NULL) {
        printf("❌ Không thể tạo file export: %s\n", filename);
        export_enabled = 0;
        return;
    }
    export_enabled = 1;
    
    // Ghi header
    time_t now = time(NULL);
    fprintf(output_file, "╔════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║     CPU SCHEDULING ALGORITHMS - EXPORT REPORT            ║\n");
    fprintf(output_file, "║     NetBSD - Lập trình Hệ thống                         ║\n");
    fprintf(output_file, "╚════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\nThời gian tạo report: %s\n", ctime(&now));
    fprintf(output_file, "═══════════════════════════════════════════════════════════════\n\n");
    
    printf("✓ Export được bật: %s\n", filename);
}

// Đóng file export
void disable_export() {
    if (output_file != NULL) {
        fprintf(output_file, "\n\n═══════════════════════════════════════════════════════════════\n");
        fprintf(output_file, "                    END OF REPORT\n");
        fprintf(output_file, "═══════════════════════════════════════════════════════════════\n");
        fclose(output_file);
        output_file = NULL;
        printf("✓ Đã lưu kết quả vào file\n");
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
    export_printf("\n");
    export_printf("═══════════════════════════════════════════════════════════════\n");
    export_printf("  %s\n", algorithm_name);
    export_printf("═══════════════════════════════════════════════════════════════\n");
}

// Export bảng kết quả
void export_metrics(const char *algorithm_name, Process proc[], int n, Metrics *metrics) {
    if (!export_enabled || output_file == NULL) return;
    
    fprintf(output_file, "\n--- %s: Summary ---\n", algorithm_name);
    fprintf(output_file, "Total Execution Time: %d\n", metrics->total_time);
    fprintf(output_file, "CPU Utilization: %.2f%%\n", metrics->cpu_utilization);
    fprintf(output_file, "Average Turnaround Time: %.2f\n", metrics->avg_turnaround);
    fprintf(output_file, "Average Waiting Time: %.2f\n", metrics->avg_waiting);
    fprintf(output_file, "Average Response Time: %.2f\n", metrics->avg_response);
    fprintf(output_file, "\n");
}

// Export Gantt chart
void export_gantt_chart(const char *chart) {
    if (!export_enabled || output_file == NULL) return;
    fprintf(output_file, "\nGantt Chart:\n%s\n", chart);
}
