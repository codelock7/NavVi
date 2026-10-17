#pragma once

#include <windows.h>

class GlobalKey {
public:
    GlobalKey();
    ~GlobalKey();

    // Delete copy and move constructors
    GlobalKey(const GlobalKey&) = delete;
    GlobalKey& operator=(const GlobalKey&) = delete;
    GlobalKey(GlobalKey&&) = delete;
    GlobalKey& operator=(GlobalKey&&) = delete;

    // By default registers Ctrl+F12
    bool Register(HWND hwnd, int id, UINT modifiers = MOD_CONTROL, UINT vk = VK_F12);
    void Unregister();

    void SetKey(UINT modifiers, UINT vk);

private:
    HWND m_hwnd;
    int m_id;
    UINT m_modifiers;
    UINT m_vk;
    bool m_isRegistered;
};
