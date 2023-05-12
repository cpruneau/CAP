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
#include "CmdlineAnalyzer.hpp"
#include "Crc32.hpp"
#include <iostream>

using CAP::CmdlineAnalyzer;
using std::ostream;
using std::cout;
using std::endl;

ClassImp(CmdlineAnalyzer);

CmdlineAnalyzer::CmdlineAnalyzer()
:
name(),
title(),
version(),
description(),
hash(),
type()
{   }

CmdlineAnalyzer::CmdlineAnalyzer(const TString & _name,
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


CmdlineAnalyzer::CmdlineAnalyzer(const CmdlineAnalyzer & source)
:
name( source.name ),
title( source.title ),
version( source.version ),
description( source.description ),
hash( source.hash ),
type( source.type )
{

}

CmdlineAnalyzer & CmdlineAnalyzer::operator=(const CmdlineAnalyzer & source)
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

const TString CmdlineAnalyzer::calculateHash(TString preHash)
{
  Crc32 hash;
  hash.update(preHash.Data(), preHash.Length());
  hash.finish();
  TString value = hash.getValueHex();
  return value;
}

void CmdlineAnalyzer::print(ostream & os)
{
  os << " name.............: " << name << endl;
  os << " title............: " << title << endl;
  os << " version..........: " << version << endl;
  os << " description......: " << description << endl;
  os << " hash.............: " << hash << endl;
  os << " type.............: " << type << endl;
}



