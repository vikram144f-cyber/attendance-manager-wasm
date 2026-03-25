set PATH=%PATH%;%USERPROFILE%\emsdk;%USERPROFILE%\emsdk\upstream\emscripten;%USERPROFILE%\emsdk\node\16.20.0_64bit\bin
cd c:\Users\user\Desktop\DA1-BMAD
emcc -std=c11 -O2 -Wall -Wextra -s ASYNCIFY=1 -s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=0 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS="['stringToUTF8']" -lidbfs.js --shell-file www/shell.html src\main.c src\student_manager.c src\attendance_logic.c src\analytics_engine.c src\input_validator.c src\storage_bridge.c -o index.html
