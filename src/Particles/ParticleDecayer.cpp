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
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <TMath.h>
#include "ParticleDecayer.hpp"

using CAP::ParticleDecayer;
using CAP::LorentzVector;

ClassImp(ParticleDecayer);

double ParticleDecayer::twoPi = 2.0*3.1415927;

ParticleDecayer::ParticleDecayer()
:
random(gRandom)
{
}

void  ParticleDecayer::decay2(ParticleType   & parentType,
                              LorentzVector  & parentMomentum,
                              LorentzVector  & parentPosition,
                              ParticleType   & childType1,
                              LorentzVector  & p1,
                              LorentzVector  & r1,
                              ParticleType   & childType2,
                              LorentzVector  & p2,
                              LorentzVector  & r2)
{
  double e, vx, vy, vz, mp;
  double m1, m2, minMass,tau, taup, gamma, timeToDecay;
  mp       = parentMomentum.M();
  m1       = childType1.getMass();
  m2       = childType2.getMass();
  minMass  = m1 + m2;
  if (mp < minMass) mp = 1.02*minMass;
  e        = parentMomentum.E();
  vx       = parentMomentum.Px()/e;
  vy       = parentMomentum.Py()/e;
  vz       = parentMomentum.Pz()/e;
  gamma    = parentMomentum.Gamma();
  //mWidth   = parentType.getWidth();
  tau      = parentType.getLifeTime();
  taup     = gamma*tau;
  timeToDecay = -3.0E23 * taup * log(random->Rndm()); // fm
  r1 = parentPosition;
  r2 = parentPosition;
  LorentzVector shift;
  shift.SetXYZT(vx*timeToDecay,vy*timeToDecay,vz*timeToDecay,timeToDecay);
  r1 += shift;
  r2 += shift;
  double temp = mp*mp - m1*m1 - m2*m2;

//  cout << "           mp: " << mp << endl;
//  cout << "           m1: " << m1 << endl;
//  cout << "           m2: " << m2 << endl;
//  cout << "      minMass: " << minMass << endl;
//  cout << "           Px: " << parentMomentum.Px() << endl;
//  cout << "           Py: " << parentMomentum.Py() << endl;
//  cout << "           Pz: " << parentMomentum.Pz() << endl;
//  cout << "         beta: " << parentMomentum.Beta() << endl;
//  cout << "           vx: " << vx << endl;
//  cout << "           vy: " << vy << endl;
//  cout << "           vz: " << vz << endl;
//  cout << "        gamma: " << gamma << endl;
//  cout << "       mWidth: " << mWidth << endl;
//  cout << "          tau: " << tau << endl;
//  cout << "         taup: " << taup << endl;
//  cout << "  timeToDecay: " << timeToDecay << endl;
//  cout << "         temp: " << temp << endl;

  double p_lrf = sqrt(temp*temp - 4*m1*m1*m2*m2)/(2*mp);
  // randomly pick emission angle of particle 1
  double phi      = twoPi*random->Rndm();
  double cosPhi   = cos(phi);
  double sinPhi   = sin(phi);
  double cosTheta = 2.*random->Rndm() - 1.0;
  double sinTheta = sqrt(1.00 - cosTheta*cosTheta);
  // compute daughter particles' energy and momentum in the parent rest frame
  double e1_lrf   = sqrt(p_lrf*p_lrf + m1*m1);
  double p1_lrf_x = p_lrf*sinTheta*cosPhi;
  double p1_lrf_y = p_lrf*sinTheta*sinPhi;
  double p1_lrf_z = p_lrf*cosTheta;
  double e2_lrf   = sqrt(p_lrf*p_lrf + m2*m2);
  double p2_lrf_x = -p1_lrf_x;
  double p2_lrf_y = -p1_lrf_y;
  double p2_lrf_z = -p1_lrf_z;
  p1.SetPxPyPzE(p1_lrf_x,p1_lrf_y,p1_lrf_z,e1_lrf);
  p2.SetPxPyPzE(p2_lrf_x,p2_lrf_y,p2_lrf_z,e2_lrf);
  // boost in "lab frame"
  p1.Boost(vx,vy,vz);
  p2.Boost(vx,vy,vz);
}

