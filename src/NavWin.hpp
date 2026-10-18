#pragma once

#include "utils.hpp"
#include <windows.h>
#include <string>

class NavWin {
public:
    NavWin();
    ~NavWin();

    // Delete copy and move constructors
    NavWin(const NavWin&) = delete;
    NavWin& operator=(const NavWin&) = delete;
    NavWin(NavWin&&) = delete;
    NavWin& operator=(NavWin&&) = delete;

    bool Initialize(HINSTANCE hInstance, WNDPROC wndProc, void* userData = nullptr);

    // Draw primitives
    void DrawLine(int x1, int y1, int x2, int y2, int thickness, const Color& color);
    void DrawText(const std::string& text, int x, int y, const std::string& fontName, int fontSize, const Color& color);
    void DrawRect(int x, int y, int width, int height, int thickness, const Color& color);
    void DrawFilledRect(int x, int y, int width, int height, const Color& color);

    // Operations
    void Clear();
    void Render(); // For double buffering if needed, or applying updates

    Rect GetWindowSize() const;
    void UpdateSize();

    void Show();
    void Hide();
    bool IsVisible() const;

    HWND GetHWND() const { return m_hwnd; }

private:
    void InitGraphics();
    void CleanupGraphics();
    void UpdateLayeredWindow();

    HWND m_hwnd;
    HINSTANCE m_hInstance;
    bool m_isVisible;

    // GDI resources for layered window drawing
    HDC m_screenDC;
    HDC m_memDC;
    HBITMAP m_memBitmap;
    HBITMAP m_oldBitmap;
    int m_width;
    int m_height;

    // Default alpha for the whole window or handled per pixel?
    // Let's use pre-multiplied alpha or simple UpdateLayeredWindow.
};
