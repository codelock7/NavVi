#include "App.hpp"
#include <iostream>
#include <cctype>

LRESULT CALLBACK GlobalWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    App* app = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        app = reinterpret_cast<App*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
    } else {
        app = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (app) {
        return app->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

App::App() : m_hInstance(NULL) {}

App::~App() {}

bool App::Initialize(HINSTANCE hInstance) {
    m_hInstance = hInstance;

    m_navWin = std::make_unique<NavWin>();
    if (!m_navWin->Initialize(hInstance, GlobalWindowProc, this)) {
        return false;
    }

    m_globalKey = std::make_unique<GlobalKey>();
    // Default: Ctrl+F12
    if (!m_globalKey->Register(m_navWin->GetHWND(), HOTKEY_ID, MOD_CONTROL, VK_F12)) {
        std::cerr << "Failed to register hotkey\n";
    }

    m_navGrid = std::make_unique<NavGrid>(*m_navWin);

    return true;
}

void App::Run() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT App::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_HOTKEY:
            if (wParam == HOTKEY_ID) {
                ToggleNav();
            }
            return 0;

        case WM_KEYDOWN: {
            if (!m_navWin->IsVisible()) break;

            if (wParam == VK_ESCAPE) {
                m_navGrid->Reset();
                CenterCursor();
                Redraw();
            } else if (wParam == VK_RETURN) {
                bool shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
                PerformClick(shiftPressed);
                ToggleNav(); // typically you hide it after clicking
            } else {
                // Map a, b, c, d
                char c = std::tolower(MapVirtualKeyA(wParam, MAPVK_VK_TO_CHAR));
                if (c == 'a' || c == 'b' || c == 'c' || c == 'd') {
                    if (m_navGrid->SelectSubGrid(c)) {
                        CenterCursor();
                        Redraw();
                    }
                }
            }
            return 0;
        }

        case WM_DISPLAYCHANGE: {
            // Screen resolution changed
            m_navWin->UpdateSize();
            Rect s = m_navWin->GetWindowSize();
            m_navGrid->SetBaseSize(s.width, s.height);
            if (m_navWin->IsVisible()) {
                Redraw();
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void App::ToggleNav() {
    if (m_navWin->IsVisible()) {
        m_navWin->Hide();
        m_navGrid->Reset();
    } else {
        m_navWin->Show();
        // Give window focus so it receives keyboard events
        SetForegroundWindow(m_navWin->GetHWND());
        SetFocus(m_navWin->GetHWND());
        CenterCursor();
        Redraw();
    }
}

void App::PerformClick(bool rightClick) {
    if (rightClick) {
        EmulateRightClick();
    } else {
        EmulateLeftClick();
    }
}

void App::Redraw() {
    m_navWin->Clear();
    m_navGrid->Draw();
    m_navWin->Render();
}

void App::CenterCursor() {
    int cx, cy;
    m_navGrid->GetCenter(cx, cy);
    MoveCursorTo(cx, cy);
}
