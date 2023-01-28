// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef CAP_HadronGasGeneratorTask
#define CAP_HadronGasGeneratorTask
#include "NucleonNucleonCollisionGenerator.hpp"
#include "HadronGas.hpp"
#include "HadronGasHistograms.hpp"
#include "MomentumGenerator.hpp"

namespace CAP
{


class HadronGasGeneratorTask : public Task
{
public:

  HadronGasGeneratorTask(const String & _name,
                         Configuration & _configuration);
  virtual ~HadronGasGeneratorTask() {}
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();

protected:
  ParticleTypeCollection *   particleTypes;
  ParticleTypeCollection *   stableParticleTypes;
  ClassDef(HadronGasGeneratorTask,0)
};

} // namespace CAP


#endif /* CAP_HadronGasGeneratorTask */
