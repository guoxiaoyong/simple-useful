
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

#include <algorithm>
#include <sstream>
#include <stdio.h>

#include "taoArith.h"
#include "taoChain.h"
#include "taoClass.h"
#include "taoConst.h"
#include "taoDatatype.h"
#include "taoEnumer.h"
#include "taoExtdata.h"
#include "taoFuncall.h"
#include "taoModule.h"
#include "taoObject.h"
#include "taoPhrase.h"
#include "taoRegex.h"
#include "taoScript.h"
#include "taoUtility.h"

#include "taoLoadlib.h"
#include "taoPlugin.h"

short TaoModule::RTTI = TAO_Module;
short TaoModule::rtti() const { return RTTI; }

TaoKeyModWord TaoModule::modKey;

TaoKeyModWord::TaoKeyModWord() {
  rightUnary["++"] = TAO_PPLUS;
  rightUnary["--"] = TAO_MMINUS;
  rightUnary["'"] = TAO_TRANSPOSE;
  rightUnary["~"] = TAO_INVERSE;
  rightUnary["$"] = TAO_IMAGINARY;

  leftUnary["++"] = TAO_PPLUS;
  leftUnary["--"] = TAO_MMINUS;
  leftUnary["!"] = TAO_NOT;
  leftUnary["+"] = TAO_PLUS;
  leftUnary["-"] = TAO_MINUS;
  leftUnary["$"] = TAO_IMAGINARY;

  const char *ariopers[] = {"=~", "!~", "~~",  "?",   ":",  ":=", "+=", "-=",
                            "*=", "/=", "%=",  "^=",  "&=", "|=", "&&", "||",
                            "<",  ">",  "==",  "!=",  "<=", ">=", "+",  "-",
                            "*",  "/",  "<*>", "</>", "%",  "^",  "=?", "!?"};

  int i;

  for (i = 0; i < 32; i++) {
    arithOpers.push_back(ariopers[i]);
    arithTypes.push_back(i);
  }
  arithOpers.insert(arithOpers.begin() + 5, "=");
  arithTypes.insert(arithTypes.begin() + 5, 5);

  const char *math[] = {"abs",  "acos", "asin", "atan", "cos",    "cosh",
                        "exp",  "log",  "sin",  "sinh", "sqrt",   "tan",
                        "tanh", "arg",  "norm", "rand", "randgs", "srand"};
  for (i = 0; i < 18; i++)
    funcIntern[math[i]] = i + 1;

  const char *matharr[] = {"max",     "min",        "mean",     "stdev",
                           "varn",    "sum",        "prod",     "count",
                           "permute", "invpermute", "convolute"};
  for (i = 0; i < 11; i++)
    funcIntern[matharr[i]] = i + 20;

  const char *magic[] = {"apply",  "noapply", "iterate", "iterget",
                         "repeat", "sort",    "which",   "numarray"};
  for (i = 0; i < 8; i++)
    funcIntern[magic[i]] = i + 40;

  const char *util[] = {"about",  "compile", "eval",   "import", "load",
                        "system", "type",    "number", "pack",   "unpack",
                        "time",   "astime",  "asctime"};

  for (i = 0; i < 13; i++)
    funcIntern[util[i]] = i + 60;

  const char *inmeth[] = {"erase",   "flat",   "insert", "keys",
                          "reshape", "resize", "size",   "values"};
  for (int k = 0; k < 8; k++)
    methIntern[inmeth[k]] = k + 80;

  const char *ntype[] = {"byte", "short", "int", "float", "double"};
  for (int m = 0; m < 5; m++)
    numTypeKey[ntype[m]] = m + 1;
}

TaoModule::TaoModule() {
  varToNameSpace = 0;
  routine = 0;
  routMade = 0;
  superClass = 0;
  parsed = 0;
  infoKit = 0;

  typeNumeric = TAO_DoubleArray;
}

void TaoModule::print(ostream *out) { getRoutine()->print(out); }

void TaoModule::parseParams() {
  for (size_t i = 0; i < paramNames.size(); i++) {
    TaoParamRefer *refer = new TaoParamRefer();
    myData[paramNames[i]] = refer;
  }
}
TaoRoutine *TaoModule::makeRoutine() {
  TaoRoutine *pnew = new TaoRoutine();

  pnew->name = name;
  pnew->infoKit = infoKit;
  pnew->routProto = this;
  pnew->myData.swap(myData);
  pnew->myPhrases.swap(myPhrases);
  pnew->paramNames.swap(paramNames);
  pnew->setupStatus();

  return pnew;
}
TaoRoutine *TaoModule::getRoutine() {
  if (!routine)
    routine = makeRoutine();
  return routine;
}

