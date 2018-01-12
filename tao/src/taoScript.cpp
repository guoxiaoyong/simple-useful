
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
#include "taoScript.h"
#include "ctype.h"
#include "taoLexer.h"
#include "taoUtility.h"
#include <cctype>

TaoScript::TaoScript() {}
void TaoScript::readSource(const char *fname) {
  srcFName = fname;
  str_pos slash = srcFName.rfind('/');
  if (slash != string::npos) {
    srcFName = srcFName.substr(slash + 1);
  }

  char fline[10000];
  ifstream fincpp(fname);
  if (!fincpp.is_open()) {
    cout << "Error: can't open file \"" << fname << "\".\n";
    return;
  }
  while (!fincpp.eof()) {
    fincpp.getline(fline, 10000, '\n');
    //	cout<<fline<<"\n";
    codeSource.append(fline);
    codeSource.append("\n");
  }
  fincpp.close();
}
bool TaoScript::makeRoutines() {
  //	bool	isFun=0;
  size_t start = 0;
  int i;
  int right = 0;
  int rb;

  vector<TaoClass *> ynClassVector;

  TaoModule *modMain = new TaoModule(); // this);
  modMain->setNameSpace(inNameSpace, outNameSpace);
  //! Only the variables of a main routine is exported.
  modMain->setVarToNameSpace(1);

  if (outNameSpace->mainModule) {
    cout << "Warning: main routine of namespace " << outNameSpace->name
         << " is changed!\n";
  }
  outNameSpace->mainModule = modMain;
  outNameSpace->nsModule["main"] = modMain;

  while (start >= 0 && start < codeParsed.size()) {
    if (codeParsed[start] == "routine") {
      if (codeParsed[start + 2] == "::") {
        //	This is for functions define outside the class body:
        right = wordPair(codeParsed, "{", "}", start);
        if (right < 0) {
          error_unpaired(srcFName, posLine[start], "{}");
          return 0;
        }

        TaoClass *cluss = outNameSpace->findClass(codeParsed[start + 1]);

        if (!cluss) {
          error_undefined(srcFName, posLine[start + 1],
                          "class \"" + codeParsed[start + 1] + "\"");
          return 0;
        }
        string fun = codeParsed[start];
        cluss->appendExtraWord(fun, posLine[start]);
        for (int i = start + 3; i <= right; i++)
          cluss->appendExtraWord(codeParsed[i], posLine[i]);
      } else {
        //	For no-class-member functions:
        TaoModule *module = new TaoModule(); // this);
        module->setNameSpace(inNameSpace, outNameSpace);

        module->name = codeParsed[start + 1];
        // module->routype=codeParsed[start];

        // nameSpace->nsModule[codeParsed[start+1]]=module;
        outNameSpace->nsModule[codeParsed[start + 1]] = module;

        rb = wordPair(codeParsed, "(", ")", start);
        if (rb < 0) {
          error_unpaired(srcFName, posLine[start], "()");
          return 0;
        }
        for (i = start + 3; i < rb; i++) {
          if (codeParsed[i] != ",")
            module->appendParams(codeParsed[i]);
        }
        if (codeParsed[rb + 1] != ";") {
          right = wordPair(codeParsed, "{", "}", start);
          if (right < 0) {
            error_unpaired(srcFName, posLine[start], "{}");
            return 0;
          }
          for (i = rb + 2; i < right; i++)
            module->appendWord(codeParsed[i], posLine[i]);
        }
      }

      start = right + 1;
    } else if (codeParsed[start] == "class" && codeParsed[start + 2] == ":") {

      //	For class with is a subclass of another:

      TaoClass *cluss = new TaoClass(codeParsed[start + 1]);
      cluss->setNameSpace(inNameSpace, outNameSpace);

      TaoClass *super = 0;
      if (codeParsed[start + 4] == "::") {
        super = outNameSpace->findClass(codeParsed[start + 5],
                                        codeParsed[start + 3]);
        if (!super)
          super = inNameSpace->findClass(codeParsed[start + 5],
                                         codeParsed[start + 3]);
      } else {
        super = outNameSpace->findClass(codeParsed[start + 3]);
        if (!super)
          super = inNameSpace->findClass(codeParsed[start + 3]);
      }

      if (!super) {
        cout << "Error: parent class haven't been defined!\n";
        return 0;
      }
      //	Set a reference to its super classes:
      cluss->superClass = super;

      //	Register this class to "nameSpace":
      outNameSpace->nsClass[codeParsed[start + 1]] = cluss;

      ynClassVector.push_back(cluss);
      int left = findWord(codeParsed, "{", start);
      right = wordPair(codeParsed, "{", "}", left);
      if (right < 0) {
        error_unpaired(srcFName, posLine[start], "{}");
        return 0;
      }
      for (int i = left + 1; i < right; i++)
        cluss->appendClassWord(codeParsed[i], posLine[i]);

      start = right + 1;
    } else if (codeParsed[start] == "class") {
      //	For class without parent:
      TaoClass *cluss = new TaoClass(codeParsed[start + 1]);
      cluss->setNameSpace(inNameSpace, outNameSpace);

      outNameSpace->nsClass[codeParsed[start + 1]] = cluss;
      ynClassVector.push_back(cluss);

      right = wordPair(codeParsed, "{", "}", start);
      if (right < 0) {
        error_unpaired(srcFName, posLine[start], "{}");
        return 0;
      }
      for (int i = start + 3; i < right; i++)
        cluss->appendClassWord(codeParsed[i], posLine[i]);

      start = right + 1;
      //		}else if(codeParsed[start]==":" &&
      //codeParsed[start+1]==":"){
      //			start=right+1;
    } else {
      modMain->appendWord(codeParsed[start], posLine[start]);
      start++;
    }
  }

  // Parse main program first, since it may need to
  // "import" or "load" modules so that the classes
  // defined in the modules are usable for other routines.
  modMain->name = "main";
  modMain->srcFName = srcFName;
  if (!modMain->parsed) {
    if (!modMain->parsePhrase())
      return 0;
  }

  //	Compile parsed codes into Tao phrases, which can be executed
  //	by Tao.
  vector<TaoClass *>::iterator itkls;
  for (itkls = ynClassVector.begin(); itkls != ynClassVector.end(); ++itkls) {
    if (!(*itkls)->parsed) {
      (*itkls)->srcFName = srcFName;
      if (!(*itkls)->parseMember())
        return 0;
    }
  }

  map<string, TaoModule *>::iterator iter;
  for (iter = outNameSpace->nsModule.begin();
       iter != outNameSpace->nsModule.end(); ++iter) {
    if (!iter->second->parsed) {
      iter->second->srcFName = srcFName;
      iter->second->parseParams();
      if (!iter->second->parsePhrase())
        return 0;
    }
  }

  return 1;
}
bool TaoScript::compile(TaoNameSpace *nsIn, TaoNameSpace *nsOut, bool replace) {
  if (nsIn) {
    inNameSpace = nsIn;
    if (nsOut)
      outNameSpace = nsOut;
    else
      outNameSpace = nsIn;
  }

  TaoLexer lexer;

  if (!lexer.parseCode(codeSource, replace))
    return 0;

  codeSource.resize(0);

  codeParsed.swap(lexer.getParsedCode());
  posLine.swap(lexer.getPosLine());

  if (!makeRoutines())
    return 0;

  codeParsed.clear();

  return 1;
}
bool TaoScript::interprete() {
  if (!compile()) {
    cout << "Compiling interrupted!\n";
    return 0;
  }

  TaoRoutine *mainRoutine = outNameSpace->mainModule->getRoutine();
  bool success = mainRoutine->run();
  return success;
}
