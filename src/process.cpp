#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"

using std::string;
using std::stoi;
using std::to_string;
using std::vector;

int Process::Pid() { return pid; }

void Process::Pid(int pid) {
    this->pid = pid;
}

float Process::CpuUtilization() { return cpuUtilization; }

void Process::CpuUtilization(float cpuUtilization) {
    this->cpuUtilization = cpuUtilization;
}

string Process::Command() { return command; }

void Process::Command(string command) {
    this->command = command;
}

string Process::Ram() { return ram; }

void Process::Ram(string ram) {
    this->ram = ram;
}

string Process::User() { return user; }

void Process::User(string user) {
    this->user = user;
}

long int Process::UpTime() { return uptime; }

void Process::UpTime(long int uptime) {
    this->uptime = uptime;
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
     float this_temp = this->cpuUtilization;
     float a_temp = a.cpuUtilization;
     return this_temp < a_temp;
}

bool Process::operator>(Process const& a) const {
     float this_temp = this->cpuUtilization;
     float a_temp = a.cpuUtilization;
     return this_temp > a_temp;    
}