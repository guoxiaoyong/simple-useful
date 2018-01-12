
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
#ifndef TAO_MODULE_H
#define TAO_MODULE_H

#include "stdio.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "taoPhrase.h"

using namespace std;

class TaoClass;
class TaoRoutine;
class TaoModule;
class TaoArithBase;
class TaoPattern;
class TaoFunCall;
class TaoRegex;
class TaoEnumMatrix;

class TaoNameSpace;

class TaoModuleBase : public TaoBase {
protected:
  TaoNameSpace *inNameSpace;
  TaoNameSpace *outNameSpace;

public:
  virtual short rtti() const { return -1; }

  bool success;
  // Source file name.
  string srcFName;

  void setNameSpace(TaoNameSpace *nsIn, TaoNameSpace *nsOut = 0) {
    inNameSpace = nsIn;
    if (nsOut)
      outNameSpace = nsOut;
    else
      outNameSpace = nsIn;
  }
};

class TaoKeyModWord {
public:
  TaoKeyModWord();

  vector<string> arithOpers;
  vector<char> arithTypes;

  map<string, char> rightUnary, leftUnary;

  map<string, int> funcIntern;
  map<string, int> methIntern;

  map<string, short> numTypeKey;
};

//!	TaoModule is a class to compile Tao subroutines.
/*!
        TaoModule is one of the two classes (the other is TaoClass) for
        parsing and compiling Tao source code into a kind of automa.
        The unit of such automa is an object of class TaoPhrase,
        which represents a meaningful block of the source code,
        and contains the data referecences and instructions to
        accomplish the computations.

        All the functions started with "make" are used for making automa
        units, namely objects of class TaoPhrase.
*/
class TaoModule : public TaoModuleBase {
  stack<int> regexScope;

  int tranUsed;
  map<string, TaoReference *> varTransient;

  int scopeLevel;
  vector<map<string, TaoReference *>> varLocal;

  TaoReference *getVariable(string &name);

  TaoInfoKit *infoKit;

  short typeNumeric;

protected:
  TaoClass *superClass;

  //!	Phrase to be executed.
  vector<TaoPhrase *> myPhrases;

  /*!
          TaoModule::myData contains the data used in this module.
  */
  map<string, TaoReference *> myData;

  //! If TRUE, export variables to outNameSpace.
  //! Only the variables of a main routine is exported.
  bool varToNameSpace;

  //! Routine "prototypes", which are accessible from this TaoModule,
  //! includes non-class-method routines, public and protected
  //! methods of the parent classes and private methods of its
  //! own class.
  map<string, TaoModule *> preRoutine;

  //! Used for class methods.
  //! In class methods, if a class method "func" of its own or parent
  //! class is called, classRoutNames["func"]=TRUE, otherwise, FALSE.
  map<string, bool> classRoutNames;

  vector<string> paramNames;

  TaoRoutine *routine;

  TaoReference *makeVarTransient(int i = -1);
  TaoReference *makeVarTransient(const string &name);

public:
  TaoModule();
  virtual ~TaoModule(){};

  TaoModule *super;
  string name;
  bool parsed;
  short varType;

  static TaoKeyModWord modKey;

  void setVarToNameSpace(bool bl) { varToNameSpace = bl; }

  void setSuperClass(TaoClass *super) { superClass = super; }
  TaoClass *getSuperClass() { return superClass; }

  //! Get class data from its super class:
  TaoReference *getClassData(string &dname);
  //! Get class method from its super class:
  TaoModule *getClassMethod(string &dname);

  TaoRoutine *getRoutine();

  vector<string> myWords;
  vector<int> posLine;

  bool routMade;

  void appendWord(string &word, int pos = 0) {
    myWords.push_back(word);
    posLine.push_back(pos);
  }

  void appendParams(string &param) { paramNames.push_back(param); }
  void parseParams();
  TaoRoutine *makeRoutine();

  void declareVariable(string &name, int pos);

  bool parsePhrase();

  //! Setting up logic and loop control relationships.
  bool setupLogicLoop();

  int makeLogicLoop(TaoControl *contl, int start);
  int makeForEach(TaoForEach *foreach, int start);
  bool makeFunCall(TaoFunCall *fun, int left, int right);

  bool makeRegex(TaoRegex *pattern, int start, int end);

  bool makeEnumMatrix(TaoEnumMatrix *matx, int start, int end);

  bool makeChain(TaoChain *chain, int left, int right);
  bool makeChainArray(vector<TaoChain *> &chains, int left, int right);
  bool makeArithTree(TaoArithBase *node, int start, int end);
  bool makeArithArray(vector<TaoArithBase *> &ariths, int left, int right,
                      const string sep = ",");

  int findRootOper(int start, int end, char *optype);
  TaoArithBase *makeArithLeaf(int pos);
  TaoArithBase *makeArithNode(int start, int end);
  TaoArithBase *makeArithTree(int start, int end);

  int phrasePair(int left);

  void print(ostream *out = &cout);
  static short RTTI;
  virtual short rtti() const;
};

#endif
