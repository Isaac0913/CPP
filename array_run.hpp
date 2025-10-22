#ifndef ARRAY_FUNCTIONS_HPP
#define ARRAY_FUNCTIONS_HPP

#include "job_resume_array.hpp"
#include "user_prompt.hpp"

extern SearchMode g_searchMode;

void cloneJobs(JobArray &dst, const JobArray &src);
void cloneResumes(ResumeArray &dst, const ResumeArray &src);
void displayJobs(JobArray &jobs, int count = 10);
void displayResumes(ResumeArray &resumes, int count = 10);
void mergeJobs(Job arr[], int left, int mid, int right);
void mergeSortJobs(Job arr[], int left, int right);
void mergeResumes(Resume arr[], int left, int mid, int right);
void mergeSortResumes(Resume arr[], int left, int right);
void insertionSortJobs(Job arr[], int n);
void insertionSortResumes(Resume arr[], int n);
void searchJobsTwoPointer(const JobArray &jobs, const string &userQuery, bool useSortedView);
void searchResumesTwoPointer(const ResumeArray &resumes, const string &userQuery, bool useSortedView);
void promptAndSearchJobs(const JobArray &jobs, bool sortedView);
void promptAndSearchResumes(const ResumeArray &resumes, bool sortedView);
bool containsAllSkillsTwoPointer(const string a[], int na, const string b[], int nb);
void runJobMatching(const JobArray &jobs, const ResumeArray &resumes);

#endif