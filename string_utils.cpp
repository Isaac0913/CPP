#include "string_utils.hpp"
#include "cleaner_loader.hpp"

string toLowerCopy(const string &s)
{
    string out = s;
    for (size_t i = 0; i < out.size(); ++i)
        out[i] = static_cast<char>(tolower(static_cast<unsigned char>(out[i])));
    return out;
}

void sortStringArr(string arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        string key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

string cleanQuotes(const string &s)
{
    string out;
    for (char c : s)
    {
        if (c != '"' && c != '\'')
            out += c;
    }
    return out;
}

// note that this function returns only the count of the tokenized words
// used for user-entered job roles
int tokenizeWords(const string &s, string outTokens[], int maxTokens)
{
    // data analyst software engineer
    // ['data', 'analyst', ]
    int count = 0;
    size_t i = 0;
    while (i < s.size())
    {
        while (i < s.size() && isspace((unsigned char)s[i]))
            ++i;
        if (i >= s.size())
            break;
        size_t j = i;
        while (j < s.size() && !isspace((unsigned char)s[j]))
            ++j;
        if (count < maxTokens)
            outTokens[count++] = s.substr(i, j - i);
        i = j;
    }
    return count;
}

string removeSkillsFromQuery(const string &normQuery)
{
    string s = normQuery;
    for (int i = 0; i < SKILL_COUNT; ++i)
    {
        const string skill = SKILLS[i];
        size_t pos = 0;
        while (true)
        {
            pos = s.find(skill, pos);
            if (pos == string::npos)
                break;
            bool left_ok = (pos == 0) || !isalnum((unsigned char)s[pos - 1]);
            bool right_ok = (pos + skill.length() == s.length()) || !isalnum((unsigned char)s[pos + skill.length()]);
            if (left_ok && right_ok)
            {
                for (size_t k = 0; k < skill.length(); ++k)
                    s[pos + k] = ' ';
                pos += skill.length();
            }
            else
            {
                pos += 1;
            }
        }
    }
    string out;
    bool space = false;
    for (char c : s)
    {
        if (isspace((unsigned char)c))
        {
            if (!space)
                out += ' ';
            space = true;
        }
        else
        {
            out += c;
            space = false;
        }
    }
    return out;
}

string resumeSortKey(const Resume &r)
{
    string key;
    for (int i = 0; i < r.skillCount; ++i)
    {
        key += r.skills[i];
        key += '|';
    }
    return toLowerCopy(key);
}