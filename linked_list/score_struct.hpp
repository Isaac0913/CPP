#ifndef SCORE_STRUCT_HPP
#define SCORE_STRUCT_HPP
#include "job_resume_node.hpp"

// slightly different from array version
struct lScoreRow
{
    JobNodeSingly *jobNode;
    ResumeNodeSingly *resumeNode;
    int idx;
    int skillHits;
    int roleHits;
};

// used in matching
struct lScoredRes
{
    ResumeNodeSingly *resumeNode;
    int rIdx;
    int overlap;
    double pct;
};

#endif