void  ParticleDecayer::decay3(ParticleType & parentType,
                              LorentzVector & parentMomentum,
                              LorentzVector & parentPosition,
                              ParticleType   & childType1,
                              LorentzVector & p1,
                              LorentzVector & r1,
                              ParticleType   & childType2,
                              LorentzVector & p2,
                              LorentzVector & r2,
                              ParticleType   & childType3,
                              LorentzVector & p3,
                              LorentzVector & r3)
{
  double e, vx, vy, vz, mp;
  double m1, m2, m3, minMass,tau, taup, gamma, timeToDecay;
  double e1_lrf, e2_lrf, e3_lrf, p1_lrf, p2_lrf, cos12_lrf;
  double v2;

  mp       = parentMomentum.M();
  m1       = childType1.getMass();
  m2       = childType2.getMass();
  m3       = childType3.getMass();
  minMass  = m1 + m2 + m3;
  if (mp < minMass)
    {
    //cout << "<E> ParticleDecayer::decay3(..) Parent too light: M = " << mp << ", m1 = " << m1 << ", m2 = " << m2 << ", m3 = " << m3 << endl;
    mp =  mp = 1.02*minMass;
    }
  e        = parentMomentum.E();
  vx       = parentMomentum.Px()/e;
  vy       = parentMomentum.Py()/e;
  vz       = parentMomentum.Pz()/e;
  v2       = vx*vx + vy*vy + vz*vz;
  gamma    = parentMomentum.Gamma();
  //mWidth   = parentType.getWidth();
  tau      = parentType.getLifeTime();
  taup     = gamma*tau;
  timeToDecay = -3.0E23 * taup * log(random->Rndm()); // fm
  r1 = parentPosition;
  r2 = parentPosition;
  r3 = parentPosition;
  LorentzVector shift;
  shift.SetXYZT(vx*timeToDecay,vy*timeToDecay,vz*timeToDecay,timeToDecay);
  r1 += shift;
  r2 += shift;
  r3 += shift;
  //
  double deltaM = mp - m1 - m2 - m3;

  do {
    do {
      e1_lrf = random->Rndm()*deltaM + m1;
      e2_lrf = random->Rndm()*deltaM + m2;
    } while (e1_lrf + e2_lrf > mp);
    p1_lrf = sqrt(e1_lrf*e1_lrf - m1*m1);
    p2_lrf = sqrt(e2_lrf*e2_lrf - m2*m2);
    e3_lrf = mp - e1_lrf - e2_lrf;
    cos12_lrf = (e3_lrf*e3_lrf - p1_lrf*p1_lrf - p2_lrf*p2_lrf - m3*m3)/(2.*p1_lrf*p2_lrf);
  } while (cos12_lrf < - 1.0 || cos12_lrf > 1.0);

  // compute the momentum of decay daughters
  double tp2_lrf_x = p2_lrf*sqrt(1. - cos12_lrf*cos12_lrf);
  double tp2_lrf_z = p2_lrf*cos12_lrf;
  double tp3_lrf_x = - tp2_lrf_x;
  double tp3_lrf_z = - (p1_lrf + tp2_lrf_z);
  double phi       = twoPi*random->Rndm();
  double ksi       = twoPi*random->Rndm();
  double cos_theta = 2.0*random->Rndm() - 1.0;
  double sin_phi   = sin(phi);
  double cos_phi   = cos(phi);
  double sin_ksi   = sin(ksi);
  double cos_ksi   = cos(ksi);
  double sin_theta = sqrt(1. - cos_theta*cos_theta);

  double p1_lrf_x = - p1_lrf*sin_theta*cos_ksi;
  double p1_lrf_y = p1_lrf*sin_theta*sin_ksi;
  double p1_lrf_z = p1_lrf*cos_theta;
  e1_lrf = sqrt(m1*m1 + p1_lrf_x*p1_lrf_x + p1_lrf_y*p1_lrf_y + p1_lrf_z*p1_lrf_z);
  double p2_lrf_x = (tp2_lrf_x*(cos_phi*cos_theta*cos_ksi - sin_phi*sin_ksi)  - tp2_lrf_z*sin_theta*cos_ksi);
  double p2_lrf_y = (tp2_lrf_x*(-cos_phi*cos_theta*sin_ksi - sin_phi*cos_ksi) + tp2_lrf_z*sin_theta*sin_ksi);
  double p2_lrf_z = tp2_lrf_x*(cos_phi*sin_theta) + tp2_lrf_z*cos_theta;
  e2_lrf = sqrt(m2*m2 + p2_lrf_x*p2_lrf_x + p2_lrf_y*p2_lrf_y + p2_lrf_z*p2_lrf_z);
  double p3_lrf_x = (tp3_lrf_x*(cos_phi*cos_theta*cos_ksi - sin_phi*sin_ksi) - tp3_lrf_z*sin_theta*cos_ksi);
  double p3_lrf_y = (tp3_lrf_x*(-cos_phi*cos_theta*sin_ksi - sin_phi*cos_ksi) + tp3_lrf_z*(sin_theta*sin_ksi));
  double p3_lrf_z = tp3_lrf_x*cos_phi*sin_theta + tp3_lrf_z*cos_theta;
  e3_lrf = sqrt(m3*m3 + p3_lrf_x*p3_lrf_x + p3_lrf_y*p3_lrf_y + p3_lrf_z*p3_lrf_z);

  double gamma_m_1 = gamma - 1.;
  double vp1 = vx*p1_lrf_x + vy*p1_lrf_y + vz*p1_lrf_z;
  double vp2 = vx*p2_lrf_x + vy*p2_lrf_y + vz*p2_lrf_z;
  double vp3 = vx*p3_lrf_x + vy*p3_lrf_y + vz*p3_lrf_z;

  p1.SetPxPyPzE(p1_lrf_x + (gamma_m_1*vp1/v2 + gamma*e1_lrf)*vx,
                p1_lrf_y + (gamma_m_1*vp1/v2 + gamma*e1_lrf)*vy,
                p1_lrf_z + (gamma_m_1*vp1/v2 + gamma*e1_lrf)*vz,
                gamma*(e1_lrf + vp1));
  p2.SetPxPyPzE(p2_lrf_x + (gamma_m_1*vp2/v2 + gamma*e2_lrf)*vx,
                p2_lrf_y + (gamma_m_1*vp2/v2 + gamma*e2_lrf)*vy,
                p2_lrf_z + (gamma_m_1*vp2/v2 + gamma*e2_lrf)*vz,
                gamma*(e2_lrf + vp2));
  p3.SetPxPyPzE(p3_lrf_x + (gamma_m_1*vp3/v2 + gamma*e3_lrf)*vx,
                p3_lrf_y + (gamma_m_1*vp3/v2 + gamma*e3_lrf)*vy,
                p3_lrf_z + (gamma_m_1*vp3/v2 + gamma*e3_lrf)*vz,
                gamma*(e3_lrf + vp3));
}


