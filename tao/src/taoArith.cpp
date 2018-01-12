
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
#include <cmath>
#include <iostream>
#include <sstream>

#include <cstring>
#include <string.h>

#include "stdlib.h"

#include "taoArith.h"
#include "taoClass.h"
#include "taoEnumer.h"
#include "taoLoadlib.h"
#include "taoPlugin.h"
#include "taoRoutine.h"
#include "taoUtility.h"

extern map<int, string> __Tao_TypeToName;

short TaoArithBase::RTTI = TAO_ArithBase;
short TaoArithBase::rtti() const { return RTTI; }
short TaoArithVariable::RTTI = TAO_ArithVariable;
short TaoArithVariable::rtti() const { return RTTI; }
short TaoArithChain::RTTI = TAO_ArithChain;
short TaoArithChain::rtti() const { return RTTI; }
short TaoArithConst::RTTI = TAO_ArithConst;
short TaoArithConst::rtti() const { return RTTI; }
short TaoArithNode::RTTI = TAO_ArithNode;
short TaoArithNode::rtti() const { return RTTI; }

short TaoNullArith::RTTI = TAO_NullArith;
short TaoNullArith::rtti() const { return RTTI; }
short TaoPairArith::RTTI = TAO_PairArith;
short TaoPairArith::rtti() const { return RTTI; }
short TaoEnumArith::RTTI = TAO_EnumArith;
short TaoEnumArith::rtti() const { return RTTI; }
short TaoSwitchArith::RTTI = TAO_SwitchArith;
short TaoSwitchArith::rtti() const { return RTTI; }

short TaoArithArray::RTTI = TAO_ArithArray;
short TaoArithArray::rtti() const { return RTTI; }

////////////////////////////
//
//	TaoArithBase
//
////////////////////////////
TaoArithBase::TaoArithBase() {
  myChars = "";
  myValue = 0;
  cmplx = 0;

  arithType = -1;
  pro = left = right = 0;
  comArray = 0;
  byteArray = 0;
  shortArray = 0;
  intArray = 0;
  floatArray = 0;
  doubleArray = 0;

  number = 0;
  tstring = 0;
  complx = 0;
  interResult = 0;

  nonTrans = 1;
}
TaoArithBase::~TaoArithBase() {
  if (left)
    delete left;
  if (right)
    delete right;
}
void TaoArithBase::checkArithType(TaoBase *pbase) {
  if (!pbase) {
    myValue = 0;
    myChars.erase();
    arithType = 1;
    return;
  }
  short rt = pbase->rtti();
  if (rt == TAO_Number) {
    // TaoNumber *num = (TaoNumber*)pbase;
    myValue = ((TaoNumber *)pbase)->value;
    arithType = 1;
  } else if (rt == TAO_String) {
    TaoString *str = (TaoString *)pbase;
    myChars = str->chars;
    myValue = atof(myChars.c_str());
    arithType = 0;
  } else if (rt == TAO_Complex) {
    TaoComplex *com = (TaoComplex *)pbase;
    cmplx = com->cmplx;
    arithType = 2;
  } else if (rt == TAO_CompArray) {
    comArray = (TaoCompArray<double> *)pbase;
    if (rtti() == TAO_ArithVariable)
      comArray->resetSubIndex();
    arithType = 3;
  } else if (rt == TAO_RealArray) {
    short rt2 = ((TaoArrayBase *)pbase)->rttiNumeric();
    if (rtti() == TAO_ArithVariable)
      ((TaoArrayBase *)pbase)->resetSubIndex();

    if (rt2 == TAO_ByteArray) {
      byteArray = (TaoByteArray *)pbase;
      arithType = 4;
    } else if (rt2 == TAO_ShortArray) {
      shortArray = (TaoShortArray *)pbase;
      arithType = 5;
    } else if (rt2 == TAO_IntArray) {
      intArray = (TaoIntArray *)pbase;
      arithType = 6;
    } else if (rt2 == TAO_FloatArray) {
      floatArray = (TaoFloatArray *)pbase;
      arithType = 7;
    } else {
      doubleArray = (TaoDoubleArray *)pbase;
      arithType = 8;
    }
  } else {
    arithType = 1;
  }
}

