
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
#include <iostream>

#include "taoArith.h"
#include "taoConst.h"
#include "taoRegex.h"
#include "taoUtility.h"

short TaoRgxMatch::RTTI = TAO_RgxMatch;

void TaoRgxBase::replace_escape_chars(string &str) {
  string::iterator iter;
  for (iter = str.begin(); iter != str.end(); ++iter) {
    if (*iter == '\\') {
      if ((*(iter + 1)) == '\\') {
        (*(iter + 1)) = '\\';
        str.erase(iter);
      } else if ((*(iter + 1)) == '/') {
        (*(iter + 1)) = '/';
        str.erase(iter);
      } else if ((*(iter + 1)) == ' ') {
        (*(iter + 1)) = ' ';
        str.erase(iter);
      } else if ((*(iter + 1)) == 't') {
        (*(iter + 1)) = '\t';
        str.erase(iter);
      } else if ((*(iter + 1)) == 'n') {
        (*(iter + 1)) = '\n';
        str.erase(iter);
      } else if ((*(iter + 1)) == '?') {
        (*(iter + 1)) = '?';
        str.erase(iter);
      } else if ((*(iter + 1)) == '*') {
        (*(iter + 1)) = '*';
        str.erase(iter);
      } else if ((*(iter + 1)) == '+') {
        (*(iter + 1)) = '+';
        str.erase(iter);
      } else if ((*(iter + 1)) == '{') {
        (*(iter + 1)) = '{';
        str.erase(iter);
      } else if ((*(iter + 1)) == '}') {
        (*(iter + 1)) = '}';
        str.erase(iter);
      } else if ((*(iter + 1)) == '(') {
        (*(iter + 1)) = '(';
        str.erase(iter);
      } else if ((*(iter + 1)) == ')') {
        (*(iter + 1)) = '(';
        str.erase(iter);
      } else if ((*(iter + 1)) == '[') {
        (*(iter + 1)) = '[';
        str.erase(iter);
      } else if ((*(iter + 1)) == ']') {
        (*(iter + 1)) = ']';
        str.erase(iter);
      } else if ((*(iter + 1)) == '$') {
        (*(iter + 1)) = '$';
        str.erase(iter);
      } else if ((*(iter + 1)) == '^') {
        (*(iter + 1)) = '^';
        str.erase(iter);
      } else if ((*(iter + 1)) == '.') {
        (*(iter + 1)) = '.';
        str.erase(iter);
      }
    }
  }
}
void TaoRgxBase::compute_min_max() {
  if (minArith && maxArith) {
    minArith->compute();
    repMin = (int)minArith->myValue;

    maxArith->compute();
    repMax = (int)maxArith->myValue;
  } else if (minArith) {
    minArith->compute();
    repMin = (int)minArith->myValue;
    if (repMax != MAX_REPEAT_MATCH)
      repMax = repMin;
  }
}
bool TaoRgxBase::match_min(const string &str, str_pos pos, str_pos &shift) {
  numMatched = 0;
  charShift = 0;
  while (numMatched < repMin) {
    if (!match_repeat(str, pos, shift))
      return 0;
  }
  return 1; // match_extend(str,pos,shift);
}
bool TaoRgxBase::match_extend(const string &str, str_pos pos, str_pos &shift) {
  while (numMatched < repMin) {
    if (!match_repeat(str, pos, shift))
      return 0;
  }
  return 1;
}
void TaoRgxArith::compute() {
  myArith->compute();
  TaoBase *pbase = myArith->getResult();
  if (pbase && pbase->rtti() == TAO_String) {
    TaoString *str = (TaoString *)pbase;
    word = str->chars;
  } else {
    word = "";
  }
}
bool TaoRgxArith::match_extend(const string &str, str_pos pos, str_pos &shift) {
  compute();
  return TaoRgxBase::match_extend(str, pos, shift);
}
bool TaoRgxArith::match_repeat(const string &str, str_pos pos, str_pos &shift) {
  if (numMatched >= repMax)
    return 0;
  compute();
  return TaoRgxWord::match_repeat(str, pos, shift);
}
bool TaoRgxWord::match_repeat(const string &str, str_pos pos, str_pos &shift) {
  pos += shift;
  if (numMatched >= repMax)
    return 0;

  size_t i;
  size_t len = str.length();
  for (i = 0; i < word.length(); i++) {
    if (pos + i >= len || word[i] != str[pos + i])
      return 0;
  }
  //  pos++;
  shift += word.length();
  numMatched++;
  charShift += word.length();
  return 1;
}

