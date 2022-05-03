//
//  BWModelEventGenerator.hpp
//  Created by Victor Gonzalez on 20190206 based in the work of Claude Pruneau and the ALICE AliPhysics SW framework
//  Copyright © 2016-2019 Claude Pruneau. All rights reserved.
//
#include "BWModelEventGenerator.hpp"
#include <TParticle.h>
#include "Particle.hpp"
ClassImp(BWModelEventGenerator);


BWModelEventGenerator::BWModelEventGenerator(const TString &       _name,
                                             const Configuration & _configuration,
                                             LogLevel              _selectedLevel)
:
Task(),
useAllKinds(true),
useFlow(false),
pionFraction(0.0),
kaonFraction(0.0),
protonFraction(0.0),
ptSpectraAllKinds(nullptr),
azimuthalAngleAllKinds(nullptr),
ptSpectraPions(nullptr),
azimuthalAnglePions(nullptr),
ptSpectraKaons(nullptr),
azimuthalAngleKaons(nullptr),
ptSpectraProtons(nullptr),
azimuthalAngleProtons(nullptr)
{
  
}

BWModelEventGenerator::~BWModelEventGenerator()
{
  if (reportStart("BWModelEventGenerator",getName(),"DTOR"))
    ;
  if (ptSpectraAllKinds != nullptr)       delete ptSpectraAllKinds;
  if (azimuthalAngleAllKinds != nullptr)  delete azimuthalAngleAllKinds;
  if (ptSpectraPions != nullptr)          delete ptSpectraPions;
  if (azimuthalAnglePions != nullptr)     delete azimuthalAnglePions;
  if (ptSpectraKaons != nullptr)          delete ptSpectraKaons;
  if (azimuthalAngleKaons != nullptr)     delete azimuthalAngleKaons;
  if (ptSpectraProtons != nullptr)        delete ptSpectraProtons;
  if (azimuthalAngleProtons != nullptr)   delete azimuthalAngleProtons;
  if (reportEnd("BWModelEventGenerator",getName(),"DTOR"))
    ;
}


void BWModelEventGenerator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  Configuration & config = getConfiguration();
  config.setParameter("useParticles", true);
  config.setParameter("createHistograms", true);
  config.setParameter("saveHistograms", true);

  config.addParameter("useAllKinds",    true);
  config.addParameter("useFlow",        true);
  config.addParameter("useBGBlastWave", true);
  
  config.addParameter("pionFraction", 0.70);
  config.addParameter("kaonFraction", 0.20);
  config.addParameter("protonFraction", 0.10);
  
  config.addParameter("betaAvgBGBW", 0.6);
  config.addParameter("betaProfileBGBW", 0.4);
  
  config.addParameter("totalMultiplicityMean", 100);
  config.addParameter("totalMultiplicitySigma", 0.4);
  config.addParameter("netChargeMean", 0.0);
  config.addParameter("netChargeSigma", 2.5);
  
  config.addParameter("temperatureAllKinds", 160);
  config.addParameter("directedFlowAllKinds", 0.01);
  config.addParameter("ellipticFlowAllKinds", 0.01);
  config.addParameter("triangularFlowAllKinds", 0.01);
  config.addParameter("quadrangularFlowAllKinds", 0.01);
  config.addParameter("pentangularFlowAllKinds", 0.01);
  
  config.addParameter("temperaturePions", 160);
  config.addParameter("directedFlowPions", 0.01);
  config.addParameter("ellipticFlowPions", 0.01);
  config.addParameter("triangularFlowPions", 0.01);
  config.addParameter("quadrangularFlowPions", 0.01);
  config.addParameter("pentangularFlowPions", 0.01);

  config.addParameter("temperatureKaons", 160);
  config.addParameter("directedFlowKaons", 0.01);
  config.addParameter("ellipticFlowKaons", 0.01);
  config.addParameter("triangularFlowKaons", 0.01);
  config.addParameter("quadrangularFlowKaons", 0.01);
  config.addParameter("pentangularFlowKaons", 0.01);

  config.addParameter("temperatureProtons", 160);
  config.addParameter("directedFlowProtons", 0.01);
  config.addParameter("ellipticFlowProtons", 0.01);
  config.addParameter("triangularFlowProtons", 0.01);
  config.addParameter("quadrangularFlowProtons", 0.01);
  config.addParameter("pentangularFlowProtons", 0.01);
  
  config.addParameter("pt_Min", 0.20);
  config.addParameter("pt_Max", 2.00);
  config.addParameter("eta_Min", -1.00);
  config.addParameter("eta_Max",  1.00);
  config.addParameter("longitudinalFlow", 0.0);

}

