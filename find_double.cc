//
// Brutal force search for double number |x| that satisfies the following
// property:
//  1. |x| is integer
//  2. std::llround(x) != static_cast<int64>(x)
//
#include <cmath>
#include <iostream>

typedef long long int64;

static const int64 kInterval = 1024 * 1024 * 256;

int main(int argc, char *argv[]) {
  int64 num_of_found = 0;
  double integer = 0.0;
  // If argc is even, step = -1.0, otherwise step = +1.0
  double step = static_cast<double>((argc % 2) * 2 - 1) * 1.0;
  while (true) {
    int64 llround_res = std::llround(integer);
    int64 scast_res = static_cast<int64>(integer);
    if (llround_res != scast_res) {
      num_of_found++;
      std::cout << "(" << llround_res << ", " << scast_res << ")" << std::endl;
    }
    double next = integer + step;
    if (integer == next) {
      break;
    } else {
      integer = next;
    }
    if (llround_res % kInterval == 0) {
      std::cerr << llround_res << std::endl;
    }
  }

  std::cout << "num_of_found = " << num_of_found << std::endl;
  return 0;
}
