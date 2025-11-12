#include "MedicalSupplyStack.hpp"
#include <iostream>
#include <sstream>

MedicalSupplyStack::MedicalSupplyStack(const string& file) : top(-1), filename(file), nextID(1) {
    loadFromFile();
}

// isEmpty/isFull
bool MedicalSupplyStack::isEmpty() const { return top == -1; }
bool MedicalSupplyStack::isFull() const { return top == MAX_SIZE - 1; }

// ========== Function 1: Add Supply Stock ==========
void MedicalSupplyStack::addSupply() {
    if (isFull()) {
        cout << "[Error] Supply storage is full. Cannot add more supplies.\n";
        return;
    }

    string type, batch;
    int qty;

    cout << "Enter supply type: ";
    getline(cin >> ws, type);

    cout << "Enter quantity: ";
    while (!(cin >> qty) || qty <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter a valid positive integer for quantity: ";
    }
    cin.ignore(1000, '\n');

    cout << "Enter batch: ";
    getline(cin, batch);

    // Create a MedicalSupply object and add to stack
    supplies[++top] = MedicalSupply(nextID, type, qty, batch);
    cout << "[OK] Added supply: \n ID:" << nextID << " | Type: " << type << " | Qty: " << qty << " | Batch: " << batch << "\n";
    nextID++; 

    // Auto-save after adding
    saveToFile();
}

// useLastSupply
bool MedicalSupplyStack::useLastSupply(MedicalSupply &out) {
    if (isEmpty()) {
        cout << "[Error] No supplies available to use.\n";
        return false;
    }

    // Pop the top item
    out = supplies[top--];

    // Auto-save to file right after removing
    saveToFile();

    return true;
}

// viewSupplies
void MedicalSupplyStack::viewSupplies() const {
    if (isEmpty()) {
        cout << "No supplies available.\n";
        return;
    }
    for (int i = top; i >= 0; --i) {
        cout << "ID: " << supplies[i].id
             << ", Type: " << supplies[i].type
             << ", Qty: " << supplies[i].quantity
             << ", Batch: " << supplies[i].batch << "\n";
    }
}

// loadFromFile
void MedicalSupplyStack::loadFromFile() {
    int count = 0;
    string *lines = TextFileManager::readLinesFromFile(filename, count);
    if (lines == nullptr) {
        cout << "No file found or empty file.\n";
        return;
    }
    top = -1; // reset stack
    nextID = 1;
    int loaded = 0;
    cout << "\n=== Supplies Loaded from " << filename << " ===\n";
    for (int i = 0; i < count; ++i) {
        stringstream ss(lines[i]);
        string idStr, type, qtyStr, batch;
        if (!getline(ss, idStr, ',')) continue;
        if (!getline(ss, type, ',')) continue;
        if (!getline(ss, qtyStr, ',')) continue;
        if (!getline(ss, batch, ',')) continue;

        // trim helper
        auto trim = [](string &s) {
            while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
            while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
        };
        trim(idStr); trim(type); trim(qtyStr); trim(batch);

        try {
            int id = stoi(idStr);
            int qty = stoi(qtyStr);
            if (!isFull()) supplies[++top] = MedicalSupply(id, type, qty, batch);
            loaded++;
            if (id >= nextID) nextID = id + 1;
            cout << "ID: " << id
                 << " | Type: " << type
                 << " | Qty: " << qty
                 << " | Batch: " << batch << "\n";
        } catch (...) {
            cout << "[Warning] Skipping malformed line: " << lines[i] << "\n";
        }
    }
    delete[] lines;
    cout << "============================================\n";
}

// saveToFile
void MedicalSupplyStack::saveToFile() const {
    int size = top + 1;
    if (size <= 0) {
        // Write zero lines by calling writer with an empty array
        string *emptyArr = new string[0];
        TextFileManager::writeLinesToFile(filename, emptyArr, 0);
        delete[] emptyArr;
        cout << "[Info] Saved 0 supplies to " << filename << " (file cleared).\n";
        return;
    }
    string *lines = new string[size];
    for (int i = 0; i < size; ++i) {
        lines[i] = to_string(supplies[i].id) + "," + supplies[i].type + "," +
                   to_string(supplies[i].quantity) + "," + supplies[i].batch;
    }
    TextFileManager::writeLinesToFile(filename, lines, size);
    delete[] lines;
    cout << "[Info] Saved " << size << " supplies to " << filename << ".\n";
}