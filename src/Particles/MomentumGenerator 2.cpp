/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Victor Gonzalez
 *
 * *********************************************************************/
#include "MomentumGenerator.hpp"
using CAP::MomentumGenerator;

ClassImp(MomentumGenerator);

double probDensity(double p, double mass, double temperature, double mu, double stat)
{
  double e     = sqrt(p*p + mass*mass);
  double beta  = 1.0/temperature;
  double arg   = beta*(e-mu);
  double density = p*p/(exp(arg)+stat);
  return density;
}


MomentumGenerator::MomentumGenerator(int  _generatorType, ParticleType * _particleType,
                                     vector<double> _parameters, TRandom * _selectedRandom)
:
generatorType(_generatorType),
particleType(_particleType),
mass(0),
stat(0),
parameters(_parameters),
histograms(),
random(_selectedRandom)
{
  mass = particleType->getMass();
  stat = 0; //particleType->getStat();

  bool invalidSetup = false;
  switch (generatorType)
    {
      case 0:
      // generate isotropic distribution
      // momentum is Gaussian distributed
      // parameters[0] : gaussian width
      if (parameters.size()!=1) invalidSetup = true;
      break;

      case 1:
      // generate isotropic distribution
      // momentum is exponential distributed
      // parameters[0] : average momentum
      if (parameters.size()!=1) invalidSetup = true;
      break;

      case 2:
      // generate isotropic distribution
      // momentum is uniform distributed in range min -- max
      // parameters[0] : minimum momentum
      // parameters[1] : maximum momentum
      if (parameters.size()!=2) invalidSetup = true;
      break;

      case 3:
      // generate isotropic distribution w/ uniform density
      // parameters[0] : minimum momentum
      // parameters[1] : maximum momentum
      if (parameters.size()!=2) invalidSetup = true;
      break;

      case 4:
      // generate isotropic Maxwell Boltzmann distribution
      // parameters[0] : temperature
      if (parameters.size()!=1) invalidSetup = true;
      break;

      case 5:
      // generate cylindrical distribution w/ pt gaussian distribution
      // parameters[0] : minimum rapidity
      // parameters[1] : width of rapidity window
      // parameters[2] : width of Gauss pt
      if (parameters.size()!=3) invalidSetup = true;
      break;

      case 6:
      // generate cylindrical distribution w/ pt exponential distribution
      // parameters[0] : minimum rapidity
      // parameters[1] : width of rapidity window
      // parameters[2] : exponential slope
      if (parameters.size()!=3) invalidSetup = true;
      break;

      case 7:
      // generate cylindrical distribution
      // parameters[0] : minimum rapidity
      // parameters[1] : width of rapidity window
      // parameters[2] : temperature (GeV)
      // parameters[3] : n bins in pT
      // parameters[4] : miniumum pT
      // parameters[5] : maxiumum pT
      if (parameters.size()!=6) invalidSetup = true;
      double temp   = parameters[2];
      double zero   = 0.0;
      String hName = particleType->getName();
      hName += int(1000*temp);
      hName += "MeV";
      double mu     = 0.0;
      int nBinsP    = int(parameters[3]);
      double minP   = parameters[4];
      double maxP   = parameters[5];
      double dp     = (maxP-minP)/double(nBinsP);
      TH1 * h = new TH1D(hName,hName, nBinsP, minP, maxP);
      double p = -dp;
      double v;
      for (int j=0; j<nBinsP; j++)
        {
        p += dp;
        v = probDensity(p,mass,temp,mu,stat);
        h->SetBinContent(j,v);
        h->SetBinError(j,zero);
        }
      histograms.push_back(h);
      break;
      }
  if (invalidSetup)
    {
    cout << "<F> MomentumGenerator(..)" << endl;
    cout << "           generatorType: " << generatorType           << endl;
    cout << "            particleType: " << particleType->getName() << endl;
    cout << "       parameters.size(): " << parameters.size()       << endl;
    cout << "                ABORT!!!! " << endl;
    exit(1);
    }
}


