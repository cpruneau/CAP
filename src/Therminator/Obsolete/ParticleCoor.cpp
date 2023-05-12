/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

#include <TMath.h>
#include "THGlobal.hpp"
#include "ParticleCoor.hpp"


// CP 3/15/2023 added test to avoid self assignment and a return value
ParticleCoor& ParticleCoor::operator= (ParticleCoor& aPartCoor)
{
  if (this!=&aPartCoor)
    {
    ampss  = aPartCoor.ampss;
    t  = aPartCoor.t;
    x  = aPartCoor.x;
    y  = aPartCoor.y;
    z  = aPartCoor.z;
    e  = aPartCoor.e;
    px  = aPartCoor.px;
    py  = aPartCoor.py;
    pz  = aPartCoor.pz;
    pid  = aPartCoor.pid;
    fatherpid  = aPartCoor.fatherpid;
    rootpid  = aPartCoor.rootpid;
    eid    = aPartCoor.eid;
    fathereid  = aPartCoor.fathereid;
    decayed  = aPartCoor.decayed;
    eventid  = aPartCoor.eventid;
    }
  return *this;
}

// CP 3/15/2023 added test to avoid self assignment and a return value
ParticleCoor& ParticleCoor::operator= (ParticleCoor* aPartCoor)
{
  if (this!=aPartCoor)
    {
    ampss	= aPartCoor->ampss;
    t	= aPartCoor->t;
    x	= aPartCoor->x;
    y	= aPartCoor->y;
    z	= aPartCoor->z;
    e	= aPartCoor->e;
    px	= aPartCoor->px;
    py	= aPartCoor->py;
    pz	= aPartCoor->pz;
    pid	= aPartCoor->pid;
    fatherpid	= aPartCoor->fatherpid;
    rootpid	= aPartCoor->rootpid;
    eid		= aPartCoor->eid;
    fathereid	= aPartCoor->fathereid;
    decayed	= aPartCoor->decayed;
    eventid	= aPartCoor->eventid;
    }
  return *this;
}

void ParticleCoor::getMomentum(double* aPe, double* aPx, double* aPy, double* aPz) const
{
  *aPe = e; *aPx = px; *aPy = py; *aPz = pz;
}

void ParticleCoor::getPosition(double* aXt, double* aXx, double* aXy, double* aXz) const
{
  *aXt = t; *aXx = x; *aXy = y; *aXz = z;
}

Float_t ParticleCoor::getTau() const
{
  return sqrt(t*t - z*z)*kHbarC;
}

Float_t ParticleCoor::getR() const
{
  return sqrt(x*x + y*y + z*z)*kHbarC;
}


Float_t ParticleCoor::getRho() const
{
  return sqrt(x*x + y*y)*kHbarC;
}

Float_t ParticleCoor::getPhiS() const
{
  return TMath::ATan2(y,x);
}

Float_t ParticleCoor::getRapidityS() const
{
  return 0.5 * TMath::Log( (t + z) / (t - z) );
}

Float_t ParticleCoor::getP() const
{
  return sqrt(px*px + py*py + pz*pz);
}

Float_t ParticleCoor::getPt() const
{
  return sqrt(px*px + py*py);
}

Float_t ParticleCoor::getMt() const
{
  return sqrt(ampss*ampss + px*px + py*py);
}

Float_t ParticleCoor::getPhiP() const
{
  return TMath::ATan2(py,px);
}

Float_t ParticleCoor::getRapidityP() const
{
  return 0.5 * TMath::Log( (e + pz) / (e - pz) );
}   

Float_t ParticleCoor::getEtaP() const
{
  Float_t tP = getP();
  return 0.5 * TMath::Log( (tP + pz) / (tP - pz) );
}

Int_t ParticleCoor::getDecayed() const
{
  return decayed;
}

void ParticleCoor::setDecayed()
{
  decayed = 1;
}

void ParticleCoor::TransformToLCMS(Float_t aBetaZ)
{
  Float_t tmp;
  Float_t tGamampZ = 1.0 / sqrt(1.0 - aBetaZ*aBetaZ);
  
  tmp = tGamampZ * (e  - aBetaZ * pz);
  pz  = tGamampZ * (pz - aBetaZ * e );
  e   = tmp;
  
  tmp = tGamampZ * (t  - aBetaZ * z );
  z   = tGamampZ * (z  - aBetaZ * t );
  t   = tmp;
}

void ParticleCoor::TransformRotateZ(Float_t aPhi)
{
  Float_t tmp;
  Float_t tCosPhi = cos(aPhi);
  Float_t tSinPhi = TMath::sin(aPhi);
  
  tmp = ( px * tCosPhi + py * tSinPhi);
  py  = ( py * tCosPhi - px * tSinPhi);
  px  = tmp;
  
  tmp = ( x  * tCosPhi + y  * tSinPhi);
  y   = ( y  * tCosPhi - x  * tSinPhi);
  x   = tmp;
}

void ParticleCoor::TransformToPRF(Float_t aBetaT)
{
  Float_t tmp;
  Float_t tGamampT = 1.0 / sqrt(1.0 - aBetaT*aBetaT);
  
  tmp = tGamampT * (e  - aBetaT * px);
  px  = tGamampT * (px - aBetaT * e);
  e   = tmp;
  
  tmp = tGamampT * (t  - aBetaT * x);
  x   = tGamampT * (x  - aBetaT * t);
  t   = tmp;
}
