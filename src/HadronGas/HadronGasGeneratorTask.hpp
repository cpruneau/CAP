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

#ifndef CAP_HadronGasGeneratorTask
#define CAP_HadronGasGeneratorTask
#include "HadronGas.hpp"
//#include "MomentumGenerator.hpp"

class HadronGasGeneratorTask : public Task
{
public:

  HadronGasGeneratorTask(const TString & _name,
                         Configuration & _configuration);
  virtual ~HadronGasGeneratorTask() {}
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);

protected:
  ParticleTypeCollection *   particleTypes;
  ParticleTypeCollection *   stableParticleTypes;
  int nThermalSpecies;
  int nStableSpecies;

  TString modelName;
  int     nChemicalTemp;
  double  minChemicalTemp;
  double  maxChemicalTemp;
  double  stepTemp;
  int     nMuB;
  double  minMuB;
  double  maxMuB;
  double  stepMuB;
  int     nMuS;
  double  minMuS;
  double  maxMuS;
  double  stepMuS;

  ClassDef(HadronGasGeneratorTask,0)
};

#endif /* CAP_HadronGasGeneratorTask */
