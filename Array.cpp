#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <windows.h>
#include <psapi.h>

#include "linked_list/linked_list_menu.hpp"

using namespace std;

bool containsAllSkillsTwoPointer(const string a[], int na, const string b[], int nb)
{
    int i = 0, j = 0;
    while (i < na && j < nb)
    {
        if (a[i] == b[j])
        {
            ++i;
            ++j;
        }
        else if (a[i] < b[j])
        {
            ++i;
        }
        else
        {
            // a[i] > b[j] means the job/resume is missing b[j]
            return false;
        }
    }
    return (j == nb); // true only if all skills in b were found in a
}

size_t getMemoryUsageKB()
{
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter)))
    {
        return memCounter.WorkingSetSize / 1024; // Convert bytes to KB
    }
    return 0;
}

enum SearchMode
{
    MODE_LINEAR = 1,
    MODE_TWO_POINTER = 2
};
SearchMode g_searchMode = MODE_TWO_POINTER;

// ----------- Skill dictionary (array) ------------
const char *SKILLS[] = {
    "sql", "mysql", "postgresql", "postgres", "mongodb", "oracle", "sqlite", "redis", "nosql", "database",
    "powerbi", "power bi", "excel", "tableau", "d3", "matplotlib", "seaborn", "plotly",
    "python", "java", "javascript", "c++", "c#", "php", "ruby", "go", "rust",
    "swift", "kotlin", "scala", "r", "matlab", "typescript",
    "react", "angular", "vue", "django", "flask", "spring boot", "express",
    "laravel", "rails", "bootstrap", "jquery", "tensorflow", "pytorch", "keras",
    "scikit", "pandas", "numpy",
    "git", "docker", "kubernetes", "aws", "azure", "gcp", "linux", "jenkins", "jira",
    "restapi", "restapis", "rest api", "rest apis", "nlp", "machinelearning",
    "machine learning", "deeplearning", "deep learning", "datacleaning", "data cleaning", "mlops",
    "systemdesign", "system design", "agile", "scrum", "kanban",
    "stakeholdermanagement", "stakeholder management", "userstories", "user stories",
    "productroadmap", "product roadmap", "statistics", "computervision", "computer vision", "cloud"};
const int SKILL_COUNT = sizeof(SKILLS) / sizeof(SKILLS[0]);

// ----------- Normalize text ------------
string normalizeText(string input)
{
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    replace_if(input.begin(), input.end(), [](unsigned char c)
               { return !isalnum(c) && !isspace(c); }, ' ');

    const string FROM[] = {
        "powerbi", "power-bi",
        "machinelearning", "deeplearning", "datacleaning",
        "systemdesign", "system-design",
        "stakeholdermanagement", "userstories", "productroadmap"};
    const string TO[] = {
        "power bi", "power bi",
        "machine learning", "deep learning", "data cleaning",
        "system design", "system design",
        "stakeholder management", "user stories", "product roadmap"};
    const int COUNT = sizeof(FROM) / sizeof(FROM[0]);

    for (int i = 0; i < COUNT; i++)
    {
        size_t pos;
        while ((pos = input.find(FROM[i])) != string::npos)
            input.replace(pos, FROM[i].length(), TO[i]);
    }

    string clean;
    bool space = false;
    for (char c : input)
    {
        if (isspace(c))
        {
            if (!space)
                clean += ' ';
            space = true;
        }
        else
        {
            clean += c;
            space = false;
        }
    }
    return clean;
}

string toLowerCopy(const string &s)
{
    string out = s;
    for (size_t i = 0; i < out.size(); ++i)
        out[i] = static_cast<char>(tolower(static_cast<unsigned char>(out[i])));
    return out;
}

// ----------- Job Structures ------------
struct Job
{
    int id;
    string role;
    string skills[50];
    int skillCount;
    string raw_text;
};

class JobArray
{
private:
    Job *arr;
    int capacity;
    int size;

public:
    JobArray(int cap = 20000)
    {
        capacity = cap;
        size = 0;
        arr = new Job[capacity];
    }
    ~JobArray() { delete[] arr; }
    void insert(const Job &j)
    {
        if (size < capacity)
            arr[size++] = j;
    }
    int getSize() const { return size; }
    Job *getArray() { return arr; }
    const Job *getArray() const { return arr; }
};

// ----------- Resume Structures ------------
struct Resume
{
    int id;
    string skills[50];
    int skillCount;
    string raw_text;
};

