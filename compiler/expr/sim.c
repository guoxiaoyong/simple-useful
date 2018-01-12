#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ipow(int b, int e) {

  int n;
  int r = 1;

  if (e < 0) {

    return 0;
  }

  for (n = 0; n < e; n++) {

    r *= b;
  }

  return r;
}

/* program::
 *
 */

/* token category */
enum token_category {

  // arithmetic
  ADD = 1,
  MINUS,
  MUL,
  DIV,
  MOD,
  POW,

  // relational
  LT,
  LE,
  GT,
  GE,
  NE,
  EQ,

  // logical
  AND, // &&
  OR,  // ||
  NOT, // !

  ASSIGNMENT,

  LPAR,
  RPAR,
  LBRACE,
  RBRACE,

  SEMICOLON,

  NUM,
  VARIABLE, // single characters [ a-z A-Z ]
  WHILE,    // while
  IF,
  ELSE,
  ECHO,
};

enum statement_type {

  ST_ASSIGNMENT = 1,
  ST_WHILE,
  ST_IF,
  ST_BLOCK,
  ST_FUNCTION,
};

/* a structure holding scanned token */
struct token {

  enum token_category type;
  char str[8];
  int number;
};

enum token_category token_char[128] = {

        ['+'] = ADD,        ['-'] = MINUS, ['*'] = MUL,    ['/'] = DIV,
        ['%'] = MOD,        ['^'] = POW,   ['<'] = LT,     ['>'] = GT,

        ['!'] = NOT,

        ['='] = ASSIGNMENT,

        ['('] = LPAR,       [')'] = RPAR,  ['{'] = LBRACE, ['}'] = RBRACE,

        [';'] = SEMICOLON,

};

struct program {

  char src[4096];
  char *src_ptr;

  struct token tok[4096];
  int ti;
  int tok_len;

  // data[a] -- data[Z] stores variables
  int data[128];
};

struct program *program_init() {

  struct program *prog = calloc(sizeof(struct program), 1);

  prog->src_ptr = prog->src;
  prog->ti = 0;
  prog->tok_len = 0;

  int n;
  for (n = 0; n < 128; n++) {

    if (isupper(n) || islower(n)) {

      token_char[n] = VARIABLE;
    }
  }

  return prog;
}

void program_destroy(struct program *prog) { free(prog); }

void program_show_variables(struct program *prog) {

  int n;
  for (n = 0; n < 127; n++) {

    if (prog->data[n] != 0) {

      printf("%c = %d\n", (char)n, prog->data[n]);
    }
  }
}

struct token *program_get_curr_token(struct program *prog) {

  if (prog->ti >= prog->tok_len) {
    return NULL;
  }

  struct token *tok;
  tok = &(prog->tok[prog->ti]);
  prog->ti++;

  return tok;
}

struct token *program_get_curr_token_no_advance(struct program *prog) {

  if (prog->ti >= prog->tok_len) {
    return NULL;
  }

  struct token *tok;
  tok = &(prog->tok[prog->ti]);

  return tok;
}

struct token *program_get_next_token_no_advance(struct program *prog) {

  if (prog->ti >= prog->tok_len) {
    return NULL;
  }

  struct token *tok;
  tok = &(prog->tok[prog->ti + 1]);

  return tok;
}

