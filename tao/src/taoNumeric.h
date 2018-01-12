
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
#ifndef TAO_NUMERIC_H
#define TAO_NUMERIC_H

#include <algorithm>
#include <complex>
#include <map>
#include <string>
#include <valarray>
#include <vector>

#include "taoArith.h"
#include "taoConst.h"
#include "taoDatatype.h"
#include "taoRecycler.h"
#include "taoUtility.h"

// class TaoBase;
// class TaoChain;
// class TaoArithBase;

extern double mypow(double x, double y);
extern int xBaseInteger(string &chs, int xbase, TaoInfoKit *infoKit);
extern double xBaseDecimal(string &chs, int xbase, TaoInfoKit *infoKit);
extern double xBaseNumber(string &chs, int xbase, TaoInfoKit *infoKit);

enum TaoRealArrayTypes {
  TAO_ByteArray = 1,
  TAO_ShortArray = 2,
  TAO_IntArray = 3,
  TAO_FloatArray = 4,
  TAO_DoubleArray = 5
};

class TaoComplex : public TaoBase {
  TcComplex *cpptype;

public:
  TaoComplex(double real = 0, double imag = 0);
  TaoComplex(const complex<double> &);

  complex<double> cmplx;

  static short RTTI;
  virtual short rtti() const;
  void print(ostream *out = &cout);

  TaoBase *duplicate();
  TcBase *toCppType();
  void fromCppType();
};

template <class _Tp> class TaoRealArray;

class TaoIntArray;

class TaoArrayBase : public TaoBase {
protected:
  friend class TaoArithBase;
  friend class TaoChain;

  virtual TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array) {
    return array;
  }

  // Indices for subindexing:
  // numarray[a][b][c]
  // index[0] stores indices represented by a.
  // index[1] stores indices represented by b...
  valarray<valarray<size_t>> subIndex;
  void makeSubDims(valarray<size_t> &ds);

  // Indices which has been mapped to 1-D array.
  valarray<size_t> idMapped;
  //
  bool blIdMapped;

  bool onAllElements;

  // Number of total elements in subarray:
  size_t numIndexElem;
  // Number of dimensions of the subarray;
  size_t numDimSub;

  // Dimensions of the numarray:
  valarray<size_t> dims;

  TaoIntArray *whichIndex;

  // Accumulated dims for slicing:
  // numarray[a][b][c]
  // dimAccum[0] #b * #c;
  // dimAccum[1] #c;
  // dimAccum[2] 1;
  valarray<size_t> dimAccum;

public:
  TaoArrayBase();
  ~TaoArrayBase();

  //! A time tag to tell when this array is recently used,
  //! for memory recycling purpose.
  //! Because each numeric array holds an index array _whichIndex_
  //! and a sub-array _subArray_ for efficiency purpose ( avoiding
  //! frequent memory allocation in subindexing operations ). It may
  //! happen that an array is not used for long time though refCount>0,
  //! with a huge _subArray_, in this case, its size can be resized
  //! to 0 to free memory and reallocate it when next time it's used.
  //!    If abs( timeTag-TaoRecycler::gcTimes )>1000000,
  //!    subArray->resize(0); whichIndex->resize(0);
  int timeTag;

  size_t size() const { return dimAccum[0] * dims[0]; }
  size_t dim() const { return dims.size(); }
  size_t dim(size_t i) const { return dims[i]; }

  void updateDimAccum();
  valarray<size_t> &getDims() { return dims; }

  void resetSubIndex();
  void setSubIndex(vector<TaoArithBase *> &idArith, TaoInfoKit *infoKit);
  // Map "index" to "idMapped":
  void mapIndex();

  size_t getFlatIndex(valarray<int> &ids, valarray<size_t> &dimAcc);
  size_t getFlatIndex(valarray<int> &ids);

  void simplifyDims();
  void reshape(valarray<size_t> &newDims, TaoInfoKit *infoKit);

  // Resize the internal "numArray" by the sub-classes
  virtual void resizeVector(size_t size, size_t oldSize){};
  virtual void resize(size_t size);
  virtual void resize(valarray<size_t> &newDims);

  size_t count() { return numIndexElem; }
  TaoIntArray *dimSize();

  bool alignShape(valarray<size_t> &dms);

  void numFuncApply(short funtype, TaoInfoKit *infoKit);
  void comFuncApply(short funtype, TaoInfoKit *infoKit);
  void numIterate(TaoArithBase *, vector<TaoReference *> &, bool, TaoInfoKit *);
  void comIterate(TaoArithBase *, vector<TaoReference *> &, bool, TaoInfoKit *);
  // To get which elements satisfy a condition,
  // return an array of subindices:
  TaoIntArray *numWhich(TaoArithBase *, vector<TaoReference *> &);
  TaoIntArray *comWhich(TaoArithBase *, vector<TaoReference *> &);

  // This function should ONLY be used by class TaoBase and
  // TaoChain for arithmetic compuation purpose. It returns,
  // depending the sub-index types, either a pointer to the whole
  // numArray, or a sub-numArray (so far only single row or column),
  // or one element wrapped as a sub-class of TaoBase:
  virtual TaoBase *subIndexing() { return 0; }

  // Used for assignment:
  virtual void assignWith(double &val, char oper){};
  virtual void assignWith(complex<double> &val, char oper){};
  virtual double getDouble(size_t i) { return 0; }
  virtual complex<double> getComplex(size_t i) { return 0; }
  virtual void setDouble(size_t i, double value){};
  virtual void setComplex(size_t i, complex<double> value){};

  virtual void print_element(size_t i, ostream *out = &cout);
  void print(ostream *out = &cout);

  void exportSubIndex(valarray<int> &index, size_t id);
  void exportAllSubIndex(valarray<valarray<int>> &index);
  virtual short rttiNumeric() const { return 0; }
  virtual void exportAsIndex(valarray<size_t> &index){};

  // This function must be called after mapIndex():
  virtual TaoBase *basicMathFunct(TaoInfoKit *infoKit, short type = 1) {
    return 0;
  }
  // To get which elements satisfy a condition,
  // return an array of subindices:
  virtual TaoIntArray *numWhichByOper(char oper, double value) { return 0; }

  virtual TcBase *toCppType() { return 0; }
  virtual void fromCppType(){};

  virtual TaoBase *duplicate() { return 0; }
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <class _Tp> class TaoNumArray : public TaoArrayBase {
  // protected:
public:
  TaoArrayBase *subArray;
  virtual TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array);
  // Data:
  _Tp *numArray;

public:
  TaoNumArray();
  TaoNumArray(size_t size, _Tp value = 0);
  TaoNumArray(valarray<size_t> &dims, _Tp value = 0);
  ~TaoNumArray();

  TaoNumArray<_Tp> &operator+=(const _Tp &);
  TaoNumArray<_Tp> &operator-=(const _Tp &);
  TaoNumArray<_Tp> &operator*=(const _Tp &);
  TaoNumArray<_Tp> &operator/=(const _Tp &);
  // operations on elements, the matrices must have the same size:
  TaoNumArray<_Tp> &operator=(TaoNumArray<_Tp> &);
  TaoNumArray<_Tp> &operator+=(TaoNumArray<_Tp> &);
  TaoNumArray<_Tp> &operator-=(TaoNumArray<_Tp> &);
  //	TaoRealArray<_Tp>& operator*=(TaoRealArray<_Tp>&);

  void resizeVector(size_t size, size_t oldSize) {
    numArray = (_Tp *)realloc(numArray, size * sizeof(_Tp));
    for (size_t i = oldSize; i < size; i++)
      numArray[i] = 0;
  }

  _Tp *getAsVector() { return numArray; }
  void setNumArray(_Tp *vals, valarray<size_t> &dm) {
    dims.resize(dm.size());
    dims = dm;
    updateDimAccum();
    numArray = vals;
  }

  _Tp &operator()(size_t i);
  _Tp &getMapped(size_t i);

  TaoArrayBase *permute(valarray<size_t> &perm, bool inverse);
  TaoArrayBase *transpose();

  virtual TaoArrayBase *convolute(TaoArrayBase *kernel, TaoArrayBase *outArray,
                                  TaoInfoKit *infoKit) {
    return 0;
  }

  static short RTTI;
  virtual short rtti() const;
  virtual void print_element(size_t i, ostream *out = &cout){};

  virtual short rttiNumeric() const { return 0; }
  virtual void exportAsIndex(valarray<size_t> &index){};

  // This function must be called after mapIndex():
  virtual TaoBase *basicMathFunct(TaoInfoKit *infoKit, short type = 1) {
    return 0;
  }
  // To get which elements satisfy a condition,
  // return an array of subindices:
  virtual TaoIntArray *numWhichByOper(char oper, double value) { return 0; }

  virtual TcBase *toCppType() { return 0; }
  virtual void fromCppType(){};

  virtual TaoBase *duplicate();
};

