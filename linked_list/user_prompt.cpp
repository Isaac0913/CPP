#include "user_prompt.hpp"
#include "string_utils.hpp"
#include "cleaner_loader.hpp"
#include <iostream>
#include <limits>

int promptSortAlgorithm()
{
    int c;
    cout << "Choose sorting algorithm:\n"
         << "1. Merge Sort\n"
         << "2. Insertion Sort\n"
         << "Choice (1-2): ";
    cin >> c;
    if (c != 1 && c != 2)
        c = 1; // default merge sort
    return c;
}

// this can be used to set the SearchMode enum value
int promptSearchAlgorithm()
{
    int c;
    cout << "Choose search algorithm:\n"
         << "1. Linear Search\n"
         << "2. Two-Pointer Search\n"
         << "Choice (1-2): ";
    cin >> c;
    if (c != 1 && c != 2)
        c = 2; // default two-pointer
    return c;
}

// use this when prompting for search query
SearchQueryData promptSearchQuery(bool clearBuffer)
{
    if (clearBuffer)
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Enter search text (skills/keywords): ";
    string query;
    getline(cin, query);

    return parseSearchQuery(query);
}

SearchQueryData parseSearchQuery(const string &userQuery)
{
    SearchQueryData queryData{};
    string clean = normalizeText(userQuery);

    extractSkills(clean, queryData.skills, queryData.skillCount);
    string roleOnly = removeSkillsFromQuery(clean);
    queryData.roleCount = tokenizeWords(roleOnly, queryData.roles, 50);
    for (int i = 0; i < queryData.roleCount; ++i)
    {
        queryData.roles[i] = toLowerCopy(queryData.roles[i]);
    }

    return queryData;
}

// struct MatchQueryData
// {
//     string jobRole;
//     string skills[50];
//     int skillCount;
//     double thresholdPct;
// };

MatchQueryData promptMatchQuery(bool clearBuffer, SearchMode searchAlgo)
{
    if (clearBuffer)
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    MatchQueryData queryData{};

    cout << "Job position entered: ";
    string jobPosition;
    getline(cin, jobPosition);
    queryData.jobRole = jobPosition;

    cout << "Skills entered: ";
    string userQuerySkills;
    getline(cin, userQuerySkills);
    string clean = normalizeText(userQuerySkills);
    extractSkills(clean, queryData.skills, queryData.skillCount);

    double thresholdPct = 0.0;
    cout << "Enter threshold % (0 - 100): ";
    cin >> thresholdPct;
    queryData.thresholdPct = thresholdPct;

    cout << "\n===== Job Matching ("
         << (searchAlgo == MODE_LINEAR ? "Linear" : "Two-pointer")
         << ") =====\n";
    cout << "Job position entered: " << jobPosition << "\n";
    cout << "Skills entered: " << userQuerySkills << "\n";
    cout << "Threshold: " << thresholdPct << "%\n\n";

    return queryData;
}
