#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
using namespace std;

// to create copy of the string + converted to lowercase
string toLowerCopy(const string &s);

// formerly sortSkills
// to sort an array of strings by alphabet order A ~ Z
void sortStringArr(string arr[], int n);

string cleanQuotes(const string &s);

int tokenizeWords(const string &s, string outTokens[], int maxTokens);

string removeSkillsFromQuery(const string &normQuery);

#endif
