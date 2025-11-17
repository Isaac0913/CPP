#ifndef PATIENT_ADMISSION_CLERK_MENU_HPP
#define PATIENT_ADMISSION_CLERK_MENU_HPP

#include <iostream>
#include <limits>
#include "PatientAdmissionClerk.hpp"

// Define once in the translation unit that includes this header.
// If you include this header in multiple .cpp files, move these to a .cpp file.
const std::string PATIENT_FILE = "patients.txt";
PatientQueue queue;

// robust wait: read until newline (blocks until user presses Enter)
inline void waitEnter() {
std::cout << "\nPress Enter to continue...";
std::string tmp;
std::getline(std::cin, tmp); // consumes leftover newline if any (non-blocking if none)
std::getline(std::cin, tmp); // waits for the actual Enter press
}

// Single-loop menu: load once, reprint after each action
inline void runPatientMenu() {
// load once when entering the menu
if (!queue.loadFromFile(PATIENT_FILE)) {
std::cout << "No patient file found. Starting fresh.\n";
}

while (true) {
std::cout << "\n--------------------------------------------------\n";
std::cout << "              PATIENT ADMISSION CLERK\n";
std::cout << "--------------------------------------------------\n";
std::cout << "1. Admit Patient\n";
std::cout << "2. Discharge Patient\n";
std::cout << "3. View Patient Queue\n";
std::cout << "0. Return to Main Menu\n";
std::cout << "--------------------------------------------------\n";
std::cout << "Enter your choice: ";

int choice;
if (!(std::cin >> choice)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input. Try again.\n";
    continue;
}

// After reading with operator>>, remove the remaining newline so getline works later
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

if (choice == 0) {
    queue.saveToFile(PATIENT_FILE);
    break; // return to main menu
}

if (choice == 1) {
    std::string id, name, condition;
    std::cout << "Enter patient ID: ";
    std::getline(std::cin, id);
    std::cout << "Enter patient name: ";
    std::getline(std::cin, name);
    std::cout << "Enter condition/type: ";
    std::getline(std::cin, condition);

    // simple validation (optional)
    if (id.empty() || name.empty()) {
        std::cout << "ID and name cannot be empty. Admission cancelled.\n";
        waitEnter();
        continue;
    }

    Patient p(id, name, condition);
    queue.admitPatient(p);
    queue.saveToFile(PATIENT_FILE);
    std::cout << "Patient admitted.\n";
    waitEnter();
    continue; // re-show menu
}

if (choice == 2) {
    Patient p;
    if (queue.dischargePatient(p)) {
        std::cout << "Discharged patient: ID=" << p.getId()
            << " | Name=" << p.getName()
            << " | Condition=" << p.getCondition() << "\n";
        queue.saveToFile(PATIENT_FILE);
    }
    else {
        std::cout << "Queue is empty; nothing to discharge.\n";
    }
    waitEnter();
    continue; // re-show menu
}

if (choice == 3) {
    queue.viewPatientQueue();
    waitEnter();
    continue; // re-show menu
}

std::cout << "Unknown option. Try again.\n";
} // while
}

#endif // PATIENT_ADMISSION_CLERK_MENU_HPPNU_HPP