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
#include "ThermalGasModel.hpp"
#include "ThermalGasHistograms.hpp"
#include "ThermalGasVsTempHistograms.hpp"
#include "ParticleDb.hpp"
#include "MathConstants.hpp"

using CAP::ParticleDb;
using CAP::ThermalGasModel;

ClassImp(ThermalGasModel);

ThermalGasModel::ThermalGasModel(const String & _name,
                                         const Configuration & _configuration)
:
EventTask(_name,_configuration),
particleTypes(nullptr),
stableParticleTypes(nullptr),
nThermalSpecies(0),
nStableSpecies(0)
{
  appendClassName("ThermalGasModel");
}

void ThermalGasModel::setDefaultConfiguration()
{
  //Task::setDefaultConfiguration();
  addParameter("UseParticles",          false);
  addParameter("EventsUseStream0",       false);
  addParameter("StandaloneMode",        true);
  addParameter("HistogramsCreate",      true);
  addParameter("HistogramsExport",        true);
  addParameter("DoTempDependentHistos", true);
  addParameter("DoPtDistHistos",        true);
  addParameter("PlotPtDistHistos",      false);
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


void ThermalGasModel::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  particleTypes       = CAP::ParticleDb::getDefaultParticleDb();
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
  addParameter("nThermalSpecies", nThermalSpecies);
  addParameter("nStableSpecies",  nStableSpecies);

  for (int k=0;k<nThermalSpecies; k++)
    {
    String key = "Species";
    key += k;
    addParameter(key,particleTypes->getParticleType(k)->getTitle());
    }
  for (int k=0;k<nStableSpecies; k++)
    {
    String key = "StableSpecies";
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

void ThermalGasModel::execute()
{
  if (reportStart(__FUNCTION__))
    ;

  int count = 0;
  double temperature, muB, muS;
  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    temperature = minChemicalTemp+stepTemp*double(iTemp);
    String tempLabel = "T";
    tempLabel += int(0.5+1000*temperature);
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      muB = minMuB+stepMuB*double(iMuB);
      String muBLabel = "B";
      muBLabel += int(0.5+1000*muB);
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        muS = minMuS+stepMuS*double(iMuS);
        String muSLabel = "S";
        muSLabel += int(0.5+1000*muB);
        String name = createName(modelName,tempLabel,muBLabel,muSLabel);
        Configuration gasConfig;
        gasConfig.addParameter("Temperature", temperature);
        gasConfig.addParameter("MuB",         muB);
        gasConfig.addParameter("MuS",         muS);
        ThermalGas * gas = new ThermalGas(name,gasConfig, particleTypes,stableParticleTypes);
        gas->initialize();
        gas->execute();
        ThermalGasVsTempHistograms * hgVsTHistos = (ThermalGasVsTempHistograms*) histogramManager.getGroup(1,0);
        hgVsTHistos->fill(*gas);
        ThermalGasHistograms * hgHistos = (ThermalGasHistograms*) histogramManager.getGroup(0,iMuS);
        hgHistos->fill(*gas);
        count++;
        // keep track of these as subtasks for future in ParticleThermalPropertiesGenerator
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
void ThermalGasModel::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  histogramManager.addSet("ThermalGasHistograms");
  Severity debugLevel = getSeverityLevel();
  String bn = getValueString("HistoBaseName");
  bool    doTempDependentHistos = getValueBool("DoTempDependentHistos");
  HistogramGroup * histos;
  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    String tempLabel = "T";
    tempLabel += int(0.5+1000*(minChemicalTemp+stepTemp*double(iTemp)));
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      String muBLabel = "B";
      muBLabel += int(0.5+1000*(minMuB+stepMuB*double(iMuB)));
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        String muSLabel = "S";
        muSLabel += int(0.5+1000*(minMuS+stepMuS*double(iMuS)));
        histos = new ThermalGasHistograms(this,createName(bn,tempLabel,muBLabel,muSLabel),configuration);
        histos->createHistograms();
        histogramManager.addGroupInSet(0,histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histos = new ThermalGasVsTempHistograms(this,bn+"VsT",configuration);
    histos->createHistograms();
    histogramManager.addSet("ThermalGasVsTempHistograms");
    histogramManager.addGroupInSet(1,histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ThermalGasModel::importHistograms(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  Severity debugLevel = getSeverityLevel();
  histogramManager.addSet("ThermalGasHistograms");
  String bn = getValueString("HistoBaseName");
  bool doTempDependentHistos = getValueBool("DoTempDependentHistos");

  HistogramGroup * histos;
  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    String tempLabel = "T";
    tempLabel += int(0.5+1000*(minChemicalTemp+stepTemp*double(iTemp)));
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      String muBLabel = "B";
      muBLabel += int(0.5+1000*(minMuB+stepMuB*double(iMuB)));
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        String muSLabel = "S";
        muSLabel += int(0.5+1000*(minMuS+stepMuS*double(iMuS)));
        histos = new ThermalGasHistograms(this,createName(bn,tempLabel,muBLabel,muSLabel),configuration);
        histos->importHistograms(inputFile);
        histogramManager.addGroupInSet(0,histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histos = new ThermalGasVsTempHistograms(this,bn+"VsT",configuration);
    histos->importHistograms(inputFile);
    histogramManager.addSet("ThermalGasVsTempHistograms");
    histogramManager.addGroupInSet(1,histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

//void ThermalGasModel::execute()
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
//        ThermalGas * hg = new ThermalGas();
//        addSubtask(hg);
//        hg->initialize();
//        hg->execute();
//        }
//      }
//    }
//  if (reportEnd(__FUNCTION__))
//    ;
//}
