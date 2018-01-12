
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
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "taoConst.h"
#include "taoRecycler.h"
#include "taoScript.h"
#include "taoXml.h"

using namespace std;

map<int, string> __Tao_TypeToName;

void type_to_name() {
  //	__Tao_TypeToName[TAO_Base]="TAO_Base";
  __Tao_TypeToName[-1] = "NullType";
  __Tao_TypeToName[TAO_Reference] = "TaoRefer";
  __Tao_TypeToName[TAO_Number] = "TaoNumber";
  __Tao_TypeToName[TAO_String] = "TaoString";
  __Tao_TypeToName[TAO_Object] = "TaoObject";
  __Tao_TypeToName[TAO_Fin] = "TaoFin";
  __Tao_TypeToName[TAO_Fout] = "TaoFout";
  __Tao_TypeToName[TAO_Array] = "TaoArray";
  __Tao_TypeToName[TAO_Hash] = "TaoHash";
  __Tao_TypeToName[TAO_Library] = "TaoLibrary";
  __Tao_TypeToName[TAO_NameSpace] = "TaoNameSpace";
  __Tao_TypeToName[TAO_RealArray] = "TaoRealArray";
  __Tao_TypeToName[TAO_ByteArray] = "TaoByteArray";
  __Tao_TypeToName[TAO_ShortArray] = "TaoShortArray";
  __Tao_TypeToName[TAO_IntArray] = "TaoIntArray";
  __Tao_TypeToName[TAO_FloatArray] = "TaoFloatArray";
  __Tao_TypeToName[TAO_DoubleArray] = "TaoDoubleArray";
  __Tao_TypeToName[TAO_CompArray] = "TaoComplexArray";
}
int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "\nUsage: tao source.tao\n\n";
    return 1;
  }
  int idsrc = 0;
  int i;
  for (i = 1; i < argc; i++) {
    char *ch = strstr(argv[i], ".tao");
    if (ch && strcmp(ch, ".tao") == 0) {
      idsrc = i;
      break;
    }
  }
  int k = idsrc;
  if (k <= 0)
    k = argc;
  for (i = 1; i < k; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "-version") == 0) {
        cout << "======================================="
             << "==================================\n\n";
        cout << "  Tao Language for scripting and computing.\n";
        cout << "  Version 0.9.0 beta\n\n";
        cout << "  Copyright(C) 2004-2005, Fu Limin\n";
        cout << "  Contact: fu.limin.tao@gmail.com\n\n";
        cout << "  Tao may be copied only under the terms of GNU General "
                "Public License.\n\n";
        cout
            << "  Tao Language Home Page: http://taoscript.sourceforge.net\n\n";
        cout << "  Tao Language Project at sourceforge.net:\n";
        cout << "  http://sourceforge.net/projects/taoscript/\n\n";
        cout << "======================================="
             << "==================================\n";
      } else {
        cout << "Unknown option \"" << argv[i] << "\".\n";
      }
    } else {
      cout << "Unknown option \"" << argv[i] << "\".\n";
    }
  }
  if (idsrc <= 0)
    return 1;

  TaoArray comarg;
  comarg.refCount = 100;
  for (i = idsrc + 1; i < argc; i++) {
    comarg.append(new TaoString(string(argv[i])));
  }

  type_to_name();

  // Set garbage collector:
  TaoRecycler recycler;
  TaoBase::recycler = &recycler;

  // Interpreter:
  TaoScript tao;

  // Read source:
  tao.readSource(argv[idsrc]);

  // Set main namespace:
  TaoNameSpace ns;
  ns.nsData["COMARG"] = new TaoConstRefer(&comarg);
  tao.setNameSpace(&ns);

  // Init XML module:
  TaoXmlParser xmlParser("TaoXmlParser", &ns);

  return tao.interprete();
}
