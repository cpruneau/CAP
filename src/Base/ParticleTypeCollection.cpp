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

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "ParticleTypeCollection.hpp"

ClassImp(Collection<ParticleType>);

ClassImp(ParticleTypeCollection);

using namespace std;

ParticleTypeCollection::ParticleTypeCollection()
:
Collection<ParticleType>()
{
}

ParticleTypeCollection::~ParticleTypeCollection()
{
}

// ================================================================================================
// read in ParticleType information from pdg data file
// ================================================================================================
void ParticleTypeCollection::readFromFile(const TString & inputFileName)
{
  cout << "<I> ParticleTypeCollection::readFromFile() Reading particle list from file:" <<  inputFileName << endl;
  ifstream inputFile(inputFileName.Data());
  int pdgCode;
  string name;
  string title;
  double mass, width;
  int gSpin, gIsospin;
  int baryon, strange, charm, bottom, charge;
  int nDecayModes, decayNpart;
  double decayBranchingRatio;
  int decayPart[5] = {0, 0, 0, 0, 0};
  ParticleType * particleType;
  ParticleType * antiParticleType;
  int dummy_int;
  TString theName;
  TString theAntiName;
  TString theTitle;
  TString theAntiTitle;
  while (1)
    {
    //cout << "Reading in ParticleType resonance decay table... - 2 -" << endl;
    inputFile >> pdgCode;
    if (inputFile.eof())
      {
      cout << "Reach EOF" << endl;
      break;
      }
    inputFile >> name;
    inputFile >> mass;
    inputFile >> width;
    inputFile >> gSpin;
    inputFile >> baryon;
    inputFile >> strange;
    inputFile >> charm;
    inputFile >> bottom;
    inputFile >> gIsospin;
    inputFile >> charge;
    inputFile >> nDecayModes;
    inputFile >> title;
    theName  = name;
    theTitle = title;
    particleType= new ParticleType(pdgCode, theName, theTitle, mass, width, gSpin, baryon, strange,
                                   charm, bottom, gIsospin, charge);
    push_back(particleType);
    if (particleType->isFermion())
      {
      theAntiName  = "Anti-";
      theAntiName  += theName;
      theAntiTitle = "#bar ";
      theAntiTitle += theTitle;
      antiParticleType =  new ParticleType(-pdgCode, theAntiName, theAntiTitle, mass, width, gSpin,
                                           -baryon, -strange, -charm, -bottom, gIsospin,
                                           -charge);
      push_back(antiParticleType);
      }
    // read decay information
    for (int j = 0; j < nDecayModes; j++)
      {
      inputFile >> dummy_int;
      inputFile >> decayNpart;
      inputFile >> decayBranchingRatio;
      inputFile >> decayPart[0];
      inputFile >> decayPart[1];
      inputFile >> decayPart[2];
      inputFile >> decayPart[3];
      inputFile >> decayPart[4];
      decayNpart = abs(decayNpart);
      std::vector<int> children;
      std::vector<int> antiChildren;
      for (int k=0; k<decayNpart; k++)
        {
        children.push_back(decayPart[k]);
        }
      if (!particleType->isFermion())
        {
        particleType->addDecayMode(decayBranchingRatio,children);
        }
      else
        {
        for (int k=0; k<decayNpart; k++)
          {
          antiChildren.push_back(findPdgCode(decayPart[k])->getAntiParticlePdgCode());
          }
        particleType->addDecayMode(decayBranchingRatio,children);
        antiParticleType->addDecayMode(decayBranchingRatio,antiChildren);
        }
      }
    }
    //cout << "Reading in ParticleType resonance decay table... - 5- " << endl;
    inputFile.close();
    //particleList.erase(particleList.begin());  // delete gamma
    cout << "<I> Total number of Hadrons: " <<  size() << endl;
    resolveTypes();
    sortByMass();

  // setup the decay montecarlo probabilities....
  for (unsigned int k=0; k<size(); k++)
    {
    objects[k]->setupDecayGenerator();
    }

  cout << "ParticleTypeCollection::readFromFile(const TString & inputFileName) Completed." << endl;
}

void ParticleTypeCollection::sortByMass()
{
  cout << "<D> ParticleTypeCollection::sortHadronListByHadronMass() Collection size:"
  << size() << endl;
  double m1, m2;
  int n = size();
  for (int i = 1; i<n; i++)
  {
  int k = i;
  int j = i - 1;
  m1 = objects[k]->getMass();
  m2 = objects[j]->getMass();
  while (j >= 0 && (operator[](k)->getMass() < operator[](j)->getMass()) )
    {
    ParticleType* temp = objects[j];
    objects[j] = objects[k];
    objects[k] = temp;
    k--;
    j--;
    }
  }
}

void ParticleTypeCollection::resolveTypes()
{
  int nPart = size();
  cout << "<D> ParticleTypeCollection::sortHadronListByHadronMass() Collection size:"
  << nPart << endl;

  for (int iPart = 0; iPart<nPart; iPart++)
  {
  ParticleType & particleType = *objects[iPart];
  int nModes = particleType.getNDecayModes();
  for (int iMode = 0; iMode<nModes; iMode++)
    {
    particleType.getDecayMode(iMode).resolveTypes(this);
    }
  }
}


