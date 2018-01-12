
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
#ifndef TAO_CHAIN_H
#define TAO_CHAIN_H

#include "stdio.h"
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "taoDatatype.h"
#include "taoRecycler.h"

class TaoArithBase;

class TaoRegex;
class TaoComplex;
class TaoInfoKit;

using namespace std;

enum TaoRightOperType {
  TAO_OperType = 101,
  TAO_OperSizeID = 102,
  TAO_OperDerefer = 103,
  TAO_OperHashKey = 104,
  TAO_OperHashValue = 105,
};

class TaoOperType : public TaoBase {
public:
  static short RTTI;
  short rtti() const { return RTTI; }
};
class TaoOperSizeID : public TaoBase {
public:
  static short RTTI;
  short rtti() const { return RTTI; }
};
class TaoOperDerefer : public TaoBase {
public:
  static short RTTI;
  short rtti() const { return RTTI; }
};
class TaoOperHashKey : public TaoBase {
public:
  static short RTTI;
  short rtti() const { return RTTI; }
};
class TaoOperHashValue : public TaoBase {
public:
  static short RTTI;
  short rtti() const { return RTTI; }
};

// obj->hash[i]...
//	hash[i]->fun1()->...
// fun(a,b)[i]->
//	objects  [i]      ->        fun2  (a,b) -> memb1[j]...
// first : subindex : pointer : name : hash
// Tao types:
// first: arith   : base   : string : of arith
class TaoObject;

//! no comments!
class TaoChain : public TaoBase {
  TaoGcAgent gcAgent;

public:
  TaoChain() {
    isClassRout = 0;
    objCaller = 0;
    result = 0;
    myArray = 0;
    number = 0;
    tstring = 0;
    subIdxArray = 1;
    subIdxNumArray = 1;
  }
  ~TaoChain(){};

  string firstName;

  TaoInfoKit *infoKit;

  bool isClassRout;
  TaoObject *objCaller;

  vector<TaoBase *> myChain;

  TaoArray *myArray;

  TaoBase *result;

  TaoNumber *number;
  TaoString *tstring;

  bool blResult;

  bool subIdxNumArray;
  bool subIdxArray;

  TaoArithBase *subIndex;

  static short RTTI;
  short rtti() const;
  int execute();

  TaoBase *getResult() { return result; }
};

#endif
