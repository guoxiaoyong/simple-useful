
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
#include "taoXml.h"
#include "taoExtdata.h"
#include "taoUtility.h"
#include <algorithm>

bool XmlParser::startElement(const string &name,
                             vector<vector<string>> &attribs,
                             const string &qname, const string &xmlnsURI) {
  return TRUE;
}
bool XmlParser::endElement(const string &name, const string &qname,
                           const string &xmlnsURI) {
  return TRUE;
}
bool XmlParser::pairElement(const string &name, vector<vector<string>> &attribs,
                            const string &content, const string &qname,
                            const string &xmlnsURI) {
  return TRUE;
}
void XmlParser::makeElement(vector<string> &fields) {
  xmlAttribs.clear();
  int size = fields.size();
  if (size == 0) {
    xmlName = xmlQName = xmlnsURI = xmlContent = "";
    return;
  }

  int start = 0;
  if (fields[0] == "/")
    start++;
  if (size < start + 1) {
    cout << "Error: wrong XML format!\n";
    return;
  }
  if (size == start + 1 || fields[start + 1] != ":") {
    xmlName = fields[start];
    xmlQName = xmlName;
    xmlContent = "";
    xmlnsURI = xmlNameSpaces["xmlns_default"];
  } else if (fields[start + 1] == ":") {
    if (size < start + 3) {
      cout << "Error: \n";
      return;
    }
    xmlName = fields[start + 2];
    xmlQName = fields[start] + ":" + xmlName;
    xmlnsURI = xmlNameSpaces[fields[start]];
  }

  vector<string>::iterator equal = find(fields.begin(), fields.end(), "=");
  while (equal != fields.end()) {
    vector<string> oneAtt;
    if (*(equal - 2) == ":") {
      if (*(equal - 3) == "xmlns") {
        xmlNameSpaces[*(equal - 1)] = *(equal + 1);
      } else {
        oneAtt.push_back(*(equal - 3) + ":" + *(equal - 1));
        oneAtt.push_back(*(equal + 1));
        xmlAttribs.push_back(oneAtt);
      }
    } else if (*(equal - 1) == "xmlns") {
      xmlNameSpaces["xmlns_default"] = *(equal + 1);
    } else {
      oneAtt.push_back(*(equal - 1));
      oneAtt.push_back(*(equal + 1));
      xmlAttribs.push_back(oneAtt);
    }
    equal = find(equal + 1, fields.end(), "=");
  }
}
void XmlParser::parse(const string &source) {
  string::size_type pos1, pos3, pos4;

  string::size_type posnew = 0;
  string::size_type it;

  string::size_type len = source.size();

  char ch;
  bool isChars = 0;

  while (posnew < len) {
    vector<string> fields;

    it = posnew;
    isChars = 0;
    // Look for a tag:
    while (it < len && source[it] != '<') {
      it++;
    }
    it++;
    pos1 = it;
    while (it < len) {
      ch = source[it];
      if (!isChars && ch == '>')
        break;
      if (!isChars && (ch == ' ' || ch == '\t' || ch == '\n')) {
        if (it > pos1)
          fields.push_back(source.substr(pos1, it - pos1));
        pos1 = ++it;
        continue;
      } else if (!isChars && (ch == ':' || ch == '/' || ch == '=')) {
        if (it > pos1)
          fields.push_back(source.substr(pos1, it - pos1));
        pos1 = ++it;
        fields.push_back(string(1, ch));
        continue;
      } else if (ch == '\"' || ch == '\'') {
        isChars = !isChars;
        if (!isChars)
          fields.push_back(source.substr(pos1, it - pos1));
        pos1 = ++it;
        continue;
      }
      it++;
    }
    if (it > pos1)
      fields.push_back(source.substr(pos1, it - pos1));
    posnew = it + 1;

    int size = fields.size();
    if (size == 0) {
      cout << "Error: wrong XML format!\n";
      return;
    }

    makeElement(fields);

    if (fields[0] == "/") {
      endElement(xmlName, xmlQName, xmlnsURI);
      continue;
    }

    if (fields[size - 1] != "/") {
      pos3 = it;
      isChars = 0;
      while (it < len) {
        it++;
        ch = source[it];
        if (!isChars && ch == '<')
          break;
        if (ch == '\"')
          isChars = !isChars;
      }
      if (source[it + 1] == '/') {
        pos4 = source.find('>', it);
        if (pos4 == string::npos) {
          cout << "Error: unpaired '<'!\n";
          return;
        }
        xmlContent = source.substr(pos3 + 1, it - pos3 - 1);
        pairElement(xmlName, xmlAttribs, xmlContent, xmlQName, xmlnsURI);
        posnew = pos4 + 1;
        continue;
      }
      posnew = it;
    }

    startElement(xmlName, xmlAttribs, xmlQName, xmlnsURI);
  }
}