// ================================================================================================
// Extract a subset of this collection based on the given ctriterion
// option : types selected
//      0 : stable particle types
//      1 : weakly stable
//      2 : unstable
//      3 : mesons
//      4 : baryons
//      5 : leptons
//      6 : fermions
//      7 : bosons
// ================================================================================================
ParticleTypeCollection *  ParticleTypeCollection::extractCollection(int option)
{
  ParticleTypeCollection * collection = new ParticleTypeCollection();
  unsigned int n = size();
  for (unsigned int iPart = 1; iPart < n; iPart++)
  {
  ParticleType * type = objects[iPart];
  switch (option)
    {
      case 0:  if (type->isStable()     && !type->isPhoton())  collection->push_back(type); break;
      case 1:  if (type->isWeakStable() && !type->isPhoton())  collection->push_back(type); break;
      case 2:  if (!type->isStable())     collection->push_back(type); break;
      case 3:  if (!type->isWeakStable()) collection->push_back(type); break;
      case 4:  if (type->isMeson())       collection->push_back(type); break;
      case 5:  if (type->isBaryon())      collection->push_back(type); break;
      case 6:  if (type->isLepton())      collection->push_back(type); break;
      case 7:  if (type->isFermion())     collection->push_back(type); break;
      case 8:  if (type->isBoson())       collection->push_back(type); break;
    }
  }
  return collection;
}

int ParticleTypeCollection::findIndexForPdgCode(int pdgCode)
{
  for (unsigned int iPart = 0; iPart < size(); iPart++)
  {
  if (pdgCode == objects[iPart]->getPdgCode()) return iPart;
  }
  return -1;
}

int ParticleTypeCollection::findIndexForPrivateCode(int privateCode)
{
  for (unsigned int iPart = 0; iPart < size(); iPart++)
  {
  if (privateCode == objects[iPart]->getPdgCode()) return iPart;
  }
  return -1;
}

int ParticleTypeCollection::findIndexForType(ParticleType * type)
{
  for (unsigned int iPart = 0; iPart < size(); iPart++)
  {
  if (type == objects[iPart]) return iPart;
  }
  return -1;
}

ParticleType * ParticleTypeCollection::findPdgCode(int pdgCode)
{
  for (unsigned int iPart = 0; iPart<size(); iPart++)
  {
  if (pdgCode == objects[iPart]->getPdgCode())
    return objects[iPart];
  }
  // code not found in the current table.
  // create new type and add to the table.
  ParticleType * newType = new ParticleType();
  newType->setName("unknown");
  newType->setTitle("unknown");
  newType->setPdgCode(pdgCode);
  push_back(newType);
  cout << " ------------------------------------------------ Added new type with pdgCode=" << pdgCode << endl;
  return newType;
}

ParticleType * ParticleTypeCollection::findPrivateCode(int privateCode)
{
  unsigned int n = size();
  for (unsigned int iPart = 0; iPart<n; iPart++)
  {
  if (privateCode == objects[iPart]->getPdgCode()) objects[iPart];
  }
  return nullptr;
}


ostream & ParticleTypeCollection::printProperties(ostream & os)
{
  os << "============================================================================================================================================================" << endl;
  os << "ParticleTypeCollection" << endl;
  os << "============================================================================================================================================================" << endl;
  os << setw(5) << "k";
  os << setw(10) << "Pdg";
  os << setw(20) << "Name";
  os << setw(20) << "Title";
  os << setw(20) << "Mass(GeV/c2)";
  os << setw(20) << "Width(GeV/c2)";
  os << setw(7) << "gSpin";
  os << setw(5) << "B";
  os << setw(5) << "s";
  os << setw(5) << "c";
  os << setw(5) << "b";
  os << setw(5) << "gIso";
  os << setw(5) << "Q";
  os << setw(5) << "decay";
  os << setw(5) << "stbl";
  os << setw(5) << "wStbl";
  os << setw(5) << "F/B" << endl;
  os << "============================================================================================================================================================" << endl;
  for (unsigned int k=0; k<size(); k++)
  {
  os << fixed << setw(5) << setprecision(4)<< k;
  objects[k]->printProperties(os);
  }
  return os;
}

ostream & ParticleTypeCollection::printDecayProperties(ostream & os)
{
  os << "============================================================================================================================================================" << endl;
  os << setw(10) << "Pdg";
  os << setw(20) << "Title";
  os << setw(20) << "Mass(GeV/c2)";
  os << setw(20) << "Width(GeV/c2)" << endl;
  os << "============================================================================================================================================================" << endl;
  for (unsigned int k=0; k<size(); k++)
  {
  objects[k]->printDecayProperties(os);
  }
  return os;
}

ParticleTypeCollection * ParticleTypeCollection::masterParticleCollection = nullptr;

ParticleTypeCollection * ParticleTypeCollection::getMasterParticleCollection()
{
  if (masterParticleCollection == nullptr)
    {
    masterParticleCollection = new ParticleTypeCollection();
    }
  return masterParticleCollection;
}


vector<int> ParticleTypeCollection::getListOfPdgCodes() 
{
  vector<int> pdgCodes;
  for (unsigned int iPart = 0; iPart<size(); iPart++)
  {
  int code = objects[iPart]->getPdgCode();
  pdgCodes.push_back(  code );
  }
  return pdgCodes;
}
