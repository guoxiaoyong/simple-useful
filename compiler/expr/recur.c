#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_TOKENS_IN_SOURCE 1024
#define MAX_NUM_CHARS_IN_SOURCE 2048

/* token category */
enum TokenCat {
  END,

  ADDITIVE_OPER,
  MULTIPLICATIVE_OPER,
  POWER_OPER,

  NUM,

  LPAR,
  RPAR,
};

/* a structure holding scanned token */
struct Token {
  enum TokenCat type;
  union {
    char oper;
    char par;
    double number;
  } value;
};

typedef struct Token Token;

/********************************************
 *
 * LEXER
 *
 *********************************************/

void GetNum(char **CurChar, Token **CurToken) {
  char *endptr;
  double number;

  /*FIXME: numbers like .5 is not accepted!! */
  if (!isdigit(*(*CurChar))) {
    return;
  }

  number = strtod(*CurChar, &endptr);

  if (endptr > *CurChar) {
    *CurChar = endptr;
    (*CurToken)->type = NUM;
    (*CurToken)->value.number = number;
    (*CurToken)++;
  }
}

void GetOper(char **CurChar, Token **CurToken) {

  switch (*(*CurChar)) {
  case '+':
    (*CurToken)->type = ADDITIVE_OPER;
    (*CurToken)->value.oper = '+';
    (*CurChar)++;
    (*CurToken)++;
    break;
  case '-':
    (*CurToken)->type = ADDITIVE_OPER;
    (*CurToken)->value.oper = '-';
    (*CurChar)++;
    (*CurToken)++;
    break;
  case '*':
    (*CurToken)->type = MULTIPLICATIVE_OPER;
    (*CurToken)->value.oper = '*';
    (*CurChar)++;
    (*CurToken)++;
    break;
  case '/':
    (*CurToken)->type = MULTIPLICATIVE_OPER;
    (*CurToken)->value.oper = '/';
    (*CurChar)++;
    (*CurToken)++;
    break;
  case '^':
    (*CurToken)->type = POWER_OPER;
    (*CurToken)->value.oper = '^';
    (*CurChar)++;
    (*CurToken)++;
    break;
  }
}

void GetPar(char **CurChar, Token **CurToken) {
  switch (*(*CurChar)) {
  case '(':
    (*CurToken)->type = LPAR;
    (*CurToken)->value.par = '(';
    (*CurChar)++;
    (*CurToken)++;
    break;
  case ')':
    (*CurToken)->type = RPAR;
    (*CurToken)->value.par = ')';
    (*CurChar)++;
    (*CurToken)++;
    break;
  }
}

void SkipChar(char **CurChar) {
  if (*(*CurChar) == ' ' || *(*CurChar) == '\t') {
    CurChar++;
  }
}

void Lexer(char *CharSource, Token *TokSource) {
  char *CurChar = CharSource;
  Token *CurToken = TokSource;

  while (*CurChar != '\0') {
    GetNum(&CurChar, &CurToken);
    GetOper(&CurChar, &CurToken);
    GetPar(&CurChar, &CurToken);
    SkipChar(&CurChar);
  }
  CurToken->type = END;

  size_t i = 0;
  while (TokSource[i].type != END) {
    switch (TokSource[i].type) {
    /* case OPER */
    case ADDITIVE_OPER:;
    case MULTIPLICATIVE_OPER:;
    case POWER_OPER:
      printf("%c", TokSource[i].value.oper);
      break;

    /* case PAR */
    case LPAR:
    case RPAR:
      printf("%c", TokSource[i].value.par);
      break;

    /* case NUM */
    case NUM:
      printf("[%f]", TokSource[i].value.number);
      break;
    case END:
      break;
    default:
      fprintf(stderr, "unexpected token %d\n", TokSource[i].type);
    }
    i++;
  }

  printf("\n");
}

/*
 * Expression: Additive_Factor (+-) Additive_Factor (+-) Additive_Factor ...
 * Additive_Factor: Multiplicative_Factor (mul/div) Multiplicative_Factor
 * (mul/div) Multiplicative_Factor ...
 * Multiplicative_Factor: Power_Factor ^ Power_Factor ^ Power_Factor ...
 * Power_Factor: number | (+-)Power_Factor | (Expression)
 *
 * */

double Eval_Expression(Token **CurToken);

double Power_Factor(Token **CurToken) {
  double result;

  if ((*CurToken)->type == NUM) {
    result = (*CurToken)->value.number;
    (*CurToken)++;
  } else if ((*CurToken)->type == ADDITIVE_OPER) {
    switch ((*CurToken)->value.oper) {
    case '+':
      (*CurToken)++;
      result = Power_Factor(CurToken);
      break;
    case '-':
      (*CurToken)++;
      result = -Power_Factor(CurToken);
      break;
    }
  } else if ((*CurToken)->type == LPAR) {
    (*CurToken)++;
    result = Eval_Expression(CurToken);

    if ((*CurToken)->type != RPAR) {
      fprintf(stderr, "A RPAR is expected!!\n");
      abort();
    }
    (*CurToken)++;
  } else {
    fprintf(stderr, "A number or +/- sign is expected!!\n");
    abort();
  }

  return result;
}

double Multiplicative_Factor(Token **CurToken) {
  double result = Power_Factor(CurToken);

  while ((*CurToken)->type == POWER_OPER) {
    if ((*CurToken)->value.oper == '^') {
      (*CurToken)++;
      result = pow(result, Power_Factor(CurToken));
    } else {
      fprintf(stderr, " ^ Operator is expected!!\n");
      abort();
    }
  }

  return result;
}

double Additive_Factor(Token **CurToken) {
  double result = Multiplicative_Factor(CurToken);

  while ((*CurToken)->type == MULTIPLICATIVE_OPER) {
    if ((*CurToken)->value.oper == '*') {
      (*CurToken)++;
      result *= Multiplicative_Factor(CurToken);
    } else if ((*CurToken)->value.oper == '/') {
      (*CurToken)++;
      result /= Multiplicative_Factor(CurToken);
    } else {
      fprintf(stderr, " mul/div Operator is expected!!\n");
      abort();
    }
  }

  return result;
}

double Eval_Expression(Token **CurToken) {
  double result = Additive_Factor(CurToken);

  while ((*CurToken)->type == ADDITIVE_OPER) {
    if ((*CurToken)->value.oper == '+') {
      (*CurToken)++;
      result += Additive_Factor(CurToken);
    } else if ((*CurToken)->value.oper == '-') {
      (*CurToken)++;
      result -= Additive_Factor(CurToken);
    } else {
      fprintf(stderr, "+/- Operator is expected!!\n");
      abort();
    }
  }

  return result;
}

/* MAIN FUNCTION */
int main(int argc, char *argv[]) {
  char CharSource[MAX_NUM_CHARS_IN_SOURCE] = "";
  Token TokSource[MAX_NUM_TOKENS_IN_SOURCE];

  int i;

  if (argc > 1) {
    for (i = 1; i < argc; ++i) {
      strcat(CharSource, argv[i]);
    }
  } else {
    printf(">>");
    gets(CharSource);
    printf("the input expression is  %s\n", CharSource);
  }

  Lexer(CharSource, TokSource);

  Token *TokBegin = TokSource;
  printf("%f\n", Eval_Expression(&TokBegin));

  return EXIT_SUCCESS;
}
