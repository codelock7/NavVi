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

    Rect newRect = m_currentRect;
    bool matched = true;

    switch (label) {
        case 'r': // top-left
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        case 'k': // top-middle
            newRect.x += thirdWidth;
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        case 'u': // top-right
            newRect.x += 2 * thirdWidth;
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        case 'h': // middle-left
            newRect.y += thirdHeight;
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        case 'g': // middle-middle
            newRect.x += thirdWidth;
            newRect.y += thirdHeight;
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        case 'l': // middle-right
            newRect.x += 2 * thirdWidth;
            newRect.y += thirdHeight;
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        case 'v': // bottom-left
            newRect.y += 2 * thirdHeight;
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        case 'j': // bottom-middle
            newRect.x += thirdWidth;
            newRect.y += 2 * thirdHeight;
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        case 'm': // bottom-right
            newRect.x += 2 * thirdWidth;
            newRect.y += 2 * thirdHeight;
            newRect.width = thirdWidth;
            newRect.height = thirdHeight;
            break;
        default:
            matched = false;
            break;
    }

    if (!matched) return false;

    if (expand) {
        int cx = newRect.x + newRect.width / 2;
        int cy = newRect.y + newRect.height / 2;

        newRect.width = (int)(newRect.width * 1.5f);
        newRect.height = (int)(newRect.height * 1.5f);

        newRect.x = cx - newRect.width / 2;
        newRect.y = cy - newRect.height / 2;
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
