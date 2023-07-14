#!/bin/sh
# Debug, ReleaseSafe, ReleaseSmall, ReleaseFast are the options for -O
zig build-exe ../src/main.zig -lc -lSDL2 -O ReleaseSafe -flto -fstrip
