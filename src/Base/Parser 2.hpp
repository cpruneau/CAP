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
#ifndef CAP__Parser
#define CAP__Parser
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "TString.h"
#include "Aliases.hpp"
#include "MessageLogger.hpp"



using std::ostream;
using std::endl;
using std::vector;
using namespace std;

namespace CAP
{

class Parser : public MessageLogger
{
public:

  Parser();
  Parser(MessageLogger::Severity severity);

  virtual ~Parser() {}

  //!
  //! Open the root file named "fileName" located on the path "inputPath", using options specified by "ioOption".
  //! @param inputPath path where to find or create the file.
  //! @param fileName name of the root file to open for i/o.
  //! @param ioOption i/o options.
  //! @return Pointer to the file if successfully open or a null pointer if the file could not be opened.
  //!
  ifstream & openInputFile(const String  & inputPath,
                           const String  & fileName,
                           const String  & extension,
                           const String  & ioOption="") throw (FileException);
  ofstream & openOutputFile(const String  & outputPath,
                            const String  & fileName,
                            const String  & extension,
                            const String  & ioOption="") throw (FileException);

  String makeFileName(const String & inputPath,
                      const String & fileName,
                      const String & extension);

protected:

 ClassDef(Parser,0)

};



} // namespace CAP

#endif /* Parser_hpp */