TaoXmlParser::TaoXmlParser() {
  startElementHandler = 0;
  endElementHandler = 0;
  pairElementHandler = 0;

  taoName = taoQName = taoContent = taoXmlnsURI = 0;
  taoAttrib = 0;
}
TaoXmlParser::TaoXmlParser(string name, TaoNameSpace *ns) {
  startElementHandler = 0;
  endElementHandler = 0;
  pairElementHandler = 0;
  ns->nsAddOn[name] = this;

  taoName = taoQName = taoContent = taoXmlnsURI = 0;
  taoAttrib = 0;
}
TaoAddOn *TaoXmlParser::newObject() { return (TaoAddOn *)new TaoXmlParser(); }
TaoAddOn *TaoXmlParser::newObject(TaoArray *param) {
  return (TaoAddOn *)new TaoXmlParser();
}
bool TaoXmlParser::startElement(const string &name,
                                vector<vector<string>> &attribs,
                                const string &qname, const string &xmlnsURI) {
  if (!startElementHandler)
    return 1;

  startElementHandler->detachOldParams();
  taoName = TaoTryAllocator<TaoString>::tryAllocate(taoName);
  taoAttrib = TaoTryAllocator<TaoArray>::tryAllocate(taoAttrib);
  taoName->chars = name;

  int size = attribs.size();
  taoAttrib->clear();
  for (int i = 0; i < size; i++) {
    TaoArray *oneAtt = new TaoArray();
    oneAtt->append(new TaoString(attribs[i][0]));
    oneAtt->append(new TaoString(attribs[i][1]));
    taoAttrib->append(oneAtt);
  }

  startElementHandler->passParam(0, taoName);
  startElementHandler->passParam(1, taoAttrib);

  if (startElementHandler->numParams() > 2) {
    taoQName = TaoTryAllocator<TaoString>::tryAllocate(taoQName);
    taoQName->chars = qname;
    startElementHandler->passParam(2, taoQName);
  }

  if (startElementHandler->numParams() > 3) {
    taoXmlnsURI = TaoTryAllocator<TaoString>::tryAllocate(taoXmlnsURI);
    taoXmlnsURI->chars = xmlnsURI;
    startElementHandler->passParam(3, taoXmlnsURI);
  }

  return startElementHandler->run();
}
bool TaoXmlParser::endElement(const string &name, const string &qname,
                              const string &xmlnsURI) {
  if (!endElementHandler)
    return 1;

  endElementHandler->detachOldParams();
  taoName = TaoTryAllocator<TaoString>::tryAllocate(taoName);
  taoName->chars = name;
  endElementHandler->passParam(0, taoName);

  if (endElementHandler->numParams() > 1) {
    taoQName = TaoTryAllocator<TaoString>::tryAllocate(taoQName);
    taoQName->chars = qname;
    endElementHandler->passParam(1, taoQName);
  }

  if (endElementHandler->numParams() > 2) {
    taoXmlnsURI = TaoTryAllocator<TaoString>::tryAllocate(taoXmlnsURI);
    taoXmlnsURI->chars = xmlnsURI;
    endElementHandler->passParam(2, taoXmlnsURI);
  }

  return endElementHandler->run();
}
bool TaoXmlParser::pairElement(const string &name,
                               vector<vector<string>> &attribs,
                               const string &content, const string &qname,
                               const string &xmlnsURI) {
  if (!pairElementHandler)
    return 1;

  pairElementHandler->detachOldParams();

  taoName = TaoTryAllocator<TaoString>::tryAllocate(taoName);
  taoAttrib = TaoTryAllocator<TaoArray>::tryAllocate(taoAttrib);

  taoName->chars = name;

  int size = attribs.size();
  taoAttrib->clear();
  for (int i = 0; i < size; i++) {
    TaoArray *oneAtt = new TaoArray();
    oneAtt->append(new TaoString(attribs[i][0]));
    oneAtt->append(new TaoString(attribs[i][1]));
    taoAttrib->append(oneAtt);
  }

  taoContent = TaoTryAllocator<TaoString>::tryAllocate(taoContent);
  taoContent->chars = content;

  pairElementHandler->passParam(0, taoName);
  pairElementHandler->passParam(1, taoAttrib);
  pairElementHandler->passParam(2, taoContent);

  if (pairElementHandler->numParams() > 3) {
    taoQName = TaoTryAllocator<TaoString>::tryAllocate(taoQName);
    taoQName->chars = qname;
    pairElementHandler->passParam(3, taoQName);
  }

  if (pairElementHandler->numParams() > 4) {
    taoXmlnsURI = TaoTryAllocator<TaoString>::tryAllocate(taoXmlnsURI);
    taoXmlnsURI->chars = xmlnsURI;
    pairElementHandler->passParam(4, taoXmlnsURI);
  }

  return pairElementHandler->run();
}
void TaoXmlParser::runMethod(string funame, TaoArray *inp, TaoArray *outp) {
  if (funame == "setHandler") {
    setHandler(inp);
  } else if (funame == "parse") {
    TaoString *str = (TaoString *)inp->elements[0];
    parseXmlFile(str->chars);
  }
}
void TaoXmlParser::setHandler(TaoArray *inp) {
  TaoBase *start = (TaoBase *)inp->elements[0]->dereference();
  TaoBase *end = (TaoBase *)inp->elements[1]->dereference();
  TaoBase *pair = (TaoBase *)inp->elements[2]->dereference();

  if (start->rtti() == TAO_Module) {
    TaoModule *module = (TaoModule *)start;
    startElementHandler = module->getRoutine();
  }

  if (end->rtti() == TAO_Module) {
    TaoModule *module = (TaoModule *)end;
    endElementHandler = module->getRoutine();
  }

  if (pair->rtti() == TAO_Module) {
    TaoModule *module = (TaoModule *)pair;
    pairElementHandler = module->getRoutine();
  }
}
void TaoXmlParser::parseXmlFile(const string &fname) {
  std::ifstream fin(fname.c_str());

  char fline[10000];
  int num;
  while (1) {
    num = 0;
    string source;
    while (num < 100000 && !fin.eof()) {
      //			if(num%10000==0)cout<<"fline\t"<<fline<<endl;
      fin.getline(fline, 10000, '\n');
      if (source.size() >= 0.9 * source.capacity())
        source.reserve(source.capacity() * 2);

      source.append(fline);
      num++;
    }
    string sline = fline;
    while (sline.find("</") == string::npos && !fin.eof()) {
      fin.getline(fline, 10000, '\n');
      if (source.size() >= 0.9 * source.capacity())
        source.reserve(source.capacity() * 2);
      source.append(fline);
      sline = fline;
    }
    //		cout<<"100000 parsed\n";
    parse(source);
    if (fin.eof())
      break;
  }
}
