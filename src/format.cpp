#include <string>
#include <cmath>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  int hh, ss, mm;
  string _ss, _mm,_hh, time;
  
  hh = seconds/3600;
  mm = (seconds%3600)/60;
  ss = (seconds%3600)%60;

  // Convert to strings
  _ss = to_string(ss);
  _mm = to_string(mm);
  _hh = to_string(hh);
  
  // to output as 2 digit ints
  _ss.insert(0, 2 - _ss.length(), '0');
  _mm.insert(0, 2 - _mm.length(), '0');
  _hh.insert(0, 2 - _hh.length(), '0');

  time = _hh + ":" + _mm + ":" + _ss;
  
  return time;
}