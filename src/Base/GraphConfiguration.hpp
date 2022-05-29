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

#ifndef CAP__GraphConfiguration
#define CAP__GraphConfiguration

//!
//!
//! GraphConfiguration
//!
//! Class used to describe a graph configuration
//!
//!
#include <iostream>
#include "TString.h"
#include "TAttMarker.h"
#include "TAttLine.h"
#include <vector>
#include "Configuration.hpp"

using namespace std;
using std::vector;

//!
//! Class encapsulates the plotting properties or attributes of a graph or histograms.
//! This class is designed to work with other CAP graphics classes to set the plotting
//! attributes of graphs (TGraph), histograms (TH1*), and data graphs (DataGraph) displaying
//! data values, statistical error bars, and systematic error bars.
//!
//! Lines are used to draw connections between points as well as  (statistical) error bars.
//! Markers are used to represent the data point position/value
//! Systs are used to represent the systematic error bars or bounds.
//!
class GraphConfiguration : public Configuration
{
public:

  //!
  //!Default CTOR
  //!
  GraphConfiguration();

  GraphConfiguration(int d=1, int type=1);

  //!
  //!Copy CTOR
  //!
  GraphConfiguration(const GraphConfiguration & source);

  //!
  //!DTOR
  //!
  virtual ~GraphConfiguration(){}

  //!
  //!Assignment operator (COPY)
  //!
  GraphConfiguration & operator=(const GraphConfiguration & other);
  
  //!
  //! Set the default graph configuration
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Set the graph parameter for dim =1 or  =2 and according to the type.
  //!
  void setParametersWith(int dim, int type);

  //!
  //! Create a graph configuration palette.
  //!
  static vector<GraphConfiguration*> createConfigurationPalette(unsigned int n, int dimension);

  ClassDef(GraphConfiguration,0)
};

#endif