bool TaoRgxCharClass::match_escape_char(const char &ch) {
  if (escapeChar == '.' || (escapeChar == 't' && ch == '\t') ||
      (escapeChar == 'n' && ch == '\n') || (escapeChar == 'd' && isdigit(ch)) ||
      (escapeChar == 'D' && !isdigit(ch)) ||
      (escapeChar == 'c' && islower(ch)) ||
      (escapeChar == 'C' && isupper(ch)) ||
      (escapeChar == 'w' && (isalnum(ch) || ch == '_')) ||
      (escapeChar == 'W' && !(isalnum(ch) || ch == '_')) ||
      (escapeChar == 's' && isspace(ch)) ||
      (escapeChar == 'S' && !isspace(ch))) {
    return 1;
  }
  return 0;
}
bool TaoRgxCharClass::match_repeat(const string &str, str_pos pos,
                                   str_pos &shift) {
  pos += shift;
  if (numMatched >= repMax)
    return 0;

  if (pos >= str.length() || !match_escape_char(str[pos]))
    return 0;

  numMatched++;
  charShift++;
  shift++;
  //  pos++;
  return 1;
}
void TaoRgxCharEnum::makeStrClass(const string &s, TaoInfoKit *infoKit) {
  //  cout<<"TaoRgxCharEnum::makeStrClass()\n";
  //  for(size_t i=0;i<s.size();i++) cout<<s[i];
  //  cout<<endl;

  str_pos start = 0;
  toMatch = 1;
  if (s[0] == '^') {
    toMatch = 0;
    start = 1;
  }
  string temp = s.substr(start, string::npos);
  string::iterator iter;
  for (iter = temp.begin(); iter != temp.end(); ++iter) {
    if (*iter == '\\') {
      if ((*(iter + 1)) == 'd' || (*(iter + 1)) == 'D' ||
          (*(iter + 1)) == 's' || (*(iter + 1)) == 'S' ||
          (*(iter + 1)) == 'c' || (*(iter + 1)) == 'C' ||
          (*(iter + 1)) == 'w' || (*(iter + 1)) == 'W' ||
          (*(iter + 1)) == 't' || (*(iter + 1)) == 'n') {
        TaoRgxCharClass *charClass = new TaoRgxCharClass();
        charClass->set_escape_char(*(iter + 1));
        specCharClass.push_back(charClass);
        iter = temp.erase(iter);
        iter = temp.erase(iter);
        iter--;
      }
    }
  }
  //  for(iter=temp.begin();iter!=temp.end();++iter) cout<<*iter;
  //  cout<<endl;

  replace_escape_chars(temp);
  str_pos pos = temp.find('-', start);
  while (pos != string::npos) {
    if (pos == 0 || pos == temp.length() - 1)
      infoKit->error_general("invalid regular pattern");
    char ch1 = temp[start];
    char ch2 = temp[pos + 1];
    for (char c = ch1; c <= ch2; c++)
      strClass += c;
    start = pos + 2;
    pos = temp.find('-', start);
  }
}
bool TaoRgxCharEnum::match_repeat(const string &str, str_pos pos,
                                  str_pos &shift) {
  if (numMatched >= repMax)
    return 0;
  pos += shift;

  for (size_t i = 0; i < specCharClass.size(); i++) {
    if (specCharClass[i]->match_escape_char(str[pos])) {
      if (toMatch) {
        charShift++;
        numMatched++;
        shift++;

        return 1;
      } else {
        return 0;
      }
    }
  }
  if (pos >= str.length())
    return 0;
  str_pos p = strClass.find(str[pos]);
  if ((p == string::npos && !toMatch) || (p != string::npos && toMatch)) {
    charShift++;
    numMatched++;
    shift++;
  } else {
    return 0;
  }
  return 1;
}

