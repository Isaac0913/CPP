#ifndef PROMPT_HPP
#define PROMPT_HPP

#include <string>
using namespace std;

// later do this in menu (get user input):
// SearchMode searchMode = mode == value ? MODE_LINEAR : MODE_TWO_POINTER;
enum SearchMode
{
    MODE_LINEAR = 1,
    MODE_TWO_POINTER = 2
};

// later do this in menu (get user input):
// SearchType searchType = type == value ? TYPE_JOBS : TYPE_RESUME;
enum SearchType
{
    TYPE_JOBS = 1,
    TYPE_RESUME = 2
};

// data of user query
struct SearchQueryData
{
    string skills[50];
    int skillCount;
    string roles[50];
    int roleCount;
};

int promptSortAlgorithm();

int promptSearchAlgorithm();

SearchQueryData parseSearchQuery(const string &userQuery);

#endif
