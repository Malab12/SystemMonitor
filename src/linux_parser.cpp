#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value("Linux"); // Default Value 
  std::ifstream fileReader(kOSPath);
  if (fileReader.is_open()) {
    while (std::getline(fileReader, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream lineReader(line);
      while (lineReader >> key >> value) {
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memoryTotal = LinuxParser::ParseMemInfo(filterMemTotalString);
  float memoryFree = LinuxParser::ParseMemInfo(filterMemFreeString);
  float memoryUsed = memoryTotal - memoryFree;
  return memoryUsed / memoryTotal;
}

// Accessor function defined to access values of the processor count from meminfo file.
float LinuxParser::ParseMemInfo(std::string key){
  string line;
  string key_;
  float value;
  std::ifstream fileReader(kProcDirectory + kMeminfoFilename);
  if (fileReader.is_open()) {
    while (std::getline(fileReader, line)) {
      std::istringstream lineReader(line);
      while (lineReader >> key_ >> value) {
        if (key_ == key) {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long systemUpTime;
  string line;
  
  std::ifstream fileReader(kProcDirectory + kUptimeFilename);
    if (fileReader.is_open()) {
      fileReader >> systemUpTime;
      return systemUpTime;
  }
  return systemUpTime;
 
}

/*
 * Read and return utilization
 * Return type changed to float based on the helpful answer from the forum https://knowledge.udacity.com/questions/141752
 * The function is implemented under process.cpp. This was to overcome the problem with operator overloading.
 * Referred to the helpful post from the forum: https://knowledge.udacity.com/questions/96882
 */
// float LinuxParser::CpuUtilization(int pid) {}


// Read and return the total number of processes
int LinuxParser::TotalProcesses() {return LinuxParser::ParseStat(kStatFilename, filterProcesses);}

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() {return LinuxParser::ParseStat(kStatFilename, filterRunningProcesses);}

// Utility accessor function defined to access values from stat files.
int LinuxParser::ParseStat(std::string file_name, std::string key){
  string line;
  string key_;
  int value;
  std::ifstream fileReader(kProcDirectory + file_name); // streams from this file
  if (fileReader.is_open()) { // Checks if the file is open
    while (std::getline(fileReader, line)) { // Executes upto the last line
      std::istringstream lineReader(line); // reads a string until whitespace is reached (<sstream> lib)
      while (lineReader >> key_ >> value) { // extracts the keys and values. Here we need only the 1st and 2nd strings
        if (key_ == key) {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string commandLine;
  std::ifstream fileReader(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (fileReader.is_open()) {
    std::getline(fileReader, commandLine);
    return commandLine;
  }
  return commandLine;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  int ram_ = LinuxParser::ParseStat(to_string(pid) + kStatusFilename, filterProcMem);
  return to_string(ram_ / 1000);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  int Uid_ = LinuxParser::ParseStat(to_string(pid) + kStatusFilename, filterUID);
  return to_string(Uid_);
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, key, value, x, userName;
  string uid = Uid(pid);
  std::ifstream fileReader(kPasswordPath);
  if (fileReader.is_open()) {
    while (std::getline(fileReader, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineReader(line);
      lineReader >> key >> x >> value;
      if (value == uid) {
        userName = key;
        return userName;
      }
    }
  } 
  return userName; }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long processUpTime{0};
  string line, upTime;
  
  std::ifstream fileReader(kProcDirectory + to_string(pid) + kStatFilename);
    if (fileReader.is_open()) {
      std::getline(fileReader, line);
      std::istringstream lineReader(line);
      for (int i = 0; i<22; ++i){
      lineReader >> upTime;
      }
    }
  processUpTime = LinuxParser::UpTime() - std::stol(upTime) / sysconf(_SC_CLK_TCK);
  return processUpTime;
}
