/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * Copyright (C) 2016, Chun Shen
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/
#ifndef CAP__ParticleTypeCollection
#define CAP__ParticleTypeCollection
#include <iostream>
#include "TString.h"
#include <string>
#include <ostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Collection.hpp"
#include "ParticleType.hpp"

using namespace std;


namespace CAP
{

class ParticleType;

class ParticleTypeCollection : public Collection<ParticleType>
{
public:

  // const String& _name, bool _ownership, Severity logLevel
  ParticleTypeCollection();
  virtual ~ParticleTypeCollection() {} 
  void readFromFile(const String & inputFileName);
  void writeToFile(const String &  outputFileName, bool printDecayProperties=true);
  void sortByMass();
  ParticleTypeCollection * extractCollection(int option);
   int findIndexForType(ParticleType * type);
   int findIndexForPdgCode(int pdgCode);
   int findIndexForPrivateCode(int privateCode);
  void resolveTypes();
  ParticleType * findPdgCode(int pdgCode);
  ParticleType * findPrivateCode(int privateCode);
  
  ParticleType * getParticleType(int index)
  {
    return objects[index];
  }

  ParticleType * operator[](int index)
  {
    return objects[index];
  }

  vector<int> getListOfPdgCodes();

  ostream & printProperties(ostream & os);
  ostream & printDecayProperties(ostream & os);

  static ParticleTypeCollection * masterParticleCollection;
  static ParticleTypeCollection * getMasterParticleCollection();


  ClassDef(ParticleTypeCollection,0)
};

} //namespace CAP

#endif  // CAP__ParticleTypeCollection

