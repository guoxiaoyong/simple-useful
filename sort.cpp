#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

int main() {

  vector<string> V;
  string tmp;

  while (getline(cin, tmp))
    V.push_back(tmp);

  sort(V.begin(), V.end());
  copy(V.begin(), V.end(), ostream_iterator<string>(cout, "\n"));

  return 0;
}
