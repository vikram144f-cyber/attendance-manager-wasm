/**
 * analytics_engine.c - Shortage Analytics Implementation
 * ========================================================
 * Implements the decision-support layer:
 *   - Percentage calculation with zero-division safety (FR9)
 *   - Shortage filtering at < 75% threshold (FR10, FR11)
 *   - Class-wide summary statistics (FR12)
 *
 * Reads data from student_manager (read-only access).
 */

#include "analytics_engine.h"
#include "student_manager.h"
#include <stdio.h>

/**
 * Calculate attendance percentage for a single student.
 * Returns 0.00 if total sessions is 0 (zero-division safety).
 */
float analytics_calc_percentage(int attended, int total) {
    if (total <= 0) {
        return 0.0f;
    }
    return ((float)attended / (float)total) * 100.0f;
}

/**
 * Generate the shortage report.
 * Lists students with attendance strictly < 75%.
 */
void analytics_shortage_report(void) {
    int count = student_get_count();

    if (count == 0) {
        printf("\n  No students registered.\n\n");
        return;
    }

    printf("\n--- Shortage Report (Below %.1f%%) ---\n\n", SHORTAGE_THRESHOLD);
    printf("  %-10s %-30s %-12s\n", "RegNo", "Name", "Attendance");
    printf("  ---------- ------------------------------ ------------\n");

    int shortage_count = 0;

    for (int i = 0; i < count; i++) {
        const student_t *s = student_get_by_index(i);
        if (s == NULL) continue;

        float pct = analytics_calc_percentage(s->sessions_attended, s->sessions_total);

        if (pct < SHORTAGE_THRESHOLD) {
            printf("  %-10d %-30s %10.2f%%\n", s->reg_no, s->name, pct);
            shortage_count++;
        }
    }

    if (shortage_count == 0) {
        printf("  No students below the shortage threshold.\n");
    } else {
        printf("\n  Students in shortage: %d / %d\n", shortage_count, count);
    }
    printf("\n");
}

/**
 * Display summarized class statistics.
 */
void analytics_class_stats(void) {
    int count = student_get_count();

    if (count == 0) {
        printf("\n  No students registered.\n\n");
        return;
    }

    float total_pct = 0.0f;
    int shortage_count = 0;

    for (int i = 0; i < count; i++) {
        const student_t *s = student_get_by_index(i);
        if (s == NULL) continue;

        float pct = analytics_calc_percentage(s->sessions_attended, s->sessions_total);
        total_pct += pct;

        if (pct < SHORTAGE_THRESHOLD) {
            shortage_count++;
        }
    }

    float avg_pct = total_pct / (float)count;

    printf("\n--- Class Statistics ---\n\n");
    printf("  Total Students:       %d\n", count);
    printf("  Average Attendance:   %.2f%%\n", avg_pct);
    printf("  Students in Shortage: %d\n", shortage_count);
    printf("  Students Above 75%%:   %d\n", count - shortage_count);
    printf("\n");
}
