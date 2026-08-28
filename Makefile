# ============================================================================
# Student Attendance Manager (WebAssembly Build)
# ============================================================================
# Emscripten Makefile for compiling C -> WebAssembly
# Architecture: bounded C11 input, Asyncify, and IDBFS persistence
# ============================================================================

EMCC     ?= emcc
CFLAGS   ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Wno-extra-semi
LDFLAGS  = -s ASYNCIFY=1 \
           -s ASYNCIFY_IMPORTS='["js_listen_input", "js_storage_init_async", "js_storage_sync_async"]' \
           -s FORCE_FILESYSTEM=1 \
           -s EXIT_RUNTIME=0 \
           -s ALLOW_MEMORY_GROWTH=1 \
           -lidbfs.js \
           --shell-file www/shell.html

SRC_DIR  = src
SRCS     = $(wildcard $(SRC_DIR)/*.c)
HDRS     = $(wildcard $(SRC_DIR)/*.h)
OUT_DIR  = .
TARGET   = $(OUT_DIR)/index.html

.PHONY: all clean serve

all: $(TARGET)
	@echo "✅ Build complete: index.html, index.js, index.wasm"

$(TARGET): $(SRCS) $(HDRS) www/shell.html
	$(EMCC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(OUT_DIR)/index.html $(OUT_DIR)/index.js $(OUT_DIR)/index.wasm

serve:
	emrun --port 8080 $(OUT_DIR)/index.html
