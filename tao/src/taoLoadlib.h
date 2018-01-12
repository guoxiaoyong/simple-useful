
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
#ifndef TAO_LOADLIB_H
#define TAO_LOADLIB_H

#include "taoPlugin.h"

#ifdef TAO_LINUX
#include <dlfcn.h>
#elif TAO_WIN || TAO_WINDOWS || WIN32
#include <windows.h>
#endif

class TaoLibrary;

extern bool Tao_DynLoad_Lib(TaoLibrary *lib, string libpath);

extern void *Tao_DynLink_Symbol(void *lib, string fun);

#endif
