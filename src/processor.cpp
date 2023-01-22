#include "processor.h"

#include <sstream>

using std::string; 

// Return the aggregate CPU utilization
float Processor::Utilization() {
  string line;
  string key;
  std::ifstream fileReader(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
  if (fileReader.is_open()) {
    while (std::getline(fileReader, line)) {
      // Modifying string for easier parsing
      std::replace(line.begin(), line.begin() + 4, ' ', '_');
      std::istringstream lineReader(line);
      while (lineReader >> key) {
        if (key == "cpu_") {
          lineReader >> user_ >> nice_ >> system_ >> idle_ >> iowait_ >> irq_ >> softirq_ >> steal_ >> guest_ >> guest_nice_;
        }
      }
    }
 }
  
  idle = idle_ + iowait_;
  nonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
  total = idle + nonIdle;
  
  totalDelta = total - previousTotal;
  idleDelta = idle - previousIdle;
  previousTotal = total;
  previousIdle = idle;

  return (totalDelta - idleDelta) / totalDelta;
}