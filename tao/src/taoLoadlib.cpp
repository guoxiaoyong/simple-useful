
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
#include "taoLoadlib.h"
#include "taoExtdata.h"
#include "taoXml.h"

TaoPluginHandler defaultHandler;

extern map<int, string> __Tao_TypeToName;

bool Tao_DynLoad_Lib(TaoLibrary *lib, string libpath) {

  lib->libPath = libpath;
  void *handle = 0;

#ifdef TAO_LINUX

  handle = dlopen(libpath.c_str(), RTLD_NOW | RTLD_GLOBAL);
  if (!handle) {
    cout << dlerror() << "\n";
    return 0;
  }

#elif TAO_WIN || TAO_WINDOWS || WIN32

  handle = LoadLibrary(libpath.c_str());
  if (!handle)
    return 0;

#endif

  lib->libHandler = handle;

  TaoPluginProxy *pluginProxy;
  pluginProxy =
      (TaoPluginProxy *)Tao_DynLink_Symbol(handle, "_Tao_Plugin_Proxy");

  if (pluginProxy) {
    TaoPlugin **plugins = pluginProxy->getPlugins();
    char **names = pluginProxy->getNames();
    ;
    int num = pluginProxy->getPlgNumber();
    ;

    for (int i = 0; i < num; i++) {
      lib->libPlugin[names[i]] = plugins[i];
      __Tao_TypeToName[plugins[i]->rtti() + 10000] = string(names[i]);
    }
  } else {
    cout << "Warning: no symbol \"_Tao_Plugin_Proxy\" is found in \"" << libpath
         << "\".\n";
    cout << "\tNo plugin can be imported.\n";
    return 0;
  }

  return 1;
}

void *Tao_DynLink_Symbol(void *lib, string fun) {
#ifdef TAO_LINUX
  return dlsym(lib, fun.c_str());
#elif TAO_WIN || TAO_WINDOWS || WIN32
  return (void *)GetProcAddress((HMODULE)lib, fun.c_str());
#endif
}
