#include <windows.h>
#include "App.hpp"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    App app;
    if (app.Initialize(hInstance)) {
        app.Run();
    }
    return 0;
}