int program_find_end_of_statement(struct program *prog, enum statement_type st,
                                  int *more) {

  int curr_ti = prog->ti;
  struct token *tok;
  int brace = 0;
  int end_ti = 0;

  switch (st) {

  case ST_ASSIGNMENT:
    while ((tok = program_get_curr_token(prog))) {

      if (tok->type == SEMICOLON) {

        end_ti = prog->ti;
        break;
      }
    }
    break;

  case ST_BLOCK:
  case ST_WHILE:
    while ((tok = program_get_curr_token(prog))) {

      if (tok->type == LBRACE)
        brace++;

      if (tok->type == RBRACE) {
        brace--;
        if (brace == 0) {
          end_ti = prog->ti;
          break;
        }
      }

      if (tok->type == SEMICOLON && brace == 0) {

        end_ti = prog->ti;
        break;
      }
    }
    break;

  case ST_IF:

    while ((tok = program_get_curr_token(prog))) {

      if (tok->type == LBRACE)
        brace++;

      if (tok->type == RBRACE) {
        brace--;
        if (brace == 0) {
          end_ti = prog->ti;
          more[0] = end_ti;
          break;
        }
      }

      if (tok->type == SEMICOLON && brace == 0) {

        end_ti = prog->ti;
        more[0] = end_ti;
        break;
      }
    }

    tok = program_get_curr_token(prog);
    if (tok->type == ELSE) {
      while ((tok = program_get_curr_token(prog))) {

        if (tok->type == LBRACE)
          brace++;

        if (tok->type == RBRACE) {
          brace--;
          if (brace == 0) {
            end_ti = prog->ti;
            more[1] = end_ti;
            break;
          }
        }

        if (tok->type == SEMICOLON && brace == 0) {

          end_ti = prog->ti;
          more[1] = end_ti;
          break;
        }
      }
    }
    break;

  default:
    fprintf(stderr, "statement_type is wrong %d\n", st);
  }

  prog->ti = curr_ti;

  if (end_ti == 0) {

    fprintf(stderr, "syntax error: end of statement not found!\n");
  }

  return end_ti;
}

/********************************************
 *
 * LEXER
 *
 *********************************************/

void get_number(struct program *prog) {

  char *endptr;
  int number;

  if (!isdigit(*(prog->src_ptr))) {

    return;
  }

  number = strtol(prog->src_ptr, &endptr, 0);

  if (endptr > prog->src_ptr) {

    prog->tok[prog->ti].type = NUM;
    prog->tok[prog->ti].number = number;
    sprintf(prog->tok[prog->ti].str, "%d", number);

    prog->src_ptr = endptr;
    prog->ti++;
  }
}

void get_keyword(struct program *prog) {

  if (strncmp(prog->src_ptr, "echo", 4) == 0 && isblank(prog->src_ptr[4])) {

    prog->tok[prog->ti].type = ECHO;
    strcpy(prog->tok[prog->ti].str, "echo");

    prog->src_ptr += 4;
    prog->ti++;
  }

  if (strncmp(prog->src_ptr, "while", 5) == 0 && isblank(prog->src_ptr[5])) {

    prog->tok[prog->ti].type = WHILE;
    strcpy(prog->tok[prog->ti].str, "while");

    prog->src_ptr += 5;
    prog->ti++;
  }

  if (strncmp(prog->src_ptr, "if", 2) == 0 && isblank(prog->src_ptr[2])) {

    prog->tok[prog->ti].type = IF;
    strcpy(prog->tok[prog->ti].str, "if");

    prog->src_ptr += 2;
    prog->ti++;
  }

  if (strncmp(prog->src_ptr, "else", 4) == 0 && isblank(prog->src_ptr[4])) {

    prog->tok[prog->ti].type = ELSE;
    strcpy(prog->tok[prog->ti].str, "else");

    prog->src_ptr += 4;
    prog->ti++;
  }
}

