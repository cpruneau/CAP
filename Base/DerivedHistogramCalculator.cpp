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
#include "DerivedHistogramCalculator.hpp"
ClassImp(DerivedHistogramCalculator);

DerivedHistogramCalculator::DerivedHistogramCalculator(const TString &          _name,
                                                       Configuration &          _configuration,
                                                       vector<EventFilter*>     _eventFilters,
                                                       vector<ParticleFilter*>  _particleFilters,
                                                       LogLevel                 _selectedLevel)
:
Task(_name,_configuration,_eventFilters,_particleFilters, _reportLevel)
{
  setInstanceName("DerivedHistogramCalculator");
  appendClassName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void DerivedHistogramCalculator::setDefaultConfiguration()
{
  Configuration & configuration = getConfiguration();
  configuration.addParameter("inputPath", "InputPathUndefined");
  configuration.addParameter("inputFileNameBase", "");
  configuration.addParameter("firstFileIndex", 0);
  configuration.addParameter("lastFileIndex",  1);
  configuration.addParameter("outputPath",     "OutputPathUndefined");
  configuration.addParameter("outputFileName", "OutputFileNameUndefined");
  configuration.addParameter("inputPath", "InputPathUndefined");
  configuration.addParameter("appendedString",TString("_Derived"));
  if (reportDebug("DerivedHistogramCalculator",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}


void DerivedHistogramCalculator::run()
{
  timer.start();
  postTaskOk();

  vector<TString> includePattern;
  includePattern.push_back(histoBaseName+"Part");
  includePattern.push_back(".root");
  //includePattern.push_back(".root");
  vector<TString> excludePattern;
  excludePattern.push_back("Derived");
  excludePattern.push_back("Sum");
  excludePattern.push_back("BF");
  addFileNames(histoInputPath,includePattern,excludePattern);
  addSubtask(new ParticleDerivedHistogramCalculator    (partLabel, partConfig, eventFilters, particleFilters, selectedLevel));
  FileTaskIterator::run();
  
  timer.stop();
  if (reportInfo("DerivedHistogramCalculator",getName(),"run(...)"))
    {
    cout << endl;
    cout << "  Completed with status : " << getTaskStatusName() << endl;
    cout << "           : "; timer.print(cout);
    cout << endl << endl<< endl << endl;
    }
}
