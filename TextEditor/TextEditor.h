#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <iostream>
using namespace std;

// Struktur elemen karakter
struct elmChar {
    char data;
    struct elmChar* next;
    struct elmChar* prev;
};

typedef elmChar* adrChar;

// Struktur untuk menyimpan aksi (undo/redo)
struct Action {
    string type;      // Jenis aksi: "insert" atau "delete"
    char data;        // Karakter yang dimanipulasi
    adrChar position; // Posisi karakter dalam teks
};

// Struktur stack untuk undo/redo
struct Stack {
    Action* actions;  // Array penyimpanan aksi
    int top;          // Indeks elemen teratas
    int capacity;     // Kapasitas maksimum stack
};

// Struktur editor teks
struct TextEditor {
    adrChar firstChar;
    adrChar lastChar;
    Stack undoStack;  // Stack untuk undo
    Stack redoStack;  // Stack untuk redo
    string currentWord; // Menyimpan kata yang sedang dibentuk
};


// Fungsi untuk menampilkan editor
void showEditor(const TextEditor &T, adrChar cursor);

// Fungsi untuk membuat editor teks baru
void createTextEditor(TextEditor &T);

// Fungsi untuk membuat node karakter
adrChar createChar(char x);

// Fungsi untuk memasukkan karakter
void insertChar(TextEditor &T, adrChar &cursor, char x, bool fromUndoRedo=false);

// Fungsi untuk menghapus karakter
void deleteChar(TextEditor &T, adrChar &cursor, bool fromUndoRedo=false);

// Fungsi untuk memindahkan kursor ke kiri
void moveCursorLeft(adrChar &cursor);

// Fungsi untuk memindahkan kursor ke kanan
void moveCursorRight(adrChar &cursor);

// Fungsi untuk memindahkan kursor ke atas
void moveCursorUp(TextEditor T, adrChar &cursor);

// Fungsi untuk memindahkan kursor ke bawah
void moveCursorDown(TextEditor T, adrChar &cursor);

// Fungsi utilitas
void printText(const TextEditor &T, adrChar cursor);
void clearScreen();
void saveToFile(const TextEditor T, const string &fileName);
void loadFromFile(TextEditor &T, const string &fileName);

// Primitif Stack
void createStack(Stack &S, int capacity);
bool isStackEmpty(const Stack &S);
bool isStackFull(const Stack &S);
void push(Stack &S, Action action);
Action pop(Stack &S);

// Fungsi undo dan redo
void undo(TextEditor &T, adrChar &cursor);
void redo(TextEditor &T, adrChar &cursor);

#endif
