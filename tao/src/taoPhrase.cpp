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
#include "taoPhrase.h"
#include "taoArith.h"
#include "taoChain.h"
#include "taoDatatype.h"
#include "taoFuncall.h"
#include "taoNumeric.h"
#include "taoObject.h"
#include "taoRecycler.h"
#include "taoRegex.h"
#include "taoRoutine.h"
#include "taoUtility.h"
#include <iostream>

extern map<int, string> __Tao_TypeToName;

short TaoPhrase::RTTI = TAO_Phrase;
short TaoPhrase::rtti() const { return RTTI; }

short TaoControl::RTTI = TAO_Control;
short TaoControl::rtti() const { return RTTI; }
short LBrace::RTTI = TAO_LBrace;
short LBrace::rtti() const { return RTTI; }
short RBrace::RTTI = TAO_RBrace;
short RBrace::rtti() const { return RTTI; }

short TaoBreak::RTTI = TAO_Break;
short TaoBreak::rtti() const { return RTTI; }
short TaoSkip::RTTI = TAO_Skip;
short TaoSkip::rtti() const { return RTTI; }

short TaoIf::RTTI = TAO_If;
short TaoIf::rtti() const { return RTTI; }
short TaoElseIf::RTTI = TAO_ElseIf;
short TaoElseIf::rtti() const { return RTTI; }
short TaoElse::RTTI = TAO_Else;
short TaoElse::rtti() const { return RTTI; }
short TaoWhile::RTTI = TAO_While;
short TaoWhile::rtti() const { return RTTI; }
short TaoForEach::RTTI = TAO_ForEach;
short TaoForEach::rtti() const { return RTTI; }
short TaoFor::RTTI = TAO_For;
short TaoFor::rtti() const { return RTTI; }

short TaoCommand::RTTI = TAO_Command;
short TaoCommand::rtti() const { return RTTI; }
short TaoAssign::RTTI = TAO_Assign;
short TaoAssign::rtti() const { return RTTI; }

short TaoArithPhrase::RTTI = TAO_ArithPhrase;
short TaoArithPhrase::rtti() const { return RTTI; }

short TaoReturn::RTTI = TAO_Return;
short TaoReturn::rtti() const { return RTTI; }
short TaoYield::RTTI = TAO_Yield;

int TaoControl::execute() { return stepTo; }
int TaoForEach::execute() {
  chainArray->execute();
  TaoBase *parray = chainArray->getResult();
  if (parray)
    parray = parray->dereference();
  if (parray == 0)
    return jumpTo;

  short arrayType = parray->rtti();
  if (arrayType != TAO_Array && arrayType != TAO_RealArray &&
      arrayType != TAO_CompArray) {
    return jumpTo;
  }
  TaoArray *array = (TaoArray *)parray;

  if (index >= (int)array->elements.size()) {
    index = 0;
    refElement = 0;
    return jumpTo;
  } else {
    TaoBase *ptemp = 0;
    if (refElement->content)
      ptemp = refElement->content;

    refElement->content = array->elements[index]; //->content;
    index++;

    if (refElement->content)
      refElement->content->refCount++;

    if (TaoBase::recycler)
      TaoBase::recycler->pushRootCandidate(ptemp);

    return stepTo;
  }
}
int TaoFor::execute() {
  if (firstRound) {
    first->compute();
    firstRound = 0;
  } else {
    step->compute();
  }
  condition->compute();
  if (condition->bulean)
    return stepTo;
  firstRound = 1;
  return jumpTo;
}
int TaoWhile::execute() {
  condition->compute();
  if (condition->bulean)
    return stepTo;
  return jumpTo;
}

