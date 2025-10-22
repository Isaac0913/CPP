#include "linked_list_insertion_sort.hpp"
#include "string_utils.hpp"

// sorting jobs without target (lexicographically)
JobLinkedList sortInsertionJob(const JobLinkedList &jobLinkedList)
{
    JobLinkedList sortedList;
    JobNodeSingly *current = jobLinkedList.getHead();

    while (current != nullptr)
    {
        const lJob &job = current->data;
        JobNodeSingly *newNode = new JobNodeSingly(job);
        if (sortedList.getHead() == nullptr ||
            ltoLowerCopy(job.role) < ltoLowerCopy(sortedList.getHead()->data.role))
        {
            newNode->next = sortedList.getHead();
            sortedList.setHead(newNode);
            if (sortedList.getTail() == nullptr)
                sortedList.setTail(newNode);
        }
        else
        {
            JobNodeSingly *search = sortedList.getHead();
            while (search->next != nullptr &&
                   ltoLowerCopy(search->next->data.role) <= ltoLowerCopy(job.role))
            {
                search = search->next;
            }
            newNode->next = search->next;
            search->next = newNode;
            if (newNode->next == nullptr)
                sortedList.setTail(newNode);
        }
        sortedList.incrementSize();

        current = current->next;
    }

    return sortedList;
};

//  Insertion sort for RESUMES 
// ============================================================




// helper to generate the same key as merge sort uses
static string getResumeSortKey(const lResume& r)
{
    string key;
    for (int i = 0; i < r.skillCount; ++i)
    {
        key += ltoLowerCopy(r.skills[i]);
        key += '|';
    }
    return key;
}

// insertion sort for resumes
ResumeLinkedList sortInsertionResume(const ResumeLinkedList& resumeLinkedList)
{
    ResumeLinkedList sortedList;
    ResumeNodeSingly* current = resumeLinkedList.getHead();

    while (current != nullptr)
    {
        const lResume& resume = current->data;
        ResumeNodeSingly* newNode = new ResumeNodeSingly(resume);

        //  Define sort key: combine all skills into one lowercase string
        string key;
        for (int i = 0; i < resume.skillCount; ++i)
        {
            key += ltoLowerCopy(resume.skills[i]);
            key += '|';
        }

        // insert at correct sorted position
        if (sortedList.getHead() == nullptr)
        {
            // list empty
            sortedList.setHead(newNode);
            sortedList.setTail(newNode);
        }
        else if (key < getResumeSortKey(sortedList.getHead()->data))
        {
            // insert at head
            newNode->next = sortedList.getHead();
            sortedList.setHead(newNode);
        }
        else
        {
            ResumeNodeSingly* search = sortedList.getHead();
            while (search->next != nullptr &&
                getResumeSortKey(search->next->data) <= key)
            {
                search = search->next;
            }

            newNode->next = search->next;
            search->next = newNode;

            if (newNode->next == nullptr)
                sortedList.setTail(newNode);
        }

        sortedList.incrementSize();
        current = current->next;
    }

    return sortedList;
}


