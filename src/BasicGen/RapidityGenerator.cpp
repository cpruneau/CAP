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
#include "RapidityGenerator.hpp"
using CAP::RapidityGenerator;

ClassImp(RapidityGenerator);

RapidityGenerator::RapidityGenerator(int opt, int pair, double max, double s, double minSep)
:
option(opt),
pairMode(pair),
maxRapidity(max),
scale(s),
minimumSeparation(minSep),
nStored(0),
pairRapidity(0),
saveRapidity(0),
rank(0)
{
  r = new TRandom();
}


void RapidityGenerator::reset()
{
  nStored = 0;
}

double RapidityGenerator::generate()
{
  double v;
  double localRap;
  double rap1, rap2;
  bool OK = false;

  if (pairMode==0)
    {
    // Generate clusters independently.
    while (!OK)
      {
      switch (option)
        {
          case 0:  // uniform

          v = maxRapidity * (2.0*r->Rndm() - 1.0);
          break;

          case 1:  // exponential

          v = -99999;
          while (fabs(v)>maxRapidity)
            {
            v = 0.5*minimumSeparation + r->Exp(scale);
            v = (r->Rndm()>0.50) ? v : -v;
            }
          break;

          case 2:  // Gaussian

          v = -99999;
          while (fabs(v)>maxRapidity)
            {
            v = 0.5*minimumSeparation + r->Gaus(0.0, scale);
            }
          break;
        }

      OK = true;
      for (int k=0; k<nStored; k++)
        {
        if (fabs(v-store[k])<minimumSeparation)
          {
          OK = false;
          break;
          }
        }

      }
    }
  else
    {
    // Generate clusters in pairs.

    if (rank==0)
      {
      while (!OK)
        {
        // generate a new pair average rapidity
        pairRapidity = maxRapidity * (2.0*r->Rndm() - 1.0);
        switch (option)
          {
            case 0:  // uniform
            //cout << "Option not available/sensible" << endl;
            localRap = 0.5;
            break;
            case 1:  // exponential
            localRap = -99999;
            while (fabs(localRap)>2*maxRapidity)
              {
              localRap = r->Exp(scale);
              }
            break;
            case 2:  // Gaussian
            localRap = -99999;
            while (fabs(localRap)>2*maxRapidity)
              {
              localRap = r->Gaus(0.0, scale);
              }
            break;
          }
        rap1 = pairRapidity + localRap;
        rap2 = pairRapidity - localRap;
        OK = true;
        for (int k=0; k<nStored; k++)
          {
          if (fabs(rap1-store[k])<minimumSeparation || fabs(rap2-store[k])<minimumSeparation)
            {
            OK = false;
            break;
            }
          }
        }
      saveRapidity = rap2;
      v = rap1;
      rank = 1; // next we use saveRapidity
      }
    else
      {
      v = saveRapidity;
      rank = 0; // next we calculate a new pair
      }
    }
  return v;
  }
