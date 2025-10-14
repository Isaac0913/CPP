#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <limits>
using namespace std;

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

// ----------- Sorting ------------
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

void insertionSortJobs(Job arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        Job key = arr[i];
        string keyRole = toLowerCopy(key.role);
        int j = i - 1;
        while (j >= 0 && toLowerCopy(arr[j].role) > keyRole)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
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

bool isSkillToken(const string &w)
{
    for (int i = 0; i < SKILL_COUNT; ++i)
        if (w == SKILLS[i])
            return true;
    return false;
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

// ----------- SEARCH ------------
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
        view = const_cast<Job *>(jobs.getArray());
    else
    {
        view = new Job[n];
        for (int i = 0; i < n; ++i)
            view[i] = jobs.getArray()[i];
        mergeSortJobs(view, 0, n - 1);
    }

    if (!useSortedView)
        cout << "------unsorted---\nunsorted location (location is the ID)\n";
    else
        cout << "------sorted----\nsorted location (location is the ID)\n";

    if (qCount > 0 && qRoleCount == 0)
    {
        int matchIdx[20000], m = 0;
        for (int i = 0; i < n; i++)
        {
            if (countSkillMatchesTwoPointer(view[i].skills, view[i].skillCount, qSkills, qCount) > 0)
                matchIdx[m++] = i;
        }
        if (m == 0)
        {
            cout << "(no matches)\n";
            if (useSortedView)
                delete[] view;
            return;
        }
        shuffleIndices(matchIdx, m);
        int limit = (m < 10 ? m : 10);
        for (int k = 0; k < limit; ++k)
            printJobLine(view[matchIdx[k]]);
    }
    else if (qCount == 0 && qRoleCount > 0) {
    // STRICT: require ALL role tokens present in title
        int matchIdx[20000]; int m = 0;
        for (int i = 0; i < n; i++) {
            const string roleLower = toLowerCopy(view[i].role);
            int hits = roleHitCount(roleLower, qRoleTok, qRoleCount);
            if (hits == qRoleCount) { // <-- all tokens matched
                matchIdx[m++] = i;
            }
        }
        if (m == 0) {
            cout << "(no matches)\n";
            if (useSortedView) delete[] view;
            return;
        }
        // Show up to 10 (random or ranked — you asked random for role-only; leaving as random)
        shuffleIndices(matchIdx, m);
        int limit = (m < 10 ? m : 10);
        for (int k = 0; k < limit; ++k) printJobLine(view[matchIdx[k]]);
    }
    else if (qCount > 0 && qRoleCount > 0)
    {
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
            int sHits = countSkillMatchesTwoPointer(view[i].skills, view[i].skillCount, qSkills, qCount);
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
    }
    else
        cout << "(no matches)\n";

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

// ----------- Sort Menu ------------
void sortMenuJobs(JobArray &jobs)
{
    if (jobs.getSize() == 0)
    {
        cout << "No jobs to sort.\n";
        return;
    }
    int choice;
    cout << "\nSort Jobs:\n1. Merge Sort\n2. Insertion Sort\nChoice: ";
    cin >> choice;

    auto start = chrono::high_resolution_clock::now();
    if (choice == 1)
    {
        mergeSortJobs(jobs.getArray(), 0, jobs.getSize() - 1);
        cout << "Jobs sorted using Merge Sort.\n";
    }
    else if (choice == 2)
    {
        insertionSortJobs(jobs.getArray(), jobs.getSize());
        cout << "Jobs sorted using Insertion Sort.\n";
    }
    else
    {
        cout << "Invalid option.\n";
        return;
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Sorting completed in "
         << chrono::duration<double>(end - start).count() << " seconds.\n";
    displayJobs(jobs, 10);
}

// ----------- MAIN MENU ------------
void mainMenu(JobArray &jobs)
{
    int choice;
    do
    {
        cout << "\n===== MAIN MENU =====\n"
             << "1. View Jobs\n"
             << "2. Sort Jobs\n"
             << "3. Search Jobs (Sorted)\n"
             << "4. Search Jobs (Not Sorted)\n"
             << "5. Exit\n"
             << "Choice (1-5): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            displayJobs(jobs);
            break;
        case 2:
            sortMenuJobs(jobs);
            break;
        case 3:
            promptAndSearchJobs(jobs, true);
            break;
        case 4:
            promptAndSearchJobs(jobs, false);
            break;
        case 5:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

// ----------- MAIN ------------
int main()
{
    JobArray jobs(12000);
    cout << "Loading jobs...\n";
    loadJobDescriptions(jobs, "job_description.csv");
    cout << "Loaded " << jobs.getSize() << " job descriptions.\n";
    mainMenu(jobs);
    return 0;
}
