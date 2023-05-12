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
#ifndef CAP__XmlDocument
#define CAP__XmlDocument
#include "XmlParser.hpp"

namespace CAP
{

class XmlDocument
{
public:

  friend XmlParser;

  XmlDocument();
  XmlDocument(const String  & _inputPath,
              const String  & _inputFileName);
  XmlDocument(const String  & _inputFile);
  virtual ~XmlDocument();

  virtual void  read(const String  & _inputPath,
                    const String  & _inputFileName) throw (FileException);
  virtual void  read() throw (FileException);

  XmlDocument* getXmlTag(const char * aTagName, const char * aAttName, const char * aAttVal) throw(String);
  String   getXmlAttribute(const XmlTag & tag, const char* aAttName) const throw(String);
  String   getXmlAttribute(const char* aAttName) const throw(String);
  String   getXmlContent(const XmlTag & tag) const throw(String);
  String   getXmlContent() const throw(String);
  String   getXmlTagDescription();
  //void      getXmlVector3D();
  //VectorField* getXmlVector3D();
  bool checkAttributeMatch(const char* aAttName, const char* aAttVal) const;

protected:

  String inputPath;
  String inputFileName;
  XmlTag * topTag;
  XmlTag * currentTag;

  ClassDef(XmlDocument,0)

};



} // namespace CAP

#endif /* XmlDocument */
