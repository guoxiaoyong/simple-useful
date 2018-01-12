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
#include "taoEnumer.h"
#include "taoArith.h"
#include "taoChain.h"
#include "taoConst.h"
#include "taoDatatype.h"
#include "taoNumeric.h"
#include "taoRecycler.h"
#include "taoUtility.h"
#include <iostream>

short TaoEnumerate::RTTI = TAO_Enumerate;
short TaoEnumerate::rtti() const { return RTTI; }
short TaoEnumArray::RTTI = TAO_EnumArray;
short TaoEnumArray::rtti() const { return RTTI; }
short TaoRangeArray::RTTI = TAO_RangeArray;
short TaoRangeArray::rtti() const { return RTTI; }
short TaoEnumHash::RTTI = TAO_EnumHash;
short TaoEnumHash::rtti() const { return RTTI; }
short TaoEnumMatrix::RTTI = TAO_EnumMatrix;
short TaoEnumMatrix::rtti() const { return RTTI; }

TaoEnumerate::TaoEnumerate() { infoKit = 0; }
TaoEnumArray::TaoEnumArray() {
  isNumeric = 0;
  array = 0;
  comArray = 0;
  byteArray = 0;
  shortArray = 0;
  intArray = 0;
  floatArray = 0;
  doubleArray = 0;
}
void TaoEnumArray::execute() {
  if (isNumeric) {
    result = makeNumArray();
  } else {
    result = makeArray();
  }
}
TaoBase *TaoEnumArray::makeArray() {
  array = TaoTryAllocator<TaoArray>::tryAllocate(array);
  array->clear();

  for (size_t i = 0; i < rightAriths.size(); i++) {
    rightAriths[i]->compute();
    array->append(rightAriths[i]->getResult());
  }

  return array;
}
TaoBase *TaoEnumArray::makeNumArray() {
  size_t nEnum = rightAriths.size();

  TaoArray tmpArray;
  size_t j = 0;
  for (j = 0; j < nEnum; j++) {
    rightAriths[j]->compute();
    TaoBase *tbase = rightAriths[j]->getResult()->dereference();
    short rtti = tbase->rtti();
    if (rtti == TAO_Array) {
      TaoArray *array = (TaoArray *)tbase;
      for (size_t k = 0; k < array->size(); k++)
        tmpArray.append(array->elements[k]->dereference());
    } else {
      tmpArray.append(tbase);
    }
  }
  vector<TaoBase *> &telem = tmpArray.elements;
  bool isComp = 0;
  for (j = 0; j < tmpArray.size(); j++) {
    short rtti = telem[j]->rtti();
    if (rtti == TAO_Complex || rtti == TAO_CompArray) {
      isComp = 1;
    } else if (rtti != TAO_Number && rtti != TAO_RealArray &&
               rtti != TAO_String) {
      infoKit->warning("element with no conversion to numeric type");
    }
  }

  if (!isComp) {
    if (typeNumeric == TAO_DoubleArray) {
      doubleArray = TaoTryAllocator<TaoDoubleArray>::tryAllocate(doubleArray);
      doubleArray->timeTag = TaoBase::recycler->gcTimes;

      convertValues(telem, doubleArray);

      tmpArray.clear();
      return doubleArray;
    } else if (typeNumeric == TAO_FloatArray) {
      floatArray = TaoTryAllocator<TaoFloatArray>::tryAllocate(floatArray);
      floatArray->timeTag = TaoBase::recycler->gcTimes;

      convertValues(telem, floatArray);

      tmpArray.clear();
      return floatArray;
    } else if (typeNumeric == TAO_ShortArray) {
      shortArray = TaoTryAllocator<TaoShortArray>::tryAllocate(shortArray);
      shortArray->timeTag = TaoBase::recycler->gcTimes;

      convertValues(telem, shortArray);

      tmpArray.clear();
      return shortArray;
    } else if (typeNumeric == TAO_ByteArray) {
      byteArray = TaoTryAllocator<TaoByteArray>::tryAllocate(byteArray);
      byteArray->timeTag = TaoBase::recycler->gcTimes;

      convertValues(telem, byteArray);

      tmpArray.clear();
      return byteArray;
    } else {
      intArray = TaoTryAllocator<TaoIntArray>::tryAllocate(intArray);
      intArray->timeTag = TaoBase::recycler->gcTimes;

      convertValues(telem, intArray);

      tmpArray.clear();
      return intArray;
    }
  } else {
    int k = 0;
    vector<complex<double>> vals;
    for (j = 0; j < telem.size(); j++) {
      short rtti = telem[j]->rtti();
      if (rtti == TAO_Complex) {
        TaoComplex *com = (TaoComplex *)telem[j];
        vals.push_back(com->cmplx);
        k++;
      } else if (rtti == TAO_Number) {
        TaoNumber *num = (TaoNumber *)telem[j];
        vals.push_back(complex<double>(num->value));
        k++;
      } else if (rtti == TAO_CompArray) {
        TaoCompArray<double> *array = (TaoCompArray<double> *)telem[j];
        complex<double> *vec = array->getAsVector();
        for (size_t i = 0; i < array->size(); i++) {
          vals.push_back(vec[i]);
          k++;
        }
      } else {
        vals.push_back(complex<double>());
        k++;
      }
    }
    comArray = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(comArray);
    comArray->timeTag = TaoBase::recycler->gcTimes;

    comArray->resize(k);
    complex<double> *vec = comArray->getAsVector();

    for (int i = 0; i < k; i++)
      vec[i] = vals[i];

    tmpArray.clear();
    return comArray;
  }
}
TaoRangeArray::TaoRangeArray() {}
void TaoRangeArray::execute() {
  if (isNumeric) {
    result = makeNumArray();
  } else {
    result = makeArray();
  }
}
TaoBase *TaoRangeArray::makeArray() {
  array = TaoTryAllocator<TaoArray>::tryAllocate(array);
  array->clear();

  rightAriths[0]->compute();
  short rtti = rightAriths[0]->getResult()->rtti();

  if (rtti == TAO_Number) {
    double first = rightAriths[0]->myValue;
    rightAriths[1]->compute();
    double step = rightAriths[1]->myValue;
    rightAriths[2]->compute();
    for (int i = 0; i < rightAriths[2]->myValue; i++) {
      TaoNumber *num = new TaoNumber(first + i * step);
      array->append(num);
    }
  } else if (rtti == TAO_Complex) {
    complex<double> first = rightAriths[0]->cmplx;
    complex<double> step;
    rightAriths[1]->compute();
    if (rightAriths[1]->getResult()->rtti() == TAO_Complex)
      step = rightAriths[1]->cmplx;
    else
      step = rightAriths[1]->myValue;

    rightAriths[2]->compute();
    for (int i = 0; i < rightAriths[2]->myValue; i++) {
      TaoComplex *com = new TaoComplex(first + step * (double)i);
      array->append(com);
    }
  } else if (rtti == TAO_String) {
    string cs = rightAriths[0]->myChars;
    rightAriths[1]->compute();
    string chars = rightAriths[1]->myChars;
    rightAriths[2]->compute();
    for (int i = 0; i < rightAriths[2]->myValue; i++) {
      TaoString *str = new TaoString(cs);
      array->append(str);
      cs += chars;
    }
  } else {
  }
  return array;
}
TaoBase *TaoRangeArray::makeNumArray() {
  rightAriths[0]->compute();
  short type0 = rightAriths[0]->getResult()->rtti();
  double first = rightAriths[0]->myValue;
  complex<double> cmpfirst = first;
  if (type0 == TAO_Complex)
    cmpfirst = rightAriths[0]->cmplx;

  rightAriths[1]->compute();
  short type1 = rightAriths[1]->getResult()->rtti();
  double step = rightAriths[1]->myValue;
  complex<double> cmpstep = step;
  if (type1 == TAO_Complex)
    cmpstep = rightAriths[1]->cmplx;

  if ((type0 != TAO_Number && type0 != TAO_Complex) ||
      (type1 != TAO_Complex && type1 != TAO_Number)) {
    infoKit->warning("invalid range specification");
    return 0;
  }

  rightAriths[2]->compute();
  int num = (int)rightAriths[2]->myValue;
  if (type0 == TAO_Number && type1 == TAO_Number) {
    if (typeNumeric == TAO_ByteArray) {
      byteArray = TaoTryAllocator<TaoByteArray>::tryAllocate(byteArray);
      byteArray->timeTag = TaoBase::recycler->gcTimes;

      byteArray->resize(num);
      Byte *vec = byteArray->getAsVector();

      for (int i = 0; i < num; i++)
        vec[i] = first + i * step;
      return byteArray;
    } else if (typeNumeric == TAO_ShortArray) {
      shortArray = TaoTryAllocator<TaoShortArray>::tryAllocate(shortArray);
      shortArray->timeTag = TaoBase::recycler->gcTimes;

      shortArray->resize(num);
      short *vec = shortArray->getAsVector();

      for (int i = 0; i < num; i++)
        vec[i] = first + i * step;
      return shortArray;
    } else if (typeNumeric == TAO_IntArray) {
      intArray = TaoTryAllocator<TaoIntArray>::tryAllocate(intArray);
      intArray->timeTag = TaoBase::recycler->gcTimes;

      intArray->resize(num);
      int *vec = intArray->getAsVector();

      for (int i = 0; i < num; i++)
        vec[i] = first + i * step;
      return intArray;
    } else if (typeNumeric == TAO_FloatArray) {
      floatArray = TaoTryAllocator<TaoFloatArray>::tryAllocate(floatArray);
      floatArray->timeTag = TaoBase::recycler->gcTimes;

      floatArray->resize(num);
      float *vec = floatArray->getAsVector();

      for (int i = 0; i < num; i++)
        vec[i] = first + i * step;
      return floatArray;
    } else {
      doubleArray = TaoTryAllocator<TaoDoubleArray>::tryAllocate(doubleArray);
      doubleArray->timeTag = TaoBase::recycler->gcTimes;

      doubleArray->resize(num);
      double *vec = doubleArray->getAsVector();

      for (int i = 0; i < num; i++)
        vec[i] = first + i * step;
      return doubleArray;
    }
  } else {
    comArray = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(comArray);
    comArray->timeTag = TaoBase::recycler->gcTimes;

    comArray->resize(num);
    complex<double> *vec = comArray->getAsVector();

    for (int i = 0; i < num; i++)
      vec[i] = cmpfirst + cmpstep * (double)i;
    return comArray;
  }
}
TaoEnumHash::TaoEnumHash() { hash = 0; }
void TaoEnumHash::execute() {
  hash = TaoTryAllocator<TaoHash>::tryAllocate(hash);
  hash->clear();

  size_t i;
  for (i = 0; i < rightAriths.size(); i++) {
    rightAriths[i]->compute();

    TaoArithBase *keyArith = rightAriths[i]->left;
    TaoArithBase *refArith = rightAriths[i]->right;

    if (!keyArith || !keyArith->getResult() || !refArith ||
        !refArith->getResult())
      continue;

    TaoBase *tkey = keyArith->getResult()->dereference();
    short rtti = tkey->rtti();
    if (rtti == TAO_String) {
      hash->insert(keyArith->myChars, refArith->getResult());
    } else if (rtti == TAO_Array) {
      TaoArray *keyArray = (TaoArray *)tkey;
      TaoBase *tref = refArith->getResult()->dereference();
      if (tref->rtti() == TAO_Array) {
        TaoArray *refArray = (TaoArray *)tref;
        size_t n = keyArray->size();
        if (n > refArray->size())
          n = refArray->size();
        for (size_t i = 0; i < n; i++) {
          TaoString *str = (TaoString *)keyArray->elements[i]->dereference();
          if (str->rtti() == TAO_String) {
            hash->insert(str->chars, refArray->elements[i]);
          } else {
            infoKit->warning("no proper key, ignored");
          }
        }
      } else {
        infoKit->warning("no proper key/value pair, ignored");
      }
    } else {
      infoKit->error_general("key of hash must be a string");
    }
  }
  result = hash;
}
TaoEnumMatrix::TaoEnumMatrix() {
  myMatrix = 0;
  myCotrix = 0;
  nRow = nColumn = 0;
}
void TaoEnumMatrix::execute() {
  bool isComp = 0;
  for (int i = 0; i < nRow; i++) {
    int col = matxAriths[i].size();
    for (int j = 0; j < col; j++) {
      matxAriths[i][j]->compute();
      if (matxAriths[i][j]->getResult()->rtti() == TAO_Complex)
        isComp = 1;
    }
  }
  valarray<size_t> dm(2);
  dm[0] = nRow;
  dm[1] = nColumn;

  if (!isComp) {
    myMatrix = TaoTryAllocator<TaoDoubleArray>::tryAllocate(myMatrix);
    myMatrix->resize(dm);
    int k = 0;
    for (int i = 0; i < nRow; i++) {
      int col = matxAriths[i].size();
      int j;
      for (j = 0; j < col; j++) {
        (*myMatrix)(k) = matxAriths[i][j]->myValue;
        k++;
      }
      for (j = col; j < nColumn; j++)
        k++;
    }
    result = myMatrix;
  } else {
    myCotrix = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(myCotrix);
    myCotrix->resize(dm);
    int k = 0;
    for (int i = 0; i < nRow; i++) {
      int col = matxAriths[i].size();
      int j;
      for (j = 0; j < col; j++) {
        if (matxAriths[i][j]->getResult()->rtti() == TAO_Complex)
          (*myCotrix)(k) = matxAriths[i][j]->cmplx;
        else
          (*myCotrix)(k) = matxAriths[i][j]->myValue;
        k++;
      }
      for (j = col; j < nColumn; j++)
        k++;
    }
    result = myCotrix;
  }
}
