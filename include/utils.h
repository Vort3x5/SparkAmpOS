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
