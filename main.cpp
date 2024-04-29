#include <windows.h>
#include <iostream>

HANDLE hOut;
int BUFFER_WIDTH;
int BUFFER_HEIGHT;
char* TXTBUFFER;
WORD* COLORBUFFER;

BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) {
    switch (dwCtrlType) {
        case CTRL_CLOSE_EVENT:
            delete[] TXTBUFFER;
            delete[] COLORBUFFER;
            return TRUE; 
        default:
            return FALSE; 
    }
}

void initializeWindow(const char* p, int width, int height) {
    SetConsoleTitle(TEXT(p));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) { return; }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hConsole, &dwMode)) { return; }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hConsole, dwMode)) { return; }
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width, height, TRUE);
    CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
    GetConsoleScreenBufferInfo(hConsole, &scrBufferInfo);
    short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
    short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
    short scrBufferWidth = scrBufferInfo.dwSize.X;
    short scrBufferHeight = scrBufferInfo.dwSize.Y;
    COORD newSize;
    newSize.X = scrBufferWidth;
    newSize.Y = winHeight;
    int Status = SetConsoleScreenBufferSize(hConsole, newSize);
    GetConsoleScreenBufferInfo(hConsole, &scrBufferInfo);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    hOut = hConsole;
    BUFFER_WIDTH = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
    BUFFER_HEIGHT = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
    TXTBUFFER = new char[BUFFER_WIDTH * BUFFER_HEIGHT];
    COLORBUFFER = new WORD[BUFFER_WIDTH * BUFFER_HEIGHT];
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
}

void plot(int x, int y, char p, WORD color){
    int index = y*BUFFER_WIDTH+x;
    TXTBUFFER[index]   =p;
    COLORBUFFER[index] =color; 
}

void line(int x1, int y1, int x2, int y2, char p, WORD color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dx) > abs(dy)) {
        int y = y1;
        float slope = static_cast<float>(dy) / dx;
        for (int x = x1; x <= x2; ++x) {
            plot(x, y, p, color);
            y = static_cast<int>(y1 + slope * (x - x1));
        }
    } else {
        int x = x1;
        float slope = static_cast<float>(dx) / dy;
        for (int y = y1; y <= y2; ++y) {
            plot(x, y, p, color);
            x = static_cast<int>(x1 + slope * (y - y1));
        }
    }
}

void rect(int x, int y, int width, int height, char p, WORD color){
    for(int w=0;w<width;w++){
        for(int h=0;h<height;h++){
            
        }
    }
}

void clear() {
    std::fill(TXTBUFFER, TXTBUFFER + BUFFER_WIDTH * BUFFER_HEIGHT, ' ');
    std::fill(COLORBUFFER, COLORBUFFER + BUFFER_WIDTH * BUFFER_HEIGHT, BACKGROUND_BLUE);
}

void render() {
    COORD C;C.X=0;C.Y=0;
    SetConsoleCursorPosition(hOut, C); 
    DWORD written;
    WriteConsole(hOut, TXTBUFFER, BUFFER_WIDTH * BUFFER_HEIGHT, &written, NULL); 
    WriteConsoleOutputAttribute(hOut, COLORBUFFER, BUFFER_WIDTH * BUFFER_HEIGHT, C, &written); 
}


int main() {
    initializeWindow("RAYCASTER", 800, 600);
    while(true) { 
        clear();
        for(int x=0;x<BUFFER_WIDTH;x++){
            for(int y=0;y<BUFFER_HEIGHT;y++){
                plot(x,y,' ',BACKGROUND_RED);
            }
        }
        render();
        Sleep(100);

    }
    
}
