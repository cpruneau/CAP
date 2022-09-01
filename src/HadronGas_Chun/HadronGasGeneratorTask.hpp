// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_HadronGasGeneratorTask
#define WAC_HadronGasGeneratorTask
#include "NucleonNucleonCollisionGenerator.hpp"
#include "HadronGas.hpp"
#include "HadronGasHistograms.hpp"
#include "MomentumGenerator.hpp"

class HadronGasGeneratorTask : public Task
{
public:

  HadronGasGeneratorTask(const TString  & _name,
                         Configuration  & _configuration);
  virtual ~HadronGasGeneratorTask() {}
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();
  virtual void generate(Particle * parent);
  
protected:

  ParticleTypeCollection *   particleTypes;
  ParticleTypeCollection *   stableParticleTypes;
  vector<HadronGas*>         hadronGases;
  vector<MomentumGenerator*> momentumGenerators;
  TH1 * relativeAbundances;
  TH1 * relativeAbundancesGas;
  bool standaloneMode;
  int  minTotalMult;
  int  maxTotalMult;
  int  rangeTotalMult;
  double  minY;
  double  maxY;
  double  rangeY;
  ClassDef(HadronGasGeneratorTask,0)
};

#endif /* WAC_HadronGasGeneratorTask */
