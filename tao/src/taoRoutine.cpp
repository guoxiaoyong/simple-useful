
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

#include <algorithm>
#include <list>

#include "taoArith.h"
#include "taoChain.h"
#include "taoConst.h"
#include "taoExtdata.h"
#include "taoObject.h"
#include "taoRecycler.h"
#include "taoRoutine.h"

short TaoRoutine::RTTI = TAO_Routine;
short TaoRoutine::rtti() const { return RTTI; }

TaoRoutine::TaoRoutine() {
  detach = 1;
  result = 0;
  outArray = 0;
  idResume = 0;
  hostObject = 0;
  numCalled = 0;
  paramPassed = 0;

  number = 0;
  tstring = 0;
  complx = 0;
}
TaoRoutine::~TaoRoutine() {
  map<string, TaoReference *>::iterator iter;
  for (iter = myData.begin(); iter != myData.end(); ++iter) {
    TaoBase *pbase = iter->second->content;
    if (pbase) {
      //	pbase->refCount--;
      //	__Tao_Recycler.pushRootCandidate(pbase);
    }
  }
}
void TaoRoutine::print(ostream *out) { *out << "routine " << name << "()"; }
void TaoRoutine::detachOldParams() {
  map<string, TaoReference *>::iterator iter;
  for (iter = myData.begin(); iter != myData.end(); ++iter) {
    if (iter->second->rttiRefer() == TAO_ParamRefer) {
      if (TaoBase::recycler)
        TaoBase::recycler->pushRootCandidate(iter->second->content);
      iter->second->content = 0;
    }
  }
}
void TaoRoutine::passParam(int i, TaoBase *p) {
  paramPassed = 1;

  if (i >= (int)paramNames.size())
    return;

  TaoReference *ref = myData[paramNames[i]];
  /*
  short rtti=p->rtti();
  if( rtti==TAO_Reference ){
          //////////////////////////////////////////////
          // sub fun1( a ){ fun2(a); }                //
          //      a: param,  ~~~~~~~                  //
          // class klass{ a; sub meth(){ fun1(a); } } //
          //      a: class member,       ~~~~~~~~     //
          //////////////////////////////////////////////
          // The content of a TaoParamRefer or TaoDataRefer object
          // instead of itself is passed to the TaoRoutine object:
          TaoReference*reftmp=(TaoReference*)p;
          short rttiref=reftmp->rttiRefer();
          if( rttiref==TAO_ParamRefer || rttiref==TAO_DataRefer ){
                  ref->content=reftmp->content;
                  ref->content->refCount++;
                  return;
          }
  }
  */
  ref->content = p;
  p->refCount++;
}
void TaoRoutine::mapObjectData() {
  if (!hostObject)
    return;
  map<string, TaoReference *>::iterator iter;
  for (iter = myData.begin(); iter != myData.end(); ++iter) {
    if (iter->second->rttiRefer() == TAO_DataRefer) {
      iter->second->content = hostObject->searchObjectData(iter->first);
    }
  }
}
void TaoRoutine::duplicateByData(vector<TaoBase *> &data) {
  data.clear();
  map<string, TaoReference *>::iterator iter;
  for (iter = myData.begin(); iter != myData.end(); ++iter) {
    // Only TaoReference object need to be duplicated,
    // since the parameters are passed and TaoObject
    // data are mapped before run() and this function
    // are executed.
    // So TaoParamRefer and TaoDataRefer object along with
    // their contents are saved in routine stack(TaoRoutStack)
    // where functions are called recursively.
    if (iter->second->rtti() == TAO_Reference) {
      data.push_back(0);
    } else {
      data.push_back(iter->second->content);
    }
  }
}

