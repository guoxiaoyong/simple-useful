
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
#include <sstream>

#include "taoArith.h"
#include "taoConst.h"
#include "taoNumeric.h"
#include "taoRecycler.h"
#include "taoUtility.h"

extern TaoRecycler __Tao_Recycler;

int wordPair(const vector<string> &myWords, const string &lw, const string &rw,
             int start, int stop, bool fw) {
  if (start < 0)
    return -100;
  if (stop == -1) {
    if (fw)
      stop = myWords.size() - 1;
    else
      stop = 0;
  }
  int second;
  int k = 0;
  int i = start;
  while (1) {
    if ((fw && i > stop) || (!fw && i < stop))
      break;
    if (i < 0 || i >= (int)myWords.size())
      break;
    if (myWords[i] == lw)
      k++;
    else if (myWords[i] == rw) {
      k--;
      if (k == 0) {
        second = i;
        return second;
      }
    }
    if (fw)
      i++;
    else
      i--;
  }
  return -100;
}
int findWord(const vector<string> &myWords, const string &word, int start,
             int end) {
  if (start < 0)
    return -1000;
  if (end == -1)
    end = myWords.size() - 1;
  for (int i = start; i <= end; i++) {
    if (myWords[i] == word)
      return i;
  }
  return -1000;
}
int findCleanWord(const vector<string> &myWords, const string &word, int start,
                  int end, bool reverse) {
  if (start < 0)
    return -1000;
  if (end == -1)
    end = myWords.size() - 1;
  int n1, n2, n3;
  n1 = n2 = n3 = 0;
  if (reverse) {
    for (int i = end; i >= start; i--) {
      if (myWords[i] == word && n1 == 0 && n2 == 0 && n3 == 0)
        return i;
      else if (myWords[i] == "}")
        n1++;
      else if (myWords[i] == "{")
        n1--;
      else if (myWords[i] == ")")
        n2++;
      else if (myWords[i] == "(")
        n2--;
      else if (myWords[i] == "]")
        n3++;
      else if (myWords[i] == "[")
        n3--;
    }
  } else {
    for (int i = start; i <= end; i++) {
      if (myWords[i] == word && n1 == 0 && n2 == 0 && n3 == 0)
        return i;
      else if (myWords[i] == "{")
        n1++;
      else if (myWords[i] == "}")
        n1--;
      else if (myWords[i] == "(")
        n2++;
      else if (myWords[i] == ")")
        n2--;
      else if (myWords[i] == "[")
        n3++;
      else if (myWords[i] == "]")
        n3--;
    }
  }
  return -1000;
}
int findCleanWords(const vector<string> &myWords, vector<string> &words,
                   vector<char> &types, int start, int end, char *optype) {
  for (size_t i = 0; i < words.size(); i++) {
    int pos = findCleanWord(myWords, words[i], start, end, 1);
    if (pos >= 0) {
      if (optype)
        *optype = types[i];
      return pos;
    }
  }
  return -1000;
}
void stripWordPair(const vector<string> &myWords, const string &lw,
                   const string &rw, int &start, int &end) {
  int i, k;
  bool stripped = 1; // TRUE;
  while (stripped) {
    stripped = 0; // FALSE;
    if (myWords[start] == lw && myWords[end] == rw) {
      k = 0;
      for (i = start; i <= end; i++) {
        if (myWords[i] == lw)
          k++;
        else if (myWords[i] == rw)
          k--;

        if (k == 0)
          break;
      }
      if (k != 0) {
        cout << "Unparenthesis.\n";
        exit(0);
      }
      if (k == 0 && i == end) {
        stripped = 1; // TRUE;
        start++;
        end--;
        if (start > end)
          return;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////

void warning(const string &fname, int line, const string &info) {
  cout << ">>Warning( " << fname << " : " << line << " ): " << info << ".\n";
}
void error_general(const string &fname, int line, const string &info) {
  cout << ">>Error( " << fname << " : " << line << " ): " << info << ".\n";
  exit(0);
}
void error_unpaired(const string &fname, int line, const string &info) {
  cout << ">>Error( " << fname << " : " << line << " ): unpaired " << info
       << ".\n";
  exit(0);
}
void error_undefined(const string &fname, int line, const string &name) {
  cout << ">>Error( " << fname << " : " << line << " ): \"" << name
       << "\" undefined.\n";
  exit(0);
}
///////////////////////////////////////////////////////////////////
void TaoInfoKit::warning(const string &info) {
  cout << ">>Warning( " << fileName << " : " << lineID << " ): " << info
       << ".\n";
}
void TaoInfoKit::error_general(const string &info) {
  cout << ">>Error( " << fileName << " : " << lineID << " ): " << info << ".\n";
  exit(0);
}
void TaoInfoKit::error_unpaired(const string &info) {
  cout << ">>Error( " << fileName << " : " << lineID << " ): unpaired " << info
       << ".\n";
  exit(0);
}
void TaoInfoKit::error_undefined(const string &name) {
  cout << ">>Error( " << fileName << " : " << lineID << " ): \"" << name
       << "\" undefined.\n";
  exit(0);
}
///////////////////////////////////////////////////////
void TaoIndexMaker::makeIndex(valarray<size_t> &index, TaoBase *pid,
                              bool *asarray, TaoInfoKit *infoKit) {
  int rt = pid->rtti();
  if (rt == TAO_Number) {
    TaoNumber *num = (TaoNumber *)pid;
    index.resize(1, (int)num->value);
  } else if (rt == TAO_RealArray) {
    if (asarray)
      *asarray = 1;
    ((TaoArrayBase *)pid)->exportAsIndex(index);
  } else if (infoKit) {
    infoKit->warning("subindex with non-numeric types");
  }
}
void TaoIndexMaker::makeIndex(valarray<size_t> &index, int N,
                              TaoArithBase *arith, bool *asarray,
                              TaoInfoKit *infoKit) {
  if (asarray)
    *asarray = 0;
  if (arith->rtti() == TAO_PairArith) {
    if (asarray)
      *asarray = 1;
    int from = 0;
    int to = N - 1;
    if (arith->left && arith->left->rtti() != TAO_NullArith) {
      from = arith->left->myValue;
      TaoBase *tbase = arith->left->getResult();
      if (infoKit && (!tbase || tbase->rtti() != TAO_Number))
        infoKit->warning("1st index in the range is not a number");
    }
    if (arith->right && arith->right->rtti() != TAO_NullArith) {
      to = arith->right->myValue;
      TaoBase *tbase = arith->right->getResult();
      if (infoKit && (!tbase || tbase->rtti() != TAO_Number))
        infoKit->warning("2nd index in the range is not a number");
    }

    if (to - from + 1 <= 0) {
      index.resize(0);
    } else {
      index.resize(to - from + 1, 0);
      for (int j = from; j <= to; j++)
        index[j - from] = j;
    }
  } else {
    TaoBase *pid = arith->getResult();
    if (pid)
      pid = pid->dereference();

    if (pid && arith->rtti() != TAO_NullArith) {
      makeIndex(index, pid, asarray, infoKit);
    } else {
      index.resize(N, 0);
      for (int j = 0; j < N; j++)
        index[j] = j;
    }
  }
}
TaoIntArray *TaoIndexMaker::whichIndexing(vector<vector<int>> &index,
                                          TaoIntArray *intArray) {
  intArray = TaoTryAllocator<TaoIntArray>::tryAllocate(intArray);

  valarray<size_t> dm(2);
  dm = 0;
  dm[0] = index.size();
  if (dm[0] > 0)
    dm[1] = index[0].size();
  intArray->resize(dm);
  int *vec = intArray->getAsVector();
  for (size_t i = 0; i < dm[0]; i++) {
    for (size_t j = 0; j < dm[1]; j++)
      vec[i * dm[1] + j] = index[i][j];
  }
  return intArray;
}
