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

#include "CollisionGeometryMoments.hpp"
using CAP::CollisionGeometryMoments;
ClassImp(CollisionGeometryMoments);

CollisionGeometryMoments::CollisionGeometryMoments()
:
m0( 0.0 ),
m1x( 0 ),
m1y( 0 ),
m2x2( 0 ),
m2y2( 0 ),
m2xy( 0 ),
meanX( 0 ),
meanY( 0 ),
varX( 0 ),
varY( 0 ),
varXY( 0 ),
epsX( 0 ),
epsY( 0 ),
epsMod( 0 ),
psi2( 0 ),
area( 0 )
{
  reset();
}

CollisionGeometryMoments::CollisionGeometryMoments(const CollisionGeometryMoments & source)
:
m0(  source.m0 ),
m1x( source.m1x ),
m1y( source.m1y ),
m2x2( source.m2x2 ),
m2y2( source.m2y2 ),
m2xy( source.m2xy ),
meanX( source.meanX ),
meanY( source.meanX ),
varX ( source.varX ),
varY ( source.varY ),
varXY( source.varXY ),
epsX ( source.epsX ),
epsY ( source.epsY ),
epsMod( source.epsMod ),
psi2  ( source.psi2 ),
area  ( source.area )
{
  for (int iN=0;iN<10;iN++)
  {
  cphiN[iN] = source.cphiN[iN];
  sphiN[iN] = source.sphiN[iN];
  rN[iN]    = source.rN[iN];
  psiN[iN]  = source.psiN[iN];
  eccN[iN]  = source.eccN[iN];
  }
}

CollisionGeometryMoments& CollisionGeometryMoments::operator=(const CollisionGeometryMoments & source)
{
  if (&source != this)
    {
    m0    = source.m0;
    m1x   = source.m1x;
    m1y   = source.m1y;
    m2x2  = source.m2x2;
    m2y2  = source.m2y2;
    m2xy  = source.m2xy;
    meanX = source.meanX;
    meanY = source.meanX;
    varX  = source.varX;
    varY  = source.varY;
    varXY = source.varXY;
    epsX  = source.epsX;
    epsY  = source.epsY;
    epsMod = source.epsMod;
    psi2   = source.psi2;
    area   = source.area;
    for (int iN=0;iN<10;iN++)
      {
      cphiN[iN] = source.cphiN[iN];
      sphiN[iN] = source.sphiN[iN];
      rN[iN]    = source.rN[iN];
      psiN[iN]  = source.psiN[iN];
      eccN[iN]  = source.eccN[iN];
      }
    }
  return *this;
}

void CollisionGeometryMoments::reset()
{
  m0    = 0.0;
  m1x   = 0.0;
  m1y   = 0.0;
  m2x2  = 0.0;
  m2y2  = 0.0;
  m2xy  = 0.0;
  m1x   = 0.0;
  m1y   = 0.0;
  m2x2  = 0.0;
  m2y2  = 0.0;
  m2xy  = 0.0;
  meanX = 0.0;
  meanY = 0.0;
  varX  = 0.0;
  varY  = 0.0;
  varXY = 0.0;
  epsX  = 0.0;
  epsY  = 0.0;
  epsMod = 0.0;
  psi2   = 0.0;
  area   = 0.0;
  for (int iN=0;iN<10;iN++)
  {
  cphiN[iN] = 0.0;
  sphiN[iN] = 0.0;
  rN[iN]    = 0.0;
  psiN[iN]  = 0.0;
  eccN[iN]  = 0.0;
  }
}


void CollisionGeometryMoments::fill(double x ,double y)
{
  m0    += 1.0;
  m1x   += x;
  m1y   += y;
  m2x2  += x*x;
  m2y2  += y*y;
  m2xy  += x*y;
}


void CollisionGeometryMoments::calculate()
{
  if (m0>2)
    {
    meanX = m1x/m0;
    meanY = m1x/m0;
    varX  = m2x2/m0 - m1x*m1x;
    varY  = m2y2/m0 - m1y*m1y;
    varXY = m2xy/m0 - m1x*m1y;
    epsDenom = varX + varY;
    if (epsDenom>0)
      {
      epsX     = (varY - varX)/epsDenom;
      epsY     = 2*varXY/epsDenom;
      epsMod   = sqrt(epsX*epsX + epsY*epsY);
      psi2     = atan2(epsY,epsX);
      }
    }
  else
    {
    meanX = -1.0E100;
    meanY = -1.0E100;
    varX  = -1.0E100;
    varY  = -1.0E100;
    varXY = -1.0E100;
    }

}
