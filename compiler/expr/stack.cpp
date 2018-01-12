#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <stack>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstdlib>

using namespace std;

typedef stack<double, list<double>> stack_double;
typedef stack<char, list<char>> stack_char;

/* Calculator */
class Calculator {
public:
  Calculator();
  double Eval(string &expr);

private:
  bool IsOperator(char ch);
  int Compare(char optr1, char optr2);
  double Simple_Eval(double lopnd, char optr, double ropnd);

  /* Priority */
  struct Priority {
    int In_Stack;
    int In_Coming;

    Priority() {}
    Priority(int is, int ic) : In_Stack(is), In_Coming(ic) {}
  };

  /* operator set */
  map<char, Priority> Operators;
};

Calculator::Calculator() {
  Operators['$'] = Priority(0, 0);

  Operators['+'] = Priority(2, 2);
  Operators['-'] = Priority(2, 2);

  Operators['*'] = Priority(3, 3);
  Operators['/'] = Priority(3, 3);

  Operators['^'] = Priority(4, 4);

  Operators['('] = Priority(1, 9);
  Operators[')'] = Priority(1, 1);
}

bool Calculator::IsOperator(char ch) {
  if (Operators.find(ch) == Operators.end())
    return false;
  else
    return true;
}

double Calculator::Simple_Eval(double lopnd, char optr, double ropnd) {
  switch (optr) {
  case '+':
    return (lopnd + ropnd);
  case '-':
    return (lopnd - ropnd);
  case '*':
    return (lopnd * ropnd);
  case '/':
    return (lopnd / ropnd);
  case '^':
    return (pow(lopnd, ropnd));
  default:
    cerr << "invalid operator " << optr << endl;
    assert(0);
  }
}

int Calculator::Compare(char optr1, char optr2) {
  if (Operators[optr1].In_Stack < Operators[optr2].In_Coming)
    return -1;
  else
    return 1;
}

double Calculator::Eval(string &expr) {
  stack_double opnd_stack;
  stack_char optr_stack;
  char ch, optrtop;
  double operand, lopnd, ropnd;
  istringstream ExprStream;
  ExprStream.str(expr);

  optr_stack.push('$');

  while (1) {
    if (ExprStream >> ch, !IsOperator(ch)) {
      ExprStream.putback(ch);
      ExprStream >> operand;
      opnd_stack.push(operand);
    } else {
      while (optrtop = optr_stack.top(), Compare(optrtop, ch) == 1) {
        if (optrtop == '$' && ch == '$')
          goto EXIT;
        if (optrtop == '(' && ch == ')') {
          optr_stack.pop();
          ExprStream >> ch; /* ch must be an operator */

          assert(IsOperator(ch));
        } else {
          ropnd = opnd_stack.top();
          opnd_stack.pop();

          lopnd = opnd_stack.top();
          opnd_stack.pop();

          opnd_stack.push(Simple_Eval(lopnd, optrtop, ropnd));
          optr_stack.pop();
        }
      }

      optr_stack.push(ch);
    }
  }

EXIT:
  return opnd_stack.top();
}

/* Main Function */
int main(int argc, char *arg[]) {
  string expr("");
  for (int i = 1; i < argc; ++i) {
    expr += arg[i];
  }
  expr += "$";

  // expr =+ "$"; // no error!! why??

  Calculator calc;
  cout << "result = " << calc.Eval(expr) << endl;

  return 0;
}

/* EOF */
