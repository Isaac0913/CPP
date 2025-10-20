#include "job_resume_linked_list.hpp"
#include "display_output.hpp"
#include <iostream>

// Copy constructor (called when assigning a linked list to a new linked list variable)
// Job
JobLinkedList::JobLinkedList(const JobLinkedList &other)
{
    head = nullptr;
    tail = nullptr;
    size = 0;
    JobNodeSingly *current = other.head;
    while (current != nullptr)
    {
        insertAtEnd(current->data);
        current = current->next;
    }
}

// Copy assignment operator (called when assigning a linked list to another existing linked list variable)
JobLinkedList &JobLinkedList::operator=(const JobLinkedList &other)
{
    if (this == &other)
    {
        return *this;
    }

    JobNodeSingly *current = head;
    while (current != nullptr)
    {
        JobNodeSingly *temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    size = 0;

    JobNodeSingly *otherNode = other.head;
    while (otherNode != nullptr)
    {
        insertAtEnd(otherNode->data);
        otherNode = otherNode->next;
    }

    return *this;
}

// constructor
JobLinkedList::JobLinkedList()
{
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// destructor
JobLinkedList::~JobLinkedList()
{
    JobNodeSingly *current = head;
    while (current != nullptr)
    {
        JobNodeSingly *temp = current;
        current = current->next;
        delete temp;
    }
}

void JobLinkedList::insertAtFront(const Job &j)
{
    JobNodeSingly *newNode = new JobNodeSingly(j);
    newNode->next = head;
    head = newNode;
    if (tail == nullptr)
    {
        tail = newNode;
    }
    size++;
}

void JobLinkedList::insertAtEnd(const Job &j)
{
    JobNodeSingly *newNode = new JobNodeSingly(j);

    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

int JobLinkedList::getSize() const { return size; }

JobNodeSingly *JobLinkedList::getHead() const { return head; }

JobNodeSingly *JobLinkedList::getTail() const { return tail; }

void JobLinkedList::setHead(JobNodeSingly *jobNode) { head = jobNode; }

void JobLinkedList::setTail(JobNodeSingly *jobNode) { tail = jobNode; }

void JobLinkedList::incrementSize() { size++; }

void JobLinkedList::display(int count) const
{
    if (head == nullptr)
    {
        cout << "No job descriptions loaded.\n";
        return;
    }
    cout << "Showing up to " << count << " of " << size << " job descriptions:\n";
    JobNodeSingly *current = head;
    int index = 1;
    while (current != nullptr && index <= count)
    {
        const Job &j = current->data;
        displayJobMultiLine(j, index);
        current = current->next;
        index++;
    }
}

ResumeLinkedList::ResumeLinkedList()
{
    head = nullptr;
    tail = nullptr;
    size = 0;
}

ResumeLinkedList::~ResumeLinkedList()
{
    ResumeNodeSingly *current = head;
    while (current != nullptr)
    {
        ResumeNodeSingly *temp = current;
        current = current->next;
        delete temp;
    }
}

void ResumeLinkedList::insertAtFront(const Resume &r)
{
    ResumeNodeSingly *newNode = new ResumeNodeSingly(r);
    newNode->next = head;
    head = newNode;
    if (tail == nullptr)
    {
        tail = newNode;
    }
    size++;
}

void ResumeLinkedList::insertAtEnd(const Resume &r)
{
    ResumeNodeSingly *newNode = new ResumeNodeSingly{r};

    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

int ResumeLinkedList::getSize() const { return size; }

ResumeNodeSingly *ResumeLinkedList::getHead() const { return head; }

ResumeNodeSingly *ResumeLinkedList::getTail() const { return tail; }

void ResumeLinkedList::setHead(ResumeNodeSingly *resumeNode) { head = resumeNode; }

void ResumeLinkedList::setTail(ResumeNodeSingly *resumeNode) { tail = resumeNode; }

void ResumeLinkedList::incrementSize() { size++; }

void ResumeLinkedList::display(int count) const
{
    if (head == nullptr)
    {
        cout << "No resumes loaded.\n";
        return;
    }
    cout << "Showing up to " << count << " of " << size << " resumes:\n";
    ResumeNodeSingly *current = head;
    int index = 1;
    while (current != nullptr && index <= count)
    {
        const Resume &r = current->data;
        displayResumeMultiLine(r, index);
        current = current->next;
        index++;
    }
}

// Copy constructor (called when assigning a linked list to a new linked list variable)
ResumeLinkedList::ResumeLinkedList(const ResumeLinkedList &other)
{
    head = nullptr;
    tail = nullptr;
    size = 0;

    ResumeNodeSingly *current = other.head;
    while (current != nullptr)
    {
        insertAtEnd(current->data); // deep copy using existing method
        current = current->next;
    }
}

// Copy assignment operator (called when assigning a linked list to another existing linked list variable)
ResumeLinkedList &ResumeLinkedList::operator=(const ResumeLinkedList &other)
{
    if (this == &other)
    {
        return *this;
    }

    ResumeNodeSingly *current = head;
    while (current != nullptr)
    {
        ResumeNodeSingly *temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    size = 0;

    ResumeNodeSingly *otherNode = other.head;
    while (otherNode != nullptr)
    {
        insertAtEnd(otherNode->data);
        otherNode = otherNode->next;
    }

    return *this;
}
