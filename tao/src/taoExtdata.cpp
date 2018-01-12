
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
#include "taoExtdata.h"
#include "taoConst.h"

short TaoFin::RTTI = TAO_Fin;
short TaoFin::rtti() const { return RTTI; }
short TaoFout::RTTI = TAO_Fout;
short TaoFout::rtti() const { return RTTI; }
short TaoDataRefer::RTTI = TAO_DataRefer;
short TaoDataRefer::rttiRefer() const { return RTTI; }
short TaoParamRefer::RTTI = TAO_ParamRefer;
short TaoParamRefer::rttiRefer() const { return RTTI; }
short TaoShareRefer::RTTI = TAO_ShareRefer;
short TaoShareRefer::rttiRefer() const { return RTTI; }
short TaoConstRefer::RTTI = TAO_ConstRefer;
short TaoConstRefer::rttiRefer() const { return RTTI; }
short TaoNameSpace::RTTI = TAO_NameSpace;
short TaoNameSpace::rtti() const { return RTTI; }
short TaoLibrary::RTTI = TAO_Library;
short TaoLibrary::rtti() const { return RTTI; }
short TaoAddOn::RTTI = TAO_AddOn;
short TaoCppObject::RTTI = TAO_CppObject;
///////////////////////////////////////////////////////
///////////////////////////////////////////////

TaoDataRefer::TaoDataRefer(TaoBase *p) : TaoReference(p) {}
TaoParamRefer::TaoParamRefer(TaoBase *p) : TaoReference(p) {}
TaoShareRefer::TaoShareRefer(TaoBase *p) : TaoReference(p) {}
TaoConstRefer::TaoConstRefer(TaoBase *p) : TaoReference(p) {}

////////////////////////////////////////////////
TcBase *TaoCppObject::toCppType() {
  /*
  if( !cpptype ) cpptype=new TcObject;
  cpptype->object=cppObject;

  if(recycler) recycler->pushTcObject( cpptype );
*/
  cppObject->reserved = this;
  return cppObject;
}
void TaoCppObject::fromCppType() {
  //	if( !cpptype ) return;

  //	if(recycler) recycler->pushTcObject( cpptype );
  //	cpptype=0;
}

TaoClass *TaoNameSpace::findClass(string name, string ns) {
  if (ns != "") {
    if (nameSpace.find(ns) != nameSpace.end()) {
      TaoNameSpace *nspace = nameSpace[ns];
      return nspace->findClass(name);
    } else {
      cout << "No such name space\n";
    }
  } else {
    if (nsClass.find(name) != nsClass.end()) {
      return nsClass[name];
    }
  }
  return 0;
}
TaoModule *TaoNameSpace::findModule(string name, string ns) {
  if (ns != "") {
    if (nameSpace.find(ns) != nameSpace.end()) {
      TaoNameSpace *nspace = nameSpace[ns];
      return nspace->findModule(name);
    } else {
      cout << "No such name space\n";
    }
  } else {
    if (nsModule.find(name) != nsModule.end()) {
      return nsModule[name];
    }
  }
  return 0;
}
TaoAddOn *TaoNameSpace::findAddOn(string name, string ns) {
  if (ns != "") {
    if (nameSpace.find(ns) != nameSpace.end()) {
      TaoNameSpace *nspace = nameSpace[ns];
      return nspace->findAddOn(name);
    } else {
      cout << "No such name space\n";
    }
  } else {
    if (nsAddOn.find(name) != nsAddOn.end()) {
      return nsAddOn[name];
    }
  }
  return 0;
}
TaoPlugin *TaoNameSpace::findPlugin(string name, string ns) {
  if (ns != "") {
    if (nameSpace.find(ns) != nameSpace.end()) {
      TaoNameSpace *nspace = nameSpace[ns];
      return nspace->findPlugin(name);
    } else {
      cout << "No such name space\n";
    }
  } else {
    if (nsPlugin.find(name) != nsPlugin.end()) {
      return nsPlugin[name];
    }
  }
  return 0;
}
TaoReference *TaoNameSpace::findData(string name, string ns) {
  if (ns != "") {
    if (nameSpace.find(ns) != nameSpace.end()) {
      TaoNameSpace *nspace = nameSpace[ns];
      return nspace->findData(name);
    } else {
      cout << "No such name space\n";
    }
  } else {
    if (nsData.find(name) != nsData.end()) {
      return nsData[name];
    }
  }
  return 0;
}
TaoReference *TaoNameSpace::findDataShared(string name, string ns) {
  if (ns != "") {
    if (nameSpace.find(ns) != nameSpace.end()) {
      TaoNameSpace *nspace = nameSpace[ns];
      return nspace->findDataShared(name);
    } else {
      cout << "No such name space\n";
    }
  } else {
    if (nsDataShared.find(name) != nsDataShared.end()) {
      return nsDataShared[name];
    }
  }
  return 0;
}

TaoCppObject *TaoLibrary::findPlugin(string name) {
  objCpp.refCount = 1000;
  map<string, TaoPlugin *>::iterator itplg = libPlugin.find(name);
  objCpp.cppObject = itplg->second;

  return &objCpp;
}

void TaoPluginHandler::run_function(string funame, TaoArray *in,
                                    TaoArray *out) {
  if (funame == "show") {
    cout << "======================================================\n";
    cout << "<<info>> C++ classes available from module \"" + libName + "\":\n";
    cout << ">> Class Name:\tShort Description\n>>\n";
    map<string, TaoPlugin *>::iterator iter;
    for (iter = myPlugins.begin(); iter != myPlugins.end(); iter++) {
      cout << ">> " + iter->first << ": ";
      iter->second->help();
      cout << "\n";
    }
    cout << ">>\n";
  }
}
