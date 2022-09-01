/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 * Copyright 2016 Chun Shen
 * Copyright 2022 Claude Pruneau
 *
 * *********************************************************************/
#include "HadronGasGeneratorTask.hpp"
#include "HadronGasHistograms.hpp"
#include "HadronGasVsTempHistograms.hpp"

ClassImp(HadronGasGeneratorTask);

HadronGasGeneratorTask::HadronGasGeneratorTask(const TString & _name,
                                               Configuration & _configuration)
:
Task(_name,_configuration),
particleTypes(nullptr),
stableParticleTypes(nullptr),
nThermalSpecies(0),
nStableSpecies(0)
{
  appendClassName("HadronGasGeneratorTask");
}

void HadronGasGeneratorTask::setDefaultConfiguration()
{
  //Task::setDefaultConfiguration();
  addParameter("UseParticles",          false);
  addParameter("UseEventStream0",       false);
  addParameter("StandaloneMode",        true);
  setParameter("CreateHistograms",      true);
  setParameter("SaveHistograms",        true);
  setParameter("DoTempDependentHistos", true);
  setParameter("DoPtDistHistos",        true);
  setParameter("PlotPtDistHistos",      false);
  addParameter("nThermalSpecies",       1);
  addParameter("nStableSpecies",        1);
  addParameter("nChemicalTemp",         1);
  addParameter("MinChemicalTemp",       150.0);
  addParameter("MaxChemicalTemp",       240.0);
  addParameter("nMuB",                  1);
  addParameter("MinMuB",                0.0);
  addParameter("MaxMuB",                0.0);
  addParameter("nMuS",                  1);
  addParameter("MinMuS",                0.0);
  addParameter("MaxMuS",                0.0);
  addParameter("Volume",                1.0);
  addParameter("nMass",                 50);
  addParameter("MinMass",               0.0);
  addParameter("MaxMass",               3.0);
  addParameter("nP",                    500);
  addParameter("MinP",                  0.0);
  addParameter("MaxP",                  5.0);
  if (reportEnd(__FUNCTION__))
    ;
}


void HadronGasGeneratorTask::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  particleTypes       = ParticleTypeCollection::getMasterParticleCollection();
  stableParticleTypes = particleTypes->extractCollection(1);
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "Particle Properties -- Full set" << endl;
    cout << "--------------------------------------------------------" << endl;
    particleTypes->printProperties(std::cout);
    cout << "--------------------------------------------------------" << endl;
    cout << "Particle Properties -- Stable set" << endl;
    cout << "--------------------------------------------------------" << endl;
    stableParticleTypes->printProperties(std::cout);
    }
  nThermalSpecies = int(particleTypes->size());
  nStableSpecies  = int(stableParticleTypes->size());
  setParameter("nThermalSpecies", nThermalSpecies);
  setParameter("nStableSpecies",  nStableSpecies);

  for (int k=0;k<nThermalSpecies; k++)
    {
    TString key = "Species";
    key += k;
    addParameter(key,particleTypes->getParticleType(k)->getTitle());
    }
  for (int k=0;k<nStableSpecies; k++)
    {
    TString key = "StableSpecies";
    key += k;
    addParameter(key,stableParticleTypes->getParticleType(k)->getTitle());
    }

  modelName             = getValueString("HistoModelName");
  nChemicalTemp         = configuration.getValueInt(getName(),"nChemicalTemp");
  minChemicalTemp       = configuration.getValueDouble(getName(),"MinChemicalTemp");
  maxChemicalTemp       = configuration.getValueDouble(getName(),"MaxChemicalTemp");
  if (maxChemicalTemp == minChemicalTemp || nChemicalTemp==1)
    {
    nChemicalTemp = 1;
    stepTemp      = minChemicalTemp/10.0;
    }
  else
    stepTemp = (maxChemicalTemp - minChemicalTemp)/(nChemicalTemp-1);

  nMuB                  = configuration.getValueInt(getName(),"nMuB");
  minMuB                = configuration.getValueDouble(getName(),"MinMuB");
  maxMuB                = configuration.getValueDouble(getName(),"MaxMuB");
  if (maxMuB == minMuB || nMuB==1)
    {
    nMuB    = 1;
    stepMuB = 0.0;
    }
  else
    stepMuB = (maxMuB - minMuB)/double(nMuB-1);

  nMuS                  = configuration.getValueInt(getName(),"nMuS");
  minMuS                = configuration.getValueDouble(getName(),"MinMuS");
  maxMuS                = configuration.getValueDouble(getName(),"MaxMuS");
  if (maxMuS == minMuS || nMuS==1)
    {
    nMuS    = 1;
    stepMuS = 0.0;
    }
  else
    stepMuS = (maxMuS - minMuS)/double(nMuS-1);

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "        modelName:" << modelName <<  endl;
    cout << "    nChemicalTemp:" << nChemicalTemp <<  endl;
    cout << "  minChemicalTemp:" << minChemicalTemp <<  endl;
    cout << "  maxChemicalTemp:" << maxChemicalTemp <<  endl;
    cout << "         stepTemp:" << stepTemp <<  endl;
    cout << "             nMuB:" << nMuB <<  endl;
    cout << "           minMuB:" << minMuB <<  endl;
    cout << "           maxMuB:" << maxMuB <<  endl;
    cout << "          stepMuB:" << stepMuB <<  endl;
    cout << "             nMuS:" << nMuS <<  endl;
    cout << "           minMuS:" << minMuS <<  endl;
    cout << "           maxMuS:" << maxMuS <<  endl;
    cout << "          stepMuS:" << stepMuS <<  endl;
    }
  Task::initialize();

  if (reportEnd(__FUNCTION__))
    ;
}

