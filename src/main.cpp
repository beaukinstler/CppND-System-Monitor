#include "ncurses_display.h"
#include "system.h"



#include <string>
#include <cassert>
#include "format.h"

#include "linux_parser.h"

int main() {
  // testing

  // testing code given for PIDs

  vector<int> pidList = LinuxParser::Pids();
  assert(!pidList.empty());

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