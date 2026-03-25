# ============================================================================
# DA1-BMAD - Student Attendance Manager (WASM Build)
# ============================================================================
# Emscripten Makefile for compiling C -> WebAssembly
# Architecture: Industrial "Crash-Proof" Foundation (IDBFS + autoPersist)
# ============================================================================

CC       = emcc
CFLAGS   = -std=c11 -O2 -Wall -Wextra
LDFLAGS  = -s ASYNCIFY=1 \
           -s ASYNCIFY_IMPORTS="['js_storage_init', 'js_storage_sync', 'get_js_input']" \
           -s FORCE_FILESYSTEM=1 \
           -s EXIT_RUNTIME=0 \
           -s ALLOW_MEMORY_GROWTH=1 \
           -lidbfs.js \
           --shell-file www/shell.html

SRC_DIR  = src
SRCS     = $(wildcard $(SRC_DIR)/*.c)
OUT_DIR  = .
TARGET   = $(OUT_DIR)/index.html

.PHONY: all clean serve

all: $(TARGET)
	@echo "✅ Build complete: index.html, index.js, index.wasm"

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(OUT_DIR)/index.html $(OUT_DIR)/index.js $(OUT_DIR)/index.wasm

serve:
	emrun --port 8080 $(OUT_DIR)/index.html
