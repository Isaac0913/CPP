#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "score_struct.hpp"
#include "job_resume_linked_list.hpp"
#include "user_prompt.hpp"

void searchJobs(SearchMode searchMode, SearchQueryData &userQuery, JobLinkedList &jobs);

void searchResumes(SearchMode searchMode, SearchQueryData &userQuery, ResumeLinkedList &resume);

void sortScoreRows(LScoreRow rows[], int count, SearchType type);

#endif
