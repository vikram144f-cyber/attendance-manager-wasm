# WebAssembly Student Attendance Manager

A completely crash-proof terminal application built in **C11**, compiled to WebAssembly via **Emscripten**, and rendered in a high-fidelity **Dark Glassmorphism UI** inside the browser.

## Features
- **Uncrashable Input Firewall:** Replaces standard `scanf` with a robust `emscripten_sleep` async loop, completely protecting against buffer overflows and type-casting loops.
- **Persistent Data Storage:** Automatic auto-saving using Emscripten's `IDBFS` mapping to the browser's IndexedDB. Data survives page refreshes and browser restarts.
- **Inline Prompting:** Custom C-to-JS bridge dynamically renders instruction strings directly inside the DOM text input placeholder for an ultra-smooth typing experience.
- **Attendance Analytics:** Built-in Shortage Report calculation to automatically track and flag students below the 75% attendance threshold.

## Try it out
You can run the interactive terminal natively in your browser. Just host the directory with any local development server:
```bash
python -m http.server 8080
```
Then navigate to `http://localhost:8080/index.html`.

## Compilation
To modify the C backend and rebuild the WebAssembly binary, you'll need the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html). Run the included `build.bat` script to deploy changes:
```bash
./build.bat
```

## Architecture
- `src/main.c`: Application controller and menu UI.
- `src/student_manager.c`: In-memory student array database.
- `src/attendance_logic.c`: P/A session evaluation algorithms.
- `src/input_validator.c`: The core input firewall and Asyncify polling infrastructure.
- `src/storage_bridge.c`: Asynchronous hooks wrapping `IDBFS` syncs.
