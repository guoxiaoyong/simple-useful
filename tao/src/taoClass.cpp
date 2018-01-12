
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
#include "taoClass.h"
#include "taoObject.h"
#include "taoPhrase.h"
#include "taoUtility.h"
//#include"phunyx.h"

extern map<int, string> __Tao_TypeToName;

short TaoClass::RTTI = TAO_Class;
short TaoClass::rtti() const { return RTTI; }
int TaoClass::uniClassID = 1000;

TaoClass::TaoClass(string name) {
  className = name;
  superClass = 0;
  parsed = 0;
  uniClassID++;
  idClass = uniClassID;
  __Tao_TypeToName[idClass] = className;
}

//!	Parsing the class members.
bool TaoClass::parseMember() {
  unsigned int start = 0;
  unsigned int right;

  int type = 1;

  success = 1;
  parsed = 1;

  dataMember.clear();
  methMember.clear();

  while (start >= 0 && start < classWords.size()) {
    // Scope type:
    if (classWords[start] == "private") {
      type = 1;
      start++;
      continue;
    } else if (classWords[start] == "protected") {
      type = 2;
      start++;
      continue;
    } else if (classWords[start] == "public") {
      type = 3;
      start++;
      continue;
    } else if (classWords[start] == ":") {
      start++;
      continue;
    }

    // Member methods:
    if (classWords[start] == "routine") {
      int funpos = start + 1;

      TaoModule *rout;
      if (classWords[start + 1] == className) {
        rout = &preConstructor;
      } else {
        rout = new TaoModule();
        // classWords[start]="sub"/"cor"
        // rout->routype=classWords[start];
        rout->name = classWords[start + 1];
        methType[classWords[start + 1]] = type;
        methMember[classWords[start + 1]] = rout;
      }

      rout->setSuperClass(this);
      rout->setNameSpace(inNameSpace, outNameSpace);

      int lb = start + 2;
      int rb = wordPair(classWords, "(", ")", start);
      if (rb < 0) {
        error_unpaired(srcFName, posClsWord[start], "()");
        return 0;
      }

      start = rb;
      // Make parameter list:

      //			if(classWords[rb+1]!="{"){
      vector<string> params;
      for (int i = lb + 1; i < rb; i++) {
        if (classWords[i] != ",") {
          params.push_back(classWords[i]);
          rout->appendParams(classWords[i]);
        }
      }
      // For the constructor of its super class:
      if (classWords[rb + 1] == ":") {
        idParams.resize(params.size(), -1);
        int lb = rb + 3;
        rb = wordPair(classWords, "(", ")", lb);
        if (rb < 0) {
          error_unpaired(srcFName, posClsWord[lb], "()");
          return 0;
        }
        // Check if a parameter in the constructor is
        // also presented as a parameter for the constructor
        // of its super class.
        start = rb;
        int id = 0;
        for (int i = lb + 1; i < rb; i++) {
          if (classWords[i] != ",") {
            bool found = 0;
            for (unsigned int j = 0; j < params.size(); j++) {
              if (classWords[i] == params[j]) {
                idParams[j] = id;
                found = 1;
                break;
              }
            }
            if (!found)
              cout << "Warning: class123\n";
            id++;
          }
        }
      }
      //			}
      if (classWords[start + 1] != ";") {
        //	It's inline function, in C++ term.

        right = wordPair(classWords, "{", "}", start);
        if (right < 0) {
          error_unpaired(srcFName, posClsWord[start], "{}");
          return 0;
        }
        for (unsigned int i = start + 2; i < right; i++)
          rout->appendWord(classWords[i], posClsWord[i]);

        start = right + 1;
      } else {
        //	For functions define outside of the class.
        int ifun = findWord(extraWords, "routine"); // rout->routype);

        //	Look up in "extraWords" its definition:
        while (ifun >= 0 && extraWords[ifun + 1] != classWords[funpos]) {
          ifun = findWord(extraWords, "routine", ifun + 1);
          if (ifun < 0)
            break;
        }
        if (ifun < 0) {
          error_general(srcFName, posClsWord[funpos],
                        "function \"" + classWords[funpos] +
                            "\" declared but not defined");
          return 0;
        } else {
          unsigned int i;
          unsigned int rb2 = wordPair(extraWords, "(", ")", ifun);
          if (rb2 < 0) {
            error_unpaired(srcFName, posExtWord[ifun], "()");
            return 0;
          }
          for (i = ifun + 3; i < rb2; i++) {
            rout->appendParams(extraWords[i]);
          }

          right = wordPair(extraWords, "{", "}", rb2);
          if (right < 0) {
            error_unpaired(srcFName, posExtWord[rb2], "{}");
            return 0;
          }
          for (i = rb2 + 2; i < right; i++) {
            rout->appendWord(extraWords[i], posExtWord[i]);
          }
        }
        start = rb + 2;
      }
    } else {
      int end = findWord(classWords, ";", start);
      bool declare = 1;
      for (int i = start + 1; i < end; i += 2) {
        if (classWords[i] != ",") {
          declare = 0;
          break;
        }
      }
      if (classWords[start + 1] == ";" || declare) {
        for (int i = start; i < end; i += 2) {
          if (dataMember.find(classWords[i]) == dataMember.end()) {
            dataMember[classWords[i]] = new TaoReference();
            dataType[classWords[i]] = type;
          }
        }
        start = end + 1;
        continue;
      }
      start++;
    }
  }

  preConstructor.name = "constructor for class \"" + className + "\"";
  preConstructor.setNameSpace(inNameSpace, outNameSpace);
  preConstructor.parseParams();

  if (!preConstructor.parsePhrase())
    return 0;
  constructor = preConstructor.getRoutine();

  map<string, TaoModule *>::iterator iter;
  for (iter = methMember.begin(); iter != methMember.end(); ++iter) {
    iter->second->parseParams();
    if (!iter->second->parsePhrase())
      return 0;
  }

  map<string, TaoReference *>::iterator itdata;
  int id = 0;
  for (itdata = dataMember.begin(); itdata != dataMember.end(); ++itdata) {
    dataIndex[itdata->first] = id;
    id++;
  }
  return 1;
}
TaoObject *TaoClass::makeInstance() {
  TaoObject *pnew = new TaoObject(this);
  return pnew;
}
