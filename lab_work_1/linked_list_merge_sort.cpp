#include "linked_list_merge_sort.hpp"
#include <iostream>
using namespace std;
// ------------------------- Helpers -------------------------

static bool hasCycle(JobNodeSingly *head)
{
    if (!head)
        return false;
    JobNodeSingly *slow = head;
    JobNodeSingly *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return true;
    }
    return false;
}

static JobNodeSingly *jobGetMiddle(JobNodeSingly *head)
{
    if (!head)
        return nullptr;
    JobNodeSingly *slow = head;
    JobNodeSingly *fast = head->next;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

static JobNodeSingly *mergeJobsListsIterative(JobNodeSingly *left, JobNodeSingly *right)
{
    Job emptyJob;
    JobNodeSingly dummy(emptyJob); // stack-based dummy (no heap alloc)
    JobNodeSingly *tail = &dummy;

    while (left && right)
    {
        if (toLowerCopy(left->data.role) <= toLowerCopy(right->data.role))
        {
            tail->next = left;
            left = left->next;
        }
        else
        {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }
    tail->next = (left ? left : right);

    return dummy.next;
}

JobNodeSingly *mergeSortJobsLinked(JobNodeSingly *head)
{
    if (head == nullptr || head->next == nullptr)
        return head;

    JobNodeSingly *mid = jobGetMiddle(head);
    JobNodeSingly *rightHead = mid->next;
    mid->next = nullptr; // split

    JobNodeSingly *leftSorted = mergeSortJobsLinked(head);
    JobNodeSingly *rightSorted = mergeSortJobsLinked(rightHead);

    return mergeJobsListsIterative(leftSorted, rightSorted);
}

JobLinkedList mergeSortJobCopy(const JobLinkedList &jobLinkedList)
{
    if (hasCycle(jobLinkedList.getHead()))
    {
        cerr << "[ERROR] Linked list contains a cycle. Aborting merge sort.\n";
        return JobLinkedList();
    }

    JobLinkedList copy(jobLinkedList); // uses copy constructor

    JobNodeSingly *newHead = mergeSortJobsLinked(copy.getHead());
    copy.setHead(newHead);

    JobNodeSingly *tail = newHead;
    while (tail && tail->next)
        tail = tail->next;
    copy.setTail(tail);

    return copy;
}

static bool hasCycle(ResumeNodeSingly *head)
{
    if (!head)
        return false;
    ResumeNodeSingly *slow = head;
    ResumeNodeSingly *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return true;
    }
    return false;
}

static ResumeNodeSingly *resumeGetMiddle(ResumeNodeSingly *head)
{
    if (!head)
        return nullptr;
    ResumeNodeSingly *slow = head;
    ResumeNodeSingly *fast = head->next;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

static ResumeNodeSingly *mergeResumeListsIterative(ResumeNodeSingly *left, ResumeNodeSingly *right)
{
    Resume emptyResume;
    ResumeNodeSingly dummy(emptyResume);
    ResumeNodeSingly *tail = &dummy;

    while (left && right)
    {

        if (resumeSortKey(left->data) <= resumeSortKey(right->data))
        {
            tail->next = left;
            left = left->next;
        }
        else
        {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    tail->next = (left ? left : right);
    return dummy.next;
}

ResumeNodeSingly *mergeSortResumesLinked(ResumeNodeSingly *head)
{
    if (!head || !head->next)
        return head;

    ResumeNodeSingly *mid = resumeGetMiddle(head);
    ResumeNodeSingly *rightHead = mid->next;
    mid->next = nullptr;

    ResumeNodeSingly *leftSorted = mergeSortResumesLinked(head);
    ResumeNodeSingly *rightSorted = mergeSortResumesLinked(rightHead);

    return mergeResumeListsIterative(leftSorted, rightSorted);
}

ResumeLinkedList mergeSortResumeCopy(const ResumeLinkedList &resumeLinkedList)
{
    if (hasCycle(resumeLinkedList.getHead()))
    {
        cerr << "[ERROR] Resume linked list contains a cycle. Aborting merge sort.\n";
        return ResumeLinkedList();
    }

    ResumeLinkedList copy(resumeLinkedList); // deep copy

    ResumeNodeSingly *newHead = mergeSortResumesLinked(copy.getHead());
    copy.setHead(newHead);

    ResumeNodeSingly *tail = newHead;
    while (tail && tail->next)
        tail = tail->next;
    copy.setTail(tail);

    return copy;
}
