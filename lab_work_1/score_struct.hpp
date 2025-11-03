#ifndef SCORE_STRUCT_HPP
#define SCORE_STRUCT_HPP
#include "job_resume_node.hpp"

// slightly different from array version
struct LScoreRow
{
    JobNodeSingly *jobNode;
    ResumeNodeSingly *resumeNode;
    int idx;
    int skillHits;
    int roleHits;
};

// used in matching
struct LScoredRes
{
    ResumeNodeSingly *resumeNode;
    int rIdx;
    int overlap;
    double pct;
};

// Array
struct ScoreRow
{
    int idx;
    int skillHits;
    int roleHits;
};

struct ScoredRes
{
    int rIdx;
    int overlap;
    double pct;
};

#endif
