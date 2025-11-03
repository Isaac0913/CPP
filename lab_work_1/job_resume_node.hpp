#ifndef JOB_RESUME_NODE_HPP
#define JOB_RESUME_NODE_HPP

#include "job_resume_struct.hpp"
#include <string>
using namespace std;

// later can refactor/rename to just JobNode if we only do either singly or doubly
struct JobNodeSingly
{
    Job data;
    JobNodeSingly *next;
    JobNodeSingly(const Job &jobData, JobNodeSingly *n = nullptr) : data(jobData), next(n) {}
};

// later can refactor/rename to just ResumeNode if we only do either singly or doubly
struct ResumeNodeSingly
{
    Resume data;
    ResumeNodeSingly *next;
    ResumeNodeSingly(const Resume &resumeData, ResumeNodeSingly *n = nullptr) : data(resumeData), next(n) {}
};

#endif
