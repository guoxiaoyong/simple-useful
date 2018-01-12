
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
/*
XmlParser, A simple XML parser class.
*/

#ifndef TAO_XMLPARSER_H
#define TAO_XMLPARSER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "taoExtdata.h"
#include "taoRoutine.h"

#define TRUE 1
#define FALSE 0

using namespace std;

// struct xmlAttrib{
//}

//!	A simple XML parser class, using an event-driven mechanism to provide
//!the user with document information.
/*!
A simple XML parser class, using an event-driven mechanism to provide
the user with document information. This class is not aimed to provide
an universal parser for all kinds of XML documents. Currently it's just
a base class for parsing Gene Ontology XML files, without reporting the
namespaces and all features to the user for efficiency consideration.
*/
typedef string::size_type str_pos;

class XmlParser {
  string xmlName, xmlQName, xmlnsURI, xmlContent;

  vector<vector<string>> xmlAttribs;

  map<string, string> xmlNameSpaces;

public:
  XmlParser(){};
  virtual ~XmlParser(){};

  virtual bool startElement(const string &name, vector<vector<string>> &attribs,
                            const string &qname, const string &xmlnsURI);
  virtual bool endElement(const string &name, const string &qname,
                          const string &xmlnsURI);
  virtual bool pairElement(const string &name, vector<vector<string>> &attribs,
                           const string &content, const string &qname,
                           const string &xmlnsURI);

  void parse(const string &source);

  void makeElement(vector<string> &fields);
};

class TaoNameSpace;

enum TaoAddOnTypes {
  TAO_XML_PARSER = 1,
};

class TaoXmlParser : public TaoAddOn, public XmlParser {
  TaoRoutine *startElementHandler;
  TaoRoutine *endElementHandler;
  TaoRoutine *pairElementHandler;

  TaoString *taoName;
  TaoString *taoQName;
  TaoString *taoContent;
  TaoString *taoXmlnsURI;
  TaoArray *taoAttrib;

public:
  TaoXmlParser();
  TaoXmlParser(string name, TaoNameSpace *ns = 0);
  virtual ~TaoXmlParser(){};

  TaoAddOn *newObject();
  TaoAddOn *newObject(TaoArray *param);

  short rttiAddOn() { return TAO_XML_PARSER; }

  bool startElement(const string &name, vector<vector<string>> &attribs,
                    const string &qname, const string &xmlnsURI);
  bool endElement(const string &name, const string &qname,
                  const string &xmlnsURI);
  bool pairElement(const string &name, vector<vector<string>> &attribs,
                   const string &content, const string &qname,
                   const string &xmlnsURI);

  void runMethod(string funame, TaoArray *inp = 0, TaoArray *outp = 0);

  void setHandler(TaoArray *inp);
  void parseXmlFile(const string &fname);
};

#endif
