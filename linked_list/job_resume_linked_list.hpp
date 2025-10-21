#ifndef JOB_RESUME_LINKEDLIST_HPP
#define JOB_RESUME_LINKEDLIST_HPP

#include "job_resume_node.hpp"
#include <string>
using namespace std;

// here i only did singly, if really mau doubly then we change ba...
class JobLinkedList
{
private:
    JobNodeSingly *head;
    JobNodeSingly *tail;
    int size;

public:
    JobLinkedList();
    JobLinkedList(const JobLinkedList &other);
    ~JobLinkedList();
    JobLinkedList &operator=(const JobLinkedList &other);
    void insertAtFront(const lJob &j);
    void insertAtEnd(const lJob &j);
    int getSize() const; // cannot modify object state (const JobLinkedList* this)
    JobNodeSingly *getHead() const;
    JobNodeSingly *getTail() const;
    void setHead(JobNodeSingly *jobNode);
    void setTail(JobNodeSingly *jobNode);
    void incrementSize();
    void display(int count = 10) const; // a linked list version of the displayJobs function
};

class ResumeLinkedList
{
private:
    ResumeNodeSingly *head;
    ResumeNodeSingly *tail;
    int size;

public:
    ResumeLinkedList();
    ResumeLinkedList(const ResumeLinkedList &other);
    ~ResumeLinkedList();
    ResumeLinkedList &operator=(const ResumeLinkedList &other);
    void insertAtFront(const lResume &r);
    void insertAtEnd(const lResume &r);
    int getSize() const;
    ResumeNodeSingly *getHead() const;
    ResumeNodeSingly *getTail() const;
    void setHead(ResumeNodeSingly *resumeNode);
    void setTail(ResumeNodeSingly *resumeNode);
    void incrementSize();
    void display(int count = 10) const; // a linked list resume version of the displayJobs function
};

#endif
