#ifndef MEASURE_PERFORMANCE_HPP
#define MEASURE_PERFORMANCE_HPP

#include <functional>
using namespace std;

// to store the performance result
struct PerformanceResult
{
    double duration_seconds;
    size_t memory_bytes; // memory difference before and after executing the process
};

size_t getMemoryUsageKB();

size_t getProcessMemoryBytes();

// returns the result struct that contains the time and memory result
PerformanceResult measurePerformance(const function<void()> &taskToMeasure);
// how to use:
// for example: PerformanceResult result = measurePerformance([&]() {code to execute here...});
// then can just do: cout << result.duration_seconds or cout << result.memory_kb

void printPerformance(const PerformanceResult &result);

#endif
