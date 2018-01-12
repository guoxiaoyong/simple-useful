
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
#include "taoCpptype.h"
#include "taoModule.h"
#include "taoNumeric.h"
#include "taoRoutine.h"
#include "taoUtility.h"

////////////////////////////////////////////////////////////////
short TaoComplex::RTTI = TAO_Complex;
short TaoComplex::rtti() const { return RTTI; }
short TaoByteArray::RTTI = TAO_ByteArray;
short TaoShortArray::RTTI = TAO_ShortArray;
short TaoIntArray::RTTI = TAO_IntArray;
short TaoFloatArray::RTTI = TAO_FloatArray;
short TaoDoubleArray::RTTI = TAO_DoubleArray;
/*
template<class _Tp>
short TaoRealArray<_Tp>::RTTI=TAO_RealArray;
template<class _Tp>
short TaoRealArray<_Tp>::rtti()const{ return RTTI; }
template<class _Tp>
short TaoCompArray<_Tp>::RTTI=TAO_CompArray;
template<class _Tp>
short TaoCompArray<_Tp>::rtti()const{ return RTTI; }

template<class _Tp>
string TaoRealArray<_Tp>::strStart="( ";
template<class _Tp>
string TaoRealArray<_Tp>::strDelim=", ";
template<class _Tp>
string TaoRealArray<_Tp>::strEnd=" )";
*/
////////////////////////////////////////////////////////////////
TaoComplex::TaoComplex(double real, double imag) : cmplx(real, imag) {
  cpptype = 0;
}
TaoComplex::TaoComplex(const complex<double> &com) : cmplx(com) { cpptype = 0; }
TcBase *TaoComplex::toCppType() {
  if (!cpptype)
    cpptype = new TcComplex;
  cpptype->setValue(cmplx.real(), cmplx.imag());
  cpptype->reserved = this;

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoComplex::fromCppType() {
  if (!cpptype)
    return;
  cmplx = complex<double>(cpptype->getReal(), cpptype->getImag());

  if (recycler)
    recycler->pushTcObject(cpptype);
  cpptype = 0;
}
void TaoComplex::print(ostream *out) {
  *out << cmplx.real();
  if (cmplx.imag() >= 0.0)
    *out << "+";
  *out << cmplx.imag() << "$";
}
TaoBase *TaoComplex::duplicate() { return (TaoBase *)new TaoComplex(cmplx); }
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
TaoArrayBase::TaoArrayBase() {
  timeTag = 0;
  whichIndex = 0;
  blIdMapped = 0;
  onAllElements = 1;
  numIndexElem = 0;
}
TaoArrayBase::~TaoArrayBase() {
  if (whichIndex) {
    if (whichIndex->refCount == 1)
      delete whichIndex;
    else {
      // somebody is still using it:
      whichIndex->refCount--;
      whichIndex->locked = 0;
    }
  }
}
TaoIntArray *TaoArrayBase::dimSize() {
  whichIndex = TaoTryAllocator<TaoIntArray>::tryAllocate(whichIndex);

  whichIndex->resize(dims.size());
  int *vec = whichIndex->getAsVector();
  for (size_t i = 0; i < dims.size(); i++)
    vec[i] = dims[i];
  return whichIndex;
}
void TaoArrayBase::updateDimAccum() {
  dimAccum.resize(dims.size(), 1);
  for (int i = (int)dims.size() - 2; i >= 0; i--)
    dimAccum[i] = dimAccum[i + 1] * dims[i + 1];
}
void TaoArrayBase::resize(size_t size) {
  size_t oldSize = dims[0] * dimAccum[0];
  dims.resize(2);
  dims[0] = 1;
  dims[1] = size;
  updateDimAccum();
  resetSubIndex();
  if (size == oldSize)
    return;

  resizeVector(size, oldSize);
  //	numArray->resize( dims[0]*dimAccum[0], 0 );
}
void TaoArrayBase::resize(valarray<size_t> &newDims) {
  if (newDims.size() == 1) {
    resize(newDims[0]);
    return;
  }
  //	valarray<size_t> oldDims(dims);
  //	valarray<size_t> oldDimAcc(dimAccum);

  size_t oldSize = size(); // oldDims[0]*dimAccum[0];
  dims.resize(newDims.size());
  dims = newDims;
  updateDimAccum();
  resetSubIndex();
  if (dims[0] * dimAccum[0] == oldSize)
    return;

  //	if( dims.size()!=oldDims.size() ){
  resizeVector(dims[0] * dimAccum[0], oldSize);
  //	numArray->resize( dims[0]*dimAccum[0], 0 );
  //		return;
  //	}
  /*
          valarray<_Tp> *temp=new valarray<_Tp>( dims[0]*dimAccum[0], 0 );
          valarray<size_t> dmin=oldDims;
          size_t i;
          for(i=0;i<dims.size();i++){
                  if(dmin[i]>dims[i]) dmin[i]=dims[i];
          }
          delete numArray;
          numArray=temp;
  */
}
void TaoArrayBase::resetSubIndex() {
  subIndex.resize(dims.size());
  for (size_t i = 0; i < dims.size(); i++) {
    subIndex[i].resize(dims[i]);
    for (size_t j = 0; j < dims[i]; j++)
      subIndex[i][j] = j;
  }
  numIndexElem = size();
  onAllElements = 1;
}
void TaoArrayBase::exportSubIndex(valarray<int> &index, size_t id) {
  for (int j = (int)dims.size() - 1; j >= 0; j--) {
    index[j] = id % dims[j];
    id /= dims[j];
  }
}
void TaoArrayBase::exportAllSubIndex(valarray<valarray<int>> &index) {
  index.resize(size());
  for (size_t i = 0; i < index.size(); i++) {
    index[i].resize(dim());
    int mod = i;
    // See comments in print()
    for (int j = (int)dims.size() - 1; j >= 0; j--) {
      index[i][j] = mod % dims[j];
      mod /= dims[j];
    }
  }
}
void TaoArrayBase::mapIndex() {
  if (blIdMapped)
    return;
  //	int ts=multi[0]*index[0].size();

  idMapped.resize(numIndexElem);

  //	valarray<size_t>	ids;
  //	ids.resize( dims.size() );

  // Have a look at this to see how indices are enumerated.
  // 4	3	2:
  // 6  2  1: multi
  //         res:
  // i=0:	0	0	0
  // i=1:	0	0	1
  // i=2:	0	1	0
  // i=3:	0	1	1
  // i=4:	0	2	0
  // i=5:	0	2	1
  // i=6:	1	0	0
  // i=numIndexElem-1:	...

  for (size_t i = 0; i < numIndexElem; i++) {
    idMapped[i] = 0;
    int mod = i;
    for (int j = (int)subIndex.size() - 1; j >= 0; j--) {
      int res = mod % subIndex[j].size();
      mod /= subIndex[j].size();
      idMapped[i] += subIndex[j][res] * dimAccum[j];
    }
  }
}
void TaoArrayBase::setSubIndex(vector<TaoArithBase *> &idArith,
                               TaoInfoKit *infoKit) {
  onAllElements = 0;
  blIdMapped = 0;
  numDimSub = 0;
  numIndexElem = 1;
  if (idArith.size() == 1) {
    TaoBase *tbase = idArith[0]->getResult();
    if (tbase)
      tbase = tbase->dereference();
    short rtti = -1;
    if (tbase)
      rtti = tbase->rtti();
    if (rtti == TAO_RealArray) {
      TaoArrayBase *array = (TaoArrayBase *)tbase;
      if (array->size() > 0 && dims[0] > 1 && dims[1] > 1 &&
          array->dim() == 2 && array->dim(1) == dims.size()) {
        // Set no-slicing index:
        // array[b], where "b" is an array and each row
        // of "b" is a subindex of "array".
        // Usually, this kind of subindexing can be used
        // with which() together:
        //		id = which( array[,] == 5 );
        //		subarray = array[ id ];
        size_t n = array->size() / dims.size();
        blIdMapped = 1;
        idMapped.resize(n);
        for (size_t i = 0; i < n; i++) {
          idMapped[i] = 0;
          for (size_t j = 0; j < dims.size(); j++) {
            double val = array->getDouble(i * dims.size() + j);
            if (val >= 0)
              idMapped[i] += (size_t)val * dimAccum[j];
            else
              infoKit->warning("negative value in subindex ignored");
          }
        }
        numIndexElem = n;
        numDimSub = 0;
        return;
      }
    }
    if (dims.size() == 2 && (dims[0] == 1 || dims[1] == 1)) {
      // For vectors:
      int k = 0;
      if (dims[0] == 1) {
        subIndex[0].resize(1, 0);
        k = 1;
      } else {
        subIndex[1].resize(1, 0);
      }
      TaoIndexMaker::makeIndex(subIndex[k], dims[k], idArith[0], 0, infoKit);

      numIndexElem *= subIndex[k].size();
      if (subIndex[k].size() > 1)
        numDimSub++;
    } else {
      // Subindex must have been reset:
      TaoIndexMaker::makeIndex(subIndex[0], dims[0], idArith[0], 0, infoKit);
      numIndexElem = dimAccum[0] * subIndex[0].size();
      numDimSub = dims.size() - 1;
      if (subIndex[0].size() > 1)
        numDimSub++;
    }
  } else {
    size_t i;
    size_t n = idArith.size();
    if (n > dims.size())
      n = dims.size();

    for (i = 0; i < n; i++) {
      TaoIndexMaker::makeIndex(subIndex[i], dims[i], idArith[i], 0, infoKit);
      numIndexElem *= subIndex[i].size();
      if (subIndex[i].size() > 1)
        numDimSub++;
    }
    for (i = idArith.size(); i < dims.size(); i++) {
      subIndex[i].resize(dims[i]);
      for (size_t j = 0; j < dims[i]; j++)
        subIndex[i][j] = j;
      numIndexElem *= dims[i];

      if (subIndex[i].size() > 1)
        numDimSub++;
    }
  }
}
void TaoArrayBase::makeSubDims(valarray<size_t> &ds) {
  if (numDimSub == 0) {
    // It's vector, get by no-slicing index:
    // See comments in setSubIndex():
    ds.resize(2);
    ds[0] = 1;
    ds[1] = idMapped.size();
  } else if (numDimSub == 1) {
    // It's vector:
    ds.resize(2);
    int n = 0;
    for (size_t i = 0; i < dims.size(); i++) {
      if (subIndex[i].size() > 1) {
        n = subIndex[i].size();
        break;
      }
    }
    if (subIndex[dims.size() - 1].size() == 1) {
      // It's column vector:
      ds[1] = 1;
      ds[0] = n;
    } else {
      ds[0] = 1;
      ds[1] = n;
    }
  } else {
    ds.resize(numDimSub);
    int n = 0;
    for (size_t i = 0; i < dims.size(); i++) {
      if (subIndex[i].size() > 1) {
        ds[n] = subIndex[i].size();
        n++;
      }
    }
  }
}
bool TaoArrayBase::alignShape(valarray<size_t> &dms) {
  if (dms.size() != dims.size())
    return 0;

  for (size_t i = 0; i < dims.size(); i++) {
    if (dms[i] != subIndex[i].size())
      return 0;
  }
  return 1;
}
TaoIntArray *TaoArrayBase::numWhich(TaoArithBase *blArith,
                                    vector<TaoReference *> &varTran) {
  vector<vector<int>> ids;
  vector<TaoNumber *> tmpNum;
  for (size_t k = 0; k < varTran.size(); k++) {
    tmpNum.push_back(new TaoNumber);
    tmpNum[k]->refCount = 1;
    if (k < subIndex.size() + 1)
      varTran[k]->content = tmpNum[k];
  }

  bool isVector = 0;
  if (dims.size() == 2 && (dims[0] == 1 || dims[1] == 1))
    isVector = 1;

  vector<int> one;
  for (size_t i = 0; i < numIndexElem; i++) {
    int idmap = 0;
    if (isVector) {
      if (dims[0] == 1)
        idmap = subIndex[1][i];
      else
        idmap = subIndex[0][i];

      tmpNum[1] = TaoTryAllocator<TaoNumber>::tryAllocate(tmpNum[1]);
      tmpNum[1]->value = idmap;
      varTran[1]->content = tmpNum[1];
    } else {
      int mod = i;
      one.resize(subIndex.size());
      for (int j = (int)subIndex.size() - 1; j >= 0; j--) {
        one[j] = mod % subIndex[j].size();
        mod /= subIndex[j].size();

        tmpNum[j + 1] = TaoTryAllocator<TaoNumber>::tryAllocate(tmpNum[j + 1]);
        tmpNum[j + 1]->value = subIndex[j][one[j]];

        tmpNum[j + 1]->value = subIndex[j][one[j]];
        if (j + 1 < (int)varTran.size())
          varTran[j + 1]->content = tmpNum[j + 1];
        idmap += subIndex[j][one[j]] * dimAccum[j];
      }
    }
    tmpNum[0] = TaoTryAllocator<TaoNumber>::tryAllocate(tmpNum[0]);
    tmpNum[0]->value = getDouble(idmap);
    varTran[0]->content = tmpNum[0];

    blArith->compute();
    if (isVector) {
      if (blArith->bulean)
        one.push_back(idmap);
    } else {
      if (blArith->bulean)
        ids.push_back(one);
    }
  }
  if (isVector)
    ids.push_back(one);

  //	for(size_t k=0;k<varTran.size();k++){
  //		if( tmpNum[k]->refCount==0 )
  //			delete tmpNum[k];
  //	}
  return TaoIndexMaker::whichIndexing(ids, whichIndex);
}
TaoIntArray *TaoArrayBase::comWhich(TaoArithBase *blArith,
                                    vector<TaoReference *> &varTran) {
  vector<vector<int>> ids;
  vector<TaoNumber *> tmpNum;
  for (size_t k = 1; k < varTran.size(); k++) {
    tmpNum.push_back(new TaoNumber);
    tmpNum[k - 1]->refCount = 1;
    if (k < subIndex.size() + 1)
      varTran[k]->content = tmpNum[k - 1];
  }
  TaoComplex *comValue = new TaoComplex();

  bool isVector = 0;
  if (dims.size() == 2 && (dims[0] == 1 || dims[1] == 1))
    isVector = 1;

  vector<int> one;
  for (size_t i = 0; i < numIndexElem; i++) {
    int idmap = 0;
    if (isVector) {
      if (dims[0] == 1)
        idmap = subIndex[1][i];
      else
        idmap = subIndex[0][i];

      tmpNum[0] = TaoTryAllocator<TaoNumber>::tryAllocate(tmpNum[0]);
      tmpNum[0]->value = idmap;
      varTran[1]->content = tmpNum[0];
    } else {
      int mod = i;
      one.resize(subIndex.size());
      for (int j = (int)subIndex.size() - 1; j >= 0; j--) {
        one[j] = mod % subIndex[j].size();
        mod /= subIndex[j].size();

        tmpNum[j] = TaoTryAllocator<TaoNumber>::tryAllocate(tmpNum[j]);
        tmpNum[j]->value = subIndex[j][one[j]];

        tmpNum[j]->value = subIndex[j][one[j]];
        if (j + 1 < (int)varTran.size())
          varTran[j + 1]->content = tmpNum[j];
        idmap += subIndex[j][one[j]] * dimAccum[j];
      }
    }
    comValue = TaoTryAllocator<TaoComplex>::tryAllocate(comValue);
    comValue->cmplx = getComplex(idmap);
    varTran[0]->content = comValue;

    blArith->compute();
    if (isVector) {
      if (blArith->bulean)
        one.push_back(idmap);
    } else {
      if (blArith->bulean)
        ids.push_back(one);
    }
  }
  if (isVector)
    ids.push_back(one);

  //	for(size_t k=0;k<varTran.size();k++){
  //		if( tmpNum[k]->refCount==0 )
  //			delete tmpNum[k];
  //	}
  return TaoIndexMaker::whichIndexing(ids, whichIndex);
}
void TaoArrayBase::numFuncApply(short funType, TaoInfoKit *infoKit) {
  bool isVector = 0;
  if (dims.size() == 2 && (dims[0] == 1 || dims[1] == 1))
    isVector = 1;

  for (size_t i = 0; i < numIndexElem; i++) {
    int idmap = 0;
    if (isVector) {
      if (dims[0] == 1)
        idmap = subIndex[1][i];
      else
        idmap = subIndex[0][i];
    } else {
      int mod = i;
      for (int j = (int)subIndex.size() - 1; j >= 0; j--) {
        int jd = mod % subIndex[j].size();
        mod /= subIndex[j].size();

        idmap += subIndex[j][jd] * dimAccum[j];
      }
    }

    if (funType == TAO_abs)
      setDouble(idmap, abs(getDouble(idmap)));
    else if (funType == TAO_acos)
      setDouble(idmap, acos(getDouble(idmap)));
    else if (funType == TAO_asin)
      setDouble(idmap, asin(getDouble(idmap)));
    else if (funType == TAO_atan)
      setDouble(idmap, atan(getDouble(idmap)));
    else if (funType == TAO_cos)
      setDouble(idmap, cos(getDouble(idmap)));
    else if (funType == TAO_cosh)
      setDouble(idmap, cosh(getDouble(idmap)));
    else if (funType == TAO_exp)
      setDouble(idmap, exp(getDouble(idmap)));
    else if (funType == TAO_log)
      setDouble(idmap, log(getDouble(idmap)));
    else if (funType == TAO_randgs)
      setDouble(idmap, getDouble(idmap));
    else if (funType == TAO_rand)
      setDouble(idmap, rand() / (RAND_MAX + 1.0));
    else if (funType == TAO_sin)
      setDouble(idmap, sin(getDouble(idmap)));
    else if (funType == TAO_sinh)
      setDouble(idmap, sinh(getDouble(idmap)));
    else if (funType == TAO_sqrt)
      setDouble(idmap, sqrt(getDouble(idmap)));
    else if (funType == TAO_tan)
      setDouble(idmap, tan(getDouble(idmap)));
    else if (funType == TAO_tanh)
      setDouble(idmap, tanh(getDouble(idmap)));
    else
      infoKit->warning("no such function for real number");
  }
}
void TaoArrayBase::numIterate(TaoArithBase *arith,
                              vector<TaoReference *> &varTran, bool apply,
                              TaoInfoKit *infoKit) {
  TaoRoutine *rout = 0;

  if (arith->nonTrans) {
    short rtti = arith->rtti();
    if (rtti == TAO_ArithVariable || rtti == TAO_ArithChain ||
        rtti == TAO_ArithConst) {
      arith->compute();
      TaoBase *tbase = arith->getResult();
      if (tbase)
        tbase = tbase->dereference();
      short rt = -1;
      if (tbase)
        rt = tbase->rtti();
      if (rt == TAO_Module) {
        rout = ((TaoModule *)tbase)->getRoutine();
        rout->setDetachParam(0);
        for (size_t k = 0; k < varTran.size(); k++)
          rout->passParam(k, varTran[k]);
      } else if (rt == TAO_String) {
        map<string, int> &funcIntern = TaoModule::modKey.funcIntern;
        if (funcIntern.find(arith->myChars) != funcIntern.end()) {
          short funtype = funcIntern[arith->myChars];
          if (funtype <= TAO_tan) {
            numFuncApply(funtype, infoKit);
            return;
          } else {
            infoKit->warning("the string specifies no basic math function");
            return;
          }
        } else {
          infoKit->warning("the string specifies no basic math function");
          return;
        }
      }
    }
  }

  vector<TaoNumber *> tmpNum;

  for (size_t k = 0; k < varTran.size(); k++) {
    tmpNum.push_back(new TaoNumber);
    tmpNum[k]->refCount = 1;
    tmpNum[k]->locked = 1;
    if (k < subIndex.size() + 1)
      varTran[k]->content = tmpNum[k];
  }

  bool isVector = 0;
  if (dims.size() == 2 && (dims[0] == 1 || dims[1] == 1))
    isVector = 1;

  for (size_t i = 0; i < numIndexElem; i++) {
    int idmap = 0;
    if (isVector) {
      if (dims[0] == 1)
        idmap = subIndex[1][i];
      else
        idmap = subIndex[0][i];

      tmpNum[1] = TaoTryAllocator<TaoNumber>::tryAllocate(tmpNum[1]);
      tmpNum[1]->value = idmap;
      varTran[1]->content = tmpNum[1];
    } else {
      int mod = i;
      for (int j = (int)subIndex.size() - 1; j >= 0; j--) {
        int jd = mod % subIndex[j].size();
        mod /= subIndex[j].size();

        tmpNum[j + 1] = TaoTryAllocator<TaoNumber>::tryAllocate(tmpNum[j + 1]);
        tmpNum[j + 1]->value = subIndex[j][jd];
        if (j + 1 < (int)varTran.size())
          varTran[j + 1]->content = tmpNum[j + 1];

        idmap += subIndex[j][jd] * dimAccum[j];
      }
    }
    tmpNum[0] = TaoTryAllocator<TaoNumber>::tryAllocate(tmpNum[0]);
    tmpNum[0]->value = getDouble(idmap);
    varTran[0]->content = tmpNum[0];

    if (rout) {
      rout->run(0);

      if (apply) {
        TaoBase *result = rout->getResult();
        if (result)
          result = result->dereference();
        if (!result || result->rtti() != TAO_Number)
          infoKit->warning("non-numeric result to apply");
        else
          setDouble(idmap, ((TaoNumber *)result)->value);
      }
      //(*numArray)[idmap]=((TaoNumber*)result)->value;
    } else {
      arith->compute();
      if (apply)
        setDouble(idmap, arith->myValue);
      //(*numArray)[idmap]=arith->myValue;
    }
  }
  if (rout) {
    rout->detachOldParams();
    rout->setDetachParam(1);
    rout->setParamPassed(0);
  }
  //	for(size_t k=0;k<varTran.size();k++){
  //		if( tmpNum[k]->refCount==1 )
  //			delete tmpNum[k];
  //	}
}
void TaoArrayBase::comFuncApply(short funType, TaoInfoKit *infoKit) {
  bool isVector = 0;
  if (dims.size() == 2 && (dims[0] == 1 || dims[1] == 1))
    isVector = 1;

  for (size_t i = 0; i < numIndexElem; i++) {
    int idmap = 0;
    if (isVector) {
      if (dims[0] == 1)
        idmap = subIndex[1][i];
      else
        idmap = subIndex[0][i];
    } else {
      int mod = i;
      for (int j = (int)subIndex.size() - 1; j >= 0; j--) {
        int jd = mod % subIndex[j].size();
        mod /= subIndex[j].size();

        idmap += subIndex[j][jd] * dimAccum[j];
      }
    }

    if (funType == TAO_abs)
      setComplex(idmap, abs(getComplex(idmap)));
    else if (funType == TAO_cos)
      setComplex(idmap, cos(getComplex(idmap)));
    else if (funType == TAO_cosh)
      setComplex(idmap, cosh(getComplex(idmap)));
    else if (funType == TAO_exp)
      setComplex(idmap, exp(getComplex(idmap)));
    else if (funType == TAO_log)
      setComplex(idmap, log(getComplex(idmap)));
    else if (funType == TAO_sin)
      setComplex(idmap, sin(getComplex(idmap)));
    else if (funType == TAO_sinh)
      setComplex(idmap, sinh(getComplex(idmap)));
    else if (funType == TAO_sqrt)
      setComplex(idmap, sqrt(getComplex(idmap)));
    else
      infoKit->warning("no such function for complex number");
  }
}
void TaoArrayBase::comIterate(TaoArithBase *arith,
                              vector<TaoReference *> &varTran, bool apply,
                              TaoInfoKit *infoKit) {
  TaoRoutine *rout = 0;
  if (arith->nonTrans) {
    short rtti = arith->rtti();
    if (rtti == TAO_ArithVariable || rtti == TAO_ArithChain ||
        rtti == TAO_ArithConst) {
      arith->compute();
      TaoBase *tbase = arith->getResult();
      if (tbase)
        tbase = tbase->dereference();
      short rt = -1;
      if (tbase)
        rt = tbase->rtti();
      if (rt == TAO_Module) {
        rout = ((TaoModule *)tbase)->getRoutine();
        rout->setDetachParam(0);
        for (size_t k = 0; k < varTran.size(); k++)
          rout->passParam(k, varTran[k]);
      } else if (rt == TAO_String) {
        map<string, int> &funcIntern = TaoModule::modKey.funcIntern;
        if (funcIntern.find(arith->myChars) != funcIntern.end()) {
          short funtype = funcIntern[arith->myChars];
          if (funtype <= TAO_tan) {
            comFuncApply(funtype, infoKit);
            return;
          } else {
            infoKit->warning("the string specifies no basic math function");
            return;
          }
        } else {
          infoKit->warning("the string specifies no basic math function");
          return;
        }
      }
    }
  }

  vector<TaoComplex *> tmpCom;
  size_t k;
  for (k = 0; k < varTran.size(); k++) {
    tmpCom.push_back(new TaoComplex);
    tmpCom[k]->refCount = 1;
    varTran[k]->content = tmpCom[k];
  }

  bool isVector = 0;
  if (dims.size() == 2 && (dims[0] == 1 || dims[1] == 1))
    isVector = 1;

  for (size_t i = 0; i < numIndexElem; i++) {
    int idmap = 0;
    if (isVector) {
      if (dims[0] == 1)
        idmap = subIndex[1][i];
      else
        idmap = subIndex[0][i];

      tmpCom[1] = TaoTryAllocator<TaoComplex>::tryAllocate(tmpCom[1]);
      tmpCom[1]->cmplx = idmap;
      varTran[1]->content = tmpCom[1];
    } else {
      int mod = i;
      for (int j = (int)subIndex.size() - 1; j >= 0; j--) {
        int jd = mod % subIndex[j].size();
        mod /= subIndex[j].size();

        tmpCom[j + 1] = TaoTryAllocator<TaoComplex>::tryAllocate(tmpCom[j + 1]);
        tmpCom[j + 1]->cmplx = subIndex[j][jd];

        if (j + 1 < (int)varTran.size())
          varTran[j + 1]->content = tmpCom[j + 1];
        idmap += subIndex[j][jd] * dimAccum[j];
      }
    }
    tmpCom[0] = TaoTryAllocator<TaoComplex>::tryAllocate(tmpCom[0]);
    tmpCom[0]->cmplx = getComplex(idmap); //(*numArray)[idmap];
    varTran[0]->content = tmpCom[0];

    if (rout) {
      rout->run(0);

      if (apply) {
        TaoBase *result = rout->getResult();
        if (result)
          result = result->dereference();
        short rtti = -1;
        if (result)
          rtti = result->rtti();
        if (rtti == TAO_Complex) {
          setComplex(idmap, ((TaoComplex *)result)->cmplx);
        } else if (rtti == TAO_Number) {
          setComplex(idmap, ((TaoNumber *)result)->value);
        } else {
          infoKit->warning("non-numeric result to apply");
        }
      }

      //(*numArray)[idmap]=((TaoComplex*)result)->cmplx;
    } else {
      arith->compute();
      if (apply) {
        TaoBase *result = arith->getResult()->dereference();
        if (result->rtti() == TAO_Complex) {
          setComplex(idmap, arith->cmplx);
        } else if (result->rtti() == TAO_Number) {
          setComplex(idmap, arith->myValue);
        } else {
          infoKit->warning("non-numeric result to apply");
        }
      }
      //(*numArray)[idmap]=arith->myValue;
    }
  }
  for (k = 0; k < varTran.size(); k++) {
    if (tmpCom[k]->refCount == 1)
      delete tmpCom[k];
  }
}
void TaoArrayBase::simplifyDims() {
  if (dims.size() == 2)
    return;
  vector<size_t> newDims;
  size_t i;
  for (i = 0; i < dims.size(); i++)
    if (dims[i] > 1)
      newDims.push_back(dims[i]);

  dims.resize(newDims.size());
  for (i = 0; i < dims.size(); i++)
    dims[i] = newDims[i];
  updateDimAccum();
}
void TaoArrayBase::reshape(valarray<size_t> &newDims, TaoInfoKit *infoKit) {
  size_t nnew = 1;
  for (size_t i = 0; i < newDims.size(); i++)
    nnew *= newDims[i];
  if (nnew != size()) {
    infoKit->warning("improper shape for reshape()");
    return;
  }
  // size_t	oldSize=dims[0]*dimAccum[0];
  dims.resize(newDims.size());
  dims = newDims;
  updateDimAccum();
  resetSubIndex();
}
void TaoArrayBase::print_element(size_t i, ostream *out) {}
void TaoArrayBase::print(ostream *out) {
  if (dims.size() < 2)
    return;
  size_t i;
  if (dims.size() == 2 && (dims[0] == 1 || dims[1] == 1)) {
    // For vectors:
    *out << "[ ";
    for (i = 0; i < size(); i++) {
      print_element(i, out);
      if (i + 1 < size())
        *out << ", ";
    }
    if (dims[1] == 1)
      *out << " (T)]";
    else
      *out << " ]";
    return;
  }

  vector<int> tmp(dims.size());
  for (i = 0; i < size(); i++) {
    int mod = i;
    for (int j = (int)dims.size() - 1; j >= 0; j--) {
      int res = mod % dims[j];
      mod /= dims[j];
      tmp[j] = res;
    }
    // for array[2][3]:
    //      tmp=:
    // i=0: 0,0
    // i=1: 0,1
    // i=2: 0,2
    // i=3: 1,0
    // i=4: 1,1
    // i=5: 1,2
    if (tmp[dims.size() - 1] == 0) {
      *out << "row(";
      for (size_t j = 0; j + 1 < dims.size(); j++)
        *out << tmp[j] << ",";
      *out << ":):\t";
    }
    print_element(i, out);
    if (i + 1 < size())
      *out << "\t";

    if (tmp[dims.size() - 1] == (int)(dims[dims.size() - 1] - 1))
      *out << "\n";
  }
}

//=============================================================
//=============================================================
TaoByteArray::TaoByteArray(TaoByteArray &array)
    : TaoRealArray<Byte>::TaoRealArray((TaoRealArray<Byte> &)array) {
  // guo
  // TaoRealArray<Byte>::TaoRealArray( (TaoRealArray<Byte>&)array );
  cpptype = 0;
}
void TaoByteArray::print_element(size_t i, ostream *out) {
  *out << (short)numArray[i];
}
TaoArrayBase *TaoByteArray::tryAllocateSubArray(TaoArrayBase *array) {
  return TaoTryAllocator<TaoByteArray>::tryAllocate((TaoByteArray *)array);
}
TcBase *TaoByteArray::toCppType() {
  if (!cpptype)
    cpptype = new TcByteArray;
  cpptype->reserved = this;

  int ndim = dims.size();
  int *cdims = (int *)calloc(ndim, sizeof(int));
  for (int i = 0; i < ndim; i++)
    cdims[i] = dims[i];
  cpptype->setByteArray(numArray, cdims, ndim);

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoByteArray::fromCppType() {
  if (!cpptype)
    return;

  int ndim = cpptype->dim();
  int *cdims = cpptype->getDims();
  dims.resize(ndim);
  for (int i = 0; i < ndim; i++)
    dims[i] = cdims[i];
  numArray = cpptype->getAsVector();
  updateDimAccum();

  if (recycler)
    recycler->pushTcObject(cpptype);

  cpptype = 0;
}

//=============================================================
TaoShortArray::TaoShortArray(TaoShortArray &array)
    : TaoRealArray<short>::TaoRealArray((TaoRealArray<short> &)array) {
  // guo
  // TaoRealArray<short>::TaoRealArray( (TaoRealArray<short>&)array );
  cpptype = 0;
}
TaoArrayBase *TaoShortArray::tryAllocateSubArray(TaoArrayBase *array) {
  return TaoTryAllocator<TaoShortArray>::tryAllocate((TaoShortArray *)array);
}
TcBase *TaoShortArray::toCppType() {
  if (!cpptype)
    cpptype = new TcShortArray;
  cpptype->reserved = this;

  int ndim = dims.size();
  int *cdims = (int *)calloc(ndim, sizeof(int));
  for (int i = 0; i < ndim; i++)
    cdims[i] = dims[i];
  cpptype->setShortArray(numArray, cdims, ndim);

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoShortArray::fromCppType() {
  if (!cpptype)
    return;

  int ndim = cpptype->dim();
  int *cdims = cpptype->getDims();
  dims.resize(ndim);
  for (int i = 0; i < ndim; i++)
    dims[i] = cdims[i];
  numArray = cpptype->getAsVector();
  updateDimAccum();

  if (recycler)
    recycler->pushTcObject(cpptype);

  cpptype = 0;
}

//=============================================================
TaoIntArray::TaoIntArray(TaoIntArray &array)
    : TaoRealArray<int>::TaoRealArray((TaoRealArray<int> &)array) {
  // guo
  // TaoRealArray<int>::TaoRealArray( (TaoRealArray<int>&)array );
  cpptype = 0;
}
TaoArrayBase *TaoIntArray::tryAllocateSubArray(TaoArrayBase *array) {
  return TaoTryAllocator<TaoIntArray>::tryAllocate((TaoIntArray *)array);
}
TcBase *TaoIntArray::toCppType() {
  if (!cpptype)
    cpptype = new TcIntArray;
  cpptype->reserved = this;

  int ndim = dims.size();
  int *cdims = (int *)calloc(ndim, sizeof(int));
  for (int i = 0; i < ndim; i++)
    cdims[i] = dims[i];
  cpptype->setIntArray(numArray, cdims, ndim);

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoIntArray::fromCppType() {
  if (!cpptype)
    return;

  int ndim = cpptype->dim();
  int *cdims = cpptype->getDims();
  dims.resize(ndim);
  for (int i = 0; i < ndim; i++)
    dims[i] = cdims[i];
  numArray = cpptype->getAsVector();
  updateDimAccum();

  if (recycler)
    recycler->pushTcObject(cpptype);

  cpptype = 0;
}
//=============================================================
TaoFloatArray::TaoFloatArray(TaoFloatArray &array)
    : TaoRealArray<float>::TaoRealArray((TaoRealArray<float> &)array) {
  // guo
  // TaoRealArray<float>::TaoRealArray( (TaoRealArray<float>&)array );
  cpptype = 0;
}
TaoArrayBase *TaoFloatArray::tryAllocateSubArray(TaoArrayBase *array) {
  return TaoTryAllocator<TaoFloatArray>::tryAllocate((TaoFloatArray *)array);
}
TcBase *TaoFloatArray::toCppType() {
  if (!cpptype)
    cpptype = new TcFloatArray;
  cpptype->reserved = this;

  int ndim = dims.size();
  int *cdims = (int *)calloc(ndim, sizeof(int));
  for (int i = 0; i < ndim; i++)
    cdims[i] = dims[i];
  cpptype->setFloatArray(numArray, cdims, ndim);

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoFloatArray::fromCppType() {
  if (!cpptype)
    return;

  int ndim = cpptype->dim();
  int *cdims = cpptype->getDims();
  dims.resize(ndim);
  for (int i = 0; i < ndim; i++)
    dims[i] = cdims[i];
  numArray = cpptype->getAsVector();
  updateDimAccum();

  if (recycler)
    recycler->pushTcObject(cpptype);

  cpptype = 0;
}
//=============================================================
TaoDoubleArray::TaoDoubleArray(TaoDoubleArray &array)
    : TaoRealArray<double>::TaoRealArray((TaoRealArray<double> &)array) {
  // guo
  // TaoRealArray<double>::TaoRealArray( (TaoRealArray<double>&)array );
  cpptype = 0;
}
TaoArrayBase *TaoDoubleArray::tryAllocateSubArray(TaoArrayBase *array) {
  return TaoTryAllocator<TaoDoubleArray>::tryAllocate((TaoDoubleArray *)array);
}
TcBase *TaoDoubleArray::toCppType() {
  if (!cpptype)
    cpptype = new TcDoubleArray;
  cpptype->reserved = this;

  int ndim = dims.size();
  int *cdims = (int *)calloc(ndim, sizeof(int));
  for (int i = 0; i < ndim; i++)
    cdims[i] = dims[i];
  cpptype->setDoubleArray(numArray, cdims, ndim);

  if (recycler)
    recycler->pushTcObject(cpptype);

  return cpptype;
}
void TaoDoubleArray::fromCppType() {
  if (!cpptype)
    return;

  int ndim = cpptype->dim();
  int *cdims = cpptype->getDims();
  dims.resize(ndim);
  for (int i = 0; i < ndim; i++)
    dims[i] = cdims[i];
  numArray = cpptype->getAsVector();
  updateDimAccum();

  if (recycler)
    recycler->pushTcObject(cpptype);

  cpptype = 0;
}
//=============================================================
TaoArrayBase *allocRealArrayByType(short rootype) {
  TaoArrayBase *root = 0;
  if (rootype == TAO_ByteArray) {
    root = new TaoByteArray;
  } else if (rootype == TAO_ShortArray) {
    root = new TaoShortArray;
  } else if (rootype == TAO_IntArray) {
    root = new TaoIntArray;
  } else if (rootype == TAO_FloatArray) {
    root = new TaoFloatArray;
  } else if (rootype == TAO_DoubleArray) {
    root = new TaoDoubleArray;
  }
  return root;
}
//=============================================================
double mypow(double x, double y) {

  if (y != (int)y) {
    return pow(x, y);
  } else {
    register double z = 1;
    register int pw = (int)abs(y);
    register int i = 0;
    while ((i++) < pw)
      z *= x;
    if (y < 0)
      z = 1.0 / z;
    return z;
  }
}

int xBaseInteger(string &chs, int xbase, TaoInfoKit *infoKit) {
  int i = chs.size() - 1;
  int m = 1;
  int k = 0;
  while (i >= 0) {
    int d = isdigit(chs[i]) ? (chs[i] - '0') : (chs[i] - 'a') + 10;
    if (d >= xbase) {
      stringstream ss;
      ss << "invalid digit \"" << chs[i] << "\" for base " << xbase;
      infoKit->warning(ss.str());
      return 0;
    }
    k += d * m;
    m *= xbase;
    i--;
  }
  return k;
}
double xBaseDecimal(string &chs, int xbase, TaoInfoKit *infoKit) {
  double inv = 1.0 / xbase;
  double m = inv;

  size_t i = 0;
  double k = 0;
  while (i < chs.size()) {
    int d = isdigit(chs[i]) ? (chs[i] - '0') : (chs[i] - 'a') + 10;
    if (d >= xbase) {
      stringstream ss;
      ss << "invalid digit \"" << chs[i] << "\" for base " << xbase;
      infoKit->warning(ss.str());
      return 0;
    }
    k += d * m;
    m *= inv;
    i++;
  }
  return k;
}
double xBaseNumber(string &chs, int xbase, TaoInfoKit *infoKit) {
  double num = 0;
  str_pos dot = chs.find('.');
  if (dot != string::npos) {
    if (dot > 0) {
      string ds = chs.substr(0, dot);
      num += xBaseInteger(ds, xbase, infoKit);
    }
    if (dot + 1 < chs.size()) {
      string ds = chs.substr(dot + 1);
      num += xBaseDecimal(ds, xbase, infoKit);
    }
  } else {
    num += xBaseInteger(chs, xbase, infoKit);
  }
  return num;
}
