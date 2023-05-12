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
#include "ParticleDb.hpp"
using CAP::Collection;
using CAP::ParticleType;
using CAP::ParticleDb;
using namespace std;

ClassImp(Collection<ParticleType>);

ClassImp(ParticleDb);


ParticleDb::ParticleDb()
:
Collection<ParticleType>()
{
}

// ================================================================================================
// read in ParticleType information from pdg data file
// ================================================================================================
//void ParticleDb::readFromFile(const String & inputFileName)
//{
//  cout << "<I> ParticleDb::readFromFile() Reading particle list from file:" <<  inputFileName << endl;
//  ifstream inputFile(inputFileName.Data());
//  if (!inputFile.is_open())
//    {
//    cout << "<E> ParticleDb::readFromFile() File not found:" <<  inputFileName << endl;
//    exit(1);
//    }
//  int pdgCode;
//  string name;
//  string title;
//  double mass, width;
//  int gSpin, gIsospin;
//  int baryon, strange, charm, bottom, charge;
//  int nDecayModes, decayNpart;
//  double decayBranchingRatio;
//  int decayPart[5] = {0, 0, 0, 0, 0};
//  ParticleType * particleType;
//  ParticleType * antiParticleType;
//  int dummy_int;
//  String theName;
//  String theAntiName;
//  String theTitle;
//  String theAntiTitle;
//  while (1)
//    {
//    //cout << "Reading in ParticleType resonance decay table.. - 2 -" << endl;
//    inputFile >> pdgCode;
//    if (inputFile.eof())
//      {
//      cout << "Reach EOF" << endl;
//      break;
//      }
//    inputFile >> name;
//    inputFile >> mass;
//    inputFile >> width;
//    inputFile >> gSpin;
//    inputFile >> baryonNumber;
//    inputFile >> strange;
//    inputFile >> charm;
//    inputFile >> bottom;
//    inputFile >> gIsospin;
//    inputFile >> charge;
//    inputFile >> nDecayModes;
//    inputFile >> title;
//    theName  = name;
//    theTitle = title;
//    particleType= new ParticleType(pdgCode, theName, theTitle, mass, width, gSpin, baryon, strange,
//                                   charm, bottom, gIsospin, charge);
//    push_back(particleType);
//    if (particleType->isFermion())
//      {
//      theAntiName  = "Anti-";
//      theAntiName  += theName;
//      theAntiTitle = "#bar ";
//      theAntiTitle += theTitle;
//      antiParticleType =  new ParticleType(-pdgCode, theAntiName, theAntiTitle, mass, width, gSpin,
//                                           -baryon, -strange, -charm, -bottom, gIsospin,
//                                           -charge);
//      push_back(antiParticleType);
//      }
//    // read decay information
//    for (int j = 0; j < nDecayModes; j++)
//      {
//      inputFile >> dummy_int;
//      inputFile >> decayNpart;
//      inputFile >> decayBranchingRatio;
//      inputFile >> decayPart[0];
//      inputFile >> decayPart[1];
//      inputFile >> decayPart[2];
//      inputFile >> decayPart[3];
//      inputFile >> decayPart[4];
//      decayNpart = abs(decayNpart);
//      std::vector<int> children;
//      std::vector<int> antiChildren;
//      for (int k=0; k<decayNpart; k++)
//        {
//        children.push_back(decayPart[k]);
//        }
//      if (!particleType->isFermion())
//        {
//        particleType->addDecayMode(decayBranchingRatio,children);
//        }
//      else
//        {
//        for (int k=0; k<decayNpart; k++)
//          {
//          antiChildren.push_back(findPdgCode(decayPart[k])->getAntiParticlePdgCode());
//          }
//        particleType->addDecayMode(decayBranchingRatio,children);
//        antiParticleType->addDecayMode(decayBranchingRatio,antiChildren);
//        }
//      }
//    }
//    //cout << "Reading in ParticleType resonance decay table.. - 5- " << endl;
//    inputFile.close();
//    //particleList.erase(particleList.begin());  // delete gamma
//    cout << "<I> Total number of Hadrons: " <<  size() << endl;
//    resolveTypes();
//    sortByMass();
//
//  // setup the decay montecarlo probabilities...
//  for (unsigned int k=0; k<size(); k++)
//    {
//    objects[k]->setupDecayGenerator();
//    }
//
//  cout << "ParticleDb::readFromFile(const String  & inputFileName) Completed." << endl;
//}
//
//void ParticleDb::writeToFile(const String & outputFileName, bool printDecayProperties)
//{
//  cout << "<I> ParticleDb::readFromFile() Writing particle list to file:" <<  outputFileName << endl;
//  ofstream outputFile(outputFileName.Data());
//  for (unsigned int iType=0; iType<size(); iType++)
//    {
//    ParticleType * type = operator[](iType);
//    if (type->getPdgCode()<0) continue; // not printing antiparticles
//    //type->printToFile(outputFile,true);
//    if (printDecayProperties)
//      {
//      //
//      }
//    }
//  outputFile.close();
//  cout << "ParticleDb::readFromFile(const String  & inputFileName) Completed." << endl;
//}

