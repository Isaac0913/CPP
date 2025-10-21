#include "measure_performance.hpp"
#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <chrono>
#include <ctime>

size_t lgetMemoryUsageKB()
{
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter)))
    {
        return memCounter.WorkingSetSize / 1024; // Convert bytes to KB
    }
    return 0;
}

PerformanceResult measurePerformance(const function<void()> &taskToMeasure)
{
    size_t memBefore = lgetMemoryUsageKB();
    auto start = chrono::high_resolution_clock::now();

    taskToMeasure();

    auto end = chrono::high_resolution_clock::now();
    size_t memAfter = lgetMemoryUsageKB();
    chrono::duration<double> duration = end - start;
    PerformanceResult result;
    result.duration_seconds = duration.count();
    result.memory_kb = (memAfter > memBefore) ? (memAfter - memBefore) : 0;

    return result;
}

void printPerformance(const PerformanceResult &result)
{
    cout << "Execution time: " << result.duration_seconds << " seconds." << endl;
    cout << "Memory usage: " << result.memory_kb << "KB." << endl
         << endl;
}