////////////////////////////////////////////////////////////
// TaoRealArray: numeric array.
template <class _Tp> class TaoRealArray : public TaoNumArray<_Tp> {
protected:
  // Will be return by subIndexing():
  TaoNumber *number;

  virtual TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array);

  bool compare(_Tp left, _Tp right, char oper);

public:
  TaoRealArray();
  TaoRealArray(size_t size, _Tp value = 0);
  TaoRealArray(valarray<size_t> &dims, _Tp value = 0);
  TaoRealArray(TaoRealArray<_Tp> &);
  virtual ~TaoRealArray();

  void exportAsIndex(valarray<size_t> &index);

  // Virtual function inherited from TaoArrayBase.
  TaoBase *subIndexing();

  // To get which elements satisfy a condition,
  // return an array of subindices:
  TaoIntArray *numWhichByOper(char oper, double value);

  // Used for assignment:
  void assignWith(double &val, char oper = TAO_ASSIGN);

  double getDouble(size_t i) { return (double)this->numArray[i]; }
  void setDouble(size_t i, double value) { this->numArray[i] = (_Tp)value; }

  // This function must be called after mapIndex():
  TaoBase *basicMathFunct(TaoInfoKit *infoKit, short type = 1);

  TaoArrayBase *convolute(TaoArrayBase *kernel, TaoArrayBase *outArray,
                          TaoInfoKit *infoKit);
  void realConvolution(valarray<valarray<int>> &, valarray<double> &,
                       TaoArrayBase *);

  static short RTTI;
  virtual short rtti() const;
  virtual void print_element(size_t i, ostream *out = &cout);

  void copyFrom(TaoRealArray<_Tp> *array);

  virtual short rttiNumeric() const { return 0; }
  virtual TcBase *toCppType() { return 0; }
  virtual void fromCppType(){};
};
//===================================================================
//===================================================================
class TaoByteArray : public TaoRealArray<Byte> {
  TcByteArray *cpptype;

  TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array);

public:
  TaoByteArray() { cpptype = 0; };
  TaoByteArray(TaoByteArray &);

  void print_element(size_t i, ostream *out = &cout);

  static short RTTI;
  short rttiNumeric() const { return RTTI; }

  TcBase *toCppType();
  void fromCppType();
};
class TaoShortArray : public TaoRealArray<short> {
  TcShortArray *cpptype;

  TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array);

public:
  TaoShortArray() { cpptype = 0; };
  TaoShortArray(TaoShortArray &);

  static short RTTI;
  short rttiNumeric() const { return RTTI; }

  TcBase *toCppType();
  void fromCppType();
};
class TaoIntArray : public TaoRealArray<int> {
  TcIntArray *cpptype;

  TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array);

public:
  TaoIntArray() { cpptype = 0; };
  TaoIntArray(TaoIntArray &);

  static short RTTI;
  short rttiNumeric() const { return RTTI; }

  TcBase *toCppType();
  void fromCppType();
};
class TaoFloatArray : public TaoRealArray<float> {
  TcFloatArray *cpptype;

  TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array);

public:
  TaoFloatArray() { cpptype = 0; };
  TaoFloatArray(TaoFloatArray &);

  static short RTTI;
  short rttiNumeric() const { return RTTI; }

  TcBase *toCppType();
  void fromCppType();
};
class TaoDoubleArray : public TaoRealArray<double> {
  TcDoubleArray *cpptype;

  TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array);

public:
  TaoDoubleArray() { cpptype = 0; };
  TaoDoubleArray(TaoDoubleArray &);

  static short RTTI;
  short rttiNumeric() const { return RTTI; }

  TcBase *toCppType();
  void fromCppType();
};
//===================================================================
//===================================================================
template <class _Tp> class TaoCompArray : public TaoNumArray<complex<_Tp>> {
  TcComplexArray *cpptype;

  // Will be return by subIndexing():
  TaoComplex *complx;

  TaoArrayBase *tryAllocateSubArray(TaoArrayBase *array);

  bool compare(complex<_Tp> left, complex<_Tp> right, char oper);

public:
  TaoCompArray();
  TaoCompArray(size_t size, _Tp value = 0);
  TaoCompArray(valarray<size_t> &ds, _Tp value = 0);
  TaoCompArray(TaoCompArray<_Tp> &);
  TaoCompArray(TaoRealArray<_Tp> *rmat, TaoRealArray<_Tp> *imat);
  ~TaoCompArray();

  complex<_Tp> *getAsVector() { return this->numArray; }

  complex<double> getComplex(size_t i) {
    return (complex<double>)this->numArray[i];
  }
  void setComplex(size_t i, complex<double> value) {
    this->numArray[i] = (complex<_Tp>)value;
  }

  // This function must be called after mapIndex():
  TaoBase *basicMathFunct(TaoInfoKit *infoKit, short type = 1);

  // operations on elements, the matrices must have the same size:
  TaoCompArray<_Tp> &operator=(TaoRealArray<_Tp> &);
  TaoCompArray<_Tp> &operator+=(TaoRealArray<_Tp> &);
  TaoCompArray<_Tp> &operator-=(TaoRealArray<_Tp> &);

  TaoArrayBase *convolute(TaoArrayBase *kernel, TaoArrayBase *outArray,
                          TaoInfoKit *infoKit);
  void compConvolution(valarray<valarray<int>> &, valarray<complex<double>> &,
                       TaoArrayBase *);

  // Used for assignment:
  void assignWith(double &val, char oper = TAO_ASSIGN);
  void assignWith(complex<double> &val, char oper = TAO_ASSIGN);

  // To get which elements satisfy a condition,
  // return an array of subindices:
  TaoIntArray *comWhichByOper(char oper, complex<_Tp> value);

  // Virtual function inherited from TaoArrayBase.
  TaoBase *subIndexing();

  static short RTTI;
  virtual short rtti() const;
  virtual short rttiNumeric() const { return RTTI; }
  void print_element(size_t i, ostream *out = &cout);

  TcBase *toCppType();
  void fromCppType();
};
//=============================================================
//=============================================================
// inline functions:
inline size_t TaoArrayBase::getFlatIndex(valarray<int> &ids,
                                         valarray<size_t> &dimAcc) {
  size_t ind = 0;
  for (size_t i = 0; i < ids.size(); i++)
    ind += ids[i] * dimAcc[i];
  return ind;
}
inline size_t TaoArrayBase::getFlatIndex(valarray<int> &ids) {
  size_t ind = 0;
  for (size_t i = 0; i < ids.size(); i++)
    ind += ids[i] * dimAccum[i];
  return ind;
}
template <class _Tp> inline _Tp &TaoNumArray<_Tp>::operator()(size_t i) {
  return numArray[i];
}
template <class _Tp> inline _Tp &TaoNumArray<_Tp>::getMapped(size_t i) {
  return numArray[idMapped[i]];
}
template <class _Tp>
inline TaoNumArray<_Tp> &TaoNumArray<_Tp>::operator+=(const _Tp &f) {
  for (size_t i = 0; i < size(); i++)
    numArray[i] += f;
  return *this;
}
template <class _Tp>
inline TaoNumArray<_Tp> &TaoNumArray<_Tp>::operator-=(const _Tp &f) {
  for (size_t i = 0; i < size(); i++)
    numArray[i] -= f;
  return *this;
}
template <class _Tp>
inline TaoNumArray<_Tp> &TaoNumArray<_Tp>::operator*=(const _Tp &f) {
  for (size_t i = 0; i < size(); i++)
    numArray[i] *= f;
  return *this;
}
template <class _Tp>
inline TaoNumArray<_Tp> &TaoNumArray<_Tp>::operator/=(const _Tp &f) {
  for (size_t i = 0; i < size(); i++)
    numArray[i] /= f;
  return *this;
}
template <class _Tp>
inline TaoNumArray<_Tp> &TaoNumArray<_Tp>::operator=(TaoNumArray<_Tp> &mat) {
  _Tp *vec = mat.getAsVector();
  for (size_t i = 0; i < size(); i++)
    numArray[i] = vec[i];
  return *this;
}
template <class _Tp>
inline TaoNumArray<_Tp> &TaoNumArray<_Tp>::operator+=(TaoNumArray<_Tp> &mat) {
  _Tp *vec = mat.getAsVector();
  for (size_t i = 0; i < size(); i++)
    numArray[i] += vec[i];
  return *this;
}
template <class _Tp>
inline TaoNumArray<_Tp> &TaoNumArray<_Tp>::operator-=(TaoNumArray<_Tp> &mat) {
  _Tp *vec = mat.getAsVector();
  for (size_t i = 0; i < size(); i++)
    numArray[i] -= vec[i];
  return *this;
}
//=============================================================
//=============================================================

