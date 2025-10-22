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
                   ltoLowerCopy(search->next->data.role) < ltoLowerCopy(job.role))
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
