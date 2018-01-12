
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
#include <cstdlib>
#include <cstring>

#include "taoClass.h"
#include "taoDatatype.h"
#include "taoExtdata.h"
#include "taoModule.h"

short TaoBase::RTTI = TAO_Base;
short TaoReference::RTTI = TAO_Reference;
short TaoNumber::RTTI = TAO_Number;
short TaoString::RTTI = TAO_String;
short TaoArray::RTTI = TAO_Array;
short TaoHash::RTTI = TAO_Hash;

TaoRecycler *TaoBase::recycler = 0;

bool TaoString::blQuotate = 0;

using namespace std;

//////////////////////////////////////////////////////////////////

TaoBase::TaoBase() {
  refCount = scanned = visited = locked = 0;
  if (recycler)
    recycler->objectNewed();
}
int TaoBase::getExactType(TaoBase *tbase) {
  if (!tbase)
    return -1;
  int rtti = tbase->rtti();
  if (rtti == TAO_RealArray) {
    TaoArrayBase *array = (TaoArrayBase *)tbase;
    rtti = array->rttiNumeric();
  } else if (rtti == TAO_Class) {
    TaoClass *klass = (TaoClass *)tbase;
    rtti = klass->classid();
  } else if (rtti == TAO_Object) {
    TaoObject *obj = (TaoObject *)tbase;
    rtti = obj->myClass->classid();
  } else if (rtti == TAO_CppObject) {
    TaoCppObject *obj = (TaoCppObject *)tbase;
    rtti = obj->cppObject->rtti() + 10000;
  }
  return rtti;
}
TaoBase *TaoBase::fromCppType(TcNumber *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoNumber *num = new TaoNumber(tc->getValue());
  tc->reserved = (void *)num;
  return num;
}
TaoBase *TaoBase::fromCppType(TcComplex *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoComplex *com = new TaoComplex(tc->getReal(), tc->getImag());
  tc->reserved = (void *)com;
  return com;
}
TaoBase *TaoBase::fromCppType(TcString *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoString *str = new TaoString;
  str->chars = tc->getChars();
  tc->reserved = (void *)str;
  return str;
}
TaoBase *TaoBase::fromCppType(TcArray *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoArray *array = new TaoArray;
  tc->reserved = (void *)array;
  for (int i = 0; i < tc->size(); i++) {
    TcBase *tbase = tc->getElement(i);
    short rtti = tbase->rtti();
    if (rtti == TC_NUMBER)
      array->append(fromCppType((TcNumber *)tbase));
    else if (rtti == TC_COMPLEX)
      array->append(fromCppType((TcComplex *)tbase));
    else if (rtti == TC_STRING)
      array->append(fromCppType((TcString *)tbase));
    else if (rtti == TC_ARRAY)
      array->append(fromCppType((TcArray *)tbase));
    else if (rtti == TC_HASH)
      array->append(fromCppType((TcHash *)tbase));
    else if (tbase->type() == TAO_PLUGIN)
      array->append(fromCppType((TaoPlugin *)tbase));
    else if (rtti == TC_ARRAY_BYTE)
      array->append(fromCppType((TcByteArray *)tbase));
    else if (rtti == TC_ARRAY_SHORT)
      array->append(fromCppType((TcShortArray *)tbase));
    else if (rtti == TC_ARRAY_INT)
      array->append(fromCppType((TcIntArray *)tbase));
    else if (rtti == TC_ARRAY_FLOAT)
      array->append(fromCppType((TcFloatArray *)tbase));
    else if (rtti == TC_ARRAY_DOUBLE)
      array->append(fromCppType((TcDoubleArray *)tbase));
    else if (rtti == TC_ARRAY_COMPLEX)
      array->append(fromCppType((TcComplexArray *)tbase));
  }
  return array;
}
TaoBase *TaoBase::fromCppType(TcHash *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoHash *hash = new TaoHash;
  tc->reserved = (void *)hash;
  for (int i = 0; i < tc->size(); i++) {
    TcBase *tbase = tc->getValue(i);
    string key = tc->getKey(i);
    short rtti = tbase->rtti();
    if (rtti == TC_NUMBER)
      hash->insert(key, fromCppType((TcNumber *)tbase));
    else if (rtti == TC_COMPLEX)
      hash->insert(key, fromCppType((TcComplex *)tbase));
    else if (rtti == TC_STRING)
      hash->insert(key, fromCppType((TcString *)tbase));
    else if (rtti == TC_ARRAY)
      hash->insert(key, fromCppType((TcArray *)tbase));
    else if (rtti == TC_HASH)
      hash->insert(key, fromCppType((TcHash *)tbase));
    else if (tbase->type() == TAO_PLUGIN)
      hash->insert(key, fromCppType((TaoPlugin *)tbase));
    else if (rtti == TC_ARRAY_BYTE)
      hash->insert(key, fromCppType((TcByteArray *)tbase));
    else if (rtti == TC_ARRAY_SHORT)
      hash->insert(key, fromCppType((TcShortArray *)tbase));
    else if (rtti == TC_ARRAY_INT)
      hash->insert(key, fromCppType((TcIntArray *)tbase));
    else if (rtti == TC_ARRAY_FLOAT)
      hash->insert(key, fromCppType((TcFloatArray *)tbase));
    else if (rtti == TC_ARRAY_DOUBLE)
      hash->insert(key, fromCppType((TcDoubleArray *)tbase));
    else if (rtti == TC_ARRAY_COMPLEX)
      hash->insert(key, fromCppType((TcComplexArray *)tbase));
  }
  return hash;
}
TaoBase *TaoBase::fromCppType(TaoPlugin *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoCppObject *obj = new TaoCppObject;
  obj->cppObject = tc;
  return obj;
}
TaoBase *TaoBase::fromCppType(TcByteArray *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoByteArray *array = new TaoByteArray;
  int ndim = tc->dim();
  int *dims = tc->getDims();
  valarray<size_t> dm(ndim);
  for (int i = 0; i < ndim; i++)
    dm[i] = dims[i];
  array->setNumArray(tc->getAsVector(), dm);
  return array;
}
TaoBase *TaoBase::fromCppType(TcShortArray *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoShortArray *array = new TaoShortArray;
  int ndim = tc->dim();
  int *dims = tc->getDims();
  valarray<size_t> dm(ndim);
  for (int i = 0; i < ndim; i++)
    dm[i] = dims[i];
  array->setNumArray(tc->getAsVector(), dm);
  return array;
}
TaoBase *TaoBase::fromCppType(TcIntArray *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoIntArray *array = new TaoIntArray;
  int ndim = tc->dim();
  int *dims = tc->getDims();
  valarray<size_t> dm(ndim);
  for (int i = 0; i < ndim; i++)
    dm[i] = dims[i];
  array->setNumArray(tc->getAsVector(), dm);
  return array;
}
TaoBase *TaoBase::fromCppType(TcFloatArray *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoFloatArray *array = new TaoFloatArray;
  int ndim = tc->dim();
  int *dims = tc->getDims();
  valarray<size_t> dm(ndim);
  for (int i = 0; i < ndim; i++)
    dm[i] = dims[i];
  array->setNumArray(tc->getAsVector(), dm);
  return array;
}
TaoBase *TaoBase::fromCppType(TcDoubleArray *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoDoubleArray *array = new TaoDoubleArray;
  int ndim = tc->dim();
  int *dims = tc->getDims();
  valarray<size_t> dm(ndim);
  for (int i = 0; i < ndim; i++)
    dm[i] = dims[i];
  array->setNumArray(tc->getAsVector(), dm);
  return array;
}
TaoBase *TaoBase::fromCppType(TcComplexArray *tc) {
  if (tc->reserved)
    return (TaoBase *)tc->reserved;
  TaoCompArray<double> *array = new TaoCompArray<double>;
  int ndim = tc->dim();
  int *dims = tc->getDims();
  valarray<size_t> dm(ndim);
  for (int i = 0; i < ndim; i++)
    dm[i] = dims[i];
  array->resize(dm);
  complex<double> *vec = array->getAsVector();
  for (int k = 0; k < (int)array->size(); k++) {
    vec[k] = complex<double>(tc->real(k), tc->imag(k));
  }
  return array;
}

