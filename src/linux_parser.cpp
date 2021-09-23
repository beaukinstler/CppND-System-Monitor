#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line;
  string _temp;
  string MemTotal;
  string MemFree;
  string MemAvail;
  if (stream.is_open()) {
    // get total
    getline(stream, line);
    std::istringstream linestream(line);
    linestream >> _temp >> MemTotal;
    linestream.clear();

    // get free
    getline(stream, line);
    // linestream(line);
    linestream = std::istringstream(line);
    linestream >> _temp >> MemFree;

    // get avail
    getline(stream, line);
    linestream = std::istringstream(line);
    linestream >> _temp >> MemAvail;
  }

  stream.close();

  float result = 0.0;
  result = stof(MemAvail) / stof(MemTotal) * 100;
  return result;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string cpu;
  string line;
  vector<string> results = {};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    while (linestream) {
      string token = "";
      linestream >> token;
      results.push_back(token);
    }
  }

  stream.close();

  // erase the first element, which should be "CPU"
  if ((results.front()) == "cpu") {
    results.erase(results.begin());
    return results;
  }

  // if we don't have a line that starts with CPU, we can't be sure we
  // have the right data, so just send back empty vector.
  results.clear();
  return results;
}

// Done: Read and return the total number of processes

vector<string> LinuxParser::ParseProcStat(string key) {
  vector<string> result_line{};
  string line;
  string results = "";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    bool found = false;
    while (std::getline(stream, line) && found == false) {
      std::istringstream linestream(line);
      vector<string> linedata;
      while (linestream) {
        string token = "";
        linestream >> token;
        linedata.push_back(token);
      }
      if (linedata[0] == key) {
        found = true;
        for (auto d : linedata) {
          result_line.push_back(d);
        };
      }
    }
  }

  stream.close();

  return result_line;
}

int LinuxParser::TotalProcesses() {
  string line;
  string results = "";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    bool found = false;
    while (std::getline(stream, line) && found == false) {
      std::istringstream linestream(line);
      vector<string> linedata;
      while (linestream) {
        string token = "";
        linestream >> token;
        linedata.push_back(token);
      }
      if (linedata[0] == "procs_running") {
        found = true;
        results = linedata[1];
      }
    }
  }

  stream.close();

  // if empty string still, return 0
  if (results == "") {
    return 0;
  } else {
    return std::stoi(results);
  }
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