void ParticleDb::sortByMass()
{
  cout << "<D> ParticleDb::sortHadronListByHadronMass() Collection size:"
  << size() << endl;
  //double m1, m2;
  int n = size();
  for (int i = 1; i<n; i++)
  {
  int k = i;
  int j = i - 1;
  //m1 = objects[k]->getMass();
  //m2 = objects[j]->getMass();
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

void ParticleDb::resolveTypes()
{
  int nPart = size();
  cout << "<D> ParticleDb::resolveTypes()  Collection size:"
  << nPart << endl;

  for (int iPart = 0; iPart<nPart; iPart++)
  {
  ParticleType & particleType = *objects[iPart];
  int nModes = particleType.getNDecayModes();
  for (int iMode = 0; iMode<nModes; iMode++)
    {
    particleType.getDecayMode(iMode).resolveTypes(*this);
    }
  }
}

void ParticleDb::setupDecayGenerator()
{
  // setup the decay montecarlo probabilities...
  for (unsigned int k=0; k<size(); k++)
    {
    getParticleType(k)->setupDecayGenerator();
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
ParticleDb *  ParticleDb::extractCollection(int option)
{
  ParticleDb * collection = new ParticleDb();
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

unsigned int ParticleDb::findIndexForType(ParticleType * type)
{
  for (unsigned int iPart = 0; iPart < size(); iPart++)
    {
    if (type == objects[iPart]) return iPart;
    }
  return -1;
}

unsigned int ParticleDb::findIndexForName(const CAP::String & name)
{
  for (unsigned int iPart = 0; iPart < size(); iPart++)
    {
    if (objects[iPart]->getName().EqualTo(name)) return iPart;
    }
  return -1;
}


unsigned int ParticleDb::findIndexForPdgCode(int pdgCode)
{
  for (unsigned int iPart = 0; iPart < size(); iPart++)
  {
  if (pdgCode == objects[iPart]->getPdgCode()) return iPart;
  }
  return -1;
}

unsigned int ParticleDb::findIndexForPrivateCode(int privateCode)
{
  for (unsigned int iPart = 0; iPart < size(); iPart++)
  {
  if (privateCode == objects[iPart]->getPdgCode()) return iPart;
  }
  return -1;
}

bool ParticleDb::containsTypeNamed(CAP::String name)
{
  for (unsigned int iPart = 0; iPart < size(); iPart++)
    {
    if (objects[iPart]->getName().EqualTo(name)) return true;
    }
  return false;
}

ParticleType * ParticleDb::findPdgCode(int pdgCode)
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

ParticleType * ParticleDb::findPrivateCode(int privateCode)
{
  unsigned int n = size();
  for (unsigned int iPart = 0; iPart<n; iPart++)
  {
  if (privateCode == objects[iPart]->getPdgCode()) objects[iPart];
  }
  return nullptr;
}

void ParticleDb::addParticleType(ParticleType * particleType)
{
  append(particleType);
}

ParticleType * ParticleDb::getParticleType(String name)
{
  unsigned int index = findIndexForName(name);
  if (index>=0)
    return objects[index];
  else
    return nullptr;
}


ParticleType * ParticleDb::getParticleType(unsigned int index)
{
  if (index<objects.size())
    return objects[index];
  else
    return nullptr;
}



ostream & ParticleDb::printProperties(ostream & os)
{
  os << "============================================================================================================================================================" << endl;
  os << "ParticleDb" << endl;
  os << "============================================================================================================================================================" << endl;
  os <<  fixed << setw(5)  << "k";
  os <<  fixed << setw(10) << "PDG";
  os <<  fixed << setw(10) << "Name";
  os <<  fixed << setw(12) << "Title";
  os <<  fixed << setw(12) << "Mass";
  os <<  fixed << setw(12) << "Width";
  os <<  fixed << setw(9)  << "Charge";
  os <<  fixed << setw(9)  << "Spin";
  os <<  fixed << setw(9)  << "Isospin";
  os <<  fixed << setw(9)  << "Isospin3";
  os <<  fixed << setw(5)  << "Nq";
  os <<  fixed << setw(5)  << "Naq";
  os <<  fixed << setw(5)  << "Ns";
  os <<  fixed << setw(5)  << "Nas";
  os <<  fixed << setw(5)  << "Nc";
  os <<  fixed << setw(5)  << "Nac";
  os <<  fixed << setw(5)  << "Nb";
  os <<  fixed << setw(5)  << "Nab";
  os <<  fixed << setw(5)  << "Nt";
  os <<  fixed << setw(5)  << "Nat";
  os <<  fixed << setw(5)  << "L(e)";
  os <<  fixed << setw(5)  << "L(m)";
  os <<  fixed << setw(5)  << "L(t)";
  os <<  fixed << setw(5)  << "S";
  os <<  fixed << setw(5)  << "W";
  os << "============================================================================================================================================================" << endl;
  for (unsigned int k=0; k<size(); k++)
  {
  os << fixed << setw(5) << setprecision(4)<< k;
  objects[k]->printProperties(os);
  }
  return os;
}

ostream & ParticleDb::printDecayProperties(ostream & os)
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

ParticleDb * ParticleDb::defaultParticleDb = nullptr;

ParticleDb * ParticleDb::getDefaultParticleDb()
{
  if (defaultParticleDb == nullptr)
    {
    defaultParticleDb = new ParticleDb();
    }
  return defaultParticleDb;
}

void ParticleDb::setDefaultParticleDb(ParticleDb * newDb)
{
  if (defaultParticleDb) return;
  defaultParticleDb = newDb;
}


vector<int> ParticleDb::getListOfPdgCodes() 
{
  vector<int> pdgCodes;
  for (unsigned int iPart = 0; iPart<size(); iPart++)
  {
  int code = objects[iPart]->getPdgCode();
  pdgCodes.push_back(  code );
  }
  return pdgCodes;
}
