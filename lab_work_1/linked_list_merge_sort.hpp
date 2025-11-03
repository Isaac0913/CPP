#ifndef LINKED_LIST_MERGE_SORT_HPP
#define LINKED_LIST_MERGE_SORT_HPP

#include "job_resume_linked_list.hpp"  // your JobLinkedList, JobNodeSingly
#include "string_utils.hpp"             // for toLowerCopy()

// Internal merge sort helper (sorts by job role)
JobNodeSingly* mergeSortJobsLinked(JobNodeSingly* head);

// Public function — returns a new sorted copy of the given list
JobLinkedList mergeSortJobCopy(const JobLinkedList& jobLinkedList);



// Internal merge sort helper for resumes (sorts by name or skill)
ResumeNodeSingly* mergeSortResumesLinked(ResumeNodeSingly* head);

// Public function — returns a new sorted copy of the given list
ResumeLinkedList mergeSortResumeCopy(const ResumeLinkedList& resumeLinkedList);


#endif