#pragma once

#include <windows.h>
#include <string>

// Structure to hold coordinates and size
struct Rect {
    int x;
    int y;
    int width;
    int height;
};

// Color structure
struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a; // alpha channel (0-255)
};

// Generates a COLORREF from our Color struct
inline COLORREF ToColorRef(const Color& c) {
    return RGB(c.r, c.g, c.b);
}

// Emulate left mouse click
void EmulateLeftClick();

// Emulate right mouse click
void EmulateRightClick();

// Move cursor to specific screen coordinates
void MoveCursorTo(int x, int y);
