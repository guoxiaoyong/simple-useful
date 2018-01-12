%{
    #include <stdio.h>
    #include <math.h>

    int yylex(void);
    void yyerror(char *);
%}

%token INTEGER

%%

prog: expr | expr prog;

expr: add_expr '\n' {printf("%d\n", $$); fflush(NULL);}
      ;

add_expr: mul_expr {$$ = $1;}
        | add_expr '+' add_expr   {$$=$1+$3;}
        | add_expr '-' add_expr   {$$=$1-$3;} 
        ;

mul_expr: exp_expr 
        | mul_expr '*' mul_expr {$$=$1*$3;}
        | mul_expr '/' mul_expr {$$=$1/$3;}
        ;

exp_expr: INTEGER
        | exp_expr '^' INTEGER {$$=pow($1,$3);printf("%d^%d\n",$1,$3);}
        | '(' add_expr ')'      {$$=$2;}
        ;

%%

void yyerror(char *s) {

    fprintf(stderr, "%s\n", s);
}

int main(void) {

    yyparse();
    return 0;
}

