#ifndef AMBULANCE_DISPATCH_MENU_HPP
#define AMBULANCE_DISPATCH_MENU_HPP

#include "AmbulanceDispatcher.hpp"
#include <limits>

void runAmbulanceDispatcherMenu()
{
    int input;
    AmbulanceDispatcher ambulanceDispatcher; // instantiate the dispatcher

    do
    {
        cout << "\n--------------------------------------------------\n";
        cout << "              AMBULANCE DISPATCHER\n";
        cout << "--------------------------------------------------\n";
        cout << "1. Register Ambulance\n";
        cout << "2. Rotate Ambulance Shift\n";
        cout << "3. Display Ambulance Schedule\n";
        cout << "0. Return to Main Menu\n";
        cout << "--------------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> input;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out newline

        switch (input)
        {
        case 1:
        {
            Ambulance newAmbulance = ambulanceDispatcher.createNewAmbulance();
            ambulanceDispatcher.registerAmbulance(newAmbulance, true);
            break;
        }

        case 2:
        {
            ambulanceDispatcher.rotateShift();
            break;
        }

        case 3:
        {
            ambulanceDispatcher.displaySchedule();
            break;
        }
        case 0:
        {
            cout << "Returning to Main Menu...\n";
            break;
        }
        default:
            cout << "Invalid choice! Please try again!\n";
        }
    } while (input != 0);
}

#endif