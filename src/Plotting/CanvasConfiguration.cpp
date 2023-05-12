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
#include "CanvasCollection.hpp"
using CAP::CanvasConfiguration;

ClassImp(CanvasConfiguration);

CanvasConfiguration::CanvasConfiguration()
:
Configuration()
{
  setDefaultConfiguration();
}



CanvasConfiguration::CanvasConfiguration(CanvasFormat format, CanvasAxes axes)
:
Configuration()
{
  setDefaultConfiguration();
  // chose the aspect ratio and the margin style (tight, regular, wide)
  setFormat(format);
  // chose the axes settings
  setAxes(axes);
}


//!
//! Copy CTOR
//!
CanvasConfiguration::CanvasConfiguration(const Configuration & config)
:
Configuration(config)
{}

////////////////////////////////////////////////////
// Assignment operator
////////////////////////////////////////////////////
CanvasConfiguration & CanvasConfiguration::operator=(const Configuration & config)
{
  if (this!=&config) Configuration::operator=(config);
  return *this;
}

CanvasConfiguration::~CanvasConfiguration()
{
}

void CanvasConfiguration::setDefaultConfiguration()
{
  addParameter("windowXPosition",     20);
  addParameter("windowYPosition",     20);
  addParameter("windowWidth",        700);
  addParameter("windowHeight",       700);
  addParameter("windowLeftMargin",  0.15);
  addParameter("windowTopMargin",   0.03);
  addParameter("windowRightMargin", 0.03);
  addParameter("windowBottomMargin",0.15);
  addParameter("windowLogX", false);
  addParameter("windowLogY", false);
  addParameter("windowLogZ", false);
  addParameter("windowTheta", 35.0);
  addParameter("windowPhi",   25.0);
  
  addParameter("windowFillColor", kWhite);
  addParameter("windowFillStyle", 1001);
  addParameter("windowBorderSize",0);
  addParameter("windowBorderMode",0);
}


void CanvasConfiguration::setFormat(CanvasFormat format)
{
  switch (format)
    {
      case PortraitTight:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        500);
      addParameter("windowHeight",       800);
      addParameter("windowLeftMargin",  0.10);
      addParameter("windowTopMargin",   0.03);
      addParameter("windowRightMargin", 0.03);
      addParameter("windowBottomMargin",0.10);
      break;
    
      case Portrait:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        500);
      addParameter("windowHeight",       800);
      addParameter("windowLeftMargin",  0.15);
      addParameter("windowTopMargin",   0.05);
      addParameter("windowRightMargin", 0.05);
      addParameter("windowBottomMargin",0.15);
      break;

      case PortraitWide:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        500);
      addParameter("windowHeight",       800);
      addParameter("windowLeftMargin",  0.20);
      addParameter("windowTopMargin",   0.10);
      addParameter("windowRightMargin", 0.10);
      addParameter("windowBottomMargin",0.20);
      break;

      case SquareTight:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        700);
      addParameter("windowHeight",       700);
      addParameter("windowLeftMargin",  0.10);
      addParameter("windowTopMargin",   0.03);
      addParameter("windowRightMargin", 0.03);
      addParameter("windowBottomMargin",0.10);
      break;

      case Square:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        700);
      addParameter("windowHeight",       700);
      addParameter("windowLeftMargin",  0.15);
      addParameter("windowTopMargin",   0.05);
      addParameter("windowRightMargin", 0.05);
      addParameter("windowBottomMargin",0.15);
      break;

      case SquareWide:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        700);
      addParameter("windowHeight",       700);
      addParameter("windowLeftMargin",  0.20);
      addParameter("windowTopMargin",   0.05);
      addParameter("windowRightMargin", 0.10);
      addParameter("windowBottomMargin",0.20);
      break;

      case LandscapeTight:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        800);
      addParameter("windowHeight",       500);
      addParameter("windowLeftMargin",  0.10);
      addParameter("windowTopMargin",   0.03);
      addParameter("windowRightMargin", 0.03);
      addParameter("windowBottomMargin",0.10);
      break;

      case Landscape:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        800);
      addParameter("windowHeight",       500);
      addParameter("windowLeftMargin",  0.15);
      addParameter("windowTopMargin",   0.05);
      addParameter("windowRightMargin", 0.10);
      addParameter("windowBottomMargin",0.15);
      break;
      
      case LandscapeWide:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        800);
      addParameter("windowHeight",       500);
      addParameter("windowLeftMargin",  0.20);
      addParameter("windowTopMargin",   0.05);
      addParameter("windowRightMargin", 0.15);
      addParameter("windowBottomMargin",0.15);
      break;

      case LandscapeXtop:
      addParameter("windowXPosition",     20);
      addParameter("windowYPosition",     20);
      addParameter("windowWidth",        800);
      addParameter("windowHeight",       600);
      addParameter("windowLeftMargin",  0.20);
      addParameter("windowTopMargin",   0.20);
      addParameter("windowRightMargin", 0.05);
      addParameter("windowBottomMargin",0.15);
      break;

    };
}


void CanvasConfiguration::setAxes(CanvasAxes axes)
{
  switch (axes)
    {
      case Linear:
      addParameter("windowLogX", false);
      addParameter("windowLogY", false);
      addParameter("windowLogZ", false);
      break;
     
      case LogX:
      addParameter("windowLogX", true);
      addParameter("windowLogY", false);
      addParameter("windowLogZ", false);
      break;
      
      case LogY:
      addParameter("windowLogX", false);
      addParameter("windowLogY", true);
      addParameter("windowLogZ", false);
      break;

      case LogZ:
      addParameter("windowLogX", false);
      addParameter("windowLogY", false);
      addParameter("windowLogZ", true);
      break;
      
      case LogXY:
      addParameter("windowLogX", true);
      addParameter("windowLogY", true);
      addParameter("windowLogZ", false);
      break;
      
      case LogXZ:
      addParameter("windowLogX", true);
      addParameter("windowLogY", false);
      addParameter("windowLogZ", true);
      break;
      
      case LogYZ:
      addParameter("windowLogX", false);
      addParameter("windowLogY", true);
      addParameter("windowLogZ", true);
      break;
      
      case LogXYZ:
      addParameter("windowLogX", true);
      addParameter("windowLogY", true);
      addParameter("windowLogZ", true);
      break;
    };
}

void CanvasConfiguration::setMargins(float left, float top, float right, float bottom)
{
  addParameter("windowLeftMargin",  left);
  addParameter("windowTopMargin",   top);
  addParameter("windowRightMargin", right);
  addParameter("windowBottomMargin",bottom);
}

void CanvasConfiguration::setSize(int width, int heigh)
{
  addParameter("windowWidth",        width);
  addParameter("windowHeight",       heigh);
}

void CanvasConfiguration::setPosition(int x, int y)
{
  addParameter("windowXPosition",x);
  addParameter("windowYPosition",y);
}
