#ifndef DISPLAY_OUTPUT_HPP
#define DISPLAY_OUTPUT_HPP

#include "job_resume_linked_list.hpp"
#include "score_struct.hpp"
#include "user_prompt.hpp"

// the exact same format used in displayJobs function
void displayJobMultiLine(const Job &j, int index);

// we didnt have this so i just add ba
void displayResumeMultiLine(const Resume &j, int index);

void printResumeLine(const Resume &r);

void printJobLine(const Job &j);

void displayMatchedResume(const Resume &R, const LScoredRes &m, const MatchQueryData &matchQuery, int denom, int index);

void displayNoMatches();

#endif