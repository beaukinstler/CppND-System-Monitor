#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::sort;

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  vector<int> pids = LinuxParser::Pids();
  for (auto pid : pids) {
    System::processes_.push_back(Process(pid));
  }
}

// Done: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  System::processes_.clear();
  vector<int> pids = LinuxParser::Pids();
  for (auto pid : pids) {
    System::processes_.emplace_back(Process(pid));
  }

  // sorting as well, by the CPU utilization descending
  if( System::processes_.size() < 2){
    return System::processes_;
  }
  sort(System::processes_.begin(), System::processes_.end());
  return System::processes_;
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() {
  if (this->kernel_ == "") {
    this->kernel_ = LinuxParser::Kernel();
  }
  return this->kernel_;
}

// Done: Return the system's memory utilization
float System::MemoryUtilization() {
  float mu = LinuxParser::MemoryUtilization();
  return mu / 100.0;
}

// Done: Return the operating system name
std::string System::OperatingSystem() {
  // return string();
  return LinuxParser::OperatingSystem();
}

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Done: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Done: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }