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

GlobalHistos::GlobalHistos(Task * _parent,
                           const TString & _name,
                           Configuration & _configuration,
                           vector<ParticleFilter*> _particleFilters)
:
Histograms(_parent,_name,_configuration),
particleFilters(_particleFilters),
h_n(),
h_e(),
h_q(),
h_s(),
h_b(),
h_ptSum(),
h_ptAvg(),
h_nVsN(),
h_eVsN(),
h_qVsN(),
h_bVsN(),
h_ptSumVsN(),
h_ptAvgVsN(),
h_eVsE(),
h_qVsE(),
h_bVsE(),
h_qVsQ(),
h_bVsQ(),
h_bVsB(),
fill2D(false),
fillCorrelationHistos(false)
{
  appendClassName("GlobalHistos");
}



// for now use the same boundaries for eta and y histogram
void GlobalHistos::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nParticleFilters = particleFilters.size();
  Configuration & configuration = getConfiguration();
  const TString & bn  = getName();
  const TString & ptn = getParentTaskName();
  const TString & ppn = getParentPathName();
  fillCorrelationHistos = configuration.getValueBool(ppn,"FillCorrelationHistos");
  fill2D                = configuration.getValueBool(ppn,"Fill2D");
  int nBins_n           = configuration.getValueInt(ppn,"nBins_n");
  int nBins_n2          = configuration.getValueInt(ppn,"nBins_n2");
  double min_n          = configuration.getValueDouble(ppn,"Min_n");
  double max_n          = configuration.getValueDouble(ppn,"Max_n");
  int nBins_e           = configuration.getValueInt(ppn,"nBins_e");
  int nBins_e2          = configuration.getValueInt(ppn,"nBins_e2");
  double min_e          = configuration.getValueDouble(ppn,"Min_e");
  double max_e          = configuration.getValueDouble(ppn,"Max_e");
  int nBins_q           = configuration.getValueInt(ppn,"nBins_q");
  int nBins_q2          = configuration.getValueInt(ppn,"nBins_q2");
  double min_q          = configuration.getValueDouble(ppn,"Min_q");
  double max_q          = configuration.getValueDouble(ppn,"Max_q");
  int nBins_b           = configuration.getValueInt(ppn,"nBins_b");
  int nBins_b2          = configuration.getValueInt(ppn,"nBins_b2");
  double min_b          = configuration.getValueDouble(ppn,"Min_b");
  double max_b          = configuration.getValueDouble(ppn,"Max_b");
  int nBins_ptSum       = configuration.getValueInt(ppn,"nBins_ptSum");
  int nBins_ptSum2      = configuration.getValueInt(ppn,"nBins_ptSum2");
  double min_ptSum      = configuration.getValueDouble(ppn,"Min_ptSum");
  double max_ptSum      = configuration.getValueDouble(ppn,"Max_ptSum");
  int nBins_ptAvg       = configuration.getValueInt(ppn,"nBins_ptAvg");
  int nBins_ptAvg2      = configuration.getValueInt(ppn,"nBins_ptAvg2");
  double min_ptAvg      = configuration.getValueDouble(ppn,"Min_ptAvg");
  double max_ptAvg      = configuration.getValueDouble(ppn,"Max_ptAvg");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  G:Parent Task Name....................: " << ptn << endl;
    cout << "  G:Parent Path Name....................: " << ppn << endl;
    cout << "  G:Histo Base Name.....................: " << bn << endl;
    cout << "  G:nParticleFilters....................: "  <<  nParticleFilters << endl;
    cout << "  G:FillCorrelationHistos...............: "  <<  fillCorrelationHistos << endl;
    cout << "  G:Fill2D..............................: "  <<  fill2D << endl;
    cout << "  G:nBins_n.............................: "  <<  nBins_n << endl;
    cout << "  G:nBins_n2............................: "  <<  nBins_n2 << endl;
    cout << "  G:Min_n...............................: "  <<  min_n << endl;
    cout << "  G:Max_n...............................: "  <<  max_n << endl;
    cout << "  G:nBins_e.............................: "  <<  nBins_e << endl;
    cout << "  G:nBins_e2............................: "  <<  nBins_e2 << endl;
    cout << "  G:Min_e...............................: "  <<  min_e << endl;
    cout << "  G:Max_e...............................: "  <<  max_e << endl;
    cout << "  G:nBins_q.............................: "  <<  nBins_q << endl;
    cout << "  G:nBins_q2............................: "  <<  nBins_q2 << endl;
    cout << "  G:Min_q...............................: "  <<  min_q << endl;
    cout << "  G:Max_q...............................: "  <<  max_q << endl;
    cout << "  G:nBins_b.............................: "  <<  nBins_b << endl;
    cout << "  G:nBins_b2............................: "  <<  nBins_b2 << endl;
    cout << "  G:Min_b...............................: "  <<  min_b << endl;
    cout << "  G:Max_b...............................: "  <<  max_b << endl;
    cout << "  G:nBins_ptSum.........................: "  <<  nBins_ptSum << endl;
    cout << "  G:nBins_ptSum2........................: "  <<  nBins_ptSum2 << endl;
    cout << "  G:Min_ptSum...........................: "  <<  min_ptSum << endl;
    cout << "  G:Max_ptSum...........................: "  <<  max_ptSum << endl;
    cout << "  G:nBins_ptAvg.........................: "  <<  nBins_ptAvg << endl;
    cout << "  G:nBins_ptAvg2........................: "  <<  nBins_ptAvg2 << endl;
    cout << "  G:Min_ptAvg...........................: "  <<  min_ptAvg << endl;
    cout << "  G:Max_ptAvg...........................: "  <<  max_ptAvg << endl;
    }

  //setSeverityLevel(MessageLogger::Debug);

  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  TString name;
  TString pfName1 = particleFilters[k1]->getName();
  h_n.push_back(     createHistogram(makeName(bn,pfName1,"n"), nBins_n, min_n,  max_n, "n","N") );
  h_e.push_back(     createHistogram(makeName(bn,pfName1,"e"), nBins_e, min_e,  max_e, "e","N") );
  h_q.push_back(     createHistogram(makeName(bn,pfName1,"q"), nBins_q, min_q,  max_q, "q","N") );
  h_s.push_back(     createHistogram(makeName(bn,pfName1,"s"), nBins_b, min_b,  max_b, "s","N") );
  h_b.push_back(     createHistogram(makeName(bn,pfName1,"b"), nBins_b, min_b,  max_b, "b","N") );
  h_ptSum.push_back( createHistogram(makeName(bn,pfName1,"ptSum"), nBins_ptSum, min_ptSum,  max_ptSum, "ptSum","N") );
  h_ptAvg.push_back( createHistogram(makeName(bn,pfName1,"ptAvg"), nBins_ptAvg, min_ptAvg,  max_ptAvg, "ptAvg","N") );
  p_eVsN.push_back(  createProfile(makeName(bn,pfName1,TString("n"),pfName1,TString("eProf")),nBins_n, min_n, max_n, "n", "e"));
  p_qVsN.push_back(  createProfile(makeName(bn,pfName1,TString("n"),pfName1,TString("qProf")),nBins_n, min_n, max_n, "n", "q"));
  p_sVsN.push_back(  createProfile(makeName(bn,pfName1,TString("n"),pfName1,TString("sProf")),nBins_n, min_n, max_n, "n", "s"));
  p_bVsN.push_back(  createProfile(makeName(bn,pfName1,TString("n"),pfName1,TString("bProf")),nBins_n, min_n, max_n, "n", "b"));
  p_ptSumVsN.push_back( createProfile(makeName(bn,pfName1,TString("n"),pfName1,TString("ptSumProf")),nBins_n, min_n, max_n, "n", "ptSum"));
  p_ptAvgVsN.push_back( createProfile(makeName(bn,pfName1,TString("n"),pfName1,TString("ptAvgProf")),nBins_n, min_n, max_n, "n", "ptAvg"));
  if ( fill2D )
    {
    h_ptSumVsN.push_back( createHistogram(makeName(bn,pfName1,TString("n"),pfName1,TString("ptSum")), nBins_n2, min_n, max_n, nBins_ptSum2, min_ptSum,  max_ptSum, "n", "ptSum", "N") );
    h_ptAvgVsN.push_back( createHistogram(makeName(bn,pfName1,TString("n"),pfName1,TString("ptAvg")), nBins_n2, min_n, max_n, nBins_ptAvg2, min_ptAvg,  max_ptAvg, "n", "ptAvg", "N") );
    h_eVsN.push_back(     createHistogram(makeName(bn,pfName1,TString("n"),pfName1,TString("e")),     nBins_n2, min_n, max_n, nBins_e2, min_e,  max_e, "n", "e", "N") );
    h_qVsN.push_back(     createHistogram(makeName(bn,pfName1,TString("n"),pfName1,TString("q")),     nBins_n2, min_n, max_n, nBins_q2, min_q,  max_q, "n", "q", "N") );
    h_bVsN.push_back(     createHistogram(makeName(bn,pfName1,TString("n"),pfName1,TString("b")),     nBins_n2, min_n, max_n, nBins_b2, min_b,  max_b, "n", "b", "N") );
    }

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
        h_eVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"e"), nBins_n2, min_n, max_n, nBins_e2, min_e,  max_e, "n", "e", "N") );
        h_qVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"q"), nBins_n2, min_n, max_n, nBins_q2, min_q,  max_q, "n", "q", "N") );
        h_bVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"b"), nBins_n2, min_n, max_n, nBins_b2, min_b,  max_b, "n", "b", "N") );
        h_qVsE.push_back( createHistogram(makeName(bn,pfName1,"e",pfName2,"q"), nBins_e2, min_e, max_e, nBins_q2, min_q,  max_q, "e", "q", "N") );
        h_bVsE.push_back( createHistogram(makeName(bn,pfName1,"e",pfName2,"b"), nBins_e2, min_e, max_e, nBins_b2, min_b,  max_b, "e", "b", "N") );
        h_bVsQ.push_back( createHistogram(makeName(bn,pfName1,"q",pfName2,"b"), nBins_q2, min_q, max_q, nBins_b2, min_b,  max_b, "q", "b", "N") );
        }
      }
    }
  cout << "done: " <<  endl;

  }
  if (reportEnd(__FUNCTION__))
    ;
}

void GlobalHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  Configuration & configuration = getConfiguration();
  const TString & bn  = getName();
  const TString & ptn = getParentTaskName();
  const TString & ppn = getParentPathName();
  fillCorrelationHistos = configuration.getValueBool(ppn,"FillCorrelationHistos");
  fill2D                = configuration.getValueBool(ppn,"Fill2D");
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  G:Parent Task Name....................: " << ptn << endl;
    cout << "  G:Parent Path Name....................: " << ppn << endl;
    cout << "  G:Histo Base Name.....................: " << bn << endl;
    cout << "  G:nParticleFilters....................: "  <<  nParticleFilters << endl;
    cout << "  G:FillCorrelationHistos...............: "  <<  fillCorrelationHistos << endl;
    cout << "  G:Fill2D..............................: "  <<  fill2D << endl;
    }

  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  TString pfName1 = particleFilters[k1]->getName();
  h_n.push_back( loadH1(inputFile, makeName(bn,pfName1,"n")) );
  h_e.push_back( loadH1(inputFile, makeName(bn,pfName1,"e")) );
  h_q.push_back( loadH1(inputFile, makeName(bn,pfName1,"q")) );
  h_s.push_back( loadH1(inputFile, makeName(bn,pfName1,"s")) );
  h_b.push_back( loadH1(inputFile, makeName(bn,pfName1,"b")) );
  h_ptSum.push_back( loadH1(inputFile, makeName(bn,pfName1,"ptSum")));
  h_ptAvg.push_back( loadH1(inputFile, makeName(bn,pfName1,"ptAvg")));

  p_eVsN.push_back(     loadProfile(inputFile, makeName(bn,pfName1,"n",pfName1,"eProf")));
  p_qVsN.push_back(     loadProfile(inputFile, makeName(bn,pfName1,"n",pfName1,"qProf")));
  p_sVsN.push_back(     loadProfile(inputFile, makeName(bn,pfName1,"n",pfName1,"sProf")));
  p_bVsN.push_back(     loadProfile(inputFile, makeName(bn,pfName1,"n",pfName1,"bProf")));
  p_ptSumVsN.push_back( loadProfile(inputFile, makeName(bn,pfName1,"n",pfName1,"ptSumProf")));
  p_ptAvgVsN.push_back( loadProfile(inputFile, makeName(bn,pfName1,"n",pfName1,"ptAvgProf")));

  if ( fill2D )
    {
    h_ptSumVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName1,"ptSum")));
    h_ptAvgVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName1,"ptAvg")));
    h_eVsN.push_back(     loadH2(inputFile, makeName(bn,pfName1,"n",pfName1,"e")));
    h_qVsN.push_back(     loadH2(inputFile, makeName(bn,pfName1,"n",pfName1,"q")));
    h_bVsN.push_back(     loadH2(inputFile, makeName(bn,pfName1,"n",pfName1,"b")));
    }

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
        h_eVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"e")) );
        h_qVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"q")) );
        h_bVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"b")) );
        h_qVsE.push_back( loadH2(inputFile, makeName(bn,pfName1,"e",pfName2,"q")) );
        h_bVsE.push_back( loadH2(inputFile, makeName(bn,pfName1,"e",pfName2,"b")) );
        h_bVsQ.push_back( loadH2(inputFile, makeName(bn,pfName1,"q",pfName2,"b")) );
        }
      }
    }
  }
  if (reportEnd(__FUNCTION__))
    ;
}

