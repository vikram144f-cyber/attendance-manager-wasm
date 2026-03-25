/**
 * attendance_logic.h - Attendance Tracking API
 * ==============================================
 * Manages session marking (P/A) and tally updates.
 * Depends on student_manager for data access.
 */

#ifndef ATTENDANCE_LOGIC_H
#define ATTENDANCE_LOGIC_H

/**
 * Mark attendance for a single student.
 * Prompts for Registration Number and P/A input.
 * Increments sessions_total for ALL students.
 * Increments sessions_attended only for 'P' students.
 *
 * @return 0 on success, -1 on failure
 */
int attendance_mark_session(void);

/**
 * Increment the total sessions held for ALL students.
 * Called once per marking session.
 *
 * @return 0 on success, -1 on failure
 */
int attendance_increment_all_totals(void);

#endif /* ATTENDANCE_LOGIC_H */
