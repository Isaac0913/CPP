#include "linked_list_search.hpp"
#include "job_resume_linked_list.hpp"
#include "score_struct.hpp"
#include "display_output.hpp"
#include "string_utils.hpp"
#include "linked_list_insertion_sort.hpp"
#include "count_matches.hpp"
#include <iostream>

// GO LOOK INTO user_prompt.hpp for the search mode and type enums

// remember to ask for search type (job or resume) and search mode (linear or two-pointer)
// then prompt the query by doing SearchQueryData userQuery = promptSearchQuery();
// then do:
// if (searchType == TYPE_JOBS) {searchJobs(searchMode, true or false, userQuery, jobs)}
// else if (searchType == TYPE_RESUME) {searchResume(searchMode, true or false, userQuery, jobs)}
// use PerformanceResult wrapper to measure performance if needed
void searchJobs(SearchMode searchMode, bool sortedView, SearchQueryData &userQuery, JobLinkedList &jobs)
{
    JobLinkedList *jobsView;
    if (!sortedView)
    {
        jobsView = const_cast<JobLinkedList *>(&jobs);
        cout << "------unsorted---\nunsorted location (location is the ID)\n";
    }
    else
    {
        // jobsView = new JobLinkedList(jobs);
        // mergeSortJobs(*jobsView, 0, n - 1);4
        jobsView = new JobLinkedList(sortInsertionJob(jobs));
        cout << "------sorted----\nsorted location (location is the ID)\n";
    }

    int maxSize = jobs.getSize();
    ScoreRow *scoreRows = new ScoreRow[maxSize];
    int r = 0;

    JobNodeSingly *current = jobsView->getHead();
    int index = 0;

    while (current != nullptr)
    {
        const Job &job = current->data;
        int sHits = 0;
        if (searchMode == MODE_LINEAR)
        {
            sHits = countSkillMatchesLinear(job.skills, job.skillCount, userQuery.skills, userQuery.skillCount);
        }
        else if (searchMode == MODE_TWO_POINTER)
        {
            sHits = countSkillMatchesTwoPointer(job.skills, job.skillCount, userQuery.skills, userQuery.skillCount);
        }
        int rHits = roleHitCount(toLowerCopy(job.role), userQuery.roles, userQuery.roleCount);

        if (sHits != 0 || rHits != 0)
        {
            scoreRows[r].jobNode = current;
            scoreRows[r].idx = index;
            scoreRows[r].skillHits = sHits;
            scoreRows[r].roleHits = rHits;
            r++;
        }

        current = current->next;
        index++;
    }

    if (r == 0)
    {
        cout << "(no matches)\n";
        if (sortedView)
        {
            delete jobsView;
        }
        delete[] scoreRows;
        return;
    }

    // use enum SearchType?
    sortScoreRows(scoreRows, r, TYPE_JOBS);

    int limit = (r < 10 ? r : 10);
    for (int i = 0; i < limit; ++i)
    {
        printJobLine(scoreRows[i].jobNode->data);
    }

    if (sortedView)
    {
        delete jobsView;
    }

    delete[] scoreRows;
}

void searchResumes(SearchMode &searchMode, bool sortedView, SearchQueryData &userQuery, ResumeLinkedList &resume)
{
    ResumeLinkedList *resumesView;
    if (!sortedView)
    {
        resumesView = const_cast<ResumeLinkedList *>(&resume);
        cout << "------unsorted---\nunsorted location (location is the ID)\n";
    }
    else
    {
        resumesView = new ResumeLinkedList(resume);
        // mergeSortResumes(*resumesView, 0, n - 1);
        cout << "------sorted----\nsorted location (location is the ID)\n";
    }

    int maxSize = resume.getSize();
    ScoreRow scoreRows[maxSize];
    int r = 0;

    ResumeNodeSingly *current = resumesView->getHead();
    int index = 0;

    while (current != nullptr)
    {
        const Resume &resume = current->data;
        int sHits = 0;
        if (searchMode == MODE_LINEAR)
        {
            sHits = countSkillMatchesLinear(resume.skills, resume.skillCount, userQuery.skills, userQuery.skillCount);
        }
        else if (searchMode == MODE_TWO_POINTER)
        {
            sHits = countSkillMatchesTwoPointer(resume.skills, resume.skillCount, userQuery.skills, userQuery.skillCount);
        }
        int rHits = roleHitCount(toLowerCopy(resume.raw_text), userQuery.roles, userQuery.roleCount);

        if (sHits != 0 || rHits != 0)
        {
            scoreRows[r].resumeNode = current;
            scoreRows[r].idx = index;
            scoreRows[r].skillHits = sHits;
            scoreRows[r].roleHits = rHits;
            r++;
        }

        current = current->next;
        index++;
    }

    if (r == 0)
    {
        cout << "(no matches)\n";
        if (sortedView)
        {
            delete resumesView;
        }
        return;
    }

    sortScoreRows(scoreRows, r, TYPE_RESUME);

    int limit = (r < 10 ? r : 10);
    for (int i = 0; i < limit; ++i)
    {
        printResumeLine(scoreRows[i].resumeNode->data);
    }

    if (sortedView)
    {
        delete resumesView;
    }
}

// placed it here because it's inherently a part of search
// this is selection sort
void sortScoreRows(ScoreRow rows[], int count, SearchType type)
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
                    if (type == TYPE_JOBS)
                    {
                        if (rows[j].jobNode->data.id < rows[best].jobNode->data.id)
                            better = true;
                    }
                    else
                    {
                        if (rows[j].resumeNode->data.id < rows[best].resumeNode->data.id)
                            better = true;
                    }
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
