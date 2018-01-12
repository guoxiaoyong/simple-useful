
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
#ifndef TAO_DATA_H
#define TAO_DATA_H

#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <valarray>
#include <vector>

#define TAO_RTTI(Klass) int Tao##Klass::RTTI = TAO_##Klass;

#define MAX(x, y) (x) > (y) ? (x) : (y)

#include "taoCpptype.h"

using namespace std;

typedef string::size_type str_pos;

class TaoRecycler;
class TaoIntArray;
class TaoInfoKit;
class TaoArithBase;
class TaoArray;
class TaoPlugin;
//! Base class Tao data types.
/*	TaoBase is the base class for most of Tao data type classes.
        All class inherited from TaoBase can be holded in
        TaoArray and TaoHash, which is also derived from TaoBase
        to provide complex data structures.
*/
class TaoBase {
public:
  TaoBase();
  virtual ~TaoBase(){};

  // Reference count.
  int refCount;

  // Used for Garbage Collection scanning of objects:
  bool scanned;
  // Used for avoid printing/duplicating cyclic structure:
  // It's also used in arithmetic computation:
  bool visited;
  //	Used for Garbage Collection, it's set to TRUE
  // to prevent deletion in GC:
  bool locked;

  static TaoRecycler *recycler;

  static short RTTI;
  virtual short rtti() const { return RTTI; }
  virtual void print(ostream *out = &cout){};

  virtual TaoBase *duplicate() { return new TaoBase; }
  virtual TaoBase *dereference() { return this; }

  virtual TcBase *toCppType() { return 0; }
  virtual void fromCppType(){};

  static int getExactType(TaoBase *tbase);

  static TaoBase *fromCppType(TcNumber *tc);
  static TaoBase *fromCppType(TcComplex *tc);
  static TaoBase *fromCppType(TcString *tc);
  static TaoBase *fromCppType(TcArray *tc);
  static TaoBase *fromCppType(TcHash *tc);
  static TaoBase *fromCppType(TaoPlugin *tc);
  static TaoBase *fromCppType(TcByteArray *tc);
  static TaoBase *fromCppType(TcShortArray *tc);
  static TaoBase *fromCppType(TcIntArray *tc);
  static TaoBase *fromCppType(TcFloatArray *tc);
  static TaoBase *fromCppType(TcDoubleArray *tc);
  static TaoBase *fromCppType(TcComplexArray *tc);
};

//! Tao reference type.
class TaoReference : public TaoBase {
public:
  TaoReference() { content = 0; }
  TaoReference(TaoBase *p) {
    if ((content = p))
      p->refCount++;
  }
  ~TaoReference(){};

  TaoBase *content;

  static short RTTI;
  short rtti() const { return RTTI; }
  virtual short rttiRefer() const { return RTTI; }
  void print(ostream *out = &cout);

  TaoBase *dereference();

  TaoBase *duplicate();
  virtual TaoReference *newObject() { return new TaoReference; }
};

/*
        TaoNumber is a class to hold double type data.
        It's created for array/hash data type, since
        TaoArray/TaoHash can't hold primitive double data.
*/
//! Tao numeric type.
class TaoNumber : public TaoBase {
  TcNumber *cpptype;

public:
  TaoNumber(double num = 0) {
    value = num;
    cpptype = 0;
  }
  virtual ~TaoNumber(){};

  double value;

  static short RTTI;
  short rtti() const { return RTTI; }
  void print(ostream *out = &cout) { *out << value; }

  TaoBase *duplicate() { return new TaoNumber(value); }
  TcBase *toCppType();
  void fromCppType();
};

/*
        TaoString is a class to hold string type data.
        It's created for array/hash data type, since
        TaoArray/TaoHash can't hold primitive C++ STL
        string type.
*/
//! Tao string type.
class TaoString : public TaoBase {
  TcString *cpptype;

  static bool blQuotate;

public:
  TaoString();
  TaoString(const string &chs);
  virtual ~TaoString(){};

  string chars;

  int start, end;

  TaoString *tstring;

  static short RTTI;
  short rtti() const { return RTTI; }

  static void setOnQuotate(bool on) { blQuotate = on; }
  void print(ostream *out = &cout);

  TaoBase *duplicate() { return new TaoString(chars); }

  void assignWith(TaoBase *tbase);
  void resetSubIndex() { start = end = -1; }
  void setSubIndex(TaoArithBase *arith, TaoInfoKit *infoKit);

  TaoBase *subIndexing(TaoInfoKit *infoKit);

  TcBase *toCppType();
  void fromCppType();
};

class TaoComplex;
//! Tao array/list type, capable of holding all data types.
class TaoArray : public TaoBase {
  TcArray *cpptype;

protected:
  static short RTTI;

public:
  TaoArray(int num = 0);
  virtual ~TaoArray();

  vector<TaoBase *> elements;

  valarray<size_t> subIndex;

  // If TRUE, return an array in subindexing,
  // even if the subindex only specifies one element.
  bool asArray;

  TaoArray *subArray;
  TaoNumber *number;
  TaoString *tstring;
  TaoComplex *complx;

  TaoIntArray *whichIndex;

  virtual short rtti() const { return RTTI; }

  virtual void print(ostream *out = &cout);

  virtual TaoBase *duplicate();

  TaoBase *element(size_t i) { return elements[i]; }

  virtual void append(TaoBase *pbase);
  virtual void insert(TaoBase *pbase, int pos);
  virtual void replace(TaoBase *pbase, int pos);
  virtual void erase(int pos);
  virtual void clear();

  size_t size() { return elements.size(); }
  void exportElements(TaoArray *array);

  virtual void resetSubIndex() { subIndex.resize(0); }
  virtual void setSubIndex(TaoArithBase *arith, TaoInfoKit *infoKit);
  virtual TaoBase *subIndexing(TaoInfoKit *infoKit);

  TaoBase *basicMathFunct(TaoInfoKit *infoKit, short type = 1);

  void heapSort(TaoArithBase *, int, vector<TaoReference *> &, TaoInfoKit *);
  bool compare(TaoBase *, TaoBase *, TaoArithBase *, vector<TaoReference *> &,
               TaoInfoKit *);

  TaoIntArray *whichByOper(char oper, TaoBase *tbase);
  TaoIntArray *whichByArith(TaoArithBase *, vector<TaoReference *> &);

  virtual void assignWith(TaoBase *tbase);

  virtual TcBase *toCppType();
  virtual void fromCppType();
};

//	Tao hash/dictionary data type.
class TaoHash : public TaoBase {
  TcHash *cpptype;

public:
  TaoHash() {
    subHash = 0;
    cpptype = 0;
    asHash = 0;
  }
  virtual ~TaoHash() { clear(); }

  map<string, TaoBase *> elements;

  string keyLower;
  string keyUpper;

  TaoHash *subHash;

  // If TRUE, return a hash in subindexing,
  // even if the subindex only specifies one element.
  bool asHash;

  static short RTTI;
  virtual short rtti() const { return RTTI; }

  void print(ostream *out = &cout);

  void insert(const string &key, TaoBase *pbase);
  void clear();

  void resetSubIndex();
  void setSubIndex(TaoArithBase *arith, TaoInfoKit *infoKit);
  TaoBase *subIndexing(TaoInfoKit *infoKit);

  TaoBase *duplicate();

  void assignWith(TaoBase *tbase);

  TcBase *toCppType();
  void fromCppType();
};

#endif
