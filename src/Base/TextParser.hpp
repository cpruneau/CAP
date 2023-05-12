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
#ifndef CAP__TextParser
#define CAP__TextParser
#include "Parser.hpp"


namespace CAP
{
class Configuration;

class TextParser : public Parser
{
public:

  TextParser();
  TextParser(MessageLogger::Severity _severity);
  virtual ~TextParser() {}

  virtual int  read(Configuration & _configuration,
                    const String  & _inputPath,
                    const String  & _inputFileName,
                    const String  & _extension = ".ini");

  ClassDef(TextParser,0)

};

} // namespace CAP

#endif /* TextParser_hpp */