void HadronGasGeneratorTask::execute()
{
  if (reportStart(__FUNCTION__))
    ;

  int count = 0;
  double temperature, muB, muS;
  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    temperature = minChemicalTemp+stepTemp*double(iTemp);
    TString tempLabel = "T";
    tempLabel += int(0.5+1000*temperature);
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      muB = minMuB+stepMuB*double(iMuB);
      TString muBLabel = "B";
      muBLabel += int(0.5+1000*muB);
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        muS = minMuS+stepMuS*double(iMuS);
        TString muSLabel = "S";
        muSLabel += int(0.5+1000*muB);
        TString name = makeHistoName(modelName,tempLabel,muBLabel,muSLabel);
        Configuration gasConfig(name);
        gasConfig.addParameter("Temperature", temperature);
        gasConfig.addParameter("MuB",         muB);
        gasConfig.addParameter("MuS",         muS);
        HadronGas * gas = new HadronGas(name,gasConfig, particleTypes,stableParticleTypes,getLogLevel());
        gas->initialize();
        gas->execute();
        HadronGasVsTempHistograms * hgVsTHistos = (HadronGasVsTempHistograms*) histograms[0];
        hgVsTHistos->fill(*gas);
        HadronGasHistograms * hgHistos = (HadronGasHistograms*) baseSingleHistograms[count];
        hgHistos->fill(*gas);
        count++;
        // keep track of these as subtasks for future in HadronGasParticleGenerator
        addSubTask(gas);
        if (reportDebug(__FUNCTION__)) gas->printProperties(std::cout);
        }
      }
    }
  //Task::finalize();
  if (reportEnd(__FUNCTION__))
    ;
}


// temperature, muB, and muS  must be input in GeV
void HadronGasGeneratorTask::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  histograms.clear();
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  LogLevel debugLevel = getReportLevel();
  TString bn = getValueString("HistoBaseName");
  bool    doTempDependentHistos = getValueBool("DoTempDependentHistos");
  Histograms * histos;
  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    TString tempLabel = "T";
    tempLabel += int(0.5+1000*(minChemicalTemp+stepTemp*double(iTemp)));
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      TString muBLabel = "B";
      muBLabel += int(0.5+1000*(minMuB+stepMuB*double(iMuB)));
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        TString muSLabel = "S";
        muSLabel += int(0.5+1000*(minMuS+stepMuS*double(iMuS)));
        histos = new HadronGasHistograms(this,makeHistoName(bn,tempLabel,muBLabel,muSLabel),configuration);
        histos->createHistograms();
        baseSingleHistograms.push_back(histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histos = new HadronGasVsTempHistograms(this,bn+"VsT",configuration);
    histos->createHistograms();
    histograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void HadronGasGeneratorTask::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  histograms.clear();
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  LogLevel debugLevel = getReportLevel();

  TString bn                    = getValueString("HistoBaseName");
  bool    doTempDependentHistos = getValueBool("DoTempDependentHistos");

  Histograms * histos;
  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    TString tempLabel = "T";
    tempLabel += int(0.5+1000*(minChemicalTemp+stepTemp*double(iTemp)));
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      TString muBLabel = "B";
      muBLabel += int(0.5+1000*(minMuB+stepMuB*double(iMuB)));
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        TString muSLabel = "S";
        muSLabel += int(0.5+1000*(minMuS+stepMuS*double(iMuS)));
        histos = new HadronGasHistograms(this,makeHistoName(bn,tempLabel,muBLabel,muSLabel),configuration);
        histos->loadHistograms(inputFile);
        baseSingleHistograms.push_back(histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histos = new HadronGasVsTempHistograms(this,bn+"VsT",configuration);
    histos->loadHistograms(inputFile);
    histograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

//void HadronGasGeneratorTask::execute()
//{
//  if (reportStart(__FUNCTION__))
//    ;
//  bool    doTempDependentHistos = getValueBool("DoTempDependentHistos");
//  int     nChemicalTemp         = configuration.getValueInt(getName(),"nChemicalTemp");
//  double  minChemicalTemp       = configuration.getValueDouble(getName(),"MinChemicalTemp");
//  double  maxChemicalTemp       = configuration.getValueDouble(getName(),"MaxChemicalTemp");
//  double  stepTemp              = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);
//
//  int     nMuB                  = configuration.getValueInt(getName(),"nMuB");
//  double  minMuB                = configuration.getValueDouble(getName(),"MinMuB");
//  double  maxMuB                = configuration.getValueDouble(getName(),"MaxMuB");
//  double  stepMuB               = (maxMuB - minMuB)/double(nMuB);
//
//  int     nMuS                  = configuration.getValueInt(getName(),"nMuS");
//  double  minMuS                = configuration.getValueDouble(getName(),"MinMuS");
//  double  maxMuS                = configuration.getValueDouble(getName(),"MaxMuS");
//  double  stepMuS               = (maxMuS - minMuS)/double(nMuS);
//
//  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++)
//    {
//    double temperature = minChemicalTemp+stepTemp*double(iTemp);
//    for (int iMuB=0; iMuB<nMuB; iMuB++)
//      {
//      double muB = minMuB+stepMuB*double(iMuB);
//      for (int iMuS=0; iMuS<nMuS; iMuS++)
//        {
//        double muB = minMuB+stepMuB*double(iMuB);
//        HadronGas * hg = new HadronGas();
//        addSubtask(hg);
//        hg->initialize();
//        hg->execute();
//        }
//      }
//    }
//  if (reportEnd(__FUNCTION__))
//    ;
//}
