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
#ifndef CAP__IdentifiedObject
#define CAP__IdentifiedObject
#include "TString.h"
using namespace std;

namespace CAP
{
class IdentifiedObject
{

protected:

  TString  name;
  TString  title;
  TString  version;
  TString  description;
  TString  hash;
  int      type;

public:

  IdentifiedObject();

  IdentifiedObject(const TString & _name,
                   const TString & _title,
                   const TString & _version,
                   const TString & _description,
                   const TString & _preHash,
                   int   _type);

  IdentifiedObject(const IdentifiedObject & source);

  IdentifiedObject & operator=(const IdentifiedObject & source);

  virtual ~IdentifiedObject(){}

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

  const TString & getDescription() const
  {
  return description;
  }

  const TString & getHash() const
  {
  return hash;
  }

  int getType() const
  {
  return type;
  }

  bool sameNameAs(const IdentifiedObject & otherObject) const
  {
  return name.EqualTo(otherObject.name);
  }

  bool sameNameAs(IdentifiedObject * otherObject) const
  {
  return name.EqualTo(otherObject->name);
  }

  const TString calculateHash(TString preHash);
  
  void print(ostream & os);

  virtual int analyzeCmdLineInput(int argc, char **argv);

  virtual void printVersion(const TString & option="") const;

  virtual void printHelp(const TString & option="") const;

  virtual void printUnknownOption(const TString & option="") const;

  // ============================
  // for backward compatibility
  // ============================


  const char* GetName()
  {
  return name.Data();
  }

  const char*  GetHash() const
  {
  return hash.Data();
  }

  const char* GetDescription()
  {
  return description.Data();
  }

  void CalculateHash(const TString & preHash)
  {
  hash = calculateHash(preHash);
  }

  ClassDef(IdentifiedObject,0)
};

}



#endif /* CAP__IdentifiedObject */
