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
#ifndef CAP__CmdlineAnalyzer
#define CAP__CmdlineAnalyzer
#include "TString.h"
using namespace std;

namespace CAP
{
class CmdlineAnalyzer
{
public:

  CmdlineAnalyzer();

  virtual ~CmdlineAnalyzer(){}

  int analyzer(int argc, char **argv);

  void setName(const TString & _name)
  {
    name = _name;
  }

  void setTitle(const TString & _title)
  {
  title = _title;
  }

  void setVersion(const TString & _version)
  {
  version = _version;
  }

  void setDescription(const TString & _description)
  {
  description = _description;
  }

  void setHash(const TString & _hash)
  {
  hash = _hash;
  }

  void setType(int _type)
  {
  type = _type;
  }

  const TString & getName() const
  {
  return name;
  }

  const TString & getTitle() const
  {
  return title;
  }

  const TString & getVersion() const
  {
  return version;
  }

 

  ClassDef(CmdlineAnalyzer,0)
};

}



#endif /* CAP__CmdlineAnalyzer */
