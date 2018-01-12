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

#ifndef TAO_RECYCLER_H
#define TAO_RECYCLER_H

#include "taoDatatype.h"

/*!
        It is only capable to collect objects which are instances
        of the subclasses of TaoBase.

        TaoRecycler is capable to collect cyclic referenced objects.
*/
class TaoRecycler {
  //! Candidate objects, whose reference counts have been decreased,
  //! are taged as candidate garbage:
  vector<TaoBase *> rootCandidates;

  //! Number of objects with 0 reference count:
  int nObjWithZeroRC;

  //! Number of objects created by "new" operator
  //! since the last garbage recycling:
  int nObjNewed;

  vector<TaoBase *> objLocked;

  vector<TcBase *> tcObjects;

  //! Temporary variable for marking alive objects:
  vector<TaoBase *> objAlive;
  void pushAliveObject(TaoBase *tbase);

public:
  TaoRecycler() {
    nObjWithZeroRC = nObjNewed = 0;
    gcTimes = 0;
  }

  //! Number of times when GC was actually invoked:
  int gcTimes;

  //! Count new objects created since last GC.
  void objectNewed() { nObjNewed++; }

  //! Store objects with 0 reference count.
  void pushRootZeroRc(TaoBase *pbase);
  //! Store candidate object.
  void pushRootCandidate(TaoBase *pbase);

  void pushTcObject(TcBase *tbase);
  void cleanTcObjects();

  //! Try to do GC.
  //! Check some criteria, if one of them is TRUE, do GC.
  void tryInvoke();

  void recycleGarbage();
  //! Scan for all candidate garbage from "rootCandidates",
  //! and decrease their reference counts. After this scan,
  //! objects with ZERO reference counts are dead objects,
  //! while others are still alive.
  //! This can detect dead cyclically-referenced objects.
  void scanForCandidates();
  //! Mark all live objects which are reachable from any
  //! "rootCandidates" which has NON-ZERO reference count.
  void markLiveObjects(TaoBase *root);
};

class TaoGcAgent {
  vector<TaoBase *> newObjects;

public:
  void appendNewObject(TaoBase *tbase) {
    tbase->refCount++;
    newObjects.push_back(tbase);
  }
  void clearNewObjects() {
    for (size_t i = 0; i < newObjects.size(); i++) {
      TaoBase *tbase = newObjects[i];
      tbase->refCount--;
      if (tbase->refCount == 0 && TaoBase::recycler) {
        TaoBase::recycler->pushRootZeroRc(tbase);
      }
    }
    newObjects.clear();
  }
};

#endif
