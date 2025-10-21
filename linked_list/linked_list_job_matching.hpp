#ifndef JOB_MATCHING_HPP
#define JOB_MATCHING_HPP

#include "score_struct.hpp"
#include "job_resume_linked_list.hpp"
#include "user_prompt.hpp"

void runJobMatching(SearchMode searchMode, MatchQueryData &matchQuery, const JobLinkedList &jobs, const ResumeLinkedList &resumes);

void sortScoreRes(ScoredRes rows[], int count);

#endif
