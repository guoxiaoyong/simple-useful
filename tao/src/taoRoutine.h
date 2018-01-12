
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
#ifndef TAO_ROUTINE_H
#define TAO_ROUTINE_H

#include <stack>

#include "stdio.h"

//#include"yn_thread.h"
#include "taoModule.h"

class TaoObject;

//! Class for Tao routines.
class TaoRoutine : public TaoBase
#ifdef TAO_THREAD_H
                   ,
                   public TaoThread
#endif
{
protected:
  friend class TaoModule;

  TaoObject *hostObject;

  TaoInfoKit *infoKit;

  //! Phrases of this routine.
  vector<TaoPhrase *> myPhrases;
  //! Data of this routine.
  map<string, TaoReference *> myData;
  //! Name of parameter variables.
  vector<string> paramNames;

  //! Used in recursive routine call.
  //! Number of copies of this routine in running.
  //! Whenever this routine is running and itself is called,
  //! it is increased by one.
  int numCalled;

  //! Used in recursive routine call.
  //! The copied data of this routine is stored in "dataStack"
  //! as vectors, "nameToIndex" is used to access them by names.
  map<string, int> nameToIndex;

  //! Used in recursive routine call.
  //! When a routine is executed completely,
  //! its corresponding copied data is popped and data corresponding
  //! to local variables are taged for garbage collection.
  vector<bool> isLocalVar;

  stack<vector<TaoBase *>> dataStack;

  bool paramPassed;

  int idResume;

  bool detach;

  TaoNumber *number;
  TaoString *tstring;
  TaoComplex *complx;

public:
  TaoRoutine();
  ~TaoRoutine();

  //! Detach references to old parameters.
  void detachOldParams();
  //! Passing "pbase" as the i-th parameter.
  void passParam(int i, TaoBase *pbase);

  //! Initialize some parameters for storing and
  //! accessing data in "dataStack".
  void setupStatus();

  void setParamPassed(bool pass) { paramPassed = pass; }
  void setDetachParam(bool det) { detach = det; }

  TaoArray *outArray;
  TaoBase *result;

  string name;
  TaoModule *routProto;

  void setHostObject(TaoObject *obj) { hostObject = obj; }

  //! For class methods.
  //! Map the local pointers of a class method to
  //! to the data of an object.
  void mapObjectData();

  int numParams() { return paramNames.size(); }

  //! Run routine.
  bool run(TaoArray *in = 0);

  TaoBase *getResult();

  bool moreToYield() {
    if (idResume > 0)
      return 1;
    return 0;
  }

  //! Duplicate the local variable of a routine
  //! and store the duplication in "data".
  void duplicateByData(vector<TaoBase *> &data);

  static short RTTI;
  virtual short rtti() const;
  void print(ostream *out = &cout);
};

#endif
