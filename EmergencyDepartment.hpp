#ifndef EMERGENCY_DEPARTMENT_HPP
#define EMERGENCY_DEPARTMENT_HPP

#include <string>
#include "EmergencyCase.hpp" // include your entity struct

class EmergencyDepartment {
public:
    EmergencyDepartment();

    void logEmergencyCase(const std::string& name, const std::string& type, int priority);
    void processMostCriticalCase();
    void viewPendingCases() const;

private:
    static const int MAX = 100;
    static const char* FILENAME;

    EmergencyCase cases_[MAX];
    int size_;

    void saveToFile() const;
    void loadFromFile();
    void insertSorted(const EmergencyCase& ec);
};

#endif // EMERGENCY_DEPARTMENT_HPP
