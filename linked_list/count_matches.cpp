#include "count_matches.hpp"

int lcountSkillMatchesLinear(const string a[], int na, const string b[], int nb)
{
    int hits = 0;
    for (int i = 0; i < na; ++i)
    {
        for (int j = 0; j < nb; ++j)
        {
            if (a[i] == b[j])
            {
                ++hits;
                break; // avoid double-counting this a[i]
            }
        }
    }
    return hits;
}

int lcountSkillMatchesTwoPointer(const string a[], int na, const string b[], int nb)
{
    int i = 0, j = 0, hits = 0;
    while (i < na && j < nb)
    {
        if (a[i] == b[j])
        {
            ++hits;
            ++i;
            ++j;
        }
        else if (a[i] < b[j])
            ++i;
        else
            ++j;
    }
    return hits;
}

int lroleHitCount(const string &roleLower, const string roleTokens[], int tokCount)
{
    int hits = 0;
    for (int i = 0; i < tokCount; ++i)
    {
        if (roleLower.find(roleTokens[i]) != string::npos)
            ++hits;
    }
    return hits;
}