TaoReference *TaoModule::getClassData(string &dname) {
  int type;
  if (superClass) {
    type = superClass->getDataType(dname);
    if (type > 0) {
      return superClass->giveClassData(dname);
    } else {
      TaoClass *pclass = superClass;
      while (pclass->superClass) {
        pclass = pclass->superClass;
        type = pclass->getDataType(dname);
        if (type > 1)
          return pclass->giveClassData(dname);
      }
    }
  }
  return 0;
}
TaoModule *TaoModule::getClassMethod(string &rname) {
  int type;
  if (superClass) {
    type = superClass->getMethType(rname);
    if (type > 0) {
      return superClass->getClassMethod(rname);
    } else {
      TaoClass *pclass = superClass;
      while (pclass->superClass) {
        pclass = pclass->superClass;
        type = pclass->getMethType(rname);
        if (type > 1)
          return pclass->getClassMethod(rname);
      }
    }
  }
  return 0;
}
void TaoModule::declareVariable(string &name, int pos) {
  if (name[0] == '@') {
    error_general(srcFName, posLine[pos],
                  "transient variables shouldn't be used here");
  }
  if (varType == 1) {
    // constant data:
    if (myData.find(name) == myData.end()) {
      TaoConstRefer *ref = new TaoConstRefer();
      myData[name] = ref;
      outNameSpace->nsData[name] = ref;
    }
  } else if (varType == 2) {
    // shared data:
    if (TaoReference *ref = inNameSpace->findDataShared(name)) {
      myData[name] = ref;
      outNameSpace->nsData[name] = ref;
    } else {
      ref = new TaoShareRefer();
      myData[name] = ref;
      outNameSpace->nsDataShared[name] = ref;
      outNameSpace->nsData[name] = ref;
    }
  } else if (varType == 3) {
    // extern data:
    if (TaoReference *ref = inNameSpace->findData(name)) {
      myData[name] = ref;
      outNameSpace->nsData[name] = ref;
    } else {
      error_undefined(srcFName, posLine[pos], "extern data");
    }
  } else if (varType == 4) {
    // local data:
    // if(myData.find(name)==myData.end()){
    varLocal[scopeLevel][name] = new TaoReference();
    //}
  } else if (myData.find(name) == myData.end()) {
    if (myWords[pos + 1] == "=" &&
        (myWords[pos + 2] == "open" || myWords[pos + 2] == "compile" ||
         myWords[pos + 2] == "eval" || myWords[pos + 2] == "import" ||
         myWords[pos + 2] == "load")) {
      TaoConstRefer *ref = new TaoConstRefer();
      myData[name] = ref;
      outNameSpace->nsData[name] = ref;
    } else {
      TaoReference *ref = new TaoReference();
      myData[name] = ref;
      if (varToNameSpace)
        outNameSpace->nsData[name] = ref;
    }
  }
}
TaoReference *TaoModule::getVariable(string &name) {
  if (varTransient.find(name) != varTransient.end()) {
    tranUsed++;
    return varTransient[name];
  }

  for (int i = scopeLevel; i >= 0; i--) {
    if (varLocal[i].find(name) != varLocal[i].end())
      return varLocal[i][name];
  }

  if (myData.find(name) != myData.end()) {
    return myData[name];
  }

  return 0;
}
TaoReference *TaoModule::makeVarTransient(int i) {
  // For transient variables:
  int id = i;
  if (id < 0)
    id = varTransient.size() + 1;

  stringstream s;
  s << id;
  string name = "@" + s.str();

  TaoReference *ref = new TaoReference();
  varTransient[name] = ref;
  return ref;
}
TaoReference *TaoModule::makeVarTransient(const string &name) {
  TaoReference *ref = new TaoReference();
  varTransient[name] = ref;
  return ref;
}
bool TaoModule::parsePhrase() {
  map<string, short> &numTypeKey = modKey.numTypeKey;

  parsed = 1;

  //	cout<<"parsing routine \""<<name<<"\".\n";

  TaoConstRefer *ref = new TaoConstRefer(outNameSpace);
  myData["this"] = ref;

  int rbrack;
  int end;
  if (myWords.size() == 0)
    return 1;

  size_t start = 0;
  while (start < myWords.size() - 1) {
    if (myWords[start + 1] != "(") {
      TaoReference *pbase = getClassData(myWords[start]);
      // Don't do this:
      // if( !pbase ) pbase = inNameSpace->findData( myWords[start] );
      if (pbase) {
        myData[myWords[start]] = new TaoDataRefer(pbase);
      } else {
        // Use routine or class as variables:
        TaoBase *p = outNameSpace->findModule(myWords[start]);
        if (!p)
          p = inNameSpace->findModule(myWords[start]);
        if (!p)
          p = outNameSpace->findClass(myWords[start]);
        if (!p)
          p = inNameSpace->findClass(myWords[start]);
        if (p) {
          myData[myWords[start]] = new TaoConstRefer(p);
        }
      }
    } else if (start == 0 || (start > 0 && (myWords[start - 1] != "." ||
                                            myWords[start - 1] != "::"))) {
      //	myWords[start+1]="("
      TaoModule *module = getClassMethod(myWords[start]);
      if (!module)
        module = outNameSpace->findModule(myWords[start]);
      if (!module)
        module = inNameSpace->findModule(myWords[start]);
      if (module) {
        preRoutine[myWords[start]] = module;
        classRoutNames[myWords[start]] = 1;
      }
    }
    start++;
  }

#ifdef DEBUG
  size_t i;
  cout << "TaoModule::parsePhrase:\t";
  for (i = 0; i < myWords.size(); i++)
    cout << myWords[i] << "  ";
  cout << "END\n";
#endif

  varLocal.push_back(map<string, TaoReference *>());
  scopeLevel = 0;
  regexScope.push(0);

  start = 0;
  while (start >= 0 && start < myWords.size()) {
    infoKit = new TaoInfoKit(srcFName, posLine[start]);

    if (myWords[start] == "while") {
      TaoWhile *control = new TaoWhile();
      if ((rbrack = makeLogicLoop(control, start)) < 0)
        return 0;
      start = rbrack + 1;
      continue;
    } else if (myWords[start] == "if") {
      TaoIf *control = new TaoIf();
      if ((rbrack = makeLogicLoop(control, start)) < 0)
        return 0;
      start = rbrack + 1;
      continue;
    } else if (myWords[start] == "else" && myWords[start + 1] == "if") {
      TaoElseIf *control = new TaoElseIf();
      if ((rbrack = makeLogicLoop(control, start)) < 0)
        return 0;
      start = rbrack + 1;
      continue;
    } else if (myWords[start] == "else") {
      TaoElse *control = new TaoElse();
      myPhrases.push_back(control);
      control->stepTo = myPhrases.size();
      start++;
      continue;
    } else if (myWords[start] == "foreach") {
      TaoForEach *foreach = new TaoForEach();
      if ((rbrack = makeForEach(foreach, start)) < 0)
        return 0;
      start = rbrack + 1;
      continue;
    } else if (myWords[start] == "for") {
      TaoFor *tfor = new TaoFor;
      myPhrases.push_back(tfor);
      int rbrack = wordPair(myWords, "(", ")", start);
      vector<int> seps(3);
      seps[0] = findCleanWord(myWords, ",", start + 2, rbrack);
      seps[1] = findCleanWord(myWords, ";", start + 2, rbrack);
      seps[2] = findCleanWord(myWords, ":", start + 2, rbrack);

      sort(seps.begin(), seps.end());
      if (seps[1] >= 0 || seps[2] < 0)
        infoKit->warning("invalid separator in for(...)");
      vector<TaoArithBase *> ariths;
      if (!makeArithArray(ariths, start + 1, rbrack, myWords[seps[2]]))
        return 0;
      if (ariths.size() != 3)
        infoKit->warning("invalid expressions in for(...)");

      tfor->first = ariths[0];
      tfor->condition = ariths[1];
      tfor->step = ariths[2];

      tfor->stepTo = myPhrases.size();
      tfor->jumpTo = myPhrases.size();

      start = rbrack + 1;
      continue;
    } else if (myWords[start] == "skip") {
      TaoSkip *skip = new TaoSkip();
      myPhrases.push_back(skip);
      start += 2;
      continue;
    } else if (myWords[start] == "break") {
      TaoBreak *brk = new TaoBreak();
      myPhrases.push_back(brk);
      start += 2;
      continue;
    }

    if (myWords[start] == "{") {
      scopeLevel++;
      varLocal.resize(scopeLevel + 1);

      LBrace *lb = new LBrace();
      myPhrases.push_back(lb);
      lb->stepTo = myPhrases.size();
      start++;
      continue;
    } else if (myWords[start] == "}") {
      scopeLevel--;
      varLocal.resize(scopeLevel + 1);

      RBrace *rb = new RBrace();
      myPhrases.push_back(rb);
      rb->stepTo = myPhrases.size();
      rb->jumpTo = myPhrases.size();
      start++;
      continue;
    }

    if (myWords[start] == "import") {
      string fname = myWords[start + 3];
      if (fname[0] != '"') {
        cout << "Invalid path\n";
        return 0;
      }
      fname.erase(fname.end() - 1);
      fname.erase(fname.begin());

      TaoScript tao;
      tao.readSource(fname.c_str());

      TaoNameSpace *ns = new TaoNameSpace();
      myData[myWords[start + 1]] = new TaoConstRefer(ns);

      outNameSpace->nameSpace["imported"] = ns;
      tao.setNameSpace(ns, ns);

      if (!tao.compile()) {
        error_general(srcFName, posLine[start],
                      "importing " + myWords[start + 2] + " failed");
        return 0;
      }

      start += 5;
      continue;
    }

    if (myWords[start] == "load") {
      string lbname = myWords[start + 3];
      lbname.erase(lbname.end() - 1);
      lbname.erase(lbname.begin());

      TaoLibrary *lib = new TaoLibrary;
      myData[myWords[start + 1]] = new TaoConstRefer(lib);

      if (!Tao_DynLoad_Lib(lib, lbname)) {
        error_general(srcFName, posLine[start + 1],
                      "Cpp Module " + myWords[start + 3] + " loading failed");
        return 0;
      }
      outNameSpace->nsLibrary[lib->libHandler] = lib;

      start += 5;
      continue;
    }

    end = findCleanWord(myWords, ";", start);
    if (end < 0) {
      error_general(srcFName, posLine[start],
                    "un-parenthesised or ended phrase.\n");
      exit(0);
    }

    if (myWords[start] == "return" || myWords[start] == "yield") {
      TaoReturn *retn = 0;
      if (myWords[start] == "return")
        retn = new TaoReturn();
      else
        retn = new TaoYield();

      retn->infoKit = infoKit;

      if (!makeArithArray(retn->returnAriths, start, end))
        return 0;

      myPhrases.push_back(retn);
      retn->stepTo = myPhrases.size();
      start = end + 1;
      continue;
    }

    //	Parse:
    //	var1,var2,var3;
    // share var1,var2;
    varType = 0;
    if (myWords[start] == "const") {
      varType = 1;
      start++;
    } else if (myWords[start] == "share") {
      varType = 2;
      start++;
    } else if (myWords[start] == "extern") {
      varType = 3;
      start++;
    } else if (myWords[start] == "local") {
      varType = 4;
      start++;
    }
    bool declare = 0;
    if (myWords[start + 1] == ",")
      declare = 1;
    for (int i = start + 3; i < end; i += 2) {
      if (myWords[i] != ",") {
        declare = 0;
        break;
      }
    }
    if (myWords[start + 1] == ";" || declare) {
      for (int i = start; i < end; i += 2) {
        declareVariable(myWords[i], i);
      }
      start = end + 1;
      continue;
    }

    int equal;
    equal = findCleanWord(myWords, "=", start, end);
    TaoChain *lchain = 0;
    vector<TaoChain *> chains;
    varTransient.clear();
    if (equal >= 0) {
      if (myWords[start] == "[" && myWords[equal - 1] == "]") {
        // For multi-assignment:
        // [a,b,c]=something;
        if (!makeChainArray(chains, start, equal - 1))
          return 0;
        for (size_t k = 0; k < chains.size(); k++) {
          chains[k]->subIdxArray = 0;
          chains[k]->subIdxNumArray = 0;
        }
      } else {
        // For normal assigment:
        lchain = new TaoChain();
        lchain->subIdxArray = 0;
        lchain->subIdxNumArray = 0;
        if ((int)start == equal - 1)
          declareVariable(myWords[equal - 1], equal - 1);

        if (!makeChain(lchain, start, equal - 1))
          return 0;
      }

      // For Parsing:
      //
      // a=[10]: "test";

      int workpos = equal + 1;
      /////////////////////////////////////////////////////////////
      vector<TaoArithBase *> dimAriths;
      dimAriths.clear();

      // Compile dimension specifier:
      // {dim1}{dim2}{...} or:
      // int[dim1][dim2][...]
      // double[dim1][dim2][...]
      short typeDim = TAO_DoubleArray;
      if (numTypeKey.find(myWords[workpos]) != numTypeKey.end()) {
        typeDim = numTypeKey[myWords[workpos]];
        workpos++;
      }

      bool isDim = 0;
      if (myWords[workpos] == "[") {
        int rb = wordPair(myWords, "[", "]", workpos, end);
        if (myWords[rb + 1] == "[" || myWords[rb + 1] == ":")
          isDim = 1;
      }
      if (myWords[workpos] == "{" || (myWords[workpos] == "[" && isDim)) {
        // a={5}: 10;
        // b={5}: "adf";
        // Or:
        // a=[5][5]: 10;
        string lchar = "[";
        string rchar = "]";
        if (myWords[workpos] == "{") {
          lchar = "{";
          rchar = "}";
          typeDim = 0;
        }
        int rb = wordPair(myWords, lchar, rchar, workpos, end);
        if (rb < 0) {
          error_unpaired(srcFName, posLine[workpos], lchar + rchar);
          return 0;
        }
        if (rb + 1 < end) {
          TaoArithBase *dim = makeArithTree(workpos + 1, rb - 1);
          if (!dim)
            return 0;
          dimAriths.push_back(dim);

          while (myWords[rb + 1] == lchar) {
            workpos = rb + 1;
            rb = wordPair(myWords, lchar, rchar, workpos, end);
            if (rb < 0) {
              error_unpaired(srcFName, posLine[workpos], lchar + rchar);
              return 0;
            }
            dim = makeArithTree(workpos + 1, rb - 1);
            if (!dim)
              return 0;
            dimAriths.push_back(dim);
          }
          if (myWords[rb + 1] != ":") {
            error_general(srcFName, posLine[rb + 1], "\":\" not found");
            return 0;
          }
          workpos = rb + 2;
        }
      }

      TaoAssign *assign = new TaoAssign();

      TaoArithBase *proto = makeArithTree(workpos, end - 1);
      if (!proto)
        return 0;
      if (varType == 1 || myWords[workpos] == "open" ||
          myWords[workpos] == "compile" || myWords[workpos] == "eval" ||
          myWords[workpos] == "import" || myWords[workpos] == "load")
        assign->keepConst = 0;

      assign->rightArith = proto;

      assign->leftChain = lchain;
      assign->typeNumeric = typeDim;

      assign->infoKit = infoKit;

      assign->dimAriths.swap(dimAriths);
      assign->lchainArray.swap(chains);
      myPhrases.push_back(assign);
      assign->stepTo = myPhrases.size();

      start = end + 1;
    } else {
      TaoArithPhrase *arithFrase = new TaoArithPhrase();
      arithFrase->myArith = makeArithTree(start, end - 1);
      if (!arithFrase->myArith)
        return 0;
      arithFrase->infoKit = infoKit;
      myPhrases.push_back(arithFrase);
      arithFrase->stepTo = myPhrases.size();
      start = end + 1;
    }
  }
  return setupLogicLoop();
}
bool TaoModule::setupLogicLoop() {
  size_t i;
  for (i = 0; i < myPhrases.size(); i++) {
    short rtti = myPhrases[i]->rtti();
    if (rtti == TAO_Return || rtti == TAO_Yield)
      myPhrases[i]->jumpTo = myPhrases.size();

    if (rtti == TAO_While || rtti == TAO_ForEach || rtti == TAO_For) {
      int j = phrasePair(i + 1);
      // It's checked before, safe now:
      myPhrases[i]->jumpTo = j + 1;
      myPhrases[j]->jumpTo = i;
    }
    if (rtti == TAO_If || rtti == TAO_ElseIf) {
      int j = phrasePair(i + 1);
      myPhrases[i]->jumpTo = j + 1;
      // It's checked before, safe now:
      size_t k = j;
      while (k >= 0 && k < myPhrases.size() - 1 &&
             (myPhrases[k + 1]->rtti() == TAO_ElseIf ||
              myPhrases[k + 1]->rtti() == TAO_Else)) {
        k = phrasePair(k + 1);
        if (k >= myPhrases.size() - 1)
          break;
      }

      myPhrases[j]->jumpTo = k + 1;
    }
    if (rtti == TAO_Else) {
      int j = phrasePair(i + 1);
      myPhrases[j]->jumpTo = j + 1;
    }
  }
  for (i = 0; i < myPhrases.size(); i++) {
    short type = myPhrases[i]->rtti();
    if (type == TAO_Break || type == TAO_Skip) {
      for (int j = i; j >= 0; j--) {
        if (myPhrases[j]->rtti() == TAO_While ||
            myPhrases[j]->rtti() == TAO_ForEach ||
            myPhrases[j]->rtti() == TAO_For) {
          if (type == TAO_Break)
            myPhrases[i]->jumpTo = myPhrases[j]->jumpTo;
          if (type == TAO_Skip)
            myPhrases[i]->jumpTo = j;
          break;
        }
      }
    }
  }
  return 1;
}