void get_double(struct program *prog) {

  if (strncmp(prog->src_ptr, "<=", 2) == 0) {

    prog->tok[prog->ti].type = LE;
    strcpy(prog->tok[prog->ti].str, "<=");
    prog->src_ptr += 2;
    prog->ti++;

  } else if (strncmp(prog->src_ptr, ">=", 2) == 0) {

    prog->tok[prog->ti].type = GE;
    strcpy(prog->tok[prog->ti].str, ">=");
    prog->src_ptr += 2;
    prog->ti++;

  } else if (strncmp(prog->src_ptr, "!=", 2) == 0) {

    prog->tok[prog->ti].type = EQ;
    strcpy(prog->tok[prog->ti].str, "!=");
    prog->src_ptr += 2;
    prog->ti++;

  } else if (strncmp(prog->src_ptr, "==", 2) == 0) {

    prog->tok[prog->ti].type = EQ;
    strcpy(prog->tok[prog->ti].str, "==");
    prog->src_ptr += 2;
    prog->ti++;
  } else if (strncmp(prog->src_ptr, "&&", 2) == 0) {

    prog->tok[prog->ti].type = AND;
    strcpy(prog->tok[prog->ti].str, "&&");
    prog->src_ptr += 2;
    prog->ti++;
  } else if (strncmp(prog->src_ptr, "||", 2) == 0) {

    prog->tok[prog->ti].type = OR;
    strcpy(prog->tok[prog->ti].str, "||");
    prog->src_ptr += 2;
    prog->ti++;
  }
}

void get_single(struct program *prog) {

  if (token_char[(int)*(prog->src_ptr)] != 0) {

    prog->tok[prog->ti].type = token_char[(int)*(prog->src_ptr)];
    prog->tok[prog->ti].str[0] = *(prog->src_ptr);
    prog->tok[prog->ti].str[1] = '\0';
    prog->src_ptr++;
    prog->ti++;
  }
}

void skipchar(struct program *prog) {

  char c = *(prog->src_ptr);

  while (c == '\r' || c == '\n' || c == ' ' || c == '\t') {

    prog->src_ptr++;
    c = *(prog->src_ptr);
  }
}

void lexer(struct program *prog) {

  while (*(prog->src_ptr) != '\0') {

    get_number(prog);
    get_keyword(prog);
    get_double(prog);
    get_single(prog);
    skipchar(prog);
  }

  prog->tok_len = prog->ti;
  prog->ti = 0;

  int i = 0;
  while (i < prog->tok_len) {

    printf("[%s]", prog->tok[i].str);
    i++;
  }
  printf("\n");

  printf("%d tokens were found!\n", prog->tok_len);
}

/*
 * Expression: Additive_Factor (+-) Additive_Factor (+-) Additive_Factor ...
 * Additive_Factor: Multiplicative_Factor (mul/div) Multiplicative_Factor
 * (mul/div) Multiplicative_Factor ...
 * Multiplicative_Factor: Power_Factor ^ Power_Factor ^ Power_Factor ...
 * Power_Factor: number | (+-)Power_Factor | (Expression)
 *
 * */

int program_eval_expression(struct program *prog);

int program_power_factor(struct program *prog) {

  int result;
  struct token *tok = program_get_curr_token(prog);

  switch (tok->type) {

  case NUM:
    result = tok->number;
    break;

  case VARIABLE:
    result = prog->data[(int)tok->str[0]];
    break;

  case LPAR:
    result = program_eval_expression(prog);

    tok = program_get_curr_token(prog);
    if (tok->type != RPAR) {

      fprintf(stderr, "A RPAR is expected!!\n");
      exit(1);
    }
    break;

  default:
    fprintf(stderr, "A number or +/- sign is expected!!\n");
    fprintf(stderr, "token type = %d\n", tok->type);
    exit(2);
  }

  return result;
}

int program_multiplicative_factor(struct program *prog) {

  int result = program_power_factor(prog);
  struct token *tok = program_get_curr_token_no_advance(prog);

  if (tok == NULL)
    return result;

  if (tok->type == SEMICOLON) {

    return result;
  }

  while (tok->type == POW) {

    prog->ti++;
    result = ipow(result, program_power_factor(prog));
    tok = program_get_curr_token_no_advance(prog);
  }

  return result;
}

