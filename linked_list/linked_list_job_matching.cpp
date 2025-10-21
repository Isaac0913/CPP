#include "linked_list_job_matching.hpp"
#include "count_matches.hpp"
#include "user_prompt.hpp"
#include "string_utils.hpp"
#include "display_output.hpp"
#include <iostream>

void linkedlistRunJobMatching(SearchMode searchMode, MatchQueryData &matchQuery, const JobLinkedList &jobs, const ResumeLinkedList &resumes)
{
    int jobMaxSize = jobs.getSize();
    int resumeMaxSize = resumes.getSize();
    int totalMatchCount = 0;

    JobNodeSingly *jobCurrent = jobs.getHead();

    while (jobCurrent != nullptr)
    {
        const lJob &J = jobCurrent->data;

        if (ltoLowerCopy(J.role).find(ltoLowerCopy(matchQuery.jobRole)) == string::npos)
        {
            jobCurrent = jobCurrent->next;
            continue;
        }

        int jobQuerySkillCount;
        if (searchMode == MODE_LINEAR)
        {
            jobQuerySkillCount = lcountSkillMatchesLinear(J.skills, J.skillCount, matchQuery.skills, matchQuery.skillCount);
        }
        else
        {
            jobQuerySkillCount = lcountSkillMatchesTwoPointer(J.skills, J.skillCount, matchQuery.skills, matchQuery.skillCount);
        }

        if (jobQuerySkillCount != matchQuery.skillCount)
        {
            jobCurrent = jobCurrent->next;
            continue;
        }

        const int denom = (J.skillCount > 0 ? J.skillCount : 1);
        lScoredRes *matches = new lScoredRes[resumeMaxSize];
        int matchCount = 0;

        ResumeNodeSingly *resumeCurrent = resumes.getHead();
        int rIdx = 0;

        while (resumeCurrent != nullptr)
        {
            const lResume &R = resumeCurrent->data;
            int overlap = 0;
            int userOverlap = 0;

            if (searchMode == MODE_LINEAR)
            {
                overlap = lcountSkillMatchesLinear(J.skills, J.skillCount, R.skills, R.skillCount);
            }
            else
            {
                overlap = lcountSkillMatchesTwoPointer(J.skills, J.skillCount, R.skills, R.skillCount);
            }

            if (overlap <= 0)
            {
                resumeCurrent = resumeCurrent->next;
                ++rIdx;
                continue;
            }

            if (matchQuery.skillCount > 0)
            {
                if (searchMode == MODE_LINEAR)
                {
                    userOverlap = lcountSkillMatchesLinear(R.skills, R.skillCount, matchQuery.skills, matchQuery.skillCount);
                }
                else
                {
                    userOverlap = lcountSkillMatchesTwoPointer(R.skills, R.skillCount, matchQuery.skills, matchQuery.skillCount);
                }

                if (userOverlap == 0 && userOverlap != matchQuery.skillCount)
                {
                    resumeCurrent = resumeCurrent->next;
                    ++rIdx;
                    continue;
                }
            }
            double pct = (100.0 * overlap) / denom;
            if (pct < matchQuery.thresholdPct)
            {
                resumeCurrent = resumeCurrent->next;
                ++rIdx;
                continue;
            }

            if (matchCount < resumeMaxSize)
            {
                matches[matchCount].resumeNode = resumeCurrent;
                matches[matchCount].rIdx = rIdx;
                matches[matchCount].overlap = overlap;
                matches[matchCount].pct = pct;
                ++matchCount;
                ++totalMatchCount;
            }

            resumeCurrent = resumeCurrent->next;
            ++rIdx;
        }

        if (matchCount == 0)
        {
            delete[] matches;
            jobCurrent = jobCurrent->next;
            continue;
        }

        sortScoreRes(matches, matchCount);

        cout << "Job ID " << J.id << "\n";
        cout << "Job role " << J.role << "\n";
        cout << "Skills: [";
        for (int k = 0; k < J.skillCount; ++k)
            cout << J.skills[k] << (k + 1 < J.skillCount ? ", " : "");
        cout << "]\n";

        int show = (matchCount < 10 ? matchCount : 10);
        for (int i = 0; i < show; ++i)
        {
            const lScoredRes &m = matches[i];
            const lResume &R = m.resumeNode->data;
            ldisplayMatchedResume(R, m, matchQuery, denom, i);
        }
        cout << "\n";
        delete[] matches;
        jobCurrent = jobCurrent->next;
    }

    if (totalMatchCount == 0)
    {
        ldisplayNoMatches();
    }
}

void sortScoreRes(lScoredRes rows[], int count)
{
    for (int i = 0; i < count - 1; ++i)
    {
        int best = i;
        for (int j = i + 1; j < count; ++j)
        {
            bool better = false;
            if (rows[j].pct > rows[best].pct)
                better = true;
            else if (rows[j].pct == rows[best].pct)
            {
                if (rows[j].overlap > rows[best].overlap)
                    better = true;
                else if (rows[j].overlap == rows[best].overlap)
                {
                    if (rows[j].resumeNode->data.id < rows[best].resumeNode->data.id)
                        better = true;
                }
            }
            if (better)
                best = j;
        }

        if (best != i)
        {
            lScoredRes tmp = rows[best];
            rows[best] = rows[i];
            rows[i] = tmp;
        }
    }
}
