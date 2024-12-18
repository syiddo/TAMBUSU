#include "TextEditor.h"
#include <cstdlib>

// Inisialisasi text editor
void createTextEditor(TextEditor &T) {
    T.firstLine = nullptr;
    T.lastLine = nullptr;
}

// Membuat node baris
adrLine createLine() {
    adrLine newLine = new elmLine();
    newLine->firstChar = nullptr;
    newLine->lastChar = nullptr;
    newLine->nextLine = nullptr;
    newLine->prevLine = nullptr;
    return newLine;
}

// Membuat node karakter
adrChar createChar(char x) {
    adrChar newChar = new elmChar();
    newChar->data = x;
    newChar->next = nullptr;
    newChar->prev = nullptr;
    return newChar;
}

// Menambahkan karakter
void insertChar(TextEditor &T, adrLine &currentLine, adrChar &cursor, char x) {
    if (!currentLine) {
        currentLine = createLine();
        if (!T.firstLine) {
            T.firstLine = T.lastLine = currentLine;
        }
    }

    adrChar newChar = createChar(x);

    if (!cursor) {
        newChar->next = nullptr;
        newChar->prev = currentLine->lastChar;

        if (currentLine->lastChar) {
            currentLine->lastChar->next = newChar;
        } else {
            currentLine->firstChar = newChar;
        }
        currentLine->lastChar = newChar;
        cursor = newChar;
    } else {
        newChar->next = cursor->next;
        if (cursor->next) {
            cursor->next->prev = newChar;
        } else {
            currentLine->lastChar = newChar;
        }
        newChar->prev = cursor;
        cursor->next = newChar;
        cursor = newChar;
    }
}

// Menghapus karakter
void deleteChar(TextEditor &T, adrLine &currentLine, adrChar &cursor) {
    if (!cursor) {
        // Jika kursor berada di awal baris dan tidak ada karakter
        if (currentLine && currentLine->prevLine) {
            adrLine previousLine = currentLine->prevLine;
            // Gabungkan karakter baris ini dengan baris sebelumnya
            if (currentLine->firstChar) {
                if (!previousLine->lastChar) {
                    previousLine->firstChar = currentLine->firstChar;
                } else {
                    previousLine->lastChar->next = currentLine->firstChar;
                    currentLine->firstChar->prev = previousLine->lastChar;
                }
                previousLine->lastChar = currentLine->lastChar;
            }

            // Hapus baris saat ini
            adrLine toDelete = currentLine;
            if (currentLine->nextLine) {
                currentLine->nextLine->prevLine = previousLine;
                previousLine->nextLine = currentLine->nextLine;
            } else {
                previousLine->nextLine = nullptr;
                T.lastLine = previousLine;
            }

            // Pindahkan kursor ke baris sebelumnya dan ke akhir baris
            currentLine = previousLine;
            cursor = previousLine->lastChar;

            delete toDelete;
        }
        return;
    }

    // Cek jika kursor berada di karakter pertama dalam baris
    if (cursor == currentLine->firstChar) {
        // Hapus karakter pertama
        adrChar toDelete = cursor;
        cursor = cursor->next; // Pindahkan kursor ke karakter berikutnya
        if (cursor) {
            cursor->prev = nullptr;
        }
        currentLine->firstChar = cursor;

        delete toDelete;

        // Jika setelah penghapusan baris kosong
        if (!currentLine->firstChar) {
            adrLine previousLine = currentLine->prevLine;
            deleteLine(T, currentLine); // Fungsi untuk menghapus baris
            currentLine = previousLine ? previousLine : T.firstLine;
            cursor = currentLine ? currentLine->lastChar : nullptr;
        }
        return;
    }

    // Penghapusan karakter normal
    adrChar toDelete = cursor;

    if (cursor->prev) {
        cursor->prev->next = cursor->next;
    } else {
        currentLine->firstChar = cursor->next;
    }

    if (cursor->next) {
        cursor->next->prev = cursor->prev;
    } else {
        currentLine->lastChar = cursor->prev;
    }

    // Pindahkan kursor
    cursor = cursor->prev ? cursor->prev : cursor->next;

    // Hapus karakter yang dihapus
    delete toDelete;

    // Jika baris menjadi kosong, hapus baris
    if (!currentLine->firstChar) {
        adrLine previousLine = currentLine->prevLine;
        deleteLine(T, currentLine);
        currentLine = previousLine ? previousLine : T.firstLine;
        cursor = currentLine ? currentLine->lastChar : nullptr;
    }
}