int program_additive_factor(struct program *prog) {

  struct token *tok;

  tok = program_get_curr_token_no_advance(prog);

  if (tok->type == ADD) {

    prog->ti++;
    return program_additive_factor(prog);

  } else if (tok->type == MINUS) {

    prog->ti++;
    return -program_additive_factor(prog);
  }

  int result = program_multiplicative_factor(prog);
  tok = program_get_curr_token_no_advance(prog);

  if (tok == NULL)
    return result;

  if (tok->type == SEMICOLON) {

    return result;
  }

  while (tok->type == MUL || tok->type == DIV || tok->type == MOD) {

    prog->ti++;
    if (tok->type == MUL) {

      result *= program_multiplicative_factor(prog);

    } else if (tok->type == DIV) {

      result /= program_multiplicative_factor(prog);

    } else {

      result %= program_multiplicative_factor(prog);
    }

    tok = program_get_curr_token_no_advance(prog);
  }

  return result;
}

int program_eval_alg(struct program *prog) {

  int result = program_additive_factor(prog);
  struct token *tok = program_get_curr_token_no_advance(prog);

  if (tok == NULL)
    return result;

  if (tok->type == SEMICOLON) {

    return result;
  }

  while (tok->type == ADD || tok->type == MINUS) {

    prog->ti++;
    if (tok->type == ADD) {

      result += program_additive_factor(prog);

    } else {

      result -= program_additive_factor(prog);
    }

    tok = program_get_curr_token_no_advance(prog);
  }

  return result;
}

int program_eval_relational(struct program *prog) {

  int result = program_eval_alg(prog);
  struct token *tok = program_get_curr_token_no_advance(prog);

  if (tok == NULL)
    return result;

  if (tok->type == SEMICOLON) {

    return result;
  }

  while (tok->type >= LT && tok->type <= EQ) {

    prog->ti++;
    switch (tok->type) {

    case LT:
      result = (result < program_eval_alg(prog));
      break;

    case LE:
      result = (result <= program_eval_alg(prog));
      break;

    case GT:
      result = (result > program_eval_alg(prog));
      break;

    case GE:
      result = (result >= program_eval_alg(prog));
      break;

    case NE:
      result = (result != program_eval_alg(prog));
      break;

    case EQ:
      result = (result == program_eval_alg(prog));
      break;

    default:
      fprintf(stderr, "wrong type %d\n", tok->type);
      exit(0);
    }

    tok = program_get_curr_token_no_advance(prog);
  }

  // printf("last = %s\n", tok->str);
  return result;
}

int program_eval_logic(struct program *prog) {

  struct token *tok;
  tok = program_get_curr_token_no_advance(prog);

  if (tok->type == NOT) {

    prog->ti++;
    return !program_eval_logic(prog);
  }

  int result = program_eval_relational(prog);
  tok = program_get_curr_token_no_advance(prog);

  if (tok == NULL)
    return result;

  if (tok->type == SEMICOLON) {

    return result;
  }

  while (tok->type >= AND && tok->type <= NOT) {

    prog->ti++;
    switch (tok->type) {

    case AND:
      result = (result && program_eval_relational(prog));
      break;

    case OR:
      result = (result || program_eval_relational(prog));
      break;

    case NOT:
      result = (result > program_eval_alg(prog));
      break;

    case GE:
      result = (result >= program_eval_alg(prog));
      break;

    case NE:
      result = (result != program_eval_alg(prog));
      break;

    case EQ:
      result = (result == program_eval_alg(prog));
      break;

    default:
      fprintf(stderr, "wrong type %d\n", tok->type);
    }
  }

  return result;
}

int program_eval_expression(struct program *prog) {

  return program_eval_logic(prog);
}

int program_eval_assignment(struct program *prog) {

  struct token *tok = program_get_curr_token(prog);
  int *r;

  assert(tok != NULL && tok->type == VARIABLE);
  r = &(prog->data[(int)tok->str[0]]);

  tok = program_get_curr_token(prog);
  assert(tok->type == ASSIGNMENT);

  *r = program_eval_expression(prog);

  return *r;
}

int program_eval_statement(struct program *prog);

