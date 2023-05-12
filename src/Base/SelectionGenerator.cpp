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
using CAP::SelectionGenerator;

ClassImp(SelectionGenerator);

SelectionGenerator::SelectionGenerator()
:
cumulativeProbability()
{    }

SelectionGenerator::SelectionGenerator(std::vector<double> & probabilities)
:
cumulativeProbability()
{
  initializeWith(probabilities);
}

void SelectionGenerator::initializeWith(std::vector<double> & probabilities)
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
    cumulativeProbability.push_back(prob);
    }
}


int SelectionGenerator::generate()
{
  double v = gRandom->Rndm();
  int n = cumulativeProbability.size();
  for (int k=0; k<n; k++)
  {
  if (v <= cumulativeProbability[k]) return k;
  }
  return n-1;
}

