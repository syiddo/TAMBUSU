#include "TextEditor.h"
#include <conio.h>
#include <cstdlib>

using namespace std;

int main() {
    TextEditor T;
    createTextEditor(T);

    adrChar cursor = nullptr;

    bool running = true;

    while (running) {
        showEditor(T, cursor);

        char key = _getch();

        if (key == 27) { // ESC untuk keluar
            running = false;
        } else if (key == -32) { // Tombol panah
            char arrow = _getch();
            if (arrow == 75) { // Panah kiri
                moveCursorLeft(cursor);
            } else if (arrow == 77) { // Panah kanan
                moveCursorRight(cursor);
            } else if (arrow == 72) { // Panah atas
                moveCursorUp(T, cursor);
            } else if (arrow == 80) { // Panah bawah
                moveCursorDown(T, cursor);
            }
        } else if (key == 8) { // Backspace
            deleteChar(T, cursor);
        } else if (key == 13) { // Enter (baris baru)
            insertChar(T, cursor, '\n');
        } else if (key == 26) { // Ctrl+Z untuk undo
            undo(T, cursor);
        } else if (key == 25) { // Ctrl+Y untuk redo
            redo(T, cursor);
        } else if (key >= 32 && key <= 126) { // Karakter dapat dicetak
            insertChar(T, cursor, key);
        }
    }

    cout << "Keluar dari editor. Terima kasih!" << endl;

    return 0;
}
