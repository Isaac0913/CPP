#ifndef EMERGENCY_CASE_HPP
#define EMERGENCY_CASE_HPP

#include <string>

struct EmergencyCase {
    std::string patientName;
    std::string emergencyType;
    int priority; // 1 = highest, 5 = lowest
};

#endif // EMERGENCY_CASE_HPP