void BWModelEventGenerator::initialize()
{
  if (reportStart("BWModelEventGenerator",getName(),"initialize()"))
    ;
  Configuration & config = getConfiguration();

  //==============Particles and spectra==============//
  useAllKinds    = config.getValueBool("useAllKinds");
  useFlow        = config.getValueBool("useFlow");
  useBGBlastWave = config.getValueBool("useBGBlastWave");
  
  pionFraction = config.getValueDouble("pionFraction");
  kaonFraction = config.getValueDouble("kaonFraction");
  protonFraction = config.getValueDouble("protonFraction");
  
  pt_Min = config.getValueDouble("pt_Min");
  pt_Max = config.getValueDouble("pt_Max");
  eta_Min = config.getValueDouble("eta_Min");
  eta_Max = config.getValueDouble("eta_Max");
  longitudinalFlow = config.getValueDouble("longitudinalFlow");
  
  double betaAvgBGBW = config.getValueDouble("betaAvgBGBW");
  double betaProfileBGBW = config.getValueDouble("betaProfileBGBW");

  double temperatureAllKinds = config.getValueDouble("temperatureAllKinds");
  double directedFlowAllKinds = config.getValueDouble("directedFlowAllKinds");
  double ellipticFlowAllKinds = config.getValueDouble("ellipticFlowAllKinds");
  double triangularFlowAllKinds = config.getValueDouble("triangularFlowAllKinds");
  double quadrangularFlowAllKinds = config.getValueDouble("quadrangularFlowAllKinds");
  double pentangularFlowAllKinds = config.getValueDouble("pentangularFlowAllKinds");
  
  double temperaturePions = config.getValueDouble("temperaturePions");
  double directedFlowPions = config.getValueDouble("directedFlowPions");
  double ellipticFlowPions = config.getValueDouble("ellipticFlowPions");
  double triangularFlowPions = config.getValueDouble("triangularFlowPions");
  double quadrangularFlowPions = config.getValueDouble("quadrangularFlowPions");
  double pentangularFlowPions  = config.getValueDouble("pentangularFlowPions");

  double temperatureKaons = config.getValueDouble("temperatureKons");
  double directedFlowKaons = config.getValueDouble("directedFlowKaons");
  double ellipticFlowKaons = config.getValueDouble("ellipticFlowKaons");
  double triangularFlowKaons = config.getValueDouble("triangularFlowKaons");
  double quadrangularFlowKaons = config.getValueDouble("quadrangularFlowKaons");
  double pentangularFlowKaons = config.getValueDouble("pentangularFlowKaons");

  double temperatureProtons  = config.getValueDouble("temperatureProtons");
  double directedFlowProtons = config.getValueDouble("directedFlowProtons");
  double ellipticFlowProtons = config.getValueDouble("ellipticFlowProtons");
  double triangularFlowProtons = config.getValueDouble("triangularFlowProtons");
  double quadrangularFlowProtons = config.getValueDouble("quadrangularFlowProtons");
  double pentangularFlowProtons = config.getValueDouble("pentangularFlowProtons");
  
  if (useAllKinds)
    {
    TParticle pion = TParticle();
    pion.SetPdgCode(211);

    if (useBGBlastWave)
      ptSpectraAllKinds = GetBGBWSpectra_FN("PtSpectraAllKinds",pion.GetMass(),temperatureAllKinds,betaAvgBGBW,betaProfileBGBW,pt_Min,pt_Max);
    else
      ptSpectraAllKinds = GetExponentialSpectra_FN("PtSpectraAllKinds",pion.GetMass(),temperatureAllKinds,pt_Min,pt_Max);

    if (useFlow)
      {
      azimuthalAngleAllKinds = new TF1("fAzimuthalAngleAllKinds","1+2.*[1]*TMath::Cos(x-[0])+2.*[2]*TMath::Cos(2*(x-[0]))+2.*[3]*TMath::Cos(3*(x-[0]))+2.*[4]*TMath::Cos(4*(x-[0]))+2.*[5]*TMath::Cos(5*(x-[0]))",0.,2.*TMath::Pi());
      azimuthalAngleAllKinds->SetParName(0,"Reaction Plane");
      azimuthalAngleAllKinds->SetParName(1,"Directed flow");
      azimuthalAngleAllKinds->SetParameter(1,directedFlowAllKinds);
      azimuthalAngleAllKinds->SetParName(2,"Elliptic flow");
      azimuthalAngleAllKinds->SetParameter(2,ellipticFlowAllKinds);
      azimuthalAngleAllKinds->SetParName(3,"Triangular flow");
      azimuthalAngleAllKinds->SetParameter(3,triangularFlowAllKinds);
      azimuthalAngleAllKinds->SetParName(4,"Quadrangular flow");
      azimuthalAngleAllKinds->SetParameter(4,quadrangularFlowAllKinds);
      azimuthalAngleAllKinds->SetParName(5,"Pentangular flow");
      azimuthalAngleAllKinds->SetParameter(5,pentangularFlowAllKinds);
    }
    else {
      useFlow = false;
      azimuthalAngleAllKinds = new TF1("fAzimuthalAngleAllKinds","1",0.,2.*TMath::Pi());
    }
  }
  else {
    TParticle pion = TParticle();
    pion.SetPdgCode(211);
    if (useBGBlastWave)
      ptSpectraProtons = GetBGBWSpectra_FN("PtSpectraPions",pion.GetMass(),temperaturePions,betaAvgBGBW,betaProfileBGBW,pt_Min,pt_Max);
    else
      ptSpectraProtons = GetExponentialSpectra_FN("PtSpectraPions",pion.GetMass(),temperaturePions,pt_Min,pt_Max);

    TParticle kaon = TParticle();
    kaon.SetPdgCode(321);

    if (useBGBlastWave)
      ptSpectraProtons = GetBGBWSpectra_FN("PtSpectraKaons",kaon.GetMass(),temperatureKaons,betaAvgBGBW,betaProfileBGBW,pt_Min,pt_Max);
    else
      ptSpectraProtons = GetExponentialSpectra_FN("PtSpectraKaons",kaon.GetMass(),temperatureKaons,pt_Min,pt_Max);

    TParticle proton = TParticle();
    proton.SetPdgCode(2212);

    if (useBGBlastWave)
      ptSpectraProtons = GetBGBWSpectra_FN("PtSpectraProtons",proton.GetMass(),temperatureProtons,betaAvgBGBW,betaProfileBGBW,pt_Min,pt_Max);
    else
      ptSpectraProtons = GetExponentialSpectra_FN("PtSpectraProtons",proton.GetMass(),temperatureProtons,pt_Min,pt_Max);

    if (useFlow)
      {
      useFlow = true;

      azimuthalAnglePions = new TF1("fAzimuthalAnglePions","1+2.*[1]*TMath::Cos(x-[0])+2.*[2]*TMath::Cos(2*(x-[0]))+2.*[3]*TMath::Cos(3*(x-[0]))+2.*[4]*TMath::Cos(4*(x-[0]))+2.*[5]*TMath::Cos(5*(x-[0]))",0.,2.*TMath::Pi());
      azimuthalAnglePions->SetParName(0,"Reaction Plane");
      azimuthalAnglePions->SetParName(1,"Directed flow");
      azimuthalAnglePions->SetParameter(1,directedFlowPions);
      azimuthalAnglePions->SetParName(2,"Elliptic flow");
      azimuthalAnglePions->SetParameter(2,ellipticFlowPions);
      azimuthalAnglePions->SetParName(3,"Triangular flow");
      azimuthalAnglePions->SetParameter(3,triangularFlowPions);
      azimuthalAnglePions->SetParName(4,"Quadrangular flow");
      azimuthalAnglePions->SetParameter(4,quadrangularFlowPions);
      azimuthalAnglePions->SetParName(5,"Pentangular flow");
      azimuthalAnglePions->SetParameter(5,pentangularFlowPions);

      azimuthalAngleKaons = new TF1("fAzimuthalAngleKaons","1+2.*[1]*TMath::Cos(x-[0])+2.*[2]*TMath::Cos(2*(x-[0]))+2.*[3]*TMath::Cos(3*(x-[0]))+2.*[4]*TMath::Cos(4*(x-[0]))+2.*[5]*TMath::Cos(5*(x-[0]))",0.,2.*TMath::Pi());
      azimuthalAngleKaons->SetParName(0,"Reaction Plane");
      azimuthalAngleKaons->SetParName(1,"Directed flow");
      azimuthalAngleKaons->SetParameter(1,directedFlowKaons);
      azimuthalAngleKaons->SetParName(2,"Elliptic flow");
      azimuthalAngleKaons->SetParameter(2,ellipticFlowKaons);
      azimuthalAngleKaons->SetParName(3,"Triangular flow");
      azimuthalAngleKaons->SetParameter(3,triangularFlowKaons);
      azimuthalAngleKaons->SetParName(4,"Quadrangular flow");
      azimuthalAngleKaons->SetParameter(4,quadrangularFlowKaons);
      azimuthalAngleKaons->SetParName(5,"Pentangular flow");
      azimuthalAngleKaons->SetParameter(5,pentangularFlowKaons);

      azimuthalAngleProtons = new TF1("fAzimuthalAngleProtons","1+2.*[1]*TMath::Cos(x-[0])+2.*[2]*TMath::Cos(2*(x-[0]))+2.*[3]*TMath::Cos(3*(x-[0]))+2.*[4]*TMath::Cos(4*(x-[0]))+2.*[5]*TMath::Cos(5*(x-[0]))",0.,2.*TMath::Pi());
      azimuthalAngleProtons->SetParName(0,"Reaction Plane");
      azimuthalAngleProtons->SetParName(1,"Directed flow");
      azimuthalAngleProtons->SetParameter(1,directedFlowProtons);
      azimuthalAngleProtons->SetParName(2,"Elliptic flow");
      azimuthalAngleProtons->SetParameter(2,ellipticFlowProtons);
      azimuthalAngleProtons->SetParName(3,"Triangular flow");
      azimuthalAngleProtons->SetParameter(3,triangularFlowProtons);
      azimuthalAngleProtons->SetParName(4,"Quadrangular flow");
      azimuthalAngleProtons->SetParameter(4,quadrangularFlowProtons);
      azimuthalAngleProtons->SetParName(5,"Pentangular flow");
      azimuthalAngleProtons->SetParameter(5,pentangularFlowProtons);
    }
    else
      {
      azimuthalAnglePions   = new TF1("fAzimuthalAnglePions","1",0.,2.*TMath::Pi());
      azimuthalAngleKaons   = new TF1("fAzimuthalAngleKaons","1",0.,2.*TMath::Pi());
      azimuthalAngleProtons = new TF1("fAzimuthalAngleProtons","1",0.,2.*TMath::Pi());
    }
  }
  if (reportEnd("BWModelEventGenerator",getName(),"initialize()"))
    ;
}


