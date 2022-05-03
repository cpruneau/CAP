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

ClassImp(CanvasCollection);

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
CanvasConfiguration::CanvasConfiguration(const CanvasConfiguration & config)
:
Configuration(config)
{}

////////////////////////////////////////////////////
// Assignment operator
////////////////////////////////////////////////////
CanvasConfiguration & CanvasConfiguration::operator=(const CanvasConfiguration & config)
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
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        500);
      setParameter("windowHeight",       800);
      setParameter("windowLeftMargin",  0.10);
      setParameter("windowTopMargin",   0.03);
      setParameter("windowRightMargin", 0.03);
      setParameter("windowBottomMargin",0.10);
      break;
    
      case Portrait:
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        500);
      setParameter("windowHeight",       800);
      setParameter("windowLeftMargin",  0.15);
      setParameter("windowTopMargin",   0.05);
      setParameter("windowRightMargin", 0.05);
      setParameter("windowBottomMargin",0.15);
      break;

      case PortraitWide:
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        500);
      setParameter("windowHeight",       800);
      setParameter("windowLeftMargin",  0.20);
      setParameter("windowTopMargin",   0.10);
      setParameter("windowRightMargin", 0.10);
      setParameter("windowBottomMargin",0.20);
      break;

      case SquareTight:
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        700);
      setParameter("windowHeight",       700);
      setParameter("windowLeftMargin",  0.10);
      setParameter("windowTopMargin",   0.03);
      setParameter("windowRightMargin", 0.03);
      setParameter("windowBottomMargin",0.10);
      break;

      case Square:
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        700);
      setParameter("windowHeight",       700);
      setParameter("windowLeftMargin",  0.15);
      setParameter("windowTopMargin",   0.05);
      setParameter("windowRightMargin", 0.05);
      setParameter("windowBottomMargin",0.15);
      break;

      case SquareWide:
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        700);
      setParameter("windowHeight",       700);
      setParameter("windowLeftMargin",  0.20);
      setParameter("windowTopMargin",   0.05);
      setParameter("windowRightMargin", 0.10);
      setParameter("windowBottomMargin",0.20);
      break;

      case LandscapeTight:
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        800);
      setParameter("windowHeight",       500);
      setParameter("windowLeftMargin",  0.10);
      setParameter("windowTopMargin",   0.03);
      setParameter("windowRightMargin", 0.03);
      setParameter("windowBottomMargin",0.10);
      break;

      case Landscape:
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        800);
      setParameter("windowHeight",       500);
      setParameter("windowLeftMargin",  0.15);
      setParameter("windowTopMargin",   0.05);
      setParameter("windowRightMargin", 0.10);
      setParameter("windowBottomMargin",0.15);
      break;
      
      case LandscapeWide:
      setParameter("windowXPosition",     20);
      setParameter("windowYPosition",     20);
      setParameter("windowWidth",        800);
      setParameter("windowHeight",       500);
      setParameter("windowLeftMargin",  0.20);
      setParameter("windowTopMargin",   0.05);
      setParameter("windowRightMargin", 0.15);
      setParameter("windowBottomMargin",0.15);
      break;
    };
}


void CanvasConfiguration::setAxes(CanvasAxes axes)
{
  switch (axes)
    {
      case Linear:
      setParameter("windowLogX", false);
      setParameter("windowLogY", false);
      setParameter("windowLogZ", false);
      break;
     
      case LogX:
      setParameter("windowLogX", true);
      setParameter("windowLogY", false);
      setParameter("windowLogZ", false);
      break;
      
      case LogY:
      setParameter("windowLogX", false);
      setParameter("windowLogY", true);
      setParameter("windowLogZ", false);
      break;

      case LogZ:
      setParameter("windowLogX", false);
      setParameter("windowLogY", false);
      setParameter("windowLogZ", true);
      break;
      
      case LogXY:
      setParameter("windowLogX", true);
      setParameter("windowLogY", true);
      setParameter("windowLogZ", false);
      break;
      
      case LogXZ:
      setParameter("windowLogX", true);
      setParameter("windowLogY", false);
      setParameter("windowLogZ", true);
      break;
      
      case LogYZ:
      setParameter("windowLogX", false);
      setParameter("windowLogY", true);
      setParameter("windowLogZ", true);
      break;
      
      case LogXYZ:
      setParameter("windowLogX", true);
      setParameter("windowLogY", true);
      setParameter("windowLogZ", true);
      break;
    };
}

void CanvasConfiguration::setMargins(float left, float top, float right, float bottom)
{
  setParameter("windowLeftMargin",  left);
  setParameter("windowTopMargin",   top);
  setParameter("windowRightMargin", right);
  setParameter("windowBottomMargin",bottom);
}

void CanvasConfiguration::setSize(int width, int heigh)
{
  setParameter("windowWidth",        width);
  setParameter("windowHeight",       heigh);
}

void CanvasConfiguration::setPosition(int x, int y)
{
  setParameter("windowXPosition",x);
  setParameter("windowYPosition",y);
}
