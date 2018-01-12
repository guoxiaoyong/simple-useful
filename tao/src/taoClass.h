
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
#ifndef TAO_CLASS_H
#define TAO_CLASS_H

#include "stdio.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "taoModule.h"
#include "taoRoutine.h"

using namespace std;

class TaoObject;
class TaoNameSpace;

//! TaoClass is the class to compile Tao classes.
/*!
*/
class TaoClass : public TaoModuleBase {
  static int uniClassID;

  int idClass;

protected:
  friend class TaoObject;

  // For holding words inside the "class name{ }" block (main body).
  vector<string> classWords;
  // For holding words of defining member methods outside
  // the class main body.
  vector<string> extraWords;
  // Position of the words in the source file.
  vector<int> posClsWord;
  vector<int> posExtWord;

  // Scope type of member data,
  // private=1, protected=2, public=3.
  map<string, int> dataType;
  // Scope type of member methods.
  map<string, int> methType;
  // For mapping variable name to its index in
  // "dataMember" of TaoObject!!!
  map<string, int> dataIndex;

  // Holding class member data.
  map<string, TaoReference *> dataMember;
  // Holding prototypes of class member methods.
  map<string, TaoModule *> methMember;

  // Pre-constructor of class constructor.
  TaoModule preConstructor;

public:
  TaoClass(string name);
  virtual ~TaoClass(){};

  // Super class.
  TaoClass *superClass;

  bool parsed;

  // Used for the constructor:
  // class son : base{}
  // sub son ( a, b, c, d):base(c,a,d)...
  //           1,-1, 0, 2
  // idParams[i] takes value "id", if it appears in "id-1"-th
  // position in the parameter list of its super class;
  // Otherwise -1.
  vector<int> idParams;

  // Class name
  string className;

  // Class contructor.
  TaoRoutine *constructor;

  void appendClassWord(string &str, int pos = 0) {
    classWords.push_back(str);
    posClsWord.push_back(pos);
  };

  void appendExtraWord(string &word, int pos = 0) {
    extraWords.push_back(word);
    posExtWord.push_back(pos);
  }
  bool parseMember();

  int getMethType(string &name) {
    if (methType.find(name) == methType.end())
      return -1;
    else
      return methType[name];
  }
  int getDataType(string &name) {
    if (dataType.find(name) == dataType.end())
      return -1;
    else
      return dataType[name];
  }
  TaoReference *giveClassData(string &name) { return dataMember[name]; }
  TaoModule *getClassMethod(string &name) { return methMember[name]; }

  TaoModule *getPubMethod(string &name) {
    int type = getMethType(name);
    if (type > 2)
      return methMember[name];
    else if (type < 0 && superClass)
      // If a public method in a parent class is overrided as a
      // private or protected method, it will not be public any more.
      return superClass->getPubMethod(name);
    return 0;
  }

  TaoObject *makeInstance();

  static short RTTI;
  virtual short rtti() const;

  int classid() const { return idClass; }
};

#endif
