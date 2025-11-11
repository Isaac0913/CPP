#include "EmergencyDepartment.hpp"
#include "TextFileManager.hpp"
#include <iostream>
#include <sstream>

using namespace std;

const char* EmergencyDepartment::FILENAME = "case.txt";

EmergencyDepartment::EmergencyDepartment() : size_(0) {
    loadFromFile();
}

void EmergencyDepartment::insertSorted(const EmergencyCase& ec) {
    if (size_ >= MAX) {
        cout << "Queue full! Cannot add more cases.\n";
        return;
    }
    int i = size_ - 1;
    while (i >= 0 && cases_[i].priority > ec.priority) {
        cases_[i + 1] = cases_[i];
        --i;
    }
    cases_[i + 1] = ec;
    ++size_;
}

void EmergencyDepartment::logEmergencyCase(const string& name, const string& type, int priority) {
    EmergencyCase ec { name, type, priority };
    insertSorted(ec);
    saveToFile();
    cout << "Emergency case logged and saved successfully!\n";
}

void EmergencyDepartment::processMostCriticalCase() {
    if (size_ == 0) {
        cout << "No emergency cases to process.\n";
        return;
    }

    cout << "\nProcessing most critical case:\n";
    cout << "Patient:  " << cases_[0].patientName  << endl;
    cout << "Type:     " << cases_[0].emergencyType << endl;
    cout << "Priority: " << cases_[0].priority      << endl;

    for (int i = 1; i < size_; ++i) {
        cases_[i - 1] = cases_[i];
    }
    --size_;
    saveToFile();
}

void EmergencyDepartment::viewPendingCases() const {
    if (size_ == 0) {
        cout << "No pending emergency cases.\n";
        return;
    }

    cout << "\n--- Pending Emergency Cases (by Priority) ---\n";
    for (int i = 0; i < size_; ++i) {
        cout << (i + 1) << ". "
             << cases_[i].patientName  << " | "
             << cases_[i].emergencyType << " | "
             << "Priority: " << cases_[i].priority << endl;
    }
    cout << endl;
}

void EmergencyDepartment::saveToFile() const {
    string* lines = new string[size_];
    for (int i = 0; i < size_; ++i) {
        lines[i] = cases_[i].patientName + "," +
                   cases_[i].emergencyType + "," +
                   to_string(cases_[i].priority);
    }
    TextFileManager::writeLinesToFile(FILENAME, lines, size_);
    delete[] lines;
}

void EmergencyDepartment::loadFromFile() {
    int lineCount = 0;
    string* lines = TextFileManager::readLinesFromFile(FILENAME, lineCount);

    size_ = 0;
    if (!lines || lineCount <= 0) {
        if (lines) delete[] lines;
        return;
    }

    for (int i = 0; i < lineCount && i < MAX; ++i) {
        string line = lines[i];
        stringstream ss(line);
        string name, type, priorityStr;

        getline(ss, name, ',');
        getline(ss, type, ',');
        getline(ss, priorityStr, ',');

        if (!name.empty() && !type.empty() && !priorityStr.empty()) {
            EmergencyCase ec;
            ec.patientName = name;
            ec.emergencyType = type;
            try {
                ec.priority = stoi(priorityStr);
            } catch (...) {
                ec.priority = 5;
            }
            insertSorted(ec);
        }
    }

    delete[] lines;
}
