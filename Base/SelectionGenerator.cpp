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
#include <vector>
#include "SelectionGenerator.hpp"

ClassImp(SelectionGenerator);

SelectionGenerator::SelectionGenerator(std::vector<double> probabilities)
:
cProbs()
{
  int n = probabilities.size();
  double sum = 0.0;
  for (int k=0; k<n; k++)
  {
  sum += probabilities[k];
  }
  double prob = 0.0;
  for (int k=0; k<n; k++)
  {
  prob += probabilities[k]/sum;
  cProbs.push_back(prob);
  }
}

int SelectionGenerator::generate()
{
  double v = gRandom->Rndm();
  int n = cProbs.size();
  for (int k=0; k<n; k++)
  {
  if (v <= cProbs[k]) return k;
  }
  return n-1;
}

