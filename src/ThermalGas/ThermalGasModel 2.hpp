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
#ifndef CAP_ThermalGasModel
#define CAP_ThermalGasModel
#include "ThermalGas.hpp"
#include "EventTask.hpp"

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
  virtual void execute();
  virtual void createHistograms();
  virtual void importHistograms(TFile & inputFile);

protected:
  ParticleDb *   particleTypes;
  ParticleDb *   stableParticleTypes;
  int nThermalSpecies;
  int nStableSpecies;

  String modelName;
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

  ClassDef(ThermalGasModel,0)
};

} // namespace CAP

#endif /* CAP_ThermalGasModel */
