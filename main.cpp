#include "AmbulanceDispatcherMenu.hpp"
#include "EmergencyDepartmentMenu.hpp"
#include "MedicalSupplyMenu.hpp"

// REMEMBER TO COMPILE ALL FILES { g++ *.cpp -o main }

int main()
{
    int choice = -1;
    do
    {
        cout << "\n--------------------------------------------------\n";
        cout << "           HOSPITAL MANAGEMENT SYSTEM\n";
        cout << "--------------------------------------------------\n";
        cout << "Select Role:\n";
        cout << "1. Patient Admission Clerk\n";
        cout << "2. Medical Supply Manager\n";
        cout << "3. Emergency Department Officer\n";
        cout << "4. Ambulance Dispatcher\n";
        cout << "0. Exit\n";
        cout << "--------------------------------------------------\n";
        cout << "Enter your choice: ";

        // if entered a non int
        if (!(cin >> choice))
        {
            cin.clear();             // clear error state
            cin.ignore(10000, '\n'); // discard invalid input
            cout << "Invalid input! Please enter a number.\n\n";
            choice = -1; // prevent accidental choice = 0;
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            // patient menu here
            break;
        }
        case 2:
        {
            cout << "\n\n";
            runMedicalSupplyMenu();
            break;
        }
        case 3:
        {
            cout << "\n\n";
            runEmergencyDepartmentMenu();
            break;
        }
        case 4:
        {
            cout << "\n";
            runAmbulanceDispatcherMenu();
            break;
        }
        case 0:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}