// Menambahkan baris baru
void insertNewLine(TextEditor &T, adrLine &currentLine, adrChar &cursor) {
    adrLine newLine = createLine();

    newLine->prevLine = currentLine;
    newLine->nextLine = currentLine ? currentLine->nextLine : nullptr;

    if (currentLine) {
        if (currentLine->nextLine) {
            currentLine->nextLine->prevLine = newLine;
        } else {
            T.lastLine = newLine;
        }
        currentLine->nextLine = newLine;
    } else {
        T.firstLine = T.lastLine = newLine;
    }

    if (cursor) {
        adrChar temp = cursor->next;
        cursor->next = nullptr;

        while (temp) {
            adrChar nextTemp = temp->next;
            temp->next = nullptr;
            temp->prev = nullptr;

            if (!newLine->firstChar) {
                newLine->firstChar = newLine->lastChar = temp;
            } else {
                newLine->lastChar->next = temp;
                temp->prev = newLine->lastChar;
                newLine->lastChar = temp;
            }
            temp = nextTemp;
        }
    }

    cursor = nullptr;
    currentLine = newLine;
}

// Menghapus baris
void deleteLine(TextEditor &T, adrLine &currentLine) {
    if (!currentLine) return;

    // Update koneksi baris sebelumnya
    if (currentLine->prevLine) {
        currentLine->prevLine->nextLine = currentLine->nextLine;
    } else {
        T.firstLine = currentLine->nextLine; // Update baris pertama
    }

    // Update koneksi baris berikutnya
    if (currentLine->nextLine) {
        currentLine->nextLine->prevLine = currentLine->prevLine;
    } else {
        T.lastLine = currentLine->prevLine; // Update baris terakhir
    }

    delete currentLine; // Hapus baris
}

// Fungsi navigasi kursor
void moveCursorLeft(adrLine &currentLine, adrChar &cursor) {
    if (cursor) {
        if (cursor->prev) {
            cursor = cursor->prev;  // Move to previous character
        } else {
            // If at the first character, set cursor to nullptr (start of line)
            cursor = nullptr;
        }
    } else if (currentLine->prevLine) {
        // If already at start of line, move to previous line's end
        currentLine = currentLine->prevLine;
        cursor = currentLine->lastChar;
    }
}

void moveCursorRight(adrLine &currentLine, adrChar &cursor) {
    if (!cursor && currentLine->firstChar) {
        // If at start of line, move to first character
        cursor = currentLine->firstChar;
        return;
    }

    if (cursor) {
        if (cursor->next) {
            cursor = cursor->next;  // Move to next character
        } else if (currentLine->nextLine) {
            // Move to next line
            currentLine = currentLine->nextLine;
            cursor = currentLine->firstChar;
        }
    }
}

void moveCursorUp(TextEditor T, adrLine &currentLine, adrChar &cursor) {
    if (currentLine && currentLine->prevLine) {
        currentLine = currentLine->prevLine;
        cursor = currentLine->lastChar;
    }
}

void moveCursorDown(TextEditor T, adrLine &currentLine, adrChar &cursor) {
    if (currentLine && currentLine->nextLine) {
        currentLine = currentLine->nextLine;
        cursor = currentLine->firstChar;
    }
}

// Fungsi utilitas
void printText(TextEditor T, adrLine currentLine, adrChar cursor) {
    adrLine line = T.firstLine;

    while (line) {
        adrChar charNode = line->firstChar;

        // Special case for current line's cursor positioning
        if (line == currentLine) {
            // If cursor is explicitly set to nullptr (start of line)
            if (cursor == nullptr) {
                cout << '|';
            }
        }

        while (charNode) {
            cout << charNode->data;
            if (line == currentLine && charNode == cursor) {
                cout << '|';
            }
            charNode = charNode->next;
        }

        cout << '\n';
        line = line->nextLine;
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[H\033[J";
#endif
}
