#include "linked_list_insertion_sort.hpp"
#include "string_utils.hpp"

// sorting jobs without target (lexicographically)
JobLinkedList sortInsertionJob(const JobLinkedList &jobLinkedList)
{
    JobLinkedList sortedList;
    JobNodeSingly *current = jobLinkedList.getHead();

    while (current != nullptr)
    {
        const Job &job = current->data;
        JobNodeSingly *newNode = new JobNodeSingly(job);
        if (sortedList.getHead() == nullptr ||
            toLowerCopy(job.role) < toLowerCopy(sortedList.getHead()->data.role))
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
                   toLowerCopy(search->next->data.role) <= toLowerCopy(job.role))
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

static string getResumeSortKey(const Resume &r)
{
    string key;
    for (int i = 0; i < r.skillCount; ++i)
    {
        key += toLowerCopy(r.skills[i]);
        key += '|';
    }
    return key;
}

// insertion sort for resumes
ResumeLinkedList sortInsertionResume(const ResumeLinkedList &resumeLinkedList)
{
    ResumeLinkedList sortedList;
    ResumeNodeSingly *current = resumeLinkedList.getHead();

    while (current != nullptr)
    {
        const Resume &resume = current->data;
        ResumeNodeSingly *newNode = new ResumeNodeSingly(resume);

        string key;
        for (int i = 0; i < resume.skillCount; ++i)
        {
            key += toLowerCopy(resume.skills[i]);
            key += '|';
        }

        if (sortedList.getHead() == nullptr)
        {
            sortedList.setHead(newNode);
            sortedList.setTail(newNode);
        }
        else if (key < getResumeSortKey(sortedList.getHead()->data))
        {
            newNode->next = sortedList.getHead();
            sortedList.setHead(newNode);
        }
        else
        {
            ResumeNodeSingly *search = sortedList.getHead();
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