TaoString *TaoArithBase::getString() {
  tstring = TaoTryAllocator<TaoString>::tryAllocate(tstring);
  tstring->chars = myChars;
  return tstring;
}
TaoNumber *TaoArithBase::getNumber() {
  number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
  number->value = myValue;
  return number;
}
TaoComplex *TaoArithBase::getComplex() {
  complx = TaoTryAllocator<TaoComplex>::tryAllocate(complx);
  complx->cmplx = cmplx;
  return complx;
}
void TaoArithBase::printString(ostream *out) {
  tstring = getString();
  tstring->print(out);
}
void TaoArithBase::printNumber(ostream *out) {
  number = getNumber();
  number->print(out);
}
void TaoArithBase::printComplex(ostream *out) {
  complx = getComplex();
  complx->print(out);
}
////////////////////////////
//
// TaoArithVariable
// TaoArithChain
//
////////////////////////////

void TaoArithVariable::compute() {
  interResult = refVariable->content;
  if (interResult && (interResult->rtti() == TAO_Reference))
    interResult = ((TaoReference *)interResult)->content;

  checkArithType(interResult);
  bulean = myValue;
}
void TaoArithChain::compute() {
  interResult = 0;
  myChain->execute();
  if (myChain->result)
    interResult = myChain->result->dereference();

  checkArithType(interResult);
  bulean = myChain->blResult;
}

void TaoEnumArith::compute() {
  myEnum->execute();
  interResult = myEnum->result;
  checkArithType(interResult);
}

TaoBase *TaoArithVariable::getResult() { return refVariable->dereference(); }
void TaoArithVariable::print(ostream *out) { refVariable->print(out); }
TaoBase *TaoArithChain::getResult() {
  TaoBase *p = myChain->getResult();
  if (p)
    p = p->dereference();
  return p;
}
void TaoArithChain::print(ostream *out) {
  if (myChain->result)
    myChain->result->print(out);
}
void TaoEnumArith::print(ostream *out) { myEnum->result->print(out); }
TaoBase *TaoEnumArith::getResult() { return myEnum->result; }
void TaoSwitchArith::compute() {
  left->compute();
  if (right->rtti() == TAO_PairArith) {
    TaoPairArith *pair = (TaoPairArith *)right;
    if (left->bulean) {
      pair->left->compute();
      interResult = pair->left->getResult();
    } else {
      pair->right->compute();
      interResult = pair->right->getResult();
    }
  } else {
    if (left->bulean) {
      right->compute();
      interResult = right->getResult();
    }
  }
  checkArithType(interResult);
  bulean = left->bulean;
}

