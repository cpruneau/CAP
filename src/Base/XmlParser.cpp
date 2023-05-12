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
#include "XmlParser.hpp"
#include "XmlDocument.hpp"
using CAP::Parser;
using CAP::XmlParser;
using CAP::MessageLogger;

ClassImp(CAP::XmlParser);

XmlParser::XmlParser()
:
Parser(Info)
{ }

XmlParser::XmlParser(MessageLogger::Severity _severity)
:
Parser(_severity)
{ }


void XmlParser::read(XmlDocument  & _xmlDocument,
                     const String & _inputPath,
                     const String & _inputFileName)
{
  long int filePosition;
  XmlTag* newTag;
  TString textBuffer;
  char    chr;
  int     eofFlag;

  std::ifstream & inputFile = openInputFile(_inputPath,_inputFileName,".xml","");;
  if (reportDebug(__FUNCTION__))
    cout << "Parsing file "<< _inputPath + _inputFileName << endl;
  inputFile.seekg(0, std::ios::beg);
  while(!inputFile.eof())
    {
    textBuffer = "";
    eofFlag = 0;
    // find a TAG
    do {
      inputFile.get(chr);
      if(inputFile.eof())
        {
        eofFlag = -1; break;
        }
    } while (chr != '<');
    // no tag
    if(eofFlag==-1) break;

    // copy a TAG
    //  inputFile.unget();
    do {
      textBuffer += chr;
      if(chr=='[')
        { // DTD list
          do
            {
            inputFile.get(chr);
            textBuffer += chr;
            } while (chr!=']');
        }
      if(textBuffer.BeginsWith("<!--")) { // comment
        do {
          inputFile.get(chr);
          textBuffer += chr;
        } while ( !( textBuffer.EndsWith("--") && (inputFile.peek()=='>') ) );
      }
      inputFile.get(chr);
    } while ( (chr!='>') );

    textBuffer += chr;
    textBuffer.ReplaceAll("\n"," ");
    filePosition = inputFile.tellg();
    // analyze TAG
    if(textBuffer.BeginsWith("<?") && textBuffer.EndsWith("?>"))
      {
      if (reportDebug(__FUNCTION__))
        cout << "Xml declaration : "<<textBuffer<<"  skipping" << endl;
      }
    else if(textBuffer.BeginsWith("<!--") && textBuffer.EndsWith("-->"))
      {
      if (reportDebug(__FUNCTION__))
        cout << "Xml commentary  : "<<textBuffer<<"  skipping" << endl;
      }
    else if(textBuffer.BeginsWith("<!"))
      {
      if (reportDebug(__FUNCTION__))
        cout << "Xml DTD         : "<<textBuffer<<"  skipping" << endl;
      }
    else if(textBuffer.BeginsWith("</"))
      {
      // CLOSE TAG
      textBuffer.ReplaceAll("</","");
      textBuffer.ReplaceAll(">","");
      if(_xmlDocument.currentTag->end !=-1)
        _xmlDocument.currentTag = _xmlDocument.currentTag->father;
      // TAG content ends in file
      _xmlDocument.currentTag->end = filePosition - (textBuffer.Length() + 3);
      // Check if TAG name matches
      if(textBuffer.CompareTo(_xmlDocument.currentTag->name))
        {
        if (reportWarning(__FUNCTION__))
          {
          cout << endl;
          cout << "Closing tag name mismatch \""<< _xmlDocument.currentTag->name<<"\" != \""<<textBuffer<<"\"" << endl;
          cout << "Xml end tag     : </"<<_xmlDocument.currentTag->name<<"> @ "<<_xmlDocument.currentTag->end << endl;
          }
        }
      // TAG closed - go to parent
      }
    else if(textBuffer.EndsWith("/>"))
      {
      // NEW EMPTY TAG
      textBuffer.ReplaceAll("<","");
      textBuffer.ReplaceAll("/>","");
      newTag = createTag(textBuffer);
      newTag->begin = filePosition;
      newTag->end   = newTag->begin;
      // TAG structure BEGIN & END
      if(_xmlDocument.currentTag->end == -1) { // end TAG not found - go level down
        _xmlDocument.currentTag->child  = newTag;
        newTag->father    = _xmlDocument.currentTag;
      }
      else
        { // add new sibling
          _xmlDocument.currentTag->next  = newTag;
          newTag->father          = _xmlDocument.currentTag->father;
          newTag->prev            = _xmlDocument.currentTag;
        }
      // TAG closed - go to parent
      _xmlDocument.currentTag = newTag;
      if (reportWarning(__FUNCTION__))
        {
        cout << endl;
        cout << "Xml empty tag...: <"<<_xmlDocument.currentTag->name<<" /> @ "<<_xmlDocument.currentTag->begin <<  endl;
        }
      if (reportDebug(__FUNCTION__))
        {
        cout << endl;
        for(std::list<XmlAttribute>::iterator iter = _xmlDocument.currentTag->attributes.begin(); iter != _xmlDocument.currentTag->attributes.end(); iter++)
          cout << "  attribute.....:  "<<iter->name<<"=\""<<iter->value<<"\"" << endl;
        }
      }
    else
      {
      // NEW TAG
      textBuffer.ReplaceAll("<","");
      textBuffer.ReplaceAll(">","");
      newTag = createTag(textBuffer);
      // TAG content begins in file
      newTag->begin = filePosition;
      // TAG structure BEGIN
      if(_xmlDocument.topTag == NULL)
        { // create main tag
          _xmlDocument.topTag = newTag;
        }
      else
        {
        if(_xmlDocument.currentTag->end == -1)
          { // end TAG not found - go level down
            _xmlDocument.currentTag->child  = newTag;
            newTag->father           = _xmlDocument.currentTag;
          }
        else
          { // add new sibling
            _xmlDocument.currentTag->next  = newTag;
            newTag->father    = _xmlDocument.currentTag->father;
            newTag->prev      = _xmlDocument.currentTag;
          }
        }
      _xmlDocument.currentTag = newTag;
      if (reportDebug(__FUNCTION__))
        {
        cout << endl;
        cout << "Xml begin tag.....: <"<<_xmlDocument.currentTag->name<<"> @ "<<_xmlDocument.currentTag->begin << endl;
        for(std::list<XmlAttribute>::iterator iter = _xmlDocument.currentTag->attributes.begin(); iter != _xmlDocument.currentTag->attributes.end(); iter++)
          cout << "  attribute.....:  "<<iter->name<<"=\""<<iter->value<<"\"" << endl;
        }
      }
    }
}

CAP::XmlTag* XmlParser::createTag(String & aBuff)
{
  XmlTag *  newTag;
  XmlAttribute  newAttribute;
  int    i;

  newTag = new XmlTag;
  newTag->name = "";
  newTag->attributes.clear();
  newTag->begin  = -1;
  newTag->end    = -1;
  newTag->father = NULL;
  newTag->child  = NULL;
  newTag->prev   = NULL;
  newTag->next   = NULL;

  // TAG name
  for(i=0; i<aBuff.Length(); i++)
    {
    if(aBuff[i] == ' ') break;
    newTag->name += aBuff[i];
    }
  // TAG attributes
  while(i<aBuff.Length())
    {
    if((aBuff[i] != ' ') && (aBuff[i] != '\t') && (aBuff[i] != '/'))
      {
      newAttribute.name  = "";
      newAttribute.value = "";
      while((aBuff[i] != '='))
        {
        newAttribute.name += aBuff[i];
        i++;
        }
      i += 2;
      while((aBuff[i] != '\"') && (aBuff[i] != '\''))
        {
        newAttribute.value += aBuff[i];
        i++;
        }
      newTag->attributes.push_back(newAttribute);
    }
    i++;
    }
  return newTag;
}

