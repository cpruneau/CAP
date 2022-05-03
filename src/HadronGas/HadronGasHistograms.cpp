//
//  HadronGasVsTempHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "HadronGasHistograms.hpp"
ClassImp(HadronGasHistograms);

HadronGasHistograms::HadronGasHistograms(const TString &       _name,
                                         const Configuration & _config,
                                         HadronGas *           _hadronGas,
                                         LogLevel              _debugLevel)
:
Histograms(_name,_config,_debugLevel),
hadronGas(_hadronGas)
{
  // no ops
}

HadronGasHistograms::~HadronGasHistograms()
{
  // no ops
}

void HadronGasHistograms::createHistograms()
{
  if (reportStart("HadronGasHistograms",getName(),"createHistograms()"))
    ;
  TString bn = getHistoBaseName();
  const Configuration & config = getConfiguration();
  int nMass      = config.getValueInt("nMass");
  double minMass = config.getValueDouble("minMass");
  double maxMass = config.getValueDouble("maxMass");
  
  int nP      = config.getValueInt("nP");
  double minP = config.getValueDouble("minP");
  double maxP = config.getValueDouble("maxP");

  int nSpecies       = hadronGas->particleTypes->size();
  int nStableSpecies = hadronGas->stableParticleTypes->size();
  double dSpecies = nSpecies;
  double dStableSpecies = nStableSpecies;
  
  h_rho1ThVsMass    = createProfile(bn+TString("rho1ThVsMass"), nMass,minMass,maxMass,"Mass (GeV)","#rho_{1}");
  h_rho1AllVsMass   = createProfile(bn+TString("rho1AllVsMass"),nMass,minMass,maxMass,"Mass (GeV)","#rho_{1}");
  h_rho1VsMass      = createProfile(bn+TString("rho1VsMass"),   nMass,minMass,maxMass,"Mass (GeV)","#rho_{1}");

  h_rho1All         = createHistogram(bn+TString("rho1All"),nSpecies,        0.0, dSpecies,       "Species","#rho_{1}");
  h_rho1Th          = createHistogram(bn+TString("rho1Th"), nStableSpecies,  0.0, dStableSpecies, "Species","#rho1_{1}^{Th}");
  h_rho1            = createHistogram(bn+TString("rho1"),   nStableSpecies,  0.0, dStableSpecies, "Species","#rho_{1}");
  h_rho1ToRho1Th    = createHistogram(bn+TString("rho1ToRho1Th"),nStableSpecies,  0.0, dStableSpecies, "Species","#rho_{1}");
  h_rho1rho1        = createHistogram(bn+TString("rho1rho1"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","Pairs");
  h_rho1thrho1th    = createHistogram(bn+TString("rho1thrho1th"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","#rho_{1}^{Th}#rho_{1}^{Th}");
  h_rho2Corr        = createHistogram(bn+TString("rho2Corr"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","Correlated Pairs");
  h_rho2Uncorr      = createHistogram(bn+TString("rho2Uncorr"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","Uncorrelated Pairs");

  h_rho2            = createHistogram(bn+TString("rho2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","#rho_{2}");

  h_C2              = createHistogram(bn+TString("C2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","C_{2}");
  h_R2              = createHistogram(bn+TString("R2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","R_{2}");
  h_BF              = createHistogram(bn+TString("BF"),18,  0.0, 18.0, "Pairs","BF");

  // set labels
  for (int iSpecies=1; iSpecies<=nSpecies; iSpecies++)
    {
    TString label = hadronGas->particleTypes->getParticleType(iSpecies-1)->getTitle();
    h_rho1All->GetXaxis()->SetBinLabel(iSpecies,label);
    }
  for (int iSpecies=1; iSpecies<=nStableSpecies; iSpecies++)
    {
    TString label = hadronGas->stableParticleTypes->getParticleType(iSpecies-1)->getTitle();
    h_rho1Th->GetXaxis()->SetBinLabel(iSpecies,label);
    h_rho1->GetXaxis()->SetBinLabel(iSpecies,label);
    h_rho1ToRho1Th->GetXaxis()->SetBinLabel(iSpecies,label);
    h_rho2Corr->GetXaxis()->SetBinLabel(iSpecies,label);
    h_rho2Corr->GetYaxis()->SetBinLabel(iSpecies,label);
    h_rho2->GetXaxis()->SetBinLabel(iSpecies,label);
    h_rho2->GetYaxis()->SetBinLabel(iSpecies,label);
    h_rho1rho1->GetXaxis()->SetBinLabel(iSpecies,label);
    h_rho1rho1->GetYaxis()->SetBinLabel(iSpecies,label);
    h_rho1thrho1th->GetXaxis()->SetBinLabel(iSpecies,label);
    h_rho1thrho1th->GetYaxis()->SetBinLabel(iSpecies,label);
    h_C2->GetXaxis()->SetBinLabel(iSpecies,label);
    h_C2->GetYaxis()->SetBinLabel(iSpecies,label);
    h_R2->GetXaxis()->SetBinLabel(iSpecies,label);
    h_R2->GetYaxis()->SetBinLabel(iSpecies,label);
    h_rho2Uncorr->GetXaxis()->SetBinLabel(iSpecies,label);
    h_rho2Uncorr->GetYaxis()->SetBinLabel(iSpecies,label);
    }

  h_BF->GetXaxis()->SetBinLabel(1,"#pi^{-} | #pi^{+}");
  h_BF->GetXaxis()->SetBinLabel(2,"K^{-} | #pi^{+}");
  h_BF->GetXaxis()->SetBinLabel(3,"#bar{p} | #pi^{+}");

  h_BF->GetXaxis()->SetBinLabel(4,"#pi^{+} | #pi^{-}");
  h_BF->GetXaxis()->SetBinLabel(5,"K^{+} | #pi^{-}");
  h_BF->GetXaxis()->SetBinLabel(6,"p | #pi^{-}");

  h_BF->GetXaxis()->SetBinLabel(7,"#pi^{-} | K^{+}");
  h_BF->GetXaxis()->SetBinLabel(8,"K^{-} | K^{+}");
  h_BF->GetXaxis()->SetBinLabel(9,"#bar{p} | K^{+}");

  h_BF->GetXaxis()->SetBinLabel(10,"#pi^{+} | K^{-}");
  h_BF->GetXaxis()->SetBinLabel(11,"K^{+} | K^{-}");
  h_BF->GetXaxis()->SetBinLabel(12,"p | K^{-}");

  h_BF->GetXaxis()->SetBinLabel(13,"#pi^{-} | p");
  h_BF->GetXaxis()->SetBinLabel(14,"K^{-} | p");
  h_BF->GetXaxis()->SetBinLabel(15,"#bar{p} | p");

  h_BF->GetXaxis()->SetBinLabel(16,"#pi^{+} | #bar{p}");
  h_BF->GetXaxis()->SetBinLabel(17,"K^{+} | #bar{p}");
  h_BF->GetXaxis()->SetBinLabel(18,"p | #bar{p}");


  TString histoName;
  h_rho1ThVsP = new TH1*[nSpecies];
  for (int iSpecies=0; iSpecies<nSpecies; iSpecies++)
    {
    histoName = bn+TString("rho1ThVsP_S");
    histoName += iSpecies; //hadronGas->getParticleType(iSpecies)->getName();
    h_rho1ThVsP[iSpecies] = createHistogram(histoName,nP,minP,maxP,"p (GeV/c)","#rho_{1}^{Th} (c/GeV)");
    }
  if (reportEnd("HadronGasHistograms",getName(),"createHistograms()"))
    ;
}

void HadronGasHistograms::fillBf(int trig, int sameSignAssoc, int oppSignAssoc, int bfIndex)
{
  if (reportStart("HadronGasHistograms",getName(),"fillBf(...)"))
    ;
  double sameSignAssocYield = h_rho1->GetBinContent(sameSignAssoc);
  double oppSignAssocYield  = h_rho1->GetBinContent(oppSignAssoc);
  double sameSignPair       = h_R2->GetBinContent(trig,sameSignAssoc);
  double oppSignPair        = h_R2->GetBinContent(trig,oppSignAssoc);
  double bf = oppSignAssocYield*oppSignPair - sameSignAssocYield*sameSignPair;
  h_BF->SetBinContent(bfIndex, bf);
  h_BF->SetBinError(bfIndex, 0.0);
  if (reportEnd("HadronGasHistograms",getName(),"fillBf(...)"))
    ;
}

void HadronGasHistograms::fillRho1VsP(HadronGas & hadronGas, double volume)
{
  if (reportStart("HadronGasHistograms",getName(),"fillRho1VsP(...)"))
    ;
  const Configuration & config = getConfiguration();
  int    nP   = config.getValueInt("nP");
  double minP = config.getValueDouble("minP");
  double maxP = config.getValueDouble("maxP");
  double deltaP = (maxP - minP)/double(nP);
  
  double zero = 0.0;
  double density;
  double temperature = hadronGas.getTemperature();
  ParticleTypeCollection & particleTypes = hadronGas.getParticleTypes();
  for (unsigned int iSpecies=0; iSpecies<particleTypes.size(); iSpecies++)
    {
    ParticleType & particleType = *(particleTypes[iSpecies]);
    double g    = particleType.getSpinFactor();
    double mass = particleType.getMass();
    double sign = particleType.getStatistics();
    double mu   = hadronGas.chemicalPotentials[iSpecies];
    double p = 0.0;
    for (int iP=1; iP<=nP; iP++)
      {
      density = hadronGas.computeThermalDensityVsP(temperature, mass,  mu,  g, sign, p);
      h_rho1ThVsP[iSpecies]->SetBinContent(iP, density);
      h_rho1ThVsP[iSpecies]->SetBinError(iP,zero);
      p += deltaP;
      }
    }
  if (reportEnd("HadronGasHistograms",getName(),"fillRho1VsP(...)"))
    ;
}

void HadronGasHistograms::calculateDerivedHistograms()
{
  if (reportStart("HadronGasHistograms",getName(),"calculateDerivedHistograms()"))
    ;
  double zero = 0.0;
  if (!h_rho1)         { cout << "h_rho1   Does not exist" << endl; return;}
  if (!h_rho1Th)       { cout << "h_rho1Th   Does not exist" << endl; return;}
  if (!h_rho1ToRho1Th) { cout << "h_rho1ToRho1Th   Does not exist" << endl; return;}
  if (!h_rho2Corr)     { cout << "h_rho2Corr   Does not exist" << endl; return;}
  if (!h_rho2Uncorr)   { cout << "h_rho2Uncorr   Does not exist" << endl; return;}
  if (!h_rho1rho1)     { cout << "h_rho1rho1   Does not exist" << endl; return;}
  if (!h_rho1thrho1th) { cout << "h_rho1thrho1th   Does not exist" << endl; return;}
  if (!h_rho2)         { cout << "h_rho2   Does not exist" << endl; return;}
  if (!h_C2)           { cout << "h_C2   Does not exist" << endl; return;}
  if (!h_R2)           { cout << "h_R2   Does not exist" << endl; return;}
  if (!h_BF)           { cout << "h_BF   Does not exist" << endl; return;}
  h_rho1ToRho1Th->Divide(h_rho1,h_rho1Th);
  int n = h_rho1Th->GetNbinsX();
  for (int i1=1; i1<= n; i1++)
    {
    double rho1Th_1 = h_rho1Th->GetBinContent(i1);
    double rho1_1   = h_rho1->GetBinContent(i1);

    for (int i2=1; i2<= n; i2++)
      {
      double rho1Th_2     = h_rho1Th->GetBinContent(i2);
      double rho1_2       = h_rho1->GetBinContent(i2);
      double c2           = h_rho2Corr->GetBinContent(i1,i2);
      double c1c1         = h_rho2Uncorr->GetBinContent(i1,i2);
      double rho2         = c2 + rho1Th_1*rho1_2 + rho1_1*rho1Th_2- rho1Th_1*rho1Th_2 +c1c1;
      double rho1rho1     = rho1_1*rho1_2;
      double rho1Thrho1Th = rho1Th_1*rho1Th_2;
      double C2 = rho2 - rho1rho1;
      double R2 = (rho1rho1>0) ? C2/rho1rho1 - 1.0 : 0.0;
      h_rho1rho1->SetBinContent(i1,i2,rho1rho1);         h_rho1rho1->SetBinError(i1,i2,zero);
      h_rho1thrho1th->SetBinContent(i1,i2,rho1Thrho1Th); h_rho1thrho1th->SetBinError(i1,i2,zero);
      h_rho2->SetBinContent(i1,i2,rho2); h_rho2->SetBinError(i1,i2,zero);
      h_C2  ->SetBinContent(i1,i2,C2);   h_C2->SetBinError(i1,i2,zero);
      h_R2  ->SetBinContent(i1,i2,R2);   h_R2->SetBinError(i1,i2,zero);
      }
    if (reportEnd("HadronGasHistograms",getName(),"calculateDerivedHistograms()"))
      ;
    }

  for (int k=1;k<=18;k++)
    {
    h_BF->SetBinError(k,zero);
    }
  // pi- | pi+
  fillBf(2, 2, 3, 1);
  // K- | pi+
  fillBf(2, 4, 5, 2);
  // p- | pi+
  fillBf(2, 8, 9, 3);
  // pi+ | pi-
  fillBf(3, 3, 2, 4);
  // K+ | pi-
  fillBf(3, 5, 4, 5);
  // p+ | pi-
  fillBf(3, 9, 8, 6);
  // pi- | K+
  fillBf(4, 2, 3, 7);
  // K- | K+
  fillBf(4, 4, 5, 8);
  // p- | K+
  fillBf(4, 8, 9, 9);
  // pi+ | K-
  fillBf(5, 3, 2, 10);
  // K+ | K-
  fillBf(5, 5, 4, 11);
  // p+ | K-
  fillBf(5, 9, 8, 12);
  // pi- | p+
  fillBf(8, 2, 3, 13);
  // K- | p+
  fillBf(8, 4, 5, 14);
  // p- | p+
  fillBf(8, 8, 9, 15);
  // pi+ | p-
  fillBf(9, 3, 2, 16);
  // K+ | p-
  fillBf(9, 5, 4, 17);
  // p+ | p-
  fillBf(9, 9, 8, 18);
  if (reportEnd("HadronGasHistograms",getName(),"calculateDerivedHistograms()"))
    ;
}

void HadronGasHistograms::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "Attempting to load HadronGasVsTempHistograms from an invalid file pointer" << endl;
    return;
    }
  //HadronGasConfiguration & ac = *(HadronGasConfiguration*) getConfiguration();
  TString bn  = getHistoBaseName();
  h_rho1All   = loadH1(inputFile,bn+TString("rho1All"));
  if (!h_rho1All)
    {
    if (reportError()) cout << "Could not load histogram: " << bn+TString("allYields") << endl;
    return;
    }
  h_rho1AllVsMass = loadProfile(inputFile,bn+TString("rho1AllVsMass"));
  h_rho1ThVsMass  = loadProfile(inputFile,bn+TString("rho1ThVsMass"));
  h_rho1VsMass    = loadProfile(inputFile,bn+TString("rho1VsMass"));

  h_rho1Th        = loadH1(inputFile,bn+TString("rho1Th"));
  h_rho1          = loadH1(inputFile,bn+TString("rho1"));
  h_rho2Uncorr    = loadH2(inputFile,bn+TString("rho2Uncorr"));
  h_rho2Corr      = loadH2(inputFile,bn+TString("rho2Corr"));

  h_rho2          = loadH2(inputFile,bn+TString("rho2"));
  h_rho1rho1      = loadH2(inputFile,bn+TString("rho1rho1"));
  h_rho1thrho1th  = loadH2(inputFile,bn+TString("rho1thrho1th"));
  h_C2            = loadH2(inputFile,bn+TString("C2"));
  h_R2            = loadH2(inputFile,bn+TString("R2"));
  h_BF            = loadH1(inputFile,bn+TString("BF"));
}

void HadronGasHistograms::fill(HadronGas & hadronGas)
{
  if (reportStart("HadronGasHistograms",getName(),"fill(HadronGas & hadronGas)"))
    ;
  double rho1th1, rho1_1, rho1_2, rho_2_12c, rho_2_12u, mass, g;
  double zero = 0.0;
  int pdgCode1;
  ParticleTypeCollection & particleTypes       = hadronGas.getParticleTypes();
  ParticleTypeCollection & stableParticleTypes = hadronGas.getStableParticleTypes();
  vector<double> & particleDensities           = hadronGas.particleDensities;
  vector<double> & stableParticleDensities     = hadronGas.stableParticleDensities;
  unsigned int nSpecies       = particleTypes.size();
  unsigned int nStableSpecies = stableParticleTypes.size();
  for (unsigned int iSpecies1=0; iSpecies1<nSpecies; iSpecies1++)
    {
    ParticleType * ptr = particleTypes.getParticleType(iSpecies1);
    if (ptr==nullptr)
      {
      if (reportError("HadronGasHistograms",getName(),"fill(HadronGas & hadronGas)"))
        {
        cout << "Null pointer at iSpecies1:" << iSpecies1 << endl;
        exit(1);
        }
      }
    ParticleType & particleType = *ptr;
    g       = particleType.getSpinFactor();
    mass    = particleType.getMass();
    rho1th1 = particleDensities[iSpecies1];   ///
    h_rho1All->SetBinContent(iSpecies1+1, rho1th1);
    h_rho1All->SetBinError  (iSpecies1+1, 0.0);
    h_rho1AllVsMass->Fill(mass,rho1th1/g);
    }
  for (unsigned int iStableSpecies1=0; iStableSpecies1<nStableSpecies; iStableSpecies1++)
    {
    ParticleType & type1 = *(stableParticleTypes[iStableSpecies1]);
    g         = type1.getSpinFactor();
    mass      = type1.getMass();
    pdgCode1  = type1.getPdgCode();
    int iSpecies1 = particleTypes.findIndexForPdgCode(pdgCode1);
    rho1_1    = stableParticleDensities[iStableSpecies1];
    rho1th1   = particleDensities[iSpecies1];
    h_rho1Th->SetBinContent(iStableSpecies1+1, rho1th1);  h_rho1Th->SetBinError(iStableSpecies1,zero);
    h_rho1  ->SetBinContent(iStableSpecies1+1, rho1_1);   h_rho1  ->SetBinError(iStableSpecies1,zero);
    h_rho1ThVsMass->Fill(mass,rho1th1/g);
    h_rho1VsMass->Fill(mass,rho1_1);
    for (unsigned int iStableSpecies2=0; iStableSpecies2<nStableSpecies; iStableSpecies2++)
      {
      rho1_2    = stableParticleDensities[iStableSpecies2];
      rho_2_12c = hadronGas.stableParticlePairDensities[iStableSpecies1][iStableSpecies2];
      rho_2_12u = hadronGas.stableParticlePairUncorrelatedDensities[iStableSpecies1][iStableSpecies2];
      h_rho1rho1->SetBinContent  (iStableSpecies1+1, iStableSpecies2+1, rho1_1*rho1_2);
      h_rho1rho1->SetBinError    (iStableSpecies1+1, iStableSpecies2+1, zero);
      h_rho2Corr->SetBinContent  (iStableSpecies1+1, iStableSpecies2+1, rho_2_12c);
      h_rho2Corr->SetBinError    (iStableSpecies1+1, iStableSpecies2+1, zero);
      h_rho2Uncorr->SetBinContent(iStableSpecies1+1, iStableSpecies2+1, rho_2_12u);
      h_rho2Uncorr->SetBinError  (iStableSpecies1+1, iStableSpecies2+1, zero);
      }
    }
  if (reportEnd("HadronGasHistograms",getName(),"fill(HadronGas & hadronGas)"))
    ;
}

