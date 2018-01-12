
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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
////////////////////////////////////////////////////////////////////////////
#ifndef TAO_PLUGIN_H
#define TAO_PLUGIN_H

#include <iostream>

///////////////////////////////////////////////////////////////////////////
//
//	Please read the following comments carefully
//	to see how to define a Tao plugin class.
//
///////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define TAO_DLL __declspec(dllexport)
#else
#define TAO_DLL
#endif

//! Constructor macro to register a plugin instance in "pluginProxy".
//! This macro should be put in the beginning of each TaoPlugin subclass.
#define TAO_PLUGIN(x)                                                          \
public:                                                                        \
  x(char *y) { _Tao_Plugin_Proxy.addPlugin(this, y); }                         \
private:

//! Macro to declare "pluginProxy".
#define TAO_PLUGIN_PROXY TAO_DLL TaoPluginProxy _Tao_Plugin_Proxy;

//! Macro of macros.
//! Put it in your plugin program
#define INIT_TAO_PLUGIN TAO_PLUGIN_PROXY

//! The last thing to do is to
//! declare an instance of each TaoPlugin subclass.
//! see the following example.

#include "taoCpptype.h"

using namespace std;

class TaoPlugin;

//! Used to export plugins defined in a C++ module to Tao.
/*!
C++ Module developers should not need this class.
*/
class TaoPluginProxy {
  TaoPlugin **plugins;
  char **names;

  int size;
  int nplg;

public:
  TaoPluginProxy() { size = nplg = 0; }

  void addPlugin(TaoPlugin *plg, char *name) {
    if (nplg + 2 > size) {
      size += 100;
      plugins = (TaoPlugin **)realloc(plugins, size * sizeof(TaoPlugin *));
      names = (char **)realloc(names, size * sizeof(char *));
    }
    plugins[nplg] = plg;
    names[nplg] = name;
    nplg++;
  }
  TaoPlugin **getPlugins() { return plugins; }
  char **getNames() { return names; }
  int getPlgNumber() { return nplg; }
};

#ifndef WIN32
extern TaoPluginProxy _Tao_Plugin_Proxy;
#endif
//! TaoPlugin: the base class for Tao plugins.
/*!
This is the key class for the feature of dynamical loading C++
modules (plugins) in Tao Language. All classes derived from
TaoPlugin are loadable from Tao script.

The basic idea of the plugin technique used in Tao is that,
an instance is created for each plugin classe when the module
defining them is loaded, and these instances register themselves
in a global variable defined by the main program (Tao Interpreter),
so that these instances can be used by the main program to create
other instances by using function "newObject()".
The plugins are registered throught class TaoPluginProxy.

Example:

\code
myplugin.h:

#include"taoPlugin.h"

class MyPlugin : public TaoPlugin
{

        // A constructor:
        TAO_PLUGIN(MyPlugin);

        public:
        // Reimplement some virtual functions which serve as
        // interfaces to the Tao interpreter.

        // Other functions.
};

myplugin.cpp:

#include"myplugin.h"

INIT_TAO_PLUGIN;

MyPlugin("MyPlugin"); // Register class MyPlugin with name "MyPlugin".

// Other codes.
\endcode
*/
class TaoPlugin : public TcBase {
public:
  TaoPlugin() {}
  virtual ~TaoPlugin() {}

  //! Get object type, to tell this is a plugin.
  short type() const { return TAO_PLUGIN; }
  //! Get object Running Time Type Identification.
  //! To tell which type of plugin it is.
  virtual short rtti() const { return TAO_PLUGIN; }

  //! Reimplemented to create user defined plugin.
  virtual TaoPlugin *newObject(TcArray *param = 0) { return new TaoPlugin(); }
  //! Reimplemented this method to invoke other method.
  virtual void runMethod(const char *funame, TcArray *in = 0,
                         TcArray *out = 0) {
    cout << "TaoPlugin::runMethod() is not reimplemented.\n";
    // To reimplement, add:
    /*
    if( strcmp(funame,"test")==0 ){
            test(...);
    }else if(...){
            ...
    }
    */
  }
  //! Reimplement this method if one want to print something
  //! when the plugin is printed by Tao function print().
  virtual void print(ostream *out = 0, bool reserved = 0) {
    if (out == 0)
      out = &cout;
    *out << "TaoPlugin" << this;
  }
  //! Reimplement this method to give short description of
  //! which methods can be called throught runMethod().
  virtual void help(ostream *out = 0) {
    if (out == 0)
      out = &cout;
    *out << "No method is provided through TaoPlugin::runMethod().\n";
  }
};

#endif
