#ifndef COUNT_MATCH_HPP
#define COUNT_MATCH_HPP

#include <string>
using namespace std;

int countSkillMatchesLinear(const string a[], int na, const string b[], int nb);

int countSkillMatchesTwoPointer(const string a[], int na, const string b[], int nb);

int roleHitCount(const string &roleLower, const string roleTokens[], int tokCount);

#endif
