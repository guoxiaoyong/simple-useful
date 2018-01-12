#include <iostream>
#include <sstream>

using namespace std;

int main(void) {
  int arr[23];
  int n = (&arr)[1] - arr;
  std::cout << n << std::endl;

  int brr[4][5];
  std::cout << sizeof(brr) / sizeof(brr[0][0]) << std::endl;
  std::cout << (&brr)[1] - brr << std::endl;
  return 0;
}
