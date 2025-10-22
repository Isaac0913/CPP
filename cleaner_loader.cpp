#include "cleaner_loader.hpp"
#include "string_utils.hpp"
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

const string JOB_DESC_FILENAME = "job_description.csv";
const string RESUME_FILENAME = "resume.csv";

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
    sortStringArr(skillArray, skillCount);
}

bool checkDuplicate(string toCheck, string a[], int size)
{
    bool duplicate;
    for (int i = 0; i < size; i++)
    {
        if (toCheck == a[i])
        {
            return true;
        }
    }
    return false;
}

void loadCSV(const string &filename, const function<void(const string &line, const string &clean, int id)> &processLine)
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
    string unique[10000];
    int size = 0;

    while (getline(file, line))
    {
        if (first)
        {
            first = false;
            continue;
        }
        if (line.empty())
        {
            continue;
        }

        string clean = normalizeText(line);

        if (filename == JOB_DESC_FILENAME)
        {
            string tempSkills[50];
            string tempRawText;
            int skillCount;

            extractSkills(clean, tempSkills, skillCount);

            string tempRole;
            istringstream srole(line);
            string w1, w2;
            srole >> w1 >> w2;
            tempRole = w1 + " " + w2;
            tempRole = cleanQuotes(tempRole);
            tempSkills[skillCount++] = toLowerCopy(tempRole);

            for (int i = 0; i < skillCount; i++)
            {
                if (i > 0)
                {
                    tempRawText += ",";
                }
                tempRawText += tempSkills[i];
            }

            if (!checkDuplicate(tempRawText, unique, size))
            {
                processLine(line, clean, id++);
                unique[size++] = tempRawText;
            }
        }
        else
        {
            processLine(line, clean, id++);
        }
    }

    file.close();
}

void loadJobDescIntoLinkedList(JobLinkedList &jobsLinkedList)
{
    loadCSV(JOB_DESC_FILENAME, [&](const string &line, const string &clean, int id)
            {
        Job j;
        j.id = id;
        j.raw_text = line;

        istringstream ss(line);
        string w1, w2;
        ss >> w1 >> w2;
        j.role = w1 + " " + w2;
        j.role = cleanQuotes(j.role);

        extractSkills(clean, j.skills, j.skillCount);
        
        if (j.skillCount > 0)
            jobsLinkedList.insertAtEnd(j); });

    cout << "Loaded " << jobsLinkedList.getSize() << " job descriptions into linked list.\n";
}

void loadResumeIntoLinkedList(ResumeLinkedList &resumesLinkedList)
{
    loadCSV(RESUME_FILENAME, [&](const string &line, const string &clean, int id)
            {
        Resume r;
        r.id = id;
        r.raw_text = line;

        extractSkills(clean, r.skills, r.skillCount);

        if (r.skillCount > 0)
            resumesLinkedList.insertAtEnd(r); });

    cout << "Loaded " << resumesLinkedList.getSize() << " resumes into linked list.\n";
}

void loadJobDescIntoArray(JobArray &jobs)
{
    loadCSV(JOB_DESC_FILENAME, [&](const string &line, const string &clean, int id)
            {
        Job j;
        j.id = id++;
        j.raw_text = line;
        istringstream ss(line);

        string w1, w2;
        ss >> w1 >> w2;
        j.role = w1 + " " + w2;
        j.role = cleanQuotes(j.role);

        extractSkills(clean, j.skills, j.skillCount);

        if (j.skillCount > 0)
            jobs.insert(j); });
}

void loadResumeIntoArray(ResumeArray &resumes)
{
    loadCSV(RESUME_FILENAME, [&](const string &line, const string &clean, int id)
            {
        Resume r;
        r.id = id;
        r.raw_text = line;

        extractSkills(clean, r.skills, r.skillCount);

        if (r.skillCount > 0)
             resumes.insert(r); });
}