class ResumeArray
{
private:
    Resume *arr;
    int capacity;
    int size;

public:
    ResumeArray(int cap = 20000) : capacity(cap), size(0)
    {
        arr = new Resume[capacity];
    }
    ~ResumeArray() { delete[] arr; }
    void insert(const Resume &r)
    {
        if (size < capacity)
            arr[size++] = r;
    }
    int getSize() const { return size; }
    Resume *getArray() { return arr; }
    const Resume *getArray() const { return arr; }
};

// ----------- Helpers ------------
void sortSkills(string arr[], int n)
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

void extractSkills(const string &clean_line, string skillArray[], int &skillCount)
{
    skillCount = 0;
    for (int s = 0; s < SKILL_COUNT; s++)
    {
        string skill = SKILLS[s];
        size_t pos = clean_line.find(skill);
        while (pos != string::npos)
        {
            bool left_ok = (pos == 0) || !isalnum(clean_line[pos - 1]);
            bool right_ok = (pos + skill.length() == clean_line.length()) || !isalnum(clean_line[pos + skill.length()]);
            if (left_ok && right_ok)
            {
                bool already = false;
                for (int k = 0; k < skillCount; k++)
                    if (skillArray[k] == skill)
                        already = true;
                if (!already && skillCount < 50)
                    skillArray[skillCount++] = skill;
                break;
            }
            pos = clean_line.find(skill, pos + 1);
        }
    }
    sortSkills(skillArray, skillCount);
}

// ----------- Loaders ------------
void loadJobDescriptions(JobArray &jobs, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening " << filename << "\n";
        return;
    }

    string line;
    int id = 1;
    bool first = true;
    while (getline(file, line))
    {
        if (first)
        {
            first = false;
            continue;
        }
        if (line.empty())
            continue;
        Job j;
        j.id = id++;
        j.raw_text = line;
        istringstream ss(line);
        string w1, w2;
        ss >> w1 >> w2;
        j.role = w1 + " " + w2;
        string clean = normalizeText(line);
        extractSkills(clean, j.skills, j.skillCount);
        if (j.skillCount > 0)
            jobs.insert(j);
    }
    file.close();
}

void loadResumes(ResumeArray &resumes, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening " << filename << "\n";
        return;
    }

    string line;
    int id = 1;
    bool first = true;
    while (getline(file, line))
    {
        if (first)
        {
            first = false;
            continue;
        }
        if (line.empty())
            continue;

        Resume r;
        r.id = id++;
        r.raw_text = line;

        string clean = normalizeText(line);
        extractSkills(clean, r.skills, r.skillCount);
        if (r.skillCount > 0)
            resumes.insert(r);
    }
    file.close();
}

