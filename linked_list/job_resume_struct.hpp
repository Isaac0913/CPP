#ifndef JOB_RESUME_STRUCT_HPP
#define JOB_RESUME_STRUCT_HPP

#include <string>
using namespace std;

// we use the same Job and Resume objects/structs for both array and linked list
struct lJob
{
    int id;
    string role;
    string skills[50];
    int skillCount;
    string raw_text;
};

struct lResume
{
    int id;
    string skills[50];
    int skillCount;
    string raw_text;
};

#endif
