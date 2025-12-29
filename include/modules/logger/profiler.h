#ifndef _PROFILER_H
#define _PROFILER_H

#include <iostream>
#include <chrono>
#include <string>
#include <fstream>

typedef struct profiler
{
    std::chrono::_V2::high_resolution_clock::time_point start;
    std::chrono::_V2::high_resolution_clock::time_point end;

    std::string function{""};

    auto inline start_timer()
    {
        return std::chrono::high_resolution_clock::now();
    }

    auto inline end_timer()
    {
        return std::chrono::high_resolution_clock::now();
    }

    void report(std::string filename);
} profiler;

#endif