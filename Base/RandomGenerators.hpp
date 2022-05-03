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
#ifndef CAP__RandomGenerators
#define CAP__RandomGenerators
#include "TRandom.h"
#include "TH1.h"
#include "TH2.h"

class RandomGenerator
{
public:
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////
  RandomGenerator(TRandom * rg);
  virtual ~RandomGenerator(){}
  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  TRandom * randomGenerator;

  ClassDef(RandomGenerator,0)
};




class ScalarIntRandomGenerator : public RandomGenerator
{
public:
  ScalarIntRandomGenerator(int min, int max, TRandom * rg);
  virtual ~ScalarIntRandomGenerator(){}

  virtual int generate();

  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  int minimum;
  int maximum;
  int range;

  ClassDef(ScalarIntRandomGenerator,0)
};


///////////////////////////////////////////////////////////////////////////////////
// Returns A Fixed (Preselected) Integer
///////////////////////////////////////////////////////////////////////////////////
class FixedScalarIntRandomGenerator : public ScalarIntRandomGenerator
{
public:
  FixedScalarIntRandomGenerator(int fixedValue);
  virtual ~FixedScalarIntRandomGenerator(){}
  virtual int generate();

  ClassDef(FixedScalarIntRandomGenerator,0)
};

///////////////////////////////////////////////////////////////////////////////////
// Returns exponentially distributed integers above given minimum
///////////////////////////////////////////////////////////////////////////////////
class ExpScalarIntRandomGenerator : public ScalarIntRandomGenerator
{
public:
  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  double slope;

  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////
  ExpScalarIntRandomGenerator(int    minimum,
                              double theSlope,
                              TRandom * rg);
  virtual ~ExpScalarIntRandomGenerator(){}
  virtual int generate();

  ClassDef(ExpScalarIntRandomGenerator,0)

};


///////////////////////////////////////////////////////////////////////////////////
// Generates Random Doubles in a bounded range w/ uniform probability
///////////////////////////////////////////////////////////////////////////////////
class ScalarDoubleRandomGenerator  : public RandomGenerator
{
public:

  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////

  ScalarDoubleRandomGenerator(double min,double max,TRandom * rg);
 virtual ~ScalarDoubleRandomGenerator(){}
  virtual double generate();

  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  double minimum;
  double maximum;
  double range;

  ClassDef(ScalarDoubleRandomGenerator,0)

};


///////////////////////////////////////////////////////////////////////////////////
// Returns A Fixed (Preselected) Double
///////////////////////////////////////////////////////////////////////////////////
class FixedScalarDoubleRandomGenerator : public ScalarDoubleRandomGenerator
{
public:
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////
  FixedScalarDoubleRandomGenerator(double fixedValue);
  virtual ~FixedScalarDoubleRandomGenerator(){}
  virtual double generate();

  ClassDef(FixedScalarDoubleRandomGenerator,0)
};

class LinearScalarDoubleRandomGenerator : public ScalarDoubleRandomGenerator
{
public:
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////

  LinearScalarDoubleRandomGenerator(double minimum,double maximum, TRandom * randomGenerator);
  virtual ~LinearScalarDoubleRandomGenerator(){}
  virtual double  generate();

  ClassDef(LinearScalarDoubleRandomGenerator,0)
};



class ExpScalarDoubleRandomGenerator : public ScalarDoubleRandomGenerator
{
public:
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////

  ExpScalarDoubleRandomGenerator(double minimum,
                                 double slope,
                                 TRandom * randomGenerator);
  virtual ~ExpScalarDoubleRandomGenerator(){}
  virtual double  generate();

  ClassDef(ExpScalarDoubleRandomGenerator,0)
};

class HistoScalarDoubleRandomGenerator : public ScalarDoubleRandomGenerator
{
public:

  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  TH1 * histogram;

  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////

  HistoScalarDoubleRandomGenerator(TH1 * theHistogram);
  virtual ~HistoScalarDoubleRandomGenerator(){}
  virtual double  generate();

  ClassDef(HistoScalarDoubleRandomGenerator,0)
};


///////////////////////////////////////////////////////////////////////////////////
// Generates a unit vector uniformly distributed in 4pi
///////////////////////////////////////////////////////////////////////////////////
class VectorRandomGenerator :  public RandomGenerator
{
public:
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////

  VectorRandomGenerator(TRandom * rg);
  virtual ~VectorRandomGenerator(){}
  virtual void generate(double &theta, double &phi, double & ax, double & ay,double & az);

  ClassDef(VectorRandomGenerator,0)
};

#endif /* RandomGenerators_hpp */