void TaoRoutine::setupStatus() {
  int i = 0;
  map<string, TaoReference *>::iterator iter;
  for (iter = myData.begin(); iter != myData.end(); ++iter) {
    nameToIndex[iter->first] = i;
    i++;
    if (iter->second->rtti() == TAO_Reference)
      isLocalVar.push_back(1);
    else
      isLocalVar.push_back(0);
  }
}
bool TaoRoutine::run(TaoArray *in) {
  if (!paramPassed) {
    // For class constructor, because their parameters
    // is passed in a different way:
    if (in) {
      for (size_t i = 0; i < in->elements.size(); i++)
        passParam(i, in->elements[i]);
      paramPassed = 0;
    }
  }

  if (numCalled > 0) {
    dataStack.push(vector<TaoBase *>());
    // Modify the last element directly:
    duplicateByData(dataStack.top());
  }

  if (hostObject) {
    vector<TaoPhrase *>::iterator fit;
    for (fit = myPhrases.begin(); fit != myPhrases.end(); ++fit) {
      if ((*fit)->rtti() == TAO_Chain) {
        TaoChain *chain = (TaoChain *)(*fit);
        if (chain->isClassRout)
          chain->objCaller = hostObject;
      }
    }
  }

  numCalled++;
  int old = -1;
  int next = idResume;
  idResume = 0; // In the case of recursive call.
  while (next >= 0 && next < (int)myPhrases.size()) {
    old = next;
    next = myPhrases[old]->execute();
    if (next < 0)
      return 0;
    //! Try to invoke GC:
    if (TaoBase::recycler)
      TaoBase::recycler->tryInvoke();
  }
  //! Try to invoke GC:
  if (TaoBase::recycler)
    TaoBase::recycler->tryInvoke();

  short rtti = -1;
  if (old >= 0 && old < (int)myPhrases.size())
    rtti = myPhrases[old]->rtti();

  result = 0;
  if (rtti == TAO_Return || rtti == TAO_Yield) {
    TaoReturn *retn = (TaoReturn *)myPhrases[old];
    if (retn->returnAriths.size() > 1) {
      outArray = TaoTryAllocator<TaoArray>::tryAllocate(outArray);
      outArray->clear();
      for (size_t i = 0; i < retn->returnAriths.size(); i++) {
        TaoBase *tbase = retn->returnAriths[i]->getResult();
        if (!tbase) {
          infoKit->warning("null object is ignored");
          return 1;
        }
        short rtd = tbase->rtti();
        short rta = retn->returnAriths[i]->rtti();
        if (rta == TAO_ArithVariable) {
          if (rtd == TAO_Number || rtd == TAO_String || rtd == TAO_Complex)
            tbase = tbase->duplicate();
        }
        outArray->append(tbase);
      }
      result = outArray;
    } else if (retn->returnAriths.size() > 0) {
      TaoBase *tbase = retn->returnAriths[0]->getResult();
      if (!tbase) {
        infoKit->warning("null object is ignored");
        return 1;
      }
      result = tbase;
      short rtd = tbase->rtti();
      short rta = retn->returnAriths[0]->rtti();
      // routine test(){ i=0; return i; }
      if (rta == TAO_ArithVariable) {
        if (rtd == TAO_Number) {
          TaoNumber *num = (TaoNumber *)tbase;
          number = TaoTryAllocator<TaoNumber>::tryAllocate(number);
          number->value = num->value;
          result = number;
        } else if (rtd == TAO_String) {
          TaoString *str = (TaoString *)tbase;
          tstring = TaoTryAllocator<TaoString>::tryAllocate(tstring);
          tstring->chars = str->chars;
          result = tstring;
        } else if (rtd == TAO_Complex) {
          TaoComplex *com = (TaoComplex *)tbase;
          complx = TaoTryAllocator<TaoComplex>::tryAllocate(complx);
          com->cmplx = com->cmplx;
          result = complx;
        }
      }
    }
  }

  idResume = old + 1;
  if (idResume >= (int)myPhrases.size())
    idResume = 0;

  if (rtti != TAO_Yield) {
    // If this routine is not returned by "yield" statement,
    // reset its state:
    idResume = 0;

    map<string, TaoReference *>::iterator iter;
    for (iter = myData.begin(); iter != myData.end(); ++iter) {
      TaoBase *pbase = iter->second->content;
      if (pbase) {
        short rt = pbase->rtti();
        if (rt == TAO_Fin) {
          TaoFin *tfin = (TaoFin *)pbase;
          tfin->fin->close();
        } else if (rt == TAO_Fout) {
          TaoFout *tfout = (TaoFout *)pbase;
          tfout->fout->close();
        }
      }
    }

    numCalled--;
    if (numCalled > 0) {
      vector<TaoBase *> &data = dataStack.top();
      for (iter = myData.begin(); iter != myData.end(); ++iter)
        iter->second->content = data[nameToIndex[iter->first]];
      dataStack.pop();
    }
  }
  // For apply(numarray, routine):
  if (detach)
    detachOldParams();
  return 1;
}
TaoBase *TaoRoutine::getResult() { return result; }