void ParticleDecayer::decay4(ParticleType & parentType,
                             LorentzVector & parentMomentum,
                             LorentzVector & parentPosition,
                             ParticleType   & childType1,
                             LorentzVector & p1,
                             LorentzVector & r1,
                             ParticleType   & childType2,
                             LorentzVector & p2,
                             LorentzVector & r2,
                             ParticleType   & childType3,
                             LorentzVector & p3,
                             LorentzVector & r3,
                             ParticleType   & childType4,
                             LorentzVector & p4,
                             LorentzVector & r4
                             )
{
  double e, vx, vy, vz, mp;
  //double mWidth;
  double m1, m2, m3, m4, minMass,tau, taup, gamma, lifeTime;
  //double e1_lrf, e2_lrf, e3_lrf, e4_lrf, p1_lrf, p2_lrf, cos12_lrf;
  //double v2;

  mp       = parentMomentum.M();
  m1       = childType1.getMass();
  m2       = childType2.getMass();
  m3       = childType3.getMass();
  m4       = childType4.getMass();
  minMass  = m1 + m2 + m3 + m4;
  if (mp < minMass)
    {
    mp = 1.02*minMass;
    }
  e        = parentMomentum.E();
  vx       = parentMomentum.Px()/e;
  vy       = parentMomentum.Py()/e;
  vz       = parentMomentum.Pz()/e;
  //v2       = vx*vx + vy*vy + vz*vz;
  gamma    = parentMomentum.Gamma();
  //mWidth   = parentType.getWidth();
  tau      = parentType.getLifeTime();
  taup     = gamma*tau;
  lifeTime = -3.0E23 * taup * log(random->Rndm()); // fm
  r1 = parentPosition;
  r2 = parentPosition;
  r3 = parentPosition;
  LorentzVector shift;
  shift.SetXYZT(vx*lifeTime,vy*lifeTime,vz*lifeTime,lifeTime);
  r1 += shift;
  r2 += shift;
  r3 += shift;
  r4 += shift;
  //
  //double deltaM = mp - m1 - m2 - m3;

  p1.SetPxPyPzE(0.0,
                0.0,
                0.0,
                m1);
  p2.SetPxPyPzE(0.0,
                0.0,
                0.0,
                m2);
  p3.SetPxPyPzE(0.0,
                0.0,
                0.0,
                m3);
  p4.SetPxPyPzE(0.0,
                0.0,
                0.0,
                m4);
  p1.Boost(vx,vy,vz);
  p2.Boost(vx,vy,vz);
  p3.Boost(vx,vy,vz);
  p4.Boost(vx,vy,vz);
}

