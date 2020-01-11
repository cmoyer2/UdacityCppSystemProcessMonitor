#include "processor.h"
#include <string>
#include <iostream>

Processor::Processor() {};
Processor::  Processor(int user, int nice, int system, int idle, int iowait, int irq, int softirq, int steal, int guest, int guestnice) :
    user(user), nice(nice), system(system), idle(idle), iowait(iowait), irq(irq), softirq(softirq), steal(steal), guest(guest), guestnice(guestnice) {}

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    
    int usertime = user - guest;
    int nicetime = nice - guestnice;
    float idlealltime = idle + iowait;
    int systemalltime = system + irq + softirq;
    int virtualtime = guest + guestnice;
    
    double totaltime = (usertime + nicetime + systemalltime + idlealltime + steal + virtualtime);

    return (totaltime - idlealltime) / idlealltime;
 }