// ----------- Sorting for Jobs ------------
void mergeJobs(Job arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1, n2 = right - mid;
    Job *L = new Job[n1];
    Job *R = new Job[n2];
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        string a = toLowerCopy(L[i].role);
        string b = toLowerCopy(R[j].role);
        if (a <= b)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSortJobs(Job arr[], int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    mergeSortJobs(arr, left, mid);
    mergeSortJobs(arr, mid + 1, right);
    mergeJobs(arr, left, mid, right);
}

// ----------- Sorting Resumes ------------
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

void mergeResumes(Resume arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1, n2 = right - mid;
    Resume *L = new Resume[n1];
    Resume *R = new Resume[n2];
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (resumeSortKey(L[i]) <= resumeSortKey(R[j]))
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSortResumes(Resume arr[], int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    mergeSortResumes(arr, left, mid);
    mergeSortResumes(arr, mid + 1, right);
    mergeResumes(arr, left, mid, right);
}

// ----------- Random + Token helpers ------------
void seedRandOnce()
{
    static bool seeded = false;
    if (!seeded)
    {
        srand((unsigned)time(nullptr));
        seeded = true;
    }
}

void shuffleIndices(int idx[], int n)
{
    seedRandOnce();
    for (int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        int tmp = idx[i];
        idx[i] = idx[j];
        idx[j] = tmp;
    }
}

int tokenizeWords(const string &s, string outTokens[], int maxTokens)
{
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

int countSkillMatchesLinear(const string a[], int na, const string b[], int nb)
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

int countSkillMatchesTwoPointer(const string a[], int na, const string b[], int nb)
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

int roleHitCount(const string &roleLower, const string roleTokens[], int tokCount)
{
    int hits = 0;
    for (int i = 0; i < tokCount; ++i)
    {
        if (roleLower.find(roleTokens[i]) != string::npos)
            ++hits;
    }
    return hits;
}

// ----------- Display ------------
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

// ----------- SEARCH JOBS ------------
void searchJobsTwoPointer(const JobArray &jobs, const string &userQuery, bool useSortedView)
{
    int n = jobs.getSize();
    if (n == 0)
    {
        cout << "No job descriptions loaded.\n";
        return;
    }

    string clean = normalizeText(userQuery);
    string qSkills[50];
    int qCount = 0;
    extractSkills(clean, qSkills, qCount);

    string roleOnlyQ = removeSkillsFromQuery(clean);
    string qRoleTok[50];
    int qRoleCount = tokenizeWords(roleOnlyQ, qRoleTok, 50);
    for (int i = 0; i < qRoleCount; ++i)
        qRoleTok[i] = toLowerCopy(qRoleTok[i]);

    Job *view = nullptr;
    if (!useSortedView)
    {
        view = const_cast<Job *>(jobs.getArray());
        cout << "------unsorted---\nunsorted location (location is the ID)\n";
    }
    else
    {
        view = new Job[n];
        for (int i = 0; i < n; ++i)
            view[i] = jobs.getArray()[i];
        mergeSortJobs(view, 0, n - 1);
        cout << "------sorted----\nsorted location (location is the ID)\n";
    }

    struct ScoreRow
    {
        int idx;
        int skillHits;
        int roleHits;
    };

    ScoreRow rows[20000];
    int r = 0;

    for (int i = 0; i < n; ++i)
    {
        int sHits = 0;
        if (g_searchMode == MODE_LINEAR)
            sHits = countSkillMatchesLinear(view[i].skills, view[i].skillCount, qSkills, qCount);
        else
            sHits = countSkillMatchesTwoPointer(view[i].skills, view[i].skillCount, qSkills, qCount);
        int rHits = roleHitCount(toLowerCopy(view[i].role), qRoleTok, qRoleCount);
        if (sHits == 0 && rHits == 0)
            continue;
        rows[r++] = {i, sHits, rHits};
    }

    if (r == 0)
    {
        cout << "(no matches)\n";
        if (useSortedView)
            delete[] view;
        return;
    }

    for (int i = 0; i < r; i++)
    {
        int best = i;
        for (int j = i + 1; j < r; j++)
        {
            bool better = false;
            if (rows[j].skillHits > rows[best].skillHits)
                better = true;
            else if (rows[j].skillHits == rows[best].skillHits)
            {
                if (rows[j].roleHits > rows[best].roleHits)
                    better = true;
                else if (rows[j].roleHits == rows[best].roleHits)
                    if (view[rows[j].idx].id < view[rows[best].idx].id)
                        better = true;
            }
            if (better)
            {
                ScoreRow tmp = rows[j];
                rows[j] = rows[best];
                rows[best] = tmp;
            }
        }
    }
    int limit = (r < 10 ? r : 10);
    for (int i = 0; i < limit; ++i)
        printJobLine(view[rows[i].idx]);

    if (useSortedView)
        delete[] view;
}

void promptAndSearchJobs(const JobArray &jobs, bool sortedView)
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter search text (skills/keywords): ";
    string query;
    getline(cin, query);

    auto start = chrono::high_resolution_clock::now();
    searchJobsTwoPointer(jobs, query, sortedView);
    auto end = chrono::high_resolution_clock::now();

    cout << "Search completed in " << chrono::duration<double>(end - start).count() << " seconds.\n";
}

// ----------- SEARCH RESUMES ------------
void searchResumesTwoPointer(const ResumeArray &resumes, const string &userQuery, bool useSortedView)
{
    int n = resumes.getSize();
    if (n == 0)
    {
        cout << "No resumes loaded.\n";
        return;
    }

    string clean = normalizeText(userQuery);
    string qSkills[50];
    int qCount = 0;
    extractSkills(clean, qSkills, qCount);

    string roleOnlyQ = removeSkillsFromQuery(clean);
    string qRoleTok[50];
    int qRoleCount = tokenizeWords(roleOnlyQ, qRoleTok, 50);
    for (int i = 0; i < qRoleCount; ++i)
        qRoleTok[i] = toLowerCopy(qRoleTok[i]);

    Resume *view = nullptr;
    if (!useSortedView)
    {
        view = const_cast<Resume *>(resumes.getArray());
        cout << "------unsorted---\nunsorted location (location is the ID)\n";
    }
    else
    {
        view = new Resume[n];
        for (int i = 0; i < n; ++i)
            view[i] = resumes.getArray()[i];
        mergeSortResumes(view, 0, n - 1);
        cout << "------sorted----\nsorted location (location is the ID)\n";
    }

    struct ScoreRow
    {
        int idx;
        int skillHits;
        int roleHits;
    };
    ScoreRow rows[20000];
    int r = 0;

    for (int i = 0; i < n; ++i)
    {
        int sHits = 0;
        if (g_searchMode == MODE_LINEAR)
            sHits = countSkillMatchesLinear(view[i].skills, view[i].skillCount, qSkills, qCount);
        else
            sHits = countSkillMatchesTwoPointer(view[i].skills, view[i].skillCount, qSkills, qCount);

        int rHits = roleHitCount(toLowerCopy(view[i].raw_text), qRoleTok, qRoleCount);
        if (sHits == 0 && rHits == 0)
            continue;
        rows[r++] = {i, sHits, rHits};
    }

    if (r == 0)
    {
        cout << "(no matches)\n";
        if (useSortedView)
            delete[] view;
        return;
    }

    for (int i = 0; i < r; i++)
    {
        int best = i;
        for (int j = i + 1; j < r; j++)
        {
            bool better = false;
            if (rows[j].skillHits > rows[best].skillHits)
                better = true;
            else if (rows[j].skillHits == rows[best].skillHits)
            {
                if (rows[j].roleHits > rows[best].roleHits)
                    better = true;
                else if (rows[j].roleHits == rows[best].roleHits)
                    if (view[rows[j].idx].id < view[rows[best].idx].id)
                        better = true;
            }
            if (better)
            {
                ScoreRow tmp = rows[j];
                rows[j] = rows[best];
                rows[best] = tmp;
            }
        }
    }
    int limit = (r < 10 ? r : 10);
    for (int i = 0; i < limit; ++i)
        printResumeLine(view[rows[i].idx]);

    if (useSortedView)
        delete[] view;
}

void promptAndSearchResumes(const ResumeArray &resumes, bool sortedView)
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter search text (skills/keywords): ";
    string query;
    getline(cin, query);

    auto start = chrono::high_resolution_clock::now();
    searchResumesTwoPointer(resumes, query, sortedView);
    auto end = chrono::high_resolution_clock::now();

    cout << "Search completed in " << chrono::duration<double>(end - start).count() << " seconds.\n";
}

// ----------- Display Jobs ------------
void displayJobs(JobArray &jobs, int count = 10)
{
    int total = jobs.getSize();
    if (total == 0)
    {
        cout << "No job descriptions loaded.\n";
        return;
    }
    cout << "Showing up to " << count << " of " << total << " job descriptions:\n";
    for (int i = 0; i < count && i < total; ++i)
    {
        Job j = jobs.getArray()[i];
        cout << i + 1 << ". ID: " << j.id << "\n   Role: " << j.role << "\n   Skills: [";
        for (int k = 0; k < j.skillCount; ++k)
        {
            cout << j.skills[k];
            if (k < j.skillCount - 1)
                cout << ", ";
        }
        cout << "]\n   Raw: " << j.raw_text << "\n\n";
    }
}

// ----------- JOB MATCHING ------------
int overlapSkillCount(const Job &J, const Resume &R)
{
    return countSkillMatchesTwoPointer(J.skills, J.skillCount, R.skills, R.skillCount);
}

void runJobMatching(const JobArray &jobs, const ResumeArray &resumes)
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Job position entered: ";
    string jobPosition;
    getline(cin, jobPosition);

    cout << "Skills entered: ";
    string skills;
    getline(cin, skills);

    double thresholdPct = 0.0;
    cout << "Enter threshold % (0 - 100): ";
    cin >> thresholdPct;

    // --- NEW: prepare user-entered skills for matching ---
    string cleanUserSkills = normalizeText(skills);
    string qUserSkills[50];
    int qUserCount = 0;
    extractSkills(cleanUserSkills, qUserSkills, qUserCount);
    // -----------------------------------------------------

    cout << "\n===== Job Matching ("
         << (g_searchMode == MODE_LINEAR ? "Linear" : "Two-pointer")
         << ") =====\n";
    cout << "Job position entered: " << jobPosition << "\n";
    cout << "Skills entered: " << skills << "\n";
    cout << "Threshold: " << thresholdPct << "%\n\n";

    struct ScoredRes
    {
        int rIdx;
        int overlap;
        double pct;
    };

    for (int j = 0; j < jobs.getSize(); ++j)
    {
        const Job &J = jobs.getArray()[j];

        // only jobs whose title matches the entered job position
        if (toLowerCopy(J.role).find(toLowerCopy(jobPosition)) == string::npos)
            continue;
        // NEW: Require the job to contain ALL user-entered skills
        if (qUserCount > 0)
        {
            if (!containsAllSkillsTwoPointer(J.skills, J.skillCount, qUserSkills, qUserCount))
                continue;
        }

        const int denom = (J.skillCount > 0 ? J.skillCount : 1);

        // collect matches for this job
        int maxR = resumes.getSize();
        ScoredRes *matches = new ScoredRes[maxR];
        int matchCount = 0;

        for (int r = 0; r < resumes.getSize(); ++r)
        {
            const Resume &R = resumes.getArray()[r];
            int overlap = 0;

            if (g_searchMode == MODE_LINEAR)
            {
                for (int a = 0; a < J.skillCount; ++a)
                {
                    for (int b = 0; b < R.skillCount; ++b)
                    {
                        if (J.skills[a] == R.skills[b])
                        {
                            ++overlap;
                            break;
                        }
                    }
                }
            }
            else
            {
                overlap = countSkillMatchesTwoPointer(J.skills, J.skillCount, R.skills, R.skillCount);
            }

            if (overlap <= 0)
                continue;

            // --- NEW: ensure resume also matches at least one user-entered skill ---
            // NEW: Require the resume to contain ALL user-entered skills
            if (qUserCount > 0)
            {
                if (!containsAllSkillsTwoPointer(R.skills, R.skillCount, qUserSkills, qUserCount))
                    continue;
            }

            // ---------------------------------------------------------------

            double pct = (100.0 * overlap) / denom;
            if (pct < thresholdPct)
                continue;

            if (matchCount < maxR)
            {
                matches[matchCount].rIdx = r;
                matches[matchCount].overlap = overlap;
                matches[matchCount].pct = pct;
                ++matchCount;
            }
        }

        if (matchCount == 0)
        {
            delete[] matches;
            continue;
        }

        // sort matches
        for (int a = 0; a < matchCount - 1; ++a)
        {
            int best = a;
            for (int b = a + 1; b < matchCount; ++b)
            {
                bool better = false;
                if (matches[b].pct > matches[best].pct)
                    better = true;
                else if (matches[b].pct == matches[best].pct)
                {
                    if (matches[b].overlap > matches[best].overlap)
                        better = true;
                    else if (matches[b].overlap == matches[best].overlap)
                    {
                        const Resume &RB = resumes.getArray()[matches[b].rIdx];
                        const Resume &RA = resumes.getArray()[matches[best].rIdx];
                        if (RB.id < RA.id)
                            better = true;
                    }
                }

                if (better)
                {
                    ScoredRes tmp = matches[b];
                    matches[b] = matches[best];
                    matches[best] = tmp;
                }
            }
        }

        // print job and top 10 matching resumes
        cout << "Job ID " << J.id << "\n";
        cout << "Job role " << J.role << "\n";
        cout << "Skills: [";
        for (int k = 0; k < J.skillCount; ++k)
            cout << J.skills[k] << (k + 1 < J.skillCount ? ", " : "");
        cout << "]\n";

        int show = (matchCount < 10 ? matchCount : 10);
        for (int i = 0; i < show; ++i)
        {
            const ScoredRes &m = matches[i];
            const Resume &R = resumes.getArray()[m.rIdx];
            cout << "Resume ID " << R.id << "  Skill matched "
                 << m.overlap << "/" << denom << " (" << (int)(m.pct + 0.5) << "%)\n";
            cout << "Skills: [";
            for (int k = 0; k < R.skillCount; ++k)
                cout << R.skills[k] << (k + 1 < R.skillCount ? ", " : "");
            cout << "]\n";
            if (qUserCount > 0)
            {
                cout << "  | Matched user skills: [";
                bool first = true;
                for (int us = 0; us < qUserCount; ++us)
                {
                    for (int rk = 0; rk < R.skillCount; ++rk)
                    {
                        if (qUserSkills[us] == R.skills[rk])
                        {
                            if (!first)
                                cout << ", ";
                            cout << qUserSkills[us];
                            first = false;
                            break;
                        }
                    }
                }
                cout << "]\n";
            }
        }
        cout << "\n";

        delete[] matches;
    }
}

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

