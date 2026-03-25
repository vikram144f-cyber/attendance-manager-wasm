/**
 * DA1-BMAD - Student Attendance Manager
 * ======================================
 * Entry point for the WebAssembly application.
 *
 * Architecture: C11 -> Emscripten -> WASM
 * Persistence: IDBFS (autoPersist)
 * IO: ASYNCIFY-enabled terminal-style interaction
 */

#include <stdio.h>
#include <stdlib.h>
#include "storage_bridge.h"
#include "input_validator.h"
#include "student_manager.h"
#include "attendance_logic.h"
#include "analytics_engine.h"

/* Forward declarations for menu handlers */
static void handle_add_student(void);
static void handle_view_students(void);
static void handle_search_student(void);
static void handle_delete_student(void);
static void handle_delete_all(void);

int main(void) {
    printf("========================================\n");
    printf("  DA1-BMAD: Student Attendance Manager\n");
    printf("  Compiled to WebAssembly via Emscripten\n");
    printf("========================================\n\n");

    /* Initialize persistent filesystem */
    if (storage_init() != 0) {
        fprintf(stderr, "[FATAL] Failed to initialize storage.\n");
        return 1;
    }

    /* Initialize student registry (loads from file if available) */
    if (student_init() != 0) {
        fprintf(stderr, "[FATAL] Failed to initialize student manager.\n");
        return 1;
    }

    printf("\nSystem ready. Select a menu option.\n\n");

    int choice = 0;
    int running = 1;

    while (running) {
        printf("\n\n=== MAIN MENU ===\n");
        printf("What would you like to do?\n");
        printf("  1. Add New Student Record\n");
        printf("  2. Mark Daily Attendance (P/A)\n");
        printf("  3. Search Student by Registration Number\n");
        printf("    ----------\n");
        printf("  4. View Attendance Shortage (< 75%%)\n");
        printf("  5. View Overall Class Summary\n");
        printf("  6. Class Statistic\n");
        printf("    ----------\n");
        printf("  7. Delete Student Record\n");
        printf("  8. Delete All Saved Data\n");
        printf("  9. Exit Menu\n\n");

        if (input_read_int("Please type a number (1-9) and press Enter > ", &choice) != 0) {
            printf("\n[ERROR] Failed to read input.\n");
            continue;
        }

        switch (choice) {
            case 1:
                handle_add_student();
                break;
            case 2:
                attendance_mark_session();
                break;
            case 3:
                handle_search_student();
                break;
            case 4:
                analytics_shortage_report();
                break;
            case 5:
                handle_view_students();
                break;
            case 6:
                analytics_class_stats();
                break;
            case 7:
                handle_delete_student();
                break;
            case 8:
                handle_delete_all();
                break;
            case 9:
                printf("\nSaving data before exit...\n");
                student_save();
                printf("Session Saved. You can now close this tab.\n");
                running = 0;
                break;
            default:
                printf("[ERROR] Invalid option '%d'. Please enter 1-9.\n\n", choice);
                break;
        }
    }

    return 0;
}

/**
 * Handle "Add Student" menu option.
 * Prompts for RegNo and Name, validates, and adds to the roster.
 */
static void handle_add_student(void) {
    int reg_no = 0;
    char name[MAX_NAME_LEN];

    printf("\n=== Add a New Student ===\n");
    printf("Let's add a new student to your class roster.\n\n");

    if (input_read_int("Step 1/2: Enter their Registration Number (e.g., 101) > ", &reg_no) != 0) {
        printf("\n[ERROR] Failed to read registration number.\n");
        return;
    }

    if (input_read_line("Step 2/2: Enter their full Name > ", name, MAX_NAME_LEN) != 0) {
        printf("\n[ERROR] Failed to read student name.\n");
        return;
    }

    if (!input_validate_string(name, MAX_NAME_LEN - 1)) {
        printf("[ERROR] Name cannot be empty or exceeds maximum length.\n");
        return;
    }

    if (student_add(reg_no, name) == 0) {
        /* Auto-save after successful add */
        student_save();
    }

    printf("\n");
}

/**
 * Handle "View All Students" menu option.
 */
static void handle_view_students(void) {
    printf("\n--- Student Roster ---");
    student_list_all();
}

/**
 * Handle "Search Student"
 */
static void handle_search_student(void) {
    int reg_no = 0;
    
    printf("\n=== Search Student ===\n");
    if (input_read_int("Enter Registration Number to search (e.g., 101) > ", &reg_no) != 0) return;
    
    student_t *s = student_find_by_reg_no(reg_no);
    if (!s) {
        printf("\n[ERROR] Student %d not found in the registry.\n", reg_no);
        return;
    }
    
    float pct = s->sessions_total > 0 ? ((float)s->sessions_attended / s->sessions_total) * 100.0f : 0.0f;
    printf("\n--- Student Found ---\n");
    printf("Registration No: %d\n", s->reg_no);
    printf("Name           : %s\n", s->name);
    printf("Attendance     : %d/%d (%.2f%%)\n", s->sessions_attended, s->sessions_total, pct);
    printf("---------------------\n");
}

/**
 * Handle "Delete Student"
 */
static void handle_delete_student(void) {
    int reg_no = 0;
    char confirm;
    
    printf("\n=== Delete Student Record ===\n");
    if (input_read_int("Enter Registration Number to delete (e.g., 101) > ", &reg_no) != 0) return;
    
    if (!student_exists(reg_no)) {
        printf("\n[ERROR] Student %d not found in the registry.\n", reg_no);
        return;
    }
    
    printf("WARNING: This will permanently remove the student's records.\n");
    if (input_read_char("Are you sure you want to delete them? (Y/N) > ", "YyNn", &confirm) != 0) return;
    if (confirm == 'Y' || confirm == 'y') {
        student_delete(reg_no);
        student_save();
    } else {
        printf("\nDeletion cancelled.\n");
    }
}

/**
 * Handle "Delete All"
 */
static void handle_delete_all(void) {
    char confirm;
    
    printf("\n=== Clear ALL Databases ===\n");
    printf("WARNING: This will permanently delete ALL students and attendance records!\n");
    if (input_read_char("Are you absolutely sure? Type 'Y' to clear everything > ", "YyNn", &confirm) != 0) return;
    if (confirm == 'Y' || confirm == 'y') {
        student_delete_all();
        student_save();
        printf("\nDatabase cleared successfully.\n");
    } else {
        printf("\nClear database cancelled.\n");
    }
}
