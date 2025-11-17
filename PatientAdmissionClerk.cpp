#include "PatientAdmissionClerk.hpp"
#include <sstream>
#include "TextFileManager.hpp"
#include <iostream>
#include <filesystem>
#include <direct.h>  //

using namespace std;

const std::string PATIENT_FILE = "patients.txt";

PatientQueue::PatientQueue() : head(nullptr), tail(nullptr), size(0) {}

PatientQueue::~PatientQueue() {
    while (head != nullptr) {
        PatientNode* tmp = head;
        head = head->next;
        delete tmp;
    }
    tail = nullptr;
    size = 0;
}

void PatientQueue::admitPatient(const Patient& p) {
    PatientNode* node = new PatientNode(p);
    if (tail == nullptr) { // empty queue
        head = tail = node;
    }
    else {
        tail->next = node;
        tail = node;
    }
    ++size;
}

bool PatientQueue::dischargePatient(Patient& out) {
    if (head == nullptr) return false;
    PatientNode* node = head;
    out = node->data;
    head = head->next;
    if (head == nullptr) tail = nullptr;
    delete node;
    --size;
    return true;
}

void PatientQueue::viewPatientQueue() const {
    if (head == nullptr) {
        std::cout << "Patient queue is empty.\n";
        return;
    }
    PatientNode* curr = head;
    std::cout << "Patients in queue (front -> rear):\n";
    int idx = 1;
    while (curr != nullptr) {
        std::cout << idx++ << ". ID: " << curr->data.getId()
            << " | Name: " << curr->data.getName()
            << " | Condition: " << curr->data.getCondition() << "\n";
        curr = curr->next;
    }
}

int PatientQueue::getSize() const { return size; }

Patient PatientQueue::parseLine(const std::string& line) {
    // expected format: ID|Name|Condition
    std::string id, name, condition;
    std::istringstream ss(line);
    std::getline(ss, id, '|');
    std::getline(ss, name, '|');
    std::getline(ss, condition);
    return Patient(id, name, condition);
}

std::string PatientQueue::formatPatient(const Patient& p)const {
    // format: ID|Name|Condition
    return p.getId() + "|" + p.getName() + "|" + p.getCondition();
}

bool PatientQueue::loadFromFile(const std::string& filename) {
    int lineCount = 0;
    std::string* lines = TextFileManager::readLinesFromFile(filename, lineCount);
    if (lines == nullptr || lineCount == 0) {
        if (lines) delete[] lines;
        return false; // nothing loaded (file missing or empty)
    }

    // Clear existing queue first
    while (head != nullptr) {
        PatientNode* tmp = head;
        head = head->next;
        delete tmp;
    }
    tail = nullptr;
    size = 0;

    for (int i = 0; i < lineCount; ++i) {
        Patient p = parseLine(lines[i]);
        admitPatient(p);
    }

    delete[] lines;
    return true;
}
bool PatientQueue::saveToFile(const std::string& filename) const {
    if (head == nullptr) {
        // write empty file (0 lines)
        string* empty = nullptr;
        int count = 0;
        // TextFileManager writes from array; to create empty file just open and close
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error writing to file: " << filename << std::endl;
            return false;
        }
        file.close();
        return true;
    }

    // collect lines
    int count = size;
    string* lines = new string[count];
    PatientNode* curr = head;
    int idx = 0;
    while (curr != nullptr) {
        lines[idx++] = formatPatient(curr->data);
        curr = curr->next;
    }

    TextFileManager::writeLinesToFile(filename, lines, count);
    delete[] lines;
    char buff[512];
  
    return true;
}