// ----------- MAIN MENU ------------
void mainMenu(JobArray &jobs, ResumeArray &resumes)
{
    int choice;
    do
    {
        cout << "\n===== MAIN MENU =====\n"
             << "1. View Jobs\n"
             << "2. Sort Jobs\n"
             << "3. Search Jobs (Sorted)\n"
             << "4. Search Jobs (Not Sorted)\n"
             << "5. Search Resumes (Sorted)\n"
             << "6. Search Resumes (Not Sorted)\n"
             << "7. Job Matching\n"
             << "8. Return\n"
             << "Choice (1-8): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            displayJobs(jobs);
            break;

        case 2:
        {
            int algo = promptSortAlgorithm();
            size_t memBefore = getMemoryUsageKB();
            auto start = chrono::high_resolution_clock::now();
            if (algo == 1)
            {
                mergeSortJobs(jobs.getArray(), 0, jobs.getSize() - 1);
                cout << "Jobs sorted using Merge Sort.\n";
            }
            else
            {
                // Simple insertion sort for demonstration
                Job *arr = jobs.getArray();
                int n = jobs.getSize();
                for (int i = 1; i < n; i++)
                {
                    Job key = arr[i];
                    int j = i - 1;
                    while (j >= 0 && toLowerCopy(arr[j].role) > toLowerCopy(key.role))
                    {
                        arr[j + 1] = arr[j];
                        j--;
                    }
                    arr[j + 1] = key;
                }
                cout << "Jobs sorted using Insertion Sort.\n";
            }
            auto end = chrono::high_resolution_clock::now(); // End timer
            size_t memAfter = getMemoryUsageKB();
            chrono::duration<double> duration = end - start;
            cout << "\nExecution time: " << duration.count() << " seconds.\n";
            cout << "Memory usage: " << (memAfter - memBefore) << " KB.\n";
            displayJobs(jobs);
            break;
        }

        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        {
            int algo = promptSearchAlgorithm();
            g_searchMode = (algo == 1 ? MODE_LINEAR : MODE_TWO_POINTER);
            size_t memBefore = getMemoryUsageKB();
            auto start = chrono::high_resolution_clock::now();

            if (choice == 3)
                promptAndSearchJobs(jobs, true);
            else if (choice == 4)
                promptAndSearchJobs(jobs, false);
            else if (choice == 5)
                promptAndSearchResumes(resumes, true);
            else if (choice == 6)
                promptAndSearchResumes(resumes, false);
            else if (choice == 7)
                runJobMatching(jobs, resumes);
            auto end = chrono::high_resolution_clock::now();
            size_t memAfter = getMemoryUsageKB();
            chrono::duration<double> duration = end - start;
            cout << "\nExecution time: " << duration.count() << " seconds.\n";
            cout << "Memory usage: " << (memAfter - memBefore) << " KB.\n";
            break;
        }

        case 8:
            cout << "Returning to data structures selection...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 8);
}

void arrayMainMenuEntry()
{
    JobArray jobs(12000);
    ResumeArray resumes(12000);

    cout << "Loading jobs...\n";
    loadJobDescriptions(jobs, "job_description.csv");
    cout << "Loaded " << jobs.getSize() << " job descriptions.\n";

    cout << "Loading resumes...\n";
    loadResumes(resumes, "resume.csv");
    cout << "Loaded " << resumes.getSize() << " resumes.\n";

    mainMenu(jobs, resumes);
}

// ----------- MAIN ------------
int main()
{
    int dsChoice;

    do
    {
        cout << "\n===== SELECT DATA STRUCTURE =====\n"
             << "1. Array\n"
             << "2. Linked List\n"
             << "0. Exit\n"
             << "Choice (0-2): ";
        cin >> dsChoice;

        switch (dsChoice)
        {
        case 1:
            arrayMainMenuEntry();
            break;
        case 2:
            linkedListMainMenu();
            break;
        case 0:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (dsChoice != 0);

    return 0;
}

// To COMPILE PROJECT: do { g++ Array.cpp linked_list/*.cpp -o Array -lpsapi }
