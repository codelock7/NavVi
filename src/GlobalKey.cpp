#include "GlobalKey.hpp"

GlobalKey::GlobalKey()
    : m_hwnd(NULL), m_id(0), m_modifiers(MOD_CONTROL), m_vk(VK_F12), m_isRegistered(false)
{
}

GlobalKey::~GlobalKey() {
    Unregister();
}

bool GlobalKey::Register(HWND hwnd, int id, UINT modifiers, UINT vk) {
    Unregister(); // Make sure previous is unregistered

    m_hwnd = hwnd;
    m_id = id;
    m_modifiers = modifiers;
    m_vk = vk;

    m_isRegistered = RegisterHotKey(m_hwnd, m_id, m_modifiers, m_vk);
    return m_isRegistered;
}

void GlobalKey::Unregister() {
    if (m_isRegistered && m_hwnd) {
        UnregisterHotKey(m_hwnd, m_id);
        m_isRegistered = false;
    }
}

void GlobalKey::SetKey(UINT modifiers, UINT vk) {
    if (m_isRegistered) {
        Unregister();
        Register(m_hwnd, m_id, modifiers, vk);
    } else {
        m_modifiers = modifiers;
        m_vk = vk;
    }
}
