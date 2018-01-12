
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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
////////////////////////////////////////////////////////////////////////////
#ifndef TAO_CPPTYPE_H
#define TAO_CPPTYPE_H

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

enum TaoCppTypes {
  TC_BASE = 0,
  TC_NUMBER = 1,
  TC_COMPLEX = 2,
  TC_STRING = 3,
  TC_ARRAY = 4,
  TC_HASH = 5,
  TAO_PLUGIN = 6,

  TC_NUM_ARRAY = 10,
  TC_ARRAY_BYTE = 11,
  TC_ARRAY_SHORT = 12,
  TC_ARRAY_INT = 13,
  TC_ARRAY_FLOAT = 14,
  TC_ARRAY_DOUBLE = 15,
  TC_ARRAY_COMPLEX = 16,
};

//==========================================================
/*
It's strongly recommended to use the following types ONLY
to pass objects between Tao interpreter and user-defined
modules.

The memory management for objects passed to and from Tao
interpreter is handled by the interpreter. The users are
reponsible to free objects which are created in the modules
and not passed to Tao interpreter.
*/
//==========================================================
//==========================================================
//! Base class for all Tao C++ types.
class TcBase {
public:
  TcBase() { reserved = 0; }
  virtual ~TcBase() {}

  //! Used for GC in Tao interpreter.
  //! Don't touch it!
  bool visited;

  //! Only used for holding a pointer of TaoXyz converted from TcXyz.
  //! Don't touch it!
  void *reserved;

  //! Get object type.
  virtual short type() const { return TC_BASE; }
  //! Get object Running Time Type Identification.
  //! It is different from type() only for numeric arrays.
  virtual short rtti() const { return TC_BASE; }

  //! Get value, used for TcNumber without casting it from TcBase.
  virtual double getValue() { return 0; }
  //! Get chars, used for TcString without casting it from TcBase.
  virtual char *getChars() { return 0; }
  //! Get the i-th element, used for TcArray without casting it from TcBase.
  virtual TcBase *getElement(int i) { return this; }

  //! Print with default format.
  virtual void print(ostream *out = 0, bool reserved = 0){};
};

//==========================================================
//==========================================================
//! Corresponding to TaoNumber.
class TcNumber : public TcBase {
  double value;

public:
  TcNumber(double val = 0) { value = val; }
  ~TcNumber() {}

  //! Set value.
  void setValue(double val) { value = val; }
  //! Get value.
  double getValue() { return value; }

  short type() const { return TC_NUMBER; }
  short rtti() const { return TC_NUMBER; }
  void print(ostream *out = 0, bool reserved = 0) {
    if (!out)
      out = &cout;
    *out << value;
  }
};
//==========================================================
//==========================================================
//! Corresponding to TaoComplex.
class TcComplex : public TcBase {
  double real, imag;

public:
  TcComplex(double rl = 0, double ig = 0) {
    real = rl;
    imag = ig;
  }
  ~TcComplex() {}

  //! Set both the real and imaginary parts.
  void setValue(double rl, double ig) {
    real = rl;
    imag = ig;
  }
  //! Set real part.
  void setReal(double rl) { real = rl; }
  //! Set imaginary part.
  void setImag(double ig) { imag = ig; }
  //! Get real part.
  double getReal() { return real; }
  //! Get imaginary part.
  double getImag() { return imag; }

  short type() const { return TC_COMPLEX; }
  short rtti() const { return TC_COMPLEX; }
  void print(ostream *out = 0, bool reserved = 0) {
    if (!out)
      out = &cout;
    *out << real << "+" << imag << "$";
  }
};

//==========================================================
//==========================================================
//! Corresponding to TaoString.
class TcString : public TcBase {
  char *chars;

public:
  TcString() { chars = (char *)calloc(0, sizeof(char)); }
  ~TcString() { free(chars); }

  //! Set chars.
  void setChars(const char *ch) {
    chars = (char *)realloc(chars, (strlen(ch) + 1) * sizeof(char));
    memcpy(chars, ch, (strlen(ch) + 1) * sizeof(char));
  }
  //! Get chars.
  char *getChars() { return chars; }

