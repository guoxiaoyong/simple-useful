
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
#ifndef TAO_PHRASE_H
#define TAO_PHRASE_H

#include "stdio.h"
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "taoDatatype.h"
#include "taoNumeric.h"

class TaoArithBase;
class TaoChain;
class TaoInfoKit;

using namespace std;
enum TaoPhraseRTTI {
  TAO_Phrase = 0,
  TAO_LBrace = 1,
  TAO_RBrace = 2,
  TAO_Break = 3,
  TAO_Skip = 4,

  TAO_Control = 11,
  TAO_ForEach = 12,
  TAO_While = 13,
  TAO_If = 14,
  TAO_ElseIf = 15,
  TAO_Else = 16,
  TAO_For = 17,

  TAO_Command = 21,
  TAO_Assign = 22,
  TAO_ArithPhrase = 27,

  TAO_Return = 55,
  TAO_Yield = 56,
};

//!	TaoPhrase is the base class for all Tao phrase.
/*
        Each Tao phrase contains pointers pointed to memory allocated
        by TaoModule, such that there is no need for resoving variable
        names in running time. It is the same case in class TaoNumArith.

        After execution of TaoPhrase::execute(bool*),
        it returns the ID of phrase
        which will be executed next. Normal phrases return the ID of their
        next phrase in vector<TaoPhrase*>TaoModule::myPhrases; logical control
        phrases the phrase ID, depending the value of Cotrol::condition.
*/
class TaoPhrase {
public:
  TaoPhrase() { stepTo = jumpTo = -1; };
  virtual ~TaoPhrase(){};

  int stepTo;
  int jumpTo;

  TaoInfoKit *infoKit;

  //	return the next phrase.
  virtual int execute() = 0;

  static short RTTI;
  virtual short rtti() const;
};
//! no comments!
class LBrace : public TaoPhrase {
public:
  LBrace(){};

  static short RTTI;
  short rtti() const;

  int execute() { return stepTo; }
};
//! no comments!
class RBrace : public TaoPhrase {
public:
  RBrace(){};

  static short RTTI;
  short rtti() const;

  int execute() { return jumpTo; }
};

class TaoBreak : public TaoPhrase {
public:
  TaoBreak(){};

  static short RTTI;
  short rtti() const;
  int execute() { return jumpTo; }
};
class TaoSkip : public TaoPhrase {
public:
  TaoSkip(){};

  static short RTTI;
  short rtti() const;
  int execute() { return jumpTo; }
};

//////////////////////////////////

//! no comments!
class TaoControl : public TaoPhrase {
public:
  TaoControl() { condition = 0; }
  virtual ~TaoControl(){};

  static short RTTI;
  virtual short rtti() const;

  TaoArithBase *condition;

  virtual int execute();
};
//	For:
//	WHILE(e)
//	IF(e)...ELSE IF(e)
//	if e=TRUE, execute the codes following that statement,
//	otherwise jump to appropriate place.
//! no comments!
class TaoForEach : public TaoControl {
public:
  TaoForEach() { index = 0; };

  string elemName;

  int index;

  TaoReference *refElement;
  TaoChain *chainArray;

  static short RTTI;
  short rtti() const;
  int execute();
};
//! no comments!
class TaoWhile : public TaoControl {
public:
  TaoWhile(){};

  static short RTTI;
  short rtti() const;

  int execute();
};
class TaoFor : public TaoControl {
  bool firstRound;

public:
  TaoFor() { firstRound = 1; }

  TaoArithBase *first;
  TaoArithBase *step;

  static short RTTI;
  short rtti() const;

  int execute();
};
//! no comments!
class TaoIf : public TaoControl {
public:
  TaoIf(){};

  static short RTTI;
  short rtti() const;

  int execute();
};
//! no comments!
class TaoElseIf : public TaoControl {
public:
  TaoElseIf(){};

  static short RTTI;
  short rtti() const;

  int execute();
};
//! no comments!
class TaoElse : public TaoControl {
public:
  TaoElse(){};

  static short RTTI;
  short rtti() const;

  int execute();
};

//! no comments!
class TaoCommand : public TaoPhrase {
public:
  TaoCommand(){};
  virtual ~TaoCommand(){};

  static short RTTI;
  short rtti() const;

  int execute() = 0;
};

//! no comments!
class TaoArithPhrase : public TaoCommand {
public:
  TaoArithPhrase() {}

  TaoArithBase *myArith;

  static short RTTI;
  short rtti() const;
  int execute();
};

//	Completely simplified to:
//	leftChain = rightArith;
//! no comments!
class TaoAssign : public TaoPhrase {
public:
  TaoAssign();

  // For assigning to a constant,
  // if TRUE, assignment is not allowed:
  bool keepConst;

  // Dimensions: a=[dim1][dim2][] : ... for normal array:
  // Or: a=(dim1)(dim2)() : ... for numeric array:
  vector<TaoArithBase *> dimAriths;

  // If TRUE, the dimension specifier is for numeric array:
  short typeNumeric;

  // Left chain array for multi-assignment,
  // such as: [a,array[1],...]=fun();
  vector<TaoChain *> lchainArray;

  TaoReference *refPrevious;

  // For normal assignment:
  TaoChain *leftChain;
  TaoArithBase *rightArith;

  TaoArray *multiArray;

  TaoByteArray *byteArray;
  TaoShortArray *shortArray;
  TaoIntArray *intArray;
  TaoFloatArray *floatArray;
  TaoDoubleArray *doubleArray;

  TaoCompArray<double> *comArray;

  static short RTTI;
  short rtti() const;
  int execute();

  void compChainAriths();

  void assign();

  static void assign(TaoBase *lbase, TaoBase *rbase, TaoInfoKit *infoKit,
                     bool keepcnst = 0);

  void multiAssign(TaoBase *pbase);
  void make_mdim_array(TaoArray *array, int i);

  virtual TaoBase *make_element();
  virtual TaoBase *make_numeric();
};

//! no comments!
class TaoReturn : public TaoCommand {
public:
  TaoReturn() {}

  static short RTTI;
  virtual short rtti() const;

  vector<TaoArithBase *> returnAriths;

  int execute();
};

class TaoYield : public TaoReturn {
public:
  static short RTTI;
  short rtti() const { return RTTI; }
};

#endif
