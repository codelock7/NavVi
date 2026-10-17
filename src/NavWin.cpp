#include "NavWin.hpp"
#include <stdexcept>
#include <iostream>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

namespace {
    ULONG_PTR g_gdiplusToken = 0;
    int g_navWinCount = 0;
}

NavWin::NavWin()
    : m_hwnd(NULL), m_hInstance(NULL), m_isVisible(false),
      m_screenDC(NULL), m_memDC(NULL), m_memBitmap(NULL), m_oldBitmap(NULL),
      m_width(0), m_height(0)
{
    if (g_navWinCount == 0) {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);
    }
    g_navWinCount++;
}

NavWin::~NavWin() {
    CleanupGraphics();
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
    }

    g_navWinCount--;
    if (g_navWinCount == 0 && g_gdiplusToken != 0) {
        Gdiplus::GdiplusShutdown(g_gdiplusToken);
        g_gdiplusToken = 0;
    }
}

bool NavWin::Initialize(HINSTANCE hInstance, WNDPROC wndProc, void* userData) {
    m_hInstance = hInstance;

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = wndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "NavWinClass";
    wc.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    // It's okay if RegisterClassEx fails because it's already registered by another instance
    RegisterClassEx(&wc);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    m_hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        "NavWinClass",
        "NavWin",
        WS_POPUP,
        0, 0, screenWidth, screenHeight,
        NULL, NULL, hInstance, userData
    );

    if (!m_hwnd) {
        return false;
    }

    // Set the user data so the wndProc can access the App object
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)userData);

    InitGraphics();
    Clear();
    UpdateLayeredWindow();

    return true;
}

void NavWin::InitGraphics() {
    m_width = GetSystemMetrics(SM_CXSCREEN);
    m_height = GetSystemMetrics(SM_CYSCREEN);

    m_screenDC = GetDC(NULL);
    m_memDC = CreateCompatibleDC(m_screenDC);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = m_width;
    bmi.bmiHeader.biHeight = -m_height; // Top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits = nullptr;
    m_memBitmap = CreateDIBSection(m_memDC, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    m_oldBitmap = (HBITMAP)SelectObject(m_memDC, m_memBitmap);
}

void NavWin::CleanupGraphics() {
    if (m_memDC) {
        if (m_oldBitmap) {
            SelectObject(m_memDC, m_oldBitmap);
        }
        if (m_memBitmap) {
            DeleteObject(m_memBitmap);
        }
        DeleteDC(m_memDC);
        m_memDC = NULL;
    }
    if (m_screenDC) {
        ReleaseDC(NULL, m_screenDC);
        m_screenDC = NULL;
    }
}

void NavWin::UpdateSize() {
    int newWidth = GetSystemMetrics(SM_CXSCREEN);
    int newHeight = GetSystemMetrics(SM_CYSCREEN);

    if (newWidth != m_width || newHeight != m_height) {
        CleanupGraphics();

        SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);

        InitGraphics();
        Clear();
        UpdateLayeredWindow();
    }
}

void NavWin::UpdateLayeredWindow() {
    if (!m_isVisible || !m_hwnd) return;

    POINT ptSrc = {0, 0};
    POINT ptWinPos = {0, 0};
    SIZE szWin = {m_width, m_height};
    BLENDFUNCTION blend = {0};
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;

    ::UpdateLayeredWindow(m_hwnd, m_screenDC, &ptWinPos, &szWin, m_memDC, &ptSrc, 0, &blend, ULW_ALPHA);
}

void NavWin::Clear() {
    if (!m_memDC) return;

    Gdiplus::Graphics graphics(m_memDC);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.Clear(Gdiplus::Color(0, 0, 0, 0)); // Transparent
}

void NavWin::Render() {
    UpdateLayeredWindow();
}

::Rect NavWin::GetWindowSize() const {
    return {0, 0, m_width, m_height};
}

void NavWin::Show() {
    if (!m_isVisible && m_hwnd) {
        ShowWindow(m_hwnd, SW_SHOWNA);
        m_isVisible = true;
        UpdateLayeredWindow();
    }
}

void NavWin::Hide() {
    if (m_isVisible && m_hwnd) {
        ShowWindow(m_hwnd, SW_HIDE);
        m_isVisible = false;
    }
}

bool NavWin::IsVisible() const {
    return m_isVisible;
}

void NavWin::DrawLine(int x1, int y1, int x2, int y2, int thickness, const ::Color& color) {
    if (!m_memDC) return;
    Gdiplus::Graphics graphics(m_memDC);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b), (Gdiplus::REAL)thickness);
    graphics.DrawLine(&pen, x1, y1, x2, y2);
}

void NavWin::DrawRect(int x, int y, int width, int height, int thickness, const ::Color& color) {
    if (!m_memDC) return;
    Gdiplus::Graphics graphics(m_memDC);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b), (Gdiplus::REAL)thickness);
    // Draw outer contour
    graphics.DrawRectangle(&pen, x, y, width, height);
}

void NavWin::DrawText(const std::string& text, int x, int y, const std::string& fontName, int fontSize, const ::Color& color) {
    if (!m_memDC) return;
    Gdiplus::Graphics graphics(m_memDC);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

    std::wstring wText(text.begin(), text.end());
    std::wstring wFontName(fontName.begin(), fontName.end());

    Gdiplus::FontFamily fontFamily(wFontName.c_str());
    Gdiplus::Font font(&fontFamily, (Gdiplus::REAL)fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush solidBrush(Gdiplus::Color(color.a, color.r, color.g, color.b));

    // Calculate text size to center it
    Gdiplus::RectF boundingBox;
    Gdiplus::PointF origin(0.0f, 0.0f);
    graphics.MeasureString(wText.c_str(), -1, &font, origin, &boundingBox);

    Gdiplus::PointF pos((Gdiplus::REAL)x - boundingBox.Width / 2.0f, (Gdiplus::REAL)y - boundingBox.Height / 2.0f);

    graphics.DrawString(wText.c_str(), -1, &font, pos, &solidBrush);
}