//!
//! Reset and Initialize the generator
//!
void BWModelEventGenerator::reset()
{
  eventStreams[0]->reset();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate an event
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BWModelEventGenerator::execute()
{
  /* event multiplicity */
  int multiplicity = int(gRandom->Gaus(totalMultiplicityMean,totalMultiplicitySigma));
  int netcharge    = int(gRandom->Gaus(netChargeMean,netChargeSigma));
  int ntogenplus   = int(multiplicity/2.0) + netcharge;
  int ntogenminus  = multiplicity - ntogenplus;
  double reactionplane = TMath::TwoPi()*gRandom->Rndm();
  if (useAllKinds)
    {
    azimuthalAngleAllKinds->SetParameter(0,reactionplane);
    }
  else
    {
    azimuthalAnglePions->SetParameter(0,reactionplane);
    azimuthalAngleKaons->SetParameter(0,reactionplane);
    azimuthalAngleProtons->SetParameter(0,reactionplane);
  }

  /* generate particles */
  int nGenerated = 0;
  int nGeneratedPlus = 0;
  int nGeneratedMinus = 0;
  TParticle pion = TParticle();
  pion.SetPdgCode(211);
  TParticle kaon = TParticle();
  kaon.SetPdgCode(321);
  TParticle proton = TParticle();
  proton.SetPdgCode(2212);

  Event & event = * eventStreams[0];
  /* longitudinal flow */
  double lbeta = longitudinalFlow;
  for (int iparticle = 0; iparticle < multiplicity; iparticle++)
    {
    /* pseudo-rapidity */
    double eta = gRandom->Uniform(eta_Min,eta_Max);
    /* charge */
    int charge = 0;
    if (double(ntogenplus)/double(multiplicity) < gRandom->Rndm()) {
      charge = -1;
      nGeneratedMinus++;
    }
    else {
      charge = 1;
      nGeneratedPlus++;
    }

    /* momentum and azimuthal angle */
    double phi;
    double pT;
    double mass;
    double pid;
    if (useAllKinds)
      {
      pT = ptSpectraAllKinds->GetRandom();
      phi = azimuthalAngleAllKinds->GetRandom();
      mass = pion.GetMass();
      pid  = pion.GetPdgCode();
      }
    else
      {
      double species = gRandom->Rndm();
      if (species < pionFraction) {
        pT = ptSpectraPions->GetRandom();
        phi = azimuthalAnglePions->GetRandom();
        mass = pion.GetMass();
        pid  = pion.GetPdgCode();
      }
      else if (species < pionFraction + kaonFraction)
        {
        pT = ptSpectraKaons->GetRandom();
        phi = azimuthalAngleKaons->GetRandom();
        mass = kaon.GetMass();
          pid  = kaon.GetPdgCode();
        }
      else
        {
        pT = ptSpectraProtons->GetRandom();
        phi = azimuthalAngleProtons->GetRandom();
        mass = proton.GetMass();
          pid  = proton.GetPdgCode();
        }
      }
    Particle *particle = particleFactory->getNextObject();
    double px = pT*TMath::Cos(phi);
    double py = pT*TMath::Sin(phi);
    double pz = pT*TMath::SinH(eta);
    double E = TMath::Sqrt(mass*mass+px*px+py*py+pz*pz);
    particle->setPidPxPyPzE( pid , px,py,pz,E);
    particle->boost(0.0,0.0,(pz < 0) ? -lbeta : lbeta);
    event.add(particle);
    nGenerated++;
    if (reportDebug()) particle->printProperties(cout);
  }
  if (reportDebug("BWModelEventGenerator",getName(),"execute()"))
    {
    cout << "BWModelEventGenerator::execute() " << nGenerated << " particles, " << nGeneratedPlus << " positive, " << nGeneratedMinus << " negative" << endl;
    }
}

TF1 *  BWModelEventGenerator::GetExponentialSpectra_FN(const char *name, double mass, double temp, double xmin, double xmax) {
  TF1 *function = new TF1(name,"x*TMath::Exp(-TMath::Power([0]*[0]+x*x,0.5)/[1])",xmin,xmax);
  function->SetParameters(mass,temp);
  function->SetParNames("mass","T");
  return function;
}

double BWModelEventGenerator::BGBlastWaveIntegrand(double *x, double *par)
{
  // integrand for boltzman-gibbs blast wave
  // x[0] -> r (radius)
  // p[0] -> mass
  // p[1] -> pT (transverse momentum)
  // p[2] -> beta_avg (avg radial velocity)
  // p[3] -> T (freezout temperature)
  // p[4] -> n (velocity profile)
  double x0 = x[0];
  double mass     = par[0];
  double pT       = par[1];
  double beta_avg = par[2];
  double temp     = par[3];
  double n        = par[4];

  // Keep beta within reasonable limits
  double beta_max = beta_avg*(n+2.0)*0.5;
  double beta = beta_max * TMath::Power(x0, n);
  if (beta > 0.9999999999999999) beta = 0.9999999999999999;
  double mT     = TMath::Sqrt(mass*mass+pT*pT);
  double rho0   = TMath::ATanH(beta);
  double arg00  = pT*TMath::SinH(rho0)/temp;
  if (arg00 > 700.) arg00 = 700.; // avoid FPE
  double arg01  = mT*TMath::CosH(rho0)/temp;
  double f0     = x0*mT*TMath::BesselI0(arg00)*TMath::BesselK1(arg01);
  return f0;
}


Double_t BWModelEventGenerator::BGBlastWavedNdPt_FN(double *x, double *par) {

  // implementation of BGBW (dNdpt)

  double pT          = x[0];
  double mass        = par[0];
  double beta_avg    = par[1];
  double temp        = par[2];
  double n           = par[3];
  static TF1 fIntBG = TF1 ("fBGBWIntegrand", BGBlastWaveIntegrand, 0, 1, 5);
  fIntBG.SetParameters(mass, pT, beta_avg, temp,n);
  double result = fIntBG.Integral(0,1);
  return pT*result;
}

TF1 * BWModelEventGenerator::GetBGBWSpectra_FN(const char *name, double mass, double temp, double beta_avg,
                                               double n, double xmin, double xmax){
  // BGBW 1/pt dNdpt
  TF1 *function  = new TF1 (name, BGBlastWavedNdPt_FN, xmin, xmax,4);
  function->SetParameters(mass,beta_avg,temp,n);
  function->SetParNames("mass", "#beta", "T", "n");
  return function;
}
