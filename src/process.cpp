#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  Process::pid_ = pid;
  Process::command_ = LinuxParser::Command(pid);
  Process::stat_ = Process::GetStat();
}

void Process::Refresh(){
  Process::stat_ = Process::GetStat();
}

vector<string> Process::GetStat() const {
  // Don't read from disk if already in the member variable
  // if(Process::stat_.size() > 0){
  //   return Process::stat_;
  // }
  return LinuxParser::GetPidStat(Process::pid_);
}

// Done: Return this process's ID
int Process::Pid() { return Process::pid_; }

// Done: Return this process's CPU utilization
// using the logic here
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599

float Process::CpuUtilization() const {
  // token 1 from /proc/uptime
  System system;
  int sysUptime = system.UpTime();
  // tokens 14, 15, 16, 17
  int utime, stime, cutime, cstime;
  float totaltime;

  // use the function already built for this
  int elapsedTime;
  // elapsedTime = LinuxParser::UpTime(this->pid_);

  // int seconds = sysUptime - elapsedTime;


  // get file data
  vector<string> stat = Process::GetStat();

  // make sure there are stats for this process. If not return 0
  if(stat.size() > 0 ){
    utime = stoi(stat[13]);
    stime = stoi(stat[14]);
    cutime = stoi(stat[15]);
    cstime = stoi(stat[16]);

    // convert ticks to seconds using Hertz as sysconf(_SC_CLK_TCK))
    totaltime = ((utime + stime + cutime + cstime) * 1.0) / sysconf(_SC_CLK_TCK);
    int seconds = sysUptime - (stoi(stat[21]) / sysconf(_SC_CLK_TCK));


    float result =  ( (totaltime * 1.0) / (seconds * 1.0));
    return result;  
  }
  else{
    return 0;
  }
  
}

// Done: Return the command that generated this process
string Process::Command() { 
  if(Process::command_.length() > 50){
    return Process::command_.substr(0,47) + "...";
  } 
  else{
    return Process::command_;
  }
}

// Done: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::pid_); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::pid_); }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime() - LinuxParser::UpTime(Process::pid_); 
}

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  if(a.CpuUtilization() >= Process::CpuUtilization()){
    return false;
  }
  else{
    return true;
  }
}