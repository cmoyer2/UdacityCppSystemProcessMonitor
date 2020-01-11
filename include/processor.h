#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  Processor();
  Processor(int user, int nice, int system, int idle, int iowait, int irq, int softirq, int steal, int guest, int guestnice);

  std::string to_string();
  // TODO: Declare any necessary private members
 private:
    int user;
    int nice;
    int system;
    int idle;
    int iowait;
    int irq;
    int softirq;
    int steal;
    int guest;
    int guestnice;
};

#endif