#ifndef PATIENT_ADMISSION_CLERK_HPP
#define PATIENT_ADMISSION_CLERK_HPP

#include "Patient.hpp"
#include "TextFileManager.hpp"
#include <string>
#include <iostream>

struct PatientNode {
    Patient data;
    PatientNode* next;
    PatientNode(const Patient& p) : data(p), next(nullptr) {}
};

class PatientQueue {
private:
    PatientNode* head; // dequeue from head
    PatientNode* tail; // enqueue at tail
    int size;

    // helpers for parsing/formatting
    Patient parseLine(const std::string& line);
    std::string formatPatient(const Patient& p)const;

public:
    PatientQueue();
    ~PatientQueue();

    // core functionalities
    void admitPatient(const Patient& p);    // enqueue
    bool dischargePatient(Patient& out);    // dequeue; returns false if empty
    void viewPatientQueue() const;
    int getSize() const;

    // persistence
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
};

#endif // PATIENT_QUEUE_HPP
