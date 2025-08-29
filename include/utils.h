#pragma once

#include <video.h>
#include <interrupts.h>

#define BRK() do { \
    Print("Breakpoint!", YELLOW); \
    _Halt(); \
} while (0)

#define FAILED(msg) do { \
    Print(msg, RED); \
    _Halt(); \
} while (0)

#define MAX(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; \
})

#define MIN(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; \
})