int TaoModule::makeLogicLoop(TaoControl *control, int start) {
  int lbrack, rbrack;
  int stop = findWord(myWords, "{", start);
  lbrack = findWord(myWords, "(", start, stop);
  rbrack = wordPair(myWords, "(", ")", lbrack, stop);
  if (lbrack < 0 || rbrack < 0) {
    error_unpaired(srcFName, posLine[start], "()");
    return -1;
  }

  control->condition = makeArithTree(lbrack + 1, rbrack - 1);
  if (!control->condition)
    return -1;

  control->infoKit = infoKit;
  myPhrases.push_back(control);
  control->stepTo = myPhrases.size();

  return rbrack;
}
bool TaoModule::makeRegex(TaoRegex *regex, int start, int end) {
  int i, j;
  string word;
  for (i = start; i <= end; i++) {
    if ((myWords[i] == "@" && myWords[i + 1] == "{") || myWords[i] == "[" ||
        myWords[i] == "{" || myWords[i] == "+" || myWords[i] == "*" ||
        myWords[i] == "?" || myWords[i] == "." || myWords[i] == "(") {
      if (word.length() > 0) {
        TaoRgxWord *matchWord = new TaoRgxWord();
        matchWord->setWord(word);
        regex->rgxUnit.push_back(matchWord);
        word.erase();
      }

      if (myWords[i] == "@" && myWords[i + 1] == "{") {
        TaoRgxArith *match = new TaoRgxArith();
        j = wordPair(myWords, "{", "}", i, end);
        if (j < 0) {
          error_unpaired(srcFName, posLine[i], "{}");
          return 0;
        }
        TaoArithBase *arith = makeArithTree(i + 2, j - 1);
        if (!arith)
          return 0;
        match->myArith = arith;
        regex->rgxUnit.push_back(match);
        i = j;
      } else if (myWords[i] == "[") {
        TaoRgxCharEnum *match = new TaoRgxCharEnum();
        j = wordPair(myWords, "[", "]", i, end);
        if (j < 0) {
          error_unpaired(srcFName, posLine[i], "[]");
          return 0;
        }
        string str;
        for (int k = i + 1; k < j; k++)
          str += myWords[k];
        match->makeStrClass(str, infoKit);
        regex->rgxUnit.push_back(match);
        i = j;
      } else if (myWords[i] == "(") {
        j = wordPair(myWords, "(", ")", i, end);
        TaoRgxGroup *group = new TaoRgxGroup();
        int last = i + 1;
        if (myWords[last][0] == '@') {
          group->transient = makeVarTransient(myWords[last]);
          last += 2;
        }
        int k = findCleanWord(myWords, "|", last, j);
        while (k >= 0) {
          TaoRegex *rgx = new TaoRegex;
          makeRegex(rgx, last, k - 1);
          group->myRegex.push_back(rgx);
          last = k + 1;
          k = findCleanWord(myWords, "|", last, j);
        }
        TaoRegex *rgx = new TaoRegex;
        makeRegex(rgx, last, j - 1);
        group->myRegex.push_back(rgx);
        regex->rgxUnit.push_back(group);
        i = j;
      } else if (myWords[i] == "{") {
        j = wordPair(myWords, "{", "}", i, end);
        if (j < 0) {
          error_unpaired(srcFName, posLine[i], "{}");
          return 0;
        }

        int comma = findWord(myWords, ",", i, j);
        if (comma >= 0) {
          TaoArithBase *min = makeArithTree(i + 1, comma - 1);
          if (!min)
            return 0;
          regex->rgxUnit[regex->rgxUnit.size() - 1]->minArith = min;
          if (comma + 1 <= j - 1) {
            TaoArithBase *max = makeArithTree(comma + 1, j - 1);
            if (!max)
              return 0;
            regex->rgxUnit[regex->rgxUnit.size() - 1]->maxArith = max;
          } else {
            regex->rgxUnit[regex->rgxUnit.size() - 1]->repMax =
                MAX_REPEAT_MATCH;
          }
        } else {
          TaoArithBase *min = makeArithTree(i + 1, j - 1);
          if (!min)
            return 0;
          regex->rgxUnit[regex->rgxUnit.size() - 1]->minArith = min;
        }
        i = j;
      } else if (myWords[i] == "?") {
        regex->rgxUnit[regex->rgxUnit.size() - 1]->ignorable = 1;
        regex->rgxUnit[regex->rgxUnit.size() - 1]->repMin = 1;
        regex->rgxUnit[regex->rgxUnit.size() - 1]->repMax = 1;
      } else if (myWords[i] == "*") {
        regex->rgxUnit[regex->rgxUnit.size() - 1]->ignorable = 1;
        regex->rgxUnit[regex->rgxUnit.size() - 1]->repMin = 1;
        regex->rgxUnit[regex->rgxUnit.size() - 1]->repMax = MAX_REPEAT_MATCH;
      } else if (myWords[i] == "+") {
        regex->rgxUnit[regex->rgxUnit.size() - 1]->repMin = 1;
        regex->rgxUnit[regex->rgxUnit.size() - 1]->repMax = MAX_REPEAT_MATCH;
      } else if (myWords[i] == ".") {
        TaoRgxCharClass *charClass = new TaoRgxCharClass();
        charClass->set_escape_char(myWords[i][0]);
        regex->rgxUnit.push_back(charClass);
      }
    } else if (myWords[i] == "\\d" || myWords[i] == "\\D" ||
               myWords[i] == "\\w" || myWords[i] == "\\W" ||
               myWords[i] == "\\c" || myWords[i] == "\\C" ||
               myWords[i] == "\\s" || myWords[i] == "\\S" ||
               myWords[i] == "\\t" || myWords[i] == "\\n") {
      if (word.length() > 0) {
        TaoRgxWord *matchWord = new TaoRgxWord();
        matchWord->setWord(word);
        regex->rgxUnit.push_back(matchWord);
        word.erase();
      }

      TaoRgxCharClass *charClass = new TaoRgxCharClass();
      charClass->set_escape_char(myWords[i][1]);
      regex->rgxUnit.push_back(charClass);
    } else {
      if (myWords[i][0] == '\\')
        word += myWords[i][1];
      else
        word += myWords[i];
    }
  }
  if (word.length() > 0) {
    TaoRgxWord *matchWord = new TaoRgxWord();
    matchWord->setWord(word);
    regex->rgxUnit.push_back(matchWord);
    word.erase();
  }

  return 1;
}
bool TaoModule::makeChain(TaoChain *chain, int left, int right) {
  //	cout<<"TaoModule::makeChain\t";
  //	for(int i=left;i<=right;i++)
  //		cout<<myWords[i]<<"  ";
  //	cout<<endl;
  int rbrack;
  int start = left;

  chain->infoKit = infoKit;

  chain->firstName = myWords[left];
  //	if:   func()...
  //	else: obj-> , objs[]...
  if (myWords[left + 1] == "(") {
    if (classRoutNames.find(myWords[left]) != classRoutNames.end())
      chain->isClassRout = 1;

    rbrack = wordPair(myWords, "(", ")", left, right);
    if (rbrack < 0) {
      error_unpaired(srcFName, posLine[left], "()");
      return 0;
    }
    TaoFunCall *fun = new TaoFunCall();
    fun->funName = myWords[left];
    if (!makeFunCall(fun, left, rbrack))
      return 0;
    chain->myChain.push_back(fun);
    start = rbrack + 1;
  } else {
    TaoReference *ref = getVariable(myWords[left]);
    if (ref)
      chain->myChain.push_back(ref);
    else
      infoKit->error_undefined(myWords[left]);
    start++;
  }
  while (start <= right) {
    if (myWords[start] == "(") {
      // obj->FUNCTION(A,B)->more:
      //      ~~~~~~~~~~~~
      TaoFunCall *fun = new TaoFunCall();
      fun->funName = myWords[start - 1];
      rbrack = wordPair(myWords, "(", ")", start, right);
      if (rbrack < 0) {
        error_unpaired(srcFName, posLine[start], "()");
        return 0;
      }
      if (!makeFunCall(fun, start - 1, rbrack))
        return 0;

      chain->myChain.pop_back();
      chain->myChain.push_back(fun);
      start = rbrack + 1;
    } else if (myWords[start] == "[") {
      //	hash|vector[i]:
      rbrack = wordPair(myWords, "[", "]", start, right);
      typeNumeric = TAO_IntArray;
      if (rbrack < 0) {
        error_unpaired(srcFName, posLine[left], "[]");
        return 0;
      }
      TaoArithArray *index = new TaoArithArray();
      if (!makeArithArray(index->arithArray, start, rbrack))
        return 0;

      typeNumeric = TAO_DoubleArray;
      chain->myChain.push_back(index);
      start = rbrack + 1;
    } else if (myWords[start] == "." || myWords[start] == "::") {
      chain->myChain.push_back(new TaoBase());
      start++;
    } else if (myWords[start] == ".?") {
      chain->myChain.push_back(new TaoOperType());
      start++;
    } else if (myWords[start] == ".#") {
      chain->myChain.push_back(new TaoOperSizeID());
      start++;
    } else if (myWords[start] == ".&") {
      chain->myChain.push_back(new TaoOperDerefer());
      start++;
    } else if (myWords[start] == ".%") {
      chain->myChain.push_back(new TaoOperHashKey());
      start++;
    } else if (myWords[start] == ".@") {
      chain->myChain.push_back(new TaoOperHashValue());
      start++;
    } else if (myWords[start] == "=~" || myWords[start] == "!~" ||
               myWords[start] == "~~") {
      TaoRgxMatch *rgx = new TaoRgxMatch();
      if (myWords[start] == "=~")
        rgx->rgxType = 1;
      else if (myWords[start] == "!~")
        rgx->rgxType = 2;
      else if (myWords[start] == "~~")
        rgx->rgxType = 3;

      start++;
      if (myWords[start] == "s") {
        rgx->replace = 1;
        start++;
      } else if (myWords[start] == "gs") {
        rgx->replaceAll = 1;
        start++;
      }

      if (myWords[start] != "/")
        error_general(srcFName, posLine[start],
                      "regular expression expected here");

      int pos = findCleanWord(myWords, "/", start + 1, right);
      if (pos < 0)
        error_general(srcFName, posLine[start], "invalid regular expression");

      if (regexScope.size() > 0) {
        if (regexScope.top() > 0)
          rgx->restart = 1;
        regexScope.top()++;
      }

      TaoRegex *regular = new TaoRegex;
      makeRegex(regular, start + 1, pos - 1);
      rgx->regex = regular;

      regexScope.top()--;

      int pos2 = findCleanWord(myWords, "/", pos + 1, right);
      if (rgx->replace && pos2 < 0)
        infoKit->warning("invalid regex substitution form");
      int pos3 = -1;
      if (pos2 >= 0)
        pos3 = findCleanWord(myWords, "/", pos2 + 1, right);
      if (pos3 >= 0)
        infoKit->warning("ambigous regex slashes, the last is used");
      while (pos3 >= 0) {
        pos2 = pos3;
        pos3 = findCleanWord(myWords, "/", pos2 + 1, right);
      }

      if (rgx->replace || pos2 > 0) {
        TaoArithBase *arith = makeArithTree(pos + 1, pos2 - 1);
        if (!arith)
          return 0;
        rgx->strArith = arith;
        pos = pos2;
      }

      chain->myChain.push_back(rgx);
      start = pos + 1;
    } else {
      TaoString *str = new TaoString(myWords[start]);
      chain->myChain.push_back(str);
      start++;
    }
  }
  return 1;
}
bool TaoModule::makeEnumMatrix(TaoEnumMatrix *matx, int lb, int rb) {
  int last = lb;
  int semi = findWord(myWords, ";", lb, rb);
  matx->nRow = 1;
  while (semi >= 0) {
    vector<TaoArithBase *> oneRow;
    matx->nRow++;
    if (!makeArithArray(oneRow, last, semi))
      return 0;
    if ((int)oneRow.size() > matx->nColumn)
      matx->nColumn = oneRow.size();
    matx->matxAriths.push_back(oneRow);

    last = semi;
    semi = findWord(myWords, ";", last + 1, rb);
  }
  vector<TaoArithBase *> oneRow;
  if (!makeArithArray(oneRow, last, rb))
    return 0;
  if ((int)oneRow.size() > matx->nColumn)
    matx->nColumn = oneRow.size();
  matx->matxAriths.push_back(oneRow);

  return 1;
}
bool TaoModule::makeFunCall(TaoFunCall *fun, int left, int right) {
  map<string, int> &funcIntern = modKey.funcIntern;
  map<string, int> &methIntern = modKey.methIntern;

  fun->infoKit = infoKit;

  fun->funName = myWords[left];
  fun->defaultNS = outNameSpace;

  if (funcIntern.find(myWords[left]) != funcIntern.end() &&
      (left == 0 || (myWords[left - 1] != "." && myWords[left - 1] != "::"))) {
    fun->isInternal = 1;
    fun->funType = funcIntern[myWords[left]];
  } else if (preRoutine.find(myWords[left]) != preRoutine.end()) {
    fun->preRoutine = preRoutine[myWords[left]];
  } else if (methIntern.find(myWords[left]) != methIntern.end()) {
    fun->funType = methIntern[myWords[left]];
  } else if (myWords[left] == "open") {
    fun->ioType = 0;
    fun->funType = TAO_open;
  } else if (myWords[left] == "print") {
    fun->ioType = 1;
    fun->funType = TAO_print;
  } else if (myWords[left] == "read") {
    fun->ioType = 2;
    fun->funType = TAO_read;
  } else if (outNameSpace->findClass(myWords[left])) {
    fun->myClass = outNameSpace->findClass(myWords[left]);
  } else if (inNameSpace->findClass(myWords[left])) {
    fun->myClass = inNameSpace->findClass(myWords[left]);
    //	}else if( inNameSpace->findPlugin( myWords[left] ) ){
    //		fun->myPlugin=inNameSpace->findPlugin( myWords[left] );
  } else if (left == 0 ||
             (myWords[left - 1] != "." && myWords[left - 1] != "::")) {
    cout << "Warning: unknown function " << myWords[left] << "().\n";
  }

  left = findWord(myWords, "(", left + 1, right);

  if (fun->funType == TAO_numarray || fun->funType == TAO_arg ||
      fun->funType == TAO_norm)
    fun->numarrayMaker = new TaoNumArrayMaker;

  // For transient variables:
  if (fun->funType == TAO_iterate || fun->funType == TAO_iterget) {
    fun->transient.push_back(makeVarTransient());
  } else if (fun->funType == TAO_sort) {
    varTransient.clear();
    fun->transient.push_back(makeVarTransient());
    fun->transient.push_back(makeVarTransient());
  } else if (fun->funType == TAO_which || fun->funType == TAO_numarray ||
             fun->funType == TAO_apply || fun->funType == TAO_noapply) {
    for (size_t i = 0; i < 9; i++)
      fun->transient.push_back(makeVarTransient());
    fun->transient.insert(fun->transient.begin(), makeVarTransient(0));
  } else if (fun->funType == TAO_repeat) {
    regexScope.push(0);
  }

  if (!makeArithArray(fun->paramAriths, left, right))
    return 0;

  if (fun->funType == TAO_repeat)
    regexScope.pop();

  return 1;
}
bool TaoModule::makeChainArray(vector<TaoChain *> &chains, int left,
                               int right) {
  if (right > left + 1) {
    int last = left + 1;
    int comma = findCleanWord(myWords, ",", left + 1, right);
    while (comma >= 0) {
      if (myData.find(myWords[last]) == myData.end())
        myData[myWords[last]] = new TaoReference();

      TaoChain *chn = new TaoChain();
      if (!makeChain(chn, last, comma - 1))
        return 0;
      chains.push_back(chn);

      last = comma + 1;
      comma = findCleanWord(myWords, ",", comma + 1, right);
    }
    if (myData.find(myWords[last]) == myData.end())
      myData[myWords[last]] = new TaoReference();

    TaoChain *chn = new TaoChain();
    if (!makeChain(chn, last, right - 1))
      return 0;
    chains.push_back(chn);
  }
  return 1;
}
bool TaoModule::makeArithArray(vector<TaoArithBase *> &ariths, int left,
                               int right, const string sep) {
  //	for(int i=left;i<=right;i++)
  //		cout<<myWords[i]<<"  ";
  //	cout<<"\n";

  if (right > left + 1) {
    int last = left + 1;
    int comma = findCleanWord(myWords, sep, left + 1, right);
    while (comma >= 0) {
      TaoArithBase *node = 0;
      if (last <= comma - 1) {
        node = makeArithTree(last, comma - 1);
        if (!node)
          return 0;
      } else {
        node = new TaoNullArith();
      }
      ariths.push_back(node);

      last = comma + 1;
      comma = findCleanWord(myWords, sep, comma + 1, right);
    }
    TaoArithBase *node = 0;
    if (last <= right - 1) {
      node = makeArithTree(last, right - 1);
      if (!node)
        return 0;
    } else {
      node = new TaoNullArith();
    }
    ariths.push_back(node);
  }
  return 1;
}
int TaoModule::makeForEach(TaoForEach *foreach, int start) {
  int lbrack, rbrack;
  int stop = findWord(myWords, "{", start);
  lbrack = findWord(myWords, "(", start, stop);
  rbrack = wordPair(myWords, "(", ")", lbrack, stop);
  if (lbrack < 0 || rbrack < 0) {
    error_unpaired(srcFName, posLine[start], "()");
    return -1;
  }
  int comma = findWord(myWords, ":", lbrack, rbrack);
  if (comma < 0) {
    error_general(srcFName, posLine[start], "\":\" is missing");
    return -1;
  }

  foreach
    ->elemName = myWords[rbrack - 1];
  foreach
    ->chainArray = new TaoChain();
  if (!makeChain(foreach->chainArray, lbrack + 1, comma - 1))
    return -1;

  map<string, TaoReference *>::iterator iter;
  iter = myData.find(myWords[rbrack - 1]);
  if (iter != myData.end()) {
    short rt = iter->second->rtti();
    if (rt != TAO_Reference)
      cout << "Warning: Data type will be changed!\n";
    else
      foreach
        ->refElement = (TaoReference *)iter->second;
  } else {
    foreach
      ->refElement = new TaoReference(); // myWords[rbrack-1]);
    myData[myWords[rbrack - 1]] = foreach->refElement;
  }

  foreach
    ->infoKit = infoKit;
  myPhrases.push_back(foreach);
  foreach
    ->stepTo = myPhrases.size();
  return rbrack;
}