bool TaoRgxGroup::match_min(const string &str, str_pos pos, str_pos &shift) {
  while (!idRgx.empty()) {
    idRgx.pop();
    rgxShift.pop();
  }
  numMatched = 0;
  charShift = 0;
  posStart = pos + shift;
  while (numMatched < repMin) {
    if (!match_repeat(str, pos, shift))
      return 0;
  }
  return 1; // match_extend(str,pos,shift);
}
bool TaoRgxGroup::match_extend(const string &str, str_pos pos, str_pos &shift) {
  while (numMatched < repMin) {
    if (!match_repeat(str, pos, shift))
      return 0;
  }
  return 1;
}
bool TaoRgxGroup::match_repeat(const string &str, str_pos pos, str_pos &shift) {
  size_t resume = 0;
  if (!idRgx.empty())
    resume = idRgx.top();

  if (numMatched >= repMax && resume - 1 < myRegex.size()) {
    TaoRgxBase *rgxLast =
        myRegex[resume - 1]->rgxUnit[myRegex[resume - 1]->rgxUnit.size() - 1];
    if (rgxLast->numMatched < rgxLast->repMax) {
      // If the last rgxUnit of (resume-1) not matched to "repMax"
      // a="abdcocosss";
      // b = a=~/(@1:\c+)@{@1}/e=@1/;
      // "b" should be "coco"
      str_pos start = posStart + charShift;
      str_pos p2 = 0;
      bool bl = rgxLast->match_repeat(str, start, p2);
      if (bl) {
        shift += p2;
        rgxLast->numMatched++;
        rgxShift.top() += p2;
        charShift += p2; // shift+pos-posStart;
        if (transient) {
          strExtract = TaoTryAllocator<TaoString>::tryAllocate(strExtract);
          strExtract->chars = str.substr(posStart, charShift);
          transient->content = strExtract;
        }
        return 1;
      }
    }
  }
  // For / \d+ ( ad\c+ | df | \w\c+ ) \d+ /:
  //               1     2     3
  // If (2)/df/ is matched, but /d+/ is not matched subsequently,
  // then match_repeat() should be started from (3)/\c+/:

  // a="12ababcADGabc1234aaffgsdfb223456ab";
  // b=a=~/(\d+(a| ([a-c]|\C+) )+)(1|2)/;
  if (numMatched >= repMax) {
    size_t tmp = 0;
    int num = 0;
    // Searching possible "branch" to continue:
    while (resume >= myRegex.size()) {
      tmp += rgxShift.top();
      num++;
      idRgx.pop();
      rgxShift.pop();
      if (idRgx.empty())
        return 0;
      resume = idRgx.top();
    }
    // rewind states:
    numMatched -= num;
    shift -= tmp;
    charShift -= tmp;
    if (numMatched >= repMax)
      return 0;
  } else {
    // If numMatched<repMax, match_repeat() should be started from
    // the first regex alternative:
    resume = 0;
    idRgx.push(0);
    rgxShift.push(0);
  }

  for (size_t i = resume; i < myRegex.size(); i++) {
    // str_pos p1=pos;
    str_pos p2;
    myRegex[i]->extend = 0;
    if (myRegex[i]->match(str, posStart + charShift, p2)) {
      shift += p2;
      numMatched++;
      charShift += p2;
      rgxShift.top() = p2;
      idRgx.top() = i + 1;
      if (transient) {
        strExtract = TaoTryAllocator<TaoString>::tryAllocate(strExtract);
        strExtract->chars = str.substr(posStart, charShift);
        transient->content = strExtract;
      }
      myRegex[i]->extend = 1;
      return 1;
    }
    myRegex[i]->extend = 1;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////
/*
        The basic idea of the algorithm in TaoRegex::match():

        A single regex pattern is a set of string describing which kind
        of chars will be matched followed by +,*,? or {repMin,repMax}.
        For example:
                \w{ 3 } where repMin=3 and repMax=3
                \d{2,4} where repMin=2 and repMax=4
                [1234]+ where repMin=1 and repMax=infinite

        Two functions match_min() and match_repeat() are defined for
        each regex pattern (rgxUnit):
                match_min():
                        will match a pattern to a string up to repMin times.
                match_repeat():
                        will extend the matching of one pattern one time more.

        This algorithm is a repeat process, a boolean flag "min_match"
        is used to changing the matching type:
                If TRUE, match_min() will be performed with current regex
   pattern.
                If FALSE, match_repeat() will be performed.

                If match_min() or match_repeat() is successful with rgxUnit[j]:
                        The next rgxUnit[j+1] will become the current regex
   pattern,
                        and "min_match" will be set TRUE to perform new matching
   with
                        rgxUnit[j+1].
                Otherwise:
                        The previous rgxUnit[j-1] will become the current regex
   pattern,
                        and "min_match" will be set FALSE to extend the matching
   of
                        rgxUnit[j-1].
*/
//////////////////////////////////////////////////////////////////////
bool TaoRegex::match(const string &str, str_pos pos, str_pos &shift) {
  //  for(size_t k=pos;k<str.size();k++) cout<<str[k];
  int npatt = rgxUnit.size();
  //  cout<<"\t"<<extend<<"\t"<<npatt<<"\n";
  str_pos strlen = str.length();

  // bool success=0;
  // str_pos oldStart,oldShift;

  for (int k = 0; k < npatt; k++)
    rgxUnit[k]->reset();

  shift = 0;
  int j = 0;
  bool min_match = 1;
  while (0 <= j && j < npatt) {
    str_pos start = pos; //+shift;

    // If the string is finished:
    if (start >= strlen) {
      if (j < npatt) {
        for (int jk = j; jk < npatt; jk++) {
          if (!rgxUnit[jk]->ignorable)
            return 0;
        }
      }
      return 1;
    }
    if (min_match) {
      // For the first matching of each pattern:
      if (rgxUnit[j]->match_min(str, start, shift)) {
        // Repeat matching if it is the last pattern,
        // or the next pattern can be matched with ZERO times:
        if (extend && (j == npatt - 1 || rgxUnit[j + 1]->ignorable)) {
          start = pos; //+shift;
          while (rgxUnit[j]->match_repeat(str, start, shift)) {
            start = pos; //+shift;
          }
          // If it is the last pattern:
          if (j == npatt - 1)
            break;
        }
        // If the j-th pattern is matched, move to
        // the next one and perform match_min():
        j++;
        min_match = 1;
      } else if (rgxUnit[j]->ignorable) {
        // If j-th pattern is not matched and,
        // if j-th pattern can be matched with ZERO times,
        // move to the next one:
        j++;
        min_match = 1;
      } else {
        // The first matching of j-th pattern failed,
        // get back to the previous pattern, and perform
        // match_repeat(),
        // to extend the matching of (j-1)-th pattern:
        j--;
        min_match = 0;
      }
    } else {
      if (extend && (j == npatt - 1 || rgxUnit[j + 1]->ignorable)) {
        // Repeat matching if it is the last pattern,
        // or the next pattern can be matched with ZERO times:
        start = pos; //+shift;
        while (rgxUnit[j]->match_repeat(str, start, shift)) {
          start = pos; //+shift;
        }
        if (j == npatt - 1)
          return 1;

        // If it is NOT the last pattern, and is NOT matched up
        // to "repMax",
        j++;
        min_match = 1;
      } else if (rgxUnit[j]->match_repeat(str, start, shift)) {
        // If j-th pattern was not matched up to "repMax",
        // try to match another time, and if it's matched,
        // move to (j+1)-th pattern to perform match_min():
        j++;
        min_match = 1;
      } else {
        // If repeat match of j-th pattern failed,
        // go back to (j-1)-th pattern and perform match_repeat()
        // to extend the substring matching (j-1)-th pattern:

        // Go back to the position
        // until where the first (j-1) pattern have been matched:
        shift -= rgxUnit[j]->charShift;

        j--;
        // To perform match_repeat():
        min_match = 0;
      }
    }
  }
  if (j < 0) {
    shift = 0;
    return 0;
  }
  return 1;
}
/////////////////////////////////////////////////////////////////////
/*
        This repeat matching process is started on the string from certain
        position "pos". If successful, the starting position "start" and a
        shift from "start" are returned to describe the matched substring;
        otherwise, the matching process will be repeated on the string from
        the next position "pos+1".

*/
///////////////////////////////////////////////////////////////////
bool TaoRegex::search(const string &str, str_pos &pos, str_pos &shift) {
  str_pos start = pos;

  for (size_t j = 0; j < rgxUnit.size(); j++)
    rgxUnit[j]->compute_min_max();

  str_pos strlen = str.length();

  while (start < strlen) {
    if (match(str, start, shift)) {
      pos = start;
      return 1;
    }
    start++;
  }
  return 0;
}

////////////////////////////////////////////////////////

void TaoRgxMatch::execute(TaoString *taostr) {
  string &str = taostr->chars;

  if (replaceAll) {
    str_pos start = 0;
    str_pos shift;
    bool bl = regex->search(str, start, shift);
    if (rgxType == 1) {
      while (bl) {
        if (strArith)
          strArith->compute();
        str.replace(start, shift, strArith->myChars);
        poStart = start;
        poShift = strArith->myChars.size();
        start = poStart + poShift;
        bl = regex->search(str, start, shift);
      }
    } else if (rgxType == 2) {
      while (bl) {
        strArith->compute();
        str.replace(poStart + poShift, start - poStart - poShift,
                    strArith->myChars);
        poStart = poStart + poShift + strArith->myChars.size();
        poShift = shift;
        bl = regex->search(str, start, shift);
      }
      strArith->compute();
      str.replace(poStart + poShift, str.size() - poStart - poShift,
                  strArith->myChars);
    }
    poStart = 0;
    poShift = 0;
    strResult = taostr;
    return;
  }
  //  for(size_t j=0;j<str.size();j++) cout<<str[j];
  if (restart || poStart > str.length()) {
    rgxBool = 0;
    poStart = 0;
    poShift = 0;
    strResult = 0;
    if (!restart)
      return;
  }

  strExtract = TaoTryAllocator<TaoString>::tryAllocate(strExtract);

  strResult = strExtract;

  str_pos start = poStart + poShift;
  str_pos shift;

  if (rgxType == 1) {
    // For string =~ /regex/
    bool bl = regex->search(str, start, shift);
    // For string =~ /regex/arith/
    if (strArith)
      strArith->compute();
    if (bl) {
      strExtract->chars = str.substr(start, shift);
      rgxBool = 1;
      poStart = start;
      poShift = shift;
      if (replace) {
        // For string =~ s/regex/arith/
        str.replace(start, shift, strArith->myChars);
        poShift = strArith->myChars.size();
      }
    } else {
      strExtract->chars.erase();
      strResult = 0;
      rgxBool = 0;
      poStart = poShift = 0;
    }
  } else if (rgxType == 2) {
    // For string !~ /regex/
    bool bl = regex->search(str, start, shift);
    // For string !~ /regex/arith/
    if (strArith)
      strArith->compute();
    if (bl) {
      // Get the substring in between of the last matching and this one:
      strExtract->chars =
          str.substr(poStart + poShift, start - poStart - poShift);
      rgxBool = 1;
      if (replace) {
        // For string !~ s/regex/arith/
        str.replace(poStart + poShift, start - poStart - poShift,
                    strArith->myChars);
        poStart = poStart + poShift + strArith->myChars.size();
      } else {
        poStart = start;
      }
      poShift = shift;
    } else {
      strExtract->chars = str.substr(poStart + poShift);
      rgxBool = 1;
      if (replace) {
        strArith->compute();
        str.replace(poStart + poShift, str.size() - poStart - poShift,
                    strArith->myChars);
      }
      poStart = string::npos;
      poShift = 0;
    }
  } else {
    // For string ~~ /regex/
    if (blmatch) {
      bool bl = regex->search(str, start, shift);
      // For string ~~ /regex/arith/
      if (strArith)
        strArith->compute();
      if (bl) {
        strExtract->chars =
            str.substr(poStart + poShift, start - poStart - poShift);
        rgxBool = 1;
        poStart = start;
        poShift = shift;
      } else {
        strExtract->chars = str.substr(poStart + poShift);
        rgxBool = 1;
        poStart = string::npos;
      }
      blmatch = 0;
    } else {
      blmatch = 1;
      strExtract->chars = str.substr(poStart, poShift);
      rgxBool = 1;
    }
  }
}

void TaoRgxReplace::execute(string &str) {}
