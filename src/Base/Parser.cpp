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
#include "Parser.hpp"
using CAP::Parser;

ClassImp(CAP::Parser);

Parser::Parser()
:
MessageLogger(Info)
{ }

Parser::Parser(MessageLogger::Severity _severity)
:
MessageLogger(_severity)
{ }

ifstream & Parser::openInputFile(const String & inputPath,
                                 const String & fileName,
                                 const String & extension,
                                 const String & ioOption)
{
  if (reportStart(__FUNCTION__))
;
  String inputFileName = makeFileName(inputPath,fileName,extension);
  if (reportTrace(__FUNCTION__))
    cout << "Attempting to open output file named: " << inputFileName << " in mode: " << ioOption << endl;

  ios_base::openmode mode = ios_base::in;
  if (reportDebug (__FUNCTION__))  cout << "Opening file: " << inputFileName << " with option: " << ioOption << endl;
  ifstream * inputFile = new ifstream(inputFileName.Data(),mode);
  if (inputFile && inputFile->is_open())
    {
    if (reportDebug (__FUNCTION__))
      cout << "File: " << inputFileName << " successfully opened." << endl;
    return *inputFile;
    }
  else
    {
    throw FileException(inputPath,fileName,"File not opened","Parser::openInputFile");
    }
}

ofstream & Parser::openOutputFile(const String & outputPath,
                                  const String & fileName,
                                  const String & extension,
                                  const String & ioOption)
{
  if (reportStart(__FUNCTION__))
;
  String outputFileName = makeFileName(outputPath,fileName,extension);
  if (reportTrace(__FUNCTION__))
    cout << "Attempting to open output file named: " << outputFileName << " in mode: " << ioOption << endl;

  ios_base::openmode mode = ios_base::out|ios_base::app;
  if (reportDebug (__FUNCTION__))  cout << "Opening file: " << outputFileName << " with option: " << ioOption << endl;
  ofstream * outputFile = new ofstream(outputFileName.Data(),mode);
  if (outputFile && outputFile->is_open())
    {
    if (reportDebug (__FUNCTION__))
      cout << "File: " << outputFileName << " successfully opened." << endl;
    return *outputFile;
    }
  else
    {
    throw FileException(outputFileName,"File not opened","Parser::openOutputFile");
    }
}

CAP::String Parser::makeFileName(const String & inputPath,
                            const String & fileName,
                            const String & extension)
{
  String inputFileName = inputPath;
  // make sure that if an inputPath is given, it ends with '/'
  int slash = inputFileName.First('/');
  int len   = inputFileName.Length();
  //  cout << slash << endl;
  //  cout << len << endl;
  if (len>0 && (len-1)!=slash) inputFileName += "/";
  inputFileName += fileName;
  if (!inputFileName.EndsWith(extension)) inputFileName += extension;
  return inputFileName;
}

