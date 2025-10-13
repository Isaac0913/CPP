#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;

// ----------- Skill dictionary (array) ------------
const char* SKILLS[] = {
    "sql", "mysql", "postgresql", "postgres", "mongodb", "oracle", "sqlite", "redis", "nosql", "database",
    "powerbi", "power bi", "excel", "tableau", "d3", "matplotlib", "seaborn", "plotly",
    "python", "java", "javascript", "c++", "c#", "php", "ruby", "go", "rust",
    "swift", "kotlin", "scala", "r", "matlab", "typescript",
    "react", "angular", "vue", "django", "flask", "spring boot", "express",
    "laravel", "rails", "bootstrap", "jquery", "tensorflow", "pytorch", "keras",
    "scikit", "pandas", "numpy",
    "git", "docker", "kubernetes", "aws", "azure", "gcp", "linux", "jenkins", "jira",
    "restapi", "restapis", "rest api", "rest apis", "nlp", "machinelearning",
    "machine learning", "deeplearning", "deep learning", "datacleaning", "data cleaning","mlops",
    "systemdesign", "system design", "agile", "scrum", "kanban",
    "stakeholdermanagement", "stakeholder management", "userstories", "user stories",
    "productroadmap", "product roadmap", "statistics","computervision", "computer vision", "cloud"
};
const int SKILL_COUNT = sizeof(SKILLS) / sizeof(SKILLS[0]);

// ----------- Normalize text ------------
string normalizeText(string input) {
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    replace_if(input.begin(), input.end(), [](unsigned char c){ return !isalnum(c) && !isspace(c); }, ' ');

    const string FROM[] = {
        "powerbi", "power-bi",
        "machinelearning", "deeplearning", "datacleaning",
        "systemdesign", "system-design",
        "stakeholdermanagement", "userstories", "productroadmap"
    };
    const string TO[] = {
        "power bi", "power bi",
        "machine learning", "deep learning", "data cleaning",
        "system design", "system design",
        "stakeholder management", "user stories", "product roadmap"
    };
    const int COUNT = sizeof(FROM)/sizeof(FROM[0]);

    for (int i = 0; i < COUNT; i++) {
        size_t pos;
        while ((pos = input.find(FROM[i])) != string::npos)
            input.replace(pos, FROM[i].length(), TO[i]);
    }

    string clean;
    bool space = false;
    for (char c : input) {
        if (isspace(c)) {
            if (!space) clean += ' ';
            space = true;
        } else {
            clean += c;
            space = false;
        }
    }
    return clean;
}

string toLowerCopy(const string &s) {
    string out = s;
    for (char &c : out) c = tolower(c);
    return out;
}

// ----------- Resume & Job Structures ------------
struct Resume {
    int id;
    string skills[50];
    int skillCount;
    string raw_text;
};

class ResumeArray {
private:
    Resume *arr;
    int capacity;
    int size;
public:
    ResumeArray(int cap = 20000) {
        capacity = cap;
        size = 0;
        arr = new Resume[capacity];
    }
    ~ResumeArray() { delete[] arr; }
    void insert(const Resume &r) { if (size < capacity) arr[size++] = r; }
    int getSize() const { return size; }
    Resume* getArray() { return arr; }
    Resume getResume(int idx) const { return arr[idx]; }
};

struct Job {
    int id;
    string role;
    string skills[50];
    int skillCount;
    string raw_text;
};

class JobArray {
private:
    Job *arr;
    int capacity;
    int size;
public:
    JobArray(int cap = 20000) {
        capacity = cap;
        size = 0;
        arr = new Job[capacity];
    }
    ~JobArray() { delete[] arr; }
    void insert(const Job &j) { if (size < capacity) arr[size++] = j; }
    int getSize() const { return size; }
    Job* getArray() { return arr; }
    Job getJob(int idx) const { return arr[idx]; }
};

// ----------- Helper Functions ------------
void sortSkills(string arr[], int n) {
    for (int i = 1; i < n; i++) {
        string key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) { arr[j + 1] = arr[j]; j--; }
        arr[j + 1] = key;
    }
}

