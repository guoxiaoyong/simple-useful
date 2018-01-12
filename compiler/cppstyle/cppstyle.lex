/**/
D [0-9]
L [a-zA-Z_.]

%{
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define OUTCPP    0
#define INLINECPP 1
#define BLOCKCPP  2


    void keyword();
    void macro();
    void comment();
    void blockcomment();
    void specialchar();
    void blockstart();
    void inlinestart();
    void outofcode();

    char flag = OUTCPP;
%}


%%
^"#"       { macro(); }
"//"[^\n]* { comment(); }
"/*"       { blockcomment(); }

"do"|"break"|"case"|"continue"|"default"|"else"|"for"|"goto"|"if"|"while"|"switch" { keyword(); }

"const"|"static"|"auto"|"register"|"restrict"|"extern"|"volatile"|"mutable"|"virtual"|"inline" { keyword(); }

"_Complex"|"_Imaginary"|"_Bool" { keyword(); }

"signed"|"unsigned"|"char"|"wchar_t"|"double"|"float"|"int"|"long"|"short"|"void" { keyword(); }

"struct"|"class"|"union"|"enum"|"typedef" { keyword(); }

"static_cast"|"const_cast"|"dynamic_cast"|"reinterpret_cast" { keyword(); }

"try"|"throw"|"catch" { keyword(); }

"new"|"delete" { keyword(); }

"using"|"namespace" { keyword(); }

"private"|"public"|"protected" { keyword(); }

"this"|"sizeof"|"operator"|"template"|"export"|"friend"|"return"|"asm"|"typename" { keyword(); }

"<"|">"|"&" { specialchar(); }

"<code"[ \t]+"style"[ \t]+"="+[ \t]+"\"cpp\">"  { blockstart(); }

"<code"[ \t]+"style"[ \t]+"="+[ \t]+"\"inlinecpp\">"  { inlinestart(); }

"</code>"  { outofcode(); }

{L}({L}|{D})* { ECHO; }

%%
/**/

void Code2HTML(char* code_string, char* html_style)
{
    fprintf(yyout, "<span style=\"%s\">%s</span>", html_style, code_string);
}

void blockstart()
{
    flag = BLOCKCPP ;
    fprintf(yyout, "<pre class=\"code\">");
}


void inlinestart()
{
    flag = INLINECPP;
    fprintf(yyout, "<span class=\"code\">");
}

void outofcode()
{
    if ( BLOCKCPP == flag)
    {
        fprintf(yyout, "</pre>\n");
        flag = OUTCPP;
    } 
    else if (INLINECPP == flag)
    {
        fprintf(yyout, "</span>");
        flag = OUTCPP;
    }
    else
    {
        fprintf(stderr, "ERROR: the value of flag is wrong!\n");
        fprintf(stderr, "LINE: %d\n", yylineno);
        abort();
    }
}

void SpecialChar2HTML(char* textin, char* textout)
{
    size_t textin_len = strlen(textin);

    while ( *textin != '\0')
    {
        switch ( *textin )
        {
        case '<':
            strcpy(textout, "&lt;");
            textout += 4;
            break;
        case '>':
            strcpy(textout, "&gt;");
            textout += 4;
            break;
        case '&':
            strcpy(textout, "&amp;");
            textout += 5;
            break;
        default:
            *textout = *textin;
            textout++;
            break;
        }
    textin++;
    }

    *textout = '\0';
}


/*******************************/
void specialchar()
{
    if (flag)
    {
        switch ( yytext[0] )
        {
        case '<':
            fprintf(yyout, "&lt;");
            break;
        case '>':
            fprintf(yyout, "&gt;");
            break;
        case '&':
            fprintf(yyout, "&amp;");
            break;
        }
    }
    else
    {
        ECHO;
    }
}

void keyword()
{
    char textout[32768];
    if (flag)
    {
        SpecialChar2HTML(yytext, textout);
        Code2HTML(textout, "color:#662211; font-weight:bold");
    }
    else
    {
        ECHO;
    }
}

void comment()
{
    char textout[32768];
    if (flag)
    {
        SpecialChar2HTML(yytext, textout);
        Code2HTML(textout, "color:#2244FF;");
    }
    else
    {
        ECHO;
    }
}

void macro()
{
    char  comstr[32768] = "#";
    char  textout[32768];
    char* currptr = comstr+1;

    if (flag)
    {
        while ( (*currptr = input()) != EOF )
        {
            if ( currptr[0] == '\n' && currptr[-1] != '\\' )
            {
                currptr[1] = '\0';
                SpecialChar2HTML(comstr, textout);
                Code2HTML(textout, "color:#7777AA;");
                return;
            }
            else
            {
                currptr++;
            }
        }

        fprintf(stderr, "unterminated multi-line macro!!\n");
    }
    else
    {
        ECHO;
    }
}

void blockcomment()
{
    char  comstr[32768];
    char  textout[32768];
    char* currptr = comstr+2;

    comstr[0] = '/';
    comstr[1] = '*';

    if (flag)
    {
        while ( (*currptr = input()) != EOF )
        {
            if ( currptr[0] == '/' && currptr[-1] == '*' )
            {
                currptr[1] = '\0';
                SpecialChar2HTML(comstr, textout);
                Code2HTML(textout, "color:#2244FF;");
                return;
            }
            else
            {
                currptr++;
            }
        }

        fprintf(stderr, "unterminated comment block!!\n");
    }
    else
    {
        ECHO;
    }
}


/* Main Function */
int main(int argc, char* argv[])
{
    /* check parameters */
    if ( argc >= 2 )
    {
        yyin = fopen( argv[1], "r");
        if ( NULL == yyin )
        {
            fprintf(stderr, "ERROR: cannot open file %s!!\n", argv[1]);
            abort();
        }

        if ( argc >= 3 )
        {
            yyout = fopen( argv[2], "w");
            if ( NULL == yyout )
            {
                fprintf(stderr, "ERROR: cannot open file %s!!\n", argv[2]);
                abort();
            }
        }
        else
        {
            yyout = stdout;
        }
    }
    else
    {
        yyin  = stdin;
        yyout = stdout;
    }

    yylex();

    return EXIT_SUCCESS;
}