void  ParticleDecayer::decay2(Particle & parent,
                              Particle & child1,
                              Particle & child2)
{
  ParticleType  & parentType     = parent.getType();
  LorentzVector & parentMomentum = parent.getMomentum();
  LorentzVector & parentPosition = parent.getPosition();
  ParticleType  & childType1     = child1.getType();
  LorentzVector & p1             = child1.getMomentum();
  LorentzVector & r1             = child1.getPosition();
  ParticleType  & childType2     = child2.getType();
  LorentzVector & p2             = child2.getMomentum();
  LorentzVector & r2             = child2.getPosition();
  decay2(parentType,parentMomentum,parentPosition,
         childType1,p1,r1,
         childType2,p2,r2);
}

void  ParticleDecayer::decay3(Particle & parent,
                              Particle & child1,
                              Particle & child2,
                              Particle & child3)
{
  ParticleType  & parentType     = parent.getType();
  LorentzVector & parentMomentum = parent.getMomentum();
  LorentzVector & parentPosition = parent.getPosition();
  ParticleType  & childType1     = child1.getType();
  LorentzVector & p1             = child1.getMomentum();
  LorentzVector & r1             = child1.getPosition();
  ParticleType  & childType2     = child2.getType();
  LorentzVector & p2             = child2.getMomentum();
  LorentzVector & r2             = child2.getPosition();
  ParticleType  & childType3     = child3.getType();
  LorentzVector & p3             = child3.getMomentum();
  LorentzVector & r3             = child3.getPosition();
  decay3(parentType,parentMomentum,parentPosition,
         childType1,p1,r1,
         childType2,p2,r2,
         childType3,p3,r3);
}

void  ParticleDecayer::decay4(Particle & parent,
                              Particle & child1,
                              Particle & child2,
                              Particle & child3,
                              Particle & child4)
{
  ParticleType  & parentType     = parent.getType();
  LorentzVector & parentMomentum = parent.getMomentum();
  LorentzVector & parentPosition = parent.getPosition();
  ParticleType  & childType1     = child1.getType();
  LorentzVector & p1             = child1.getMomentum();
  LorentzVector & r1             = child1.getPosition();
  ParticleType  & childType2     = child2.getType();
  LorentzVector & p2             = child2.getMomentum();
  LorentzVector & r2             = child2.getPosition();
  ParticleType  & childType3     = child3.getType();
  LorentzVector & p3             = child3.getMomentum();
  LorentzVector & r3             = child3.getPosition();
  ParticleType  & childType4     = child4.getType();
  LorentzVector & p4             = child4.getMomentum();
  LorentzVector & r4             = child4.getPosition();
  decay4(parentType,parentMomentum,parentPosition,
         childType1,p1,r1,
         childType2,p2,r2,
         childType3,p3,r3,
         childType4,p4,r4);
}
