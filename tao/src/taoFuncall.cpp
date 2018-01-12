
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
#include <complex>
#include <iostream>
#include <sstream>

#include <cstring>
#include <string.h>

#include "stdlib.h"
#include "time.h"

#include "taoArith.h"
#include "taoChain.h"
#include "taoClass.h"
#include "taoConst.h"
#include "taoExtdata.h"
#include "taoFuncall.h"
#include "taoLoadlib.h"
#include "taoNumeric.h"
#include "taoPlugin.h"
#include "taoRoutine.h"
#include "taoScript.h"
#include "taoUtility.h"

extern map<int, string> __Tao_TypeToName;

short TaoFunCall::RTTI = TAO_FunCall;
short TaoFunCall::rtti() const { return RTTI; }

TaoArrayBase *TaoNumArrayMaker::make(valarray<size_t> &dims, short type) {
  if (type == TAO_ByteArray) {
    byteArray = TaoTryAllocator<TaoByteArray>::tryAllocate(byteArray);
    byteArray->resize(dims);
    return byteArray;
  } else if (type == TAO_ShortArray) {
    shortArray = TaoTryAllocator<TaoShortArray>::tryAllocate(shortArray);
    shortArray->resize(dims);
    return shortArray;
  } else if (type == TAO_IntArray) {
    intArray = TaoTryAllocator<TaoIntArray>::tryAllocate(intArray);
    intArray->resize(dims);
    return intArray;
  } else if (type == TAO_FloatArray) {
    floatArray = TaoTryAllocator<TaoFloatArray>::tryAllocate(floatArray);
    floatArray->resize(dims);
    return floatArray;
  } else if (type == TAO_DoubleArray) {
    doubleArray = TaoTryAllocator<TaoDoubleArray>::tryAllocate(doubleArray);
    doubleArray->resize(dims);
    return doubleArray;
  } else if (type == TAO_CompArray) {
    comArray = TaoTryAllocator<TaoCompArray<double>>::tryAllocate(comArray);
    comArray->resize(dims);
    return comArray;
  }
  return 0;
}

