#include "TextEditor.h"
#include <cstdlib>

// Inisialisasi editor teks
void createTextEditor(TextEditor &T) {
    adrChar dummy = createChar('\0');
    T.firstChar = dummy;
    T.lastChar = dummy;
    dummy->next = nullptr;
    dummy->prev = nullptr;

    // Inisialisasi stack undo dan redo
    createStack(T.undoStack, 100000);
    createStack(T.redoStack, 100000);
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
void insertChar(TextEditor &T, adrChar &cursor, char x, bool fromUndoRedo) {
    adrChar newChar = createChar(x);

    if (!cursor || cursor == T.firstChar) {
        newChar->next = T.firstChar->next;
        if (T.firstChar->next) {
            T.firstChar->next->prev = newChar;
        } else {
            T.lastChar = newChar;
        }
        T.firstChar->next = newChar;
        newChar->prev = T.firstChar;
    } else {
        newChar->next = cursor->next;
        if (cursor->next) {
            cursor->next->prev = newChar;
        } else {
            T.lastChar = newChar;
        }
        newChar->prev = cursor;
        cursor->next = newChar;
    }


    // Kosongkan redo stack setelah aksi baru
    if (!fromUndoRedo) {
        while (!isStackEmpty(T.redoStack)) {
            pop(T.redoStack);
        }
    }

    cursor = newChar;

    char toInsert = cursor->data;

    if (!fromUndoRedo) push(T.undoStack, {"insert", toInsert, cursor});
}

// Menghapus karakter
void deleteChar(TextEditor &T, adrChar &cursor, bool fromUndoRedo) {
    if (!cursor || cursor == T.firstChar) return;

    adrChar toDelete = cursor;

    if (cursor->prev) {
        cursor->prev->next = cursor->next;
    }
    if (cursor->next) {
        cursor->next->prev = cursor->prev;
    } else {
        T.lastChar = cursor->prev;
    }

    if (!fromUndoRedo) push(T.undoStack, {"delete", toDelete->data, cursor});

    cursor = cursor->prev ? cursor->prev : cursor->next;

    delete toDelete;
}

// Memindahkan kursor ke kiri
void moveCursorLeft(adrChar &cursor) {
    if (cursor && cursor->prev) {
        cursor = cursor->prev;
    }
}

// Memindahkan kursor ke kanan
void moveCursorRight(adrChar &cursor) {
    if (cursor && cursor->next) {
        cursor = cursor->next;
    }
}

// Memindahkan kursor ke atas
void moveCursorUp(TextEditor T, adrChar &cursor) {
    if (!cursor || !cursor->prev || cursor->prev == T.firstChar) return;

    int horizontalPosition = 0;
    adrChar temp = cursor;

    // Count the position from the start of the line
    while (temp->data != '\n' && temp->prev != T.firstChar) {
        horizontalPosition++;
        temp = temp->prev;
    }

    if (temp) {
        temp = temp->prev;  
        if (!temp->prev) {  
            return;
        }
        while (temp && temp->data != '\n' && temp->prev != T.firstChar) {
            temp = temp->prev;
        }
        if (temp) temp = temp->next; 
    }

    int currentColumn = 1;
    while (currentColumn < horizontalPosition && temp->next->data != '\n') {
        temp = temp->next;
        currentColumn++;
    }

    if (temp && temp->data != '\n') cursor = temp;
}

// Memindahkan kursor ke bawah
void moveCursorDown(TextEditor T, adrChar &cursor) {
    if (!cursor || !cursor->next) return;

    int horizontalPosition = -1;
    adrChar temp = cursor;

    while (temp && temp->data != '\n' && temp->prev != T.firstChar) {
        horizontalPosition++;
        temp = temp->prev;
    }

    temp = cursor;
    while (temp && temp->data != '\n') {
        temp = temp->next;
    }
    if (temp && temp->next != T.firstChar) temp = temp->next; 

    int currentColumn = 1;
    while (currentColumn < horizontalPosition && temp->next && temp->next->data != '\n') {
        temp = temp->next;
        currentColumn++;
    }

    if (temp && temp->data != '\n') cursor = temp;
}

void undo(TextEditor &T, adrChar &cursor) {
    if (isStackEmpty(T.undoStack)) { 
        cout << "Tidak ada operasi untuk di-undo" << endl;
        return;
    }

    if (!isStackEmpty(T.undoStack)) { 
        Action op = pop(T.undoStack); 

        if (op.position == T.firstChar) {
            cout << "Undo berhenti: mencapai posisi awal (dummy)" << endl;
        }

        push(T.redoStack, op); 

        if (op.type == "insert") {
            deleteChar(T, cursor, true); // Hapus karakter
        } else if (op.type == "delete") { 
            insertChar(T, cursor, op.data, true); // mengembalikan karakter
        }
    }
}

void redo(TextEditor &T, adrChar &cursor) {
    if (isStackEmpty(T.redoStack)) { // Memeriksa apakah undoStack kosong
        cout << "Tidak ada operasi untuk di-undo" << endl;
        return;
    }

    if (!isStackEmpty(T.redoStack)) { // Iterasi hingga menemukan operasi yang valid
        Action op = pop(T.redoStack); // Mengambil operasi terakhir dari undoStack

        // Jika menemukan dummy (T.firstChar), berhenti
        if (op.position == T.firstChar) {
            cout << "Undo berhenti: mencapai posisi awal (dummy)" << endl;
        }

        // Operasi undo yang valid
        push(T.undoStack, op); // Memindahkan operasi tersebut ke redoStack

        if (op.type == "delete") {
            deleteChar(T, cursor, true); // Hapus kara
        } else if (op.type == "insert") { 
            insertChar(T, cursor, op.data, true); // membalikan karakter
        }
    }
}

// Primitif Stack
void createStack(Stack &S, int capacity) {
    S.actions = new Action[capacity];
    S.top = -1;
    S.capacity = capacity;
}

bool isStackEmpty(const Stack &S) {
    return S.top == -1;
}

bool isStackFull(const Stack &S) {
    return S.top == S.capacity - 1;
}

void push(Stack &S, Action action) {
    if (isStackFull(S)) {
        cout << "Stack penuh." << endl;
        return;
    }
    S.actions[++S.top] = action;
}

Action pop(Stack &S) {
    if (isStackEmpty(S)) {
        return {"", '\0', nullptr};
    }
    return S.actions[S.top--];
}

// Menampilkan teks
void printText(const TextEditor &T, adrChar cursor) {
    adrChar charNode = T.firstChar;

    while (charNode) {
        cout << charNode->data;
        if (charNode == cursor) {
            cout << '|';
        }
        charNode = charNode->next;
    }

    cout << endl;
}

// Fungsi untuk menampilkan editor
void showEditor(const TextEditor &T, adrChar cursor) {
    clearScreen();
    
    // Header
    cout << "=====================================" << endl;
    cout << "          TEXT EDITOR TAMBUSU        " << endl;
    cout << "=====================================" << endl;

    // Display text content
    printText(T, cursor);

    // Footer
    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << "Ctrl+Z: Undo | Ctrl+Y: Redo | ESC: Keluar" << endl;
    cout << "-------------------------------------" << endl;
}

// Membersihkan layar
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[H\033[J";
#endif
}