template <class _Tp> short TaoNumArray<_Tp>::RTTI = TAO_NumArray;
template <class _Tp> short TaoNumArray<_Tp>::rtti() const { return RTTI; }
template <class _Tp> short TaoRealArray<_Tp>::RTTI = TAO_RealArray;
template <class _Tp> short TaoRealArray<_Tp>::rtti() const { return RTTI; }
template <class _Tp> short TaoCompArray<_Tp>::RTTI = TAO_CompArray;
template <class _Tp> short TaoCompArray<_Tp>::rtti() const { return RTTI; }

//=============================================================
//=============================================================

//=============================================================
//=============================================================
template <class _Tp> TaoNumArray<_Tp>::TaoNumArray() {
  subArray = 0;
  dims.resize(1);
  dims[0] = 0;
  updateDimAccum();
  numArray = (_Tp *)malloc(sizeof(_Tp));
}
template <class _Tp> TaoNumArray<_Tp>::TaoNumArray(size_t size, _Tp value) {
  subArray = 0;
  dims.resize(2);
  dims[0] = 1;
  dims[1] = size;
  updateDimAccum();
  numArray = (_Tp *)calloc(size, sizeof(_Tp));
  for (size_t i = 0; i < size; i++)
    numArray[i] = value;
}
template <class _Tp>
TaoNumArray<_Tp>::TaoNumArray(valarray<size_t> &ds, _Tp value) {
  subArray = 0;
  dims.resize(ds.size());
  dims = ds;
  updateDimAccum();
  numArray = (_Tp *)calloc(size(), sizeof(_Tp));
  for (size_t i = 0; i < size(); i++)
    numArray[i] = value;
}
template <class _Tp> TaoNumArray<_Tp>::~TaoNumArray() {
  free(numArray);
  if (subArray) {
    if (subArray->refCount == 1)
      delete subArray;
    else {
      // somebody is still using it:
      subArray->locked = 0;
      subArray->refCount--;
    }
  }
}
template <class _Tp>
TaoArrayBase *TaoNumArray<_Tp>::tryAllocateSubArray(TaoArrayBase *array) {
  return TaoTryAllocator<TaoNumArray<_Tp>>::tryAllocate(
      (TaoNumArray<_Tp> *)array);
}

