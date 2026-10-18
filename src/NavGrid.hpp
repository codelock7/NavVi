#pragma once

#include "NavWin.hpp"
#include "utils.hpp"
#include <string>

class NavGrid {
public:
    NavGrid(NavWin& navWin);

    // Setters for styling
    void SetColor(const Color& color);
    void SetThickness(int thickness);

    // Modifiers for coordinate and size
    void SetBaseSize(int width, int height);
    void Reset(); // Resets to base size and {0, 0}

    // Select sub-square (3x3 grid: r, k, u, h, g, l, v, j, m)
    bool SelectSubGrid(char label);

    // Drawing
    void Draw();

    // Utility
    void GetCenter(int& outX, int& outY) const;
    Rect GetCurrentRect() const { return m_currentRect; }

private:
    NavWin& m_navWin;

    Color m_color;
    int m_thickness;

    Rect m_baseRect;
    Rect m_currentRect;

    void DrawCrosshairCentered(int cx, int cy);
};