void extractSkills(const string &clean_line, string skillArray[], int &skillCount) {
    skillCount = 0;
    for (int s = 0; s < SKILL_COUNT; s++) {
        string skill = SKILLS[s];
        size_t pos = clean_line.find(skill);
        while (pos != string::npos) {
            bool left_ok = (pos == 0) || !isalnum(clean_line[pos - 1]);
            bool right_ok = (pos + skill.length() == clean_line.length()) || !isalnum(clean_line[pos + skill.length()]);
            if (left_ok && right_ok) {
                bool already = false;
                for (int k = 0; k < skillCount; k++)
                    if (skillArray[k] == skill) { already = true; break; }
                if (!already && skillCount < 50)
                    skillArray[skillCount++] = skill;
                break;
            }
            pos = clean_line.find(skill, pos + 1);
        }
    }
    sortSkills(skillArray, skillCount);
}

// ----------- Load CSVs ------------
void loadCSVIntoArray(ResumeArray &resumes, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) { cerr << "Error opening " << filename << "\n"; return; }

    string line; int id = 1; bool firstRow = true;
    while (getline(file, line)) {
        if (firstRow) { firstRow = false; continue; }
        if (line.empty()) continue;

        Resume r; r.id = id++; r.raw_text = line;
        string clean_line = normalizeText(line);
        extractSkills(clean_line, r.skills, r.skillCount);
        if (r.skillCount > 0) resumes.insert(r);
    }
    file.close();
}

void loadJobDescriptions(JobArray &jobs, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) { cerr << "Error opening " << filename << "\n"; return; }

    string line; int id = 1; bool firstRow = true;
    while (getline(file, line)) {
        if (firstRow) { firstRow = false; continue; }
        if (line.empty()) continue;

        Job j; j.id = id++; j.raw_text = line;
        istringstream issRole(line);
        string w1, w2; issRole >> w1 >> w2;
        j.role = w1 + " " + w2;
        string clean_line = normalizeText(line);
        extractSkills(clean_line, j.skills, j.skillCount);
        if (j.skillCount > 0) jobs.insert(j);
    }
    file.close();
}

