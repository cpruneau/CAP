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
using CAP::GraphConfiguration;

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
  addParametersWith(dim,type);
}

void GraphConfiguration::setDefaultConfiguration()
{
  addParameter("npx", 100);
  addParameter("npy", 100);
  addParameter("lineColor", 1);
  addParameter("lineStyle", 1);
  addParameter("lineWidth", 1);
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
  addParameter("xTitle",       TString("x"));
  addParameter("nYDivisions",     5);
  addParameter("yTitleSize",   0.06);
  addParameter("yTitleOffset",  1.2);
  addParameter("yLabelSize",   0.05);
  addParameter("yLabelOffset", 0.01);
  addParameter("yTitle",        TString("y"));
  addParameter("nZDivisions",     5);
  addParameter("zTitleSize",   0.06);
  addParameter("zTitleOffset",  1.5);
  addParameter("zLabelSize",   0.05);
  addParameter("zLabelOffset", 0.01);
  addParameter("zLabelOffset", 0.01);
  addParameter("zTitle",        TString("z"));
  addParameter("PlotOption",    TString("E0"));

  addParameter("systsColor",1);
  addParameter("systsStyle",1001);
  addParameter("systsWidth",1);
}

void GraphConfiguration::addParametersWith(int dim, int type)
{
  if (dim==1 || dim==0)
    {
   
    switch (type)
      {
        default:

        case  0:
        addParameter("lineColor",   kBlack);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlack);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlack);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  1:
        addParameter("lineColor",   kRed);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kRed);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kRed);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  2:
        addParameter("lineColor",   kGreen);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kGreen);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kGreen);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  3:
        addParameter("lineColor",   kMagenta);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kMagenta);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kMagenta);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  4:
        addParameter("lineColor",   kBlue);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlue);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlue);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  5:
        addParameter("lineColor",   kYellow);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kYellow);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kYellow);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  6:
        addParameter("lineColor",   kPink);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kPink);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kPink);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  7:
        addParameter("lineColor",   kBlue+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlue+2);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlue+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  8:
        addParameter("lineColor",   kRed+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kRed+2);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kRed+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  9:
        addParameter("lineColor",   kGreen+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kGreen+2);
        addParameter("markerStyle", kFullSquare);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kGreen+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        

        case  10:
        addParameter("lineColor",   kBlack);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlack);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlack);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  11:
        addParameter("lineColor",   kRed);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kRed);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kRed);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  12:
        addParameter("lineColor",   kGreen);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kGreen);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kGreen);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  13:
        addParameter("lineColor",   kMagenta);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kMagenta);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kMagenta);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  14:
        addParameter("lineColor",   kBlue);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlue);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlue);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  15:
        addParameter("lineColor",   kYellow);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kYellow);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kYellow);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  16:
        addParameter("lineColor",   kPink);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kPink);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kPink);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  17:
        addParameter("lineColor",   kBlue+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlue+2);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlue+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  18:
        addParameter("lineColor",   kRed+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kRed+2);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kRed+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  19:
        addParameter("lineColor",   kGreen+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kGreen+2);
        addParameter("markerStyle", kFullCircle);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kGreen+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  20:
        addParameter("lineColor",   kBlack);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlack);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlack);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  21:
        addParameter("lineColor",   kRed);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kRed);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kRed);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  22:
        addParameter("lineColor",   kGreen);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kGreen);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kGreen);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  23:
        addParameter("lineColor",   kMagenta);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kMagenta);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kMagenta);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  24:
        addParameter("lineColor",   kBlue);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlue);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlue);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  25:
        addParameter("lineColor",   kYellow);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kYellow);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kYellow);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  26:
        addParameter("lineColor",   kPink);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kPink);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kPink);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  27:
        addParameter("lineColor",   kBlue+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kBlue+2);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kBlue+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  28:
        addParameter("lineColor",   kRed+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kRed+2);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kRed+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
        break;
        
        case  29:
        addParameter("lineColor",   kGreen+2);
        addParameter("lineStyle",   kSolid);
        addParameter("lineWidth",   2);
        addParameter("markerColor", kGreen+2);
        addParameter("markerStyle", kFullDiamond);
        addParameter("markerSize",  1.1);
        addParameter("systsColor",  kGreen+2);
        addParameter("systsStyle",  3003);
        addParameter("systsWidth",  1);
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
    addParameter("xTitle",        TString("x"));
    addParameter("nYDivisions",     5);
    addParameter("yTitleSize",   0.05);
    addParameter("yTitleOffset",  1.5);
    addParameter("yLabelSize",   0.05);
    addParameter("yLabelOffset", 0.05);
    addParameter("yTitle",        TString("y"));
    addParameter("nZDivisions",     5);
    addParameter("zTitleSize",   0.05);
    addParameter("zTitleOffset",  1.5);
    addParameter("zLabelSize",   0.05);
    addParameter("zLabelOffset",0.005);
    addParameter("zTitle",        TString("z"));
    addParameter("PlotOption",TString("SURF3"));
    }
}

GraphConfiguration::GraphConfiguration(GraphConfiguration & source)
:
Configuration(source)
{
  
}


GraphConfiguration & GraphConfiguration::operator=(GraphConfiguration & source)
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
