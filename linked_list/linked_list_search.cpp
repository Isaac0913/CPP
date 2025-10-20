#include "linked_list_search.hpp"
#include "measure_performance.hpp"
#include "job_resume_linked_list.hpp"
#include "score_struct.hpp"
#include "display_output.hpp"
#include <iostream>

// GO LOOK INTO user_prompt.hpp for the search mode and type enums

// remember to ask for search type (job or resume) and search mode (linear or two-pointer)
// then prompt the query by doing SearchQueryData userQuery = promptSearchQuery();
// then do:
// if (searchType == TYPE_JOBS) {searchJobs(searchMode, true or false, userQuery, jobs)}
// else if (searchType == TYPE_RESUME) {searchResume(searchMode, true or false, userQuery, jobs)}
// use PerformanceResult wrapper to measure performance if needed
void searchJobs(SearchMode searchMode, bool sortedView, SearchQueryData userQuery, JobLinkedList &jobs)
{
    JobLinkedList *jobsView;
    if (!sortedView)
    {
        jobsView = const_cast<JobLinkedList *>(&jobs);
        cout << "------unsorted---\nunsorted location (location is the ID)\n";
    }
    else
    {
        jobsView = new JobLinkedList;
        *jobsView = jobs;
        // mergeSortJobs(view, 0, n - 1);
        cout << "------sorted----\nsorted location (location is the ID)\n";
    }

    int maxSize = jobs.getSize();
    ScoreRow scoreRows[maxSize];
    int r = 0;

    if (searchMode == MODE_LINEAR)
    {
        // perform linear search jobs linked list
    }
    else if (searchMode == MODE_TWO_POINTER)
    {
        // perform twopointer search jobs two-pointer
    }

    // the main point is to populate the ScoreRow array

    // Array implementation, dont use:
    // for (int i = 0; i < maxSize; ++i)
    // {
    //     int sHits = 0;
    //     if (searchMode == MODE_LINEAR)
    //     {
    //         sHits = countSkillMatchesLinear(jobsView[i].skills, jobsView[i].skillCount, userQuery.skills, userQuery.skillCount);
    //     }
    //     else if (searchMode == MODE_TWO_POINTER)
    //     {
    //         sHits = countSkillMatchesTwoPointer(jobsView[i].skills, jobsView[i].skillCount, userQuery.skills, userQuery.skillCount);
    //     }
    //     int rHits = roleHitCount(toLowerCopy(jobsView[i].role), userQuery.roles, userQuery.roleCount);
    //     if (sHits == 0 && rHits == 0)
    //     {
    //         continue;
    //     }
    //     scoreRows[r++] = {i, sHits, rHits};
    // }

    if (r == 0)
    {
        cout << "(no matches)\n";
        if (sortedView)
        {
            delete jobsView;
        }
        return;
    }

    int limit = (r < 10 ? r : 10);
    for (int i = 0; i < limit; ++i)
    {
        printJobLine(scoreRows[i].jobNode->data);
    }

    if (sortedView)
    {
        delete[] jobsView;
    }
}

// placed it here because it's inherently a part of search
// this is selection sort
void sortScoreRows(ScoreRow rows[], int count)
{
    for (int i = 0; i < count - 1; ++i)
    {
        int best = i;
        for (int j = i + 1; j < count; ++j)
        {
            bool better = false;
            if (rows[j].skillHits > rows[best].skillHits)
                better = true;
            else if (rows[j].skillHits == rows[best].skillHits)
            {
                if (rows[j].roleHits > rows[best].roleHits)
                    better = true;
                else if (rows[j].roleHits == rows[best].roleHits)
                {
                    if (rows[j].jobNode->data.id < rows[best].jobNode->data.id)
                        better = true;
                }
            }

            if (better)
                best = j;
        }

        if (best != i)
        {
            ScoreRow tmp = rows[best];
            rows[best] = rows[i];
            rows[i] = tmp;
        }
    }
}
