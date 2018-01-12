#define LPAREN (
#define RPAREN )
#define COMMA ,

#define CAT(L, R) CAT_(L, R)
#define CAT_(L, R) L ## R
#define EXPAND(...) __VA_ARGS__

#define SPLIT(OP, D) EXPAND(OP CAT(SPLIT_, D) RPAREN)

#define SPLIT_int LPAREN int COMMA
#define SPLIT_char LPAREN char COMMA
#define SPLIT_float LPAREN float COMMA
#define SPLIT_double LPAREN double COMMA

#define MY_MACRO(A, B) SPLIT(MY_OTHER_MACRO, A); SPLIT(MY_OTHER_MACRO, B);

MY_MACRO(int x, char * z)

#define my_macro(a, b) fun(a); fun(b);
my_macro(int x, char* y)
