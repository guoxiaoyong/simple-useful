
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
#ifndef TAO_ENUMER_H
#define TAO_ENUMER_H

//#include"taoArith.h"
#include "taoNumeric.h"
//#include"taoConst.h"

typedef char TAO_BYTE;

enum TAO_ArithEnumTypes {
  TAO_Enumerate = 0,
  TAO_EnumHash = 1,
  TAO_EnumArray = 2,
  TAO_RangeArray = 3,
  TAO_EnumMatrix = 4,
};
class TaoChain;
class TaoInfoKit;

// class TaoArrayBase;
template <class _Tp> class TaoCompArray;
template <class _Tp> class TaoRealArray;
class TaoByteArray;
class TaoShortArray;
class TaoIntArray;
class TaoFloatArray;
class TaoDoubleArray;

// TaoEnumerate: base class for declaration of array/matrix by enumeration.
// For {a,b,c}, [a,b,c], {a:b:c}, [a:b:c] or {"A":1,"C",a}:
class TaoEnumerate {
public:
  TaoEnumerate();
  virtual ~TaoEnumerate(){};

  short typeNumeric;

  TaoBase *result;

  TaoInfoKit *infoKit;

  virtual void execute() = 0;

  static short RTTI;
  virtual short rtti() const;
};
// TaoEnumArray: class for declaration of array by enumeration.
class TaoEnumArray : public TaoEnumerate {
public:
  TaoEnumArray();
  virtual ~TaoEnumArray(){};

  // It's TRUE for [a,b,c] and [a:b:c]:
  bool isNumeric;

  vector<TaoArithBase *> rightAriths;

  TaoArray *array;

  TaoCompArray<double> *comArray;

  TaoByteArray *byteArray;
  TaoShortArray *shortArray;
  TaoIntArray *intArray;
  TaoFloatArray *floatArray;
  TaoDoubleArray *doubleArray;

  static short RTTI;
  short rtti() const;

  virtual void execute();
  virtual TaoBase *makeArray();
  virtual TaoBase *makeNumArray();

  //	template<class _Tp> void
  //convertValues(vector<TaoBase*>&,TaoRealArray<_Tp>*);
};
// TaoRangeArray: class for declaration of array by "range" expression.
class TaoRangeArray : public TaoEnumArray {
public:
  TaoRangeArray();
  virtual ~TaoRangeArray(){};

  // It's TRUE for [a:b:c]:
  bool isNumeric;

  static short RTTI;
  short rtti() const;

  void execute();

  TaoBase *makeArray();
  TaoBase *makeNumArray();
};
//	hash=[ "a" : 1 , "b"+"c" : 2*d[1] ];
//! TaoEnumHash: class for declaration of hash by enumeration.
class TaoEnumHash : public TaoEnumerate {
public:
  TaoEnumHash();
  virtual ~TaoEnumHash(){};

  vector<TaoArithBase *> rightAriths;
  TaoHash *hash;

  static short RTTI;
  short rtti() const;

  void execute();
};
// TaoEnumMatrix: class for declaration of numeric matrix
// by enumeration, such as:
// a=(1,2,3; 4,5,6; 7,8,9);
class TaoEnumMatrix : public TaoEnumerate {
public:
  TaoEnumMatrix();
  virtual ~TaoEnumMatrix(){};

  TaoDoubleArray *myMatrix;

  TaoCompArray<double> *myCotrix;

  int nRow, nColumn;
  vector<vector<TaoArithBase *>> matxAriths;

  static short RTTI;
  short rtti() const;

  void execute();
};
template <class _Tp>
void convertValues(vector<TaoBase *> &telem, TaoRealArray<_Tp> *array) {
  size_t j;
  size_t k = 0;
  for (j = 0; j < telem.size(); j++) {
    short rtti = telem[j]->rtti();
    if (rtti == TAO_Number) {
      k++;
    } else if (rtti == TAO_String) {
      k++;
    } else if (rtti == TAO_RealArray) {
      TaoArrayBase *array = (TaoArrayBase *)telem[j];
      k += array->size();
    }
  }
  array->resize(k);
  _Tp *vals = array->getAsVector();
  k = 0;
  for (j = 0; j < telem.size(); j++) {
    short rtti = telem[j]->rtti();
    if (rtti == TAO_Number) {
      TaoNumber *num = (TaoNumber *)telem[j];
      vals[k] = (_Tp)num->value;
      k++;
    } else if (rtti == TAO_String) {
      TaoString *str = (TaoString *)telem[j];
      vals[k] = (_Tp)atof(str->chars.c_str());
      k++;
    } else if (rtti == TAO_RealArray) {
      TaoArrayBase *array = (TaoArrayBase *)telem[j];
      valarray<_Tp> vec;
      exportValues(array, vec);
      size_t size = vec.size();
      for (size_t i = 0; i < size; i++) {
        vals[k] = vec[i];
        k++;
      }
    }
  }
}

#endif
