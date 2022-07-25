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

HadronGasGeneratorTask::HadronGasGeneratorTask(const TString  &  _name,
                                               Configuration  &  _configuration,
                                               LogLevel          _selectedLevel)
:
Task(_name,_configuration,_selectedLevel),
particleTypes(nullptr),
stableParticleTypes(nullptr),
nThermalSpecies(0),
nStableSpecies(0)
{
  appendClassName("HadronGasGeneratorTask");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void HadronGasGeneratorTask::setDefaultConfiguration()
{
  if (reportStart(__FUNCTION__))
    ;
  TString hg = "HG";
  Configuration & config = getConfiguration();
  config.addParameter("useParticles",          false);
  config.addParameter("useEventStream0",       false);
  config.addParameter("standaloneMode",        true);
  config.setParameter("createHistograms",      true);
  config.setParameter("saveHistograms",        true);
  config.setParameter("histoModelName",        hg);
  config.setParameter("histoAnalyzerName",     hg);
  config.setParameter("histoBaseName",         hg);
  config.setParameter("doTempDependentHistos", true);
  config.setParameter("doPtDistHistos",        true);
  config.setParameter("plotPtDistHistos",      false);
  config.addParameter("nThermalSpecies",       1);
  config.addParameter("nStableSpecies",        1);
  config.addParameter("nChemicalTemp",         1);
  config.addParameter("minChemicalTemp",       150.0);
  config.addParameter("maxChemicalTemp",       240.0);
  config.addParameter("nMuB",                  1);
  config.addParameter("minMuB",                0.0);
  config.addParameter("maxMuB",                0.0);
  config.addParameter("nMuS",                  1);
  config.addParameter("minMuS",                0.0);
  config.addParameter("maxMuS",                0.0);
  config.addParameter("volume",                1.0);
  config.addParameter("nMass",                 50);
  config.addParameter("minMass",               0.0);
  config.addParameter("maxMass",               3.0);
  config.addParameter("nP",                    500);
  config.addParameter("minP",                  0.0);
  config.addParameter("maxP",                  5.0);
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
  configuration.setParameter("nThermalSpecies", nThermalSpecies);
  configuration.setParameter("nStableSpecies",  nStableSpecies);

  for (int k=0;k<nThermalSpecies; k++)
    {
    TString key = "Species";
    key += k;
    configuration.addParameter(key,particleTypes->getParticleType(k)->getTitle());
    }
  for (int k=0;k<nStableSpecies; k++)
    {
    TString key = "StableSpecies";
    key += k;
    configuration.addParameter(key,stableParticleTypes->getParticleType(k)->getTitle());
    }

  modelName             = configuration.getValueString("histoModelName");
  nChemicalTemp         = configuration.getValueInt("nChemicalTemp");
  minChemicalTemp       = configuration.getValueDouble("minChemicalTemp");
  maxChemicalTemp       = configuration.getValueDouble("maxChemicalTemp");
  if (maxChemicalTemp == minChemicalTemp || nChemicalTemp==1)
    {
    nChemicalTemp = 1;
    stepTemp      = minChemicalTemp/10.0;
    }
  else
    stepTemp = (maxChemicalTemp - minChemicalTemp)/(nChemicalTemp-1);

  nMuB                  = configuration.getValueInt("nMuB");
  minMuB                = configuration.getValueDouble("minMuB");
  maxMuB                = configuration.getValueDouble("maxMuB");
  if (maxMuB == minMuB || nMuB==1)
    {
    nMuB    = 1;
    stepMuB = 0.0;
    }
  else
    stepMuB = (maxMuB - minMuB)/double(nMuB-1);

  nMuS                  = configuration.getValueInt("nMuS");
  minMuS                = configuration.getValueDouble("minMuS");
  maxMuS                = configuration.getValueDouble("maxMuS");
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
        gasConfig.addParameter("temperature", temperature);
        gasConfig.addParameter("muB",         muB);
        gasConfig.addParameter("muS",         muS);
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

  TString bn                    = configuration.getValueString("histoBaseName");
  bool    doTempDependentHistos = configuration.getValueBool("doTempDependentHistos");
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
        histos = new HadronGasHistograms(makeHistoName(bn,tempLabel,muBLabel,muSLabel),configuration,debugLevel);
        histos->createHistograms();
        baseSingleHistograms.push_back(histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histos = new HadronGasVsTempHistograms(bn+"VsT",configuration,debugLevel);
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

  TString bn                    = configuration.getValueString("histoBaseName");
  bool    doTempDependentHistos = configuration.getValueBool("doTempDependentHistos");

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
        histos = new HadronGasHistograms(makeHistoName(bn,tempLabel,muBLabel,muSLabel),configuration,debugLevel);
        histos->loadHistograms(inputFile);
        baseSingleHistograms.push_back(histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histos = new HadronGasVsTempHistograms(bn+"VsT",configuration,debugLevel);
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
//  bool    doTempDependentHistos = configuration.getValueBool("doTempDependentHistos");
//  int     nChemicalTemp         = configuration.getValueInt("nChemicalTemp");
//  double  minChemicalTemp       = configuration.getValueDouble("minChemicalTemp");
//  double  maxChemicalTemp       = configuration.getValueDouble("maxChemicalTemp");
//  double  stepTemp              = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);
//
//  int     nMuB                  = configuration.getValueInt("nMuB");
//  double  minMuB                = configuration.getValueDouble("minMuB");
//  double  maxMuB                = configuration.getValueDouble("maxMuB");
//  double  stepMuB               = (maxMuB - minMuB)/double(nMuB);
//
//  int     nMuS                  = configuration.getValueInt("nMuS");
//  double  minMuS                = configuration.getValueDouble("minMuS");
//  double  maxMuS                = configuration.getValueDouble("maxMuS");
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
