#include "display_output.hpp"
#include <iostream>

void displayJobMultiLine(const Job &j, int index)
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

void displayResumeMultiLine(const Resume &r, int index)
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

void printJobLine(const Job &j)
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

void printResumeLine(const Resume &r)
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
