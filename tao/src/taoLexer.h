////////////////////////////////////////////////////////////////////////
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

#ifndef TAO_LEXER_H
#define TAO_LEXER_H

#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class TaoKeyLexStroke {
public:
  TaoKeyLexStroke();

  //! All valid special characters:
  string allOpers;

  //! All valid composite operators:
  map<string, bool> compOpers;
  //! Logical and loop control keywords, only including
  //! those which require checking syntax:
  map<string, bool> keyLogLoop;

  //! Intermediate composite operators for composite operators
  //! with more than two chars, eg:
  //! <* is an intermediate composite operators for <*>.
  map<string, bool> intmOpers;
};

//! no comments!
class TaoLexer {
  static TaoKeyLexStroke lexKey;

  vector<string> codeParsed;
  vector<int> posLine;

  string codeSource;

public:
  TaoLexer();

  bool success;
  string srcFName;

  vector<string> &getParsedCode() { return codeParsed; }
  vector<int> &getPosLine() { return posLine; }

  bool parseCode(string &codeSource, bool replace = 1);

  bool isDigits(string &word);
  bool isDecimals(string &word);
  bool isSpecialChars(string &word);

  bool isValidName(string &word);
  bool isValidLiteral(string &word);

  void replaceEscape(string &chars);

  void checkClassDef(int start, int end);
  void checkRoutineDef(int start, int end);
  void checkLogicLoop(int start, int end);
};

#endif
