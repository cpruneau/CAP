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
#ifndef CAP__ParticleDbParser
#define CAP__ParticleDbParser
#include "Parser.hpp"
#include "ParticleDb.hpp"


namespace CAP
{

class ParticleDbParser : public Parser
{
public:

  ParticleDbParser();
  ParticleDbParser(MessageLogger::Severity _severity);
  virtual ~ParticleDbParser() {}

  virtual int  read(ParticleDb    & _particleDb,
                    const String  & _inputPath,
                    const String  & _inputFileName,
                    const String  & _extension = ".ini");

  ClassDef(ParticleDbParser,0)

};

} // namespace CAP

#endif /* ParticleDbParser_hpp */
