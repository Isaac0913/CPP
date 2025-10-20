#include "job_resume_linked_list.hpp"
#include "cleaner_loader.hpp"
#include "measure_performance.hpp"
#include "linked_list_insertion_sort.hpp"
#include "string_utils.hpp"
#include <iostream>
#include <string>
using namespace std;

// When execute code, must include { g++ *.cpp -o main -lpsapi } like this:
// cd "c:\Users\...\...\" ; if ($?) { g++ *.cpp -o main -lpsapi } ; if ($?) { .\main }

int main()
{
    JobLinkedList jobs;
    ResumeLinkedList resumes;

    cout << "Loading jobs...\n";
    loadJobDescIntoLinkedList(jobs);

    cout << "Loading resumes...\n";
    loadResumeIntoLinkedList(resumes);

    // jobs.display(10);
    // resumes.display(10);

    // execute one by one/one at a time to get accurate measurement
    // PerformanceResult displayJobResult =
    //     measurePerformance([&]()
    //                        { jobs.display(10); });
    // printPerformance(displayJobResult);

    // PerformanceResult displayResumeResult =
    //     measurePerformance([&]()
    //                        { jobs.display(10); });
    // printPerformance(displayResumeResult);

    // JobLinkedList sortedInsertion;
    // PerformanceResult displayResumeResult =
    //     measurePerformance([&]()
    //                        { sortedInsertion = sortInsertionJob(jobs); });
    // printPerformance(displayResumeResult);
    // sortedInsertion.display(10);

    // JobLinkedList jobs2;
    // JobLinkedList newList = sortInsertionJob(jobs);

    // PerformanceResult result;
    // result = measurePerformance([&]()
    //                             { }

    // );
    // printPerformance(result);

    // mainMenu(jobs, resumes);
    return 0;
}