// ----------- Merge Sort for Resume ------------
void mergeResumes(Resume arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Resume *L = new Resume[n1];
    Resume *R = new Resume[n2];

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        string a = (L[i].skillCount > 0) ? toLowerCopy(L[i].skills[0]) : "";
        string b = (R[j].skillCount > 0) ? toLowerCopy(R[j].skills[0]) : "";
        if (a <= b) arr[k++] = L[i++]; else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSortResumes(Resume arr[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortResumes(arr, left, mid);
    mergeSortResumes(arr, mid + 1, right);
    mergeResumes(arr, left, mid, right);
}

// ----------- Merge Sort for Job ------------
void mergeJobs(Job arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Job *L = new Job[n1];
    Job *R = new Job[n2];

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        string a = toLowerCopy(L[i].role);
        string b = toLowerCopy(R[j].role);
        if (a <= b) arr[k++] = L[i++]; else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSortJobs(Job arr[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortJobs(arr, left, mid);
    mergeSortJobs(arr, mid + 1, right);
    mergeJobs(arr, left, mid, right);
}

// ----------- Quick Sort for Resume ------------
int partitionResumes(Resume arr[], int low, int high) {
    string pivot = (arr[high].skillCount > 0) ? toLowerCopy(arr[high].skills[0]) : "";
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        string cur = (arr[j].skillCount > 0) ? toLowerCopy(arr[j].skills[0]) : "";
        if (cur < pivot) {
            ++i; swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortResumes(Resume arr[], int low, int high) {
    if (low < high) {
        int pi = partitionResumes(arr, low, high);
        quickSortResumes(arr, low, pi - 1);
        quickSortResumes(arr, pi + 1, high);
    }
}

// ----------- Quick Sort for Job ------------
int partitionJobs(Job arr[], int low, int high) {
    string pivot = toLowerCopy(arr[high].role);
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (toLowerCopy(arr[j].role) < pivot) {
            ++i; swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortJobs(Job arr[], int low, int high) {
    if (low < high) {
        int pi = partitionJobs(arr, low, high);
        quickSortJobs(arr, low, pi - 1);
        quickSortJobs(arr, pi + 1, high);
    }
}

// ----------- Display helper (prints up to n items) ------------
void displayResumes(ResumeArray &resumes, int count = 10) {
    int total = resumes.getSize();
    if (total == 0) { cout << "No resumes loaded.\n"; return; }
    cout << "Showing up to " << count << " of " << total << " resumes:\n";
    for (int i = 0; i < count && i < total; ++i) {
        Resume r = resumes.getArray()[i];
        cout << i+1 << ". ID: " << r.id << "\n   Skills: [";
        for (int j = 0; j < r.skillCount; ++j) {
            cout << r.skills[j];
            if (j < r.skillCount - 1) cout << ", ";
        }
        cout << "]\n   Raw: " << r.raw_text << "\n\n";
    }
}

void displayJobs(JobArray &jobs, int count = 10) {
    int total = jobs.getSize();
    if (total == 0) { cout << "No job descriptions loaded.\n"; return; }
    cout << "Showing up to " << count << " of " << total << " job descriptions:\n";
    for (int i = 0; i < count && i < total; ++i) {
        Job j = jobs.getArray()[i];
        cout << i+1 << ". ID: " << j.id << "\n   Role: " << j.role << "\n   Skills: [";
        for (int k = 0; k < j.skillCount; ++k) {
            cout << j.skills[k];
            if (k < j.skillCount - 1) cout << ", ";
        }
        cout << "]\n   Raw: " << j.raw_text << "\n\n";
    }
}

// ----------- Sort Menus (correct functions) ------------
void sortMenuResumes(ResumeArray &resumes) {
    if (resumes.getSize() == 0) { cout << "No resumes to sort.\n"; return; }
    int choice;
    cout << "\nSort Resumes:\n1. Merge Sort\n2. Quick Sort\nChoice: ";
    cin >> choice;
    auto start = chrono::high_resolution_clock::now();
    if (choice == 1) {
        mergeSortResumes(resumes.getArray(), 0, resumes.getSize() - 1);
        cout << "Resumes sorted using Merge Sort.\n";
    } else if (choice == 2) {
        quickSortResumes(resumes.getArray(), 0, resumes.getSize() - 1);
        cout << "Resumes sorted using Quick Sort.\n";
    } else {
        cout << "Invalid option.\n"; return;
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Sorting completed in " << chrono::duration<double>(end - start).count() << " seconds.\n";
    displayResumes(resumes, 10); // print first 10 for verification
}

void sortMenuJobs(JobArray &jobs) {
    if (jobs.getSize() == 0) { cout << "No jobs to sort.\n"; return; }
    int choice;
    cout << "\nSort Jobs:\n1. Merge Sort\n2. Quick Sort\nChoice: ";
    cin >> choice;
    auto start = chrono::high_resolution_clock::now();
    if (choice == 1) {
        mergeSortJobs(jobs.getArray(), 0, jobs.getSize() - 1);
        cout << "Jobs sorted using Merge Sort.\n";
    } else if (choice == 2) {
        quickSortJobs(jobs.getArray(), 0, jobs.getSize() - 1);
        cout << "Jobs sorted using Quick Sort.\n";
    } else {
        cout << "Invalid option.\n"; return;
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Sorting completed in " << chrono::duration<double>(end - start).count() << " seconds.\n";
    displayJobs(jobs, 10); // print first 10 for verification
}

// ----------- Main Menu ------------
void mainMenu(ResumeArray &resumes, JobArray &jobs) {
    int choice;
    do {
        cout << "\n===== MAIN MENU =====\n"
             << "1. View Jobs\n"
             << "2. View Resumes\n"
             << "3. Sort Jobs\n"
             << "4. Sort Resumes\n"
             << "5. Search Jobs (Sorted)\n"
             << "6. Search Jobs (Not Sorted)\n"
             << "7. Search Resumes (Sorted)\n"
             << "8. Search Resumes (Not Sorted)\n"
             << "9. Match Jobs and Resumes\n"
             << "10. Exit\n"
             << "Choice (1-10): ";
        cin >> choice;
        switch (choice) {
            case 1: displayJobs(jobs); break;
            case 2: displayResumes(resumes); break;
            case 3: sortMenuJobs(jobs); break;
            case 4: sortMenuResumes(resumes); break;
            // Cases 5-9 would call respective search and match functions (not implemented here)
            case 10: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (choice != 10);
}


int main() {
    ResumeArray resumes(12000);
    JobArray jobs(12000);

    cout << "Loading data...\n";
    loadCSVIntoArray(resumes, "resume.csv");
    loadJobDescriptions(jobs, "job_description.csv");

    cout << "Loaded0000000000000000000000000000000000 " << resumes.getSize() << " resumes, " << jobs.getSize() << " job descriptions.\n";
    mainMenu(resumes, jobs);
    return 0;
}
