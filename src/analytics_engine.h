/**
 * analytics_engine.h - Shortage Analytics API
 * ==============================================
 * Computes attendance percentages, filters shortage students,
 * and generates class-wide statistics.
 */

#ifndef ANALYTICS_ENGINE_H
#define ANALYTICS_ENGINE_H

#define SHORTAGE_THRESHOLD 75.0f

/**
 * Calculate attendance percentage for a single student.
 * Handles zero-division safely (returns 0.0).
 *
 * @param attended  Number of sessions attended
 * @param total     Total number of sessions held
 * @return          Percentage (0.00 - 100.00)
 */
float analytics_calc_percentage(int attended, int total);

/**
 * Generate the shortage report.
 * Lists only students with attendance strictly < 75%.
 * Displays RegNo, Name, and Percentage. (FR10, FR11)
 */
void analytics_shortage_report(void);

/**
 * Display summarized class statistics.
 * Shows total student count, average attendance, and shortage count. (FR12)
 */
void analytics_class_stats(void);

#endif /* ANALYTICS_ENGINE_H */
