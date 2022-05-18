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
#include "GlobalHistos.hpp"
ClassImp(GlobalHistos);

GlobalHistos::GlobalHistos(const TString &         _name,
                           const Configuration &   _configuration,
                           vector<ParticleFilter*> _particleFilters,
                           LogLevel                _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
particleFilters(_particleFilters),
h_n(),
h_e(),
h_q(),
h_s(),
h_b(),
h_nVsN(),
h_eVsN(),
h_qVsN(),
h_bVsN(),
h_eVsE(),
h_qVsE(),
h_bVsE(),
h_qVsQ(),
h_bVsQ(),
h_bVsB()
{
  appendClassName("GlobalHistos");
  setInstanceName(_name);
}



// for now use the same boundaries for eta and y histogram
void GlobalHistos::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  TString bn = getName();
  TString name;
  unsigned int nParticleFilters = particleFilters.size();
  if (reportDebug(__FUNCTION__))  cout << "nParticleFilters: " << nParticleFilters << endl;
  const Configuration & configuration = getConfiguration();
  fillCorrelationHistos = configuration.getValueBool("fillCorrelationHistos");
  
  int nBins_n  = configuration.getValueInt("nBins_n");
  int nBins_n2 = configuration.getValueInt("nBins_n2");
  double min_n = configuration.getValueDouble("min_n");
  double max_n = configuration.getValueDouble("max_n");

  int nBins_e  = configuration.getValueInt("nBins_e");
  int nBins_e2 = configuration.getValueInt("nBins_e2");
  double min_e = configuration.getValueDouble("min_e");
  double max_e = configuration.getValueDouble("max_e");
  
  int nBins_q  = configuration.getValueInt("nBins_q");
  int nBins_q2 = configuration.getValueInt("nBins_q2");
  double min_q = configuration.getValueDouble("min_q");
  double max_q = configuration.getValueDouble("max_q");
  
  int nBins_b  = configuration.getValueInt("nBins_b");
  int nBins_b2 = configuration.getValueInt("nBins_b2");
  double min_b = configuration.getValueDouble("min_b");
  double max_b = configuration.getValueDouble("max_b");
  
  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  TString pfName1 = particleFilters[k1]->getName();
  h_n.push_back( createHistogram(makeName(bn,pfName1,"n"),  nBins_n, min_n,  max_n, "n","N") );
  h_e.push_back( createHistogram(makeName(bn,pfName1,"e"),  nBins_e, min_e,  max_e, "e","N") );
  h_q.push_back( createHistogram(makeName(bn,pfName1,"q"),  nBins_q, min_q,  max_q, "q","N") );
  h_s.push_back( createHistogram(makeName(bn,pfName1,"s"),  nBins_b, min_b,  max_b, "s","N") );
  h_b.push_back( createHistogram(makeName(bn,pfName1,"b"),  nBins_b, min_b,  max_b, "b","N") );
  if ( fillCorrelationHistos )
    {
    for (unsigned int k2=k1; k2<nParticleFilters; k2++)
      {
      TString pfName2 = particleFilters[k2]->getName();
      if (k1!=k2)
        {
        h_nVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"n"), nBins_n2, min_n, max_n, nBins_n2, min_n,  max_n, "n", "n", "N") );
        h_eVsE.push_back( createHistogram(makeName(bn,pfName1,"e",pfName2,"e"), nBins_e2, min_e, max_e, nBins_e2, min_e,  max_e, "e", "e", "N") );
        h_qVsQ.push_back( createHistogram(makeName(bn,pfName1,"q",pfName2,"q"), nBins_q2, min_q, max_q, nBins_q2, min_q,  max_q, "q", "q", "N") );
        h_bVsB.push_back( createHistogram(makeName(bn,pfName1,"b",pfName2,"b"), nBins_b2, min_b, max_b, nBins_b2, min_b,  max_b, "b", "b", "N") );
        }
      h_eVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"e"), nBins_n2, min_n, max_n, nBins_e2, min_e,  max_e, "n", "e", "N") );
      h_qVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"q"), nBins_n2, min_n, max_n, nBins_q2, min_q,  max_q, "n", "q", "N") );
      h_bVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"b"), nBins_n2, min_n, max_n, nBins_b2, min_b,  max_b, "n", "b", "N") );
      h_qVsE.push_back( createHistogram(makeName(bn,pfName1,"e",pfName2,"q"), nBins_e2, min_e, max_e, nBins_q2, min_q,  max_q, "e", "q", "N") );
      h_bVsE.push_back( createHistogram(makeName(bn,pfName1,"e",pfName2,"b"), nBins_e2, min_e, max_e, nBins_b2, min_b,  max_b, "e", "b", "N") );
      h_bVsQ.push_back( createHistogram(makeName(bn,pfName1,"q",pfName2,"b"), nBins_q2, min_q, max_q, nBins_b2, min_b,  max_b, "q", "b", "N") );
      }
    }
  }
  if (reportEnd(__FUNCTION__))
    ;
}

void GlobalHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!inputFile)
    {
    if (reportFatal(__FUNCTION__)) cout << "Attempting to load GlobalHistos from an invalid file pointer" << endl;
    return;
    }
  const Configuration & configuration = getConfiguration();
  fillCorrelationHistos = configuration.getValueBool("fillCorrelationHistos");

  
  TString bn = getName();
  TString name;
  unsigned int nParticleFilters = particleFilters.size();
  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  TString pfName1 = particleFilters[k1]->getName();
  h_n.push_back( loadH1(inputFile, makeName(bn,pfName1,"n")) );
  h_e.push_back( loadH1(inputFile, makeName(bn,pfName1,"e")) );
  h_q.push_back( loadH1(inputFile, makeName(bn,pfName1,"q")) );
  h_s.push_back( loadH1(inputFile, makeName(bn,pfName1,"s")) );
  h_b.push_back( loadH1(inputFile, makeName(bn,pfName1,"b")) );
  if ( fillCorrelationHistos )
    {
    for (unsigned int k2=k1; k2<nParticleFilters; k2++)
      {
      TString pfName2 = particleFilters[k2]->getName();
      if (k1!=k2)
        {
        h_nVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"n")) );
        h_eVsE.push_back( loadH2(inputFile, makeName(bn,pfName1,"e",pfName2,"e")) );
        h_qVsQ.push_back( loadH2(inputFile, makeName(bn,pfName1,"q",pfName2,"q")) );
        h_bVsB.push_back( loadH2(inputFile, makeName(bn,pfName1,"b",pfName2,"b")) );
        }
      h_eVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"e")) );
      h_qVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"q")) );
      h_bVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"b")) );
      h_qVsE.push_back( loadH2(inputFile, makeName(bn,pfName1,"e",pfName2,"q")) );
      h_bVsE.push_back( loadH2(inputFile, makeName(bn,pfName1,"e",pfName2,"b")) );
      h_bVsQ.push_back( loadH2(inputFile, makeName(bn,pfName1,"q",pfName2,"b")) );
      }
    }
  }
  if (reportEnd(__FUNCTION__))
    ;
}

// =============================================================================
// n : number of particles accepted by filters
// e : total energy accepted by filters
// q : net charge accepted by filters
// b : net baryon number accepted by filters
// =============================================================================
void GlobalHistos::fill(vector<double> & n, vector<double> & e, vector<double> & q, vector<double> & s, vector<double> & b, double weight)
{
  unsigned int nParticleFilters = particleFilters.size();
  unsigned int nAll  = 0; // all
  unsigned int nND   = 0; // non diagonal only
  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  h_n[k1]->Fill(n[k1],weight);
  h_e[k1]->Fill(e[k1],weight);
  h_q[k1]->Fill(q[k1],weight);
  h_s[k1]->Fill(s[k1],weight);
  h_b[k1]->Fill(b[k1],weight);
  if ( fillCorrelationHistos )
    {
    //unsigned int n2 = nParticleFilters-k1;
    for (unsigned int k2=k1; k2<nParticleFilters; k2++)
      {
      if (k1!=k2)
        {
        h_nVsN[nND]->Fill(n[k1],n[k2],weight);
        h_eVsE[nND]->Fill(e[k1],e[k2],weight);
        h_qVsQ[nND]->Fill(q[k1],q[k2],weight);
        h_bVsB[nND]->Fill(b[k1],b[k2],weight);
        nND++;
        }
      h_eVsN[nAll]->Fill(n[k1],e[k2],weight);
      h_qVsN[nAll]->Fill(n[k1],q[k2],weight);
      h_bVsN[nAll]->Fill(n[k1],b[k2],weight);
      h_qVsE[nAll]->Fill(e[k1],q[k2],weight);
      h_bVsE[nAll]->Fill(e[k1],b[k2],weight);
      h_bVsQ[nAll]->Fill(q[k1],b[k2],weight);
      nAll++;
      }
    }
  }
}
