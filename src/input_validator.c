/**
 * input_validator.c - Centralized Input Firewall Implementation
 * ==============================================================
 * Architecture Boundary: This is the ONLY module that reads stdin.
 * Uses fgets() exclusively - never scanf().
 *
 * All validation happens here so that logic modules receive
 * clean, type-safe data. This is the "Crash-Proof" guarantee.
 */

#include "input_validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <emscripten.h>

volatile int input_ready = 0;
char global_input_buffer[INPUT_BUFFER_SIZE];

EMSCRIPTEN_KEEPALIVE
void set_input_ready(void) {
    input_ready = 1;
}

EMSCRIPTEN_KEEPALIVE
char* get_input_buffer_ptr(void) {
    return global_input_buffer;
}

EM_JS(void, js_listen_input, (const char* prompt_str), {
    var prompt = UTF8ToString(prompt_str);
    var inputEl = document.getElementById('input');
    inputEl.placeholder = prompt;
    inputEl.focus();

    Module.inputResolve = function(text) {
        inputEl.placeholder = '';
        var p = Module._get_input_buffer_ptr();
        stringToUTF8(text, p, 256);
        Module._set_input_ready();
    };
});

/**
 * Read a line of text from JS input safely via Asyncify.
 */
int input_read_line(const char* prompt, char *buffer, int size) {
    if (buffer == NULL || size <= 0) {
        return -1;
    }

    input_ready = 0;
    js_listen_input(prompt);
    while (!input_ready) {
        emscripten_sleep(50);
    }

    strncpy(buffer, global_input_buffer, size - 1);
    buffer[size - 1] = '\0';

    /* Remove trailing newline if JS sent one, though it shouldn't */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    return 0;
}

/**
 * Read and validate an integer from stdin.
 * Reprompts the user on invalid input.
 */
int input_read_int(const char *prompt, int *result) {
    char buffer[INPUT_BUFFER_SIZE];
    char *endptr;

    if (result == NULL) {
        return -1;
    }

    while (1) {
        if (input_read_line(prompt, buffer, INPUT_BUFFER_SIZE) != 0) {
            return -1;
        }

        /* Check for empty input */
        if (strlen(buffer) == 0) {
            printf("[ERROR] Input cannot be empty. Please try again.\n");
            continue;
        }

        long val = strtol(buffer, &endptr, 10);

        /* Check if entire string was consumed */
        if (*endptr != '\0') {
            printf("[ERROR] Invalid number: '%s'. Please enter a valid integer.\n", buffer);
            continue;
        }

        *result = (int)val;
        return 0;
    }
}

/**
 * Read and validate a single character from stdin.
 * Only accepts characters in the allowed set.
 */
int input_read_char(const char *prompt, const char *allowed, char *result) {
    char buffer[INPUT_BUFFER_SIZE];

    if (result == NULL || allowed == NULL) {
        return -1;
    }

    while (1) {
        if (input_read_line(prompt, buffer, INPUT_BUFFER_SIZE) != 0) {
            return -1;
        }

        /* Must be exactly one character */
        if (strlen(buffer) != 1) {
            printf("[ERROR] Please enter exactly one character.\n");
            continue;
        }

        /* Check if character is in allowed set */
        if (strchr(allowed, buffer[0]) != NULL) {
            *result = buffer[0];
            return 0;
        }

        printf("[ERROR] Invalid input '%c'. Allowed: %s\n", buffer[0], allowed);
    }
}

/**
 * Validate that a string is not empty and within bounds.
 */
bool input_validate_string(const char *str, int max_len) {
    if (str == NULL) {
        return false;
    }

    int len = (int)strlen(str);
    return (len > 0 && len <= max_len);
}
