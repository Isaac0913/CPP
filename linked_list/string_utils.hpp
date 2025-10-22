#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
using namespace std;

// to create copy of the string + converted to lowercase
string ltoLowerCopy(const string &s);

// formerly sortSkills
// to sort an array of strings by alphabet order A ~ Z
void lsortStringArr(string arr[], int n);

string cleanQuotes(const string &s);

int ltokenizeWords(const string &s, string outTokens[], int maxTokens);

string lremoveSkillsFromQuery(const string &normQuery);

#endif
