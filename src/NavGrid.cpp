#include "NavGrid.hpp"

NavGrid::NavGrid(NavWin& navWin)
    : m_navWin(navWin),
      m_color({0, 255, 0, 128}), // Acid green, 50% opacity (128/255)
      m_thickness(1)
{
    Rect winSize = m_navWin.GetWindowSize();
    m_baseRect = winSize;
    m_currentRect = m_baseRect;
}

void NavGrid::SetColor(const Color& color) {
    m_color = color;
}

void NavGrid::SetThickness(int thickness) {
    m_thickness = thickness;
}

void NavGrid::SetBaseSize(int width, int height) {
    m_baseRect = {0, 0, width, height};
    Reset();
}

void NavGrid::Reset() {
    m_currentRect = m_baseRect;
}

bool NavGrid::SelectSubGrid(char label) {
    int halfWidth = m_currentRect.width / 2;
    int halfHeight = m_currentRect.height / 2;

    if (halfWidth == 0 || halfHeight == 0) {
        return false; // Can't divide further sensibly
    }

    switch (label) {
        case 'a': // top-left
            m_currentRect.width = halfWidth;
            m_currentRect.height = halfHeight;
            return true;
        case 'b': // top-right
            m_currentRect.x += halfWidth;
            m_currentRect.width = halfWidth;
            m_currentRect.height = halfHeight;
            return true;
        case 'c': // bottom-left
            m_currentRect.y += halfHeight;
            m_currentRect.width = halfWidth;
            m_currentRect.height = halfHeight;
            return true;
        case 'd': // bottom-right
            m_currentRect.x += halfWidth;
            m_currentRect.y += halfHeight;
            m_currentRect.width = halfWidth;
            m_currentRect.height = halfHeight;
            return true;
        default:
            return false;
    }
}

void NavGrid::GetCenter(int& outX, int& outY) const {
    outX = m_currentRect.x + m_currentRect.width / 2;
    outY = m_currentRect.y + m_currentRect.height / 2;
}

void NavGrid::Draw() {
    // Draw outer contour
    m_navWin.DrawRect(m_currentRect.x, m_currentRect.y, m_currentRect.width, m_currentRect.height, m_thickness, m_color);

    // Draw cross
    int midX = m_currentRect.x + m_currentRect.width / 2;
    int midY = m_currentRect.y + m_currentRect.height / 2;

    m_navWin.DrawLine(midX, m_currentRect.y, midX, m_currentRect.y + m_currentRect.height, m_thickness, m_color);
    m_navWin.DrawLine(m_currentRect.x, midY, m_currentRect.x + m_currentRect.width, midY, m_thickness, m_color);

    // Draw labels in centers of 4 quadrants
    int qWidth = m_currentRect.width / 4;
    int qHeight = m_currentRect.height / 4;

    DrawTextCentered("a", m_currentRect.x + qWidth, m_currentRect.y + qHeight);
    DrawTextCentered("b", midX + qWidth, m_currentRect.y + qHeight);
    DrawTextCentered("c", m_currentRect.x + qWidth, midY + qHeight);
    DrawTextCentered("d", midX + qWidth, midY + qHeight);
}

void NavGrid::DrawTextCentered(const std::string& text, int cx, int cy) {
    // 20 pixels font size for example
    m_navWin.DrawText(text, cx, cy, "Arial", 20, m_color);
}
