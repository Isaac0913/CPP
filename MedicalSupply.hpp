#ifndef MEDICAL_SUPPLY_HPP
#define MEDICAL_SUPPLY_HPP  

#include <string>
using namespace std;

struct MedicalSupply {
    int id;
    string type;
    int quantity;
    string batch; 

    MedicalSupply() : id(0), type(""), quantity(0), batch("") {}
    MedicalSupply(int i, const string& t, int q, const string& b) : id(i), type(t), quantity(q), batch(b) {}  

};
#endif