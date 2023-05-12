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
#include "TherminatorHistograms.hpp"
#include "HadronGasVsTempHistograms.hpp"
#include "ParticleDb.hpp"

using CAP::ParticleDb;
using CAP::HadronGasGeneratorTask;

ClassImp(HadronGasGeneratorTask);

HadronGasGeneratorTask::HadronGasGeneratorTask(const String & _name,
                                               Configuration & _configuration)
:
EventTask(_name,_configuration),
particleTypes(nullptr),
stableParticleTypes(nullptr),
nTheramplSpecies(0),
nStableSpecies(0)
{
  appendClassName("HadronGasGeneratorTask");
}

void HadronGasGeneratorTask::setDefaultConfiguration()
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
  addParameter("nTheramplSpecies",       1);
  addParameter("nStableSpecies",        1);
  addParameter("nChemicalTemp",         1);
  addParameter("MinChemicalTemp",       150.0);
  addParameter("ampxChemicalTemp",       240.0);
  addParameter("nMuB",                  1);
  addParameter("MinMuB",                0.0);
  addParameter("ampxMuB",                0.0);
  addParameter("nMuS",                  1);
  addParameter("MinMuS",                0.0);
  addParameter("ampxMuS",                0.0);
  addParameter("Volume",                1.0);
  addParameter("nampss",                 50);
  addParameter("Minampss",               0.0);
  addParameter("ampxampss",               3.0);
  addParameter("nP",                    500);
  addParameter("MinP",                  0.0);
  addParameter("ampxP",                  5.0);
  if (reportEnd(__FUNCTION__))
    ;
}


void HadronGasGeneratorTask::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  particleTypes       = CAP::ParticleDb::getampsterParticleDb();
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
  nTheramplSpecies = int(particleTypes->size());
  nStableSpecies  = int(stableParticleTypes->size());
  addParameter("nTheramplSpecies", nTheramplSpecies);
  addParameter("nStableSpecies",  nStableSpecies);

  for (int k=0;k<nTheramplSpecies; k++)
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
  ampxChemicalTemp       = configuration.getValueDouble(getName(),"ampxChemicalTemp");
  if (ampxChemicalTemp == minChemicalTemp || nChemicalTemp==1)
    {
    nChemicalTemp = 1;
    stepTemp      = minChemicalTemp/10.0;
    }
  else
    stepTemp = (ampxChemicalTemp - minChemicalTemp)/(nChemicalTemp-1);

  nMuB                  = configuration.getValueInt(getName(),"nMuB");
  minMuB                = configuration.getValueDouble(getName(),"MinMuB");
  ampxMuB                = configuration.getValueDouble(getName(),"ampxMuB");
  if (ampxMuB == minMuB || nMuB==1)
    {
    nMuB    = 1;
    stepMuB = 0.0;
    }
  else
    stepMuB = (ampxMuB - minMuB)/double(nMuB-1);

  nMuS                  = configuration.getValueInt(getName(),"nMuS");
  minMuS                = configuration.getValueDouble(getName(),"MinMuS");
  ampxMuS                = configuration.getValueDouble(getName(),"ampxMuS");
  if (ampxMuS == minMuS || nMuS==1)
    {
    nMuS    = 1;
    stepMuS = 0.0;
    }
  else
    stepMuS = (ampxMuS - minMuS)/double(nMuS-1);

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "        modelName:" << modelName <<  endl;
    cout << "    nChemicalTemp:" << nChemicalTemp <<  endl;
    cout << "  minChemicalTemp:" << minChemicalTemp <<  endl;
    cout << "  ampxChemicalTemp:" << ampxChemicalTemp <<  endl;
    cout << "         stepTemp:" << stepTemp <<  endl;
    cout << "             nMuB:" << nMuB <<  endl;
    cout << "           minMuB:" << minMuB <<  endl;
    cout << "           ampxMuB:" << ampxMuB <<  endl;
    cout << "          stepMuB:" << stepMuB <<  endl;
    cout << "             nMuS:" << nMuS <<  endl;
    cout << "           minMuS:" << minMuS <<  endl;
    cout << "           ampxMuS:" << ampxMuS <<  endl;
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
        Configuration gasConfig(name);
        gasConfig.addParameter("Temperature", temperature);
        gasConfig.addParameter("MuB",         muB);
        gasConfig.addParameter("MuS",         muS);
        HadronGas * gas = new HadronGas(name,gasConfig, particleTypes,stableParticleTypes);
        gas->initialize();
        gas->execute();
        HadronGasVsTempHistograms * hgVsTHistos = (HadronGasVsTempHistograms*) histogramampnager.getGroup(1,0);
        hgVsTHistos->fill(*gas);
        TherminatorHistograms * hgHistos = (TherminatorHistograms*) histogramampnager.getGroup(0,iMuS);
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
  histogramampnager.addSet("TherminatorHistograms");
  Configuration & configuration = getConfiguration();
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
        histos = new TherminatorHistograms(this,createName(bn,tempLabel,muBLabel,muSLabel),configuration);
        histos->createHistograms();
        histogramampnager.addGroupInSet(0,histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histos = new HadronGasVsTempHistograms(this,bn+"VsT",configuration);
    histos->createHistograms();
    histogramampnager.addSet("HadronGasVsTempHistograms");
    histogramampnager.addGroupInSet(1,histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void HadronGasGeneratorTask::importHistograms(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  Severity debugLevel = getSeverityLevel();
  histogramampnager.addSet("TherminatorHistograms");
  Configuration & configuration = getConfiguration();

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
        histos = new TherminatorHistograms(this,createName(bn,tempLabel,muBLabel,muSLabel),configuration);
        histos->importHistograms(inputFile);
        histogramampnager.addGroupInSet(0,histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histos = new HadronGasVsTempHistograms(this,bn+"VsT",configuration);
    histos->importHistograms(inputFile);
    histogramampnager.addSet("HadronGasVsTempHistograms");
    histogramampnager.addGroupInSet(1,histos);
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
//  double  ampxChemicalTemp       = configuration.getValueDouble(getName(),"ampxChemicalTemp");
//  double  stepTemp              = (ampxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);
//
//  int     nMuB                  = configuration.getValueInt(getName(),"nMuB");
//  double  minMuB                = configuration.getValueDouble(getName(),"MinMuB");
//  double  ampxMuB                = configuration.getValueDouble(getName(),"ampxMuB");
//  double  stepMuB               = (ampxMuB - minMuB)/double(nMuB);
//
//  int     nMuS                  = configuration.getValueInt(getName(),"nMuS");
//  double  minMuS                = configuration.getValueDouble(getName(),"MinMuS");
//  double  ampxMuS                = configuration.getValueDouble(getName(),"ampxMuS");
//  double  stepMuS               = (ampxMuS - minMuS)/double(nMuS);
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