template <class _Tp>
TaoArrayBase *TaoNumArray<_Tp>::permute(valarray<size_t> &perm, bool inverse) {
  // permute(2,3,1)
  // array[i,j,k]=perm[j,k,i];

  // invpermute(2,3,1)
  // array[j,k,i]=perm[i,j,k];

  size_t i, j;

  // perm should contains a permutation of 1 to N:
  for (i = 0; i < perm.size(); i++)
    perm[i]--;

  valarray<size_t> dimtrans(dims.size());
  if (!inverse) {
    for (i = 0; i < dims.size(); i++)
      dimtrans[i] = dims[perm[i]];
  } else {
    for (i = 0; i < dims.size(); i++)
      dimtrans[perm[i]] = dims[i];
  }
  subArray = tryAllocateSubArray(subArray);
  subArray->timeTag = TaoBase::recycler->gcTimes;
  subArray->resize(dimtrans);

  _Tp *vec = ((TaoNumArray<_Tp> *)subArray)->getAsVector();

  valarray<int> index(dims.size());
  valarray<int> idtrans(dims.size());

  if (!inverse) {
    for (i = 0; i < size(); i++) {
      exportSubIndex(index, i);
      for (j = 0; j < dims.size(); j++)
        idtrans[j] = index[perm[j]];

      j = subArray->getFlatIndex(idtrans);
      vec[j] = numArray[i];
    }
  } else {
    for (i = 0; i < size(); i++) {
      subArray->exportSubIndex(idtrans, i);
      for (j = 0; j < dims.size(); j++)
        index[j] = idtrans[perm[j]];

      j = getFlatIndex(index);
      vec[i] = numArray[j];
    }
  }
  return subArray;
}
template <class _Tp> TaoArrayBase *TaoNumArray<_Tp>::transpose() {
  valarray<size_t> order(dims.size());
  for (size_t i = 0; i < dims.size(); i++)
    order[i] = i + 1;

  order[0] = 2;
  order[1] = 1;
  return permute(order, 0);
}
template <class _Tp> TaoBase *TaoNumArray<_Tp>::duplicate() {
  subArray = tryAllocateSubArray(subArray);
  TaoNumArray<_Tp> *array = (TaoNumArray<_Tp> *)subArray;
  (*array) = (*this);

  return (TaoBase *)subArray;
}
//=============================================================
//=============================================================
template <class _Tp> TaoRealArray<_Tp>::TaoRealArray() : TaoNumArray<_Tp>() {
  number = 0;
}
template <class _Tp>
TaoRealArray<_Tp>::TaoRealArray(size_t size, _Tp value)
    : TaoNumArray<_Tp>(size, value) {
  number = 0;
}
template <class _Tp>
TaoRealArray<_Tp>::TaoRealArray(valarray<size_t> &ds, _Tp value)
    : TaoNumArray<_Tp>(ds, value) {
  number = 0;
}
template <class _Tp> TaoRealArray<_Tp>::TaoRealArray(TaoRealArray<_Tp> &matx) {
  number = 0;
  this->subArray = 0;
  this->dims.resize(matx.getDims().size());
  this->dims = matx.getDims();
  this->updateDimAccum();

  this->numArray = (_Tp *)realloc(this->numArray, this->size() * sizeof(_Tp));
  _Tp *vec = matx.getAsVector();
  for (size_t i = 0; i < this->size(); i++)
    this->numArray[i] = vec[i];
}
template <class _Tp> TaoRealArray<_Tp>::~TaoRealArray() {
  if (number) {
    if (number->refCount == 1)
      delete number;
    else {
      number->refCount--;
      number->locked = 0;
    }
  }
}
template <class _Tp>
void TaoRealArray<_Tp>::exportAsIndex(valarray<size_t> &index) {
  index.resize(this->size());
  for (size_t i = 0; i < this->size(); i++)
    index[i] = (size_t)this->numArray[i];
}
template <class _Tp>
void TaoRealArray<_Tp>::assignWith(double &val, char oper) {
  this->mapIndex();

  _Tp value = (_Tp)val;
  if (oper == TAO_ASSIGN)
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] = value;
  else if (oper == TAO_ASSIGN_PLUS) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] += value;
  } else if (oper == TAO_ASSIGN_MINUS) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] -= value;
  } else if (oper == TAO_ASSIGN_TIMES) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] *= value;
  } else if (oper == TAO_ASSIGN_DIVIDE) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] /= value;
  } else if (oper == TAO_ASSIGN_MOD) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] =
          (int)this->numArray[this->idMapped[i]] % (int)val;
  } else if (oper == TAO_ASSIGN_POW) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] =
          (_Tp)mypow(this->numArray[this->idMapped[i]], val);
  } else if (oper == TAO_ASSIGN_AND) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] =
          this->numArray[this->idMapped[i]] && val;
  } else if (oper == TAO_ASSIGN_OR) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] =
          this->numArray[this->idMapped[i]] || val;
  }
}
template <class _Tp>
bool TaoRealArray<_Tp>::compare(_Tp left, _Tp right, char oper) {
  if (oper == TAO_EQUAL) {
    if (left == right)
      return 1;
  } else if (oper == TAO_NOT_EQUAL) {
    if (left != right)
      return 1;
  } else if (oper == TAO_SMALLER) {
    if (left < right)
      return 1;
  } else if (oper == TAO_LARGER) {
    if (left > right)
      return 1;
  } else if (oper == TAO_NOT_SMALLER) {
    if (left >= right)
      return 1;
  } else if (oper == TAO_NOT_LARGER) {
    if (left <= right)
      return 1;
  }
  return 0;
}
template <class _Tp>
TaoIntArray *TaoRealArray<_Tp>::numWhichByOper(char oper, double value) {
  vector<vector<int>> ids;

  for (size_t i = 0; i < this->numIndexElem; i++) {
    int idmap = 0;
    int mod = i;
    vector<int> one(this->subIndex.size());
    for (int j = (int)this->subIndex.size() - 1; j >= 0; j--) {
      one[j] = mod % this->subIndex[j].size();
      mod /= this->subIndex[j].size();
      idmap += this->subIndex[j][one[j]] * this->dimAccum[j];
    }
    if (compare(this->numArray[idmap], (_Tp)value, oper))
      ids.push_back(one);
  }
  return TaoIndexMaker::whichIndexing(ids, this->whichIndex);
}
template <class _Tp>
TaoBase *TaoRealArray<_Tp>::basicMathFunct(TaoInfoKit *infoKit, short type) {
  this->mapIndex();
  number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
  if (this->idMapped.size() == 0) {
    number->value = 0;
    infoKit->warning("No elements exist for such operation");
  }
  _Tp val = this->numArray[this->idMapped[0]];

  if (type == TAO_max) {
    _Tp max = val;
    for (size_t i = 1; i < this->idMapped.size(); i++) {
      val = this->numArray[this->idMapped[i]];
      if (max < val)
        max = val;
    }
    number->value = (double)max;
  } else if (type == TAO_min) {
    _Tp min = val;
    for (size_t i = 1; i < this->idMapped.size(); i++) {
      val = this->numArray[this->idMapped[i]];
      if (min > val)
        min = val;
    }
    number->value = (double)min;
  } else if (type == TAO_mean) {
    double sum = val;
    for (size_t i = 1; i < this->idMapped.size(); i++)
      sum += (double)this->numArray[this->idMapped[i]];
    number->value = (double)sum / this->idMapped.size();
  } else if (type == TAO_stdev) {
    double sum = val;
    double sum2 = val * val;
    for (size_t i = 1; i < this->idMapped.size(); i++) {
      val = this->numArray[this->idMapped[i]];
      sum += (double)val;
      sum2 += (double)val * val;
    }
    // SUM(Xi-mean)^2
    double dev = sum2 - sum * sum / this->idMapped.size();

    if (this->idMapped.size() == 1)
      number->value = 0;
    else
      number->value = (double)sqrt(dev / (this->idMapped.size() - 1));
  } else if (type == TAO_varn) {
    double sum = val;
    double sum2 = val * val;
    for (size_t i = 1; i < this->idMapped.size(); i++) {
      val = this->numArray[this->idMapped[i]];
      sum += (double)val;
      sum2 += (double)val * val;
    }
    // SUM(Xi-mean)^2
    double dev = sum2 - sum * sum / this->idMapped.size();

    number->value = (double)dev / this->idMapped.size();
  } else if (type == TAO_sum) {
    double sum = val;
    for (size_t i = 1; i < this->idMapped.size(); i++)
      sum += (double)this->numArray[this->idMapped[i]];
    number->value = (double)sum;
  } else if (type == TAO_prod) {
    double prod = val;
    for (size_t i = 1; i < this->idMapped.size(); i++)
      prod *= (double)this->numArray[this->idMapped[i]];
    number->value = (double)prod;
  }
  return number;
}

template <class _Tp>
TaoArrayBase *TaoRealArray<_Tp>::tryAllocateSubArray(TaoArrayBase *array) {
  return TaoTryAllocator<TaoRealArray<_Tp>>::tryAllocate(
      (TaoRealArray<_Tp> *)array);
}
template <class _Tp> TaoBase *TaoRealArray<_Tp>::subIndexing() {
  this->mapIndex();
  if (this->numIndexElem == 1) {
    number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
    number->value = (double)this->numArray[this->idMapped[0]];
    return number;
  } else {
    this->subArray = tryAllocateSubArray(this->subArray);
    this->subArray->timeTag = TaoBase::recycler->gcTimes;

    valarray<size_t> ds;
    this->makeSubDims(ds);

    this->subArray->resize(ds);
    TaoRealArray<_Tp> *array = (TaoRealArray<_Tp> *)this->subArray;
    _Tp *vec = array->getAsVector();
    for (size_t i = 0; i < array->size(); i++)
      vec[i] = this->numArray[this->idMapped[i]];

    this->subArray->resetSubIndex();
    return this->subArray;
  }
}
template <class _Tp>
void TaoRealArray<_Tp>::realConvolution(valarray<valarray<int>> &shift,
                                        valarray<double> &values,
                                        TaoArrayBase *outArray) {
  _Tp *vec = ((TaoNumArray<_Tp> *)outArray)->getAsVector();

  valarray<int> index(this->dims.size());
  valarray<int> center(this->dims.size());
  for (size_t i = 0; i < this->size(); i++) {
    this->exportSubIndex(center, i);
    double sum = 0;
    for (size_t j = 0; j < shift.size(); j++) {
      bool bl = 1;
      for (size_t k = 0; k < index.size(); k++) {
        index[k] = center[k] + shift[j][k];
        if (index[k] < 0 || index[k] >= (int)this->dims[k]) {
          bl = 0;
          break;
        }
      }
      if (bl) {
        size_t k = this->getFlatIndex(index);
        sum += this->numArray[k] * values[j];
      }
    }
    vec[i] = (_Tp)sum;
  }
}

template <class _Tp, class _Tk, class _Tv>
bool initConvolution(TaoNumArray<_Tp> *base, TaoNumArray<_Tk> *kernel,
                     valarray<valarray<int>> &shift, valarray<_Tv> &values,
                     TaoInfoKit *infoKit);