void  MomentumGenerator::generate(LorentzVector & momentum)
{
  double px, py, pz, pt, mt, p, e, ptSq;
  double phi, cosTh, y;
  switch (generatorType)
    {
      case IsotropicGaussP:
      // generate isotropic distribution
      // p is Gaussian distributed
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      p     = fabs(random->Gaus(0.0,parameters[0]));
      e     = sqrt(mass*mass + p*p);
      pt    = p*sqrt(1.0-cosTh*cosTh);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      momentum.SetPxPyPzE (px,py,pz,e);
      break;

      case IsotropicExpP:
      // generate isotropic distribution
      // p is exponential distributed
      // exp( -t/tau )
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      p     = random->Exp(parameters[0]);
      e     = sqrt(mass*mass + p*p);
      pt    = p*sqrt(1.0-cosTh*cosTh);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      momentum.SetPxPyPzE (px,py,pz,e);
      break;

      case IsotropicUniformP:
      // generate isotropic distribution
      // p is uniform distributed
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      p     = parameters[0] + random->Rndm()*parameters[1];
      e     = sqrt(mass*mass + p*p);
      pt    = p*sqrt(1.0-cosTh*cosTh);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      momentum.SetPxPyPzE (px,py,pz,e);
      break;

      case IsotropicUniformDensity:
      // generate isotropic distribution
      // uniform density
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      p     = parameters[0] + random->Rndm()*parameters[1];
      e     = sqrt(mass*mass + p*p);
      pt    = p*sqrt(1.0-cosTh*cosTh);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      momentum.SetPxPyPzE (px,py,pz,e);
      break;

      case IsotropicMaxwellP:
      // generate isotropic distribution
      // Maxwell Boltzmann
      px = random->Gaus(0.0,parameters[0] );
      py = random->Gaus(0.0,parameters[0] );
      pz = random->Gaus(0.0,parameters[0] );
      e  = sqrt(mass*mass + px*px + py*py + pz*pz);
      momentum.SetPxPyPzE (px,py,pz,e);
      break;

//      case IsotropicHistoP:
//      // generate isotropic distribution
//      // p determined by histogram
//      phi   = TMath::TwoPi()*random->Rndm( );
//      cosTh = -1.0 + 2.0*random->Rndm( );
//      p     = histogramP->GetRandom();
//      e     = sqrt(mass*mass + p*p);
//      pt    = p*sqrt(1.0-cosTh*cosTh);
//      px    = pt*cos(phi);
//      py    = pt*sin(phi);
//      pz    = p*cosTh;
//      momentum.SetPxPyPzE (px,py,pz,e);
//      break;

      case CylindricalGaussPtUniformY:
      // generate cylindrical distribution
      // parameters[0] : minimum rapidity
      // parameters[1] : width of rapidity window
      // parameters[2] : width of Gauss pt
      phi   = TMath::TwoPi()*random->Rndm( );
      px    = random->Gaus(0.0,parameters[2] );
      py    = random->Gaus(0.0,parameters[2] );
      ptSq  = px*px + py*py;
      pt    = sqrt(ptSq);
      mt    = sqrt(mass*mass + ptSq);
      y     = parameters[1]+parameters[2]*random->Rndm( );
      e     = mt*cosh(y);
      pz    = mt*sinh(y);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      momentum.SetPxPyPzE (px,py,pz,e);
      break;

      case CylindricalExpPtUniformY:
      // generate cylindrical distribution
      // parameters[0] : minimum rapidity
      // parameters[1] : width of rapidity window
      // parameters[2] : exponential slope
      phi   = TMath::TwoPi()*random->Rndm( );
      pt    = random->Exp(parameters[2]);
      mt    = sqrt(mass*mass + pt*pt);
      y     = parameters[0]+parameters[1]*random->Rndm( );
      e     = mt*cosh(y);
      pz    = mt*sinh(y);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      momentum.SetPxPyPzE (px,py,pz,e);
      break;

      case CylindricalMaxwellPtUniformY:
      // generate cylindrical distribution
      // parameters[0] : minimum rapidity
      // parameters[1] : width of rapidity window
      // parameters[2] : Gauss Width
      pt    = histograms[0]->GetRandom();
      phi   = TMath::TwoPi()*random->Rndm( );
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      mt    = sqrt(mass*mass+pt*pt);
      y     = parameters[0]+parameters[1]*random->Rndm( );
      e     = mt*cosh(y);
      pz    = mt*sinh(y);
      momentum.SetPxPyPzE (px,py,pz,e);
      break;
    }
}
