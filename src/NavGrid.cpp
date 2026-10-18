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

bool NavGrid::SelectSubGrid(char label, bool expand) {
    int thirdWidth = m_currentRect.width / 3;
    int thirdHeight = m_currentRect.height / 3;

    if (thirdWidth == 0 || thirdHeight == 0) {
        return false; // Can't divide further sensibly
    }

    int col = -1, row = -1;

    switch (label) {
        case 'r': col = 0; row = 0; break; // top-left
        case 'k': col = 1; row = 0; break; // top-middle
        case 'u': col = 2; row = 0; break; // top-right
        case 'h': col = 0; row = 1; break; // middle-left
        case 'g': col = 1; row = 1; break; // middle-middle
        case 'l': col = 2; row = 1; break; // middle-right
        case 'v': col = 0; row = 2; break; // bottom-left
        case 'j': col = 1; row = 2; break; // bottom-middle
        case 'm': col = 2; row = 2; break; // bottom-right
        default: break;
    }

    if (col == -1 || row == -1) return false;

    Rect newRect = m_currentRect;

    if (expand) {
        newRect.width = static_cast<int>(thirdWidth * 1.5);
        newRect.height = static_cast<int>(thirdHeight * 1.5);

        newRect.x += col * (m_currentRect.width - newRect.width) / 2;
        newRect.y += row * (m_currentRect.height - newRect.height) / 2;
    } else {
        newRect.width = thirdWidth;
        newRect.height = thirdHeight;

        newRect.x += col * thirdWidth;
        newRect.y += row * thirdHeight;
    }

    m_currentRect = newRect;
    return true;
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