template <class _Tp>
TaoArrayBase *TaoRealArray<_Tp>::convolute(TaoArrayBase *kernel,
                                           TaoArrayBase *outArray,
                                           TaoInfoKit *infoKit) {
  if (!outArray) {
    this->subArray = tryAllocateSubArray(this->subArray);
    this->subArray->timeTag = TaoBase::recycler->gcTimes;
    outArray = this->subArray;
  }
  outArray->resize(this->dims);
  if (outArray->rttiNumeric() != rttiNumeric()) {
    infoKit->warning("the output array in convolution must be the same type");
    return this;
  }

  valarray<valarray<int>> shift;

  short rtti = kernel->rttiNumeric();
  if (rtti == TAO_ByteArray) {
    TaoByteArray *kern = (TaoByteArray *)kernel;
    valarray<double> values;
    if (!initConvolution(this, kern, shift, values, infoKit))
      return 0;
    realConvolution(shift, values, outArray);
  } else if (rtti == TAO_ShortArray) {
    TaoShortArray *kern = (TaoShortArray *)kernel;
    valarray<double> values;
    if (!initConvolution(this, kern, shift, values, infoKit))
      return 0;
    realConvolution(shift, values, outArray);
  } else if (rtti == TAO_IntArray) {
    TaoIntArray *kern = (TaoIntArray *)kernel;
    valarray<double> values;
    if (!initConvolution(this, kern, shift, values, infoKit))
      return 0;
    realConvolution(shift, values, outArray);
  } else if (rtti == TAO_FloatArray) {
    TaoFloatArray *kern = (TaoFloatArray *)kernel;
    valarray<double> values;
    if (!initConvolution(this, kern, shift, values, infoKit))
      return 0;
    realConvolution(shift, values, outArray);
  } else if (rtti == TAO_DoubleArray) {
    TaoDoubleArray *kern = (TaoDoubleArray *)kernel;
    valarray<double> values;
    if (!initConvolution(this, kern, shift, values, infoKit))
      return 0;
    realConvolution(shift, values, outArray);
  }
  outArray->resetSubIndex();
  return outArray;
}
template <class _Tp>
inline void TaoRealArray<_Tp>::print_element(size_t i, ostream *out) {
  *out << this->numArray[i];
}
//=============================================================
//=============================================================
template <class _Tp>
TaoCompArray<_Tp>::TaoCompArray() : TaoNumArray<complex<_Tp>>() {
  complx = 0;
  cpptype = 0;
}
template <class _Tp>
TaoCompArray<_Tp>::TaoCompArray(size_t size, _Tp value)
    : TaoNumArray<complex<_Tp>>(size, complex<_Tp>(value)) {
  complx = 0;
  cpptype = 0;
}
template <class _Tp>
TaoCompArray<_Tp>::TaoCompArray(valarray<size_t> &ds, _Tp value)
    : TaoNumArray<complex<_Tp>>(ds, complex<_Tp>(value)) {
  complx = 0;
  cpptype = 0;
}
template <class _Tp> TaoCompArray<_Tp>::TaoCompArray(TaoCompArray<_Tp> &matx) {
  cpptype = 0;
  complx = 0;
  this->subArray = 0;
  this->dims.resize(matx.getDims().size());
  this->dims = matx.getDims();
  this->updateDimAccum();

  this->numArray = (complex<_Tp> *)realloc(this->numArray,
                                           this->size() * sizeof(complex<_Tp>));
  complex<_Tp> *vec = matx.getAsVector();
  for (size_t i = 0; i < this->size(); i++)
    this->numArray[i] = vec[i];
}
template <class _Tp>
TaoCompArray<_Tp>::TaoCompArray(TaoRealArray<_Tp> *rmat,
                                TaoRealArray<_Tp> *imat) {
  cpptype = 0;
  complx = 0;
  this->subArray = 0;
}
template <class _Tp> TaoCompArray<_Tp>::~TaoCompArray() {
  if (complx) {
    if (complx->refCount == 1)
      delete complx;
    else {
      complx->refCount--;
      complx->locked = 0;
    }
  }
}
template <class _Tp>
TaoArrayBase *TaoCompArray<_Tp>::tryAllocateSubArray(TaoArrayBase *array) {
  return TaoTryAllocator<TaoCompArray<_Tp>>::tryAllocate(
      (TaoCompArray<_Tp> *)array);
}
template <class _Tp>
TaoCompArray<_Tp> &TaoCompArray<_Tp>::operator=(TaoRealArray<_Tp> &mat) {
  return *this;
}
template <class _Tp>
TaoCompArray<_Tp> &TaoCompArray<_Tp>::operator+=(TaoRealArray<_Tp> &mat) {
  return *this;
}
template <class _Tp>
TaoCompArray<_Tp> &TaoCompArray<_Tp>::operator-=(TaoRealArray<_Tp> &mat) {
  //	valarray<double> &vec=*mat.getAsVector();
  return *this;
}
template <class _Tp>
void TaoCompArray<_Tp>::assignWith(double &val, char oper) {
  complex<double> v(val);
  assignWith(v, oper);
}
template <class _Tp>
void TaoCompArray<_Tp>::assignWith(complex<double> &val, char oper) {
  this->mapIndex();

  if (oper == TAO_ASSIGN)
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] = val;
  else if (oper == TAO_ASSIGN_PLUS) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] += val;
  } else if (oper == TAO_ASSIGN_MINUS) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] -= val;
  } else if (oper == TAO_ASSIGN_TIMES) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] *= val;
  } else if (oper == TAO_ASSIGN_DIVIDE) {
    for (size_t i = 0; i < this->idMapped.size(); i++)
      this->numArray[this->idMapped[i]] /= val;
  }
}
template <class _Tp>
bool TaoCompArray<_Tp>::compare(complex<_Tp> left, complex<_Tp> right,
                                char oper) {
  if (oper == TAO_EQUAL) {
    if (left == right)
      return 1;
  } else if (oper == TAO_NOT_EQUAL) {
    if (left != right)
      return 1;
  }
  return 0;
}
template <class _Tp>
TaoIntArray *TaoCompArray<_Tp>::comWhichByOper(char oper, complex<_Tp> value) {
  vector<vector<int>> ids;

  for (size_t i = 0; i < this->numIndexElem; i++) {
    int idmap = 0;
    int mod = i;
    vector<int> one(this->subIndex.size());
    for (int j = (int)this->subIndex.size() - 1; j >= 0; j--) {
      one[j] = mod % this->subIndex[j].size();
      mod /= this->subIndex[j].size();
      idmap += this->subIndex[j][one[j]] * this->dimAccum[j];
    }
    if (compare(this->numArray[idmap], value, oper))
      ids.push_back(one);
  }
  return TaoIndexMaker::whichIndexing(ids, this->whichIndex);
}
template <class _Tp>
TaoBase *TaoCompArray<_Tp>::basicMathFunct(TaoInfoKit *infoKit, short type) {
  this->mapIndex();
  complx = TaoTryAllocator<TaoComplex>::tryAllocate(complx);
  if (this->idMapped.size() == 0) {
    complx->cmplx = 0;
    infoKit->warning("No elements exist for such operation");
  }
  complex<_Tp> sum = this->numArray[this->idMapped[0]];

  if (type == TAO_mean) {
    for (size_t i = 1; i < this->idMapped.size(); i++)
      sum += this->numArray[this->idMapped[i]];
    complx->cmplx = (complex<double>)sum / (double)this->idMapped.size();
  } else if (type == TAO_sum) {
    for (size_t i = 1; i < this->idMapped.size(); i++)
      sum += this->numArray[this->idMapped[i]];
    complx->cmplx = (complex<double>)sum;
  } else if (type == TAO_prod) {
    for (size_t i = 1; i < this->idMapped.size(); i++)
      sum *= this->numArray[this->idMapped[i]];
    complx->cmplx = (complex<double>)sum;
  } else {
    complx->cmplx = 0;
    infoKit->warning("No such function for complex array");
  }
  return complx;
}
template <class _Tp> TaoBase *TaoCompArray<_Tp>::subIndexing() {
  this->mapIndex();
  if (this->numIndexElem == 1) {
    complx = TaoTryAllocator<TaoComplex>::tryAllocate(complx);
    complx->cmplx = (complex<double>)this->numArray[this->idMapped[0]];
    return complx;
  } else {
    this->subArray = tryAllocateSubArray(this->subArray);
    this->subArray->timeTag = TaoBase::recycler->gcTimes;

    valarray<size_t> ds;
    this->makeSubDims(ds);

    this->subArray->resize(ds);
    TaoCompArray<_Tp> *array = (TaoCompArray<_Tp> *)this->subArray;
    complex<_Tp> *vec = array->getAsVector();
    for (size_t i = 0; i < array->size(); i++)
      vec[i] = this->numArray[this->idMapped[i]];

    this->subArray->resetSubIndex();
    return this->subArray;
  }
}
template <class _Tp>
void TaoCompArray<_Tp>::compConvolution(valarray<valarray<int>> &shift,
                                        valarray<complex<double>> &values,
                                        TaoArrayBase *outArray) {
  complex<_Tp> *vec = ((TaoNumArray<complex<_Tp>> *)outArray)->getAsVector();

  valarray<int> index(this->dims.size());
  valarray<int> center(this->dims.size());
  for (size_t i = 0; i < this->size(); i++) {
    this->exportSubIndex(center, i);
    complex<double> sum = 0;
    for (size_t j = 0; j < shift.size(); j++) {
      bool bl = 1;
      for (size_t k = 0; k < index.size(); k++) {
        index[k] = center[k] + shift[j][k];
        if (index[k] < 0 || index[k] >= (int)this->dims[k]) {
          bl = 0;
          break;
        }
      }
      if (bl) {
        size_t k = this->getFlatIndex(index);
        sum += this->numArray[k] * values[j];
      }
    }
    vec[i] = sum;
  }
}
template <class _Tp>
TaoArrayBase *TaoCompArray<_Tp>::convolute(TaoArrayBase *kernel,
                                           TaoArrayBase *outArray,
                                           TaoInfoKit *infoKit) {
  if (!outArray) {
    this->subArray = tryAllocateSubArray(this->subArray);
    this->subArray->timeTag = TaoBase::recycler->gcTimes;
    outArray = this->subArray;
  }
  outArray->resize(this->dims);

  valarray<valarray<int>> shift;

  if (kernel->rtti() == TAO_CompArray) {
    TaoCompArray<double> *kern = (TaoCompArray<double> *)kernel;
    valarray<complex<double>> values;
    if (!initConvolution(this, kern, shift, values, infoKit))
      return 0;
    compConvolution(shift, values, outArray);
  } else {
    short rtti = kernel->rttiNumeric();
    if (rtti == TAO_ByteArray) {
      TaoByteArray *kern = (TaoByteArray *)kernel;
      valarray<complex<double>> values;
      if (!initConvolution(this, kern, shift, values, infoKit))
        return 0;
      compConvolution(shift, values, outArray);
    } else if (rtti == TAO_ShortArray) {
      TaoShortArray *kern = (TaoShortArray *)kernel;
      valarray<complex<double>> values;
      if (!initConvolution(this, kern, shift, values, infoKit))
        return 0;
      compConvolution(shift, values, outArray);
    } else if (rtti == TAO_IntArray) {
      TaoIntArray *kern = (TaoIntArray *)kernel;
      valarray<complex<double>> values;
      if (!initConvolution(this, kern, shift, values, infoKit))
        return 0;
      compConvolution(shift, values, outArray);
    } else if (rtti == TAO_FloatArray) {
      TaoFloatArray *kern = (TaoFloatArray *)kernel;
      valarray<complex<double>> values;
      if (!initConvolution(this, kern, shift, values, infoKit))
        return 0;
      compConvolution(shift, values, outArray);
    } else if (rtti == TAO_DoubleArray) {
      TaoDoubleArray *kern = (TaoDoubleArray *)kernel;
      valarray<complex<double>> values;
      if (!initConvolution(this, kern, shift, values, infoKit))
        return 0;
      compConvolution(shift, values, outArray);
    }
  }
  outArray->resetSubIndex();
  return outArray;
}
template <class _Tp> TcBase *TaoCompArray<_Tp>::toCppType() {
  if (!cpptype)
    cpptype = new TcComplexArray;
  cpptype->reserved = this;

  int ndim = this->dims.size();
  int *cdims = (int *)calloc(ndim, sizeof(int));
  for (int i = 0; i < ndim; i++)
    cdims[i] = this->dims[i];
  cpptype->resize(cdims, ndim);

  double *reals = cpptype->getAsVector();
  double *imags = cpptype->getAsVector(0);
  for (size_t k = 0; k < this->size(); k++) {
    reals[k] = this->numArray[k].real();
    imags[k] = this->numArray[k].imag();
  }

  if (this->recycler)
    this->recycler->pushTcObject(cpptype);

  return cpptype;
}
template <class _Tp> void TaoCompArray<_Tp>::fromCppType() {
  if (!cpptype)
    return;

  size_t oldSize = this->size();

  int ndim = cpptype->dim();
  int *cdims = cpptype->getDims();
  this->dims.resize(ndim);
  for (int i = 0; i < ndim; i++)
    this->dims[i] = cdims[i];

  this->updateDimAccum();
  this->resizeVector(this->dims[0] * this->dimAccum[0], oldSize);

  double *reals = cpptype->getAsVector();
  double *imags = cpptype->getAsVector(0);
  for (size_t k = 0; k < this->size(); k++)
    this->numArray[k] = complex<double>(reals[k], imags[k]);

  if (this->recycler)
    this->recycler->pushTcObject(cpptype);

  cpptype = 0;
}
template <class _Tp>
inline void TaoCompArray<_Tp>::print_element(size_t i, ostream *out) {
  *out << this->numArray[i].real();
  if (this->numArray[i].imag() >= 0.0)
    *out << "+";
  *out << this->numArray[i].imag() << "$";
}
//=============================================================
//=============================================================
template <class _Tp>
void exportValues(TaoArrayBase *array, valarray<_Tp> &vals) {
  vals = 0;
  if (vals.size() < array->size())
    vals.resize(array->size());
  for (size_t i = 0; i < array->size(); i++)
    vals[i] = (_Tp)array->getDouble(i);
}

