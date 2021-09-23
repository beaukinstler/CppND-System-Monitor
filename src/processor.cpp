#include "processor.h"

#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::vector;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpuStat =
      LinuxParser::CpuUtilization();  // from /proc/stat file

  float idleStats = stof(cpuStat[LinuxParser::kSoftIRQ_]) +
                    stof(cpuStat[LinuxParser::kIdle_]);

  float totalStats = 0;
  for (auto stat : cpuStat) {
    try {
      totalStats += stof(stat);
    } catch (const std::exception& e) {
      //std::cerr << e.what() << '\n';
    }
  }
  float nonIdle = totalStats - idleStats;
  float result = (nonIdle / totalStats);
  return result;
}