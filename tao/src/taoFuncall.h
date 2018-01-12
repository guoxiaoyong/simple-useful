
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
#ifndef TAO_FUNCALL_H
#define TAO_FUNCALL_H

#include "taoNumeric.h"
#include "taoObject.h"
#include "taoRecycler.h"

class TaoChain;

class TaoInfoKit;
class TaoAddOn;
class TaoPlugin;
class TaoModule;
class TaoArithBase;

class TaoLibrary;
class TaoNameSpace;

class TaoNumArrayMaker : public TaoBase {
public:
  TaoNumArrayMaker() {
    comArray = 0;
    byteArray = 0;
    shortArray = 0;
    intArray = 0;
    floatArray = 0;
    doubleArray = 0;
  }

  TaoCompArray<double> *comArray;

  TaoByteArray *byteArray;
  TaoShortArray *shortArray;
  TaoIntArray *intArray;
  TaoFloatArray *floatArray;
  TaoDoubleArray *doubleArray;

  TaoArrayBase *make(valarray<size_t> &dims, short type);
};

//! no comments!
class TaoFunCall : public TaoBase {
  int numCalled;

  TaoGcAgent gcAgent;

public:
  TaoFunCall() {
    numCalled = 0;
    funType = 0;
    ioType = -1;
    isInternal = 0;
    preRoutine = 0;
    funCaller = 0;
    myClass = 0;
    number = 0;
    complx = 0;
    tstring = 0;
    myArray = 0;
    blResult = 0;
    myHash = 0;
    numarrayMaker = 0;
  }

  bool isInternal;

  // 0: open
  // 1: write
  // 2: read
  int ioType;
  bool neof;

  short funType;

  string libName;
  string funName;

  TaoInfoKit *infoKit;

  TaoBase *result;
  TaoBase *funCaller;

  TaoModule *preRoutine;
  TaoClass *myClass;

  vector<TaoReference *> transient;

  vector<TaoArithBase *> paramAriths;

  TaoArray inArray;

  TaoNumber *number;
  TaoComplex *complx;
  TaoString *tstring;
  TaoArray *myArray;
  TaoHash *myHash;

  TaoNumArrayMaker *numarrayMaker;

  bool blResult;

  TaoNameSpace *defaultNS;

  static short RTTI;
  short rtti() const;
  void print(FILE *fout = 0){};

  void execute();

  bool runIOFile();

  bool runBasicMath();
  bool runNumMath();
  bool runMagicFunct();
  bool runUtileFunct();

  bool runMethod(TaoHash *caller);
  bool runMethod(TaoArray *caller);
  bool runMethod(TaoAddOn *caller);
  bool runMethod(TaoPlugin *caller);

  bool runMethod(TaoArrayBase *caller);
  bool runMethod(TaoCompArray<double> *caller);

  bool runMethod(TaoObject *caller);
  bool runMethod(TaoNameSpace *caller);
  bool runMethod(TaoModule *caller);

  bool runMethod(TaoLibrary *lib, string funame);

  TaoBase *duplicate(map<string, TaoReference *> &) { return 0; }

  TaoBase *getResult();
  bool moreToYield() { return blResult; }
};
#endif