  short type() const { return TC_STRING; }
  short rtti() const { return TC_STRING; }
  void print(ostream *out = 0, bool reserved = 0) {
    if (!out)
      out = &cout;
    if (reserved)
      *out << "\"";
    *out << chars;
    if (reserved)
      *out << "\"";
  }
};

//==========================================================
//==========================================================
//! Corresponding to TaoArray.
class TcArray : public TcBase {
  bool printed;
  int lenArray;

  int numElem;
  TcBase **elements;

public:
  TcArray() {
    lenArray = numElem = 0;
    elements = 0;
    printed = 0;
  }
  //! Objects in "elements" are not freed!
  //! If an object of this type is passed to Tao interpreter,
  //! the Tao Garbage Collector will take care of this.
  //! Otherwise, the users are responsible for it.
  ~TcArray() { free(elements); }

  short type() const { return TC_ARRAY; }
  short rtti() const { return TC_ARRAY; }

  //! Get number of elements.
  int size() const { return numElem; }

  //! Get the i-th element.
  TcBase *getElement(int i) {
    if (i < 0 || i >= numElem)
      return 0;
    return elements[i];
  }

  //! Insert an element. Default behavior is like append.
  void insert(TcBase *tbase, int id = -1) {
    if (numElem + 2 > lenArray) {
      lenArray += 100;
      elements = (TcBase **)realloc(elements, lenArray * sizeof(TcBase *));
    }
    if (id < 0)
      elements[numElem] = tbase;
    else {
      for (int i = numElem; i > id; i--)
        elements[i] = elements[i - 1];
      elements[id] = tbase;
    }
    numElem++;
  }
  //! Replace an element.
  //! Return the replaced element for proper handling by users.
  //! If index is out of range, return 0.
  TcBase *replace(TcBase *tbase, int id) {
    if (id < 0 || id >= numElem)
      return 0;
    TcBase *tmp = elements[id];
    elements[id] = tbase;
    return tmp;
  }
  void print(ostream *out = 0, bool reserved = 0) {
    if (!out)
      out = &cout;
    if (printed) {
      // for cyclic structures:
      *out << "{...}";
      return;
    }
    printed = 1;
    *out << "{ ";
    for (int i = 0; i < numElem; i++) {
      elements[i]->print(out, 1);
      if (i + 1 < numElem)
        *out << ", ";
    }
    *out << " }";
    printed = 0;
  }
};
//==========================================================
//==========================================================

//! Corresponding to TaoHash.
class TcHash : public TcBase {
  bool printed;
  int lenArray;

  int numElem;

  char **keys;
  TcBase **elements;

  //! Search for "key", return TRUE if "key" is found in the hash:
  //! If "key" is found, "id" is set to the index of "key" in "keys",
  //! otherwise, "id" is the lowmost key which is upper than "key"
  bool keyToIndex(const char *key, int &id) {
    int first = 0;
    int last = numElem - 1;
    id = 0;
    while (first <= last) {
      int mid = (first + last) / 2;
      int cmp = strcmp(keys[mid], key);
      if (cmp == 0) {
        id = mid;
        return 1;
      } else if (cmp < 0) {
        first = mid + 1;
        id = first;
      } else {
        last = mid - 1;
      }
    }
    return 0;
  }

public:
  TcHash() {
    lenArray = numElem = 0;
    printed = 0;
  }
  //! Objects in "elements" are not freed!
  //! If an object of this type is passed to Tao interpreter,
  //! the Tao Garbage Collector will take care of this.
  //! Otherwise, the users are responsible for it.
  ~TcHash() {
    for (int i = 0; i < numElem; i++)
      free(keys[i]);
    free(elements);
  }

  short type() const { return TC_HASH; }
  short rtti() const { return TC_HASH; }

  //! Get number of elements.
  int size() const { return numElem; }

  //! Get i-th key.
  char *getKey(int i) { return keys[i]; }
  //! Get i-th value.
  TcBase *getValue(int i) { return elements[i]; }
  //! Get value by key.
  TcBase *getValue(char *key) {
    int id;
    if (keyToIndex(key, id))
      return elements[id];
    return 0;
  }

