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
    int thirdWidth = m_currentRect.width / 3;
    int thirdHeight = m_currentRect.height / 3;

    if (thirdWidth == 0 || thirdHeight == 0) {
        return false; // Can't divide further sensibly
    }

    switch (label) {
        case 'r': // top-left
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
            return true;
        case 'k': // top-middle
            m_currentRect.x += thirdWidth;
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
            return true;
        case 'u': // top-right
            m_currentRect.x += 2 * thirdWidth;
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
            return true;
        case 'h': // middle-left
            m_currentRect.y += thirdHeight;
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
            return true;
        case 'g': // middle-middle
            m_currentRect.x += thirdWidth;
            m_currentRect.y += thirdHeight;
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
            return true;
        case 'l': // middle-right
            m_currentRect.x += 2 * thirdWidth;
            m_currentRect.y += thirdHeight;
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
            return true;
        case 'v': // bottom-left
            m_currentRect.y += 2 * thirdHeight;
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
            return true;
        case 'j': // bottom-middle
            m_currentRect.x += thirdWidth;
            m_currentRect.y += 2 * thirdHeight;
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
            return true;
        case 'm': // bottom-right
            m_currentRect.x += 2 * thirdWidth;
            m_currentRect.y += 2 * thirdHeight;
            m_currentRect.width = thirdWidth;
            m_currentRect.height = thirdHeight;
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

    int thirdWidth = m_currentRect.width / 3;
    int thirdHeight = m_currentRect.height / 3;

    // Draw 2 vertical lines
    m_navWin.DrawLine(m_currentRect.x + thirdWidth, m_currentRect.y,
                      m_currentRect.x + thirdWidth, m_currentRect.y + m_currentRect.height,
                      m_thickness, m_color);
    m_navWin.DrawLine(m_currentRect.x + 2 * thirdWidth, m_currentRect.y,
                      m_currentRect.x + 2 * thirdWidth, m_currentRect.y + m_currentRect.height,
                      m_thickness, m_color);

    // Draw 2 horizontal lines
    m_navWin.DrawLine(m_currentRect.x, m_currentRect.y + thirdHeight,
                      m_currentRect.x + m_currentRect.width, m_currentRect.y + thirdHeight,
                      m_thickness, m_color);
    m_navWin.DrawLine(m_currentRect.x, m_currentRect.y + 2 * thirdHeight,
                      m_currentRect.x + m_currentRect.width, m_currentRect.y + 2 * thirdHeight,
                      m_thickness, m_color);

    // Draw crosshairs in centers of 9 sub-squares
    int qWidth = m_currentRect.width / 6;
    int qHeight = m_currentRect.height / 6;

    // Row 1
    DrawCrosshairCentered(m_currentRect.x + qWidth, m_currentRect.y + qHeight);
    DrawCrosshairCentered(m_currentRect.x + 3 * qWidth, m_currentRect.y + qHeight);
    DrawCrosshairCentered(m_currentRect.x + 5 * qWidth, m_currentRect.y + qHeight);

    // Row 2
    DrawCrosshairCentered(m_currentRect.x + qWidth, m_currentRect.y + 3 * qHeight);
    DrawCrosshairCentered(m_currentRect.x + 3 * qWidth, m_currentRect.y + 3 * qHeight);
    DrawCrosshairCentered(m_currentRect.x + 5 * qWidth, m_currentRect.y + 3 * qHeight);

    // Row 3
    DrawCrosshairCentered(m_currentRect.x + qWidth, m_currentRect.y + 5 * qHeight);
    DrawCrosshairCentered(m_currentRect.x + 3 * qWidth, m_currentRect.y + 5 * qHeight);
    DrawCrosshairCentered(m_currentRect.x + 5 * qWidth, m_currentRect.y + 5 * qHeight);
}

void NavGrid::DrawCrosshairCentered(int cx, int cy) {
    // 3x3 pixel crosshair means:
    // a vertical 1x3 bar centered at (cx, cy)
    // a horizontal 3x1 bar centered at (cx, cy)
    m_navWin.DrawFilledRect(cx, cy - 1, 1, 3, m_color);
    m_navWin.DrawFilledRect(cx - 1, cy, 3, 1, m_color);
}