bool TaoFunCall::runBasicMath() {
  if (funType == TAO_rand) {
    number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
    number->value = rand() / (RAND_MAX + 1.0);
    result = number;
    return 1;
  } else if (funType == TAO_randgs) {
    return 1;
  }
  if (paramAriths.size() < 1)
    return 0;
  TaoArithBase *node = paramAriths[0];
  node->compute();
  TaoBase *tbase = node->getResult();
  short rt = -1;
  if (tbase)
    rt = tbase->rtti();

  if (rt == TAO_RealArray) {
    TaoArrayBase *parray = (TaoArrayBase *)tbase;
    TaoArrayBase *array = (TaoArrayBase *)parray->duplicate();
    array->numFuncApply(funType, infoKit);
    result = array;
  } else if (rt == TAO_CompArray) {
    TaoCompArray<double> *parray = (TaoCompArray<double> *)tbase;
    if (funType == TAO_arg || funType == TAO_norm) {
      TaoDoubleArray *array = (TaoDoubleArray *)numarrayMaker->make(
          parray->getDims(), TAO_DoubleArray);
      double *real = array->getAsVector();
      complex<double> *comp = parray->getAsVector();
      if (funType == TAO_arg) {
        for (size_t i = 0; i < array->size(); i++)
          real[i] = arg(comp[i]);
      } else {
        for (size_t i = 0; i < array->size(); i++)
          real[i] = norm(comp[i]);
      }
      result = array;
    } else {
      TaoArrayBase *array = (TaoArrayBase *)parray->duplicate();
      array->comFuncApply(funType, infoKit);
      result = array;
    }
  } else if (rt == TAO_Complex) {
    if (funType == TAO_arg || funType == TAO_norm) {
      number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
      result = number;
      if (funType == TAO_arg)
        number->value = arg(node->cmplx);
      else
        number->value = norm(node->cmplx);
    } else {
      complx = TaoTryAllocator<TaoComplex>::tryAllocate(complx);
      result = complx;
      if (funType == TAO_abs)
        complx->cmplx = abs(node->cmplx);
      else if (funType == TAO_cos)
        complx->cmplx = cos(node->cmplx);
      else if (funType == TAO_cosh)
        complx->cmplx = cosh(node->cmplx);
      else if (funType == TAO_exp)
        complx->cmplx = exp(node->cmplx);
      else if (funType == TAO_log)
        complx->cmplx = log(node->cmplx);
      else if (funType == TAO_sin)
        complx->cmplx = sin(node->cmplx);
      else if (funType == TAO_sinh)
        complx->cmplx = sinh(node->cmplx);
      else if (funType == TAO_sqrt)
        complx->cmplx = sqrt(node->cmplx);
    }
  } else {
    number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
    result = number;
    if (funType == TAO_abs)
      number->value = abs(node->myValue);
    else if (funType == TAO_acos)
      number->value = acos(node->myValue);
    else if (funType == TAO_asin)
      number->value = asin(node->myValue);
    else if (funType == TAO_atan)
      number->value = atan(node->myValue);
    else if (funType == TAO_cos)
      number->value = cos(node->myValue);
    else if (funType == TAO_cosh)
      number->value = cosh(node->myValue);
    else if (funType == TAO_exp)
      number->value = exp(node->myValue);
    else if (funType == TAO_log)
      number->value = log(node->myValue);
    else if (funType == TAO_srand)
      srand((unsigned int)node->myValue);
    else if (funType == TAO_sin)
      number->value = sin(node->myValue);
    else if (funType == TAO_sinh)
      number->value = sinh(node->myValue);
    else if (funType == TAO_tan)
      number->value = tan(node->myValue);
    else if (funType == TAO_tanh)
      number->value = tanh(node->myValue);
    else if (funType == TAO_sqrt)
      number->value = sqrt(node->myValue);
  }

  return 1;
}
bool TaoFunCall::runNumMath() {
  if (funType <= TAO_count) {
    short rtti = paramAriths[0]->rtti();
    if (rtti == TAO_ArithChain) {
      TaoChain *chain = ((TaoArithChain *)paramAriths[0])->myChain;
      chain->subIdxNumArray = 0;
      chain->subIdxArray = 1;
    }
    paramAriths[0]->compute();

    TaoBase *tbase = paramAriths[0]->getResult();
    if (tbase)
      tbase = tbase->dereference();
    if (!tbase) {
      infoKit->warning("invalid parameter");
      return 1;
    }
    short rtti2 = tbase->rtti();
    if (rtti2 == TAO_Array) {
      TaoArray *array = (TaoArray *)tbase;
      if (rtti != TAO_ArithChain)
        array->resetSubIndex();
      result = array->basicMathFunct(infoKit, funType);

    } else if (rtti2 == TAO_RealArray) {
      TaoRealArray<double> *array = (TaoRealArray<double> *)tbase;
      if (rtti != TAO_ArithChain)
        array->resetSubIndex();

      if (funType == TAO_count) {
        number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
        number->value = array->count();
        result = number;
      } else {
        result = array->basicMathFunct(infoKit, funType);
      }
    } else if (rtti2 == TAO_CompArray) {
      TaoCompArray<double> *array = (TaoCompArray<double> *)tbase;
      if (rtti != TAO_ArithChain)
        array->resetSubIndex();

      if (funType == TAO_count) {
        number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
        number->value = array->count();
        result = number;
      } else {
        result = array->basicMathFunct(infoKit, funType);
      }
    }
    return 1;
  } else if (funType == TAO_permute || funType == TAO_invpermute) {
    bool inverse = (funType == TAO_invpermute);
    paramAriths[0]->compute();
    paramAriths[1]->compute();
    TaoBase *tbase1 = paramAriths[0]->getResult();
    TaoBase *tbase2 = paramAriths[1]->getResult();
    if (tbase1)
      tbase1 = tbase1->dereference();
    if (tbase2)
      tbase2 = tbase2->dereference();
    if (!tbase1 || !tbase2) {
      infoKit->warning("invalid parameter");
      return 1;
    }
    short rtti1 = tbase1->rtti();
    short rtti2 = tbase2->rtti();
    if (rtti2 == TAO_RealArray) {
      valarray<size_t> perm;
      ((TaoArrayBase *)tbase2)->exportAsIndex(perm);
      if (rtti1 == TAO_RealArray) {
        short rtti = ((TaoArrayBase *)tbase1)->rttiNumeric();
        if (rtti == TAO_ByteArray) {
          TaoByteArray *array = (TaoByteArray *)tbase1;
          result = array->permute(perm, inverse);
        } else if (rtti == TAO_ShortArray) {
          TaoShortArray *array = (TaoShortArray *)tbase1;
          result = array->permute(perm, inverse);
        } else if (rtti == TAO_IntArray) {
          TaoIntArray *array = (TaoIntArray *)tbase1;
          result = array->permute(perm, inverse);
        } else if (rtti == TAO_FloatArray) {
          TaoFloatArray *array = (TaoFloatArray *)tbase1;
          result = array->permute(perm, inverse);
        } else if (rtti == TAO_DoubleArray) {
          TaoDoubleArray *array = (TaoDoubleArray *)tbase1;
          result = array->permute(perm, inverse);
        }
      } else if (rtti1 == TAO_CompArray) {
        TaoCompArray<double> *array = (TaoCompArray<double> *)tbase1;
        result = array->permute(perm, inverse);
      }
    }
  } else if (funType == TAO_convolute) {
    if (paramAriths.size() < 2)
      infoKit->warning("not enough parameters for convolute(), skiped");
    paramAriths[0]->compute();
    paramAriths[1]->compute();
    TaoBase *tbase1 = paramAriths[0]->getResult();
    TaoBase *tbase2 = paramAriths[1]->getResult();
    if (tbase1)
      tbase1 = tbase1->dereference();
    if (tbase2)
      tbase2 = tbase2->dereference();
    if (!tbase1 || !tbase2) {
      infoKit->warning("invalid parameter");
      return 1;
    }
    TaoArrayBase *out = 0;
    if (paramAriths.size() == 3) {
      paramAriths[2]->compute();
      TaoBase *tbase3 = paramAriths[2]->getResult();
      if (tbase3)
        tbase3 = tbase3->dereference();
      if (!tbase3) {
        infoKit->warning("invalid parameter");
        return 1;
      }
      if (tbase3->rtti() == TAO_RealArray || tbase3->rtti() == TAO_CompArray)
        out = (TaoArrayBase *)tbase3;
    }
    short rtti1 = tbase1->rtti();
    short rtti2 = tbase2->rtti();
    if (rtti1 == TAO_RealArray && rtti2 == TAO_RealArray) {
      short rtti = ((TaoArrayBase *)tbase1)->rttiNumeric();
      if (rtti == TAO_ByteArray) {
        TaoByteArray *array = (TaoByteArray *)tbase1;
        result = array->convolute((TaoArrayBase *)tbase2, out, infoKit);
      } else if (rtti == TAO_ShortArray) {
        TaoShortArray *array = (TaoShortArray *)tbase1;
        result = array->convolute((TaoArrayBase *)tbase2, out, infoKit);
      } else if (rtti == TAO_IntArray) {
        TaoIntArray *array = (TaoIntArray *)tbase1;
        result = array->convolute((TaoArrayBase *)tbase2, out, infoKit);
      } else if (rtti == TAO_FloatArray) {
        TaoFloatArray *array = (TaoFloatArray *)tbase1;
        result = array->convolute((TaoArrayBase *)tbase2, out, infoKit);
      } else if (rtti == TAO_DoubleArray) {
        TaoDoubleArray *array = (TaoDoubleArray *)tbase1;
        result = array->convolute((TaoArrayBase *)tbase2, out, infoKit);
      }
    } else if (rtti1 == TAO_CompArray &&
               (rtti2 == TAO_RealArray || rtti2 == TAO_CompArray)) {
      TaoCompArray<double> *array = (TaoCompArray<double> *)tbase1;
      result = array->convolute((TaoArrayBase *)tbase2, out, infoKit);
    }
  }
  return 1;
}
bool TaoFunCall::runMagicFunct() {
  if (funType == TAO_apply || funType == TAO_noapply) {
    bool apply = 0;
    if (funType == TAO_apply)
      apply = 1;

    TaoBase *tbase;

    short rtti = paramAriths[0]->rtti();
    if (rtti == TAO_ArithChain) {
      TaoChain *chain = ((TaoArithChain *)paramAriths[0])->myChain;
      chain->subIdxNumArray = 0;
      chain->subIdxArray = 0;
      chain->execute();
      tbase = chain->result;
    } else {
      paramAriths[0]->compute();
      tbase = paramAriths[0]->getResult();
      if (tbase)
        tbase = tbase->dereference();
    }

    short rtti2 = tbase->rtti();
    if (rtti2 == TAO_RealArray) {
      TaoArrayBase *numArray = (TaoArrayBase *)tbase;
      if (rtti == TAO_ArithVariable)
        numArray->resetSubIndex();
      numArray->numIterate(paramAriths[1], transient, apply, infoKit);
    } else if (rtti2 == TAO_CompArray) {
      TaoCompArray<double> *comArray = (TaoCompArray<double> *)tbase;
      if (rtti == TAO_ArithVariable)
        comArray->resetSubIndex();
      comArray->comIterate(paramAriths[1], transient, apply, infoKit);
    }
    return 1;
  } else if (funType == TAO_iterate || funType == TAO_iterget) {
    if (paramAriths[0]->rtti() == TAO_PairArith) {

      if (funType == TAO_iterget) {
        myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
        myArray->clear();
      }

      TaoPairArith *arith = (TaoPairArith *)paramAriths[0];
      arith->compute();
      int start = (int)arith->left->myValue;
      int end = (int)arith->right->myValue;
      for (int i = start; i <= end; i++) {
        number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
        number->value = i;
        transient[0]->content = number;
        if (paramAriths.size() > 2) {
          for (size_t j = 1; j < paramAriths.size(); j++)
            paramAriths[j]->compute();

          if (funType == TAO_iterget) {
            TaoArray *array2 = new TaoArray;
            for (size_t j = 1; j < paramAriths.size(); j++)
              array2->append(paramAriths[j]->getResult());
            myArray->append(array2);
          }
        } else if (paramAriths.size() == 2) {
          paramAriths[1]->compute();
          if (funType == TAO_iterget)
            myArray->append(paramAriths[1]->getResult());
        }
      }
    } else {
      paramAriths[0]->compute();
      TaoArray *array = (TaoArray *)paramAriths[0]->getResult();
      if (array)
        array = (TaoArray *)array->dereference();
      if (!array) {
        infoKit->warning("invalid parameter");
        return 1;
      }
      if (array->rtti() == TAO_Array) {
        if (funType == TAO_iterget) {
          myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
          myArray->clear();
        }
        for (size_t i = 0; i < array->elements.size(); i++) {
          transient[0]->content = array->elements[i];
          if (paramAriths.size() > 2) {
            for (size_t j = 1; j < paramAriths.size(); j++)
              paramAriths[j]->compute();
            if (funType == TAO_iterget) {
              TaoArray *array2 = new TaoArray;
              for (size_t j = 1; j < paramAriths.size(); j++)
                array2->append(paramAriths[j]->getResult());
              myArray->append(array2);
            }
          } else if (paramAriths.size() == 2) {
            paramAriths[1]->compute();
            if (funType == TAO_iterget)
              myArray->append(paramAriths[1]->getResult());
          }
        }
        result = myArray;
      } else {
        transient[0]->content = array;
        for (size_t j = 1; j < paramAriths.size(); j++)
          paramAriths[j]->compute();
        if (funType == TAO_iterget) {
          for (size_t j = 0; j < paramAriths.size(); j++)
            myArray->append(paramAriths[j]->getResult());
        }
      }
    }
    return 1;
  } else if (funType == TAO_repeat) {
    myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
    myArray->clear();
    paramAriths[0]->compute();
    myArray->append(paramAriths[0]->getResult());
    while (paramAriths[0]->bulean) {
      paramAriths[0]->compute();
      myArray->append(paramAriths[0]->getResult());
    }
    result = myArray;
    return 1;
  } else if (funType == TAO_sort) {
    paramAriths[0]->compute();
    TaoBase *tbase = paramAriths[0]->getResult();
    if (tbase)
      tbase = tbase->dereference();
    int n = -1;
    if (paramAriths.size() == 3) {
      paramAriths[2]->compute();
      n = (int)paramAriths[2]->myValue;
    }
    if (tbase && paramAriths.size() > 1 && tbase->rtti() == TAO_Array) {
      TaoArray *array = (TaoArray *)tbase;
      array->heapSort(paramAriths[1], n, transient, infoKit);
      result = array;
    } else {
      infoKit->warning("invalid parameter");
    }
    return 1;
  } else if (funType == TAO_which) {
    short rtti = paramAriths[0]->rtti();
    if (rtti == TAO_ArithNode) {
      TaoArithChain *charith = (TaoArithChain *)paramAriths[0]->left;

      TaoBase *tbase = 0;
      rtti = charith->rtti();
      if (rtti == TAO_ArithChain) {
        TaoChain *chain = charith->myChain;
        chain->subIdxNumArray = 0;
        chain->subIdxArray = 0;
        chain->execute();
        tbase = chain->result;
      } else if (rtti == TAO_ArithVariable) {
        charith->compute();
        tbase = charith->getResult();
        if (tbase)
          tbase = tbase->dereference();
      } else {
        infoKit->warning("in which()");
      }

      TaoArithBase *arith = paramAriths[0]->right;
      arith->compute();

      short rtti2 = tbase->rtti();
      TaoArithNode *node2 = (TaoArithNode *)paramAriths[0];

      if (rtti2 == TAO_Array) {
        TaoArray *array = (TaoArray *)tbase;
        if (rtti == TAO_ArithVariable)
          array->resetSubIndex();
        result = array->whichByOper(node2->oper, arith->getResult());
      } else if (rtti2 == TAO_RealArray) {
        TaoArrayBase *numArray = (TaoArrayBase *)tbase;
        if (rtti == TAO_ArithVariable)
          numArray->resetSubIndex();
        result = numArray->numWhichByOper(node2->oper, arith->myValue);
      } else if (rtti2 == TAO_CompArray) {
        TaoCompArray<double> *comArray = (TaoCompArray<double> *)tbase;
        if (rtti == TAO_ArithVariable)
          comArray->resetSubIndex();
        result = comArray->comWhichByOper(node2->oper, arith->cmplx);
      }
    } else if (paramAriths.size() > 1 &&
               (rtti == TAO_ArithChain || rtti == TAO_ArithVariable)) {
      TaoBase *tbase;
      if (rtti == TAO_ArithChain) {
        TaoChain *chain = ((TaoArithChain *)paramAriths[0])->myChain;
        chain->subIdxNumArray = 0;
        chain->subIdxArray = 0;
        chain->execute();
        tbase = chain->result;
      } else {
        paramAriths[0]->compute();
        tbase = paramAriths[0]->getResult();
        if (tbase)
          tbase = tbase->dereference();
      }

      short rtti2 = tbase->rtti();
      if (rtti2 == TAO_Array) {
        TaoArray *array = (TaoArray *)tbase;
        if (rtti == TAO_ArithVariable)
          array->resetSubIndex();
        result = array->whichByArith(paramAriths[1], transient);
      } else if (rtti2 == TAO_RealArray) {
        TaoArrayBase *numArray = (TaoArrayBase *)tbase;
        if (rtti == TAO_ArithVariable)
          numArray->resetSubIndex();
        result = numArray->numWhich(paramAriths[1], transient);
      } else if (rtti2 == TAO_CompArray) {
        TaoCompArray<double> *comArray = (TaoCompArray<double> *)tbase;
        if (rtti == TAO_ArithVariable)
          comArray->resetSubIndex();
        result = comArray->comWhich(paramAriths[1], transient);
      }
    } else {
      infoKit->warning("in which()");
    }
  } else if (funType == TAO_numarray) {
    paramAriths[0]->compute();
    TaoArrayBase *array = (TaoArrayBase *)paramAriths[0]->getResult();
    valarray<size_t> dims;
    if (!array || array->rtti() != TAO_RealArray || array->dim() > 2 ||
        (array->dim() == 2 && array->dim(0) > 1)) {
      infoKit->error_general("first parameter is not valid");
    }
    array->exportAsIndex(dims);
    short type = TAO_DoubleArray;
    if (paramAriths.size() > 2) {
      paramAriths[2]->compute();
      TaoBase *tbase = paramAriths[2]->getResult();
      if (tbase && tbase->rtti() == TAO_String) {
        string &s = paramAriths[2]->myChars;
        if (s == "byte")
          type = TAO_ByteArray;
        else if (s == "short")
          type = TAO_ShortArray;
        else if (s == "int")
          type = TAO_IntArray;
        else if (s == "float")
          type = TAO_FloatArray;
        else if (s == "double")
          type = TAO_DoubleArray;
        else if (s == "complex")
          type = TAO_CompArray;
      }
    }
    TaoArrayBase *array2 = numarrayMaker->make(dims, type);
    if (paramAriths.size() > 1) {
      if (type == TAO_CompArray) {
        array2->comIterate(paramAriths[1], transient, 1, infoKit);
      } else {
        array2->numIterate(paramAriths[1], transient, 1, infoKit);
      }
    }
    result = array2;
  }
  return 1;
}

