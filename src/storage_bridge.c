/**
 * storage_bridge.c - IDBFS Persistence Implementation
 * =====================================================
 * Architecture Boundary: This is the ONLY module allowed to touch /data.
 *
 * Uses Emscripten's EM_ASM to bridge C <-> JavaScript for:
 *   1. Mounting IDBFS to /data
 *   2. Loading persisted data from IndexedDB
 *   3. Syncing in-memory changes back to IndexedDB
 */

#include "storage_bridge.h"
#include <stdio.h>
#include <emscripten.h>

#define DATA_PATH "/data"

volatile int storage_ready = 0;

EMSCRIPTEN_KEEPALIVE
void set_storage_ready(void) {
    storage_ready = 1;
}

EM_JS(void, js_storage_init_async, (), {
    try {
        FS.mkdir('/data');
    } catch (e) {}
    FS.mount(IDBFS, {autoPersist: true}, '/data');

    FS.syncfs(true, function(err) {
        if (err) console.error('IDBFS load error:', err);
        else console.log('IDBFS: Persistent data loaded successfully.');
        Module._set_storage_ready();
    });
});

EM_JS(void, js_storage_sync_async, (), {
    FS.syncfs(false, function(err) {
        if (err) console.error('IDBFS sync error:', err);
        else console.log('IDBFS: Data synced to IndexedDB.');
        Module._set_storage_ready();
    });
});

/**
 * Initialize the persistent filesystem.
 * Mounts IDBFS to /data and populates it from IndexedDB.
 */
int storage_init(void) {
    storage_ready = 0;
    js_storage_init_async();
    while (!storage_ready) {
        emscripten_sleep(10);
    }
    printf("[STORAGE] Filesystem initialized at %s\n", DATA_PATH);
    return 0;
}

/**
 * Synchronize in-memory filesystem to IndexedDB.
 * Call after any data modification to ensure persistence.
 */
int storage_sync(void) {
    storage_ready = 0;
    js_storage_sync_async();
    while (!storage_ready) {
        emscripten_sleep(10);
    }
    printf("[STORAGE] Data synchronized to persistent storage.\n");
    return 0;
}

/**
 * Get the base path for persistent data storage.
 */
const char* storage_get_data_path(void) {
    return DATA_PATH;
}
