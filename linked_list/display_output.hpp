#ifndef DISPLAY_OUTPUT_HPP
#define DISPLAY_OUTPUT_HPP

#include "job_resume_linked_list.hpp"
#include "score_struct.hpp"
#include "user_prompt.hpp"

// the exact same format used in displayJobs function
void ldisplayJobMultiLine(const lJob &j, int index);

// we didnt have this so i just add ba
void ldisplayResumeMultiLine(const lResume &j, int index);

void lprintResumeLine(const lResume &r);

void lprintJobLine(const lJob &j);

void ldisplayMatchedResume(const lResume &R, const lScoredRes &m, const MatchQueryData &matchQuery, int denom, int index);

void ldisplayNoMatches();

#endif