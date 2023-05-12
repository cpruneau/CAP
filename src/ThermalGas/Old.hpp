// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef CAP_ThermalGasModel
#define CAP_ThermalGasModel
#include "EventTask.hpp"
#include "Event.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"
#include "NucleonNucleonCollisionGenerator.hpp"
#include "ThermalGas.hpp"
#include "ThermalGasHistograms.hpp"
#include "MomentumGenerator.hpp"

namespace CAP
{


class ThermalGasModel : public EventTask
{
public:

  ThermalGasModel(const String & _name,
                         const Configuration & _configuration);
  virtual ~ThermalGasModel() {}
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void createEvent();

protected:
  ParticleDb *   particleTypes;
  ParticleDb *   stableParticleTypes;
  ClassDef(ThermalGasModel,0)
};

} // namespace CAP


#endif /* CAP_ThermalGasModel */
