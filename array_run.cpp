#include "user_prompt.hpp"
#include "cleaner_loader.hpp"
#include "string_utils.hpp"
#include "measure_performance.hpp"
#include "count_matches.hpp"
#include "display_output.hpp"
#include "array_run.hpp"
#include <chrono>

#include <iostream>

SearchMode g_searchMode = MODE_TWO_POINTER;

void cloneJobs(JobArray &dst, const JobArray &src)
{
    for (int i = 0; i < src.getSize(); ++i)
        dst.insert(src.getArray()[i]);
}
void cloneResumes(ResumeArray &dst, const ResumeArray &src)
{
    for (int i = 0; i < src.getSize(); ++i)
        dst.insert(src.getArray()[i]);
}

void displayJobs(JobArray &jobs, int count)
{
    int total = jobs.getSize();
    if (total == 0)
    {
        cout << "No job descriptions loaded.\n";
        return;
    }
    cout << "Showing up to " << count << " of " << total << " job descriptions:\n";
    for (int i = 0; i < count && i < total; ++i)
    {
        Job j = jobs.getArray()[i];
        cout << i + 1 << ". ID: " << j.id << "\n   Role: " << j.role << "\n   Skills: [";
        for (int k = 0; k < j.skillCount; ++k)
        {
            cout << j.skills[k];
            if (k < j.skillCount - 1)
                cout << ", ";
        }
        cout << "]\n   Raw: " << j.raw_text << "\n\n";
    }
}

void displayResumes(ResumeArray &resumes, int count)
{
    int total = resumes.getSize();
    if (total == 0)
    {
        cout << "No resumes loaded.\n";
        return;
    }
    cout << "Showing up to " << count << " of " << total << " resumes:\n";
    for (int i = 0; i < count && i < total; ++i)
    {
        const Resume &r = resumes.getArray()[i];
        cout << i + 1 << ". ID: " << r.id << "\n   Skills: [";
        for (int k = 0; k < r.skillCount; ++k)
        {
            cout << r.skills[k];
            if (k < r.skillCount - 1)
                cout << ", ";
        }
        cout << "]\n   Raw: " << r.raw_text << "\n\n";
    }
}