void GlobalHistos::fill(vector<double> & n, vector<double> & ptSum, vector<double> & e, vector<double> & q, vector<double> & s, vector<double> & b, double weight)
{
  unsigned int nParticleFilters = particleFilters.size();
  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  double nPart = n[k1];
  double pts = ptSum[k1];
  double ptAvg = 0.0;
  if (nPart>0) ptAvg = pts/nPart;
  //cout << "nPart:" << nPart << " pts:" << pts << " ptAvg:" << ptAvg << endl;
  h_n[k1]->Fill(n[k1],weight);
  h_e[k1]->Fill(e[k1],weight);
  h_q[k1]->Fill(q[k1],weight);
  h_s[k1]->Fill(s[k1],weight);
  h_b[k1]->Fill(b[k1],weight);
  h_ptSum[k1]->Fill(pts, weight);
  if (nPart>0)
    {
    h_ptAvg[k1]->Fill(ptAvg, weight);
    p_eVsN[k1]->Fill(nPart,e[k1]);
    p_qVsN[k1]->Fill(nPart,q[k1]);
    p_sVsN[k1]->Fill(nPart,s[k1]);
    p_bVsN[k1]->Fill(nPart,b[k1]);
    p_ptSumVsN[k1]->Fill(nPart,pts);
    p_ptAvgVsN[k1]->Fill(nPart,ptAvg);
    }

  if (fill2D)
    {
    h_ptSumVsN[k1]->Fill(n[k1],pts,weight);
    if (nPart>0) h_ptAvgVsN[k1]->Fill(nPart,ptAvg, weight);
    h_eVsN[k1]->Fill(nPart,e[k1],weight);
    h_qVsN[k1]->Fill(nPart,q[k1],weight);
    h_bVsN[k1]->Fill(nPart,b[k1],weight);
    }

  if ( fillCorrelationHistos )
    {
    //unsigned int n2 = nParticleFilters-k1;
    unsigned int nND   = 0; // non diagonal only
    for (unsigned int k2=k1; k2<nParticleFilters; k2++)
      {
      if (k1!=k2)
        {
        h_nVsN[nND]->Fill(n[k1],n[k2],weight);
        h_eVsE[nND]->Fill(e[k1],e[k2],weight);
        h_qVsQ[nND]->Fill(q[k1],q[k2],weight);
        h_bVsB[nND]->Fill(b[k1],b[k2],weight);
        h_eVsN[nND]->Fill(n[k1],e[k2],weight);
        h_qVsN[nND]->Fill(n[k1],q[k2],weight);
        h_bVsN[nND]->Fill(n[k1],b[k2],weight);
        h_qVsE[nND]->Fill(e[k1],q[k2],weight);
        h_bVsE[nND]->Fill(e[k1],b[k2],weight);
        h_bVsQ[nND]->Fill(q[k1],b[k2],weight);
        nND++;
        }
      }
    }
  }
}
