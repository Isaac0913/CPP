#include "measure_performance.hpp"
#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <chrono>
#include <ctime>

size_t getMemoryUsageKB()
{
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter)))
    {
        return memCounter.WorkingSetSize / 1024; // Convert bytes to KB
    }
    return 0;
}

size_t getProcessMemoryBytes()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS *>(&pmc), sizeof(pmc)))
    {
        // pmc.PrivateUsage is often a better representation of private memory usage
        return static_cast<size_t>(pmc.WorkingSetSize); // or pmc.PrivateUsage
    }
    return 0;
}

PerformanceResult measurePerformance(const function<void()> &taskToMeasure)
{
    size_t memBefore = getProcessMemoryBytes();
    auto start = chrono::high_resolution_clock::now();

    taskToMeasure();

    auto end = chrono::high_resolution_clock::now();
    size_t memAfter = getProcessMemoryBytes();
    chrono::duration<double> duration = end - start;
    PerformanceResult result;
    result.duration_seconds = duration.count();
    result.memory_bytes = (memAfter > memBefore) ? (memAfter - memBefore) : 0;

    return result;
}

void printPerformance(const PerformanceResult &result)
{
    cout << "Execution time: " << result.duration_seconds << " seconds." << endl;
    cout << "Memory usage: " << result.memory_bytes << "bytes." << endl
         << endl;
}
