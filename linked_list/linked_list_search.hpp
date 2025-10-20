#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "score_struct.hpp"
#include "job_resume_linked_list.hpp"
#include "user_prompt.hpp"

void sortScoreRows(ScoreRow rows[], int count);

void searchJobs(SearchMode searchMode, bool sortedView, SearchQueryData userQuery, JobLinkedList &jobs);

#endif