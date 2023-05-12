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
#include "ParticleTypeTableLoader.hpp"

ClassImp(ParticleTypeTableLoader);

ParticleTypeTableLoader:: ParticleTypeTableLoader(const TString & _name,
                                                  Configuration & _configuration)
:
Task(_name,_configuration)
{
  appendClassName("ParticleTypeTableLoader");
}

void ParticleTypeTableLoader::setDefaultConfiguration()
{
  //Task::setDefaultConfiguration();
  setParameter("UseParticles",      true);
  setParameter("LoadTable",         true);
  setParameter("PathName",          TString(getenv("CAP_SRC"))+"/EOS/");
  setParameter("TableName",         TString("pdg.dat"));
  setParameter("SaveTable",         false);
  setParameter("OutputPathName",    TString(getenv("CAP_SRC"))+"/EOS/");
  setParameter("OutputTableName",   TString("savedPdg.dat"));
}

void ParticleTypeTableLoader::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  bool    loadTable          = getValueBool(  "LoadTable");
  TString inputPathName      = getValueString("PathName");
  TString inputTableName     = getValueString("TableName");
  bool    saveTable          = getValueBool(  "SaveTable");
  TString outputPathName     = getValueString("OutputPathName");
  TString outputTableName    = getValueString("OutputTableName");
  TString pdgDataFileName;
  if (loadTable)
    {
    pdgDataFileName =  inputPathName;
    pdgDataFileName += inputTableName;
    ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection();
    particles->readFromFile(pdgDataFileName);
    if (reportDebug(__FUNCTION__))
      {
      particles->printProperties(std::cout);
      particles->printDecayProperties(std::cout);
      }
    }
  if (saveTable)
    {
    pdgDataFileName =  outputPathName;
    pdgDataFileName += outputTableName;
    ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection();
    particles->writeToFile(pdgDataFileName);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

