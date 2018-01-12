#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool ConvertAndReverseNumber(string &num, vector<int> &n) {
  for (int i = 0; i < num.length(); i++) {
    n[num.length() - 1 - i] = num[i] - '0';
  }
  return true;
}

void ReverseAndConvertResult(vector<int> r, string &res) {
  int skip0 = 0;
  for (int i = 0; i < r.size(); i++) {
    if (r[r.size() - 1 - i] == 0)
      skip0++;
    else
      break;
  }
  for (int i = 0; i < r.size() - skip0; i++) {
    res.push_back(r[r.size() - 1 - i - skip0] + '0');
  }
}

void PrintVector(vector<int> &num) {
  cout << "num = ";
  for (int i = 0; i < num.size(); i++) {
    cout << num[i] << " ";
  }
  cout << endl;
}

bool BigNumberMultiply(string &num1, string &num2, string &res) {
  vector<int> n1(num1.length());
  vector<int> n2(num2.length());
  vector<int> r(num1.length() + num2.length());

  ConvertAndReverseNumber(num1, n1);
  PrintVector(n1);
  ConvertAndReverseNumber(num2, n2);
  PrintVector(n2);
  // multiply, key point n1[i] * n2[j] --> r[i+j]
  for (int i = 0; i < n1.size(); i++) {
    for (int j = 0; j < n2.size(); j++) {
      r[i + j] += n1[i] * n2[j];
    }
  }
  PrintVector(r);
  // deal with carry bit
  int cb = 0;
  for (int i = 0; i < r.size(); i++) {
    r[i] += cb;
    cb = r[i] / 10;
    r[i] %= 10;
  }
  if (cb != 0)
    r.push_back(cb);
  PrintVector(r);
  ReverseAndConvertResult(r, res);
  return true;
}

int main(void) {

  string number1, number2, result;
  cout << "please give 2 big numbers" << endl;
  cin >> number1 >> number2;
  if (BigNumberMultiply(number1, number2, result)) {
    cout << "result = " << result << endl;
    int pause;
    cin >> pause;
  } else
    cout << "parameter error!" << endl;
  return 0;
}
