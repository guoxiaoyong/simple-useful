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
#include "taoObject.h"
#include "taoArith.h"
#include "taoClass.h"
#include "taoConst.h"

short TaoObject::RTTI = TAO_Object;
short TaoObject::rtti() const { return RTTI; }

/*!
        Create objects for class "klass" and its super-classes,
        and each object contains a copy of data and a set of
        pointers to the method members of its class. It's necessary
        to create an object for each super-class, and put data in
        that object, instead of creating one object and putting all
        data in this object. Because the method have no direct access
        to the data belonging to its sub-classes, by putting all data
        in one object will complicate the process of determining the
        ownership of data and method.
        After creating one object for each super-class, each object
        is responsible for the work-to-done(mostly remapping class data
        to its method members) when calling a class method
        and accessing a class data.
*/
TaoObject::TaoObject(TaoClass *klass) {
  myClass = klass;

  subObject = 0;
  supObject = 0;

  //	Each object contains a copy of data of its class:
  dataMember.clear();
  map<string, TaoReference *>::iterator itdata;
  for (itdata = myClass->dataMember.begin();
       itdata != myClass->dataMember.end(); ++itdata) {
    TaoReference *p = (TaoReference *)itdata->second->duplicate();
    p->refCount++;
    dataMember.push_back(p);
  }
  //	Create objects for class "klass" and its super-classes:
  TaoObject *obj = this;
  while (klass->superClass) {
    obj->supObject = new TaoObject(klass->superClass);
    obj->supObject->refCount++;
    obj->supObject->subObject = obj;
    klass = klass->superClass;
    obj = obj->supObject;
  }
}
TaoObject::~TaoObject() { dataMember.clear(); }
/*!
        Finding a method in a class and its super-classes. Method type
        (private, protected or public) is saved in "type", method pointer
        is saved in "rout", and the object of the super-class containing
        that method is referenced by "obj". If no method is found, 0 is
        returned, otherwise 1 is returnd.
*/
TaoReference *TaoObject::getObjectData(const string &name, int type) {
  map<string, int>::iterator iter;
  iter = myClass->dataIndex.find(name);
  if (iter != myClass->dataIndex.end()) {
    if (myClass->dataType[name] >= type)
      return dataMember[iter->second];
  }
  return 0;
}
TaoReference *TaoObject::searchObjectData(const string &name) {
  TaoReference *pbase = getObjectData(name, 1);
  if (pbase)
    return pbase;
  TaoObject *pobj = supObject;
  while (pobj) {
    pbase = pobj->getObjectData(name, 2);
    if (pbase)
      return pbase;
    pobj = pobj->supObject;
  }
  return 0;
}
void TaoObject::setInitParams(vector<TaoArithBase *> &parAriths) {
  if (supObject)
    supObject->setInitParams(parAriths);
  for (size_t j = 0; j < parAriths.size(); j++) {
  }
}
void TaoObject::init(TaoArray *params) {
  if (supObject)
    supObject->init(params);

  myClass->constructor->setHostObject(this);
  myClass->constructor->mapObjectData();

  myClass->constructor->detachOldParams();

  for (size_t i = 0; i < params->elements.size(); i++) {
    if (subObject) {
      // Passing initialization parameters from the constructor
      // of its sub-class:
      int j = subObject->myClass->idParams[i];
      if (j >= 0)
        myClass->constructor->passParam(j, params->elements[i]);
    } else {
      myClass->constructor->passParam(i, params->elements[i]);
    }
  }

  myClass->constructor->run();
}
void TaoObject::scan_member_data(vector<TaoBase *> &cand, int mp) {
  if (supObject) {
    if (!supObject->scanned) {
      supObject->scanned = 1;
      supObject->refCount += mp;
      cand.push_back(supObject);
      //			supObject->scan_member_data(cand,mp);
    }
  }
  map<string, TaoReference *>::iterator iter;
  size_t i;
  for (i = 0; i < dataMember.size(); i++) {
    dataMember[i]->refCount += mp;
    if (!dataMember[i]->scanned) {
      dataMember[i]->scanned = 1;
      cand.push_back(dataMember[i]);
    }
  }
}
TaoBase *TaoObject::duplicate() {
  TaoObject *pnew = new TaoObject(); // myClass,rtdata);
  // if(rtdata) rtdata->rtdVector.push_back(pnew);

  pnew->myClass = myClass;
  if (supObject) {
    pnew->supObject = (TaoObject *)supObject->duplicate();
    pnew->supObject->refCount++;
    pnew->supObject->subObject = pnew;
  }

  for (unsigned int i = 0; i < dataMember.size(); i++) {
    TaoReference *p = (TaoReference *)dataMember[i]->duplicate();
    p->refCount++;
    pnew->dataMember.push_back(p);
  }

  //	pnew->init(rtdata);
  return (TaoBase *)pnew;
}