////////////////////////////////////////////////////////////////

TaoBase *TaoReference::duplicate() {
  TaoReference *pnew = this;
  if (!visited) {
    visited = 1;
    pnew = newObject();
    if (content) {
      pnew->content = content->duplicate();
      pnew->content->refCount++;
    }
  }
  visited = 0;
  return (TaoBase *)pnew;
}

TaoBase *TaoReference::dereference() {
  if (!content)
    return 0;

  TaoBase *p = content;
  short type = content->rtti();
  while (type == TAO_Reference) {
    p = ((TaoReference *)p)->content;
    if (!p)
      break;
    type = p->rtti();
  }
  return p;
}
void TaoReference::print(ostream *out) {
  if (visited) {
    *out << "...";
  } else if (content) {
    visited = 1;
    content->print(out);
  }
  visited = 0;
}

///////////////////////////////////////////////////

TcBase *TaoNumber::toCppType() {
  if (!cpptype)
    cpptype = new TcNumber;
  cpptype->setValue(value);
  cpptype->reserved = this;

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoNumber::fromCppType() {
  if (!cpptype)
    return;
  value = cpptype->getValue();

  if (recycler)
    recycler->pushTcObject(cpptype);
  cpptype = 0;
}

TaoString::TaoString() {
  cpptype = 0;
  tstring = 0;
  start = end = -1;
}
TaoString::TaoString(const string &chs) {
  cpptype = 0;
  chars = chs;
  start = end = -1;
  tstring = 0;
}
TcBase *TaoString::toCppType() {
  if (!cpptype)
    cpptype = new TcString;
  cpptype->setChars(chars.c_str());
  cpptype->reserved = this;

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoString::fromCppType() {
  if (!cpptype)
    return;
  chars = cpptype->getChars();

  if (recycler)
    recycler->pushTcObject(cpptype);
  cpptype = 0;
}

void TaoString::print(ostream *out) {
  if (blQuotate)
    *out << "\"";
  *out << chars;
  if (blQuotate)
    *out << "\"";
}
void TaoString::assignWith(TaoBase *tbase) {
  if (tbase->rtti() != TAO_String)
    return;
  if (start < 0)
    return;
  TaoString *str = (TaoString *)tbase;
  if (end >= start)
    chars.replace(start, end - start + 1, str->chars);
  else
    chars.insert(start, str->chars);
}
void TaoString::setSubIndex(TaoArithBase *arith, TaoInfoKit *infoKit) {
  start = 0;
  end = chars.size() - 1;
  if (arith->rtti() == TAO_PairArith) {
    if (arith->left && arith->left->rtti() != TAO_NullArith)
      start = (int)arith->left->myValue;
    if (arith->right && arith->right->rtti() != TAO_NullArith)
      end = (int)arith->right->myValue;
  } else {
    start = end = (int)arith->myValue;
  }
}
TaoBase *TaoString::subIndexing(TaoInfoKit *infoKit) {
  tstring = TaoTryAllocator<TaoString>::tryAllocate(tstring);
  if (start >= 0 && end >= start)
    tstring->chars = chars.substr(start, end - start + 1);
  else
    tstring->chars.erase();
  return tstring;
}
//////////////////////////////////////////////////

TaoArray::TaoArray(int num) {
  cpptype = 0;
  asArray = 0;
  whichIndex = 0;
  subArray = 0;
  number = 0;
  tstring = 0;
  complx = 0;
  elements.resize(num);
}
TaoArray::~TaoArray() { elements.clear(); }
TcBase *TaoArray::toCppType() {
  if (cpptype)
    return cpptype; // In this case, cpptype is not null.

  cpptype = new TcArray;
  cpptype->reserved = this;

  for (size_t i = 0; i < elements.size(); i++)
    cpptype->insert(elements[i]->toCppType());

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoArray::fromCppType() {
  if (visited || !cpptype)
    return;
  visited = 1;

  clear();
  for (int i = 0; i < cpptype->size(); i++) {
    TcBase *tbase = cpptype->getElement(i);
    short rtti = tbase->rtti();
    if (rtti == TC_NUMBER)
      append(TaoBase::fromCppType((TcNumber *)tbase));
    else if (rtti == TC_COMPLEX)
      append(TaoBase::fromCppType((TcComplex *)tbase));
    else if (rtti == TC_STRING)
      append(TaoBase::fromCppType((TcString *)tbase));
    else if (rtti == TC_ARRAY)
      append(TaoBase::fromCppType((TcArray *)tbase));
    else if (rtti == TC_HASH)
      append(TaoBase::fromCppType((TcHash *)tbase));
    else if (tbase->type() == TAO_PLUGIN)
      append(TaoBase::fromCppType((TaoPlugin *)tbase));
    else if (rtti == TC_ARRAY_BYTE)
      append(TaoBase::fromCppType((TcByteArray *)tbase));
    else if (rtti == TC_ARRAY_SHORT)
      append(TaoBase::fromCppType((TcShortArray *)tbase));
    else if (rtti == TC_ARRAY_INT)
      append(TaoBase::fromCppType((TcIntArray *)tbase));
    else if (rtti == TC_ARRAY_FLOAT)
      append(TaoBase::fromCppType((TcFloatArray *)tbase));
    else if (rtti == TC_ARRAY_DOUBLE)
      append(TaoBase::fromCppType((TcDoubleArray *)tbase));
    else if (rtti == TC_ARRAY_COMPLEX)
      append(TaoBase::fromCppType((TcComplexArray *)tbase));
  }

  for (size_t j = 0; j < elements.size(); j++)
    elements[j]->fromCppType();

  if (recycler)
    recycler->pushTcObject(cpptype);

  visited = 0;
  cpptype = 0;
}
TaoBase *TaoArray::duplicate() {
  TaoArray *pnew = new TaoArray();
  for (unsigned int i = 0; i < elements.size(); i++) {
    TaoBase *pbase = elements[i]->duplicate();
    pbase->refCount++;
    pnew->elements.push_back(pbase);
  }

  return (TaoBase *)pnew;
}
void TaoArray::append(TaoBase *pbase) {
  if (pbase) {
    elements.push_back(pbase);
    pbase->refCount++;
  }
}
void TaoArray::insert(TaoBase *pbase, int pos) {
  if (pbase) {
    elements.insert(elements.begin() + pos, pbase);
    pbase->refCount++;
  }
}
void TaoArray::replace(TaoBase *pbase, int pos) {
  if (recycler)
    recycler->pushRootCandidate(elements[pos]);
  if (pbase) {
    elements[pos] = pbase;
    pbase->refCount++;
  }
}
void TaoArray::erase(int pos) {
  if (recycler)
    recycler->pushRootCandidate(elements[pos]);
  elements.erase(elements.begin() + pos);
  subIndex.resize(0);
}
void TaoArray::clear() {
  for (unsigned int i = 0; i < elements.size(); i++)
    if (recycler)
      recycler->pushRootCandidate(elements[i]);

  elements.clear();
  subIndex.resize(0);
}
void TaoArray::exportElements(TaoArray *array) {
  for (unsigned int i = 0; i < elements.size(); i++) {
    if (elements[i]) {
      TaoBase *tbase = elements[i]->dereference();
      short rtti = tbase->rtti();
      if (rtti == TAO_Array) {
        TaoArray *v = (TaoArray *)tbase;
        v->exportElements(array);
      } else {
        array->append(elements[i]);
      }
    }
  }
}
void TaoArray::setSubIndex(TaoArithBase *arith, TaoInfoKit *infoKit) {
  TaoIndexMaker::makeIndex(subIndex, elements.size(), arith, &asArray, infoKit);
}
TaoBase *TaoArray::subIndexing(TaoInfoKit *infoKit) {
  if (subIndex.size() == 0) {
    subArray = TaoTryAllocator<TaoArray>::tryAllocate(subArray);
    subArray->clear();
    return subArray;
  } else if (subIndex.size() == 1) {
    if (subIndex[0] >= elements.size()) {
      infoKit->warning("index out of range");
      return 0;
    }
    if (asArray) {
      subArray = TaoTryAllocator<TaoArray>::tryAllocate(subArray);
      subArray->clear();
      subArray->append(elements[subIndex[0]]);
      return subArray;
    }
    return elements[subIndex[0]];
  } else {
    subArray = TaoTryAllocator<TaoArray>::tryAllocate(subArray);
    subArray->clear();

    for (size_t i = 0; i < subIndex.size(); i++)
      subArray->append(elements[subIndex[i]]);
    return subArray;
  }
}
void TaoArray::assignWith(TaoBase *tbase) {
  for (size_t i = 0; i < subIndex.size(); i++)
    replace(tbase, subIndex[i]);
}
bool TaoArray::compare(TaoBase *one, TaoBase *two, TaoArithBase *arith,
                       vector<TaoReference *> &transient, TaoInfoKit *infoKit) {
  transient[0]->content = one;
  transient[1]->content = two;
  arith->compute();
  return arith->bulean;
}
// See books:
// Numerical Recipes in C
// Data Structure and Algorithms in C++
void TaoArray::heapSort(TaoArithBase *arith, int nsort,
                        vector<TaoReference *> &transient,
                        TaoInfoKit *infoKit) {
  int N = elements.size();
  if (N < 2)
    return;

  int last; // index of the last unsorted elements.
  int i, j, m;
  TaoBase *tbase = 0;

  if (nsort > N || nsort <= 0)
    nsort = 0;
  else
    nsort = N - nsort - 1;

  m = N / 2;
  last = N - 1;
  while (1) {
    if (m > 0) {
      // Initial transforming array into heap:
      tbase = elements[--m];
    } else {
      // Move the heap root to the last:
      tbase = elements[last];
      elements[last] = elements[0];
      if (--last == nsort) {
        elements[0] = tbase;
        break;
      }
    }
    i = m;
    // Since index starts from 0 !
    // The children of "m" is "2m+1" and "2m+2":
    j = m + m + 1;
    // Restore heap structure of the unsorted elements:
    // Find the propriate position for "tbase":
    while (j <= last) {
      // Compare two children:
      if (j < last &&
          compare(elements[j], elements[j + 1], arith, transient, infoKit))
        j++;

      if (compare(tbase, elements[j], arith, transient, infoKit)) {
        // Promote elements[j];
        elements[i] = elements[j];
        i = j;
        // To check the children of "j":
        j += j + 1; // Since index starts from 0 !
      } else
        break;
    }
    // Swap down "tbase":
    elements[i] = tbase;
  }
}
TaoBase *TaoArray::basicMathFunct(TaoInfoKit *infoKit, short type) {
  if (subIndex.size() == 0) {
    subIndex.resize(elements.size());
    for (size_t i = 0; i < elements.size(); i++)
      subIndex[i] = i;
  }
  short rtti = elements[subIndex[0]]->rtti();
  bool same = 1;
  if (rtti == TAO_Number) {
    TaoNumber *first = (TaoNumber *)elements[subIndex[0]];
    double max = first->value;
    double min = first->value;
    double sum = first->value;
    int count = 1;
    for (size_t i = 1; i < subIndex.size(); i++) {
      TaoNumber *num = (TaoNumber *)elements[subIndex[i]];
      // cast first, then check if it's right:
      if (num->rtti() == TAO_Number) {
        if (max < num->value)
          max = num->value;
        if (min > num->value)
          min = num->value;
        count++;
        sum += num->value;
      } else {
        same = 0;
      }
    }
    if (!same)
      infoKit->warning("max() is applied to heterogenous array, elements "
                       "different from the first one are ignored.\n");
    if (!number) {
      number = new TaoNumber;
      number->locked = 1;
    } else if (number->refCount > 0) {
      number->locked = 0;
      number = new TaoNumber;
      number->locked = 1;
    }
    if (type == TAO_max)
      number->value = max;
    else if (type == TAO_min)
      number->value = min;
    else if (type == TAO_mean)
      number->value = sum / count;
    else if (type == TAO_sum)
      number->value = sum;
    return number;
  } else if (rtti == TAO_String) {
    if (type == 3) {
      infoKit->warning("mean() is undefined for string\n");
      return elements[subIndex[0]];
    }
    TaoString *first = (TaoString *)elements[subIndex[0]];
    string max = first->chars;
    string min = first->chars;
    string sum = first->chars;
    int count = 1;
    for (size_t i = 1; i < subIndex.size(); i++) {
      TaoString *str = (TaoString *)elements[subIndex[i]];
      // cast first, then check if it's right:
      if (str->rtti() == TAO_String) {
        if (max < str->chars)
          max = str->chars;
        if (min > str->chars)
          min = str->chars;
        count++;
        sum += str->chars;
      } else {
        same = 0;
      }
    }
    if (!same)
      infoKit->warning("max()/min()/mean()/sum() is applied to heterogenous "
                       "array, elements different from the first one are "
                       "ignored.\n");
    if (!tstring) {
      tstring = new TaoString;
      tstring->locked = 1;
    } else if (tstring->refCount > 0) {
      tstring->locked = 0;
      tstring = new TaoString;
      tstring->locked = 1;
    }
    if (type == TAO_max)
      tstring->chars = max;
    else if (type == TAO_min)
      tstring->chars = min;
    else if (type == TAO_sum)
      tstring->chars = sum;
    return tstring;
  } else if (rtti == TAO_Complex) {
    if (type == 3) {
      infoKit->warning("max/min() is undefined for complex\n");
      return elements[subIndex[0]];
    }
    TaoComplex *first = (TaoComplex *)elements[subIndex[0]];
    complex<double> sum = first->cmplx;
    double count = 1;
    for (size_t i = 1; i < subIndex.size(); i++) {
      TaoComplex *com = (TaoComplex *)elements[subIndex[i]];
      // cast first, then check if it's right:
      if (com->rtti() == TAO_Complex) {
        count++;
        sum += com->cmplx;
      } else {
        same = 0;
      }
    }
    if (!same)
      infoKit->warning("max() is applied to heterogenous array, elements "
                       "different from the first one are ignored.\n");
    if (!complx) {
      complx = new TaoComplex;
      complx->locked = 1;
    } else if (complx->refCount > 0) {
      complx->locked = 0;
      complx = new TaoComplex;
      complx->locked = 1;
    }
    if (type == TAO_mean)
      complx->cmplx = sum / count;
    else if (type == TAO_sum)
      complx->cmplx = sum;
    return complx;
  } else {
    infoKit->warning("method is undefined for such type\n");
    return elements[subIndex[0]];
  }
  subIndex.resize(0);
}

//! not complete!
TaoIntArray *TaoArray::whichByOper(char oper, TaoBase *tbase) {
  if (subIndex.size() == 0) {
    subIndex.resize(elements.size());
    for (size_t i = 0; i < elements.size(); i++)
      subIndex[i] = i;
  }
  // short rtb=tbase->rtti();
  return 0;
}
TaoIntArray *TaoArray::whichByArith(TaoArithBase *blArith,
                                    vector<TaoReference *> &varTrans) {
  subIndex.resize(0);
  if (subIndex.size() == 0) {
    subIndex.resize(elements.size());
    for (size_t i = 0; i < elements.size(); i++)
      subIndex[i] = i;
  }
  vector<vector<int>> ids;
  TaoNumber *idtemp = new TaoNumber;
  idtemp->refCount = 1;
  varTrans[1]->content = idtemp;
  vector<int> one;
  for (size_t i = 0; i < subIndex.size(); i++) {
    varTrans[0]->content = elements[subIndex[i]];
    idtemp = TaoTryAllocator<TaoNumber>::tryAllocate(idtemp);
    varTrans[1]->content = idtemp;
    idtemp->value = subIndex[i];
    blArith->compute();
    if (blArith->bulean) {
      one.push_back(subIndex[i]);
    }
  }
  ids.push_back(one);
  return TaoIndexMaker::whichIndexing(ids, whichIndex);
}

void TaoArray::print(ostream *out) {
  if (visited) {
    *out << "{...}";
    return;
  }
  visited = 1;
  *out << "{ ";

  int size = elements.size();
  for (int i = 0; i < size; i++) {
    TaoString::setOnQuotate(1);
    elements[i]->print(out);
    TaoString::setOnQuotate(0);
    if (i != size - 1)
      *out << ", ";
  }
  *out << " }";
  visited = 0;
}
///////////////////////////////////////////////////

TcBase *TaoHash::toCppType() {
  if (cpptype)
    return cpptype; // In this case, cpptype is not null.

  cpptype = new TcHash;
  cpptype->reserved = this;

  map<string, TaoBase *>::iterator iter;
  for (iter = elements.begin(); iter != elements.end(); iter++)
    cpptype->insert((char *)iter->first.c_str(), iter->second->toCppType());

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoHash::fromCppType() {
  if (visited || !cpptype)
    return;
  visited = 1;

  clear();
  for (int i = 0; i < cpptype->size(); i++) {
    TcBase *tbase = cpptype->getValue(i);
    string key = cpptype->getKey(i);
    short rtti = tbase->rtti();
    if (rtti == TC_NUMBER)
      insert(key, TaoBase::fromCppType((TcNumber *)tbase));
    else if (rtti == TC_COMPLEX)
      insert(key, TaoBase::fromCppType((TcComplex *)tbase));
    else if (rtti == TC_STRING)
      insert(key, TaoBase::fromCppType((TcString *)tbase));
    else if (rtti == TC_ARRAY)
      insert(key, TaoBase::fromCppType((TcArray *)tbase));
    else if (rtti == TC_HASH)
      insert(key, TaoBase::fromCppType((TcHash *)tbase));
    else if (tbase->type() == TAO_PLUGIN)
      insert(key, TaoBase::fromCppType((TaoPlugin *)tbase));
    else if (rtti == TC_ARRAY_BYTE)
      insert(key, TaoBase::fromCppType((TcByteArray *)tbase));
    else if (rtti == TC_ARRAY_SHORT)
      insert(key, TaoBase::fromCppType((TcShortArray *)tbase));
    else if (rtti == TC_ARRAY_INT)
      insert(key, TaoBase::fromCppType((TcIntArray *)tbase));
    else if (rtti == TC_ARRAY_FLOAT)
      insert(key, TaoBase::fromCppType((TcFloatArray *)tbase));
    else if (rtti == TC_ARRAY_DOUBLE)
      insert(key, TaoBase::fromCppType((TcDoubleArray *)tbase));
    else if (rtti == TC_ARRAY_COMPLEX)
      insert(key, TaoBase::fromCppType((TcComplexArray *)tbase));
  }

  map<string, TaoBase *>::iterator iter;
  for (iter = elements.begin(); iter != elements.end(); iter++)
    iter->second->fromCppType();

  if (recycler)
    recycler->pushTcObject(cpptype);

  visited = 0;
  cpptype = 0;
}
TaoBase *TaoHash::duplicate() {
  TaoHash *pnew = new TaoHash();
  /*
  pnew->elements=elements;
  for(size_t i=0;i<elements.size();i++){
          TaoBase*pbase=elements[i]->duplicate();
          pbase->refCount++;
          pnew->elements.push_back(pbase);
  }
  */

  return (TaoBase *)pnew;
}
void TaoHash::insert(const string &key, TaoBase *pbase) {
  if (!pbase)
    return;
  map<string, TaoBase *>::iterator iter = elements.find(key);
  if (iter == elements.end()) {
    elements[key] = pbase;
    pbase->refCount++;
  } else {
    pbase->refCount++;

    if (recycler)
      recycler->pushRootCandidate(iter->second);
    iter->second = pbase;
  }
}
void TaoHash::clear() {
  map<string, TaoBase *>::iterator iter;
  for (iter = elements.begin(); iter != elements.end(); iter++)
    if (recycler)
      recycler->pushRootCandidate(iter->second);

  elements.clear();
}
void TaoHash::resetSubIndex() {
  keyLower.erase();
  keyUpper.erase();
  asHash = 0;
}
void TaoHash::setSubIndex(TaoArithBase *arith, TaoInfoKit *infoKit) {
  keyLower.erase();
  keyUpper.erase();
  if (arith->rtti() == TAO_PairArith) {
    asHash = 1;
    if (arith->left && arith->left->rtti() != TAO_NullArith) {
      TaoBase *tbase = arith->left->getResult();
      if (tbase)
        tbase = tbase->dereference();
      short rtti = -1;
      if (tbase)
        rtti = tbase->rtti();
      if (rtti == TAO_String)
        keyLower = ((TaoString *)tbase)->chars;
    }
    if (arith->right && arith->right->rtti() != TAO_NullArith) {
      TaoBase *tbase = arith->right->getResult();
      if (tbase)
        tbase = tbase->dereference();
      short rtti = -1;
      if (tbase)
        rtti = tbase->rtti();
      if (rtti == TAO_String)
        keyUpper = ((TaoString *)tbase)->chars;
    }
  } else {
    asHash = 0;
    TaoBase *tbase = arith->getResult();
    if (tbase)
      tbase = tbase->dereference();
    short rtti = -1;
    if (tbase)
      rtti = tbase->rtti();
    if (rtti == TAO_String) {
      keyLower = ((TaoString *)tbase)->chars;
      keyUpper = keyLower;
    }
  }
}
TaoBase *TaoHash::subIndexing(TaoInfoKit *infoKit) {
  if (keyLower.size() == 0) {
    subHash = TaoTryAllocator<TaoHash>::tryAllocate(subHash);
    subHash->clear();
    map<string, TaoBase *>::iterator upkey = elements.find(keyUpper);
    if (upkey == elements.end()) {
      upkey = elements.upper_bound(keyUpper);
    } else {
      upkey++;
    }
    map<string, TaoBase *>::iterator iter;
    for (iter = elements.begin(); iter != upkey; iter++) {
      if (iter == elements.end())
        break;
      subHash->insert(iter->first, iter->second);
    }
    return subHash;
  } else if (keyUpper.size() == 0) {
    subHash = TaoTryAllocator<TaoHash>::tryAllocate(subHash);
    subHash->clear();
    map<string, TaoBase *>::iterator lowkey = elements.find(keyLower);
    if (lowkey == elements.end())
      lowkey = elements.upper_bound(keyLower);

    map<string, TaoBase *>::iterator iter;
    if (lowkey != elements.end()) {
      for (iter = lowkey; iter != elements.end(); iter++)
        subHash->insert(iter->first, iter->second);
    }
    return subHash;
  } else if (keyLower == keyUpper) {
    map<string, TaoBase *>::iterator iter = elements.find(keyUpper);
    if (iter != elements.end()) {
      if (asHash) {
        subHash = TaoTryAllocator<TaoHash>::tryAllocate(subHash);
        subHash->clear();
        subHash->insert(iter->first, iter->second);
        return subHash;
      } else {
        return iter->second;
      }
    } else
      return 0;
  } else {
    subHash = TaoTryAllocator<TaoHash>::tryAllocate(subHash);
    subHash->clear();
    map<string, TaoBase *>::iterator lowkey = elements.find(keyLower);
    map<string, TaoBase *>::iterator upkey = elements.find(keyUpper);
    if (lowkey == elements.end())
      lowkey = elements.upper_bound(keyLower);
    if (upkey == elements.end()) {
      upkey = elements.upper_bound(keyUpper);
    } else {
      upkey++;
    }
    map<string, TaoBase *>::iterator iter;
    for (iter = lowkey; iter != upkey; iter++) {
      if (iter == elements.end())
        break;
      subHash->insert(iter->first, iter->second);
    }
    return subHash;
  }
}
void TaoHash::assignWith(TaoBase *tbase) {
  if (keyLower.size() == 0) {
    map<string, TaoBase *>::iterator iter;
    for (iter = elements.find(keyLower); iter != elements.end(); iter++)
      subHash->insert(iter->first, tbase);
  } else if (keyUpper.size() == 0) {
    map<string, TaoBase *>::iterator upkey = elements.find(keyUpper);
    upkey++;
    map<string, TaoBase *>::iterator iter;
    for (iter = elements.begin(); iter != upkey; iter++)
      subHash->insert(iter->first, tbase);
  } else if (keyLower == keyUpper) {
    insert(keyUpper, tbase);
  } else {
    map<string, TaoBase *>::iterator lowkey = elements.find(keyLower);
    map<string, TaoBase *>::iterator upkey = elements.find(keyUpper);
    map<string, TaoBase *>::iterator iter;
    upkey++;
    for (iter = lowkey; iter != upkey; iter++)
      subHash->insert(iter->first, tbase);
  }
}
void TaoHash::print(ostream *out) {
  if (visited) {
    *out << "{...}";
    return;
  }
  visited = 1;
  *out << "{ ";

  map<string, TaoBase *>::iterator iter = elements.begin();
  while (iter != elements.end()) {
    *out << "\"" << iter->first << "\"=>";
    TaoString::setOnQuotate(1);
    iter->second->print(out);
    TaoString::setOnQuotate(0);
    if ((++iter) != elements.end())
      *out << ", ";
  }

  *out << " }";
  visited = 0;
}
