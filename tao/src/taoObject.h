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
#ifndef TAO_OBJECT_H
#define TAO_OBJECT_H

#include "taoDatatype.h"

//	In TaoObject, data are saved in TaoObject::dataMember
//	whereas, TaoObject::myData contains pointers to the dataMember
//	of its own and its super objects.
class TaoClass;
class TaoRoutine;
class TaoPhrase;
class TaoPreRoutine;
class TaoArithBase;
//! no comments!
class TaoObject : public TaoBase {
  TaoObject *subObject;
  TaoObject *supObject;

  vector<TaoReference *> dataMember;

public:
  TaoObject() {
    supObject = 0;
    subObject = 0;
  }
  TaoObject(TaoClass *klass);
  virtual ~TaoObject();

  TaoClass *myClass;

  void setInitParams(vector<TaoArithBase *> &parAriths);
  void init(TaoArray *params);

  TaoObject *getSuperObject() { return supObject; }

  TaoReference *getObjectData(const string &name, int type = 2);
  TaoReference *searchObjectData(const string &name);

  static short RTTI;
  virtual short rtti() const;

  void scan_member_data(vector<TaoBase *> &cand, int mp);

  TaoBase *duplicate();
};

#endif
