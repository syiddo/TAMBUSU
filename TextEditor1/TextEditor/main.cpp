#include "TextEditor.h"
#include <conio.h>

int main() {
    TextEditor T;
    createTextEditor(T);

    // Membuat baris pertama
    adrLine currentLine = createLine();
    T.firstLine = T.lastLine = currentLine;

    adrChar cursor = nullptr;
    bool running = true;

    while (running) {
        clearScreen();
        cout << "Text Editor Sederhana\n";
        printText(T, currentLine, cursor);

        char key = _getch();

        if (key == 27) { // ESC untuk keluar
            running = false;
        } else if (key == -32) { // Tombol panah
            char arrow = _getch();
            switch (arrow) {
                case 75: // Panah kiri
                    moveCursorLeft(currentLine, cursor);
                    break;
                case 77: // Panah kanan
                    moveCursorRight(currentLine, cursor);
                    break;
                case 72: // Panah atas
                    moveCursorUp(T, currentLine, cursor);
                    break;
                case 80: // Panah bawah
                    moveCursorDown(T, currentLine, cursor);
                    break;
            }
        } else if (key == 8) { // Backspace
            deleteChar(T, currentLine, cursor);
        } else if (key == 13) { // Enter
            insertNewLine(T, currentLine, cursor);
        } else if (key >= 32 && key <= 126) { // Karakter dapat dicetak
            insertChar(T, currentLine, cursor, key);
        }
    }

    return 0;
}
