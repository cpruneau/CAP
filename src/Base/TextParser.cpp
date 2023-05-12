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
#include "TextParser.hpp"
#include "Configuration.hpp"
using CAP::TextParser;

ClassImp(CAP::TextParser);


namespace CAP
{

TextParser::TextParser()
:
Parser()
{ }

TextParser::TextParser(MessageLogger::Severity _severity)
:
Parser(_severity)
{ }

int  TextParser::read(Configuration & _configuration,
                      const String  & _inputPath,
                      const String  & _inputFileName,
                      const String  & _extension)  throw (FileException)
{
  std::ifstream & inputFile = openInputFile(_inputPath,_inputFileName,_extension,"");;
  if (reportInfo(__FUNCTION__))
    cout << "Parsing file "<< _inputPath + _inputFileName << endl;

  //Parameter    parameter;
  TString      aString;
  int          lineCount;
  int          i;
  char         textBuffer[200];

  if (reportTrace(__FUNCTION__))
    cout << "seekg()" << endl;
  inputFile.seekg(0, std::ios::beg);
  lineCount = 0;
  while ((!inputFile.eof()))
    {
    lineCount++;
    inputFile.getline(textBuffer,200);
    aString = textBuffer;
    if((aString.IsNull()) || (aString.IsWhitespace()) || (aString[0] == '#') || (aString[0] == ';')) {
      if (reportDebug(__FUNCTION__))
        cout << "\tlineCount "<<lineCount<<" Ignoring  : "<<aString.Data() << endl;
      continue;
    } else if(aString.Contains('[')) {
      if (reportDebug(__FUNCTION__))
        cout << "\tlineCount "<<lineCount<<" Section   : "<<aString.Data() << endl;
      continue;
    } else if(!aString.Contains('=')) {
      if (reportDebug(__FUNCTION__))
        {
        cout << endl;
        cout << "\tlineCount "<<lineCount<<" WARNING   : "<<aString.Data() << endl;
        cout << "\tWrong format. Treating as commentary." << endl;
        }
      continue;
    }
    aString.ReplaceAll(" ","");
    aString.ReplaceAll("\t","");
    //    parameter.keyword = "";
    //    parameter.value   = "";
    //    for(i=0; aString[i] != '='; i++)   parameter.keyword += aString[i];
    //    for(i++; i<aString.Length(); i++)  parameter.value += aString[i];
    //    configuration.addParameter(parameter);
    String keyword = "";
    String value   = "";
    for(i=0; aString[i] != '='; i++)   keyword += aString[i];
    for(i++; i<aString.Length(); i++)  value   += aString[i];
    _configuration.addParameter(keyword,value);
    if (reportDebug(__FUNCTION__))
      cout << "\tlineCount "<<lineCount<<" Parameter : "<< keyword <<" = "<< value << endl;
    }
  return 0;
}

}
