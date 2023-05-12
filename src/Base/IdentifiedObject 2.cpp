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
#include "IdentifiedObject.hpp"
#include "Crc32.hpp"
#include <iostream>

using CAP::IdentifiedObject;
using std::ostream;
using std::cout;
using std::endl;

ClassImp(IdentifiedObject);

IdentifiedObject::IdentifiedObject()
:
name(),
title(),
version(),
description(),
hash(),
type()
{   }

IdentifiedObject::IdentifiedObject(const TString & _name,
                                   const TString & _title,
                                   const TString & _version,
                                   const TString & _description,
                                   const TString & _preHash,
                                   int   _type)
:
name(_name),
title(_title),
version(_version),
description(_description),
hash(),
type(_type)
{
  hash = calculateHash(_preHash);
}


IdentifiedObject::IdentifiedObject(const IdentifiedObject & source)
:
name( source.name ),
title( source.title ),
version( source.version ),
description( source.description ),
hash( source.hash ),
type( source.type )
{

}

IdentifiedObject & IdentifiedObject::operator=(const IdentifiedObject & source)
{
  if (this!=&source)
    {
    name         =  source.name;
    title        =  source.title;
    version      =  source.version;
    description  =  source.description;
    hash         =  source.hash;
    type         =  source.type;
    }
  return *this;
}

const TString IdentifiedObject::calculateHash(TString preHash)
{
  Crc32 hash;
  hash.update(preHash.Data(), preHash.Length());
  hash.finish();
  TString value = hash.getValueHex();
  return value;
}

void IdentifiedObject::print(ostream & os)
{
  os << " name.............: " << name << endl;
  os << " title............: " << title << endl;
  os << " version..........: " << version << endl;
  os << " description......: " << description << endl;
  os << " hash.............: " << hash << endl;
  os << " type.............: " << type << endl;
}

int IdentifiedObject::analyzeCmdLineInput(int nTokens, char **tokens)
{
  // result == 0 : exit normally after this call
  // result == 1 : exit with error status after this call
  // resutl > 1  : carry normally after this call

  if (nTokens > 1)
    {
    TString stringToken;
    for(int i=1; i<nTokens;i++)
      {
      stringToken = tokens[i];
      if ( stringToken.EqualTo("-h") || stringToken.EqualTo("--help") )
        {
        printHelp();
        return 0;
        }
      else if ( stringToken.EqualTo("-v") || stringToken.EqualTo("--version") )
        {
        printVersion();
        return 0;
        }
      else
        {
        printUnknownOption(stringToken);
        return 1;
        }
      }
    }
  return 2;
}

void IdentifiedObject::printVersion(const TString & option __attribute__((unused)))  const
{
  cout << "Name................:" << getName()<< endl;
  cout << "Version.............:" << getVersion()<< endl;
}

void IdentifiedObject::printHelp(const TString & option __attribute__((unused))) const
{
  cout << "Name................:" << getName()<< endl;
  cout << "Title...............:" << getTitle()<< endl;
  cout << "Version.............:" << getVersion()<< endl;
  cout << "Type................:" << getType()<< endl;
  cout << "Description.........:" << getDescription()<< endl;
}

void IdentifiedObject::printUnknownOption(const TString & option  __attribute__((unused))) const
{
  cout << endl;
  cout << "Not a valid option..:" << option;
  cout << endl;
}

