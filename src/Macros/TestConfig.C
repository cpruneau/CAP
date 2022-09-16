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

int TestConfig()
{
  TString includeBasePath = getenv("CAP_SRC");
  cout << "Path: " << includeBasePath << endl;
  loadBase(includeBasePath);

  std::cout << "==================================================================================" << std::endl;
  std::cout << "TestConfig" << endl;
  std::cout << "==================================================================================" << std::endl;

  Task * t0 = new Task();
  t0->setName("task0");
  Task * t1 = new Task();
  t1->setName("task1");
  t0->addSubTask(t1);
  Task * t2 = new Task();
  t2->setName("task2");
  t1->addSubTask(t2);
  Task * t3 = new Task();
  t3->setName("task3");
  t2->addSubTask(t3);
  Task * t4 = new Task();
  t4->setName("task4");
  t3->addSubTask(t4);

  Task * task = t4->getTaskAt(0);
  cout << "0:" << task->getName() << endl;
  task = t4->getTaskAt(1);
  if (task) cout << "1:" << task->getName() << endl;
  task = t4->getTaskAt(2);
  if (task) cout << "2:" << task->getName() << endl;
  task = t4->getTaskAt(3);
  if (task) cout << "3:" << task->getName() << endl;
  task = t4->getTaskAt(4);
  if (task) cout << "4:" << task->getName() << endl;

  for (int k=0; k<5; k++)
    {
    cout << "Path " << k  << "  is " << t4->getTaskPath(k) << endl;
    }

// Configuration configuration;
//  configuration.addParameter("MyKey", TString("Junk0") );
//  configuration.addParameter("task0:", "MyKey", TString("Junk4") );
//  configuration.addParameter("task0:task1:", "MyKey", TString("Junk3") );
//  configuration.addParameter("task0:task1:task2:", "MyKey", TString("Junk2") );
//  configuration.addParameter("task0:task1:task2:task3:", "MyKey", TString("Junk1") );
//  configuration.addParameter("task0:task1:task2:task3:task4:", "MyKey", TString("Junk1") );
//  configuration.printConfiguration(cout);
//
//  cout << "      " <<  configuration.getValueString("task0:task1:task2", "MyKey") << endl;

//  TString s = "Abc:Defg:hijklmop";
//  TString s2, s3;
//  int f1 =  s.Last(':');
//  int f2 =  s.Length();
//  if (f1>=0 )
//    {
//    s2 = s.Remove(f1,f2-f1);
//    }
//  cout << "   f1: " << f1 << endl;
//  cout << "   f2: " << f2 << endl;
//  cout << "   s2: " << s2 << endl;


//  Configuration c;
//  c.readFromFile("config.txt");
//  c.writeToFile("newConfig.txt");
//  c.printConfiguration(cout);
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
  gSystem->Load(includePath+"CanvasCollection.hpp");
  gSystem->Load(includePath+"HistogramCollection.hpp");
  gSystem->Load(includePath+"Histograms.hpp");
  gSystem->Load(includePath+"Particle.hpp");
  gSystem->Load(includePath+"ParticleType.hpp");
  gSystem->Load(includePath+"ParticleTypeCollection.hpp");
  gSystem->Load(includePath+"ParticleDecayMode.hpp");
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
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
  TString includePath = includeBasePath + "/Particle/";
  gSystem->Load(includePath+"ParticleHistos.hpp");
  gSystem->Load(includePath+"ParticleDerivedHistos.hpp");
  gSystem->Load(includePath+"ParticleAnalyzer.hpp");
  gSystem->Load(includePath+"ParticleDerivedHistogramCalculator.hpp");
  gSystem->Load("libParticle.dylib");
}

void loadPair(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Pair/";
  gSystem->Load(includePath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePairHistos.hpp");
  gSystem->Load(includePath+"ParticlePairDerivedHistos.hpp");
  gSystem->Load(includePath+"ParticlePairCombinedHistos.hpp");
  gSystem->Load(includePath+"ParticlePairDerivedHistogramCalculator.hpp");
  gSystem->Load(includePath+"BalanceFunctionCalculator.hpp");
  gSystem->Load("libPair.dylib");
}

void loadNuDyn(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/NuDyn/";
  gSystem->Load(includePath+"NuDynAnalyzer.hpp");
  gSystem->Load(includePath+"NuDynDerivedHistogramCalculator.hpp");
  gSystem->Load(includePath+"NuDynDerivedHistos.hpp");
  gSystem->Load(includePath+"NuDynHistos.hpp");
  gSystem->Load("libNuDyn.dylib");
}

void loadSubSample(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/SubSample/";
  gSystem->Load(includePath+"SubSampleStatCalculator.hpp");
  gSystem->Load(includePath+"SubSampleStatIterator.hpp");
  gSystem->Load("libSubSample.dylib");
}
