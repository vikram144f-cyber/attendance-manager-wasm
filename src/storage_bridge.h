/**
 * storage_bridge.h - IDBFS Persistence API
 * ==========================================
 * Architecture Boundary: This is the ONLY module allowed to touch /data.
 * All file I/O must pass through this API.
 */

#ifndef STORAGE_BRIDGE_H
#define STORAGE_BRIDGE_H

/**
 * Initialize the persistent filesystem.
 * Mounts IDBFS to /data and loads any previously saved data.
 * Must be called once at application startup.
 *
 * @return 0 on success, -1 on failure
 */
int storage_init(void);

/**
 * Synchronize in-memory filesystem to IndexedDB.
 * Triggers FS.syncfs(false, callback) to persist changes.
 * Should be called after any data modification.
 *
 * @return 0 on success, -1 on failure
 */
int storage_sync(void);

/**
 * Get the base path for persistent data storage.
 *
 * @return Pointer to the constant data directory path string
 */
const char* storage_get_data_path(void);

#endif /* STORAGE_BRIDGE_H */