  //! Insert a pair of key/value into hash:
  //! If "key" already exists in the hash, the old value
  //! is returned for proper handling by user.
  TcBase *insert(const char *key, TcBase *value) {
    int id;
    bool exists = keyToIndex(key, id);
    if (exists) {
      TcBase *p = elements[id];
      elements[id] = value;
      return p;
    } else {
      if (numElem + 2 > lenArray) {
        lenArray += 100;
        keys = (char **)realloc(keys, lenArray * sizeof(char *));
        elements = (TcBase **)realloc(elements, lenArray * sizeof(TcBase *));
      }
      numElem++;
      for (int i = id + 1; i < numElem; i++) {
        keys[i] = keys[i - 1];
        elements[i] = elements[i - 1];
      }
      char *tmp = (char *)calloc(strlen(key) + 1, sizeof(char));
      memcpy(tmp, key, (strlen(key) + 1) * sizeof(char));
      keys[id] = tmp;
      elements[id] = value;
    }
    return 0;
  }
  void print(ostream *out = 0, bool reserved = 0) {
    if (!out)
      out = &cout;
    if (printed) {
      *out << "{...}";
      return;
    }
    printed = 1;
    *out << "{ ";
    for (int i = 0; i < numElem; i++) {
      *out << "\"" << keys[i] << "\"=>";
      elements[i]->print(out, 1);
      if (i + 1 < numElem)
        *out << ", ";
    }
    *out << " }";
    printed = 0;
  }
};

//==========================================================
//==========================================================
//! The followings is a simple version of TaoNumArrays,
//! supporting arbitary dimensions.

//! Corresponding to TaoArrayBase.
class TcNumArray : public TcBase {
protected:
  //! Number of dimensions.
  int ndim;

  //! Size in each dimension.
  int *dims;

  //! Accumulative dimensions, used for subindexing.
  int *dimAccum;

  //! Update accumulative dimensions.
  void updateDimAccum() {
    dimAccum = (int *)realloc(dimAccum, ndim * sizeof(int));
    for (int j = 0; j < ndim; j++)
      dimAccum[j] = 1;
    for (int i = ndim - 2; i >= 0; i--)
      dimAccum[i] = dimAccum[i + 1] * dims[i + 1];
  }
  //! Set shape.
  void setShape(int *ds, int nd) {
    free(dims);
    dims = ds;
    ndim = nd;
    updateDimAccum();
  }

  //! To resize the data array in the subclasses.
  virtual void resizeVector(int nnew, int nold){};

  virtual void printElement(int i, ostream *out){};

public:
  TcNumArray() {
    ndim = 1;
    dims = (int *)malloc(sizeof(int));
    dimAccum = (int *)malloc(sizeof(int));
    dims[0] = 0;
    dimAccum[0] = 1;
  }
  virtual ~TcNumArray() {
    free(dims);
    free(dimAccum);
  }

  //! Number of elements.
  int size() const { return dimAccum[0] * dims[0]; }
  //! Number of dimensions.
  int dim() const { return ndim; }
  //! Size in i-th dimension.
  int dim(int i) const { return dims[i]; }
  //! Get sizes in all dimensions.
  int *getDims() { return dims; }

  //! Get the flat index as in a vector given
  //! the subindices "index" as multi-dim array.
  int getFlatIndex(int *index) {
    int id = 0;
    for (int i = 0; i < ndim; i++)
      id += index[i] * dimAccum[i];
    return id;
  }
  //! Get multi-dim subindices given the flat index "id"
  //! as in a vector, and store them in "index".
  void getMulDimIndex(int id, int *index) {
    for (int i = ndim - 1; i >= 0; i--) {
      index[i] = id % dims[i];
      id /= dims[i];
    }
  }

