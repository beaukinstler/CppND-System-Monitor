#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  if(this->kernel_ == ""){
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
int System::RunningProcesses() { 

  return LinuxParser::RunningProcesses();
  
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }