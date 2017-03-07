#include "Controls.h"
#include <windows.h>
#include <iostream>


namespace testapp {

void Controls::Progressbar(const char* title) {
    std::cout << title << " ";
    static auto dots = 0u;
    dots = dots++ % 10u;
    for ( auto i = 0u; i < dots; ++i ) {
        std::cout << ".";
    }
    std::cout << std::endl;
}


void Controls::Username(const std::string& username) {
    std::cout << "[User: " << username << "]" << std::endl;
}


void Controls::ClearScreen() {
    auto Error = [] (BOOL bSuccess, const char* api) {
                if ( !bSuccess ) {
                    printf("%s:Error %d from %s on line %d\n", __FILE__, GetLastError(), api, __LINE__);
                }
            };

    COORD coordScreen = { 0, 0 }; /* here's where we'll home the
                                      cursor */
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize; /* number of character cells in
                         the current buffer */

    /* get the number of character cells in the current buffer */

    auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    Error(bSuccess, "GetConsoleScreenBufferInfo");
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    /* fill the entire screen with blanks */

    bSuccess = FillConsoleOutputCharacter(hConsole, static_cast<TCHAR>(' '), dwConSize, coordScreen, &cCharsWritten);
    Error(bSuccess, "FillConsoleOutputCharacter");

    /* get the current text attribute */

    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    Error(bSuccess, "ConsoleScreenBufferInfo");

    /* now set the buffer's attributes accordingly */

    bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    Error(bSuccess, "FillConsoleOutputAttribute");

    /* put the cursor at (0, 0) */

    bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
    Error(bSuccess, "SetConsoleCursorPosition");
}

} // namespace testapp
