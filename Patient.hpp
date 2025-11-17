#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <string>

class Patient {
private:
    std::string id;
    std::string name;
    std::string condition;

public:
    // constructors
    Patient() : id(""), name(""), condition("") {}
    Patient(const std::string& id, const std::string& name, const std::string& condition)
        : id(id), name(name), condition(condition) {
    }

    // getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCondition() const { return condition; }

    // setters
    void setId(const std::string& v) { id = v; }
    void setName(const std::string& v) { name = v; }
    void setCondition(const std::string& v) { condition = v; }
};

#endif // PATIENT_HPP
