
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
#ifndef TAO_UTILITY_H
#define TAO_UTILITY_H

#include <string>
#include <vector>

#include "taoDatatype.h"
#include "taoRecycler.h"

using namespace std;

extern int wordPair(const vector<string> &words, const string &lw,
                    const string &rw, int start, int stop = -1, bool fw = 1);
extern int findWord(const vector<string> &words, const string &word,
                    int start = 0, int end = -1);
extern int findCleanWord(const vector<string> &words, const string &word,
                         int start = 0, int end = -1, bool reverse = 0);
extern int findCleanWords(const vector<string> &myWords, vector<string> &words,
                          vector<char> &types, int start, int end,
                          char *optype = 0);
extern void stripWordPair(const vector<string> &words, const string &lw,
                          const string &rw, int &start, int &end);

extern void warning(const string &fname, int line, const string &info);
extern void error_general(const string &fname, int line, const string &info);
extern void error_unpaired(const string &fname, int line, const string &info);
extern void error_undefined(const string &fname, int line, const string &name);

class TaoInfoKit {
public:
  TaoInfoKit(const string &fname, int line) {
    lineID = line;
    fileName = fname;
  }

  int lineID;
  string fileName;

  void warning(const string &info);
  void error_general(const string &info);
  void error_unpaired(const string &info);
  void error_undefined(const string &info);
};
//////////////////////////////////////////
//! _Tp must be a subclass of TaoBase;
/////////////////////////////////////////
template <class _Tp> class TaoTryAllocator : public TaoBase {
public:
  static _Tp *tryAllocate(_Tp *object) {
    TaoBase *tbase = (TaoBase *)object;
    if (!object) {
      tbase = (TaoBase *)new _Tp();
      tbase->refCount = 1;
      tbase->locked = 1;
    } else if (tbase->refCount > 1) {
      tbase->locked = 0;
      if (recycler)
        recycler->pushRootCandidate(tbase);
      tbase = (TaoBase *)new _Tp();
      tbase->refCount = 1;
      tbase->locked = 1;
    }
    return (_Tp *)tbase;
  }
};
class TaoIntArray;
class TaoIndexMaker {
public:
  static void makeIndex(valarray<size_t> &index, TaoBase *pid,
                        bool *asarray = 0, TaoInfoKit *infoKit = 0);
  static void makeIndex(valarray<size_t> &index, int N, TaoArithBase *arith,
                        bool *asarray = 0, TaoInfoKit *infoKit = 0);
  static TaoIntArray *whichIndexing(vector<vector<int>> &index,
                                    TaoIntArray *intArray);
};

#endif
