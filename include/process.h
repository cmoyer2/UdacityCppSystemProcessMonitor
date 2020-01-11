#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:

  int Pid();
  void Pid(int pid);   

  std::string User();
  void User(std::string user);    

  std::string Command();
  void Command(std::string command); 

  float CpuUtilization();
  void CpuUtilization(float cpuUtilization);  

  std::string Ram();
  void Ram(std::string ram);   

  long int UpTime();
  void UpTime(long int uptime); 

  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
    int pid;
    std::string user;
    std::string command;
    float cpuUtilization;
    std::string ram;
    long int uptime;
};

#endif