int TaoIf::execute() {
  condition->compute();
  if (condition->bulean)
    return stepTo;
  return jumpTo;
}
int TaoElseIf::execute() {
  condition->compute();
  if (condition->bulean)
    return stepTo;
  return jumpTo;
}
int TaoElse::execute() { return stepTo; }
int TaoArithPhrase::execute() {
  myArith->compute();
  return stepTo;
}
TaoAssign::TaoAssign() {
  refPrevious = 0;
  multiArray = 0;
  leftChain = 0;

  byteArray = 0;
  shortArray = 0;
  intArray = 0;
  floatArray = 0;
  doubleArray = 0;

  comArray = 0;

  keepConst = 1;
}
void TaoAssign::make_mdim_array(TaoArray *array, int i) {
  //	cout<<"TaoAssign::make_mdim_array\n";

  //	if(dimAriths.size()==0) return;
  if (i < (int)dimAriths.size() - 1) {
    array->elements.resize((int)dimAriths[i]->myValue);
    for (size_t j = 0; j < dimAriths[i]->myValue; j++) {
      TaoArray *vec = new TaoArray();
      array->elements[j] = vec;
      vec->refCount++;
      make_mdim_array(vec, i + 1);
    }
  } else {
    array->elements.resize((int)dimAriths[i]->myValue);
    for (size_t j = 0; j < array->elements.size(); j++) {
      array->elements[j] = make_element();
      array->elements[j]->refCount++;
    }
  }
}
void TaoAssign::assign() {
  TaoBase *pbase = 0;
  if (typeNumeric && dimAriths.size() > 0) {
    pbase = make_numeric();
  } else if (dimAriths.size() > 0) {
    multiArray = new TaoArray();
    make_mdim_array(multiArray, 0);
    pbase = multiArray;
  } else {
    pbase = make_element();
    if (pbase && pbase->rtti() == TAO_Reference) {
      TaoReference *ref = (TaoReference *)pbase;
      pbase = ref->content;
    }
  }

  if (lchainArray.size() > 0)
    multiAssign(pbase);
  else {
    assign(leftChain->result, pbase, infoKit, keepConst);
  }
  multiArray = 0;
}
void TaoAssign::assign(TaoBase *lbase, TaoBase *rbase, TaoInfoKit *infoKit,
                       bool keepcnst) {
  TaoBase *ptemp = 0;
  short rtti = lbase->rtti();
  if (rtti == TAO_Reference) {
    TaoReference *lref = (TaoReference *)lbase;
    short type = lref->rttiRefer();
    /*
    while(type==TAO_ParamRefer || type==TAO_DataRefer){
            if(!lref->content) break;
            lref=(TaoReference*)lref->content;
            type=lref->rttiRefer();
    }
    */
    if (type == TAO_ConstRefer && keepcnst) {
      infoKit->warning("Can't assign to a constant");
      return;
    }

    ptemp = 0;
    if (lref->content)
      ptemp = lref->content;

    lref->content = rbase;
    if (lref->content)
      lref->content->refCount++;
    if (TaoBase::recycler)
      TaoBase::recycler->pushRootCandidate(ptemp);
  } else if (rtti == TAO_String) {
    TaoString *str = (TaoString *)lbase;
    str->assignWith(rbase);
  } else if (rtti == TAO_Array) {
    TaoArray *array = (TaoArray *)lbase;
    array->assignWith(rbase);
  } else if (rtti == TAO_Hash) {
    TaoHash *hash = (TaoHash *)lbase;
    hash->assignWith(rbase);
  } else if (rtti == TAO_RealArray) {
    if (rbase->rtti() != TAO_Number) {
      infoKit->error_general("invalid assignment with numeric variable");
      return;
    }
    TaoNumber *num = (TaoNumber *)rbase;
    TaoArrayBase *numArray = (TaoArrayBase *)lbase;
    numArray->assignWith(num->value, TAO_ASSIGN);
  } else if (rtti == TAO_CompArray) {
    complex<double> com;
    if (rbase->rtti() == TAO_Complex) {
      TaoComplex *cpx = (TaoComplex *)rbase;
      com = cpx->cmplx;
    } else if (rbase->rtti() == TAO_Number) {
      TaoNumber *num = (TaoNumber *)rbase;
      com = num->value;
    } else {
      infoKit->error_general("invalid assignment with numeric variable");
      return;
    }
    TaoCompArray<double> *comArray = (TaoCompArray<double> *)lbase;
    comArray->assignWith(com, TAO_ASSIGN);
  }
}
void TaoAssign::multiAssign(TaoBase *pbase) {
  if (pbase->rtti() == TAO_Array) {
    TaoArray *array = (TaoArray *)pbase;
    size_t nchain = lchainArray.size();
    size_t nelem = array->elements.size();
    for (size_t i = 0; i < nchain; i++) {
      if (i >= nelem)
        break;
      // For phrase such as: [a,b]=[b,a];
      TaoBase *p = 0;
      if (array->elements[i])
        p = array->elements[i]->dereference();
      assign(lchainArray[i]->getResult(), p, infoKit, keepConst);
    }
  } else {
    for (size_t i = 0; i < lchainArray.size(); i++)
      assign(lchainArray[i]->getResult(), pbase, infoKit, keepConst);
  }
}
TaoBase *TaoAssign::make_element() {
  rightArith->compute();

  return rightArith->getResult();
}
TaoBase *TaoAssign::make_numeric() {
  rightArith->compute();
  TaoBase *tbase = rightArith->getResult()->dereference();
  short rt = tbase->rtti();

  if (rt == TAO_Number || rt == TAO_Complex) {
    valarray<size_t> dm(dimAriths.size());
    for (size_t i = 0; i < dimAriths.size(); i++)
      dm[i] = (size_t)dimAriths[i]->myValue;

    if (rt == TAO_Number) {
      if (typeNumeric == TAO_ByteArray) {
        byteArray = TaoTryAllocator<TaoByteArray>::tryAllocate(byteArray);
        byteArray->timeTag = TaoBase::recycler->gcTimes;

        byteArray->resize(dm);
        // byteArray->assignWith( num->value );
        Byte *vec = byteArray->getAsVector();
        vec[0] = (Byte)rightArith->myValue;
        size_t size = byteArray->size();
        for (size_t j = 1; j < size; j++) {
          rightArith->compute();
          vec[j] = (Byte)rightArith->myValue;
        }

        return byteArray;
      } else if (typeNumeric == TAO_ShortArray) {
        shortArray = TaoTryAllocator<TaoShortArray>::tryAllocate(shortArray);
        shortArray->timeTag = TaoBase::recycler->gcTimes;

        shortArray->resize(dm);
        // shortArray->assignWith( num->value );
        short *vec = shortArray->getAsVector();
        vec[0] = (short)rightArith->myValue;
        size_t size = shortArray->size();
        for (size_t j = 1; j < size; j++) {
          rightArith->compute();
          vec[j] = (short)rightArith->myValue;
        }

        return shortArray;
      } else if (typeNumeric == TAO_IntArray) {
        intArray = TaoTryAllocator<TaoIntArray>::tryAllocate(intArray);
        intArray->timeTag = TaoBase::recycler->gcTimes;

        intArray->resize(dm);
        // intArray->assignWith( num->value );
        int *vec = intArray->getAsVector();
        vec[0] = (int)rightArith->myValue;
        size_t size = intArray->size();
        for (size_t j = 1; j < size; j++) {
          rightArith->compute();
          vec[j] = (int)rightArith->myValue;
        }

        return intArray;
      } else if (typeNumeric == TAO_FloatArray) {
        floatArray = TaoTryAllocator<TaoFloatArray>::tryAllocate(floatArray);
        floatArray->timeTag = TaoBase::recycler->gcTimes;

        floatArray->resize(dm);
        // floatArray->assignWith( num->value );
        float *vec = floatArray->getAsVector();
        vec[0] = (float)rightArith->myValue;
        size_t size = floatArray->size();
        for (size_t j = 1; j < size; j++) {
          rightArith->compute();
          vec[j] = (float)rightArith->myValue;
        }

        return floatArray;
      } else {
        doubleArray = TaoTryAllocator<TaoDoubleArray>::tryAllocate(doubleArray);
        doubleArray->timeTag = TaoBase::recycler->gcTimes;

        doubleArray->resize(dm);
        // doubleArray->assignWith( num->value );
        double *vec = doubleArray->getAsVector();
        vec[0] = rightArith->myValue;
        size_t size = doubleArray->size();
        for (size_t j = 1; j < size; j++) {
          rightArith->compute();
          vec[j] = rightArith->myValue;
        }

        return doubleArray;
      }
    } else if (rt == TAO_Complex) {
      comArray = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(comArray);
      comArray->timeTag = TaoBase::recycler->gcTimes;

      comArray->resize(dm);
      complex<double> *vec = comArray->getAsVector();
      vec[0] = rightArith->cmplx;
      size_t size = comArray->size();
      for (size_t j = 1; j < size; j++) {
        rightArith->compute();
        vec[j] = rightArith->cmplx;
      }

      return comArray;
    }
  } else if (rt == TAO_RealArray || rt == TAO_CompArray) {
    TaoArrayBase *arrayBase = (TaoArrayBase *)tbase;
    valarray<size_t> dimElem = arrayBase->getDims();

    valarray<size_t> dm(dimAriths.size() + dimElem.size());
    size_t i;
    size_t mulOne = 1;
    size_t mulTwo = arrayBase->size();

    for (i = 0; i < dimAriths.size(); i++) {
      dm[i] = (size_t)dimAriths[i]->myValue;
      mulOne *= dm[i];
    }
    for (i = 0; i < dimElem.size(); i++)
      dm[i + dimAriths.size()] = dimElem[i];

    if (rt == TAO_RealArray) {
      if (typeNumeric == TAO_ByteArray) {
        byteArray = TaoTryAllocator<TaoByteArray>::tryAllocate(byteArray);
        byteArray->timeTag = TaoBase::recycler->gcTimes;

        byteArray->resize(dm);
        Byte *vec = byteArray->getAsVector();

        valarray<Byte> elem;
        exportValues(arrayBase, elem);
        size_t j;
        for (j = 0; j < mulTwo; j++)
          vec[j] = elem[j];
        for (i = 1; i < mulOne; i++) {
          rightArith->compute();
          tbase = rightArith->getResult()->dereference();
          rt = tbase->rtti();
          if (rt == TAO_RealArray)
            exportValues((TaoArrayBase *)tbase, elem);
          for (j = 0; j < mulTwo; j++)
            vec[i * mulTwo + j] = elem[j];
        }

        byteArray->simplifyDims();
        return byteArray;
      } else if (typeNumeric == TAO_ShortArray) {
        shortArray = TaoTryAllocator<TaoShortArray>::tryAllocate(shortArray);
        shortArray->timeTag = TaoBase::recycler->gcTimes;

        shortArray->resize(dm);
        short *vec = shortArray->getAsVector();

        valarray<short> elem;
        exportValues(arrayBase, elem);
        size_t j;
        for (j = 0; j < mulTwo; j++)
          vec[j] = elem[j];
        for (i = 1; i < mulOne; i++) {
          rightArith->compute();
          tbase = rightArith->getResult()->dereference();
          rt = tbase->rtti();
          if (rt == TAO_RealArray)
            exportValues((TaoArrayBase *)tbase, elem);
          for (j = 0; j < mulTwo; j++)
            vec[i * mulTwo + j] = elem[j];
        }

        shortArray->simplifyDims();
        return shortArray;
      } else if (typeNumeric == TAO_IntArray) {
        intArray = TaoTryAllocator<TaoIntArray>::tryAllocate(intArray);
        intArray->timeTag = TaoBase::recycler->gcTimes;

        intArray->resize(dm);
        int *vec = intArray->getAsVector();

        valarray<int> elem;
        exportValues(arrayBase, elem);
        size_t j;
        for (j = 0; j < mulTwo; j++)
          vec[j] = elem[j];
        for (i = 1; i < mulOne; i++) {
          rightArith->compute();
          tbase = rightArith->getResult()->dereference();
          rt = tbase->rtti();
          if (rt == TAO_RealArray)
            exportValues((TaoArrayBase *)tbase, elem);
          for (j = 0; j < mulTwo; j++)
            vec[i * mulTwo + j] = elem[j];
        }

        intArray->simplifyDims();
        return intArray;
      } else if (typeNumeric == TAO_FloatArray) {
        floatArray = TaoTryAllocator<TaoFloatArray>::tryAllocate(floatArray);
        floatArray->timeTag = TaoBase::recycler->gcTimes;

        floatArray->resize(dm);
        float *vec = floatArray->getAsVector();

        valarray<float> elem;
        exportValues(arrayBase, elem);
        size_t j;
        for (j = 0; j < mulTwo; j++)
          vec[j] = elem[j];
        for (i = 1; i < mulOne; i++) {
          rightArith->compute();
          tbase = rightArith->getResult()->dereference();
          rt = tbase->rtti();
          if (rt == TAO_RealArray)
            exportValues((TaoArrayBase *)tbase, elem);
          for (j = 0; j < mulTwo; j++)
            vec[i * mulTwo + j] = elem[j];
        }

        floatArray->simplifyDims();
        return floatArray;
      } else {
        doubleArray = TaoTryAllocator<TaoDoubleArray>::tryAllocate(doubleArray);
        doubleArray->timeTag = TaoBase::recycler->gcTimes;

        doubleArray->resize(dm);
        double *vec = doubleArray->getAsVector();

        valarray<double> elem;
        exportValues(arrayBase, elem);
        size_t j;
        for (j = 0; j < mulTwo; j++)
          vec[j] = elem[j];
        for (i = 1; i < mulOne; i++) {
          rightArith->compute();
          tbase = rightArith->getResult()->dereference();
          rt = tbase->rtti();
          if (rt == TAO_RealArray)
            exportValues((TaoArrayBase *)tbase, elem);
          for (j = 0; j < mulTwo; j++)
            vec[i * mulTwo + j] = elem[j];
        }

        doubleArray->simplifyDims();
        return doubleArray;
      }
    } else if (rt == TAO_CompArray) {
      comArray = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(comArray);
      comArray->timeTag = TaoBase::recycler->gcTimes;

      TaoCompArray<double> *com = (TaoCompArray<double> *)tbase;
      comArray->resize(dm);
      complex<double> *vec = comArray->getAsVector();
      complex<double> *elem = com->getAsVector();
      size_t j;
      for (j = 0; j < mulTwo; j++)
        vec[j] = elem[j];
      for (i = 1; i < mulOne; i++) {
        rightArith->compute();
        tbase = rightArith->getResult()->dereference();
        rt = tbase->rtti();
        if (rt == TAO_CompArray) {
          com = (TaoCompArray<double> *)tbase;
          elem = com->getAsVector();
        }
        if (com->size() < mulTwo) {
          for (j = 0; j < com->size(); j++)
            vec[i * mulTwo + j] = elem[j];
        } else {
          for (j = 0; j < mulTwo; j++)
            vec[i * mulTwo + j] = elem[j];
        }
      }

      comArray->simplifyDims();
      return comArray;
    }
  }
  return 0;
}
void TaoAssign::compChainAriths() {
  if (leftChain)
    leftChain->execute();

  size_t i;

  for (i = 0; i < dimAriths.size(); i++)
    dimAriths[i]->compute();

  for (i = 0; i < lchainArray.size(); i++)
    lchainArray[i]->execute();
}
int TaoAssign::execute() {
  compChainAriths();

  assign();

  return stepTo;
}

int TaoReturn::execute() {
  for (size_t i = 0; i < returnAriths.size(); i++)
    returnAriths[i]->compute();
  return jumpTo;
}
