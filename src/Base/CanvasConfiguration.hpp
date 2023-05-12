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
#ifndef CAP__CanvasConfiguration
#define CAP__CanvasConfiguration
#include "Configuration.hpp"

//!
//! Convenience class designed to facilitate the setting of root windows
//!
class CanvasConfiguration: public Configuration
{
public:

  enum CanvasFormat   { PortraitTight, Portrait, PortraitWide, SquareTight, Square, SquareWide, LandscapeTight, Landscape, LandscapeWide, LandscapeXtop };
  enum CanvasAxes     { Linear, LogX, LogY, LogZ, LogXY, LogXZ, LogYZ, LogXYZ  };

  //!
  //! Default CTOR
  //!
  CanvasConfiguration();

  //!
  //!CTOR
  //!Set the aspect ratio and the axes
  //!
  CanvasConfiguration(CanvasFormat format, CanvasAxes axes);

  //!
  //!Copy CTOR
  //!
  CanvasConfiguration(const CanvasConfiguration & config);

  //!
  //!Copy operator
  //!
  CanvasConfiguration &  operator=(const CanvasConfiguration & config);

  //!
  //!DOTR
  //!
  virtual ~CanvasConfiguration();
 
  //!
  //!Set canvas parameters to their default value
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //!Set the format
  //!
  virtual void setFormat(CanvasFormat format=Square);

  //!
  //!Set the axis style
  //!
  virtual void setAxes(CanvasAxes format=Linear);

  //!
  //!Set margine sizes
  //!
  virtual void setMargins(float leftMargin, float topMargin, float rightMargin, float bottomMargin);

  //!
  //!Set the window size
  //!
  virtual void setSize(int width, int height);

  //!
  //!Set the nominal window position
  //!
  virtual void setPosition(int x, int y);
  
  ClassDef(CanvasConfiguration,0)

};


#endif /* CanvasConfiguration_hpp */
