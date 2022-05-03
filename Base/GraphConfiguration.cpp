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

#include "GraphConfiguration.hpp"

ClassImp(GraphConfiguration);

GraphConfiguration::GraphConfiguration()
{
  setDefaultConfiguration();
}

GraphConfiguration::GraphConfiguration(int dim, int type)
:
Configuration()
{
  setDefaultConfiguration();
  setParametersWith(dim,type);
}

void GraphConfiguration::setDefaultConfiguration()
{
  addParameter("npx", 100);
  addParameter("npy", 100);
  addParameter("lineColor", 1);
  addParameter("lineStyle", 1);
  addParameter("lineWidth", 2);
  addParameter("markerColor",   1);
  addParameter("markerStyle",  20);
  addParameter("markerSize",  1.1);
  addParameter("systsColor", 1);
  addParameter("systsStyle", 3003);
  addParameter("systsWidth", 1);
  addParameter("nXDivisions", 5);
  addParameter("xTitleSize", 0.06);
  addParameter("xTitleOffset", 1.1);
  addParameter("xLabelSize",  0.05);
  addParameter("xLabelOffset", 0.01);
  addParameter("xTitle",        "x");
  addParameter("nYDivisions",     5);
  addParameter("yTitleSize",   0.06);
  addParameter("yTitleOffset",  1.2);
  addParameter("yLabelSize",   0.05);
  addParameter("yLabelOffset", 0.01);
  addParameter("yTitle",        "y");
  addParameter("nZDivisions",     5);
  addParameter("zTitleSize",   0.06);
  addParameter("zTitleOffset",  1.5);
  addParameter("zLabelSize",   0.05);
  addParameter("zLabelOffset", 0.01);
  addParameter("zLabelOffset", 0.01);
  addParameter("zTitle",        "z");
  addParameter("plotOption",   "E0");
}

void GraphConfiguration::setParametersWith(int dim, int type)
{
  if (dim==1 || dim==0)
    {
   
    switch (type)
      {
        default:

        case  0:
        setParameter("lineColor",   kBlack);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlack);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlack);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  1:
        setParameter("lineColor",   kRed);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kRed);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kRed);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  2:
        setParameter("lineColor",   kGreen);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kGreen);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kGreen);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  3:
        setParameter("lineColor",   kMagenta);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kMagenta);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kMagenta);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  4:
        setParameter("lineColor",   kBlue);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlue);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlue);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  5:
        setParameter("lineColor",   kYellow);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kYellow);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kYellow);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  6:
        setParameter("lineColor",   kPink);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kPink);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kPink);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  7:
        setParameter("lineColor",   kBlue+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlue+2);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlue+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  8:
        setParameter("lineColor",   kRed+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kRed+2);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kRed+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  9:
        setParameter("lineColor",   kGreen+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kGreen+2);
        setParameter("markerStyle", kFullSquare);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kGreen+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        

        case  10:
        setParameter("lineColor",   kBlack);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlack);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlack);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  11:
        setParameter("lineColor",   kRed);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kRed);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kRed);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  12:
        setParameter("lineColor",   kGreen);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kGreen);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kGreen);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  13:
        setParameter("lineColor",   kMagenta);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kMagenta);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kMagenta);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  14:
        setParameter("lineColor",   kBlue);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlue);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlue);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  15:
        setParameter("lineColor",   kYellow);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kYellow);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kYellow);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  16:
        setParameter("lineColor",   kPink);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kPink);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kPink);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  17:
        setParameter("lineColor",   kBlue+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlue+2);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlue+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  18:
        setParameter("lineColor",   kRed+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kRed+2);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kRed+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  19:
        setParameter("lineColor",   kGreen+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kGreen+2);
        setParameter("markerStyle", kFullCircle);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kGreen+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  20:
        setParameter("lineColor",   kBlack);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlack);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlack);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  21:
        setParameter("lineColor",   kRed);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kRed);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kRed);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  22:
        setParameter("lineColor",   kGreen);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kGreen);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kGreen);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  23:
        setParameter("lineColor",   kMagenta);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kMagenta);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kMagenta);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  24:
        setParameter("lineColor",   kBlue);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlue);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlue);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  25:
        setParameter("lineColor",   kYellow);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kYellow);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kYellow);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  26:
        setParameter("lineColor",   kPink);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kPink);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kPink);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  27:
        setParameter("lineColor",   kBlue+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kBlue+2);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kBlue+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  28:
        setParameter("lineColor",   kRed+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kRed+2);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kRed+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        case  29:
        setParameter("lineColor",   kGreen+2);
        setParameter("lineStyle",   kSolid);
        setParameter("lineWidth",   2);
        setParameter("markerColor", kGreen+2);
        setParameter("markerStyle", kFullDiamond);
        setParameter("markerSize",  1.1);
        setParameter("systsColor",  kGreen+2);
        setParameter("systsStyle",  3003);
        setParameter("systsWidth",  1);
        break;
        
        }
    }
  else if (dim==2)
    {
    addParameter("nXDivisions", 5);
    addParameter("xTitleSize", 0.05);
    addParameter("xTitleOffset", 1.3);
    addParameter("xLabelSize",  0.05);
    addParameter("xLabelOffset", 0.002);
    addParameter("xTitle",        "x");
    addParameter("nYDivisions",     5);
    addParameter("yTitleSize",   0.05);
    addParameter("yTitleOffset",  1.5);
    addParameter("yLabelSize",   0.05);
    addParameter("yLabelOffset", 0.05);
    addParameter("yTitle",        "y");
    addParameter("nZDivisions",     5);
    addParameter("zTitleSize",   0.05);
    addParameter("zTitleOffset",  1.5);
    addParameter("zLabelSize",   0.05);
    addParameter("zLabelOffset",0.005);
    addParameter("zTitle",        "z");
    addParameter("plotOption","SURF3");
    }
}

GraphConfiguration::GraphConfiguration(const GraphConfiguration & source)
:
Configuration(source)
{
  
}


GraphConfiguration & GraphConfiguration::operator=(const GraphConfiguration & source)
{
  if (this!=&source)
    {
    Configuration::operator=(source);
    }
  return *this;
}



vector<GraphConfiguration*> GraphConfiguration::createConfigurationPalette(unsigned int n, int dimension)
{
  vector<GraphConfiguration*> configs;

  for (unsigned int k=0; k<n; k++)
    {
    GraphConfiguration * c = new GraphConfiguration(dimension, k);
    configs.push_back( c );
    }
  return configs;
}
