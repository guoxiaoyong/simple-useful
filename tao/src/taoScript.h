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

#ifndef TAO_SCRIPT_H
#define TAO_SCRIPT_H

#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

//#include"langbase.h"
#include "taoClass.h"
#include "taoDatatype.h"
#include "taoExtdata.h"
#include "taoPhrase.h"

using namespace std;

//! no comments!
class TaoScript : public TaoModuleBase {
  vector<string> codeParsed;
  vector<int> posLine;

  string codeSource;

  bool makeRoutines();

public:
  TaoScript();

  void readSource(const char *fname);
  void setSource(string &source) { codeSource = source; }

  bool compile(TaoNameSpace *nsIn = 0, TaoNameSpace *nsOut = 0,
               bool replace = 1);
  bool interprete();
};

#endif
