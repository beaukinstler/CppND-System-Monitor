#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
  Process::pid_ = pid;
  Process::command_ = LinuxParser::Command(pid);
}

// TODO: Return this process's ID
int Process::Pid() { 
  return Process::pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// Done: Return the command that generated this process
string Process::Command() { return Process::command_; }

// Done: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::pid_); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}