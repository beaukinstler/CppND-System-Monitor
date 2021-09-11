#include "format.h"

#include <string>
#include <vector>
#include <cassert>

using std::string;
using std::vector;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds ) {
  vector<int> time_ints;
  // get the remainder for of seconds
  time_ints.push_back(seconds % 60);
  // get the min before finding hours
  time_ints.push_back(seconds / 60);
  // get the number of hours
  time_ints.push_back(time_ints[1] / 60);
  // now get the remainder of min
  time_ints[1] = time_ints[1] % 60;
  assert(time_ints[1] < 60 && time_ints[0] < 60);

  vector<string> time_result;

  for( int time_numbers : time_ints){
    //   account for single digits, and add a 0 if 9 or less
      if(time_numbers > 9){
          time_result.push_back(std::to_string(time_numbers));
      }
      else{
          time_result.push_back("0" + std::to_string(time_numbers));
      }
  }

  return time_result[2] + ":" + time_result[1] + ":" + time_result[0] ;
}