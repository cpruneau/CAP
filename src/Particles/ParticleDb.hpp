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
#ifndef CAP__ParticleDb
#define CAP__ParticleDb
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

class ParticleDb : public Collection<ParticleType>
{
public:

  // const String& _name, bool _ownership, Severity logLevel
  ParticleDb();
  virtual ~ParticleDb() {}
//  void readFromFile(const String & inputFileName);
//  void writeToFile(const String &  outputFileName, bool printDecayProperties=true);
  void sortByMass();
  ParticleDb * extractCollection(int option);
  unsigned int findIndexForType(ParticleType * type);
  unsigned int findIndexForName(const String & name);
  unsigned int findIndexForPdgCode(int pdgCode);
  unsigned int findIndexForPrivateCode(int privateCode);
  bool containsTypeNamed(String aName);

  int getNumberOfTypes() const { return size();}
  int getParticleTypeCount()const { return size();}

  void resolveTypes();
  void setupDecayGenerator();
  ParticleType * findPdgCode(int pdgCode);
  ParticleType * findPrivateCode(int privateCode);

  void addParticleType(ParticleType * particleType);
  ParticleType * getParticleType(String name);
  ParticleType * getParticleType(unsigned int index);
  ParticleType * operator[](unsigned int index)
  {
  if (index<objects.size())
    return objects[index];
  else
    return nullptr;
  }

  vector<int> getListOfPdgCodes();

  ostream & printProperties(ostream & os);
  ostream & printDecayProperties(ostream & os);

  static ParticleDb * defaultParticleDb;
  static void setDefaultParticleDb(ParticleDb * newDb);
  static ParticleDb * getDefaultParticleDb();

  ClassDef(ParticleDb,0)
};

} //namespace CAP

#endif  // CAP__ParticleDb

