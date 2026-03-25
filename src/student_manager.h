/**
 * student_manager.h - Student Registry API
 * ==========================================
 * Manages the in-memory student roster (CRUD operations).
 * Data Model: Fixed-Size Struct Array (MAX_STUDENTS 1000).
 */

#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include <stdbool.h>

#define MAX_STUDENTS    1000
#define MAX_NAME_LEN    50
#define STUDENT_FILE    "students.csv"

/**
 * Student record structure.
 * Fields: reg_no, name, sessions_attended, sessions_total.
 */
typedef struct {
    int  reg_no;
    char name[MAX_NAME_LEN];
    int  sessions_attended;
    int  sessions_total;
} student_t;

/**
 * Initialize the student manager.
 * Loads existing records from persistent storage if available.
 *
 * @return 0 on success, -1 on failure
 */
int student_init(void);

/**
 * Add a new student to the roster.
 *
 * @param reg_no  Registration number (must be unique)
 * @param name    Student name (max MAX_NAME_LEN chars)
 * @return        0 on success, -1 on failure (duplicate or full)
 */
int student_add(int reg_no, const char *name);

/**
 * Get the current number of registered students.
 *
 * @return Count of students in the roster
 */
int student_get_count(void);

/**
 * Get a pointer to a student by index.
 *
 * @param index  Array index (0 to count-1)
 * @return       Pointer to student, or NULL if index is invalid
 */
const student_t* student_get_by_index(int index);

/**
 * Find a student by registration number.
 *
 * @param reg_no  Registration number to search for
 * @return        Pointer to student, or NULL if not found
 */
student_t* student_find_by_reg_no(int reg_no);

/**
 * Check if a registration number already exists.
 *
 * @param reg_no  Registration number to check
 * @return        true if exists, false otherwise
 */
bool student_exists(int reg_no);

/**
 * Print all registered students to stdout.
 * Displays a formatted table of RegNo and Name.
 */
void student_list_all(void);

/**
 * Save all student records to persistent storage.
 * Writes to /data/students.csv via the storage bridge.
 *
 * @return 0 on success, -1 on failure
 */
int student_save(void);

/**
 * Load student records from persistent storage.
 * Reads from /data/students.csv.
 *
 * @return 0 on success, -1 on failure (file not found is not an error)
 */
int student_load(void);

/**
 * Delete a student by registration number.
 */
int student_delete(int reg_no);

/**
 * Delete all students from the roster.
 */
void student_delete_all(void);

#endif /* STUDENT_MANAGER_H */