  //! Resize to be a vector.
  void resize(int nnew) {
    int nold = size();
    dims = (int *)realloc(dims, 2 * sizeof(int));
    dims[0] = 1;
    dims[1] = nnew;
    ndim = 2;
    updateDimAccum();
    if (nnew == nold)
      return;
    resizeVector(nnew, nold);
  }
  //! Resize to be an multi-dim array,
  //! with shape specified by dim size "ds" and dim number "nd".
  void resize(int *ds, int nd) {
    if (nd == 1) {
      resize(ds[0]);
      return;
    }
    int nold = size();
    if (ndim != nd) {
      ndim = nd;
      dims = (int *)realloc(dims, ndim * sizeof(int));
    }
    for (int i = 0; i < ndim; i++)
      dims[i] = ds[i];
    updateDimAccum();
    if (size() == nold)
      return;
    resizeVector(size(), nold);
  }
  //! Reshape to a shape specified by dim size "ds" and dim number "nd".
  void reshape(int *ds, int nd) {
    int nnew = 1;
    for (int i = 0; i < nd; i++)
      nnew *= ds[i];
    if (nnew != size()) {
      cout << "Warning: improper reshaping\n";
      return;
    }

    if (ndim != nd) {
      ndim = nd;
      dims = (int *)realloc(dims, ndim * sizeof(int));
    }
    for (int j = 0; j < ndim; j++)
      dims[j] = ds[j];
    updateDimAccum();
  }

  short type() const { return TC_NUM_ARRAY; }
  virtual short rtti() const { return TC_NUM_ARRAY; }

  void print(ostream *out = 0, bool reserved = 0) {
    if (ndim < 2)
      return;
    int i;
    if (!out)
      out = &cout;
    if (ndim == 2 && (dims[0] == 1 || dims[1] == 1)) {
      // For vectors:
      *out << "[ ";
      for (i = 0; i < size(); i++) {
        printElement(i, out);
        if (i + 1 < size())
          *out << ", ";
      }
      if (dims[1] == 1)
        *out << " (T)]";
      else
        *out << " ]";
      return;
    }
    int *tmp = (int *)calloc(ndim, sizeof(int));
    for (i = 0; i < size(); i++) {
      int mod = i;
      for (int j = (int)ndim - 1; j >= 0; j--) {
        int res = mod % dims[j];
        mod /= dims[j];
        tmp[j] = res;
      }
      if (tmp[ndim - 1] == 0) {
        *out << "row(";
        for (int j = 0; j + 1 < ndim; j++)
          *out << tmp[j] << ",";
        *out << ":):\t";
      }
      printElement(i, out);
      if (i + 1 < size())
        *out << "\t";
      if (tmp[ndim - 1] == (dims[ndim - 1] - 1))
        *out << "\n";
    }
    free(tmp);
  }
};

//==========================================================
//==========================================================
typedef unsigned char Byte;
//! Corresponding to TaoByteArray.
class TcByteArray : public TcNumArray {
  Byte *values;
  Byte **ppters;

  void resizeVector(int nnew, int nold) {
    values = (Byte *)realloc(values, nnew * sizeof(Byte));
    for (int i = nold; i < nnew; i++)
      values[i] = 0;
  }

  void printElement(int i, ostream *out) { *out << (short)values[i]; }

public:
  TcByteArray() {
    values = NULL;
    ppters = NULL;
  };
  //! "values" are not freed!
  //! If an object of this type is passed to Tao interpreter,
  //! the Tao Garbage Collector will take care of this.
  //! Otherwise, the users are responsible for it.
  ~TcByteArray() { free(ppters); }

  //! Get "id"-th element as in a vector.
  Byte &element(int id) { return values[id]; }
  //! Get element specified by a subindex.
  Byte &element(int *index) { return values[getFlatIndex(index)]; }

  //! Get as vector.
  Byte *getAsVector() { return values; }
  //! Get as matrix.
  Byte **getAsMatrix(int nR = -1) {
    if (nR <= 0 || size() % nR != 0)
      nR = dims[0];
    int nC = size() / nR;
    ppters = (Byte **)realloc(ppters, nR * sizeof(Byte *));
    for (int i = 0; i < nR; i++)
      ppters[i] = &values[i * nC];
    return ppters;
  }

