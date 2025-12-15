#include "../include/scheduler.h"
#include <stdarg.h>

FILE *output_file = NULL;
int export_enabled = 0;
char export_filename[256] = "";

typedef struct {
    char name[50];
    Metrics metrics;
} AlgorithmResult;

AlgorithmResult results[6];
int result_count = 0;

void enable_export(const char *filename) {
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
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    fprintf(output_file, "════════════════════════════════════════════════════════════════════════════════\n");
    fprintf(output_file, "\n");
    fprintf(output_file, "           >>> CPU SCHEDULING ALGORITHMS SIMULATOR <<<\n");
    fprintf(output_file, "                  NetBSD - System Programming\n");
    fprintf(output_file, "\n");
    fprintf(output_file, "════════════════════════════════════════════════════════════════════════════════\n");
    fprintf(output_file, "\n");
    fprintf(output_file, "+------------------------ SYSTEM CONFIGURATION -------------------------+\n");
    fprintf(output_file, "| Start Time       : %04d-%02d-%02d %02d:%02d:%02d                              |\n",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    fprintf(output_file, "| Input File       : data/processes.txt                                 |\n");
    fprintf(output_file, "| Time Quantum     : %d                                                  |\n", TIME_QUANTUM);
    fprintf(output_file, "| Total Algorithms : 6                                                  |\n");
    fprintf(output_file, "+-----------------------------------------------------------------------+\n");
    fprintf(output_file, "\n");
}

void disable_export() {
    if (output_file != NULL) {
        fprintf(output_file, "\n\n");
        fprintf(output_file, "════════════════════════════════ SIMULATION END ═══════════════════════════════\n");
        fprintf(output_file, "\n");
        fprintf(output_file, "[✓] All algorithms completed successfully\n");
        fprintf(output_file, "[✓] Report saved to: %s\n", export_filename);
        fprintf(output_file, "\n");
        fprintf(output_file, "════════════════════════════════════════════════════════════════════════════════\n");
        fclose(output_file);
        output_file = NULL;
        printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("📊 COMPARISON SUMMARY\n");
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        if (result_count > 0) {
            int best_tat = 0, best_wt = 0, best_rt = 0;
            for (int i = 1; i < result_count; i++) {
                if (results[i].metrics.avg_turnaround < results[best_tat].metrics.avg_turnaround) best_tat = i;
                if (results[i].metrics.avg_waiting < results[best_wt].metrics.avg_waiting) best_wt = i;
                if (results[i].metrics.avg_response < results[best_rt].metrics.avg_response) best_rt = i;
            }
            printf("🥇 Best Avg Turnaround: %s (%.2f)\n", results[best_tat].name, results[best_tat].metrics.avg_turnaround);
            printf("🥇 Best Avg Waiting   : %s (%.2f)\n", results[best_wt].name, results[best_wt].metrics.avg_waiting);
            printf("🥇 Best Avg Response  : %s (%.2f)\n", results[best_rt].name, results[best_rt].metrics.avg_response);
        }
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    }
    export_enabled = 0;
}

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

void export_header(const char *algorithm_name) {
    export_printf("\n");
    export_printf("════════════════════════════════════════════════════════════════════════════════\n");
    export_printf("\n");
    export_printf("                         %s\n", algorithm_name);
    export_printf("\n");
    export_printf("════════════════════════════════════════════════════════════════════════════════\n");
    export_printf("\n");
}

// Log chi tiết từng event - VỪA console VỪA file
void log_event(int time, const char *event_type, int pid, const char *details) {
    if (pid > 0) {
        export_printf("[Time:%3d] [%s] P%-2d | %s\n", time, event_type, pid, details);
    } else {
        export_printf("[Time:%3d] [%s]     | %s\n", time, event_type, details);
    }
}

// Log queue status - VỪA console VỪA file
void log_queue(int time, const char *queue_content) {
    export_printf("[Time:%3d] [Queue] %s\n", time, queue_content);
}

void export_metrics(const char *algorithm_name, Process proc[], int n, Metrics *metrics) {
    (void)proc;  // Unused parameter
    (void)n;     // Unused parameter
    
    if (result_count < 6) {
        strcpy(results[result_count].name, algorithm_name);
        results[result_count].metrics = *metrics;
        result_count++;
    }
}

void export_comparison_summary() {
    if (!export_enabled || output_file == NULL || result_count == 0) return;
    
    fprintf(output_file, "\n\n");
    fprintf(output_file, "+----------------------------- FINAL STATISTICS -----------------------------+\n");
    fprintf(output_file, "|\n");
    fprintf(output_file, "| %-30s | %8s | %8s | %8s | %8s |\n", 
            "Algorithm", "Avg TAT", "Avg WT", "Avg RT", "CPU %%");
    fprintf(output_file, "|%-30s-|----------|----------|----------|----------|\n", "------------------------------");
    
    for (int i = 0; i < result_count; i++) {
        fprintf(output_file, "| %-30s | %8.2f | %8.2f | %8.2f | %7.2f%% |\n",
                results[i].name,
                results[i].metrics.avg_turnaround,
                results[i].metrics.avg_waiting,
                results[i].metrics.avg_response,
                results[i].metrics.cpu_utilization);
    }
    
    fprintf(output_file, "|\n");
    fprintf(output_file, "+----------------------------------------------------------------------------+\n");
    
    // Find best
    int best_tat = 0, best_wt = 0, best_rt = 0;
    for (int i = 1; i < result_count; i++) {
        if (results[i].metrics.avg_turnaround < results[best_tat].metrics.avg_turnaround) best_tat = i;
        if (results[i].metrics.avg_waiting < results[best_wt].metrics.avg_waiting) best_wt = i;
        if (results[i].metrics.avg_response < results[best_rt].metrics.avg_response) best_rt = i;
    }
    
    fprintf(output_file, "\n");
    fprintf(output_file, "+--------------------------- BEST PERFORMERS --------------------------------+\n");
    fprintf(output_file, "| [★] Best Avg Turnaround Time : %-30s (%.2f) |\n", 
            results[best_tat].name, results[best_tat].metrics.avg_turnaround);
    fprintf(output_file, "| [★] Best Avg Waiting Time    : %-30s (%.2f) |\n",
            results[best_wt].name, results[best_wt].metrics.avg_waiting);
    fprintf(output_file, "| [★] Best Avg Response Time   : %-30s (%.2f) |\n",
            results[best_rt].name, results[best_rt].metrics.avg_response);
    fprintf(output_file, "+----------------------------------------------------------------------------+\n");
}
