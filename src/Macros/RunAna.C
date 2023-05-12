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
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
void loadBase(const TString & includeBasePath);
void loadParticles(const TString & includeBasePath);
void loadPythia(const TString & includeBasePath);
void loadPerformance(const TString & includeBasePath);
void loadAmpt(const TString & includeBasePath);
void loadEpos(const TString & includeBasePath);
void loadHijing(const TString & includeBasePath);
void loadHerwig(const TString & includeBasePath);
void loadUrqmd(const TString & includeBasePath);
void loadBasicGen(const TString & includeBasePath);
void loadGlobal(const TString & includeBasePath);
void loadParticle(const TString & includeBasePath);
void loadPair(const TString & includeBasePath);
void loadNuDyn(const TString & includeBasePath);
void loadSubSample(const TString & includeBasePath);
void loadExec(const TString & includeBasePath);
void loadTherminator(const TString & includeBasePath);

// seed provided by slurm or directly by user
// config file must be provided
int RunAna(TString configFile, TString histoOutput="test/", bool isGrid=0)
{
  TString includeBasePath = getenv("CAP_SRC");
  cout << " includeBasePath: " << includeBasePath << endl;
  loadBase(includeBasePath);
  //loadPythia(includeBasePath);
  loadPerformance(includeBasePath);
  //loadAmpt(includeBasePath);
  //loadEpos(includeBasePath);
  //loadHijing(includeBasePath);
  //loadHerwig(includeBasePath);
  //loadUrqmd(includeBasePath);
  loadBasicGen(includeBasePath);
  loadGlobal(includeBasePath);
  loadParticle(includeBasePath);
  loadPair(includeBasePath);
  loadNuDyn(includeBasePath);
  loadSubSample(includeBasePath);
  loadTherminator(includeBasePath);
  loadExec(includeBasePath);


  try
  {
  cout << "------------------------------------------------------------------------------------------------------" << endl;
  cout << "------------------------------------------------------------------------------------------------------" << endl;
  cout << "RunAna()"  << endl;
  CAP::Configuration configuration;
  TString configurationPath = getenv("CAP_CONFIG");
  TString configurationFile = configFile;
  cout << "Configuration path......... : " << configurationPath << endl;
  cout << "Configuration file......... : " << configurationFile << endl;
  configuration.readFromFile(configurationPath,configurationFile);
  if (isGrid)
    {
    // overide the ini file
    TString dbPath = getenv("CAP_DATABASE");
    dbPath  += "ParticleData/";
    cout << "DB path.................... : " << dbPath << endl;
    configuration.addParameter("Run:Analysis:PartGen:HistogramsExportPath",histoOutput);
    configuration.addParameter("Run:Analysis:PairGen:HistogramsExportPath",histoOutput);
    configuration.addParameter("Run:ParticleDb:ParticleDbImportPath",dbPath);
    }
  cout << "------------------------------------------------------------------------------------------------------" << endl;
  cout << "------------------------------------------------------------------------------------------------------" << endl;
  CAP::RunAnalysis * analysis = new CAP::RunAnalysis("Run", configuration);
  analysis->configure();
  analysis->execute();
  }
  catch (CAP::FileException exception)
  {
  exception.print();
  exit(1);
  }
  catch (CAP::Exception exception)
  {
  exception.print();
  exit(1);
  }

  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Configuration.hpp");
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load(includePath+"TaskIterator.hpp");
  gSystem->Load(includePath+"Collection.hpp");
  gSystem->Load(includePath+"HistogramCollection.hpp");
  //gSystem->Load(includePath+"Histograms.hpp");
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
}

void loadParticles(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Particles/";
  gSystem->Load(includePath+"Particle.hpp");
  gSystem->Load(includePath+"ParticleType.hpp");
  gSystem->Load(includePath+"ParticleTypeCollection.hpp");
  gSystem->Load(includePath+"ParticleDecayMode.hpp");
  gSystem->Load("libParticles.dylib");
}


