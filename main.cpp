#include <iostream>
#include "linked_list_menu.hpp"
#include "array_menu.hpp"

using namespace std;

int main()
{
    int dsChoice;
    do
    {
        cout << "\n===== SELECT DATA STRUCTURE =====\n"
             << "1. Array\n"
             << "2. Linked List\n"
             << "0. Exit\n"
             << "Choice (0-2): ";
        cin >> dsChoice;

        switch (dsChoice)
        {
        case 1:
        {
            arrayMainMenuEntry();
            break;
        }

        case 2:
        {
            // --- LINKED LIST DATA STRUCTURE SELECTED ---
            linkedListMainMenu();
            break;
        }

        case 0:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (dsChoice != 0);

    return 0;
}