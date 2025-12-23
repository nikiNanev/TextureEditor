#include "logger/profiler.h"

void profiler::report(std::string filename)
{
    std::cout << "Done Profiling!" << std::endl;

    std::ofstream output;
    output.open("../reports/" + filename, std::ios::app);

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(this->end - this->start).count();

    output << "Report stats: \n\n";
    output << "Function/Method: " << function << "\n";
    output << "Execution time: " << duration << " microseconds ( " << duration / 1000 << "  milliseconds )\n\n";

    output.close();
}