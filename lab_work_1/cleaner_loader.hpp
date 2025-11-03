#ifndef CLEANER_LOADER_HPP
#define CLEANER_LOADER_HPP

#include "job_resume_linked_list.hpp"
#include "job_resume_array.hpp"
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

// the filenames
extern const string JOB_DESC_FILENAME;
extern const string RESUME_FILENAME;

// this is the list of confirmed available skills (already in standardized format)
extern const char *SKILLS[];
extern const int SKILL_COUNT;

// returns a single cleaned raw data line by replacing with standardized format
string normalizeText(string input);

// extract skills from single cleaned raw data line
void extractSkills(const string &clean_line, string skillArray[], int &skillCount);

// formerly loadJobDescription/loadResumes (partially)
// now takes in a callback/lambda function to process each line depending on job or resume
void loadCSV(const string &filename, const function<void(const string &line, const string &clean, int id)> &processLine);

void loadJobDescIntoLinkedList(JobLinkedList &jobsLinkedList);

void loadResumeIntoLinkedList(ResumeLinkedList &resumesLinkedList);

void loadResumeIntoArray(ResumeArray &resumes);

void loadJobDescIntoArray(JobArray &jobs);

// remove this if needed
bool checkDuplicate(string toCheck, string a[], int size);

#endif
