/**
 * attendance_logic.c - Attendance Tracking Implementation
 * ========================================================
 * Handles the core attendance workflow:
 *   1. Increment sessions_total for ALL students (FR7)
 *   2. For each student, prompt P/A and increment sessions_attended if P (FR8)
 *   3. Trigger atomic persistence via storage_bridge (Story 3.4)
 *
 * Input: All user interaction goes through input_validator (Architecture).
 * Persistence: Delegates to student_save() which uses storage_bridge.
 */

#include "attendance_logic.h"
#include "student_manager.h"
#include "input_validator.h"
#include <stdio.h>
#include <ctype.h>

/**
 * Increment the total sessions held for ALL students.
 */
int attendance_increment_all_totals(void) {
    int count = student_get_count();
    if (count == 0) {
        printf("[WARN] No students registered. Cannot increment sessions.\n");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        /* Get mutable access via find — safe because we iterate by known index */
        const student_t *cs = student_get_by_index(i);
        if (cs == NULL) continue;

        student_t *s = student_find_by_reg_no(cs->reg_no);
        if (s != NULL) {
            s->sessions_total++;
        }
    }

    return 0;
}

/**
 * Mark attendance for a full session.
 * Walks through every registered student and prompts P/A.
 */
int attendance_mark_session(void) {
    int count = student_get_count();

    if (count == 0) {
        printf("\n  No students registered. Add students first.\n\n");
        return -1;
    }

    printf("\n--- Mark Attendance ---\n");
    printf("  Marking session for %d students.\n", count);
    printf("  Enter 'P' for Present or 'A' for Absent.\n\n");

    /* Step 1: Increment total sessions for everyone (FR7) */
    attendance_increment_all_totals();

    /* Step 2: For each student, prompt P/A (FR6, FR8) */
    for (int i = 0; i < count; i++) {
        const student_t *cs = student_get_by_index(i);
        if (cs == NULL) continue;

        char prompt[128];
        snprintf(prompt, sizeof(prompt),
                 "  [%d] %s (P/A): ", cs->reg_no, cs->name);

        char mark = '\0';
        if (input_read_char(prompt, "PpAa", &mark) != 0) {
            printf("[ERROR] Failed to read attendance mark.\n");
            continue;
        }

        /* Increment attended only if Present (FR8) */
        if (toupper(mark) == 'P') {
            student_t *s = student_find_by_reg_no(cs->reg_no);
            if (s != NULL) {
                s->sessions_attended++;
            }
        }
    }

    /* Step 3: Atomic persistence (Story 3.4) */
    student_save();

    printf("\n  Session recorded and saved successfully.\n\n");
    return 0;
}
