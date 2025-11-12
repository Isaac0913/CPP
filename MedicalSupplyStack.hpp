#ifndef MEDICAL_SUPPLY_STACK_HPP
#define MEDICAL_SUPPLY_STACK_HPP

#include "MedicalSupply.hpp"
#include "TextFileManager.hpp"
#include <string>

using namespace std;

class MedicalSupplyStack  {
private:
    static const int MAX_SIZE = 100; // or use dynamic array if you prefer
    MedicalSupply supplies[MAX_SIZE];
    int top; // -1 means empty
    string filename;
    int nextID; // default file to load/save

public:
    MedicalSupplyStack (const string& file = "medical.txt");
    // core stack functions
    bool isEmpty() const;
    bool isFull() const;
    void addSupply();    // Function 1: Add Supply Stock
    bool useLastSupply(MedicalSupply &out);    // Function 2: Use Last Added Supply (returns popped item)
    void viewSupplies() const;    // Function 3: View Current Supplies

    // file I/O helpers
    void loadFromFile();
    void saveToFile() const;
};

#endif 