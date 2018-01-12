
////////////////////////////////////////////////////////////////////////////
/*
        This file is a part of an interpreter for Tao, a high level
        object-oriented computing and scripting language.

        Copyright (C) 2004-2005, Fu Limin.
        Contact: fu.limin.tao@gmail.com, limin.fu@ircc.it

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/
////////////////////////////////////////////////////////////////////////////
#include <cctype>
#include <stack>

#include "taoLexer.h"
#include "taoUtility.h"

TaoKeyLexStroke TaoLexer::lexKey;

TaoKeyLexStroke::TaoKeyLexStroke() {
  int i;

  const char *inter[] = {"<*", "</", "<%", "<&", "<|"};
  for (i = 0; i < 5; i++)
    intmOpers[inter[i]] = 1;

  const char *aroper[] = {"++", "--",  "+=",  "-=",  "*=",  "/=",  "%=", "^=",
                          "&=", "|=",  "==",  "!=",  "&&",  "||",  "<=", ">=",
                          "=~", "!~",  "~~",  "::",  ".?",  ".@",  "?%", ".#",
                          ":=", "<*>", "</>", "<%>", "<&>", "<|>", "=?", "!?"};
  for (i = 0; i < 32; i++)
    compOpers[aroper[i]] = 1;

  const char *keyll[] = {"if", "else", "while", "foreach", "for"};
  for (i = 0; i < 5; i++)
    keyLogLoop[keyll[i]] = 1;

  allOpers = "+-*/%^=<>!&|.~'()[]{}:,;?@#$\\ \t\n";
}
TaoLexer::TaoLexer() {}
bool TaoLexer::parseCode(string &codeSource, bool replace) {
  map<string, bool> &compOpers = lexKey.compOpers;
  map<string, bool> &keyLogLoop = lexKey.keyLogLoop;
  map<string, bool> &intmOpers = lexKey.intmOpers;

  string &allOpers = lexKey.allOpers;

  string::iterator it = codeSource.begin();
  string::iterator itmp;

  bool isString = 0;

  // For regex and nested regex:
  int isRegular = 0;
  stack<int> nslash, nbrack;

  // Context where normal arithmetical expression can appear:
  // Such as:
  // 1. outside of regular expressions:
  // 2. or a part of regex enclosed with @{...}
  bool isNormalContext = 1;

  bool isLineComment = 0;

  int isBlockComment = 0;

  int nline = 1;

  // bool	quoteInComment=0;

  bool tokenAdded = 0;

  int posClass, posRoutine, posLL;
  posClass = posRoutine = posLL = -1;

  string literal, opers;

  it = codeSource.begin();
  while (it != codeSource.end()) {

    tokenAdded = 0;

    // Skip DOS symbol ^M(13):
    if (*it == 13) {
      it++;
      continue;
    }
    //=================================================
    //	Skip comments first:
    //=================================================
    //	For comment started with "#":
    if (*(it - 1) != '.' && *it == '#' && !isString) {
      isLineComment = 1;
      it++;
      continue;
    }
    // It is line beginning:
    if (!isString && (it == codeSource.begin() || *(it - 1) == '\n')) {
      // Starting block comment with <<<
      if (*it == '<' && *(it + 1) == '<' && *(it + 2) == '<') {
        isBlockComment++;
        while (*it != '\n')
          it++;
        nline++;
        it++;
        continue;
      }
      // Endinging block comment with >>>
      if (*it == '>' && *(it + 1) == '>' && *(it + 2) == '>') {
        isBlockComment--;
        while (*it != '\n')
          it++;
        nline++;
        it++;
        continue;
      }
    }
    //	Single line comment ended at the end of a line:
    if (*it == '\n') {
      nline++;
      isLineComment = 0;
    }
    //	Skip commented codes:
    if (isLineComment || isBlockComment > 0) {
      it++;
      continue;
    } else if (isBlockComment < 0) {
      error_unpaired(srcFName, nline + 1, "comment symbols <<< >>>");
    }

    //=================================================
    //	Now start to handle regular expression tokens:
    //=================================================

    // If previous literal is a regex operator: =~, !~, ~~
    // string =~ /\d/;
    // string =~ s/\d/w/;
    string last;
    if (opers.size() > 0) {
      last = opers;
    } else if (literal.size() > 0) {
      last = literal;
      if (last == "s")
        if (codeParsed.size() > 0)
          last = codeParsed[codeParsed.size() - 1] + last;
    } else {
      if (codeParsed.size() > 0)
        last = codeParsed[codeParsed.size() - 1];
      if (last == "s")
        if (codeParsed.size() > 1)
          last = codeParsed[codeParsed.size() - 2] + last;
    }
    if ((last == "=~" || last == "!~" || last == "~~" || last == "=~s" ||
         last == "!~s")) {
      if (*it == '/') {
        isRegular++;
        nslash.push(0);
        nbrack.push(0);
      }
    }
    // If it's regular expression:
    if (isRegular > 0) {
      // Check parenthesis in regular expression:
      if (*it == '{' && (*(it - 1)) != '\\') {
        nbrack.top()++;
      } else if (*it == '}' && (*(it - 1)) != '\\') {
        nbrack.top()--;
      }

      if (nbrack.top() == 0) {
        // :/abc@{4/2}/
        // :s/abc@{4/2}/m/
        if (*it == '/' && (*(it - 1)) != '\\')
          nslash.top()++;

        if (nslash.top() == 2) {
          // Regular expression ended:
          isRegular--;
          nslash.pop();
          nbrack.pop();
          if (opers.size() > 0) {
            codeParsed.push_back(opers);
            posLine.push_back(nline);
            opers.erase();
          } else if (literal.size() > 0) {
            codeParsed.push_back(literal);
            posLine.push_back(nline);
            literal.erase();
          }
          codeParsed.push_back("/");
          posLine.push_back(nline);

          isNormalContext = 1;

          it++;
          continue;
        } else {
          isNormalContext = 0;
        }
      } else {
        // Inside @{...} of a regex
        isNormalContext = 1;
      }
    }

    //=============================================
    //	For strings quotated with " :
    //=============================================
    if (*it == '"' && isNormalContext) {
      //	For different escapes:
      if (*(it - 1) != '\\') {
        isString = !isString;
        if (isString) {
          // When string starts:
          if (literal.length() > 0) {
            error_general(srcFName, nline + 1, "invalid string literal");
          }
        } else {
          // When string ends:
          if (replace)
            replaceEscape(literal);
        }
      }
    }
    if (isString) {
      literal += *it;
      it++;
      continue;
    }

    // For decimals:
    if (*it == '.' && isNormalContext) {
      if ((literal.size() > 0 && isDigits(literal)) ||
          isdigit(*(it + 1))) { // guo
        // Extending it to the right side of '.':
        literal += *it;
        it++;
        while (isdigit(*it)) {
          literal += *it;
          it++;
        }
        // Modifying decimal such as ".5" to "0.5":
        if (literal.length() > 1 && literal[0] == '.')
          literal.insert(literal.begin(), '0');
      }
    }
    //======================================================
    //	When lexer meets special chars:
    //======================================================
    if (allOpers.find(*it) != string::npos) {
      // If a literal has been found in the previous step,
      // push it to "codeParsed":
      if (literal.size() > 0) {
        if (!isValidLiteral(literal))
          warning(srcFName, nline + 1, "invalid literal " + literal);
        codeParsed.push_back(literal);
        posLine.push_back(nline);
        tokenAdded = 1;

        // Store position of certain keywords,
        // for primitive syntax checking:
        if (literal == "class")
          posClass = codeParsed.size() - 1;
        else if (literal == "routine")
          posRoutine = codeParsed.size() - 1;
        else if (keyLogLoop.find(literal) != keyLogLoop.end())
          posLL = codeParsed.size() - 1;

        literal.erase();
      }
      if (opers.size() > 0) {
        // If previous token is a operators:
        bool isCombiOper = (compOpers.find(opers + *it) != compOpers.end());
        bool isInterOper = (intmOpers.find(opers + *it) != intmOpers.end());
        if (isNormalContext && (isCombiOper || isInterOper)) {
          // If "opers+*it" is valid, extend it:
          opers += *it;
        } else {
          if (intmOpers.find(opers) != intmOpers.end())
            error_undefined(srcFName, nline, "operator " + opers);
          // Otherwise, push "opers":
          codeParsed.push_back(opers);
          posLine.push_back(nline);
          tokenAdded = 1;
          opers.erase();
          // If "*it" is not whitespace, accept it as a new token:
          if (*it != ' ' && *it != '\t' && *it != '\n')
            opers = *it;
        }
      } else if (*it != ' ' && *it != '\t' && *it != '\n') {
        // If "*it" is not whitespace, accept it as a new token:
        opers = *it;
      }
      it++;
      if (opers == "@") {
        // For transient variables: @1, @2 ...
        while (isdigit(*it)) {
          opers += *it;
          it++;
        }
      } else if (opers == "\\") {
        // For escape chars in regex:
        opers += *it;
        it++;
      }
    } else {
      int size = codeParsed.size() - 1;
      if (size > 0) {
        size_t len = codeParsed[size].size();
        // For 1e-5, 1E+10 type
        if (isdigit(codeParsed[size][0]) &&
            (codeParsed[size][len - 1] == 'e' ||
             codeParsed[size][len - 1] == 'E')) {
          if (opers == "" || opers == "+" || opers == "-") {
            codeParsed[size] += opers;
            opers.erase();
            while (isdigit(*it)) {
              codeParsed[size] += *it;
              it++;
            }
            continue;
          }
        }
      }
      if (opers.size() > 0) {
        if (intmOpers.find(opers) != intmOpers.end())
          error_undefined(srcFName, nline, "operator " + opers);

        codeParsed.push_back(opers);
        posLine.push_back(nline);
        opers.erase();
        tokenAdded = 1;
      }

      literal += *it;
      it++;
    }

    if (tokenAdded) {
      string last = codeParsed[codeParsed.size() - 1];
      // Primitive syntax checking for class and routine def,
      // and logic or loop controls:
      if (last == "{") {
        if (posClass >= 0) {
          checkClassDef(posClass, codeParsed.size() - 1);
          posClass = -1;
        } else if (posRoutine >= 0) {
          checkRoutineDef(posRoutine, codeParsed.size() - 1);
          posRoutine = -1;
        } else if (posLL >= 0) {
          checkLogicLoop(posLL, codeParsed.size() - 1);
          posLL = -1;
        }
      }
    }
  }

  if (opers.size() > 0) {
    if (intmOpers.find(opers) != intmOpers.end())
      error_undefined(srcFName, nline, "operator " + opers);

    codeParsed.push_back(opers);
    posLine.push_back(nline);
  }

  //	for(size_t i=0;i<codeParsed.size();i++)
  //		cout<<codeParsed[i]<<"  ";
  return 1;
}
bool TaoLexer::isValidName(string &word) {
  if (isDecimals(word) || isSpecialChars(word))
    return 0;
  if (word[0] != '_' && !islower(word[0]) && !isupper(word[0]))
    return 0;

  return 1;
}
bool TaoLexer::isValidLiteral(string &word) {
  if (isDecimals(word) || word[0] == '"' || islower(word[0]) ||
      isupper(word[0]) || word[0] == '_' || word[0] == '@')
    return 1;
  return 0;
}
void TaoLexer::checkClassDef(int start, int end) {
  // Check syntax of class definition:
  // Should be modified,
  // if more than one parent class is to be supported.
  if (!isValidName(codeParsed[start + 1])) {
    error_general(srcFName, posLine[start + 1],
                  "\"" + codeParsed[start + 1] +
                      "\" is not a valid class name");
  } else if (codeParsed[start + 2] != ":" && codeParsed[start + 2] != "{") {
    error_general(srcFName, posLine[start + 2],
                  "invalid codes in class definition");
  } else if (codeParsed[start + 2] == ":") {
    if (!isValidName(codeParsed[start + 3])) {
      error_general(srcFName, posLine[start + 3],
                    "\"" + codeParsed[start + 3] +
                        "\" is not a valid class name");
    } else if (codeParsed[start + 4] != "{") {
      error_general(srcFName, posLine[start + 4],
                    "invalid codes in class definition");
    }
  }
}
void TaoLexer::checkRoutineDef(int start, int end) {
  // Check syntax of subroutine definition:
  int rb, rb2;
  if (!isValidName(codeParsed[start + 1])) {
    error_general(srcFName, posLine[start + 1],
                  "\"" + codeParsed[start + 1] +
                      "\" is not a valid function name");
  } else if (codeParsed[start + 2] == "::") {
    start += 2;
  }

  if (codeParsed[start + 2] != "(") {
    error_general(srcFName, posLine[start + 2],
                  "invalid codes in funciton definition");
  } else if ((rb = wordPair(codeParsed, "(", ")", start + 2)) < 0) {
    error_unpaired(srcFName, posLine[start + 2], "()");
  } else if (codeParsed[rb + 1] != ":" && codeParsed[rb + 1] != ";" &&
             codeParsed[rb + 1] != "{") {
    error_general(srcFName, posLine[rb + 2],
                  "invalid codes in funciton definition");
  } else if (codeParsed[rb + 1] == ":") {
    if (!isValidName(codeParsed[rb + 2])) {
      error_general(srcFName, posLine[rb + 2],
                    "\"" + codeParsed[rb + 2] +
                        "\" is not a valid function name");
    } else if ((rb2 = wordPair(codeParsed, "(", ")", rb + 2)) < 0) {
      error_unpaired(srcFName, posLine[rb + 2], "()");
    } else if (codeParsed[rb2 + 1] != "{") {
      error_general(srcFName, posLine[rb2 + 1],
                    "invalid codes in function definition");
    }
  }
}
void TaoLexer::checkLogicLoop(int start, int end) {
  // Check syntax of logic and loop control:

  string word = codeParsed[start];

  if (word == "else") {
    // codeParsed[end]="{"
    if (start + 1 < end)
      error_general(srcFName, posLine[start], "invalid codes after \"else\"");
    return;
  }

  int right = findWord(codeParsed, "{", start, end);
  int lbrac = findWord(codeParsed, "(", start, end);
  int rbrac = wordPair(codeParsed, "(", ")", start, end);

  if (rbrac < 0)
    error_unpaired(srcFName, posLine[start], "()");

  if (lbrac > start + 1 || right > rbrac + 1)
    error_general(srcFName, posLine[start], "bad codes");
}
void TaoLexer::replaceEscape(string &field) {
  str_pos pos = field.find('\\');
  while (pos != string::npos) {
    if (field[pos + 1] == '"')
      field.replace(pos, 2, 1, '"');
    else if (field[pos + 1] == 't')
      field.replace(pos, 2, 1, '\t');
    else if (field[pos + 1] == 'n')
      field.replace(pos, 2, 1, '\n');
    else if (field[pos + 1] == '\\')
      field.replace(pos, 2, 1, '\\');
    pos = field.find('\\', pos + 1);
  }
}
bool TaoLexer::isDigits(string &word) {
  string::iterator it;
  for (it = word.begin(); it != word.end(); it++) {
    if (!isdigit(*it))
      return 0;
  }
  return 1;
}
bool TaoLexer::isDecimals(string &word) {
  string::iterator it;
  for (it = word.begin(); it != word.end(); it++) {
    if (!isdigit(*it) && *it != '.' && *it != 'E' && *it != 'e')
      return 0;
  }
  return 1;
}
bool TaoLexer::isSpecialChars(string &word) {
  string::iterator it;
  for (it = word.begin(); it != word.end(); it++) {
    if (lexKey.allOpers.find(*it) == string::npos)
      return 0;
  }
  return 1;
}