int program_eval_block(struct program *prog) {

  struct token *tok = program_get_curr_token(prog);
  assert(tok->type == LBRACE);

  tok = program_get_curr_token_no_advance(prog);
  while (tok != NULL && tok->type != RBRACE) {

    program_eval_statement(prog);
    tok = program_get_curr_token_no_advance(prog);
  }

  return 0;
}

int program_eval_while(struct program *prog) {

  struct token *tok = program_get_curr_token(prog);
  assert(tok->type == WHILE);

  tok = program_get_curr_token_no_advance(prog);

  if (tok->type != LPAR) {
    fprintf(stderr, "a parenthesis is expected after while keyword!\n");
    exit(1);
  }

  /*not good*/
  int curr_ti = prog->ti;
  int next_ti = program_find_end_of_statement(prog, ST_WHILE, NULL);
  while (program_eval_expression(prog)) {

    program_eval_statement(prog);
    prog->ti = curr_ti;
  }

  prog->ti = next_ti;

  return 0;
}

int program_eval_if(struct program *prog) {

  struct token *tok = program_get_curr_token(prog);
  assert(tok->type == IF);

  tok = program_get_curr_token_no_advance(prog);

  if (tok->type != LPAR) {
    fprintf(stderr, "a parenthesis is expected after while keyword!\n");
    exit(1);
  }

  int more[2] = {-1, -1};
  int next_ti = program_find_end_of_statement(prog, ST_IF, more);

  if (more[1] == -1) {

    if (program_eval_expression(prog)) {

      program_eval_statement(prog);
    }

  } else {

    if (program_eval_expression(prog)) {

      program_eval_statement(prog);

    } else {

      prog->ti = more[0] + 1;
      program_eval_statement(prog);
    }
  }

  prog->ti = next_ti;

  return 0;
}

int program_eval_echo(struct program *prog) {

  struct token *tok = program_get_curr_token(prog);
  assert(tok->type == ECHO);

  struct token *tok_next;

  tok = program_get_curr_token_no_advance(prog);
  tok_next = program_get_next_token_no_advance(prog);
  if (tok->type == VARIABLE && tok_next->type == SEMICOLON) {

    printf("%c = %d\n", tok->str[0], program_eval_expression(prog));

  } else {

    printf("expression = %d\n", program_eval_expression(prog));
  }

  return 0;
}

int program_eval_statement(struct program *prog) {

  struct token *tok = program_get_curr_token_no_advance(prog);
  struct token *tok_next;
  if (tok == NULL)
    return 0;

  switch (tok->type) {

  case WHILE:
    program_eval_while(prog);
    break;

  case IF:
    program_eval_if(prog);
    break;

  case ECHO:
    program_eval_echo(prog);
    break;

  case VARIABLE:
    tok_next = program_get_next_token_no_advance(prog);
    if (tok_next->type == ASSIGNMENT) {
      program_eval_assignment(prog);
    } else {
      fprintf(stderr, "assignemnt sign is expected!\n");
    }
    break;

  case LBRACE:
    program_eval_block(prog);
    break;

  case SEMICOLON:
  case RBRACE:
    prog->ti++;
    break;

  default:
    fprintf(stderr, "token type %d is not expected!\n", tok->type);
    exit(0);
  }

  return 1;
}

int program_run(struct program *prog) {

  prog->ti = 0;
  while (prog->ti < prog->tok_len) {

    program_eval_statement(prog);
  }

  return 1;
}

int main(int argc, char *argv[]) {

  assert(argc == 2);
  FILE *fp = fopen(argv[1], "rb");

  if (fp == NULL)
    return 1;

  struct program *prog = program_init();

  int c;
  int i = 0;
  while ((c = fgetc(fp)) != EOF) {

    prog->src[i] = (char)c;
    i++;
  }
  prog->src[i] = 0;
  fclose(fp);

  lexer(prog);
  program_run(prog);

  // program_show_variables(prog);

  program_destroy(prog);
  return EXIT_SUCCESS;
}