int TaoModule::findRootOper(int start, int end, char *optype) {
  return findCleanWords(myWords, modKey.arithOpers, modKey.arithTypes, start,
                        end, optype);
}
TaoArithBase *TaoModule::makeArithLeaf(int start) {
  TaoArithBase *node = NULL;
  if (getVariable(myWords[start])) {
    TaoArithVariable *node2 = new TaoArithVariable;
    node = node2;
    node2->refVariable = getVariable(myWords[start]);
  } else if (myWords[start][0] == '@') {
    // If the transient variable is valid, it should be
    // returned by getVariable():
    error_general(srcFName, posLine[start],
                  "invalid transient variable " + myWords[start]);
  } else if (myWords[start][0] == '"') {
    TaoArithString *node2 = new TaoArithString;
    node = node2;
    node2->myChars = myWords[start];
    node2->myChars.erase(node->myChars.end() - 1);
    node2->myChars.erase(node->myChars.begin());
    node2->myValue = atof(node->myChars.c_str());
    node2->bulean = node2->myValue;
  } else if (isdigit(myWords[start][0])) {
    TaoArithNumber *node2 = new TaoArithNumber;
    node = node2;
    node2->myChars = myWords[start];
    node2->myValue = atof(node->myChars.c_str());
    node2->bulean = node2->myValue;
  } else if (myWords[start] == "$") {
    TaoArithComplex *node2 = new TaoArithComplex;
    node = node2;
    node2->cmplx = complex<double>(0, 1);
  } else if (myWords[start] == ":") {
    node = new TaoNullArith;
  } else if (myWords[start + 1] == ":=" || myWords[start + 1] == "=") {
    TaoReference *ref = new TaoReference;
    myData[myWords[start]] = ref;
    TaoArithVariable *node2 = new TaoArithVariable;
    node = node2;
    node2->refVariable = ref;
  } else {
    error_undefined(srcFName, posLine[start], myWords[start]);
    return 0;
  }
  return node;
}
TaoArithBase *TaoModule::makeArithNode(int start, int end) {
  map<string, char> &rightUnary = modKey.rightUnary;
  map<string, char> &leftUnary = modKey.leftUnary;

  TaoArithBase *node = 0;
  stripWordPair(myWords, "(", ")", start, end);

  char optype;
  int pos = findRootOper(start, end, &optype);

  if (start > end)
    node = new TaoNullArith;
  else if (start == end)
    node = makeArithLeaf(start);
  else if (pos >= 0) {
    if (myWords[pos] == "=~" || myWords[pos] == "!~" || myWords[pos] == "~~")
      node = new TaoArithChain;
    else if (myWords[pos] == ":")
      node = new TaoPairArith;
    else if (myWords[pos] == "?")
      node = new TaoSwitchArith;
    else {
      node = new TaoArithNode;
      if (myWords[pos] == "=")
        infoKit->warning("\"=\" is used as assignement inside an arith exprs, "
                         "use \":=\" instead");
    }
  } else if (rightUnary.find(myWords[end]) != rightUnary.end())
    node = new TaoArithNode;
  else if (leftUnary.find(myWords[start]) != leftUnary.end())
    node = new TaoArithNode;
  else if ((myWords[start] == "{" && myWords[end] == "}") ||
           (myWords[start] == "[" && myWords[end] == "]"))
    node = new TaoEnumArith;
  else
    node = new TaoArithChain;

  return node;
}

