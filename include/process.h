#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

using std::string;
using std::vector;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  vector<string> GetStat() const;
  void Refresh(); // to refresh member variables from disk
  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string command_;
  vector<string> stat_;
};

#endif