  //! This function is used to pass data pointer from TaoNumArray
  //! to TcNumArray (or the reverse) without copying the data chunk.
  //! It not clear that it is the best way without any problems.
  //! Otherwise, it might be better to copy the data chunk.
  void setByteArray(Byte *vals, int *dm, int nd) {
    free(values);
    values = vals;
    setShape(dm, nd);
  }

  short rtti() const { return TC_ARRAY_BYTE; }
};
//! Corresponding to TaoShortArray.
class TcShortArray : public TcNumArray {
  short *values;
  short **ppters;

  void resizeVector(int nnew, int nold) {
    values = (short *)realloc(values, nnew * sizeof(short));
    for (int i = nold; i < nnew; i++)
      values[i] = 0;
  }

  void printElement(int i, ostream *out) { *out << values[i]; }

public:
  TcShortArray() {
    values = NULL;
    ppters = NULL;
  };
  //! "values" are not freed!
  //! See comments in TcByteArray.
  ~TcShortArray() { free(ppters); }

  //! Get "id"-th element as in a vector.
  short &element(int id) { return values[id]; }
  //! Get element specified by a subindex.
  short &element(int *index) { return values[getFlatIndex(index)]; }

  //! Get as vector.
  short *getAsVector() { return values; }
  //! Get as matrix.
  short **getAsMatrix(int nR = -1) {
    if (nR <= 0 || size() % nR != 0)
      nR = dims[0];
    int nC = size() / nR;
    ppters = (short **)realloc(ppters, nR * sizeof(short *));
    for (int i = 0; i < nR; i++)
      ppters[i] = &values[i * nC];
    return ppters;
  }

  //! See comments in TcByteArray.
  void setShortArray(short *vals, int *dm, int nd) {
    free(values);
    values = vals;
    setShape(dm, nd);
  }

  short rtti() const { return TC_ARRAY_SHORT; }
};
//! Corresponding to TaoIntArray.
class TcIntArray : public TcNumArray {
  int *values;
  int **ppters;

  void resizeVector(int nnew, int nold) {
    values = (int *)realloc(values, nnew * sizeof(int));
    for (int i = nold; i < nnew; i++)
      values[i] = 0;
  }

  void printElement(int i, ostream *out) { *out << values[i]; }

public:
  TcIntArray() {
    values = NULL;
    ppters = NULL;
  };
  //! "values" are not freed!
  //! See comments in TcByteArray.
  ~TcIntArray() { free(ppters); }

  //! Get "id"-th element as in a vector.
  int &element(int id) { return values[id]; }
  //! Get element specified by a subindex.
  int &element(int *index) { return values[getFlatIndex(index)]; }

  //! Get as vector.
  int *getAsVector() { return values; }
  //! Get as matrix.
  int **getAsMatrix(int nR = -1) {
    if (nR <= 0 || size() % nR != 0)
      nR = dims[0];
    int nC = size() / nR;
    ppters = (int **)realloc(ppters, nR * sizeof(int *));
    for (int i = 0; i < nR; i++)
      ppters[i] = &values[i * nC];
    return ppters;
  }

  //! See comments in TcByteArray.
  void setIntArray(int *vals, int *dm, int nd) {
    free(values);
    values = vals;
    setShape(dm, nd);
  }

  short rtti() const { return TC_ARRAY_INT; }
};
//! Corresponding to TaoFloatArray.
class TcFloatArray : public TcNumArray {
  float *values;
  float **ppters;

  void resizeVector(int nnew, int nold) {
    values = (float *)realloc(values, nnew * sizeof(float));
    for (int i = nold; i < nnew; i++)
      values[i] = 0;
  }

  void printElement(int i, ostream *out = 0) { *out << values[i]; }

public:
  TcFloatArray() {
    values = NULL;
    ppters = NULL;
  };
  //! "values" are not freed!
  //! See comments in TcByteArray.
  ~TcFloatArray() { free(ppters); }

  //! Get "id"-th element as in a vector.
  float &element(int id) { return values[id]; }
  //! Get element specified by a subindex.
  float &element(int *index) { return values[getFlatIndex(index)]; }

