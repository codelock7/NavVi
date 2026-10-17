#pragma once

#include "NavWin.hpp"
#include "GlobalKey.hpp"
#include "NavGrid.hpp"

#include <windows.h>
#include <memory>

class App {
public:
    App();
    ~App();

    bool Initialize(HINSTANCE hInstance);
    void Run();

    // Event handlers
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    void ToggleNav();
    void HandleKey(char key);
    void HandleActionKey(WPARAM wParam);
    void PerformClick(bool rightClick);
    void Redraw();
    void CenterCursor();

    HINSTANCE m_hInstance;
    std::unique_ptr<NavWin> m_navWin;
    std::unique_ptr<GlobalKey> m_globalKey;
    std::unique_ptr<NavGrid> m_navGrid;

    // Hotkey ID
    static const int HOTKEY_ID = 1;
};
