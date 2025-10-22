#include "linked_list_merge_sort.hpp"
#include <iostream>
using namespace std;
// ------------------------- Helpers -------------------------

// Detect cycle using Floyd's tortoise & hare.
// Returns true if a cycle exists, false otherwise.
static bool hasCycle(JobNodeSingly* head)
{
    if (!head) return false;
    JobNodeSingly* slow = head;
    JobNodeSingly* fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// Find middle node (for splitting). If head is nullptr returns nullptr.
static JobNodeSingly* jobGetMiddle(JobNodeSingly* head)
{
    if (!head) return nullptr;
    JobNodeSingly* slow = head;
    JobNodeSingly* fast = head->next;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Iterative merge of two sorted lists (by role, case-insensitive).
// Why iterative? Recursive merge may use stack proportional to list length
// and cause stack overflow on large lists. Iterative merge avoids that.
static JobNodeSingly* mergeJobsListsIterative(JobNodeSingly* left, JobNodeSingly* right)
{
    // Create a dummy node using a temporary empty Job.
    // We only use its "next" pointer for merging — its data is irrelevant.
    lJob emptyJob;
    JobNodeSingly dummy(emptyJob); // stack-based dummy (no heap alloc)
    JobNodeSingly* tail = &dummy;

    while (left && right)
    {
        if (ltoLowerCopy(left->data.role) <= ltoLowerCopy(right->data.role))
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

    // Attach whatever remains
    tail->next = (left ? left : right);

    return dummy.next;
}

// Recursive merge sort on nodes (divide & conquer).
// Depth is O(log n) — safe. Merges are done iteratively to avoid deep recursion there.
JobNodeSingly* mergeSortJobsLinked(JobNodeSingly* head)
{
    if (head == nullptr || head->next == nullptr)
        return head;

    JobNodeSingly* mid = jobGetMiddle(head);
    JobNodeSingly* rightHead = mid->next;
    mid->next = nullptr; // split

    JobNodeSingly* leftSorted = mergeSortJobsLinked(head);
    JobNodeSingly* rightSorted = mergeSortJobsLinked(rightHead);

    return mergeJobsListsIterative(leftSorted, rightSorted);
}



// Returns a *new* sorted linked list without modifying the original
JobLinkedList mergeSortJobCopy(const JobLinkedList& jobLinkedList)
{
    if (hasCycle(jobLinkedList.getHead()))
    {
        cerr << "[ERROR] Linked list contains a cycle. Aborting merge sort.\n";
        return JobLinkedList();
    }

    // 1 Deep copy the input list
    JobLinkedList copy(jobLinkedList); // uses copy constructor

    // 2 Sort the copy in-place (relink nodes)
    JobNodeSingly* newHead = mergeSortJobsLinked(copy.getHead());
    copy.setHead(newHead);

    // 3 Update tail pointer after sorting
    JobNodeSingly* tail = newHead;
    while (tail && tail->next)
        tail = tail->next;
    copy.setTail(tail);

    // 4️Return the sorted copy
    return copy;
}



// ------------------------- merge sort resume -------------------------
static string resumeSortKey(const lResume& r)
{
    string key;
    for (int i = 0; i < r.skillCount; ++i)
    {
        key += r.skills[i];
        key += '|';
    }
    return ltoLowerCopy(key);
}


static bool hasCycle(ResumeNodeSingly* head)
{
    if (!head) return false;
    ResumeNodeSingly* slow = head;
    ResumeNodeSingly* fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

static ResumeNodeSingly* resumeGetMiddle(ResumeNodeSingly* head)
{
    if (!head) return nullptr;
    ResumeNodeSingly* slow = head;
    ResumeNodeSingly* fast = head->next;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Merge two sorted resume lists 
static ResumeNodeSingly* mergeResumeListsIterative(ResumeNodeSingly* left, ResumeNodeSingly* right)
{
    lResume emptyResume;
    ResumeNodeSingly dummy(emptyResume);
    ResumeNodeSingly* tail = &dummy;

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

// Recursive merge sort on resume nodes
ResumeNodeSingly* mergeSortResumesLinked(ResumeNodeSingly* head)
{
    if (!head || !head->next)
        return head;

    ResumeNodeSingly* mid = resumeGetMiddle(head);
    ResumeNodeSingly* rightHead = mid->next;
    mid->next = nullptr;

    ResumeNodeSingly* leftSorted = mergeSortResumesLinked(head);
    ResumeNodeSingly* rightSorted = mergeSortResumesLinked(rightHead);

    return mergeResumeListsIterative(leftSorted, rightSorted);
}

// Public function: returns sorted copy
ResumeLinkedList mergeSortResumeCopy(const ResumeLinkedList& resumeLinkedList)
{
    if (hasCycle(resumeLinkedList.getHead()))
    {
        cerr << "[ERROR] Resume linked list contains a cycle. Aborting merge sort.\n";
        return ResumeLinkedList();
    }

    ResumeLinkedList copy(resumeLinkedList); // deep copy

    ResumeNodeSingly* newHead = mergeSortResumesLinked(copy.getHead());
    copy.setHead(newHead);

    // Fix tail pointer
    ResumeNodeSingly* tail = newHead;
    while (tail && tail->next)
        tail = tail->next;
    copy.setTail(tail);

    return copy;
}



