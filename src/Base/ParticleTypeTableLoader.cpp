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

ParticleTypeTableLoader:: ParticleTypeTableLoader(const TString &         _name,
                                                  const Configuration &   _configuration,
                                                  LogLevel                _selectedLevel)
:
Task(_name,_configuration,_selectedLevel)
{
  appendClassName("ParticleTypeTableLoader");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void ParticleTypeTableLoader::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  configuration.setName("ParticleTypeTableLoader Configuration");
  configuration.setParameter("useParticles",      true);
  configuration.setParameter("dataInputUsed",     true);
  configuration.setParameter("dataInputPath",     TString(getenv("CAP_DATA")));
  configuration.setParameter("dataInputFileName", TString("ParticleTypeData.dat"));
  configuration.setParameter("dataOutputUsed",    false);
  configuration.setParameter("dataOutputPath",    TString(getenv("CAP_DATA")));
  configuration.setParameter("dataOutputFileName",TString("NewParticleTypeData.dat"));
  if (reportDebug("ParticleTypeTableLoader",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}

void ParticleTypeTableLoader::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  bool    dataInputUsed      = configuration.getValueBool("dataInputUsed");
  TString dataInputPath      = configuration.getValueString("dataInputPath");
  TString dataInputFileName  = configuration.getValueString("dataInputFileName");
  bool    dataOutputUsed     = configuration.getValueBool("dataOutputUsed");
  TString dataOutputPath     = configuration.getValueString("dataOutputPath");
  TString dataOutputFileName = configuration.getValueString("dataOutputFileName");

  TString pdgDataFileName;
  
  if (dataInputUsed)
    {
    pdgDataFileName =  dataInputPath;
    pdgDataFileName += dataInputFileName;
    ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection();
    particles->readFromFile(pdgDataFileName);
    if (reportDebug("ParticleTypeTableLoader",getName(),"execute()"))
      particles->printProperties(std::cout);
    }

  if (dataOutputUsed)
    {
    pdgDataFileName =  dataOutputPath;
    pdgDataFileName += dataOutputFileName;
    ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection();
    particles->writeToFile(pdgDataFileName);
    }
 
  if (reportEnd(__FUNCTION__))
    ;
}
