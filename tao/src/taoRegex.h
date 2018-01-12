
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
#ifndef TAO_PATTERN_H
#define TAO_PATTERN_H

#include <map>
#include <stack>
#include <string>
#include <vector>

#include "taoDatatype.h"

#define MAX_REPEAT_MATCH 99999999L

using namespace std;

typedef string::size_type str_pos;

class TaoReference;
class TaoArithBase;
//! no comments!
class TaoRgxBase {
public:
  TaoRgxBase() {
    repMin = repMax = 1;
    minArith = maxArith = 0;
    ignorable = 0;
    firstRound = 1;
    extend = 1;
  }
  virtual ~TaoRgxBase(){};

  str_pos charShift;

  bool ignorable;
  bool firstRound;

  bool extend;

  int numMatched, repMin, repMax;

  TaoArithBase *minArith, *maxArith;

  virtual bool reset() { return 0; };

  virtual bool match_min(const string &, str_pos, str_pos &);
  virtual bool match_repeat(const string &, str_pos, str_pos &) { return 0; }
  virtual bool match_extend(const string &, str_pos, str_pos &);

  void replace_escape_chars(string &str);

  virtual void compute_min_max();
};

class TaoRgxStartEnd : public TaoRgxBase {
public:
  char oneChar;

  bool match_repeat(const string &str, str_pos pos, str_pos &shift) {
    return 0;
  }
};

//	match word:	/cat/
//! no comments!
class TaoRgxWord : public TaoRgxBase {
public:
  TaoRgxWord(){};
  ~TaoRgxWord(){};

  string word;

  void setWord(const string &s) {
    word = s;
    replace_escape_chars(word);
  }

  bool match_repeat(const string &str, str_pos pos, str_pos &shift);

  virtual void compute_min_max() { TaoRgxBase::compute_min_max(); }
};
// match a string resulted from a string arithmetic expression:
//		/${string arithmetic expression}/
//! no comments!
class TaoRgxArith : public TaoRgxWord {
public:
  TaoRgxArith(){};
  ~TaoRgxArith(){};

  TaoArithBase *myArith;

  void compute();
  bool match_repeat(const string &str, str_pos pos, str_pos &shift);
  bool match_extend(const string &, str_pos, str_pos &);
};
// math string class: \d \s \w
//! no comments!
class TaoRgxCharClass : public TaoRgxBase {
  char escapeChar;

public:
  TaoRgxCharClass(){};
  ~TaoRgxCharClass(){};

  void set_escape_char(char c) { escapeChar = c; }

  bool match_escape_char(const char &ch);

  bool match_repeat(const string &str, str_pos pos, str_pos &shift);
};
//	math string class by enumeration:	/[abc]/	/[a-z]/	/[^abc]/
//! no comments!
class TaoRgxCharEnum : public TaoRgxBase {
  bool toMatch;
  string strClass;

public:
  TaoRgxCharEnum() { toMatch = 1; }
  ~TaoRgxCharEnum(){};

  vector<TaoRgxCharClass *> specCharClass;

  void makeStrClass(const string &s, TaoInfoKit *infoKit);

  bool match_repeat(const string &str, str_pos pos, str_pos &shift);
};

class TaoRegex;

class TaoRgxGroup : public TaoRgxBase {
  // For / \d+ ( ad\c+ | df | \c+ ) \d+ /:
  //               1     2     3
  // If (2)/df/ is matched, but /d+/ is not matched subsequently,
  // then match_repeat() should be started from (3)/\c+/:
  stack<size_t> idRgx;
  stack<size_t> rgxShift;

  str_pos posStart;

  TaoString *strExtract;

public:
  TaoRgxGroup() {
    transient = 0;
    strExtract = 0;
  };
  ~TaoRgxGroup(){};

  vector<TaoRegex *> myRegex;

  TaoReference *transient;

  bool reset() { return 0; }
  bool match_min(const string &, str_pos, str_pos &);
  bool match_repeat(const string &str, str_pos pos, str_pos &shift);
  bool match_extend(const string &, str_pos, str_pos &);
};

//! no comments!
class TaoRegex : public TaoRgxBase {
public:
  TaoRegex(){};
  ~TaoRegex(){};

  vector<TaoRgxBase *> rgxUnit;

  // Return TRUE, if there is a substring matching this TaoRegex
  // from position "pos", the length is saved in shift:
  bool match(const string &str, str_pos pos, str_pos &shift);
  // Return TRUE, if a substring matching this TaoRegex is found
  // starting from position "pos", "pos" saves the new position
  // where it is matched and the length is saved in shift:
  bool search(const string &str, str_pos &pos, str_pos &shift);
};

/////////////////////////////////////////////////////////////

class TaoRgxMatch : public TaoBase {
protected:
  // "poStart" is the starting position of each matched substring;
  // "poShift" is its length:
  str_pos poStart, poShift;

public:
  TaoRgxMatch() {
    rgxType = 0;
    poStart = poShift = 0;

    regex = 0;
    strArith = 0;
    replace = replaceAll = 0;
    strExtract = 0;

    blmatch = 1;
    restart = 0;

    rgxBool = 0;
  }

  bool restart;

  bool rgxBool;

  bool replace;
  bool replaceAll;

  // For ~~ only, if blmatch=1, perform matching:
  bool blmatch;

  int rgxType;

  TaoRegex *regex;
  TaoArithBase *strArith;

  TaoString *strExtract;
  TaoString *strResult;

  virtual void execute(TaoString *taostr);
  static short RTTI;
  short rtti() const { return RTTI; }
};
class TaoRgxReplace : public TaoRgxMatch {
public:
  void execute(string &str);
};

#endif
