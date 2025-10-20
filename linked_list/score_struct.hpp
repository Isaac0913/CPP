#ifndef SCORE_STRUCT_HPP
#define SCORE_STRUCT_HPP
#include "job_resume_node.hpp"

// slightly different from array version
struct ScoreRow
{
    JobNodeSingly *jobNode;
    int idx;
    int skillHits;
    int roleHits;
};

struct ScoredRes
{
    int idx;
    int skillHits;
    int roleHits;
};

#endif
