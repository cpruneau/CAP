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

#ifndef CAP__LegendConfiguration
#define CAP__LegendConfiguration

//!
//!
//! LegendConfiguration
//!
//! Class used to describe a graph configuration
//!
//!
#include <iostream>
#include <vector>
#include "Aliases.hpp"
#include "TLatex.h"
#include "Configuration.hpp"
using std::vector;

namespace CAP
{


//!
//! Class encapsulates the  properties or attributes of the legend of a graph.
//!
class LegendConfiguration : public Configuration
{
public:

  //!
  //!Default CTOR
  //!
  LegendConfiguration();

  LegendConfiguration(double _xLeft,
                      double _xRight,
                      double _yLow,
                      double _yHigh,
                      double _textSize);

  LegendConfiguration(double _xLeft,
                      double _xRight,
                      double _yLow,
                      double _yHigh,
                      double _textSize,
                      VectorString  & _entries);
  //!
  //!Copy CTOR
  //!
  LegendConfiguration(const LegendConfiguration & source);

  //!
  //!DTOR
  //!
  virtual ~LegendConfiguration(){}

  //!
  //!Assignment operator (COPY)
  //!
  LegendConfiguration & operator=(const LegendConfiguration & source);
  
  //!
  //! Set the default graph configuration
  //!
  virtual void setDefaultConfiguration();

  void addLegend(const String & legend);
  String getLegendAt(int index) const;
  void setLegendAt(int index,const String & legend);

  bool useLegend() const
  {
  return getValueBool("useLegend");
  }


  bool useLabels()  const
  {
    return getValueBool("useLabels") && labels.size()>0;
  }

  int getNLabels() const
  {
  return labels.size();
  }

  void addLabel(const String & text, double x, double y, double angle, int color,  double fontSize);

  TLatex * getLabelAt(int index) const;

  void drawLabels();

  VectorString  getTexts() const
  {
  return legends;
  }

protected:

//  bool drawLegend;
//  bool drawBorder;
//  int  nColumns;
//  int  borderColor;
//  int  borderSize;
//  int  fillColor;
//  int  fillStyle;
//  int  fontIndex;
//  int  fontColor;
//  int  fontSize;
//  int  textAlign;
//  double xLeft;
//  double xRight;
//  double yLow;
//  double yHigh;
//  String header;

  VectorString  legends;
  vector<TLatex*> labels;

  ClassDef(LegendConfiguration,0)
};

} // namespace CAP

#endif
