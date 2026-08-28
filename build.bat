@echo off
setlocal
pushd "%~dp0"

where emcc >nul 2>nul
if errorlevel 1 (
    echo [ERROR] emcc was not found on PATH.
    echo Activate the Emscripten SDK, then run this script again.
    echo See: https://emscripten.org/docs/getting_started/downloads.html
    popd
    exit /b 1
)

emcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Wno-extra-semi ^
    -s ASYNCIFY=1 ^
    -s ASYNCIFY_IMPORTS="['js_listen_input', 'js_storage_init_async', 'js_storage_sync_async']" ^
    -s FORCE_FILESYSTEM=1 ^
    -s EXIT_RUNTIME=0 ^
    -s ALLOW_MEMORY_GROWTH=1 ^
    -s EXPORTED_RUNTIME_METHODS="['stringToUTF8']" ^
    -lidbfs.js ^
    --shell-file "www\shell.html" ^
    src\main.c src\student_manager.c src\attendance_logic.c ^
    src\analytics_engine.c src\input_validator.c src\storage_bridge.c ^
    -o "index.html"

set "BUILD_EXIT=%ERRORLEVEL%"
popd
exit /b %BUILD_EXIT%
