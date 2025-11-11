#ifndef ED_MENU_HPP
#define ED_MENU_HPP

#include <iostream>
#include <string>
#include "EmergencyDepartment.hpp"

inline void runEmergencyDepartmentMenu() {
    using namespace std;

    EmergencyDepartment ED;  // internally managed instance

    int choice = 0;
    string name, type;
    int priority = 0;

    do {
        cout << "\n=== Emergency Department (Role 3) ===\n";
        cout << "1. Log Emergency Case\n";
        cout << "2. Process Most Critical Case\n";
        cout << "3. View Pending Emergency Cases\n";
        cout << "4. Return to Main Menu\n";
        cout << "Choose an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter patient name: ";
                getline(cin, name);
                cout << "Enter emergency type: ";
                getline(cin, type);
                cout << "Enter priority (1 = Highest, 5 = Lowest): ";
                if (!(cin >> priority)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid priority.\n";
                    break;
                }
                cin.ignore();

                if (priority < 1) priority = 1;
                if (priority > 5) priority = 5;

                ED.logEmergencyCase(name, type, priority);
                break;

            case 2:
                ED.processMostCriticalCase();
                break;

            case 3:
                ED.viewPendingCases();
                break;

            case 4:
                cout << "Returning to Main Menu...\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);
}

#endif // ED_MENU_HPP
