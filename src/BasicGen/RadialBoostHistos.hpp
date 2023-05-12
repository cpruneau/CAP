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
#ifndef CAP__RadialBoostHistos
#define CAP__RadialBoostHistos
#include "HistogramGroup.hpp"
#include "Configuration.hpp"

namespace CAP
{

class RadialBoostHistos : public HistogramGroup
{
public:

  RadialBoostHistos(Task * _partial,
                    const String & _name,
                    const Configuration & _configuration);
  virtual ~RadialBoostHistos() {}
  virtual void createHistograms();
  virtual void importHistograms(TFile & inputFile);
  virtual void fill(double x, double y, double r, double phi, double beta, double weight);

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
  ////////////////////////////////////////////////////////////////////////////
  TH1 * h_phi;
  TH1 * h_r;
  TH1 * h_beta;
  TH2 * h_xy;

  ClassDef(RadialBoostHistos,0)

};

}

#endif /* CAP__RadialBoostHistos  */



