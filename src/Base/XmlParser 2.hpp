/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/
#ifndef CAP__XmlParser
#define CAP__XmlParser
#include <list>
#include "Aliases.hpp"
#include "Parser.hpp"
//#include "XmlDocument.hpp"

namespace CAP
{

struct XmlAttribute
{
  TString name;
  TString value;
};

struct XmlTag
{
  TString  name;
  std::list<XmlAttribute> attributes;
  long int begin;
  long int end;
  XmlTag*  father;
  XmlTag*  child;
  XmlTag*  prev;
  XmlTag*  next;
};

class XmlDocument;

class XmlParser : public Parser
{
public:

  XmlParser();
  XmlParser(MessageLogger::Severity severity);
  virtual ~XmlParser() {}

  virtual void read(XmlDocument   & _xmlDocument,
                    const String & _inputPath,
                    const String & _inputFileName)  throw (FileException);
  XmlTag* createTag(String& aBuff);

  ClassDef(XmlParser,0)

};



} // namespace CAP

#endif /* XmlParser_hpp */