bool TaoFunCall::runUtileFunct() {
  if (funType == TAO_time) {
    number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
    result = number;
    number->value = (double)time(NULL);
    return 1;
  }

  if (paramAriths.size() < 1)
    return 0;
  TaoArithBase *node = paramAriths[0];
  node->compute();

  if (funType == TAO_about) {
    tstring = TaoTryAllocator<TaoString>::tryAllocate(tstring);
    result = tstring;
    TaoBase *tbase = paramAriths[0]->getResult();
    short rtti = getExactType(tbase);
    stringstream ss;
    ss << __Tao_TypeToName[rtti] << "_" << tbase;
    for (size_t i = 1; i < paramAriths.size(); i++) {
      paramAriths[i]->compute();
      tbase = paramAriths[i]->getResult();
      rtti = getExactType(tbase);
      ss << " " << __Tao_TypeToName[rtti] << "_" << tbase;
    }
    tstring->chars = ss.str();
  } else if (funType == TAO_system) {
    number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
    result = number;
    number->value = system(node->myChars.c_str());
  } else if (funType == TAO_astime) {
    myHash = TaoTryAllocator<TaoHash>::tryAllocate(myHash);
    myHash->clear();
    result = myHash;
    size_t daysec = 24 * 3600;
    size_t seconds = (size_t)paramAriths[0]->myValue;
    string type = "";
    if (paramAriths.size() > 1) {
      paramAriths[1]->compute();
      TaoBase *tbase = paramAriths[1]->getResult();
      if (tbase->rtti() == TAO_String)
        type = paramAriths[1]->myChars;
    }
    if (type == "day") {
      myHash->insert("days", new TaoNumber(seconds / daysec));
      myHash->insert("hours", new TaoNumber((seconds % daysec) / 3600));
      myHash->insert("minutes", new TaoNumber((seconds % 3600) / 60));
      myHash->insert("seconds", new TaoNumber(seconds % 60));
    } else if (type == "hour") {
      myHash->insert("hours", new TaoNumber(seconds / 3600));
      myHash->insert("minutes", new TaoNumber((seconds % 3600) / 60));
      myHash->insert("seconds", new TaoNumber(seconds % 60));
    } else if (type == "minute") {
      myHash->insert("minutes", new TaoNumber(seconds / 60));
      myHash->insert("seconds", new TaoNumber(seconds % 60));
    } else {
      myHash->insert("seconds", new TaoNumber(seconds));
    }
  } else if (funType == TAO_asctime) {
    myHash = TaoTryAllocator<TaoHash>::tryAllocate(myHash);
    myHash->clear();
    result = myHash;
    time_t t = (time_t)paramAriths[0]->myValue;
    struct tm *ctime = gmtime(&t);
    ctime->tm_year += 1900;
    myHash->insert("year", new TaoNumber(ctime->tm_year));
    myHash->insert("month", new TaoNumber(ctime->tm_mon + 1));
    myHash->insert("day", new TaoNumber(ctime->tm_mday));
    myHash->insert("hour", new TaoNumber(ctime->tm_hour));
    myHash->insert("minute", new TaoNumber(ctime->tm_min));
    myHash->insert("second", new TaoNumber(ctime->tm_sec));
    myHash->insert("wday", new TaoNumber(ctime->tm_wday));
    myHash->insert("yday", new TaoNumber(ctime->tm_yday + 1));
    myHash->insert("isdst", new TaoNumber(ctime->tm_isdst));
  } else if (funType == TAO_load) {
    string fname = node->myChars;

    TaoLibrary *lib = new TaoLibrary;
    gcAgent.appendNewObject(lib);
    result = lib;

    if (!Tao_DynLoad_Lib(lib, fname)) {
      infoKit->error_general("Cpp Module " + fname + " loading failed");
      return 0;
    }
  } else if (funType == TAO_compile || funType == TAO_eval ||
             funType == TAO_import) {
    TaoScript tao;
    bool replace = 0;
    if (funType == TAO_import) {
      replace = 1;
      tao.readSource(node->myChars.c_str());
    } else {
      tao.setSource(node->myChars);
    }

    int nparam = paramAriths.size();
    TaoNameSpace *nsIn = 0;
    TaoNameSpace *nsOut = 0;
    if (nparam == 1) {
      nsIn = new TaoNameSpace;
      gcAgent.appendNewObject(nsIn);
      nsOut = nsIn;
    } else if (nparam == 2) {
      paramAriths[1]->compute();
      nsIn = (TaoNameSpace *)paramAriths[1]->getResult();
      short rt = -1;
      if (nsIn)
        rt = nsIn->rtti();
      if (rt != TAO_NameSpace) {
        infoKit->warning("parameter is not a namespace, abort");
        return 1;
      }
      nsOut = new TaoNameSpace;
      gcAgent.appendNewObject(nsOut);
    } else if (nparam == 3) {
      paramAriths[1]->compute();
      paramAriths[2]->compute();
      nsIn = (TaoNameSpace *)paramAriths[1]->getResult();
      nsOut = (TaoNameSpace *)paramAriths[2]->getResult();
      short rt1 = -1;
      short rt2 = -1;
      if (nsIn)
        rt1 = nsIn->rtti();
      if (nsOut)
        rt2 = nsOut->rtti();
      if (rt1 != TAO_NameSpace || rt2 != TAO_NameSpace) {
        infoKit->warning("parameter is not namespaces, abort");
        return 1;
      }
    }
    if (!tao.compile(nsIn, nsOut, replace)) {
      if (funType == TAO_import)
        infoKit->error_general("importing " + node->myChars + " failed");
      else
        infoKit->error_general("compiling failed");
    }
    if (funType == TAO_eval || funType == TAO_import) {
      TaoRoutine *rout = nsOut->mainModule->getRoutine();
      rout->run();
      blResult = rout->moreToYield();
    }
    result = nsOut;
  } else if (funType == TAO_type) {
    number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
    result = number;
    number->value = 0;
    TaoBase *tbase = node->getResult();
    number->value = getExactType(tbase);
    result = number;
  } else if (funType == TAO_number) {
    int xb = 10;
    if (paramAriths.size() > 1) {
      paramAriths[1]->compute();
      xb = (int)paramAriths[1]->myValue;
    }
    TaoBase *tbase = paramAriths[0]->getResult();
    short rtti = -1;
    if (tbase)
      rtti = tbase->rtti();
    if (rtti == TAO_String) {
      number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
      TaoString *str = (TaoString *)tbase;
      number->value = xBaseNumber(str->chars, xb, infoKit);
      result = number;
    } else if (rtti == TAO_Array) {
      myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
      myArray->clear();
      TaoArray *array = (TaoArray *)tbase;
      TaoString *str;
      for (size_t i = 0; i < array->size(); i++) {
        str = (TaoString *)array->elements[i];
        if (str->rtti() == TAO_String)
          myArray->append(new TaoNumber(xBaseNumber(str->chars, xb, infoKit)));
      }
      result = myArray;
    }
  } else if (funType == TAO_pack) {
    tstring = TaoTryAllocator<TaoString>::tryAllocate(tstring);
    result = tstring;
    tstring->chars.erase();
    TaoBase *tbase = paramAriths[0]->getResult();
    short rtti = -1;
    if (tbase)
      rtti = tbase->rtti();
    if (rtti == TAO_Number) {
      TaoNumber *num = (TaoNumber *)tbase;
      tstring->chars += char((int)num->value);
    } else if (rtti == TAO_Array) {
      TaoArray *array = (TaoArray *)tbase;
      TaoNumber *num;
      for (size_t i = 0; i < array->size(); i++) {
        num = (TaoNumber *)array->elements[i];
        if (num->rtti() == TAO_Number)
          tstring->chars += char((int)num->value);
      }
    }
  } else if (funType == TAO_unpack) {
    myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
    myArray->clear();
    TaoBase *tbase = paramAriths[0]->getResult();
    short rtti = -1;
    if (tbase)
      rtti = tbase->rtti();
    if (rtti == TAO_String) {
      TaoString *str = (TaoString *)tbase;
      string &chars = str->chars;
      for (size_t i = 0; i < chars.size(); i++) {
        myArray->append(new TaoNumber(chars[i]));
      }
    }
    result = myArray;
  }

  return 1;
}
bool TaoFunCall::runMethod(TaoLibrary *lib, string funame) {
  map<string, TaoPlugin *>::iterator itplg = lib->libPlugin.find(funame);
  if (itplg != lib->libPlugin.end()) {
    TaoPlugin *plg = itplg->second;
    TaoCppObject *obj = new TaoCppObject;
    gcAgent.appendNewObject(obj);

    obj->cppObject = plg->newObject((TcArray *)inArray.toCppType());

    inArray.fromCppType();
    if (recycler)
      recycler->cleanTcObjects();
    result = obj;
    return 1;
  }
  return 1;
}
bool TaoFunCall::runMethod(TaoHash *caller) {
  if (funType != TAO_keys && funType != TAO_values) {
    infoKit->warning("no such method for hash type");
    return 1;
  }
  myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
  myArray->clear();
  result = myArray;

  map<string, TaoBase *>::iterator iter;
  for (iter = caller->elements.begin(); iter != caller->elements.end();
       iter++) {
    if (funType == TAO_keys) {
      myArray->append(new TaoString(iter->first));
    } else if (funType == TAO_values) {
      myArray->append(iter->second);
    }
  }
  return 1;
}
bool TaoFunCall::runMethod(TaoArray *caller) {
  if (funType == TAO_size) {
    number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
    result = number;
    if (caller->subIndex.size() == 0)
      number->value = caller->elements.size();
    else
      number->value = caller->subIndex.size();
  } else if (funType == TAO_insert) {
    int pos = caller->elements.size();
    if (paramAriths.size() > 1) {
      TaoBase *pid = paramAriths[1]->getResult();
      if (pid)
        pid = pid->dereference();
      if (pid && pid->rtti() != TAO_Number) {
        infoKit->warning("2nd parameter must be a number");
        return 1;
      }
      TaoNumber *num = (TaoNumber *)pid;
      pos = (int)num->value;
    }

    TaoBase *tbase = paramAriths[0]->getResult();
    if (!tbase) {
      infoKit->warning("null object is not inserted");
      return 1;
    }
    short rtd = tbase->rtti();
    short rta = paramAriths[0]->rtti();
    if (rta == TAO_ArithVariable) {
      // for(i:=0;i<5;i++){ a.insert(i); }
      if (rtd == TAO_Number || rtd == TAO_String || rtd == TAO_Complex)
        tbase = tbase->duplicate();
    }
    caller->insert(tbase, pos);
  } else if (funType == TAO_erase) {
    int n1 = 0;
    if (paramAriths.size() > 0) {
      TaoBase *pid1 = paramAriths[0]->getResult();
      if (pid1)
        pid1 = pid1->dereference();
      if (pid1 && pid1->rtti() != TAO_Number) {
        infoKit->warning("1st parameter must be a number");
        return 1;
      }
      TaoNumber *num1 = (TaoNumber *)pid1;
      n1 = (int)num1->value;
    }

    int n2 = -1 + (int)caller->elements.size();
    if (paramAriths.size() > 1) {
      TaoBase *pid2 = paramAriths[1]->getResult();
      if (pid2)
        pid2 = pid2->dereference();
      if (pid2 && pid2->rtti() != TAO_Number) {
        infoKit->warning("2nd parameter must be a number");
        return 1;
      }
      TaoNumber *num2 = (TaoNumber *)pid2;
      n2 = (int)num2->value;
    }
    for (int i = n1; i <= n2; i++)
      caller->erase(n1);
  } else if (funType == TAO_flat) {
    myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
    myArray->clear();
    caller->exportElements(myArray);
    result = myArray;
  } else {
    infoKit->warning("no such method for array type");
  }
  return 1;
}
bool TaoFunCall::runMethod(TaoArrayBase *caller) {
  if (funType == TAO_size) {
    result = caller->dimSize();
    return 1;
  }
  if (paramAriths.size() < 1) {
    infoKit->warning("not enough parameters");
    return 1;
  }

  result = caller;
  TaoArrayBase *array = (TaoArrayBase *)paramAriths[0]->getResult();
  short rtti = array->rtti();
  if (funType == TAO_reshape) {
    valarray<size_t> dm;
    if (paramAriths.size() == 1 && rtti == TAO_RealArray) {
      if (array->dim() > 1 && array->dim(0) > 1 && array->dim(1) > 1)
        infoKit->warning("improper numeric array shape as parameter");
      array->exportAsIndex(dm);
    } else {
      dm.resize(paramAriths.size());
      for (size_t i = 0; i < paramAriths.size(); i++)
        dm[i] = (size_t)paramAriths[i]->myValue;
    }
    caller->reshape(dm, infoKit);
  } else if (funType == TAO_resize) {
    valarray<size_t> dm;
    if (paramAriths.size() == 1 && rtti == TAO_RealArray) {
      if (array->dim() > 1 && array->dim(0) > 1 && array->dim(1) > 1)
        infoKit->warning("improper numeric array shape as parameter");
      array->exportAsIndex(dm);
    } else {
      dm.resize(paramAriths.size());
      for (size_t i = 0; i < paramAriths.size(); i++)
        dm[i] = (size_t)paramAriths[i]->myValue;
    }
    caller->resize(dm);
  } else {
    infoKit->warning("no such method for numeric array");
  }

  return 1;
}
bool TaoFunCall::runMethod(TaoCompArray<double> *caller) {
  if (funType == TAO_size) {
    result = caller->dimSize();
    return 1;
  }
  if (paramAriths.size() < 1) {
    infoKit->warning("not enough parameters");
    return 1;
  }

  result = caller;
  TaoArrayBase *array = (TaoArrayBase *)paramAriths[0]->getResult();
  short rtti = array->rtti();
  if (funType == TAO_reshape) {
    valarray<size_t> dm;
    if (paramAriths.size() == 1 && rtti == TAO_RealArray) {
      if (array->dim() > 1 && array->dim(0) > 1 && array->dim(1) > 1)
        infoKit->warning("improper numeric array shape as parameter");
      array->exportAsIndex(dm);
    } else {
      dm.resize(paramAriths.size());
      for (size_t i = 0; i < paramAriths.size(); i++)
        dm[i] = (size_t)paramAriths[i]->myValue;
    }
    caller->reshape(dm, infoKit);
  } else if (funType == TAO_resize) {
    valarray<size_t> dm;
    if (paramAriths.size() == 1 && rtti == TAO_RealArray) {
      if (array->dim() > 1 && array->dim(0) > 1 && array->dim(1) > 1)
        infoKit->warning("improper numeric array shape as parameter");
      array->exportAsIndex(dm);
    } else {
      dm.resize(paramAriths.size());
      for (size_t i = 0; i < paramAriths.size(); i++)
        dm[i] = (size_t)paramAriths[i]->myValue;
    }
    caller->resize(dm);
  } else
    infoKit->warning("no such method for complex numeric array");

  return 1;
}
bool TaoFunCall::runMethod(TaoAddOn *caller) {
  myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
  myArray->clear();
  caller->runMethod(funName, &inArray, myArray);
  result = myArray;
  return 1;
}
bool TaoFunCall::runMethod(TaoPlugin *caller) {
  myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
  myArray->clear();
  TcArray *temp = new TcArray;
  caller->runMethod(funName.c_str(), (TcArray *)inArray.toCppType(), temp);

  delete myArray;
  myArray = (TaoArray *)fromCppType(temp);

  inArray.fromCppType();
  if (recycler)
    recycler->cleanTcObjects();

  if (myArray->size() > 1)
    result = myArray;
  else if (myArray->size() == 1)
    result = myArray->elements[0];
  else
    result = 0;

  return 1;
}
bool TaoFunCall::runMethod(TaoObject *caller) {
  TaoRoutine *rout = 0;

  TaoClass *klass = caller->myClass;

  TaoModule *module = klass->getPubMethod(funName);
  if (!module) {
    infoKit->error_general(
        "\"" + klass->className + "::" + funName +
        "\" is a private, or protected, or undefined class method");
  }
  TaoClass *hostClass = module->getSuperClass();
  TaoObject *obj = caller;
  while (obj) {
    if (obj->myClass == hostClass)
      break;
    obj = obj->getSuperObject();
  }
  rout = module->getRoutine();
  rout->setHostObject(obj);
  rout->mapObjectData();

  bool bl = rout->run(&inArray);
  blResult = rout->moreToYield();
  if (!bl)
    return 0;

  result = rout->getResult();

  return 1;
}
bool TaoFunCall::runMethod(TaoNameSpace *caller) {
  if (TaoAddOn *addon = caller->findAddOn(funName)) {
    addon = addon->newObject(&inArray);
    result = addon;
  } else if (TaoClass *klass = caller->findClass(funName)) {
    TaoObject *obj = new TaoObject(klass);
    obj->init(&inArray);
    gcAgent.appendNewObject(obj);
    result = obj;
  } else if (TaoModule *module = caller->findModule(funName)) {
    TaoRoutine *rout = module->getRoutine();
    rout->run(&inArray);
    blResult = rout->moreToYield();
    result = rout->getResult();
  } else if (TaoReference *data = caller->findData(funName)) {
    result = data;
  }
  return 1;
}
bool TaoFunCall::runMethod(TaoModule *caller) {
  if (funName == "run") {
    TaoRoutine *rout = caller->getRoutine();
    rout->run(&inArray);
    blResult = rout->moreToYield();
    result = rout->getResult();
  } else {
    infoKit->warning("unknown method \"" + funName + "\"");
  }
  return 1;
}
bool TaoFunCall::runIOFile() {
  neof = 0;
  if (ioType == 0) {
    // open()
    if (paramAriths.size() != 2) {
      infoKit->warning("Invalid parameters");
    }
    paramAriths[0]->compute();
    if (paramAriths[0]->getArithType() != 0) {
      infoKit->warning("Invalid file name");
    }
    paramAriths[1]->compute();
    if (paramAriths[1]->getArithType() != 0) {
      infoKit->warning("Invalid file IO type");
    }
    if (paramAriths[1]->myChars == "r") {
      TaoFin *tfin = new TaoFin();
      gcAgent.appendNewObject(tfin);
      tfin->fname = paramAriths[0]->myChars;
      tfin->fin = new ifstream(tfin->fname.c_str());
      if (!tfin->fin->is_open())
        infoKit->error_general("can't open file " + tfin->fname);
      result = tfin;
    } else if (paramAriths[1]->myChars == "w") {
      TaoFout *tfout = new TaoFout();
      gcAgent.appendNewObject(tfout);
      tfout->fname = paramAriths[0]->myChars;
      tfout->fout = new ofstream(tfout->fname.c_str());
      if (!tfout->fout->is_open())
        infoKit->error_general("can't open file " + tfout->fname);
      result = tfout;
    } else {
      infoKit->warning("Unsupported type");
    }
  } else {
    // print() or read()
    ostream *out = &cout;
    ifstream *fin = 0;
    if (funCaller) {
      short rt = funCaller->rtti();
      if (rt == TAO_Fin) {
        TaoFin *tfin = (TaoFin *)funCaller;
        fin = tfin->fin;
      } else if (rt == TAO_Fout) {
        TaoFout *tfout = (TaoFout *)funCaller;
        out = tfout->fout;
      } else {
        infoKit->error_general("not a file object");
      }
    }
    if (ioType == 1) {
      for (size_t i = 0; i < paramAriths.size(); i++) {
        paramAriths[i]->compute();
        paramAriths[i]->print(out);
      }
    } else {
      if (paramAriths.size() == 0) {
        tstring = TaoTryAllocator<TaoString>::tryAllocate(tstring);
        if (fin) {
          neof = !fin->eof();
          getline(*fin, tstring->chars, '\n');
        } else
          getline(cin, tstring->chars, '\n');
        result = tstring;
      } else {
        number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
        for (size_t i = 0; i < paramAriths.size(); i++) {
          if (fin) {
            neof = !fin->eof();
            number->value = neof;
            if (fin->eof())
              break;
          }

          paramAriths[i]->compute();
          TaoBase *tbase = paramAriths[i]->getResult();
          if (tbase)
            tbase = tbase->dereference();
          if (tbase && tbase->rtti() == TAO_String) {
            TaoString *str = (TaoString *)tbase;
            if (fin)
              getline(*fin, str->chars, '\n');
            else
              getline(cin, str->chars, '\n');
          }
          //	paramAriths[i]->read(fin);
        }
        result = number;
      }
    }
  }
  return 1;
}
void TaoFunCall::execute() {
  if (numCalled == 0)
    gcAgent.clearNewObjects();

  numCalled++;

  result = 0;
  if (ioType >= 0) {
    if (!funCaller || (funCaller && (funCaller->rtti() == TAO_Fin ||
                                     funCaller->rtti() == TAO_Fout))) {
      runIOFile();
      numCalled--;
      return;
    }
  }
  if (isInternal) {
    if (funType < TAO_max)
      runBasicMath();
    else if (funType <= TAO_convolute)
      runNumMath();
    else if (funType <= TAO_numarray)
      runMagicFunct();
    else
      runUtileFunct();

    numCalled--;
    return;
  }

  inArray.clear();
  for (size_t i = 0; i < paramAriths.size(); i++) {
    paramAriths[i]->compute();
    inArray.append(paramAriths[i]->getResult());
  }

  if (myClass) {
    TaoObject *obj = new TaoObject(myClass);
    obj->init(&inArray);
    gcAgent.appendNewObject(obj);
    result = obj;
    numCalled--;
    return;
  } else if (funCaller) {
    short type = funCaller->rtti();
    if (type == TAO_Hash)
      runMethod((TaoHash *)funCaller);
    else if (type == TAO_Array)
      runMethod((TaoArray *)funCaller);
    else if (type == TAO_RealArray || type == TAO_CompArray)
      runMethod((TaoArrayBase *)funCaller);
    //		else if(type==TAO_CompArray)
    //			runMethod ( (TaoCompArray<double>*) funCaller );
    else if (type == TAO_Object)
      runMethod((TaoObject *)funCaller);
    else if (type == TAO_Module)
      runMethod((TaoModule *)funCaller);
    else if (type == TAO_CppObject) {
      TaoCppObject *obj = (TaoCppObject *)funCaller;
      // Run plugin method:
      runMethod(obj->cppObject);
    } else if (type == TAO_AddOn)
      runMethod((TaoAddOn *)funCaller);
    else if (type == TAO_Library)
      runMethod((TaoLibrary *)funCaller, funName);
    else if (type == TAO_NameSpace)
      runMethod((TaoNameSpace *)funCaller);

    numCalled--;
    return;
  } else if (!preRoutine) {
    runMethod(defaultNS);
    return;
  }

  if (!preRoutine) {
    infoKit->error_undefined("subroutine \"" + funName + "\"");
  }
  TaoRoutine *rout = preRoutine->getRoutine();
  rout->run(&inArray);
  blResult = rout->moreToYield();
  result = rout->getResult();

  numCalled--;
}

TaoBase *TaoFunCall::getResult() { return result; }
