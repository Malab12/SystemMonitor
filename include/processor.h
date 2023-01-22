#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  float total;
  float idle;
  float nonIdle;
  float totalDelta;
  float idleDelta;
  float previousTotal;
  float previousIdle;


  // TODO: Declare any necessary private members
 private:
  float user_;
  float nice_;
  float system_;
  float idle_;
  float iowait_;
  float irq_;
  float softirq_;
  float steal_;
  float guest_;
  float guest_nice_;
  std::string processor_;
};

#endif