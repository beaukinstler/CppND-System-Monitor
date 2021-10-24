#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#include "format.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;
using std::setprecision;

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
  result = (1 - stof(MemAvail) / stof(MemTotal)) * 100;
  return result;
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  // get first number from the /proc/uptime file
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string line;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> line;
  }
  long result = std::stol(line);  // string to long
  return result;
}


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
  // get the "proceses" value from /proc/stat
  string results = (LinuxParser::ParseProcStat("processes"))[1];

  // if empty string still, return 0
  if (results == "") {
    return 0;
  } else {
    return std::stoi(results);
  }
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  // get the "procs_running" value from /proc/stat
  string results = (LinuxParser::ParseProcStat("procs_running"))[1];
  // if empty string still, return 0
  if (results == "") {
    return 0;
  } else {
    return std::stoi(results);
  }
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kCmdlineFilename);
  // read cmdline file for the PID in /proc/PID/cmdline
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.length() > 0) {
        return line;
      }
    }
  }

  // clean up name if needed

  return "";
}

// helper to reduce the same code between functions
string LinuxParser::GetValuePidProcStatus(int pid, string key, int index) {
  // add colon to match the expected format
  key += ":";
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatusFilename);
  // read stats file for the PID in /proc/PID/status
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream values(line);
      vector<string> results{};
      std::string token;
      while (values >> token) {
        results.push_back(token);
      }
      if (results[0] == key) {
        return results[index];
      }
    }
  }

  // return "" if not found
  // users of this function will need to handle this for
  // their use cases.
  return "";
}

// Done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatusFilename);
  // read stats file for the PID in /proc/PID/status
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream values(line);
      vector<string> results{};
      std::string token;
      while (values >> token) {
        results.push_back(token);
      }
      // Based on a Udacity review, and the man page for proc(5), it seems like VmSize is not a good value to use,
      // for physical RAM size.
      // It's what was given by the assignment, but better to use VmRSS values instead
      // So, I'm adding that change.
      // if (results[0] == ("VmSize:")) {
      if (results[0] == ("VmRSS:")) {

        // rounding double based on stack overflow answer https://stackoverflow.com/a/29200671
        std::stringstream stream;
        double result =  std::stoi(results[1])/1024.0;
        stream << std::fixed << setprecision(2) << result;
        return stream.str();

      }
    }
  }

  // clean up name if needed
  // return 0 if not found
  return "0";
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string result = "-1";
  result = LinuxParser::GetValuePidProcStatus(pid, "Uid", 1);
  return (result != "" ? result : "-1");
}

// Done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  // set the delimiter on the passwd file
  char delim = ':';

  // get the Uid of process
  string uid = LinuxParser::Uid(pid);

  // parse the file
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream values(line);
      vector<string> results{};
      std::string token;

      // fill up the results until no more delim and end of line
      while (std::getline(values, token, delim)) {
        results.push_back(token);
      }

      if (results[2] == uid) {
        return results[0];
      }
    }
  }
  return "";
}

// parse the /proc/<pid>/stat file, and return all tokens
vector<string> LinuxParser::GetPidStat(int pid) {
  string line;
  vector<string> results{};
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatFilename);
  // read stat file for the PID in /proc/PID/status
  if (stream.is_open()) {
    // get the first and presumably only line
    std::getline(stream, line);

    // now that the line is loaded into line
    // get all the values into the results array
    std::istringstream values(line);

    std::string token;
    while (values >> token) {
      results.push_back(token);
    }
  }
  return results;
}

// Done: Read and return the uptime of a process
// according to proc(5) — Linux manual page, the value we need is going to be
// the 22nd value
long LinuxParser::UpTime(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatFilename);
  // read stat file for the PID in /proc/PID/status
  if (stream.is_open()) {
    // get the first and presumably only line
    std::getline(stream, line);

    // now that the line is loaded into line
    // get all the values into the results array
    std::istringstream values(line);
    vector<string> results{};
    std::string token;
    while (values >> token) {
      results.push_back(token);
    }

    // return the 22nd value as the clock ticks
    long clockTicks = stol(results[21]);

    // convert to seconds
    long time = clockTicks / sysconf(_SC_CLK_TCK);

    // // return though the time format function
    // return stol(Format::ElapsedTime(time));
    return time;
  }
  stream.close();

  // return 0 if not found
  return 0;
}
