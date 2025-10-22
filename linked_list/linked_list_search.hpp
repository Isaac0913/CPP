#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "score_struct.hpp"
#include "job_resume_linked_list.hpp"
#include "user_prompt.hpp"

void searchJobs(SearchMode searchMode, bool sortedView, SearchQueryData &userQuery, JobLinkedList &jobs);

void searchResumes(SearchMode searchMode, bool sortedView, SearchQueryData &userQuery, ResumeLinkedList &resume);

void sortScoreRows(lScoreRow rows[], int count, SearchType type);

#endif
