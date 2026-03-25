/**
 * input_validator.h - Centralized Input Firewall
 * ================================================
 * Architecture Boundary: This is the ONLY module that reads stdin.
 * All logic modules receive already-validated data.
 */

#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <stdbool.h>

#define INPUT_BUFFER_SIZE 256

/**
 * Read a line of text from stdin safely.
 * Uses fgets internally - never scanf.
 *
 * @param buffer  Output buffer to store the input
 * @param size    Maximum number of characters to read
 * @return        0 on success, -1 on failure
 */
int input_read_line(const char *prompt, char *buffer, int size);

/**
 * Read and validate an integer from stdin.
 * Reprompts the user on invalid input.
 *
 * @param prompt  The prompt message to display
 * @param result  Pointer to store the validated integer
 * @return        0 on success, -1 on failure
 */
int input_read_int(const char *prompt, int *result);

/**
 * Read and validate a single character from stdin.
 * Only accepts characters in the allowed set.
 *
 * @param prompt   The prompt message to display
 * @param allowed  String of allowed characters (e.g., "PpAa")
 * @param result   Pointer to store the validated character
 * @return         0 on success, -1 on failure
 */
int input_read_char(const char *prompt, const char *allowed, char *result);

/**
 * Validate that a string is not empty and within bounds.
 *
 * @param str       The string to validate
 * @param max_len   Maximum allowed length
 * @return          true if valid, false otherwise
 */
bool input_validate_string(const char *str, int max_len);

#endif /* INPUT_VALIDATOR_H */
