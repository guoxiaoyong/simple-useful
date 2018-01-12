
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
#ifndef TAO_ARITH_H
#define TAO_ARITH_H

#include "taoChain.h"
#include "taoObject.h"
//#include"taoEnumer.h"
//#include"taoNumeric.h"

enum TaoArithType {
  TAO_NullArith = 1,
  TAO_PairArith = 2,
  TAO_EnumArith = 3,
  TAO_SwitchArith = 4,
};

typedef char TAO_BYTE;

class TaoInfoKit;

template <class _Tp> class TaoCompArray;
class TaoByteArray;
class TaoShortArray;
class TaoIntArray;
class TaoFloatArray;
class TaoDoubleArray;

class TaoTagGenerator {
public:
  bool resume;
  vector<TaoRoutine *> routine;
};

//! Base class for arithmetic expression tree.
class TaoArithBase : public TaoBase {
protected:
public:
  TaoArithBase();
  virtual ~TaoArithBase();

  TaoArithBase *pro, *left, *right;

  //! Type of intermediate result.
  TAO_BYTE arithType;

  TaoBase *interResult;

  //! Intermediate results of number.
  string myChars;
  //! Intermediate results of string.
  double myValue;
  //! Intermediate results of complex number.
  complex<double> cmplx;

  bool bulean;

  TaoString *tstring;
  TaoNumber *number;
  TaoComplex *complx;

  //! Intermediate results of real numeric array.
  TaoByteArray *byteArray;
  TaoShortArray *shortArray;
  TaoIntArray *intArray;
  TaoFloatArray *floatArray;
  TaoDoubleArray *doubleArray;
  //! Intermediate results of complex numeric array.
  TaoCompArray<double> *comArray;

  TaoInfoKit *infoKit;

  bool nonTrans;

  TAO_BYTE getArithType() { return arithType; }

  //! Check arithmetic type of leaves after evaluation.
  void checkArithType(TaoBase *pbase);

  static short RTTI;
  virtual short rtti() const;

  //! Printing result.
  virtual void print(ostream *out = &cout){};

  //! Evaluate this node.
  virtual void compute() { ; }

  //! Get the final result.
  //! New object is created when neccessary.
  virtual TaoBase *getResult() { return 0; }

  TaoString *getString();
  TaoNumber *getNumber();
  TaoComplex *getComplex();

  void printString(ostream *out = &cout);
  void printNumber(ostream *out = &cout);
  void printComplex(ostream *out = &cout);
};
class TaoArithConst : public TaoArithBase {
public:
  static short RTTI;
  virtual short rtti() const;

  virtual void print(ostream *out = &cout){};
  virtual TaoBase *getResult() { return 0; }
};
class TaoArithString : public TaoArithConst {
public:
  TaoArithString() { arithType = 0; };

  void print(ostream *out = &cout) { printString(out); }
  TaoBase *getResult() { return getString(); }
};
class TaoArithNumber : public TaoArithConst {
public:
  TaoArithNumber() { arithType = 1; };

  void print(ostream *out = &cout) { printNumber(out); }
  TaoBase *getResult() { return getNumber(); }
};
class TaoArithComplex : public TaoArithConst {
public:
  TaoArithComplex() { arithType = 2; };

  void print(ostream *out = &cout) { printComplex(out); }
  TaoBase *getResult() { return (TaoBase *)getComplex(); }
};
class TaoArithVariable : public TaoArithBase {
public:
  TaoArithVariable() { refVariable = 0; }

  TaoReference *refVariable;

  void compute();
  void print(ostream *out = &cout);
  TaoBase *getResult();

  static short RTTI;
  virtual short rtti() const;
};
class TaoArithChain : public TaoArithBase {
public:
  TaoArithChain() { myChain = 0; }

  TaoChain *myChain;

  void compute();
  void print(ostream *out = &cout);
  TaoBase *getResult();

  static short RTTI;
  virtual short rtti() const;
};
class TaoArithNode : public TaoArithBase {
public:
  TaoArithNode();

  char oper;

  void compute();

  void numberArith();
  void stringArith();

  void numComArith();
  void comNumArith();
  void comComArith();

  void numArrayArith();
  void comArrayArith();

  void unaryArithLeft();
  void unaryArithRight();
  void binaryArith();

  void assignArith();

  TaoBase *duplicate(map<string, TaoReference *> &) { return 0; }

  void print(ostream *out = &cout);
  TaoBase *getResult();

  static short RTTI;
  virtual short rtti() const;
};
class TaoNullArith : public TaoArithBase {
public:
  static short RTTI;
  virtual short rtti() const;
};
class TaoSwitchArith : public TaoArithBase {
public:
  static short RTTI;
  virtual short rtti() const;
  void compute();
};
class TaoPairArith : public TaoArithBase {
public:
  static short RTTI;
  virtual short rtti() const;

  void compute() {
    if (left)
      left->compute();
    if (right)
      right->compute();
  }
};

class TaoEnumerate;
class TaoEnumArith : public TaoArithBase {
public:
  TaoEnumerate *myEnum;

  void compute();
  void print(ostream *out = &cout);
  TaoBase *getResult();

  static short RTTI;
  virtual short rtti() const;
};

// Mainly created for TaoRealArray subindexing,
// it's also used in other places.
class TaoArithArray : public TaoBase {
public:
  vector<TaoArithBase *> arithArray;

  void compute();
  size_t size() const { return arithArray.size(); }

  static short RTTI;
  virtual short rtti() const;
};

double mypow(double x, double y);

#endif
