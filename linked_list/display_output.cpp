#include "display_output.hpp"
#include "score_struct.hpp"
#include "user_prompt.hpp"
#include <iostream>

void ldisplayJobMultiLine(const lJob &j, int index)
{
    cout << index << ". ID: " << j.id << "\n   Role: " << j.role << "\n   Skills: [";
    for (int i = 0; i < j.skillCount; ++i)
    {
        cout << j.skills[i];
        if (i < j.skillCount - 1)
            cout << ", ";
    }
    cout << "]\n   Raw: " << j.raw_text << "\n\n";
}

void ldisplayResumeMultiLine(const lResume &r, int index)
{
    cout << index++ << ". ID: " << r.id << "\n   Skills: [";
    for (int i = 0; i < r.skillCount; ++i)
    {
        cout << r.skills[i];
        if (i < r.skillCount - 1)
            cout << ", ";
    }
    cout << "]\n   Raw: " << r.raw_text << "\n\n";
}

void lprintJobLine(const lJob &j)
{
    cout << j.id << "  " << j.role << "  [";
    for (int k = 0; k < j.skillCount; k++)
    {
        cout << j.skills[k];
        if (k < j.skillCount - 1)
            cout << ", ";
    }
    cout << "]\n";
}

void lprintResumeLine(const lResume &r)
{
    cout << r.id << "  [";
    for (int k = 0; k < r.skillCount; k++)
    {
        cout << r.skills[k];
        if (k < r.skillCount - 1)
            cout << ", ";
    }
    cout << "]\n";
}

void ldisplayMatchedResume(const lResume &R, const lScoredRes &m, const MatchQueryData &matchQuery, int denom, int index)
{
    cout << index + 1 << ". Resume ID " << R.id
         << "  Skill matched " << m.overlap << "/" << denom
         << " (" << (int)(m.pct + 0.5) << "%)\n";

    cout << "   Skills: [";
    for (int k = 0; k < R.skillCount; ++k)
    {
        cout << R.skills[k];
        if (k < R.skillCount - 1)
            cout << ", ";
    }
    cout << "]\n";

    if (matchQuery.skillCount > 0)
    {
        cout << "   | Matched user skills: [";
        bool first = true;
        for (int us = 0; us < matchQuery.skillCount; ++us)
        {
            for (int rk = 0; rk < R.skillCount; ++rk)
            {
                if (matchQuery.skills[us] == R.skills[rk])
                {
                    if (!first)
                        cout << ", ";
                    cout << matchQuery.skills[us];
                    first = false;
                    break;
                }
            }
        }
        cout << "]\n";
    }
    cout << "\n";
}

void ldisplayNoMatches()
{
    cout << "No matches found!" << endl
         << endl;
}