void loadPythia(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/CAPPythia/";
  gSystem->Load(includePath+"PythiaEventGenerator.hpp");
  gSystem->Load(includePath+"PythiaEventReader.hpp");
  gSystem->Load("libCAPPythia.dylib");
}

void loadPerformance(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Performance/";
  gSystem->Load(includePath+"ClosureCalculator.hpp");
  gSystem->Load(includePath+"ClosureIterator.hpp");
  gSystem->Load(includePath+"MeasurementPerformanceSimulator.hpp");
  gSystem->Load(includePath+"ParticlePerformanceSimulator.hpp");
  gSystem->Load(includePath+"ParticlePerformanceAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePerformanceHistos.hpp");
  gSystem->Load("libPerformance.dylib");
}

void loadAmpt(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Ampt/";
  gSystem->Load(includePath+"AmptEventReader.hpp");
  gSystem->Load("libAmpt.dylib");
}

void loadEpos(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Epos/";
  gSystem->Load(includePath+"EposEventReader.hpp");
  gSystem->Load("libEpos.dylib");
}

void loadHijing(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Hijing/";
  gSystem->Load(includePath+"HijingEventReader.hpp");
  gSystem->Load("libHijing.dylib");
}

void loadHerwig(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Herwig/";
  gSystem->Load(includePath+"HerwigEventReader.hpp");
  gSystem->Load("libHerwig.dylib");
}

void loadUrqmd(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Urqmd/";
  gSystem->Load(includePath+"UrqmdEventReader.hpp");
  gSystem->Load("libUrqmd.dylib");
}


void loadBasicGen(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/BasicGen/";
  gSystem->Load(includePath+"GaussianGeneratorTask.hpp");
  gSystem->Load(includePath+"RadialBoostHistos.hpp");
  gSystem->Load(includePath+"RadialBoostTask.hpp");
  gSystem->Load(includePath+"RapidityGenerator.hpp");
  gSystem->Load("libBasicGen.dylib");
}

void loadGlobal(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Global/";
  gSystem->Load(includePath+"GlobalAnalyzer.hpp");
  gSystem->Load(includePath+"GlobalHistos.hpp");
  gSystem->Load(includePath+"TransverseSpherocityHistos.hpp");
  gSystem->Load(includePath+"TransverseSpherocityAnalyzer.hpp");
  gSystem->Load("libBasicGen.dylib");
}

void loadParticle(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/ParticleSingle/";
  gSystem->Load(includePath+"ParticleSingleHistos.hpp");
  gSystem->Load(includePath+"ParticleSingleDerivedHistos.hpp");
  gSystem->Load(includePath+"ParticleSingleAnalyzer.hpp");
  gSystem->Load("libParticleSingle.dylib");
}

void loadPair(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/ParticlePair/";
  gSystem->Load(includePath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePairHistos.hpp");
  gSystem->Load(includePath+"ParticlePairDerivedHistos.hpp");
  gSystem->Load(includePath+"BalanceFunctionCalculator.hpp");
  gSystem->Load("libParticlePair.dylib");
}

void loadNuDyn(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/NuDyn/";
  gSystem->Load(includePath+"NuDynAnalyzer.hpp");
  gSystem->Load(includePath+"NuDynDerivedHistos.hpp");
  gSystem->Load(includePath+"NuDynHistos.hpp");
  gSystem->Load("libNuDyn.dylib");
}

void loadSubSample(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/SubSample/";
  gSystem->Load(includePath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libSubSample.dylib");
}

void loadExec(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Exec/";
  gSystem->Load(includePath+"RunAnalysis.hpp");
  gSystem->Load("libExec.dylib");
}

void loadTherminator(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Therminator/";
  gSystem->Load(includePath+"Model.hpp");
  gSystem->Load("libTherminator.dylib");
}
