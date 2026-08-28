# C11 Student Attendance Manager in WebAssembly

A browser-based attendance manager written in **C11**, compiled with
[Emscripten](https://emscripten.org/), and driven through an Asyncify-enabled
terminal interface. It is a deliberately local, single-user demo that shows
how a native-style C application can run in the browser while keeping its data
in the browser's IndexedDB through Emscripten's IDBFS.

## What it demonstrates

- Bounded line input and centralized validation across the C application.
- Student CRUD operations backed by a fixed-size in-memory roster.
- Attendance session marking with present/absent tracking.
- Shortage reporting below the 75% attendance threshold.
- Class-level attendance statistics with zero-session handling.
- C-to-JavaScript input and storage bridges using Emscripten Asyncify.
- Local persistence through IDBFS; no server or account is required.

## Run the checked-in demo

The compiled `index.html`, `index.js`, and `index.wasm` are checked in so the
demo can be previewed without installing the Emscripten toolchain. Serve the
repository over HTTP because WebAssembly and IndexedDB should not be opened
from a `file://` URL:

```bash
python -m http.server 8080
```

Then open <http://localhost:8080/index.html>.

## Rebuild from C source

Install and activate the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html), then make sure `emcc --version` works in the same terminal. The build scripts resolve all project paths relative to the repository; they do not depend on a particular user account or checkout directory.

On Windows:

```powershell
.\build.bat
```

On systems with GNU Make:

```bash
make
```

The build uses `www/shell.html` as the single source template and regenerates
the root `index.html`, `index.js`, and `index.wasm` artifacts.

## Architecture

- `src/main.c`: Application controller and terminal menu.
- `src/student_manager.c`: Fixed-size in-memory student registry and CSV persistence.
- `src/attendance_logic.c`: Session marking and present/absent updates.
- `src/analytics_engine.c`: Percentages, shortage filtering, and class statistics.
- `src/input_validator.c`: Bounded input bridge and validation helpers.
- `src/storage_bridge.c`: IDBFS mount, load, and synchronization bridge.
- `www/shell.html`: Canonical Emscripten HTML shell used for rebuilds.

## Scope and limitations

This is a local browser application rather than a multi-user attendance
service. Data is scoped to the browser origin, the roster has a fixed maximum
of 1,000 records, and persistence uses a simple CSV representation. There is
no server synchronization, authentication, or cross-device data sharing.