  //! Get as vector.
  float *getAsVector() { return values; }
  //! Get as matrix.
  float **getAsMatrix(int nR = -1) {
    if (nR <= 0 || size() % nR != 0)
      nR = dims[0];
    int nC = size() / nR;
    ppters = (float **)realloc(ppters, nR * sizeof(float *));
    for (int i = 0; i < nR; i++)
      ppters[i] = &values[i * nC];
    return ppters;
  }

  //! See comments in TcByteArray.
  void setFloatArray(float *vals, int *dm, int nd) {
    free(values);
    values = vals;
    setShape(dm, nd);
  }

  short rtti() const { return TC_ARRAY_FLOAT; }
};
//! Corresponding to TaoDoubleArray.
class TcDoubleArray : public TcNumArray {
  double *values;
  double **ppters;

  void resizeVector(int nnew, int nold) {
    values = (double *)realloc(values, nnew * sizeof(double));
    for (int i = nold; i < nnew; i++)
      values[i] = 0;
  }

  void printElement(int i, ostream *out = 0) { *out << values[i]; }

public:
  TcDoubleArray() {
    values = NULL;
    ppters = NULL;
  };
  //! "values" are not freed!
  //! See comments in TcByteArray.
  ~TcDoubleArray() { free(ppters); }

  //! Get "id"-th element as in a vector.
  double &element(int id) { return values[id]; }
  //! Get element specified by a subindex.
  double &element(int *index) { return values[getFlatIndex(index)]; }

  //! Get as vector.
  double *getAsVector() { return values; }
  //! Get as matrix.
  double **getAsMatrix(int nR = -1) {
    if (nR <= 0 || size() % nR != 0)
      nR = dims[0];
    int nC = size() / nR;
    ppters = (double **)realloc(ppters, nR * sizeof(double *));
    for (int i = 0; i < nR; i++)
      ppters[i] = &values[i * nC];
    return ppters;
  }

  //! See comments in TcByteArray.
  void setDoubleArray(double *vals, int *dm, int nd) {
    free(values);
    values = vals;
    setShape(dm, nd);
  }

  short rtti() const { return TC_ARRAY_DOUBLE; }
};

//! Corresponding to TaoCompArray<double>.
class TcComplexArray : public TcNumArray {
  double *reals;
  double *imags;
  double **ppters;

  void resizeVector(int nnew, int nold) {
    reals = (double *)realloc(reals, nnew * sizeof(double));
    imags = (double *)realloc(imags, nnew * sizeof(double));
    for (int i = nold; i < nnew; i++)
      reals[i] = 0;
    for (int j = nold; j < nnew; j++)
      imags[j] = 0;
  }

  void printElement(int i, ostream *out = 0) {
    *out << reals[i] << "+" << imags[i] << "$";
  }

public:
  TcComplexArray() {
    reals = imags = NULL;
    ppters = NULL;
  };
  ~TcComplexArray() {
    free(reals);
    free(imags);
    free(ppters);
  }

  double &real(int id) { return reals[id]; }
  double &imag(int id) { return imags[id]; }
  double &real(int *index) { return reals[getFlatIndex(index)]; }
  double &imag(int *index) { return imags[getFlatIndex(index)]; }

  //! Get as vector. If r=TRUE, get the real part.
  double *getAsVector(bool r = 1) {
    if (r)
      return reals;
    else
      return imags;
  }
  //! Get as matrix. If r=TRUE, get the real part.
  double **getAsMatrix(int nR = -1, bool getreal = 1) {
    if (nR <= 0 || size() % nR != 0)
      nR = dims[0];
    int nC = size() / nR;
    ppters = (double **)realloc(ppters, nR * sizeof(double *));
    if (getreal)
      for (int i = 0; i < nR; i++)
        ppters[i] = &reals[i * nC];
    else
      for (int i = 0; i < nR; i++)
        ppters[i] = &imags[i * nC];
    return ppters;
  }

  short rtti() const { return TC_ARRAY_COMPLEX; }
};

#endif
