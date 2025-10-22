#include "job_resume_linked_list.hpp"
#include "cleaner_loader.hpp"
#include "linked_list_menu.hpp"
#include "measure_performance.hpp"
#include "linked_list_insertion_sort.hpp"
#include "linked_list_merge_sort.hpp"
#include "string_utils.hpp"
#include "linked_list_search.hpp"
#include "linked_list_job_matching.hpp"
#include "user_prompt.hpp"
#include <chrono>
#include <iostream>
#include <string>
using namespace std;

void linkedListMainMenu()
{
    JobLinkedList jobs;
    ResumeLinkedList resumes;
    cout << "Loading jobs...\n";
    loadJobDescIntoLinkedList(jobs);
    cout << "Loading resumes...\n";
    loadResumeIntoLinkedList(resumes);

    int choice;
    do
    {
        cout << "\n===== LINKED LIST MENU =====\n"
             << "1. View Jobs\n"
             << "2. View Resumes\n"
             << "3. Sort Jobs\n"
             << "4. Sort Resumes\n"
             << "5. Search Jobs (Sorted)\n"
             << "6. Search Jobs (Unsorted)\n"
             << "7. Search Resumes (Sorted)\n"
             << "8. Search Resumes (Unsorted)\n"
             << "9. Job Matching\n"
             << "0. Return\n"
             << "Choice (1-0): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            jobs.display(10);
            break;

        case 2:
            resumes.display(10);
            break;

        case 3:
        {
            int algo = promptSortAlgorithm(); // ask merge or insertion

            if (algo == 1)
            {
                JobLinkedList mergeSorted;
                PerformanceResult result = measurePerformance([&]()
                                                              { mergeSorted = mergeSortJobCopy(jobs); });

                cout << "\nJobs sorted using Merge Sort.\n";
                printPerformance(result);
                mergeSorted.display(10);
            }
            else
            {
                JobLinkedList insertionSorted;
                PerformanceResult result = measurePerformance([&]()
                                                              { insertionSorted = sortInsertionJob(jobs); });

                cout << "\nJobs sorted using Insertion Sort.\n";
                printPerformance(result);
                insertionSorted.display(10);
            }
            break;
        }

        case 4:
        {
            int algo = promptSortAlgorithm(); // ask merge or insertion

            if (algo == 1)
            {
                ResumeLinkedList mergeSorted;
                PerformanceResult result = measurePerformance([&]()
                                                              { mergeSorted = mergeSortResumeCopy(resumes); });

                cout << "\nResumes sorted using Merge Sort.\n";
                printPerformance(result);
                mergeSorted.display(10);
            }
            else
            {
                ResumeLinkedList insertionSorted;
                PerformanceResult result = measurePerformance([&]()
                                                              { insertionSorted = sortInsertionResume(resumes); });

                cout << "\nResumes sorted using Insertion Sort.\n";
                printPerformance(result);
                insertionSorted.display(10);
            }
            break;
        }
        case 5: // Search Jobs (Sorted)
        case 6: // Search Jobs (Unsorted)
        case 7: // Search Resumes (Sorted)
        case 8: // Search Resumes (Unsorted)
        {
            int algo = promptSearchAlgorithm(); // 1=linear, 2=two-pointer
            SearchMode searchMode = (algo == 1 ? MODE_LINEAR : MODE_TWO_POINTER);
            SearchQueryData userQuery = promptSearchQuery(true);

            PerformanceResult result;
            if (choice == 5)
                result = measurePerformance([&]()
                                            { searchJobs(searchMode, true, userQuery, jobs); });
            else if (choice == 6)
                result = measurePerformance([&]()
                                            { searchJobs(searchMode, false, userQuery, jobs); });
            else if (choice == 7)
                result = measurePerformance([&]()
                                            { searchResumes(searchMode, true, userQuery, resumes); });
            else
                result = measurePerformance([&]()
                                            { searchResumes(searchMode, false, userQuery, resumes); });

            printPerformance(result);
            break;
        }

        case 9: // Job Matching
        {
            int algo = promptSearchAlgorithm();
            SearchMode searchMode = (algo == 1 ? MODE_LINEAR : MODE_TWO_POINTER);
            MatchQueryData matchQuery = promptMatchQuery(true, searchMode);

            PerformanceResult result = measurePerformance([&]()
                                                          { linkedlistRunJobMatching(searchMode, matchQuery, jobs, resumes); });
            printPerformance(result);
            break;
        }

        case 0:

            cout << "Returning to data structures selection...\n";

            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}
