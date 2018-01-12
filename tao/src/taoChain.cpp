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
#include "taoChain.h"
#include "taoArith.h"
#include "taoClass.h"
#include "taoDatatype.h"
#include "taoExtdata.h"
#include "taoFuncall.h"
#include "taoNumeric.h"
#include "taoObject.h"
#include "taoRecycler.h"
#include "taoRegex.h"
#include "taoRoutine.h"
#include "taoUtility.h"
#include <iostream>

extern map<int, string> __Tao_TypeToName;

short TaoOperType::RTTI = TAO_OperType;
short TaoOperSizeID::RTTI = TAO_OperSizeID;
short TaoOperDerefer::RTTI = TAO_OperDerefer;
short TaoOperHashKey::RTTI = TAO_OperHashKey;
short TaoOperHashValue::RTTI = TAO_OperHashValue;

short TaoChain::RTTI = TAO_Chain;
short TaoChain::rtti() const { return RTTI; }

int TaoChain::execute() {
  gcAgent.clearNewObjects();

  result = 0;
  short rt1;

  rt1 = myChain[0]->rtti();
  if (rt1 == TAO_FunCall) {
    TaoFunCall *fun = (TaoFunCall *)myChain[0];

    // For functions called in a class method:
    // Relevant code:
    // taoroutine.cpp:    chain->objCaller=hostObject;
    if (objCaller)
      fun->funCaller = objCaller;

    fun->execute();
    blResult = fun->moreToYield();
    // Get returned results:
    result = fun->getResult();
  } else {
    // myChain[0] must be a variable:
    result = myChain[0];
    TaoBase *tbase = result;
    if (tbase)
      tbase = result->dereference();
    if (tbase) {
      rt1 = tbase->rtti();
      // If it's a string, array, hash or numarray, reset its subindex:
      if (rt1 == TAO_Array) {
        TaoArray *v = (TaoArray *)tbase;
        v->resetSubIndex();
      } else if (rt1 == TAO_Hash) {
        TaoHash *v = (TaoHash *)tbase;
        v->resetSubIndex();
      } else if (rt1 == TAO_RealArray) {
        TaoArrayBase *array = (TaoArrayBase *)tbase;
        array->resetSubIndex();
      } else if (rt1 == TAO_CompArray) {
        TaoCompArray<double> *array = (TaoCompArray<double> *)tbase;
        array->resetSubIndex();
      }
    }
  }

  size_t i = 1;
  while (i < myChain.size()) {
    TaoBase *tbase = result;
    if (tbase)
      tbase = result->dereference();
    if (tbase) {
      rt1 = tbase->rtti();
      // If it's a string, array, hash or numarray, reset its subindex:
      if (rt1 == TAO_Array) {
        TaoArray *v = (TaoArray *)tbase;
        v->resetSubIndex();
      } else if (rt1 == TAO_Hash) {
        TaoHash *v = (TaoHash *)tbase;
        v->resetSubIndex();
      } else if (rt1 == TAO_RealArray) {
        TaoArrayBase *array = (TaoArrayBase *)tbase;
        array->resetSubIndex();
      } else if (rt1 == TAO_CompArray) {
        TaoCompArray<double> *array = (TaoCompArray<double> *)tbase;
        array->resetSubIndex();
      }
    } else {
      infoKit->warning("operation on a null object, skipped");
      return 1;
    }
    result = tbase;

    short rt2 = myChain[i]->rtti();
    //! Subindexing:
    if (rt2 == TAO_ArithArray) {
      TaoArithArray *index = (TaoArithArray *)myChain[i];
      index->compute();
      if (rt1 == TAO_String) {
        TaoString *str = (TaoString *)result;
        str->setSubIndex(index->arithArray[0], infoKit);
        result = str;
        if (subIdxArray || i + 1 < myChain.size())
          result = str->subIndexing(infoKit);
      } else if (rt1 == TAO_Array) {
        TaoArray *array = (TaoArray *)result;
        array->setSubIndex(index->arithArray[0], infoKit);
        result = array;
        if (subIdxArray || i + 1 < myChain.size())
          result = array->subIndexing(infoKit);
      } else if (rt1 == TAO_Hash) {
        TaoHash *hash = (TaoHash *)result;
        hash->setSubIndex(index->arithArray[0], infoKit);
        result = hash;
        if (subIdxArray || i + 1 < myChain.size())
          result = hash->subIndexing(infoKit);
      } else if (rt1 == TAO_RealArray) {
        TaoArrayBase *numArray = (TaoArrayBase *)result;
        numArray->setSubIndex(index->arithArray, infoKit);
        result = numArray;
        if (subIdxNumArray || i + 1 < myChain.size())
          result = numArray->subIndexing();
      } else if (rt1 == TAO_CompArray) {
        TaoCompArray<double> *numArray = (TaoCompArray<double> *)result;
        numArray->setSubIndex(index->arithArray, infoKit);
        result = numArray;
        if (subIdxNumArray || i + 1 < myChain.size())
          result = numArray->subIndexing();
      }
      i++;
    } else if (rt2 == TAO_Base) {
      // Function call by object
      short type = myChain[i + 1]->rtti();
      if (type == TAO_FunCall) {
        //	Caller object
        TaoBase *caller = (TaoObject *)result;
        caller = caller->dereference();

        // Function to be called:
        TaoFunCall *fun = (TaoFunCall *)myChain[i + 1];
        fun->funCaller = caller;
        fun->execute();
        blResult = fun->moreToYield();

        result = fun->getResult();
        if (fun->ioType >= 0)
          blResult = fun->neof;

      } else {
        short type = result->rtti();
        TaoString *str = (TaoString *)myChain[i + 1];
        if (type == TAO_Object) {
          // Accessing class data member:
          TaoObject *p = (TaoObject *)result;
          while (p) {
            result = p->getObjectData(str->chars, 3);
            if (result)
              break;
            p = p->getSuperObject();
          }
        } else if (type == TAO_NameSpace) {
          TaoNameSpace *p = (TaoNameSpace *)result;
          result = p->findData(str->chars);
        } else if (type == TAO_Library) {
          TaoLibrary *lib = (TaoLibrary *)result;
          result = lib->findPlugin(str->chars);
        }
      }
      i += 2;
    } else if (rt2 == TAO_OperType) {
      tstring = TaoTryAllocator<TaoString>::tryAllocate(tstring);
      int rtti = getExactType(result);

      stringstream ss;
      ss << __Tao_TypeToName[rtti] << "_" << result;
      tstring->chars = ss.str();
      result = tstring;
      i++;
    } else if (rt2 == TAO_OperSizeID) {
      TaoBase *tbase = result->dereference();
      short rtti = tbase->rtti();
      number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
      if (rtti == TAO_String) {
        TaoString *str = (TaoString *)tbase;
        number->value = str->chars.size();
      } else if (rtti == TAO_Array) {
        TaoArray *array = (TaoArray *)tbase;
        number->value = array->elements.size();
      } else if (rtti == TAO_Hash) {
        TaoHash *hash = (TaoHash *)tbase;
        number->value = hash->elements.size();
      } else if (rtti == TAO_RealArray || rtti == TAO_CompArray) {
        TaoArrayBase *array = (TaoArrayBase *)tbase;
        number->value = array->size();
      }
      result = number;
      i++;
    } else if (rt2 == TAO_OperDerefer) {
      result = result->dereference();
      i++;
    } else if (rt2 == TAO_OperHashKey) {
      if (result->dereference()->rtti() != TAO_Hash)
        infoKit->error_general("no elements can be taken");
      TaoHash *hash = (TaoHash *)result->dereference();
      myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
      myArray->clear();
      result = myArray;
      map<string, TaoBase *>::iterator iter;
      for (iter = hash->elements.begin(); iter != hash->elements.end(); iter++)
        myArray->append(new TaoString(iter->first));
      i++;
    } else if (rt2 == TAO_OperHashValue) {
      if (result->dereference()->rtti() != TAO_Hash)
        infoKit->error_general("no values can be taken");
      TaoHash *hash = (TaoHash *)result->dereference();
      myArray = TaoTryAllocator<TaoArray>::tryAllocate(myArray);
      myArray->clear();
      result = myArray;
      map<string, TaoBase *>::iterator iter;
      for (iter = hash->elements.begin(); iter != hash->elements.end(); iter++)
        myArray->append(iter->second);
      i++;
    } else if (rt2 == TAO_RgxMatch) {
      if (result->dereference()->rtti() != TAO_String)
        infoKit->error_general("operon is not a string");
      TaoRgxMatch *rgx = (TaoRgxMatch *)myChain[i];
      TaoString *str = (TaoString *)result->dereference();
      rgx->execute(str);
      blResult = rgx->rgxBool;
      result = rgx->strResult;
      i++;
    } else {
      i++;
    }
  }
  return 1;
}
