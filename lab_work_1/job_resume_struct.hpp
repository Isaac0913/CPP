#ifndef JOB_RESUME_STRUCT_HPP
#define JOB_RESUME_STRUCT_HPP

#include <string>
using namespace std;

// we use the same Job and Resume objects/structs for both array and linked list
struct Job
{
    int id;
    string role;
    string skills[50];
    int skillCount;
    string raw_text;
};

struct Resume
{
    int id;
    string skills[50];
    int skillCount;
    string raw_text;
};

#endif
