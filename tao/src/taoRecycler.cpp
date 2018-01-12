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

#include "taoRecycler.h"
#include "taoConst.h"
#include "taoNumeric.h"
#include "taoObject.h"

using namespace std;

void TaoRecycler::pushRootZeroRc(TaoBase *pbase) {
  if (pbase->scanned)
    return;

  pbase->scanned = 1;
  rootCandidates.push_back(pbase);

  nObjWithZeroRC++;
}
void TaoRecycler::pushRootCandidate(TaoBase *pbase) {
  if (!pbase)
    return;
  pbase->refCount--;
  if (pbase->scanned)
    return;

  pbase->scanned = 1;
  rootCandidates.push_back(pbase);

  if (pbase->refCount == 0)
    nObjWithZeroRC++;
}
void TaoRecycler::pushAliveObject(TaoBase *tbase) {
  if (tbase) {
    tbase->refCount++;
    if (!tbase->scanned) {
      tbase->scanned = 1;
      objAlive.push_back(tbase);
    }
  }
}
void TaoRecycler::pushTcObject(TcBase *tbase) {
  if (!tbase->visited)
    tcObjects.push_back(tbase);
}
void TaoRecycler::cleanTcObjects() {
  size_t i;
  for (i = 0; i < tcObjects.size(); i++)
    tcObjects[i]->visited = 0;

  vector<TcBase *> tcTemp;
  for (i = 0; i < tcObjects.size(); i++) {
    if (!tcObjects[i]->visited) {
      tcTemp.push_back(tcObjects[i]);
      tcObjects[i]->visited = 1;
    }
  }
  for (i = 0; i < tcTemp.size(); i++)
    delete tcTemp[i];
  tcObjects.clear();
}
void TaoRecycler::tryInvoke() {
  if (rootCandidates.size() > 1000 || nObjWithZeroRC > 10 ||
      nObjNewed > 1000000)
    recycleGarbage();
}
void TaoRecycler::recycleGarbage() {
  //	cout<<"TaoRecycler::recycleGarbage()>>\n";
  size_t i;

  gcTimes++;
  /*
          for(i=0;i<objLocked.size();i++){
                  if( !objLocked[i]->scanned && objLocked[i]->refCount==0 ){
                          rootCandidates.push_back( objLocked[i] );
                          objLocked[i]->scanned=1;
                  }
          }
  */
  scanForCandidates();

  size_t size = rootCandidates.size();
  for (i = 0; i < size; i++) {
    rootCandidates[i]->scanned = 0;
  }

  for (i = 0; i < size; i++) {
    if (rootCandidates[i]->refCount > 0 && !rootCandidates[i]->scanned)
      markLiveObjects(rootCandidates[i]);
  }

  //	objLocked.clear();

  // TaoBase *base=0;
  for (i = 0; i < size; i++) {
    rootCandidates[i]->scanned = 0;
    if (rootCandidates[i]->refCount == 0) {
      //	if( !rootCandidates[i]->locked ){
      delete rootCandidates[i];
      // If it is "locked", this means it's still refered by
      // certain object without giving a reference count,
      // for efficiency purpose.
      //	}
    } else {
      /*
      int rtti=rootCandidates[i]->rtti();
      if( rtti==TAO_RealArray || rtti==TAO_CompArray ){
              // See comments in TaoArrayBase
              TaoArrayBase *narray=(TaoArrayBase*)rootCandidates[i];
              if( abs( gcTimes - narray->timeTag )>1000000 ){
                      narray->resize(0);
                      narray->timeTag=gcTimes;
              }
      }
      */
    }
  }
  //	cout<<objLocked.size()<<" locked objects.\n";

  rootCandidates.clear();
  nObjWithZeroRC = 0;
  nObjNewed = 0;
  //	cout<<"TaoRecycler::recycleGarbage()<<\n";
}
void TaoRecycler::scanForCandidates() {
  //	cout<<"TaoRecycler::scanForCandidates>>\n";
  size_t i;

  vector<TaoBase *>::iterator iter;
  for (i = 0; i < rootCandidates.size(); i++) {
    int type = rootCandidates[i]->rtti();
    if (type == TAO_Reference) {
      TaoReference *ref = (TaoReference *)rootCandidates[i];
      pushRootCandidate(ref->content);
    } else if (type == TAO_Array) {
      TaoArray *array = (TaoArray *)rootCandidates[i];
      pushRootCandidate(array->subArray);
      for (size_t j = 0; j < array->elements.size(); j++)
        pushRootCandidate(array->elements[j]);
    } else if (type == TAO_Hash) {
      TaoHash *hash = (TaoHash *)rootCandidates[i];
      pushRootCandidate(hash->subHash);
      map<string, TaoBase *>::iterator iter;
      for (iter = hash->elements.begin(); iter != hash->elements.end(); iter++)
        pushRootCandidate(iter->second);
    } else if (type == TAO_Object) {
      TaoObject *obj = (TaoObject *)rootCandidates[i];
      obj->scan_member_data(rootCandidates, -1);
    }
  }
  //	cout<<"TaoRecycler::scanForCandidates<<\n";
  //	cout<<"size\t"<<rootCandidates.size()<<endl;
}
void TaoRecycler::markLiveObjects(TaoBase *root) {
  //	cout<<"TaoRecycler::markLiveObjects>>1\t"<<objAlive.size()<<"\n";

  root->scanned = 1;
  objAlive.clear();
  objAlive.push_back(root);

  for (size_t i = 0; i < objAlive.size(); i++) {
    int type = objAlive[i]->rtti();
    if (type == TAO_Reference) {
      TaoReference *ref = (TaoReference *)objAlive[i];
      pushAliveObject(ref->content);
    } else if (type == TAO_Array) {
      TaoArray *array = (TaoArray *)objAlive[i];
      pushAliveObject(array->subArray);
      for (size_t j = 0; j < array->elements.size(); j++)
        pushAliveObject(array->elements[j]);
    } else if (type == TAO_Hash) {
      TaoHash *hash = (TaoHash *)objAlive[i];
      pushAliveObject(hash->subHash);
      map<string, TaoBase *>::iterator iter;
      for (iter = hash->elements.begin(); iter != hash->elements.end(); iter++)
        pushAliveObject(iter->second);
    } else if (type == TAO_Object) {
      TaoObject *obj = (TaoObject *)objAlive[i];
      obj->scan_member_data(objAlive, 1);
    }
  }
  //	cout<<"TaoRecycler::markLiveObjects<<\n";
}