TaoArithBase *TaoModule::makeArithTree(int start, int end) {
  tranUsed = 0;
  TaoArithBase *arith = makeArithNode(start, end);
  if (!makeArithTree(arith, start, end)) {
    delete arith;
    return 0;
  }
  if (tranUsed > 0)
    arith->nonTrans = 0;
  return arith;
}
bool TaoModule::makeArithTree(TaoArithBase *node2, int start, int end) {
  map<string, char> &rightUnary = modKey.rightUnary;
  map<string, char> &leftUnary = modKey.leftUnary;

  node2->infoKit = infoKit;

  if (start > end) {
    // node2 must be TaoNullArith:
    return 1;
  }
#ifdef DEBUG
  cout << "TaoModule::makeArithTree\n";
  for (int i = start; i <= end; i++)
    cout << myWords[i] << "  ";
  cout << endl;
#endif

  stripWordPair(myWords, "(", ")", start, end);
  if (start == end)
    return 1;

  char optype;
  int pos = findRootOper(start, end, &optype);

  if (pos > start &&
      (myWords[pos] == "=~" || myWords[pos] == "!~" || myWords[pos] == "~~")) {
    TaoArithChain *node = (TaoArithChain *)node2;
    TaoChain *chain = new TaoChain();
    if (!makeChain(chain, start, end))
      return 0;
    node->myChain = chain;
  } else if (pos >= start &&
             leftUnary.find(myWords[start]) == leftUnary.end()) {
    TaoArithNode *node = (TaoArithNode *)node2;
    string opstr = myWords[pos];

    int end2 = pos - 1;
    int start2 = pos + 1;

    TaoArithBase *left = makeArithNode(start, end2);
    TaoArithBase *right = makeArithNode(start2, end);

    if (!makeArithTree(left, start, end2))
      return 0;
    if (!makeArithTree(right, start2, end))
      return 0;

    node->left = left;
    node->right = right;
    left->pro = node;
    right->pro = node;

    node->oper = optype;
  } else if (rightUnary.find(myWords[end]) != rightUnary.end()) {
    TaoArithNode *node = (TaoArithNode *)node2;
    TaoArithBase *left = makeArithNode(start, end - 1);
    if (!makeArithTree(left, start, end - 1))
      return 0;
    node->left = left;
    left->pro = node;
    node->oper = rightUnary[myWords[end]];

  } else if (leftUnary.find(myWords[start]) != leftUnary.end()) {
    TaoArithNode *node = (TaoArithNode *)node2;
    TaoArithBase *right = makeArithNode(start + 1, end);
    if (!makeArithTree(right, start + 1, end))
      return 0;
    node->right = right;
    right->pro = node;
    node->oper = leftUnary[myWords[start]];
  } else if ((myWords[start] == "{" && myWords[end] == "}") ||
             (myWords[start] == "[" && myWords[end] == "]")) {

    TaoEnumArith *node = (TaoEnumArith *)node2;

    // { "a" : 1, "b" : [] };
    // {1,2,10}
    // {1:2:10}
    // [1,2,10]
    // [1:2:10]
    int lb = start;
    bool isNumeric = 0;
    if (myWords[start] == "[") {
      isNumeric = 1;
      if (myWords[start + 1] == "byte") {
        lb++;
        typeNumeric = TAO_ByteArray;
      } else if (myWords[start + 1] == "short") {
        lb++;
        typeNumeric = TAO_ShortArray;
      } else if (myWords[start + 1] == "int") {
        lb++;
        typeNumeric = TAO_IntArray;
      } else if (myWords[start + 1] == "float") {
        lb++;
        typeNumeric = TAO_FloatArray;
      } else if (myWords[start + 1] == "double") {
        lb++;
        typeNumeric = TAO_DoubleArray;
      }
    }

    int rb = end;
    int colon = findCleanWord(myWords, ",", lb + 1, rb - 1);
    int comma1 = findCleanWord(myWords, ":", lb + 1, rb - 1);
    int semi = findCleanWord(myWords, ";", lb + 1, rb - 1);

    int comma2 = -1;
    if (comma1 >= 0)
      comma2 = findCleanWord(myWords, ":", comma1 + 1, rb - 1);

    if ((colon >= 0 && comma1 >= 0) || (comma1 >= 0 && comma2 < 0)) {

      // [ "a" : 1, "b" : [] ];
      // [:];
      TaoEnumHash *hash = new TaoEnumHash;
      hash->infoKit = infoKit;
      node->myEnum = hash;
      if (!makeArithArray(hash->rightAriths, lb, rb))
        return 0;
    } else if (comma2 >= 0) {
      // [1:2:10]
      TaoRangeArray *vectde = new TaoRangeArray();
      vectde->isNumeric = isNumeric;
      vectde->typeNumeric = typeNumeric;
      vectde->infoKit = infoKit;
      node->myEnum = vectde;
      if (!makeArithArray(vectde->rightAriths, lb, rb, ":"))
        return 0;

    } else if (semi < 0) {
      // [a,b,c]
      TaoEnumArray *vectde = new TaoEnumArray();
      vectde->isNumeric = isNumeric;
      vectde->typeNumeric = typeNumeric;
      vectde->infoKit = infoKit;
      node->myEnum = vectde;
      if (!makeArithArray(vectde->rightAriths, lb, rb))
        return 0;
    } else {
      // (1,2; 3,4)
      TaoEnumMatrix *matx = new TaoEnumMatrix();
      matx->infoKit = infoKit;
      matx->typeNumeric = typeNumeric;
      node->myEnum = matx;
      if (!makeEnumMatrix(matx, lb, rb))
        return 0;
    }
  } else {
    TaoArithChain *node = (TaoArithChain *)node2;
    TaoChain *chain = new TaoChain();
    if (!makeChain(chain, start, end))
      return 0;
    node->myChain = chain;
  }
  return 1;
}

int TaoModule::phrasePair(int left) {
  int k = 0;
  for (size_t j = left; j < myPhrases.size(); j++) {
    if (myPhrases[j]->rtti() == TAO_LBrace)
      k++;
    else if (myPhrases[j]->rtti() == TAO_RBrace) {
      k--;
      if (k == 0)
        return j;
    }
  }
  return -1;
}
