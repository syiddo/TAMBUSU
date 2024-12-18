#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <iostream>
using namespace std;

// Pointer untuk karakter dan baris
typedef struct elmChar* adrChar;
typedef struct elmLine* adrLine;

// Struktur node karakter
struct elmChar {
    char data;         // Karakter yang disimpan
    adrChar next;      // Pointer ke karakter berikutnya
    adrChar prev;      // Pointer ke karakter sebelumnya
};

// Struktur node baris
struct elmLine {
    adrChar firstChar; // Pointer ke karakter pertama di baris
    adrChar lastChar;  // Pointer ke karakter terakhir di baris
    adrLine nextLine;  // Pointer ke baris berikutnya
    adrLine prevLine;  // Pointer ke baris sebelumnya
};

// Struktur utama untuk text editor
struct TextEditor {
    adrLine firstLine; // Pointer ke baris pertama
    adrLine lastLine;  // Pointer ke baris terakhir
};

// Fungsi untuk manipulasi text editor
void createTextEditor(TextEditor &T);
adrLine createLine();
adrChar createChar(char x);
void insertChar(TextEditor &T, adrLine &currentLine, adrChar &cursor, char x);
void deleteChar(TextEditor &T, adrLine &currentLine, adrChar &cursor);
void insertNewLine(TextEditor &T, adrLine &currentLine, adrChar &cursor);
void deleteLine(TextEditor &T, adrLine &currentLine);

// Fungsi navigasi kursor
void moveCursorLeft(adrLine &currentLine, adrChar &cursor);
void moveCursorRight(adrLine &currentLine, adrChar &cursor);
void moveCursorUp(TextEditor T, adrLine &currentLine, adrChar &cursor);
void moveCursorDown(TextEditor T, adrLine &currentLine, adrChar &cursor);

// Fungsi utilitas
void printText(const TextEditor T, adrLine currentLine, adrChar cursor);
void clearScreen();

#endif
