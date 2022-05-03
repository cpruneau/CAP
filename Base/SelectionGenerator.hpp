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

#ifndef CAP__SelectionGenerator
#define CAP__SelectionGenerator

//////////////////////////////////////////////////////////////////////////
// SelectionGenerator
//
// Produces a random integer based
// on a probability distribution.
//
////////////////////////////////////////////////////////////////////////
#include <vector>
#include "TRandom.h"

class SelectionGenerator
{
public:
  SelectionGenerator(std::vector<double> probabilities);
  virtual ~SelectionGenerator(){}
  virtual int generate();
  int nPartitions() const
  {
    return cProbs.size();
  }

protected:

  std::vector<double> cProbs;
  ClassDef(SelectionGenerator,0)
};

#endif /* SelectionGenerator_hpp */
