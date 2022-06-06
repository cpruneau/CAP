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
#ifndef CAP__ParticlePairBFHistos
#define CAP__ParticlePairBFHistos

#include "Histograms.hpp"
#include "Particle.hpp"
#include "ParticleDigit.hpp"

class ParticlePairBFHistos : public Histograms
{
public:
    
    ParticlePairBFHistos(const TString &       name,
                         const Configuration & configuration,
                         LogLevel  debugLevel);
    virtual ~ParticlePairBFHistos();
    virtual void createHistograms();
    virtual void loadHistograms(TFile * inputFile);
    virtual void calculateBalanceFunctions();

protected:
    
    bool fillEta;
    bool fillY;
    bool fillP2;
    unsigned int nBins_n2;
    float        min_n2;
    float        max_n2;
    unsigned int nBins_pt;
    float        min_pt;
    float        max_pt;
    float        scale_pt;
    unsigned int nBins_phi;
    float        min_phi;
    float        max_phi;
    float        scale_phi;
    unsigned int nBins_eta;
    float        min_eta;
    float        max_eta;
    float        range_eta;
    unsigned int nBins_y;
    float        min_y;
    float        max_y;
    float        range_y;
    unsigned int nBins_Dphi;
    float        min_Dphi;
    float        max_Dphi;
    unsigned int nBins_Deta;
    float        min_Deta;
    float        max_Deta;
    float        range_Deta;
    unsigned int nBins_Dy;
    float        min_Dy;
    float        max_Dy;
    float        range_Dy;
    
    TH1 * rho1_1;
    TH1 * rho1_1B;
    TH1 * rho1_2;
    TH1 * rho1_2B;
    TH2 * rho1_1_phiY;
    TH2 * rho1_1B_phiY;
    TH2 * rho1_2_phiY;
    TH2 * rho1_2B_phiY;
    TH2 * r2_12;
    TH2 * r2_1B2;
    TH2 * r2_12B;
    TH2 * r2_1B2B;
    TH2 * r2CI;
    TH2 * r2CD;
    TH2 * b12B;
    TH2 * b1B2;
    TH1 * h_x;
    TH1 * h_y;
    TH2 * r2Diff;
    TH2 * b2Diff;
    
    ClassDef(ParticlePairBFHistos,0)
};


#endif /* CAP__ParticlePairBFHistos  */
