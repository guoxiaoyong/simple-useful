
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
#ifndef TAO_EXTDATA_H
#define TAO_EXTDATA_H

#include "taoDatatype.h"
#include "taoPlugin.h"

class TaoClass;
class TaoModule;
class TaoRoutine;

/*
class TaoFile : public TaoBase
{
        public:
        string fname;
};
*/

class TaoFin : public TaoBase {
public:
  TaoFin() { fin = 0; }
  ~TaoFin() {
    if (fin)
      fin->close();
  }

  string fname;
  ifstream *fin;

  static short RTTI;
  virtual short rtti() const;
};

class TaoFout : public TaoBase {
public:
  TaoFout() { fout = 0; }
  ~TaoFout() {
    if (fout)
      fout->close();
  }

  string fname;
  ofstream *fout;

  static short RTTI;
  virtual short rtti() const;
};

//! Tao reference type for Tao object data.
class TaoDataRefer : public TaoReference {
public:
  TaoDataRefer(){};
  TaoDataRefer(TaoBase *p);

  static short RTTI;
  virtual short rttiRefer() const;

  //	TaoBase*	duplicate();
  TaoReference *newObject() { return new TaoDataRefer; }
};
//! Tao reference type for function parameters.
class TaoParamRefer : public TaoReference {
public:
  TaoParamRefer(){};
  TaoParamRefer(TaoBase *p);

  static short RTTI;
  virtual short rttiRefer() const;

  //	TaoBase*	duplicate();
  TaoReference *newObject() { return new TaoParamRefer; }
};
//! Tao reference type for global data shared in the same namespace.
class TaoShareRefer : public TaoReference {
public:
  TaoShareRefer(){};
  TaoShareRefer(TaoBase *p);

  static short RTTI;
  virtual short rttiRefer() const;

  //	TaoBase*	duplicate();
  TaoReference *newObject() { return new TaoShareRefer; }
};
class TaoConstRefer : public TaoReference {
public:
  TaoConstRefer(){};
  TaoConstRefer(TaoBase *p);

  static short RTTI;
  virtual short rttiRefer() const;

  //	TaoBase*	duplicate();
  TaoReference *newObject() { return new TaoConstRefer; }
};

class TaoAddOn : public TaoBase {
public:
  TaoAddOn() {}
  virtual ~TaoAddOn() {}

  static short RTTI;
  short rtti() const { return RTTI; }
  virtual short rttiAddOn() const { return RTTI; }

  virtual TaoAddOn *newObject() { return new TaoAddOn(); }
  virtual TaoAddOn *newObject(TaoArray *param) { return new TaoAddOn(); }

  virtual void runMethod(string funame, TaoArray *in = 0, TaoArray *out = 0) {}
};

class TaoLibrary;

class TaoCppObject : public TaoBase {
public:
  TaoCppObject() { cppObject = NULL; }
  ~TaoCppObject() {
    if (cppObject)
      delete cppObject;
  }

  TaoLibrary *libHandler;

  string typeName;

  TaoPlugin *cppObject;

  static short RTTI;
  virtual short rtti() const { return RTTI; }

  void print(ostream *out = &cout) {
    if (cppObject)
      cppObject->print(out);
  }

  TaoBase *duplicate() { return this; }
  TcBase *toCppType();
  void fromCppType();
};

class TaoNameSpace : public TaoBase {
public:
  TaoNameSpace() {
    mainModule = 0;
    name = "Null";
  }

  string name;

  static short RTTI;
  virtual short rtti() const;

  TaoModule *mainModule;

  //! Data in the name space:
  map<string, TaoReference *> nsData;
  //! Data in the name space:
  map<string, TaoReference *> nsDataShared;
  //! Modules in the name space:
  map<string, TaoModule *> nsModule;
  //! Classes in the name space:
  map<string, TaoClass *> nsClass;

  //! AddOns in the name space:
  map<string, TaoAddOn *> nsAddOn;
  //! Plugins in the name space:
  map<string, TaoPlugin *> nsPlugin;

  //! Name spaces in the name space:
  map<string, TaoNameSpace *> nameSpace;
  //! C/C++ library in the name space:
  map<void *, TaoLibrary *> nsLibrary;

  TaoClass *findClass(string name, string ns = "");
  TaoModule *findModule(string name, string ns = "");
  TaoAddOn *findAddOn(string name, string ns = "");
  TaoPlugin *findPlugin(string name, string ns = "");
  TaoReference *findData(string name, string ns = "");
  TaoReference *findDataShared(string name, string ns = "");
};

class TaoLibrary : public TaoBase {
  TaoCppObject objCpp;

public:
  TaoLibrary() {}

  static short RTTI;
  virtual short rtti() const;

  TaoCppObject *findPlugin(string name);

  string libName;
  string libPath;
  void *libHandler;

  //! Plugins defined the library:
  map<string, TaoPlugin *> libPlugin;
};

class TaoPluginHandler : public TaoPlugin {
public:
  TaoPluginHandler(){};
  virtual ~TaoPluginHandler(){};

  string libName;

  map<string, TaoPlugin *> myPlugins;

  void run_function(string funame, TaoArray *in = 0, TaoArray *out = 0);
};

#endif
