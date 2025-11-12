#include <iostream>
#include "MedicalSupplyStack.hpp"

void runMedicalSupplyMenu() {
    MedicalSupplyStack manager("medical.txt");
    int choice;

    do {
        cout << "\n1. Add Supply\n2. Use Last Added Supply\n3. View Supplies\n4. Exit\nChoice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1000,'\n'); continue; }
        cin.ignore(1000,'\n');
        switch (choice) {
            case 1: manager.addSupply(); break;
            case 2: {
                MedicalSupply  out;
                if (manager.useLastSupply(out)) {
                    cout << "ID: " << out.id << " Used: " << out.type << " Batch:" << out.batch << "\n";
                } else {
                    cout << "No supplies.\n";
                }
                break;
            }
            case 3: manager.viewSupplies(); break;
            case 4: cout << "Bye\n"; break;
            default: cout << "Invalid\n";
        }
    } while (choice != 4);
}