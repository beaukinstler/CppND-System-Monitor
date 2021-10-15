#include <cassert>
#include <iostream>
#include <string>

#include "format.h"
#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"
using std::cout;

int main() {
  // testing

  // testing code given for PIDs

  vector<int> pidList = LinuxParser::Pids();
  assert(!pidList.empty());

  // testing linux parser GetPidStat
  vector<string> pidstats = LinuxParser::GetPidStat(pidList.front());
  assert(pidstats.size() > 0);

  for (auto pid : pidList) {
    // testing LinuxParser::Command(int pid)
    cout << (LinuxParser::Command(pid)) << "\n";

    // testing  LinuxParser::Ram(int pid)
    string temp = LinuxParser::Ram(pid);
    assert(temp.length() > 0);

    // test LinuxParser::GetValuePidProcStatus(pid, "Uid", 1)
    temp = LinuxParser::GetValuePidProcStatus(pid, "Uid", 1);
    assert(temp.length() > 0);
  }

  // // testing Format::ElapsedTime
  long timeinsec = 10040;
  std::string result = Format::ElapsedTime(timeinsec);
  assert(result == "02:47:20");

  // testing linux parser
  int total_procs = LinuxParser::TotalProcesses();
  assert(total_procs > 0);

  // testing
  float memUsed = LinuxParser::MemoryUtilization();
  assert(memUsed <= 100 && memUsed > 0.00);

  // testing
  std::string kernel = LinuxParser::Kernel();
  assert(kernel.length() > 0);

  // end of testing

  System system;
  NCursesDisplay::Display(system);
}