void mergeJobs(Job arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1, n2 = right - mid;
    Job *L = new Job[n1];
    Job *R = new Job[n2];
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        string a = toLowerCopy(L[i].role);
        string b = toLowerCopy(R[j].role);
        if (a <= b)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSortJobs(Job arr[], int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    mergeSortJobs(arr, left, mid);
    mergeSortJobs(arr, mid + 1, right);
    mergeJobs(arr, left, mid, right);
}

void mergeResumes(Resume arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1, n2 = right - mid;
    Resume *L = new Resume[n1];
    Resume *R = new Resume[n2];
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (resumeSortKey(L[i]) <= resumeSortKey(R[j]))
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSortResumes(Resume arr[], int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    mergeSortResumes(arr, left, mid);
    mergeSortResumes(arr, mid + 1, right);
    mergeResumes(arr, left, mid, right);
}

// ===== NEW: Insertion sorts for Jobs and Resumes (used when user chooses it) =====
void insertionSortJobs(Job arr[], int n)
{
    for (int i = 1; i < n; ++i)
    {
        Job key = arr[i];
        int j = i - 1;
        while (j >= 0 && toLowerCopy(arr[j].role) > toLowerCopy(key.role))
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void insertionSortResumes(Resume arr[], int n)
{
    for (int i = 1; i < n; ++i)
    {
        Resume key = arr[i];
        int j = i - 1;
        while (j >= 0 && resumeSortKey(arr[j]) > resumeSortKey(key))
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void searchJobsTwoPointer(const JobArray &jobs, const SearchQueryData &queryData, bool useSortedView)
{
    int n = jobs.getSize();
    if (n == 0)
    {
        cout << "No job descriptions loaded.\n";
        return;
    }

    Job *view = nullptr;
    if (!useSortedView)
    {
        view = const_cast<Job *>(jobs.getArray());
        cout << "------searching current order---\n";
    }
    else
    {
        view = new Job[n];
        for (int i = 0; i < n; ++i)
            view[i] = jobs.getArray()[i];
        mergeSortJobs(view, 0, n - 1);
        cout << "------sorted (view copy)----\n";
    }

    ScoreRow rows[20000];
    int r = 0;

    for (int i = 0; i < n; ++i)
    {
        int sHits = (g_searchMode == MODE_LINEAR)
                        ? countSkillMatchesLinear(view[i].skills, view[i].skillCount,
                                                  queryData.skills, queryData.skillCount)
                        : countSkillMatchesTwoPointer(view[i].skills, view[i].skillCount,
                                                      queryData.skills, queryData.skillCount);
        int rHits = roleHitCount(toLowerCopy(view[i].role), queryData.roles, queryData.roleCount);
        if (sHits == 0 && rHits == 0)
            continue;
        rows[r++] = {i, sHits, rHits};
    }

    if (r == 0)
    {
        cout << "(no matches)\n";
        if (useSortedView)
            delete[] view;
        return;
    }

    for (int i = 0; i < r; i++)
    {
        int best = i;
        for (int j = i + 1; j < r; j++)
        {
            bool better = false;
            if (rows[j].skillHits > rows[best].skillHits)
                better = true;
            else if (rows[j].skillHits == rows[best].skillHits)
            {
                if (rows[j].roleHits > rows[best].roleHits)
                    better = true;
                else if (rows[j].roleHits == rows[best].roleHits)
                    if (view[rows[j].idx].id < view[rows[best].idx].id)
                        better = true;
            }
            if (better)
            {
                best = j;
            }
        }
        if (best != i)
        {
            ScoreRow tmp = rows[best];
            rows[best] = rows[i];
            rows[i] = tmp;
        }
    }
    int limit = (r < 10 ? r : 10);
    for (int i = 0; i < limit; ++i)
        printJobLine(view[rows[i].idx]);

    if (useSortedView)
        delete[] view;
}

void searchResumesTwoPointer(const ResumeArray &resumes, const SearchQueryData &queryData, bool useSortedView)
{
    int n = resumes.getSize();
    if (n == 0)
    {
        cout << "No resumes loaded.\n";
        return;
    }

    Resume *view = nullptr;
    if (!useSortedView)
    {
        view = const_cast<Resume *>(resumes.getArray());
        cout << "------searching current order---\n";
    }
    else
    {
        view = new Resume[n];
        for (int i = 0; i < n; ++i)
            view[i] = resumes.getArray()[i];
        mergeSortResumes(view, 0, n - 1);
        cout << "------sorted (view copy)----\n";
    }

    ScoreRow rows[20000];
    int r = 0;

    for (int i = 0; i < n; ++i)
    {
        int sHits = (g_searchMode == MODE_LINEAR)
                        ? countSkillMatchesLinear(view[i].skills, view[i].skillCount,
                                                  queryData.skills, queryData.skillCount)
                        : countSkillMatchesTwoPointer(view[i].skills, view[i].skillCount,
                                                      queryData.skills, queryData.skillCount);

        int rHits = roleHitCount(toLowerCopy(view[i].raw_text), queryData.roles, queryData.roleCount);
        if (sHits == 0 && rHits == 0)
            continue;
        rows[r++] = {i, sHits, rHits};
    }

    if (r == 0)
    {
        cout << "(no matches)\n";
        if (useSortedView)
            delete[] view;
        return;
    }

    for (int i = 0; i < r; i++)
    {
        int best = i;
        for (int j = i + 1; j < r; j++)
        {
            bool better = false;
            if (rows[j].skillHits > rows[best].skillHits)
                better = true;
            else if (rows[j].skillHits == rows[best].skillHits)
            {
                if (rows[j].roleHits > rows[best].roleHits)
                    better = true;
                else if (rows[j].roleHits == rows[best].roleHits)
                    if (view[rows[j].idx].id < view[rows[best].idx].id)
                        better = true;
            }
            if (better)
            {
                best = j;
            }
        }
        if (best != i)
        {
            ScoreRow tmp = rows[best];
            rows[best] = rows[i];
            rows[i] = tmp;
        }
    }
    int limit = (r < 10 ? r : 10);
    for (int i = 0; i < limit; ++i)
        printResumeLine(view[rows[i].idx]);

    if (useSortedView)
        delete[] view;
}

void promptAndSearchJobs(const JobArray &jobs, bool sortedView)
{
    SearchQueryData queryData = promptSearchQuery(true);
    size_t before = getProcessMemoryBytes();
    auto start = chrono::high_resolution_clock::now();
    searchJobsTwoPointer(jobs, queryData, sortedView);
    auto end = chrono::high_resolution_clock::now();
    size_t after = getProcessMemoryBytes();

    long long diff = static_cast<long long>(after) - static_cast<long long>(before);
    if (diff < 0)
        diff = 0;
    cout << "Before: " << before << " bytes, After: " << after << " bytes\n";
    cout << "Memory usage: " << diff << " bytes (" << (diff / 1024.0) << " KB)\n";
    cout << "Search completed in " << chrono::duration<double>(end - start).count() << " seconds.\n";
}

void promptAndSearchResumes(const ResumeArray &resumes, bool sortedView)
{
    SearchQueryData queryData = promptSearchQuery(true);
    size_t before = getProcessMemoryBytes();
    auto start = chrono::high_resolution_clock::now();
    searchResumesTwoPointer(resumes, queryData, sortedView);
    auto end = chrono::high_resolution_clock::now();
    size_t after = getProcessMemoryBytes();
    long long diff = static_cast<long long>(after) - static_cast<long long>(before);
    if (diff < 0)
        diff = 0;
    cout << "Before: " << before << " bytes, After: " << after << " bytes\n";
    cout << "Memory usage: " << diff << " bytes (" << (diff / 1024.0) << " KB)\n";
    cout << "Search completed in " << chrono::duration<double>(end - start).count() << " seconds.\n";
}

bool containsAllSkillsTwoPointer(const string a[], int na, const string b[], int nb)
{
    int i = 0, j = 0;
    while (i < na && j < nb)
    {
        if (a[i] == b[j])
        {
            ++i;
            ++j;
        }
        else if (a[i] < b[j])
        {
            ++i;
        }
        else
        {
            // a[i] > b[j] means the job/resume is missing b[j]
            return false;
        }
    }
    return (j == nb);
}

void runJobMatching(const JobArray &jobs, const ResumeArray &resumes)
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Job position entered: ";
    string jobPosition;
    getline(cin, jobPosition);

    cout << "Skills entered: ";
    string skills;
    getline(cin, skills);

    double thresholdPct = 0.0;
    cout << "Enter threshold % (0 - 100): ";
    cin >> thresholdPct;

    string cleanUserSkills = normalizeText(skills);
    string qUserSkills[50];
    int qUserCount = 0;
    extractSkills(cleanUserSkills, qUserSkills, qUserCount);

    cout << "\n===== Job Matching ("
         << (g_searchMode == MODE_LINEAR ? "Linear" : "Two-pointer")
         << ") =====\n";
    cout << "Job position entered: " << jobPosition << "\n";
    cout << "Skills entered: " << skills << "\n";
    cout << "Threshold: " << thresholdPct << "%\n\n";

    int totalMatchCount = 0;
    size_t memBefore = getProcessMemoryBytes();
    auto start = chrono::high_resolution_clock::now();
    for (int j = 0; j < jobs.getSize(); ++j)
    {
        const Job &J = jobs.getArray()[j];

        if (toLowerCopy(J.role).find(toLowerCopy(jobPosition)) == string::npos)
            continue;
        if (qUserCount > 0)
        {
            if (!containsAllSkillsTwoPointer(J.skills, J.skillCount, qUserSkills, qUserCount))
                continue;
        }

        const int denom = (J.skillCount > 0 ? J.skillCount : 1);

        int maxR = resumes.getSize();
        ScoredRes *matches = new ScoredRes[maxR];
        int matchCount = 0;

        for (int r = 0; r < resumes.getSize(); ++r)
        {
            const Resume &R = resumes.getArray()[r];
            int overlap = 0;

            if (g_searchMode == MODE_LINEAR)
            {
                // for (int a = 0; a < J.skillCount; ++a)
                // {
                //     for (int b = 0; b < R.skillCount; ++b)
                //     {
                //         if (J.skills[a] == R.skills[b])
                //         {
                //             ++overlap;
                //             break;
                //         }
                //     }
                // }

                overlap = countSkillMatchesLinear(J.skills, J.skillCount, R.skills, R.skillCount);
            }
            else
            {
                overlap = countSkillMatchesTwoPointer(J.skills, J.skillCount, R.skills, R.skillCount);
            }

            if (overlap <= 0)
                continue;

            if (qUserCount > 0)
            {
                if (!containsAllSkillsTwoPointer(R.skills, R.skillCount, qUserSkills, qUserCount))
                    continue;
            }

            double pct = (100.0 * overlap) / denom;
            if (pct < thresholdPct)
                continue;

            if (matchCount < maxR)
            {
                matches[matchCount].rIdx = r;
                matches[matchCount].overlap = overlap;
                matches[matchCount].pct = pct;
                ++matchCount;
            }
        }

        if (matchCount == 0)
        {
            delete[] matches;
            continue;
        }

        for (int a = 0; a < matchCount - 1; ++a)
        {
            int best = a;
            for (int b = a + 1; b < matchCount; ++b)
            {
                bool better = false;
                if (matches[b].pct > matches[best].pct)
                    better = true;
                else if (matches[b].pct == matches[best].pct)
                {
                    if (matches[b].overlap > matches[best].overlap)
                        better = true;
                    else if (matches[b].overlap == matches[best].overlap)
                    {
                        const Resume &RB = resumes.getArray()[matches[b].rIdx];
                        const Resume &RA = resumes.getArray()[matches[best].rIdx];
                        if (RB.id < RA.id)
                            better = true;
                    }
                }
                if (better)
                {
                    auto tmp = matches[b];
                    matches[b] = matches[best];
                    matches[best] = tmp;
                }
            }
        }

        cout << "Job ID " << J.id << "\n";
        cout << "Job role " << J.role << "\n";
        cout << "Skills: [";
        for (int k = 0; k < J.skillCount; ++k)
            cout << J.skills[k] << (k + 1 < J.skillCount ? ", " : "");
        cout << "]\n";

        int show = (matchCount < 10 ? matchCount : 10);
        for (int i = 0; i < show; ++i)
        {
            const ScoredRes &m = matches[i];
            const Resume &R = resumes.getArray()[m.rIdx];
            cout << "Resume ID " << R.id << "  Skill matched "
                 << m.overlap << "/" << denom << " (" << (int)(m.pct + 0.5) << "%)\n";
            cout << "Skills: [";
            for (int k = 0; k < R.skillCount; ++k)
                cout << R.skills[k] << (k + 1 < R.skillCount ? ", " : "");
            cout << "]\n";
            if (qUserCount > 0)
            {
                cout << "  | Matched user skills: [";
                bool first = true;
                for (int us = 0; us < qUserCount; ++us)
                {
                    for (int rk = 0; rk < R.skillCount; ++rk)
                    {
                        if (qUserSkills[us] == R.skills[rk])
                        {
                            if (!first)
                                cout << ", ";
                            cout << qUserSkills[us];
                            first = false;
                            break;
                        }
                    }
                }
                cout << "]\n";
            }
        }
        cout << "\n";

        totalMatchCount += matchCount;

        delete[] matches;
    }
    auto end = chrono::high_resolution_clock::now();
    size_t memAfter = getProcessMemoryBytes();

    long long diff = static_cast<long long>(memAfter) - static_cast<long long>(memBefore);
    if (diff < 0)
        diff = 0;

    chrono::duration<double> duration = end - start;
    cout << "\nExecution time: " << duration.count() << " seconds.\n";
    cout << "Memory usage: " << diff << " bytes (" << diff / 1024.0 << " KB)\n";
    cout << "Total matched resumes across all jobs: " << totalMatchCount << "\n";
}
