#include "array_menu.hpp"
#include "array_run.hpp"
#include "measure_performance.hpp"
#include "cleaner_loader.hpp"
#include "user_prompt.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <windows.h>
#include <psapi.h>

void mainMenu(JobArray &jobs, ResumeArray &resumes, JobArray &jobs_unsorted, ResumeArray &resumes_unsorted)
{
    int choice;
    do
    {
        cout << "\n===== MAIN MENU =====\n"
             << "1. View Jobs\n"
             << "2. Sort Jobs\n"
             << "3. Search Jobs (Sorted)\n"
             << "4. Search Jobs (Not Sorted)\n"
             << "5. View Resumes\n" // NEW
             << "6. Sort Resumes\n" // NEW
             << "7. Search Resumes (Sorted)\n"
             << "8. Search Resumes (Not Sorted)\n"
             << "9. Job Matching\n"
             << "10. Return\n"
             << "Choice (1-10): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            displayJobs(jobs);
            break;

        case 2: // Sort Jobs
        {
            int algo = promptSortAlgorithm();
            auto start = chrono::high_resolution_clock::now();
            if (jobs.getSize() > 0)
            {
                if (algo == 1)
                {
                    size_t memBefore = getProcessMemoryBytes();
                    auto s2 = chrono::high_resolution_clock::now();
                    mergeSortJobs(jobs.getArray(), 0, jobs.getSize() - 1);
                    auto e2 = chrono::high_resolution_clock::now();
                    size_t memAfter = getProcessMemoryBytes();
                    // (intentionally quiet like your current code)
                }
                else
                {
                    insertionSortJobs(jobs.getArray(), jobs.getSize());
                    cout << "Jobs sorted using Insertion Sort.\n";
                }
            }
            else
            {
                cout << "(no jobs to sort)\n";
            }
            displayJobs(jobs);
            break;
        }

        case 3: // Search Jobs (Sorted)
        {
            int sortAlgo = promptSortAlgorithm();
            if (jobs.getSize() > 0)
            {
                if (sortAlgo == 1)
                {
                    mergeSortJobs(jobs.getArray(), 0, jobs.getSize() - 1);
                    cout << "[Jobs] Resort complete: Merge Sort.\n";
                }
                else
                {
                    insertionSortJobs(jobs.getArray(), jobs.getSize());
                    cout << "[Jobs] Resort complete: Insertion Sort.\n";
                }
            }
            else
            {
                cout << "(no jobs to sort)\n";
            }

            int sAlgo = promptSearchAlgorithm();
            g_searchMode = (sAlgo == 1 ? MODE_LINEAR : MODE_TWO_POINTER);

            promptAndSearchJobs(jobs, false); // already sorted above
            break;
        }

        case 4: // Search Jobs (Not Sorted)
        {
            int sAlgo = promptSearchAlgorithm();
            g_searchMode = (sAlgo == 1 ? MODE_LINEAR : MODE_TWO_POINTER);
            promptAndSearchJobs(jobs_unsorted, false); // explicitly not sorted
            break;
        }

        case 5: // NEW: View Resumes
            displayResumes(resumes);
            break;

        case 6: // NEW: Sort Resumes
        {
            int algo = promptSortAlgorithm();
            if (resumes.getSize() > 0)
            {
                if (algo == 1)
                {
                    size_t memBefore = getProcessMemoryBytes();
                    auto s2 = chrono::high_resolution_clock::now();
                    mergeSortResumes(resumes.getArray(), 0, resumes.getSize() - 1);
                    auto e2 = chrono::high_resolution_clock::now();
                    size_t memAfter = getProcessMemoryBytes();
                    cout << "Resumes sorted using Merge Sort.\n";
                    double seconds = chrono::duration<double>(s2 - e2).count();
                    cout << "runtime: " << seconds << "\n";
                }
                else
                {
                    auto s2 = chrono::high_resolution_clock::now();
                    insertionSortResumes(resumes.getArray(), resumes.getSize());
                    auto e2 = chrono::high_resolution_clock::now();
                    cout << "Resumes sorted using Insertion Sort.\n";
                    double seconds = chrono::duration<double>(s2 - e2).count();
                    cout << "runtime: " << seconds << "\n";
                }
            }
            else
            {
                cout << "(no resumes to sort)\n";
            }
            displayResumes(resumes);
            break;
        }

        case 7: // Search Resumes (Sorted)
        {
            int sortAlgo = promptSortAlgorithm();
            if (resumes.getSize() > 0)
            {
                if (sortAlgo == 1)
                {
                    mergeSortResumes(resumes.getArray(), 0, resumes.getSize() - 1);
                    cout << "[Resumes] Resort complete: Merge Sort.\n";
                }
                else
                {
                    insertionSortResumes(resumes.getArray(), resumes.getSize());
                    cout << "[Resumes] Resort complete: Insertion Sort.\n";
                }
            }
            else
            {
                cout << "(no resumes to sort)\n";
            }

            int sAlgo = promptSearchAlgorithm();
            g_searchMode = (sAlgo == 1 ? MODE_LINEAR : MODE_TWO_POINTER);

            promptAndSearchResumes(resumes, false); // already sorted above
            break;
        }

        case 8: // Search Resumes (Not Sorted)
        {
            int sAlgo = promptSearchAlgorithm();
            g_searchMode = (sAlgo == 1 ? MODE_LINEAR : MODE_TWO_POINTER);
            promptAndSearchResumes(resumes_unsorted, false); // explicitly not sorted
            break;
        }

        case 9: // Job Matching
        {
            int sAlgo = promptSearchAlgorithm();
            g_searchMode = (sAlgo == 1 ? MODE_LINEAR : MODE_TWO_POINTER);
            runJobMatching(jobs, resumes);
            break;
        }

        case 10:
            cout << "Returning to data structures selection...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 10);
}

void arrayMainMenuEntry()
{
    // --- ARRAY DATA STRUCTURE SELECTED ---
    JobArray jobs(12000);
    JobArray jobs_unsorted(12000);
    ResumeArray resumes(12000);
    ResumeArray resumes_unsorted(12000);

    cout << "Loading jobs...\n";
    loadJobDescIntoArray(jobs);
    cloneJobs(jobs_unsorted, jobs);
    cout << "Loaded " << jobs.getSize() << " job descriptions.\n";

    cout << "Loading resumes...\n";
    loadResumeIntoArray(resumes);
    cloneResumes(resumes_unsorted, resumes);
    cout << "Loaded " << resumes.getSize() << " resumes.\n";

    // Now enter the array main menu
    mainMenu(jobs, resumes, jobs_unsorted, resumes_unsorted);
}