////////////////////////////
//
//	TaoArithNode
//
////////////////////////////
TaoArithNode::TaoArithNode() { oper = '0'; }
TaoBase *TaoArithNode::getResult() {
  if (arithType == 0) {
    return getString();
  } else if (arithType == 1) {
    return getNumber();
  } else if (arithType == 2) {
    return getComplex();
  } else if (arithType == 3) {
    return comArray;
  } else if (arithType == 4) {
    return byteArray;
  } else if (arithType == 5) {
    return shortArray;
  } else if (arithType == 6) {
    return intArray;
  } else if (arithType == 7) {
    return floatArray;
  } else if (arithType == 8) {
    return doubleArray;
  }
  return 0;
}
void TaoArithNode::print(ostream *out) {
  if (arithType == 0) {
    printString(out);
  } else if (arithType == 1) {
    printNumber(out);
  } else if (arithType == 2) {
    printComplex(out);
  } else if (arithType == 3) {
    comArray->print(out);
  } else if (arithType == 4) {
    byteArray->print(out);
  } else if (arithType == 5) {
    shortArray->print(out);
  } else if (arithType == 6) {
    intArray->print(out);
  } else if (arithType == 7) {
    floatArray->print(out);
  } else if (arithType == 8) {
    doubleArray->print(out);
  }
}
void TaoArithNode::numberArith() {
  if (oper == TAO_PLUS)
    myValue = left->myValue + right->myValue;
  else if (oper == TAO_MINUS)
    myValue = left->myValue - right->myValue;
  else if (oper == TAO_TIMES)
    myValue = left->myValue * right->myValue;
  else if (oper == TAO_DIVIDE)
    myValue = left->myValue / right->myValue;
  else if (oper == TAO_MOD)
    myValue = (int)left->myValue % (int)right->myValue;
  else if (oper == TAO_POW)
    myValue = mypow(left->myValue, right->myValue);
  else if (oper == TAO_AND)
    myValue = left->myValue && right->myValue;
  else if (oper == TAO_OR)
    myValue = left->myValue || right->myValue;
  else if (oper == TAO_EQUAL)
    myValue = left->myValue == right->myValue;
  else if (oper == TAO_SMALLER)
    myValue = left->myValue < right->myValue;
  else if (oper == TAO_LARGER)
    myValue = left->myValue > right->myValue;
  else if (oper == TAO_NOT_EQUAL)
    myValue = left->myValue != right->myValue;
  else if (oper == TAO_NOT_LARGER)
    myValue = left->myValue <= right->myValue;
  else if (oper == TAO_NOT_SMALLER)
    myValue = left->myValue >= right->myValue;
  else {
    short rtti = -1;
    if (left->interResult)
      rtti = left->interResult->rtti();
    if (rtti == TAO_Number) {
      TaoNumber *num = (TaoNumber *)left->interResult;
      if (oper == TAO_ASSIGN_PLUS) {
        num->value += right->myValue;
      } else if (oper == TAO_ASSIGN_MINUS) {
        num->value -= right->myValue;
      } else if (oper == TAO_ASSIGN_TIMES) {
        num->value *= right->myValue;
      } else if (oper == TAO_ASSIGN_DIVIDE) {
        num->value /= right->myValue;
      } else if (oper == TAO_ASSIGN_MOD) {
        num->value = (int)num->value % (int)right->myValue;
      } else if (oper == TAO_ASSIGN_POW) {
        num->value = mypow(num->value, right->myValue);
      } else if (oper == TAO_ASSIGN_AND) {
        num->value = num->value && right->myValue;
      } else if (oper == TAO_ASSIGN_OR) {
        num->value = num->value || right->myValue;
      }
    }
  }
}
void TaoArithNode::stringArith() {
  if (oper == TAO_PLUS) {
    myChars = left->myChars + right->myChars;
    arithType = 0;
  } else if (oper == TAO_EQUAL)
    myValue = left->myChars == right->myChars;
  else if (oper == TAO_SMALLER)
    myValue = left->myChars < right->myChars;
  else if (oper == TAO_LARGER)
    myValue = left->myChars > right->myChars;
  else if (oper == TAO_NOT_EQUAL)
    myValue = left->myChars != right->myChars;
  else if (oper == TAO_NOT_LARGER)
    myValue = left->myChars <= right->myChars;
  else if (oper == TAO_NOT_SMALLER)
    myValue = left->myChars >= right->myChars;
  else {
    short rtti = -1;
    if (left->interResult)
      rtti = left->interResult->rtti();
    if (rtti == TAO_String) {
      arithType = 0;
      TaoString *str = (TaoString *)left->interResult;
      if (oper == TAO_ASSIGN_PLUS)
        str->chars += right->myChars;
    }
  }
}
void TaoArithNode::numComArith() {
  if (oper == TAO_PLUS)
    cmplx = left->myValue + right->cmplx;
  else if (oper == TAO_MINUS)
    cmplx = left->myValue - right->cmplx;
  else if (oper == TAO_TIMES)
    cmplx = left->myValue * right->cmplx;
  else if (oper == TAO_DIVIDE)
    cmplx = left->myValue / right->cmplx;
  else if (oper == TAO_EQUAL) {
    myValue = left->myValue == right->cmplx;
    arithType = 1;
  } else if (oper == TAO_NOT_EQUAL) {
    myValue = left->myValue != right->cmplx;
    arithType = 1;
  }
}
void TaoArithNode::comNumArith() {
  if (oper == TAO_PLUS)
    cmplx = left->cmplx + right->myValue;
  else if (oper == TAO_MINUS)
    cmplx = left->cmplx - right->myValue;
  else if (oper == TAO_TIMES)
    cmplx = left->cmplx * right->myValue;
  else if (oper == TAO_DIVIDE)
    cmplx = left->cmplx / right->myValue;
  else if (oper == TAO_EQUAL) {
    myValue = left->cmplx == right->myValue;
    arithType = 1;
  } else if (oper == TAO_NOT_EQUAL) {
    myValue = left->cmplx != right->myValue;
    arithType = 1;
  } else {
    short rtti = -1;
    if (left->interResult)
      rtti = left->interResult->rtti();
    if (rtti == TAO_Complex) {
      TaoComplex *com = (TaoComplex *)left->interResult;
      if (oper == TAO_ASSIGN_PLUS) {
        com->cmplx += right->myValue;
      } else if (oper == TAO_ASSIGN_MINUS) {
        com->cmplx -= right->myValue;
      } else if (oper == TAO_ASSIGN_TIMES) {
        com->cmplx *= right->myValue;
      } else if (oper == TAO_ASSIGN_DIVIDE) {
        com->cmplx /= right->myValue;
      }
    }
  }
}
void TaoArithNode::comComArith() {
  if (oper == TAO_PLUS)
    cmplx = left->cmplx + right->cmplx;
  else if (oper == TAO_MINUS)
    cmplx = left->cmplx - right->cmplx;
  else if (oper == TAO_TIMES)
    cmplx = left->cmplx * right->cmplx;
  else if (oper == TAO_DIVIDE)
    cmplx = left->cmplx / right->cmplx;
  else if (oper == TAO_EQUAL) {
    myValue = left->cmplx == right->cmplx;
    arithType = 1;
  } else if (oper == TAO_NOT_EQUAL) {
    myValue = left->cmplx != right->cmplx;
    arithType = 1;
  } else {
    short rtti = -1;
    if (left->interResult)
      rtti = left->interResult->rtti();
    if (rtti == TAO_Complex) {
      TaoComplex *com = (TaoComplex *)left->interResult;
      if (oper == TAO_ASSIGN_PLUS) {
        com->cmplx += right->cmplx;
      } else if (oper == TAO_ASSIGN_MINUS) {
        com->cmplx -= right->cmplx;
      } else if (oper == TAO_ASSIGN_TIMES) {
        com->cmplx *= right->cmplx;
      } else if (oper == TAO_ASSIGN_DIVIDE) {
        com->cmplx /= right->cmplx;
      }
    }
  }
}
void TaoArithNode::numArrayArith() {
  //	cout<<(int)left->arithType<<" "<<(int)oper<<"
  //"<<(int)right->arithType<<"\n";

  //================================================
  //	One byte array:
  //================================================
  if (left->arithType == 4 && right->arithType == 4) {
    arithType = 4;
    byteArray = (TaoByteArray *)realarray_arrayR_arrayL(
        byteArray, (short)TAO_ByteArray, left->byteArray, right->byteArray,
        oper, infoKit);
  } else if (left->arithType == 4 && right->arithType == 5) {
    arithType = 5;
    shortArray = (TaoShortArray *)realarray_arrayR_arrayL(
        shortArray, (short)TAO_ShortArray, left->byteArray, right->shortArray,
        oper, infoKit);
  } else if (left->arithType == 5 && right->arithType == 4) {
    arithType = 5;
    shortArray = (TaoShortArray *)realarray_arrayR_arrayL(
        shortArray, (short)TAO_ShortArray, left->shortArray, right->byteArray,
        oper, infoKit);
  } else if (left->arithType == 4 && right->arithType == 6) {
    arithType = 6;
    intArray = (TaoIntArray *)realarray_arrayR_arrayL(
        intArray, (short)TAO_IntArray, left->byteArray, right->intArray, oper,
        infoKit);
  } else if (left->arithType == 6 && right->arithType == 4) {
    arithType = 6;
    intArray = (TaoIntArray *)realarray_arrayR_arrayL(
        intArray, (short)TAO_IntArray, left->intArray, right->byteArray, oper,
        infoKit);
  } else if (left->arithType == 4 && right->arithType == 7) {
    arithType = 7;
    floatArray = (TaoFloatArray *)realarray_arrayR_arrayL(
        floatArray, (short)TAO_FloatArray, left->byteArray, right->floatArray,
        oper, infoKit);
  } else if (left->arithType == 7 && right->arithType == 4) {
    arithType = 7;
    floatArray = (TaoFloatArray *)realarray_arrayR_arrayL(
        floatArray, (short)TAO_FloatArray, left->floatArray, right->byteArray,
        oper, infoKit);
  } else if (left->arithType == 4 && right->arithType == 8) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->byteArray,
        right->doubleArray, oper, infoKit);
  } else if (left->arithType == 8 && right->arithType == 4) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->doubleArray,
        right->byteArray, oper, infoKit);
    //================================================
    //	One short array:
    //================================================
  } else if (left->arithType == 5 && right->arithType == 5) {
    arithType = 5;
    shortArray = (TaoShortArray *)realarray_arrayR_arrayL(
        shortArray, (short)TAO_ShortArray, left->shortArray, right->shortArray,
        oper, infoKit);
  } else if (left->arithType == 5 && right->arithType == 6) {
    arithType = 6;
    intArray = (TaoIntArray *)realarray_arrayR_arrayL(
        intArray, (short)TAO_IntArray, left->shortArray, right->intArray, oper,
        infoKit);
  } else if (left->arithType == 6 && right->arithType == 5) {
    arithType = 6;
    intArray = (TaoIntArray *)realarray_arrayR_arrayL(
        intArray, (short)TAO_IntArray, left->intArray, right->shortArray, oper,
        infoKit);
  } else if (left->arithType == 5 && right->arithType == 7) {
    arithType = 7;
    floatArray = (TaoFloatArray *)realarray_arrayR_arrayL(
        floatArray, (short)TAO_FloatArray, left->shortArray, right->floatArray,
        oper, infoKit);
  } else if (left->arithType == 7 && right->arithType == 5) {
    arithType = 7;
    floatArray = (TaoFloatArray *)realarray_arrayR_arrayL(
        floatArray, (short)TAO_FloatArray, left->floatArray, right->shortArray,
        oper, infoKit);
  } else if (left->arithType == 5 && right->arithType == 8) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->shortArray,
        right->doubleArray, oper, infoKit);
  } else if (left->arithType == 8 && right->arithType == 5) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->doubleArray,
        right->shortArray, oper, infoKit);
    //================================================
    //	One int array:
    //================================================
  } else if (left->arithType == 6 && right->arithType == 6) {
    arithType = 6;
    intArray = (TaoIntArray *)realarray_arrayR_arrayL(
        intArray, (short)TAO_IntArray, left->intArray, right->intArray, oper,
        infoKit);
  } else if (left->arithType == 6 && right->arithType == 7) {
    arithType = 7;
    floatArray = (TaoFloatArray *)realarray_arrayR_arrayL(
        floatArray, (short)TAO_FloatArray, left->intArray, right->floatArray,
        oper, infoKit);
  } else if (left->arithType == 7 && right->arithType == 6) {
    arithType = 7;
    floatArray = (TaoFloatArray *)realarray_arrayR_arrayL(
        floatArray, (short)TAO_FloatArray, left->floatArray, right->intArray,
        oper, infoKit);
  } else if (left->arithType == 6 && right->arithType == 8) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->intArray, right->doubleArray,
        oper, infoKit);
  } else if (left->arithType == 8 && right->arithType == 6) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->doubleArray, right->intArray,
        oper, infoKit);
    //================================================
    //	One float array:
    //================================================
  } else if (left->arithType == 7 && right->arithType == 7) {
    arithType = 7;
    floatArray = (TaoFloatArray *)realarray_arrayR_arrayL(
        floatArray, (short)TAO_FloatArray, left->floatArray, right->floatArray,
        oper, infoKit);
  } else if (left->arithType == 7 && right->arithType == 8) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->floatArray,
        right->doubleArray, oper, infoKit);
  } else if (left->arithType == 8 && right->arithType == 7) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->doubleArray,
        right->floatArray, oper, infoKit);
    //================================================
    //	Double array:
    //================================================
  } else if (left->arithType == 8 && right->arithType == 8) {
    arithType = 8;
    doubleArray = (TaoDoubleArray *)realarray_arrayR_arrayL(
        doubleArray, (short)TAO_DoubleArray, left->doubleArray,
        right->doubleArray, oper, infoKit);
  }
}
void TaoArithNode::comArrayArith() {}
void TaoArithNode::unaryArithLeft() {
  if (oper == TAO_NOT) {
    arithType = 1;
    myValue = !right->myValue;
  } else if (oper == TAO_MINUS) {
    arithType = 1;
    myValue = -right->myValue;
  } else if (oper == TAO_PPLUS || oper == TAO_MMINUS) {
    arithType = 1;
    if (right->interResult) {
      short rtti = right->interResult->rtti();
      if (rtti == TAO_Number) {
        TaoNumber *num = (TaoNumber *)right->interResult;
        if (oper == TAO_PPLUS)
          num->value++;
        else if (oper == TAO_MMINUS)
          num->value--;
        myValue = num->value;
      } else if (rtti == TAO_RealArray || rtti == TAO_CompArray) {
        TaoArrayBase *array = (TaoArrayBase *)right->interResult;
        double v = -1;
        if (oper == TAO_PPLUS)
          v = 1;
        array->assignWith(v, (char)TAO_ASSIGN_PLUS);

        interResult = array->subIndexing();
        checkArithType(interResult);
      }
    } else if (right->arithType == 1) {
      if (oper == TAO_PPLUS)
        myValue = ++right->myValue;
      else if (oper == TAO_MMINUS)
        myValue = --right->myValue;
    } else {
      TaoBase *tbase = right->getResult();
      short rtti = getExactType(tbase);
      stringstream ss;
      ss << __Tao_TypeToName[rtti] << tbase;
      if (oper == TAO_PPLUS)
        infoKit->warning("++ not supported for " + ss.str());
      else if (oper == TAO_MMINUS)
        infoKit->warning("-- not supported for " + ss.str());
    }
  }
}
void TaoArithNode::unaryArithRight() {
  if (oper == TAO_PPLUS || oper == TAO_MMINUS) {
    arithType = 1;

    if (left->interResult) {
      short rtti = left->interResult->rtti();
      if (rtti == TAO_Number) {
        TaoNumber *num = (TaoNumber *)left->interResult;
        if (oper == TAO_PPLUS)
          num->value++;
        else if (oper == TAO_MMINUS)
          num->value--;
        myValue = num->value;
      } else if (rtti == TAO_RealArray || rtti == TAO_CompArray) {
        TaoArrayBase *array = (TaoArrayBase *)left->interResult;
        double v = -1;
        if (oper == TAO_PPLUS)
          v = 1;
        array->assignWith(v, (char)TAO_ASSIGN_PLUS);

        interResult = array->subIndexing();
        checkArithType(interResult);
      }
    } else if (left->arithType == 1) {
      if (oper == TAO_PPLUS)
        myValue = ++left->myValue;
      else if (oper == TAO_MMINUS)
        myValue = --left->myValue;
    } else {
      TaoBase *tbase = left->getResult();
      short rtti = getExactType(tbase);
      stringstream ss;
      ss << __Tao_TypeToName[rtti] << tbase;
      if (oper == TAO_PPLUS)
        infoKit->warning("++ not supported for " + ss.str());
      else if (oper == TAO_MMINUS)
        infoKit->warning("-- not supported for " + ss.str());
    }
  } else if (oper == TAO_IMAGINARY) {
    arithType = 2;
    if (left->arithType == 1)
      cmplx = complex<double>(0, 1) * left->myValue;
    else if (left->arithType == 2)
      cmplx = complex<double>(0, 1) * left->cmplx;
  } else if (oper == TAO_TRANSPOSE) {
    if (left->arithType < 3)
      cout << "Error: TAO_TRANSPOSE\n";

    arithType = left->arithType;
    if (arithType == 3) {
      comArray = (TaoCompArray<double> *)left->comArray->transpose();
    } else if (arithType == 4) {
      byteArray = (TaoByteArray *)left->byteArray->transpose();
    } else if (arithType == 5) {
      shortArray = (TaoShortArray *)left->shortArray->transpose();
    } else if (arithType == 6) {
      intArray = (TaoIntArray *)left->intArray->transpose();
    } else if (arithType == 7) {
      floatArray = (TaoFloatArray *)left->floatArray->transpose();
    } else if (arithType == 8) {
      doubleArray = (TaoDoubleArray *)left->doubleArray->transpose();
    }
  }
}
void TaoArithNode::binaryArith() {
  //	cout<<(int)left->arithType<<" "<<(int)right->arithType<<"\t"
  //		<<"oper: "<<(int)oper<<"\n";
  if (left->arithType == 1 && right->arithType < 2) {
    // If the left operand is numeric scale:
    if (right->arithType == 0)
      right->myValue = atof(right->myChars.c_str());

    numberArith();
    arithType = 1;
  } else if (left->arithType == 0 && right->arithType < 2) {
    // If the left operand is a string:
    if (right->arithType == 1) {
      stringstream ss;
      ss << right->myValue;
      right->myChars = ss.str();
    }
    // For comparison:
    arithType = 1;
    stringArith();
  } else if (left->arithType <= 2 && right->arithType <= 2) {
    // If one of the operands is a complex number(there must be one):
    arithType = 2;
    if (left->arithType == 0) {
      left->myValue = atof(left->myChars.c_str());
    } else if (right->arithType == 0) {
      right->myValue = atof(right->myChars.c_str());
    }
    if (left->arithType == 2 && right->arithType == 2)
      comComArith();
    else if (left->arithType == 2)
      comNumArith();
    else
      numComArith();

  } else if ((left->arithType == 2 && right->arithType >= 4) ||
             ((left->arithType <= 2 && right->arithType == 3))) {
    // If the left operand is a real number and the right is a comparray;
    // or the left is a complex and the right one is a real array:

    if (left->arithType == 1)
      left->cmplx = left->myValue;

    arithType = 3;
    if (right->arithType == 3) {
      comArray =
          comparray_scaleL_arrayR(comArray, left->cmplx, right->comArray, oper);
    } else if (right->arithType == 4) {
      comArray = comparray_scaleL_arrayR(comArray, left->cmplx,
                                         right->byteArray, oper);
    } else if (right->arithType == 5) {
      comArray = comparray_scaleL_arrayR(comArray, left->cmplx,
                                         right->shortArray, oper);
    } else if (right->arithType == 6) {
      comArray =
          comparray_scaleL_arrayR(comArray, left->cmplx, right->intArray, oper);
    } else if (right->arithType == 7) {
      comArray = comparray_scaleL_arrayR(comArray, left->cmplx,
                                         right->floatArray, oper);
    } else if (right->arithType == 8) {
      comArray = comparray_scaleL_arrayR(comArray, left->cmplx,
                                         right->doubleArray, oper);
    }
  } else if ((left->arithType >= 4 && right->arithType == 2) ||
             ((left->arithType == 3 && right->arithType <= 2))) {
    // If the right operand is a real number and the left is a comparray;
    // or the right is a complex and the left one is a real array:

    if (right->arithType == 1)
      right->cmplx = right->myValue;

    arithType = 3;
    if (TAO_ASSIGN < oper && oper <= TAO_ASSIGN_OR) {
      if (left->arithType == 3) {
        comArray = left->comArray;
        left->comArray->assignWith(right->cmplx, oper);
      }
      return;
    }

    if (left->arithType == 3) {
      comArray =
          comparray_arrayL_scaleR(comArray, left->comArray, right->cmplx, oper);
    } else if (left->arithType == 4) {
      comArray = comparray_arrayL_scaleR(comArray, left->byteArray,
                                         right->cmplx, oper);
    } else if (left->arithType == 5) {
      comArray = comparray_arrayL_scaleR(comArray, left->shortArray,
                                         right->cmplx, oper);
    } else if (left->arithType == 6) {
      comArray =
          comparray_arrayL_scaleR(comArray, left->intArray, right->cmplx, oper);
    } else if (left->arithType == 7) {
      comArray = comparray_arrayL_scaleR(comArray, left->floatArray,
                                         right->cmplx, oper);
    } else if (left->arithType == 8) {
      comArray = comparray_arrayL_scaleR(comArray, left->doubleArray,
                                         right->cmplx, oper);
    }

  } else if (left->arithType == 3 || right->arithType == 3) {
    // If one of the two operand is a TaoCompArray:
    arithType = 3;
    if (left->arithType == 3) {
      if (right->arithType == 3) {
        comArray = (TaoCompArray<double> *)comparray_arrayR_arrayL(
            comArray, left->comArray, right->comArray, oper, infoKit);
      } else if (right->arithType == 4) {
        comArray = (TaoCompArray<double> *)comparray_arrayR_arrayL(
            comArray, left->comArray, right->byteArray, oper, infoKit);
      } else if (right->arithType == 5) {
        comArray = (TaoCompArray<double> *)comparray_arrayR_arrayL(
            comArray, left->comArray, right->shortArray, oper, infoKit);
      } else if (right->arithType == 6) {
        comArray = (TaoCompArray<double> *)comparray_arrayR_arrayL(
            comArray, left->comArray, right->intArray, oper, infoKit);
      } else if (right->arithType == 7) {
        comArray = (TaoCompArray<double> *)comparray_arrayR_arrayL(
            comArray, left->comArray, right->floatArray, oper, infoKit);
      } else if (right->arithType == 8) {
        comArray = (TaoCompArray<double> *)comparray_arrayR_arrayL(
            comArray, left->comArray, right->doubleArray, oper, infoKit);
      }
    } else {
      if (left->arithType == 4) {
        comArray = comparray_arrayR_arrayL(comArray, left->byteArray,
                                           right->comArray, oper, infoKit);
      } else if (left->arithType == 5) {
        comArray = comparray_arrayR_arrayL(comArray, left->shortArray,
                                           right->comArray, oper, infoKit);
      } else if (left->arithType == 6) {
        comArray = comparray_arrayR_arrayL(comArray, left->intArray,
                                           right->comArray, oper, infoKit);
      } else if (left->arithType == 7) {
        comArray = comparray_arrayR_arrayL(comArray, left->floatArray,
                                           right->comArray, oper, infoKit);
      } else if (left->arithType == 8) {
        comArray = comparray_arrayR_arrayL(comArray, left->doubleArray,
                                           right->comArray, oper, infoKit);
      }
    }
  } else if (left->arithType == 1 && right->arithType >= 4) {
    // If the left operand is a number
    // and the right one is a TaoRealArray:

    arithType = right->arithType;
    if (right->arithType == 4) {
      byteArray = realarray_scaleL_arrayR(byteArray, left->myValue,
                                          right->byteArray, oper);
    } else if (right->arithType == 5) {
      shortArray = realarray_scaleL_arrayR(shortArray, left->myValue,
                                           right->shortArray, oper);
    } else if (right->arithType == 6) {
      intArray = realarray_scaleL_arrayR(intArray, left->myValue,
                                         right->intArray, oper);
    } else if (right->arithType == 7) {
      floatArray = realarray_scaleL_arrayR(floatArray, left->myValue,
                                           right->floatArray, oper);
    } else if (right->arithType == 8) {
      doubleArray = realarray_scaleL_arrayR(doubleArray, left->myValue,
                                            right->doubleArray, oper);
    }
  } else if (left->arithType >= 4 && right->arithType == 1) {
    // If the left operand is a doubleArray
    // and the right one is a number:
    arithType = left->arithType;
    if (left->arithType == 4) {
      byteArray = realarray_arrayL_scaleR(byteArray, left->byteArray,
                                          right->myValue, oper);
    } else if (left->arithType == 5) {
      shortArray = realarray_arrayL_scaleR(shortArray, left->shortArray,
                                           right->myValue, oper);
    } else if (left->arithType == 6) {
      intArray = realarray_arrayL_scaleR(intArray, left->intArray,
                                         right->myValue, oper);
    } else if (left->arithType == 7) {
      floatArray = realarray_arrayL_scaleR(floatArray, left->floatArray,
                                           right->myValue, oper);
    } else if (left->arithType == 8) {
      doubleArray = realarray_arrayL_scaleR(doubleArray, left->doubleArray,
                                            right->myValue, oper);
    }
  } else {
    // If both operand are TaoRealArray
    numArrayArith();
  }
}
void TaoArithNode::assignArith() {}
void TaoArithNode::compute() {
  if (left && right) {
    if (TAO_ASSIGN <= oper && oper <= TAO_ASSIGN_OR) {
      short rtti = left->rtti();
      if (rtti == TAO_ArithVariable) {
        TaoReference *lref = ((TaoArithVariable *)left)->refVariable;
        left->compute();
        right->compute();

        if (oper == TAO_ASSIGN) {
          interResult = right->getResult();
          TaoAssign::assign(lref, interResult, infoKit);
        } else {
          binaryArith();
        }
        checkArithType(interResult);
      } else if (rtti == TAO_ArithChain) {
        TaoChain *chain = ((TaoArithChain *)left)->myChain;
        chain->subIdxNumArray = 0;
        chain->subIdxArray = 1;
        if (oper == TAO_ASSIGN)
          chain->subIdxArray = 0;

        left->compute();
        right->compute();
        if (oper == TAO_ASSIGN) {
          interResult = right->getResult();
          TaoAssign::assign(chain->result, interResult, infoKit);
        } else {
          binaryArith();
        }
        checkArithType(interResult);
      } else {
        cout << "Warning: can't do assignment\n";
      }
    } else if (oper >= TAO_TYPE_SAME) {
      left->compute();
      right->compute();
      arithType = 1;
      TaoBase *opleft = left->getResult();
      TaoBase *opright = right->getResult();
      if (!opleft || !opright) {
        if (oper == TAO_TYPE_DIFF)
          myValue = 1;
        else
          myValue = 0;
        bulean = myValue;
        return;
      }
      int rtleft = opleft->rtti();
      int rtright = opright->rtti();
      if (rtleft == rtright || (rtleft == TAO_Object && rtright == TAO_Class) ||
          (rtright == TAO_Object && rtleft == TAO_Class)) {
        rtleft = getExactType(opleft);
        rtright = getExactType(opright);
        if ((rtleft == rtright && oper == TAO_TYPE_SAME) ||
            (rtleft != rtright && oper == TAO_TYPE_DIFF))
          myValue = 1;
        else
          myValue = 0;
      } else {
        if (oper == TAO_TYPE_DIFF)
          myValue = 1;
        else
          myValue = 0;
      }
    } else {
      left->compute();
      right->compute();
      binaryArith();
    }
  } else if (left) {
    if (left->rtti() == TAO_ArithChain) {
      TaoChain *chain = ((TaoArithChain *)left)->myChain;
      chain->subIdxNumArray = 0;
    }

    left->compute();
    unaryArithRight();
  } else if (right) {
    if (right->rtti() == TAO_ArithChain) {
      TaoChain *chain = ((TaoArithChain *)right)->myChain;
      chain->subIdxNumArray = 0;
    }

    right->compute();
    unaryArithLeft();
  }
  //	cout<<(int)oper<<"\t"<<myValue<<"\n";
  bulean = myValue;
}

void TaoArithArray::compute() {
  for (size_t i = 0; i < arithArray.size(); i++)
    arithArray[i]->compute();
}