template <class _Tp, class _Tk, class _Tv>
bool initConvolution(TaoNumArray<_Tp> *base, TaoNumArray<_Tk> *kernel,
                     valarray<valarray<int>> &shift, valarray<_Tv> &values,
                     TaoInfoKit *infoKit) {
  if (kernel->dim() != base->dim()) {
    infoKit->warning("can't convolute with this kernel");
    return 0;
  }
  valarray<size_t> dimKern = kernel->getDims();
  size_t i, k;
  valarray<int> center(dimKern.size());
  for (i = 0; i < dimKern.size(); i++) {
    if (dimKern[i] % 2 != 1) {
      infoKit->warning("kernel is not symetric");
      return 0;
    }
    center[i] = dimKern[i] / 2;
  }

  kernel->exportAllSubIndex(shift);
  // Remove indices of zero elements:
  vector<valarray<int>> cleaned;
  for (i = 0; i < shift.size(); i++) {
    if ((*kernel)(i) != _Tk(0)) {
      cleaned.push_back(shift[i]);
    }
  }

  values.resize(cleaned.size());
  k = 0;
  for (i = 0; i < shift.size(); i++) {
    if ((*kernel)(i) != _Tk(0)) {
      values[k] = (_Tv)(*kernel)(i);
      k++;
    }
  }

  shift.resize(cleaned.size());
  for (i = 0; i < shift.size(); i++) {
    shift[i].resize(cleaned[i].size());
    shift[i] = cleaned[i] - center;
  }

  return 1;
}

