#include <boost/regex.hpp>
#include <map>
#include <string>
#include <vector>

char *WordPattern[] = {
    "case", "continue", "default", "do", "else", "for", "goto", "if", "return",
    "switch", "while", "",
    //////////////////////////////////////////////////////////////////////////////////////////////////
    "NULL", "false", "break", "true", "enum", "errno", "EDOM", "ERANGE",
    "FLT_RADIX", "FLT_ROUNDS", "FLT_DIG", "DBL_DIG", "LDBL_DIG", "FLT_EPSILON",
    "DBL_EPSILON", "LDBL_EPSILON", "FLT_MANT_DIG", "DBL_MANT_DIG",
    "LDBL_MANT_DIG", "FLT_MAX", "DBL_MAX", "LDBL_MAX", "FLT_MAX_EXP",
    "DBL_MAX_EXP", "LDBL_MAX_EXP", "FLT_MIN", "DBL_MIN", "LDBL_MIN",
    "FLT_MIN_EXP", "DBL_MIN_EXP", "LDBL_MIN_EXP", "CHAR_BIT", "CHAR_MAX",
    "CHAR_MIN", "SCHAR_MAX", "SCHAR_MIN", "UCHAR_MAX", "SHRT_MAX", "SHRT_MIN",
    "USHRT_MAX", "INT_MAX", "INT_MIN", "UINT_MAX", "LONG_MAX", "LONG_MIN",
    "ULONG_MAX", "HUGE_VAL", "SIGABRT", "SIGFPE", "SIGILL", "SIGINT", "SIGSEGV",
    "SIGTERM", "SIG_DFL", "SIG_ERR", "SIG_IGN", "BUFSIZ", "EOF", "FILENAME_MAX",
    "FOPEN_MAX", "L_tmpnam", "NULL", "SEEK_CUR", "SEEK_END", "SEEK_SET",
    "TMP_MAX", "stdin", "stdout", "stderr", "EXIT_FAILURE", "EXIT_SUCCESS",
    "RAND_MAX", "CLOCKS_PER_SEC", "virtual", "public", "private", "protected",
    "template", "using", "namespace", "try", "catch", "inline", "dynamic_cast",
    "const_cast", "reinterpret_cast", "static_cast", "explicit", "friend",
    "wchar_t", "typename", "typeid", "class", "",
    //////////////////////////////////////////////////////////////////////////////////////////////////
    "cin", "cerr", "clog", "cout", "delete", "new", "this", "printf", "fprintf",
    "snprintf", "sprintf", "assert", "isalnum", "isalpha", "isdigit", "iscntrl",
    "isgraph", "islower", "isprint", "ispunct", "isspace", "ispunct", "isupper",
    "isxdigit", "tolower", "toupper", "exp", "log", "log10", "pow", "sqrt",
    "ceil", "floor", "fabs", "ldexp", "frexp", "modf", "fmod", "sin", "cos",
    "tan", "asin", "acos", "atan", "atan2", "sinh", "cosh", "tanh", "setjmp",
    "longjmp", "asin", "acos", "atan", "atan2", "va_start", "va_arg", "va_end",
    "offsetof", "sizeof", "fopen", "freopen", "fflush", "fclose", "remove",
    "rename", "tmpfile", "tmpname", "setvbuf", "setbuf", "vfprintf", "vprintf",
    "vsprintf", "fscanf", "scanf", "sscanf", "fgetc", "fgets", "fputc", "fputs",
    "getc", "getchar", "gets", "putc", "putchar", "puts", "ungetc", "fread",
    "fwrite", "fseek", "ftell", "rewind", "fgetpos", "fsetpos", "clearerr",
    "feof", "ferror", "perror", "abs", "labs", "div", "ldiv", "atof", "atoi",
    "atol", "strtod", "strtol", "strtoul", "calloc", "malloc", "realloc",
    "free", "abort", "exit", "atexit", "system", "getenv", "bsearch", "qsort",
    "rand", "srand", "strcpy", "strncpy", "strcat", "strncat", "strcmp",
    "strncmp", "strcoll", "strchr", "strrchr", "strspn", "strcspn", "strpbrk",
    "strstr", "strlen", "strerror", "strtok", "strxfrm", "memcpy", "memmove",
    "memcmp", "memchr", "memset", "clock", "time", "difftime", "mktime",
    "asctime", "ctime", "gmtime", "localtime", "strftime",
    ""
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    "auto",
    "bool", "char", "const", "double", "float", "int", "long", "longint",
    "register", "short", "shortint", "signed", "static", "struct", "typedef",
    "union", "unsigned", "void", "volatile", "extern", "jmp_buf", "signal",
    "raise", "va_list", "ptrdiff_t", "size_t", "FILE", "fpos_t", "div_t",
    "ldiv_t", "clock_t", "time_t", "tm",
    ""
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    "(",
    ")", "{", "}", "[", "]", "=", "+", "-", "*", "/", "!", "%", "^", "&", ":",
    ""
    "\\\\",
    "#", "", "/*[^.]*/", "'", ""
                              "",
    "", ".", "::", "\\"};

char *color[] = {"#0000ff", "#0000ff", "#0000dd", "#0000ff", "#ff0000",
                 "#339900"
                 "#ff0000",
                 "#666666", "#000000", "#666666", "#0000dd", "#00eeff",
                 "#00eeff", "#000000"};

//! StrArray: string array
typedef std::vector<std::string> StrArray;

//! associate container
typedef std::map<std::string, StrArray> LangConf;

//! Patterns
typedef std::vector<boost::regex> PatGrp;
typedef std::vector<PatGrp> PatGrpCls;

/**
 * LangStyle:
 */
class LangStyle {
private:
  //! the name of the language
  std::string LANG_NAME;

  //! number of patterns
  int nPat;

  //! number of pattern classes
  int nPatCls;

  //! PatGrpCls
  PatGrpCls PGCls;

  //! Colors
  std::string Colors;

public:
  LangStyle() : nPat(0), nPatCls(0) {}
  virtual ~LangStyle() {}

  virtual void SetLangName(std::string &name) { LANG_NAME = name; }

  virtual void SetPatterns() = 0;
};

class CPPStyle : public LangStyle {
public:
  CPPStyle() {}
};
}
