/**
 * student_manager.c - Student Registry Implementation
 * =====================================================
 * CRUD operations for student records.
 * Uses a fixed-size array (MAX_STUDENTS 1000) as defined in Architecture.
 *
 * Persistence: Delegates to storage_bridge for /data access.
 * File Format: CSV (reg_no,name,attended,total)
 */

#include "student_manager.h"
#include "storage_bridge.h"
#include <stdio.h>
#include <string.h>

/* In-memory student roster */
static student_t students[MAX_STUDENTS];
static int student_count = 0;

/**
 * Initialize the student manager.
 * Attempts to load existing data from persistent storage.
 */
int student_init(void) {
    student_count = 0;
    memset(students, 0, sizeof(students));

    /* Attempt to load from file — not finding it is OK (fresh start) */
    student_load();

    printf("[REGISTRY] Student manager initialized. %d records loaded.\n", student_count);
    return 0;
}

/**
 * Add a new student to the roster.
 */
int student_add(int reg_no, const char *name) {
    if (name == NULL) {
        printf("[ERROR] Student name cannot be empty.\n");
        return -1;
    }

    if (student_count >= MAX_STUDENTS) {
        printf("[ERROR] Student roster is full (max %d).\n", MAX_STUDENTS);
        return -1;
    }

    if (student_exists(reg_no)) {
        printf("[ERROR] Registration number %d already exists.\n", reg_no);
        return -1;
    }

    student_t *s = &students[student_count];
    s->reg_no = reg_no;
    strncpy(s->name, name, MAX_NAME_LEN - 1);
    s->name[MAX_NAME_LEN - 1] = '\0';
    s->sessions_attended = 0;
    s->sessions_total = 0;

    student_count++;
    printf("[REGISTRY] Student added: %d - %s\n", reg_no, name);
    return 0;
}

/**
 * Get the current number of registered students.
 */
int student_get_count(void) {
    return student_count;
}

/**
 * Get a pointer to a student by index.
 */
const student_t* student_get_by_index(int index) {
    if (index < 0 || index >= student_count) {
        return NULL;
    }
    return &students[index];
}

/**
 * Find a student by registration number.
 */
student_t* student_find_by_reg_no(int reg_no) {
    for (int i = 0; i < student_count; i++) {
        if (students[i].reg_no == reg_no) {
            return &students[i];
        }
    }
    return NULL;
}

/**
 * Check if a registration number already exists.
 */
bool student_exists(int reg_no) {
    return student_find_by_reg_no(reg_no) != NULL;
}

/**
 * Print all registered students to stdout.
 */
void student_list_all(void) {
    if (student_count == 0) {
        printf("\n  No students registered yet.\n\n");
        return;
    }

    printf("\n");
    printf("  %-10s %-30s %-10s %-10s %-10s\n",
           "RegNo", "Name", "Attended", "Total", "Percent");
    printf("  ---------- ------------------------------ ---------- ---------- ----------\n");

    for (int i = 0; i < student_count; i++) {
        const student_t *s = &students[i];
        float pct = 0.0f;
        if (s->sessions_total > 0) {
            pct = ((float)s->sessions_attended / (float)s->sessions_total) * 100.0f;
        }
        printf("  %-10d %-30s %-10d %-10d %9.2f%%\n",
               s->reg_no, s->name,
               s->sessions_attended, s->sessions_total, pct);
    }
    printf("\n  Total students: %d\n\n", student_count);
}

/**
 * Save all student records to persistent storage.
 * Format: CSV with header (reg_no,name,attended,total)
 */
int student_save(void) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s",
             storage_get_data_path(), STUDENT_FILE);

    FILE *fp = fopen(filepath, "w");
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] Cannot open %s for writing.\n", filepath);
        return -1;
    }

    fprintf(fp, "reg_no,name,attended,total\n");
    for (int i = 0; i < student_count; i++) {
        const student_t *s = &students[i];
        fprintf(fp, "%d,%s,%d,%d\n",
                s->reg_no, s->name,
                s->sessions_attended, s->sessions_total);
    }

    fclose(fp);

    /* Trigger IDBFS sync */
    storage_sync();

    printf("[REGISTRY] %d records saved to %s\n", student_count, filepath);
    return 0;
}

/**
 * Load student records from persistent storage.
 */
int student_load(void) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s",
             storage_get_data_path(), STUDENT_FILE);

    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        /* File not found is OK — fresh start */
        return 0;
    }

    char line[512];
    int line_num = 0;
    student_count = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_num++;

        /* Skip header line */
        if (line_num == 1) {
            continue;
        }

        /* Remove trailing newline */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (student_count >= MAX_STUDENTS) {
            fprintf(stderr, "[WARN] Max students reached during load.\n");
            break;
        }

        student_t *s = &students[student_count];
        int matched = sscanf(line, "%d,%49[^,],%d,%d",
                             &s->reg_no, s->name,
                             &s->sessions_attended, &s->sessions_total);

        if (matched == 4) {
            student_count++;
        } else {
            fprintf(stderr, "[WARN] Skipping malformed line %d: %s\n", line_num, line);
        }
    }

    fclose(fp);
    return 0;
}

/**
 * Delete a student by registration number.
 */
int student_delete(int reg_no) {
    int index = -1;
    for (int i = 0; i < student_count; i++) {
        if (students[i].reg_no == reg_no) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("[ERROR] Student %d not found.\n", reg_no);
        return -1;
    }
    
    // Shift remaining elements left
    for (int i = index; i < student_count - 1; i++) {
        students[i] = students[i + 1];
    }
    
    student_count--;
    printf("[REGISTRY] Student %d successfully deleted.\n", reg_no);
    return 0;
}

/**
 * Delete all students and clear saved data.
 */
void student_delete_all(void) {
    student_count = 0;
    memset(students, 0, sizeof(students));
    printf("[REGISTRY] All student records deleted from memory.\n");
}