template <class _Tp>
static TaoCompArray<double> *
comparray_scaleL_arrayR(TaoCompArray<double> *root, complex<double> left,
                        TaoNumArray<_Tp> *right, short oper) {
  root = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(root);
  root->resize(right->getDims());

  complex<double> *vcroot = root->getAsVector();
  _Tp *vcright = right->getAsVector();

  if (oper == TAO_PLUS) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = left + (complex<double>)vcright[i];
  } else if (oper == TAO_MINUS) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = left - (complex<double>)vcright[i];
  } else if (oper == TAO_TIMES) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = left * (complex<double>)vcright[i];
  }
  return root;
}
template <class _Tp>
static TaoCompArray<double> *
comparray_arrayL_scaleR(TaoCompArray<double> *root, TaoNumArray<_Tp> *left,
                        complex<double> right, short oper) {
  // if( TAO_ASSIGN < oper && oper <= TAO_ASSIGN_OR )
  // it is handled in TaoArith::binaryArith().

  root = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(root);
  root->resize(left->getDims());

  complex<double> *vcroot = root->getAsVector();
  _Tp *vcleft = left->getAsVector();

  if (oper == TAO_PLUS) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (complex<double>)vcleft[i] + right;
  } else if (oper == TAO_MINUS) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (complex<double>)vcleft[i] - right;
  } else if (oper == TAO_TIMES) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (complex<double>)vcleft[i] * right;
  } else if (oper == TAO_DIVIDE) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (complex<double>)vcleft[i] / right;
  }
  return root;
}
template <class _Troot, class _Tright>
static _Troot *realarray_scaleL_arrayR(_Troot *root, double left,
                                       TaoRealArray<_Tright> *right,
                                       short oper) {
  if (right->refCount == 1)
    root = (_Troot *)right;
  else {
    root = TaoTryAllocator<_Troot>::tryAllocate(root);
    root->resize(right->getDims());
  }

  _Tright *vcroot = ((TaoRealArray<_Tright> *)root)->getAsVector();
  _Tright *vcright = right->getAsVector();

  if (oper == TAO_PLUS) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (_Tright)left + vcright[i];
  } else if (oper == TAO_MINUS) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (_Tright)left - vcright[i];
  } else if (oper == TAO_TIMES) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (_Tright)left * vcright[i];
  }
  return root;
}
template <class _Troot, class _Tleft>
static _Troot *realarray_arrayL_scaleR(_Troot *root, TaoRealArray<_Tleft> *left,
                                       double right, short oper) {
  if (TAO_ASSIGN < oper && oper <= TAO_ASSIGN_OR) {
    TaoArrayBase *array = (TaoArrayBase *)left;
    array->assignWith(right, oper);
    return (_Troot *)array;
  }
  if (left->refCount == 1)
    root = (_Troot *)left;
  else {
    root = TaoTryAllocator<_Troot>::tryAllocate(root);
    root->resize(left->getDims());
  }

  _Tleft *vcroot = ((TaoRealArray<_Tleft> *)root)->getAsVector();
  _Tleft *vcleft = left->getAsVector();

  if (oper == TAO_PLUS) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = vcleft[i] + (_Tleft)right;
  } else if (oper == TAO_MINUS) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = vcleft[i] - (_Tleft)right;
  } else if (oper == TAO_TIMES) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = vcleft[i] * (_Tleft)right;
  } else if (oper == TAO_DIVIDE) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = vcleft[i] / (_Tleft)right;
  } else if (oper == TAO_MOD) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (_Tleft)((int)vcleft[i] % (int)right);
  } else if (oper == TAO_POW) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = (_Tleft)mypow((double)vcleft[i], (double)right);
  } else if (oper == TAO_AND) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = vcleft[i] && (_Tleft)right;
  } else if (oper == TAO_OR) {
    for (size_t i = 0; i < root->size(); i++)
      vcroot[i] = vcleft[i] || (_Tleft)right;
  }
  return root;
}
template <class _Tleft, class _Tright>
static TaoCompArray<double> *
comparray_arrayR_arrayL(TaoCompArray<double> *root, TaoNumArray<_Tleft> *left,
                        TaoNumArray<_Tright> *right, short oper,
                        TaoInfoKit *infoKit) {
  if (!left || !right)
    return 0;

  short rtL = left->rttiNumeric();
  short rtR = right->rttiNumeric();
  if (oper == TAO_PLUS || oper == TAO_MINUS || oper == TAO_PAIR_TIMES ||
      oper == TAO_PAIR_DIVIDE) {
    if (rtL == TAO_CompArray && left->refCount == 1) {
      root = (TaoCompArray<double> *)left;
      complex<double> *vcroot = root->getAsVector();
      _Tright *vcright = right->getAsVector();
      if (oper == TAO_PLUS) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] += (complex<double>)vcright[i];
      } else if (oper == TAO_MINUS) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] -= (complex<double>)vcright[i];
      } else if (oper == TAO_PAIR_TIMES) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] *= (complex<double>)vcright[i];
      } else {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] /= (complex<double>)vcright[i];
      }
    } else if (rtR == TAO_CompArray && right->refCount == 1) {
      root = (TaoCompArray<double> *)right;
      complex<double> *vcroot = root->getAsVector();
      _Tleft *vcleft = left->getAsVector();
      if (oper == TAO_PLUS) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] = (complex<double>)vcleft[i] + vcroot[i];
      } else if (oper == TAO_MINUS) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] = (complex<double>)vcleft[i] - vcroot[i];
      } else if (oper == TAO_PAIR_TIMES) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] = (complex<double>)vcleft[i] * vcroot[i];
      } else {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] = (complex<double>)vcleft[i] / vcroot[i];
      }
    } else {
      root = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(root);
      root->resize(right->getDims());
      complex<double> *vcroot = root->getAsVector();
      _Tleft *vcleft = left->getAsVector();
      _Tright *vcright = right->getAsVector();
      if (oper == TAO_PLUS) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] = (complex<double>)vcleft[i] + (complex<double>)vcright[i];
      } else if (oper == TAO_MINUS) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] = (complex<double>)vcleft[i] - (complex<double>)vcright[i];
      } else if (oper == TAO_PAIR_TIMES) {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] = (complex<double>)vcleft[i] * (complex<double>)vcright[i];
      } else {
        for (size_t i = 0; i < root->size(); i++)
          vcroot[i] = (complex<double>)vcleft[i] / (complex<double>)vcright[i];
      }
    }
  } else if (oper == TAO_TIMES) {
    if (left->dim() != 2 || right->dim() != 2 ||
        left->dim(1) != right->dim(0)) {
      infoKit->warning("improper array shape for matrix multiplication");
      return 0;
    }
    size_t S = left->dim(1);
    size_t N = left->dim(0);
    size_t M = right->dim(1);
    valarray<size_t> dims(2);
    dims[0] = N;
    dims[1] = M;
    root = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(root);
    root->resize(dims);

    complex<double> *vcroot = root->getAsVector();
    _Tleft *vcleft = left->getAsVector();
    _Tright *vcright = right->getAsVector();

    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        vcroot[i * M + j] = 0;
        for (size_t k = 0; k < S; k++)
          vcroot[i * M + j] += (complex<double>)vcleft[i * S + k] *
                               (complex<double>)vcright[k * M + j];
      }
    }
  } else if (TAO_ASSIGN < oper || oper <= TAO_ASSIGN_DIVIDE) {
    root = (TaoCompArray<double> *)left;
    complex<double> *vcroot = root->getAsVector();
    _Tright *vcright = right->getAsVector();
    if (oper == TAO_ASSIGN_PLUS) {
      for (size_t i = 0; i < root->size(); i++)
        vcroot[i] += (complex<double>)vcright[i];
    } else if (oper == TAO_ASSIGN_MINUS) {
      for (size_t i = 0; i < root->size(); i++)
        vcroot[i] -= (complex<double>)vcright[i];
    } else if (oper == TAO_ASSIGN_MINUS) {
      for (size_t i = 0; i < root->size(); i++)
        vcroot[i] *= (complex<double>)vcright[i];
    } else if (oper == TAO_ASSIGN_MINUS) {
      for (size_t i = 0; i < root->size(); i++)
        vcroot[i] /= (complex<double>)vcright[i];
    }
  }
  return root;
}

extern TaoArrayBase *allocRealArrayByType(short rootype);

