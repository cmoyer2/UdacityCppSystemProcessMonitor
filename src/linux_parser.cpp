#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::stoi;
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
  string os, version, kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
    string line, key, value, kb;
    float total, free;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value >> kb;
      
      if(key == "MemTotal:") {
        total = stof(value);
      }

      if(key == "MemFree:") {
        free = stof(value);
      }
    }
  }
  return (total-free)/total;
 }

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, uptime, idletime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime >> idletime;
      return std::stol(uptime);
    }
  }
  return 0;
 }

// DONE: Read and return CPU utilization
Processor LinuxParser::CpuUtilization() {
  Processor processor;
  string line, temp, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> temp >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;

      processor = Processor(stoi(user), stoi(nice), stoi(nice), stoi(system), stoi(idle), stoi(iowait), stoi(irq), stoi(steal), stoi(guest), stoi(guestnice));

      return processor;
    }
  }
  return processor; 
 }

 float round(float var) {
   float value = (int) (var * 1000 + .5);
   return (float)value / 1000;
 }

 float LinuxParser::CpuUtilization(int pid) {
   vector<string> stats = Stats(pid);

   double uptime = (double) UpTime();
   double utime = stod(stats[13]);
   double stime = stod(stats[14]);
   double cutime = stod(stats[15]);
   double cstime = stod(stats[16]);
   double starttime = stod(stats[21]);
   double hertz = (double) sysconf(_SC_CLK_TCK);
   double total_time = utime + stime + cutime + cstime;
   double seconds = uptime - (starttime / hertz);
   double temp = round(((total_time / hertz) / seconds));
   return temp;
 }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return Processes("processes");
}

int LinuxParser::Processes(std::string type) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(type) != std::string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> value;
        return stoi(value);
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return Processes("procs_running");
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      return line;
    }
  }
  return line;
 }

vector<string> LinuxParser::Stats(int pid) {
  string line;
  vector<string> stats;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::string stat;
      while(std::getline(linestream, stat, ' ')) {
        stats.push_back(stat);
      }
    }
  }
  return stats;  
}

std::string LinuxParser::Statuses(int pid, std::string status) {
  string line, key, value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == status) {
        return value;
      }
    }
  }
  return value;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string ram = Statuses(pid, "VmSize:");
  int mb = stoi(ram)/1000;
  return to_string(mb);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return Statuses(pid, "Uid:");
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(string uid) { 
  string line, username, temp, userId;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      //username
      int index = line.find(":");
      username = line.substr(0, index);

      // x value
      line = line.substr(index + 1);
      index = line.find(":");
      temp = line.substr(0, index);

      // user ID
      line = line.substr(index + 1);
      index = line.find(":");
      userId = line.substr(0, index);

      if(userId == uid) {
        return username;
      }
    }
  }
  return username;
  
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
   string uptime = Stats(pid)[21];
   long ticks = sysconf(_SC_CLK_TCK);
   return stol(uptime)/ticks;
}