template <class _Tleft, class _Tright>
static TaoArrayBase *realarray_arrayR_arrayL(TaoArrayBase *root, short rootype,
                                             TaoNumArray<_Tleft> *left,
                                             TaoNumArray<_Tright> *right,
                                             short oper, TaoInfoKit *infoKit) {
  if (!left || !right)
    return 0;

  short rtL = left->rttiNumeric();
  short rtR = right->rttiNumeric();
  if (rootype != rtL && rootype != rtR) {
    infoKit->warning("internal, unmatch operands in realarray_arrayR_arrayL()");
    return 0;
  }
  if (oper == TAO_PLUS || oper == TAO_MINUS || oper == TAO_PAIR_TIMES ||
      oper == TAO_PAIR_DIVIDE) {

    if (!left->alignShape(right->getDims())) {
      infoKit->warning("improper array shape for numarray operation");
      return root;
    }
    // Propagate temporary objects:
    if (rootype == rtL && left->refCount == 1) {
      // If "left" is not assigned to any variable,
      // it can be used temporary TaoRealArray in this evaluation.
      root = left;
      _Tleft *vcleft = left->getAsVector();
      _Tright *vcright = right->getAsVector();
      if (oper == TAO_PLUS) {
        for (size_t i = 0; i < left->size(); i++)
          vcleft[i] += (_Tleft)vcright[i];
      } else if (oper == TAO_MINUS) {
        for (size_t i = 0; i < left->size(); i++)
          vcleft[i] -= (_Tleft)vcright[i];
      } else if (oper == TAO_PAIR_TIMES) {
        for (size_t i = 0; i < left->size(); i++)
          vcleft[i] *= (_Tleft)vcright[i];
      } else {
        for (size_t i = 0; i < left->size(); i++)
          vcleft[i] /= (_Tleft)vcright[i];
      }
    } else if (rootype == rtR && right->refCount == 1) {
      // If "right" is not assigned to any variable,
      // it can be used temporary TaoRealArray in this evaluation.
      root = right;
      _Tleft *vcleft = left->getAsVector();
      _Tright *vcright = right->getAsVector();
      if (oper == TAO_PLUS) {
        for (size_t i = 0; i < left->size(); i++)
          vcright[i] += (_Tright)vcleft[i];
      } else if (oper == TAO_MINUS) {
        for (size_t i = 0; i < left->size(); i++)
          vcright[i] = (_Tright)vcleft[i] - vcright[i];
      } else if (oper == TAO_PAIR_TIMES) {
        for (size_t i = 0; i < left->size(); i++)
          vcright[i] *= (_Tright)vcleft[i];
      } else {
        for (size_t i = 0; i < left->size(); i++)
          vcright[i] = (_Tright)vcleft[i] / vcright[i];
      }
    } else {
      if (!root || root->refCount > 1) {
        // doubleArray->refCount>1:
        // doubleArray has been assigned to a variable, not free any more.
        // Unlock it, and allocate new object.
        if (root)
          root->locked = 0;
        root = allocRealArrayByType(rootype);
        root->refCount = 1;
        root->locked = 1;
      }
      root->resize(left->getDims());

      // "rootype" must be equal to "rtL" or "rtR":
      if (rootype == rtL) {
        TaoNumArray<_Tleft> *root2 = (TaoNumArray<_Tleft> *)root;
        root2->resize(left->getDims());

        _Tleft *vcroot = root2->getAsVector();
        _Tleft *vcleft = left->getAsVector();
        _Tright *vcright = right->getAsVector();
        if (oper == TAO_PLUS) {
          for (size_t i = 0; i < root->size(); i++)
            vcroot[i] = vcleft[i] + (_Tleft)vcright[i];
        } else if (oper == TAO_MINUS) {
          for (size_t i = 0; i < root->size(); i++)
            vcroot[i] = vcleft[i] - (_Tleft)vcright[i];
        } else if (oper == TAO_PAIR_TIMES) {
          for (size_t i = 0; i < root->size(); i++)
            vcroot[i] = vcleft[i] * (_Tleft)vcright[i];
        } else {
          for (size_t i = 0; i < root->size(); i++)
            vcroot[i] = vcleft[i] / (_Tleft)vcright[i];
        }
      } else {
        TaoNumArray<_Tright> *root2 = (TaoNumArray<_Tright> *)root;
        root2->resize(left->getDims());

        _Tright *vcroot = root2->getAsVector();
        _Tleft *vcleft = left->getAsVector();
        _Tright *vcright = right->getAsVector();
        if (oper == TAO_PLUS) {
          for (size_t i = 0; i < root->size(); i++)
            vcroot[i] = (_Tright)vcleft[i] + vcright[i];
        } else if (oper == TAO_MINUS) {
          for (size_t i = 0; i < root->size(); i++)
            vcroot[i] = (_Tright)vcleft[i] - vcright[i];
        } else if (oper == TAO_PAIR_TIMES) {
          for (size_t i = 0; i < root->size(); i++)
            vcroot[i] = (_Tright)vcleft[i] * vcright[i];
        } else {
          for (size_t i = 0; i < root->size(); i++)
            vcroot[i] = (_Tright)vcleft[i] / vcright[i];
        }
      }
    }
  } else if (oper == TAO_TIMES) {
    if (left->dim() != 2 || right->dim() != 2 ||
        left->dim(1) != right->dim(0)) {
      infoKit->warning("improper array shape for matrix multiplication");
      return 0;
    }
    size_t S = left->dim(1);
    size_t N = left->dim(0);
    size_t M = right->dim(1);
    valarray<size_t> dims(2);
    dims[0] = N;
    dims[1] = M;
    if (!root || root->refCount > 1) {
      if (root)
        root->locked = 0;
      root = allocRealArrayByType(rootype);
      root->refCount = 1;
      root->locked = 1;
    }
    root->resize(dims);

    if (rootype == rtL) {
      TaoNumArray<_Tleft> *root2 = (TaoNumArray<_Tleft> *)root;

      _Tleft *vcroot = root2->getAsVector();
      _Tleft *vcleft = left->getAsVector();
      _Tright *vcright = right->getAsVector();

      for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
          vcroot[i * M + j] = 0;
          for (size_t k = 0; k < S; k++)
            vcroot[i * M + j] += vcleft[i * S + k] * (_Tleft)vcright[k * M + j];
        }
      }
    } else {
      TaoNumArray<_Tright> *root2 = (TaoNumArray<_Tright> *)root;

      _Tright *vcroot = root2->getAsVector();
      _Tleft *vcleft = left->getAsVector();
      _Tright *vcright = right->getAsVector();

      for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
          vcroot[i * M + j] = 0;
          for (size_t k = 0; k < S; k++)
            vcroot[i * M + j] +=
                (_Tright)vcleft[i * S + k] * vcright[k * M + j];
        }
      }
    }
  } else if (TAO_ASSIGN < oper || oper <= TAO_ASSIGN_OR) {
    _Tright *vcright = right->getAsVector();
    root = (TaoArrayBase *)left;
    left->mapIndex();
    if (!left->alignShape(right->getDims())) {
      infoKit->warning("improper numarray shape for composite assignment");
      return root;
    }
    if (oper == TAO_ASSIGN_PLUS) {
      for (size_t i = 0; i < right->size(); i++)
        left->getMapped(i) += (_Tleft)vcright[i];
    } else if (oper == TAO_ASSIGN_MINUS) {
      for (size_t i = 0; i < right->size(); i++)
        left->getMapped(i) -= (_Tleft)vcright[i];
    } else if (oper == TAO_ASSIGN_TIMES) {
      for (size_t i = 0; i < right->size(); i++)
        left->getMapped(i) *= (_Tleft)vcright[i];
    } else if (oper == TAO_ASSIGN_DIVIDE) {
      for (size_t i = 0; i < right->size(); i++)
        left->getMapped(i) /= (_Tleft)vcright[i];
    } else if (oper == TAO_ASSIGN_MOD) {
      for (size_t i = 0; i < right->size(); i++)
        left->getMapped(i) =
            (_Tleft)((int)left->getMapped(i) % (int)vcright[i]);
    } else if (oper == TAO_ASSIGN_POW) {
      for (size_t i = 0; i < right->size(); i++)
        left->getMapped(i) =
            (_Tleft)mypow((double)left->getMapped(i), (double)vcright[i]);
    } else if (oper == TAO_ASSIGN_AND) {
      for (size_t i = 0; i < right->size(); i++)
        left->getMapped(i) = (_Tleft)left->getMapped(i) && vcright[i];
    } else if (oper == TAO_ASSIGN_OR) {
      for (size_t i = 0; i < right->size(); i++)
        left->getMapped(i) = (_Tleft)left->getMapped(i) || vcright[i];
    }
  }
  return root;
}
#endif
