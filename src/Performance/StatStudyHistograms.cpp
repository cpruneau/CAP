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
#include "StatStudyHistograms.hpp"
using CAP::StatStudyHistograms;

ClassImp(StatStudyHistograms);

StatStudyHistograms::StatStudyHistograms(Task * _parent,
                                         const String & _name
                                         const Configuration & _configuration)
:
HistogramGroup(_parent,_name,_configuration),
nSubSamples(10)
{
  appendClassName("StatStudyHistograms");
}

StatStudyHistograms::~StatStudyHistograms()
{

}

void StatStudyHistograms::HistogramsCreate()
{
  int    nBins  = 400;
  double f1_min = 0.0;
  double f1_max = 400.0;
  int    nBins2 = 400;
  double f2_min = 0.0;
  double f2_max = f1_max*f1_max;
  const Configuration & configuration = getConfiguration();
  nSubSamples = configuration->getValueInt(getParentTaskName(),"nSubSamples");

  String bn = getParentTaskName();
  f1_1     = createHistogram(createName(bn,"f1_1",    nBins, f1_min, f1_max, "n_{1}^{(1)}",  "Counts");
  f1_2     = createHistogram(createName(bn,"f1_2",    nBins, f1_min, f1_max, "n_{1}^{(2)}",  "Counts");
  f1_1Eff  = createHistogram(createName(bn,"f1_1Eff", nBins, f1_min, f1_max, "n_{1}^{(1)}",  "Counts");
  f1_2Eff  = createHistogram(createName(bn,"f1_2Eff", nBins, f1_min, f1_max, "n_{1}^{(2)}",  "Counts");

  f2_11    = createHistogram(createName(bn,"f2_11",   nBins2, f2_min, f2_max, "n_{2}^{(11)}", "Counts");
  f2_12    = createHistogram(createName(bn,"f2_12",   nBins2, f2_min, f2_max, "n_{2}^{(12)}", "Counts");
  f2_22    = createHistogram(createName(bn,"f2_22",   nBins2, f2_min, f2_max, "n_{2}^{(22)}", "Counts");
  f2_11Eff = createHistogram(createName(bn,"f2_11Eff",nBins2, f2_min, f2_max, "n_{2}^{(11)}", "Counts");
  f2_12Eff = createHistogram(createName(bn,"f2_12Eff",nBins2, f2_min, f2_max, "n_{2}^{(12)}", "Counts");
  f2_22Eff = createHistogram(createName(bn,"f2_22Eff",nBins2, f2_min, f2_max, "n_{2}^{(22)}", "Counts");

  nBins = 1000;
  r2_11       = createHistogram(createName(bn,"r2_11",    nBins, -0.02,  0.02,    "R_{2}^{(11)}", "Counts");
  r2_12       = createHistogram(createName(bn,"r2_12",    nBins, -0.02,  0.02,    "R_{2}^{(12)}", "Counts");
  r2_22       = createHistogram(createName(bn,"r2_22",    nBins, -0.02,  0.02,    "R_{2}^{(22)}", "Counts");
  nudyn_12    = createHistogram(createName(bn,"nudyn_12", nBins, -0.02,   0.02,   "#nu_{dyn}^{(12)}", "Counts");
  r2_11Eff    = createHistogram(createName(bn,"r2_11Eff", nBins, -0.02,  0.02,    "R_{2}^{(11)}", "Counts");
  r2_12Eff    = createHistogram(createName(bn,"r2_12Eff", nBins, -0.02,  0.02,    "R_{2}^{(12)}", "Counts");
  r2_22Eff    = createHistogram(createName(bn,"r2_22Eff",   nBins, -0.02,  0.02,  "R_{2}^{(22)}", "Counts");
  nudyn_12Eff = createHistogram(createName(bn,"nudyn_12Eff",nBins, -0.02,   0.02, "#nu_{dyn}^{(12)}", "Counts");

  f1_1Ratio     = createHistogram(createName(bn,"f1_1Ratio",    nBins,0.8,   0.90, "f_{1}^{(1,eff)}/f_{1}^{(1)}",   "Counts");
  f1_2Ratio     = createHistogram(createName(bn,"f1_2Ratio",    nBins,0.8,   0.90, "f_{1}^{(2,eff)}/f_{1}^{(2)}",   "Counts");
  f2_11Ratio    = createHistogram(createName(bn,"f2_11Ratio",   nBins,0.5,   1.00, "f_{2}^{(11,eff)}/f_{2}^{(11)}", "Counts");
  f2_12Ratio    = createHistogram(createName(bn,"f2_12Ratio",   nBins,0.5,   1.00, "f_{2}^{(12,eff)}/f_{2}^{(12)}", "Counts");
  f2_22Ratio    = createHistogram(createName(bn,"f2_22Ratio",   nBins,0.5,   1.00, "f_{2}^{(22,eff)}/f_{2}^{(22)}", "Counts");

  r2_11Ratio    = createHistogram(createName(bn,"r2_11Ratio",   nBins,0.00,  2.00, "R_{2}^{(11,eff)}/R_{2}^{(11)}", "Counts");
  r2_12Ratio    = createHistogram(createName(bn,"r2_12Ratio",   nBins,0.00,  2.00, "R_{2}^{(12,eff)}/R_{2}^{(12)}", "Counts");
  r2_22Ratio    = createHistogram(createName(bn,"r2_22Ratio",   nBins,0.00,  2.00, "R_{2}^{(22,eff)}/R_{2}^{(22)}", "Counts");
  nudyn_12Ratio = createHistogram(createName(bn,"nudyn_12Ratio",nBins,0.00,  2.00, "#nu_{dyn}^{(12,eff)}/#nu_{dyn}^{(12)}", "Counts");

  ///
  nBins = nSubSamples+20;

  f1_1_vsSub     = createProfile(createName(bn,"f1_1_vsSub"),    nBins,0.0,  double(nBins),  "sub-sample","f_{1}^{(1)}");
  f1_2_vsSub     = createProfile(createName(bn,"f1_2_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","f_{1}^{(2)}");
  f2_11_vsSub    = createProfile(createName(bn,"f2_11_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(11)}");
  f2_12_vsSub    = createProfile(createName(bn,"f2_12_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(12)}");
  f2_22_vsSub    = createProfile(createName(bn,"f2_22_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(22)}");
  r2_11_vsSub    = createHistogram(createName(bn,"r2_11_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(11)}");
  r2_12_vsSub    = createHistogram(createName(bn,"r2_12_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(12)}");
  r2_22_vsSub    = createHistogram(createName(bn,"r2_22_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(22)}");
  nudyn_12_vsSub = createHistogram(createName(bn,"nudyn_12_vsSub",nBins,0.0,  double(nBins),  "sub-sample","#nu_{dyn}^{(12)}");

  f1_1Eff_vsSub     = createProfile(createName(bn,"f1_1Eff_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","f_{1}^{(1,eff)}");
  f1_2Eff_vsSub     = createProfile(createName(bn,"f1_2Eff_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","f_{1}^{(2,eff)}");
  f2_11Eff_vsSub    = createProfile(createName(bn,"f2_11Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(11,eff)}");
  f2_12Eff_vsSub    = createProfile(createName(bn,"f2_12Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(12,eff)}");
  f2_22Eff_vsSub    = createProfile(createName(bn,"f2_22Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(22,eff)}");
  r2_11Eff_vsSub    = createHistogram(createName(bn,"r2_11Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(11,eff)}");
  r2_12Eff_vsSub    = createHistogram(createName(bn,"r2_12Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(12,eff)}");
  r2_22Eff_vsSub    = createHistogram(createName(bn,"r2_22Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(22,eff)}");
  nudyn_12Eff_vsSub = createHistogram(createName(bn,"nudyn_12Eff_vsSub"),nBins,0.0,  double(nBins),  "sub-sample","#nu_{dyn}^{(12,eff)}");

  f1_1Ratio_vsSub     = createHistogram(createName(bn,"f1_1Ratio_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","f_{1}^{(1,eff)}/f_{1}^{(1))");
  f1_2Ratio_vsSub     = createHistogram(createName(bn,"f1_2Ratio_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","f_{1}^{(2,eff)}/f_{1}^{(2)}");
  f2_11Ratio_vsSub    = createHistogram(createName(bn,"f2_11Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(11,eff)}/f_{2}^{(11)}");
  f2_12Ratio_vsSub    = createHistogram(createName(bn,"f2_12Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(12,eff)}/f_{2}^{(12)}");
  f2_22Ratio_vsSub    = createHistogram(createName(bn,"f2_22Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(22,eff)}/f_{2}^{(22)}");
  r2_11Ratio_vsSub    = createHistogram(createName(bn,"r2_11Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(11,eff)}/R_{2}^{(11)}");
  r2_12Ratio_vsSub    = createHistogram(createName(bn,"r2_12Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(12,eff)}/R_{2}^{(12)}");
  r2_22Ratio_vsSub    = createHistogram(createName(bn,"r2_22Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(22,eff)}/R_{2}^{(22)}");
  nudyn_12Ratio_vsSub = createHistogram(createName(bn,"nudyn_12Ratio_vsSub"),nBins,0.0,  double(nBins),  "sub-sample","#nu_{dyn}^{(12,eff)}/#nu_{dyn}^{(12)}");

  f1_1RatioOptB_vsSub     = createHistogram(createName(bn,"f1_1RatioOptB_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","f_{1}^{(1,eff)}/f_{1}^{(1))");
  f1_2RatioOptB_vsSub     = createHistogram(createName(bn,"f1_2RatioOptB_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","f_{1}^{(2,eff)}/f_{1}^{(2)}");
  f2_11RatioOptB_vsSub    = createHistogram(createName(bn,"f2_11RatioOptB_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(11,eff)}/f_{2}^{(11)}");
  f2_12RatioOptB_vsSub    = createHistogram(createName(bn,"f2_12RatioOptB_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(12,eff)}/f_{2}^{(12)}");
  f2_22RatioOptB_vsSub    = createHistogram(createName(bn,"f2_22RatioOptB_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","f_{2}^{(22,eff)}/f_{2}^{(22)}");
  r2_11RatioOptB_vsSub    = createHistogram(createName(bn,"r2_11RatioOptB_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(11,eff)}/R_{2}^{(11)}");
  r2_12RatioOptB_vsSub    = createHistogram(createName(bn,"r2_12RatioOptB_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(12,eff)}/R_{2}^{(12)}");
  r2_22RatioOptB_vsSub    = createHistogram(createName(bn,"r2_22RatioOptB_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","R_{2}^{(22,eff)}/R_{2}^{(22)}");
  nudyn_12RatioOptB_vsSub = createHistogram(createName(bn,"nudyn_12RatioOptB_vsSub"),nBins,0.0,  double(nBins),  "sub-sample","#nu_{dyn}^{(12,eff)}/#nu_{dyn}^{(12)}");


  ef1_1_vsSub     = createHistogram(createName(bn,"ef1_1_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{1}^{(1)})");
  ef1_2_vsSub     = createHistogram(createName(bn,"ef1_2_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{1}^{(2)})");
  ef2_11_vsSub    = createHistogram(createName(bn,"ef2_11_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(11)})");
  ef2_12_vsSub    = createHistogram(createName(bn,"ef2_12_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(12)})");
  ef2_22_vsSub    = createHistogram(createName(bn,"ef2_22_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(22)})");
  er2_11_vsSub    = createHistogram(createName(bn,"er2_11_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(11)})");
  er2_12_vsSub    = createHistogram(createName(bn,"er2_12_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(12)})");
  er2_22_vsSub    = createHistogram(createName(bn,"er2_22_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(22)})");
  enudyn_12_vsSub = createHistogram(createName(bn,"enudyn_12_vsSub"),nBins,0.0,  double(nBins),  "sub-sample","#delta(#nu_{dyn}^{(12)})");

  ef1_1Eff_vsSub     = createHistogram(createName(bn,"ef1_1Eff_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{1}^{(1,eff)})");
  ef1_2Eff_vsSub     = createHistogram(createName(bn,"ef1_2Eff_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{1}^{(2,eff)})");
  ef2_11Eff_vsSub    = createHistogram(createName(bn,"ef2_11Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(11,eff)})");
  ef2_12Eff_vsSub    = createHistogram(createName(bn,"ef2_12Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(12,eff)})");
  ef2_22Eff_vsSub    = createHistogram(createName(bn,"ef2_22Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(22,eff)})");
  er2_11Eff_vsSub    = createHistogram(createName(bn,"er2_11Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(11,eff)})");
  er2_12Eff_vsSub    = createHistogram(createName(bn,"er2_12Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(12,eff)})");
  er2_22Eff_vsSub    = createHistogram(createName(bn,"er2_22Eff_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(22,eff)})");
  enudyn_12Eff_vsSub = createHistogram(createName(bn,"enudyn_12Eff_vsSub"),nBins,0.0,  double(nBins),  "sub-sample","#delta(#nu_{dyn}^{(12,eff)})");

  ef1_1Ratio_vsSub      = createHistogram(createName(bn,"ef1_1Ratio_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{1}^{(1,eff)}>/f_{1}^{(1)})");
  ef1_2Ratio_vsSub      = createHistogram(createName(bn,"ef1_2Ratio_vsSub"),   nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{1}^{(2,eff)}>/f_{1}^{(2)})");
  ef2_11Ratio_vsSub     = createHistogram(createName(bn,"ef2_11Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(11,eff)}>/f_{2}^{(11)})");
  ef2_12Ratio_vsSub     = createHistogram(createName(bn,"ef2_12Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(12,eff)}>/f_{2}^{(12)})");
  ef2_22Ratio_vsSub     = createHistogram(createName(bn,"ef2_22Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(f_{2}^{(22,eff)}>/f_{2}^{(22)})");
  er2_11Ratio_vsSub     = createHistogram(createName(bn,"er2_11Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(11,eff)}>/R_{2}^{(11)})");
  er2_12Ratio_vsSub     = createHistogram(createName(bn,"er2_12Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(12,eff)}>/R_{2}^{(12)})");
  er2_22Ratio_vsSub     = createHistogram(createName(bn,"er2_22Ratio_vsSub"),  nBins,0.0,  double(nBins),  "sub-sample","#delta(R_{2}^{(22,eff)}>/R_{2}^{(22)})");
  enudyn_12Ratio_vsSub  = createHistogram(createName(bn,"enudyn_12Ratio_vsSub"),nBins,0.0,  double(nBins),  "sub-sample","#delta(#nu_{dyn}^{(12,eff)}/#nu_{dyn}^{(12)}");

}

void StatStudyHistograms::HistogramsImport(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  String bn = getParentTaskName();
  f1_1     = loadH1(inputFile,  createName(bn,"f1_1"));
  f1_2     = loadH1(inputFile,  createName(bn,"f1_2"));
  f2_11    = loadH1(inputFile,  createName(bn,"f2_11"));
  f2_12    = loadH1(inputFile,  createName(bn,"f2_12"));
  f2_22    = loadH1(inputFile,  createName(bn,"f2_22"));
  r2_11    = loadH1(inputFile,  createName(bn,"r2_11"));
  r2_12    = loadH1(inputFile,  createName(bn,"r2_12"));
  r2_22    = loadH1(inputFile,  createName(bn,"r2_22"));
  nudyn_12 = loadH1(inputFile,  createName(bn,"nudyn_12"));
  f1_1Eff     = loadH1(inputFile,  createName(bn,"f1_1Eff"));
  f1_2Eff     = loadH1(inputFile,  createName(bn,"f1_2Eff"));
  f2_11Eff    = loadH1(inputFile,  createName(bn,"f2_11Eff"));
  f2_12Eff    = loadH1(inputFile,  createName(bn,"f2_12Eff"));
  f2_22Eff    = loadH1(inputFile,  createName(bn,"f2_22Eff"));
  r2_11Eff    = loadH1(inputFile,  createName(bn,"r2_11Eff"));
  r2_12Eff    = loadH1(inputFile,  createName(bn,"r2_12Eff"));
  r2_22Eff    = loadH1(inputFile,  createName(bn,"r2_22Eff"));
  nudyn_12Eff = loadH1(inputFile,  createName(bn,"nudyn_12Eff"));
  f1_1Ratio     = loadH1(inputFile,  createName(bn,"f1_1Ratio"));
  f1_2Ratio     = loadH1(inputFile,  createName(bn,"f1_2Ratio"));
  f2_11Ratio    = loadH1(inputFile,  createName(bn,"f2_11Ratio"));
  f2_12Ratio    = loadH1(inputFile,  createName(bn,"f2_12Ratio"));
  f2_22Ratio    = loadH1(inputFile,  createName(bn,"f2_22Ratio"));
  r2_11Ratio    = loadH1(inputFile,  createName(bn,"r2_11Ratio"));
  r2_12Ratio    = loadH1(inputFile,  createName(bn,"r2_12Ratio")0;
  r2_22Ratio    = loadH1(inputFile,  createName(bn,"r2_22Ratio"));
  nudyn_12Ratio = loadH1(inputFile,  createName(bn,"nudyn_12Ratio"));
  f1_1_vsSub     = loadProfile(inputFile,  createName(bn,"f1_1_vsSub"));
  f1_2_vsSub     = loadProfile(inputFile,  createName(bn,"f1_2_vsSub"));
  f2_11_vsSub    = loadProfile(inputFile,  createName(bn,"f2_11_vsSub"));
  f2_12_vsSub    = loadProfile(inputFile,  createName(bn,"f2_12_vsSub"));
  f2_22_vsSub    = loadProfile(inputFile,  createName(bn,"f2_22_vsSub"));
  r2_11_vsSub    = loadH1(inputFile,  createName(bn,"r2_11_vsSub"));
  r2_12_vsSub    = loadH1(inputFile,  createName(bn,"r2_12_vsSub"));
  r2_22_vsSub    = loadH1(inputFile,  createName(bn,"r2_22_vsSub"));
  nudyn_12_vsSub = loadH1(inputFile,  createName(bn,"nudyn_12_vsSub"));

  f1_1Eff_vsSub     = loadProfile(inputFile,  createName(bn,"f1_1Eff_vsSub"));
  f1_2Eff_vsSub     = loadProfile(inputFile,  createName(bn,"f1_2Eff_vsSub"));
  f2_11Eff_vsSub    = loadProfile(inputFile,  createName(bn,"f2_11Eff_vsSub"));
  f2_12Eff_vsSub    = loadProfile(inputFile,  createName(bn,"f2_12Eff_vsSub"));
  f2_22Eff_vsSub    = loadProfile(inputFile,  createName(bn,"f2_22Eff_vsSub"));
  r2_11Eff_vsSub    = loadH1(inputFile,  createName(bn,"r2_11Eff_vsSub"));
  r2_12Eff_vsSub    = loadH1(inputFile,  createName(bn,"r2_12Eff_vsSub"));
  r2_22Eff_vsSub    = loadH1(inputFile,  createName(bn,"r2_22Eff_vsSub"));
  nudyn_12Eff_vsSub = loadH1(inputFile,  createName(bn,"nudyn_12Eff_vsSub"));

  f1_1Ratio_vsSub     = loadH1(inputFile,  createName(bn,"f1_1Ratio_vsSub"));
  f1_2Ratio_vsSub     = loadH1(inputFile,  createName(bn,"f1_2Ratio_vsSub"));
  f2_11Ratio_vsSub    = loadH1(inputFile,  createName(bn,"f2_11Ratio_vsSub"));
  f2_12Ratio_vsSub    = loadH1(inputFile,  createName(bn,"f2_12Ratio_vsSub"));
  f2_22Ratio_vsSub    = loadH1(inputFile,  createName(bn,"f2_22Ratio_vsSub"));
  r2_11Ratio_vsSub    = loadH1(inputFile,  createName(bn,"r2_11Ratio_vsSub"));
  r2_12Ratio_vsSub    = loadH1(inputFile,  createName(bn,"r2_12Ratio_vsSub"));
  r2_22Ratio_vsSub    = loadH1(inputFile,  createName(bn,"r2_22Ratio_vsSub"));
  nudyn_12Ratio_vsSub = loadH1(inputFile,  createName(bn,"nudyn_12Ratio_vsSub"));

  f1_1RatioOptB_vsSub     = loadH1(inputFile,  createName(bn,"f1_1RatioOptB_vsSub"));
  f1_2RatioOptB_vsSub     = loadH1(inputFile,  createName(bn,"f1_2RatioOptB_vsSub"));
  f2_11RatioOptB_vsSub    = loadH1(inputFile,  createName(bn,"f2_11RatioOptB_vsSub"));
  f2_12RatioOptB_vsSub    = loadH1(inputFile,  createName(bn,"f2_12RatioOptB_vsSub"));
  f2_22RatioOptB_vsSub    = loadH1(inputFile,  createName(bn,"f2_22RatioOptB_vsSub"));
  r2_11RatioOptB_vsSub    = loadH1(inputFile,  createName(bn,"r2_11RatioOptB_vsSub"));
  r2_12RatioOptB_vsSub    = loadH1(inputFile,  createName(bn,"r2_12RatioOptB_vsSub"));
  r2_22RatioOptB_vsSub    = loadH1(inputFile,  createName(bn,"r2_22RatioOptB_vsSub"));
  nudyn_12RatioOptB_vsSub = loadH1(inputFile,  createName(bn,"nudyn_12RatioOptB_vsSub"));


  ef1_1_vsSub     = loadH1(inputFile,  createName(bn,"ef1_1_vsSub"));
  ef1_2_vsSub     = loadH1(inputFile,  createName(bn,"ef1_2_vsSub"));
  ef2_11_vsSub    = loadH1(inputFile,  createName(bn,"ef2_11_vsSub"));
  ef2_12_vsSub    = loadH1(inputFile,  createName(bn,"ef2_12_vsSub"));
  ef2_22_vsSub    = loadH1(inputFile,  createName(bn,"ef2_22_vsSub"));
  er2_11_vsSub    = loadH1(inputFile,  createName(bn,"er2_11_vsSub"));
  er2_12_vsSub    = loadH1(inputFile,  createName(bn,"er2_12_vsSub"));
  er2_22_vsSub    = loadH1(inputFile,  createName(bn,"er2_22_vsSub"));
  enudyn_12_vsSub = loadH1(inputFile,  createName(bn,"enudyn_12_vsSub"));

  ef1_1Eff_vsSub     = loadH1(inputFile,  createName(bn,"ef1_1Eff_vsSub"));
  ef1_2Eff_vsSub     = loadH1(inputFile,  createName(bn,"ef1_2Eff_vsSub"));
  ef2_11Eff_vsSub    = loadH1(inputFile,  createName(bn,"ef2_11Eff_vsSub"));
  ef2_12Eff_vsSub    = loadH1(inputFile,  createName(bn,"ef2_12Eff_vsSub"));
  ef2_22Eff_vsSub    = loadH1(inputFile,  createName(bn,"ef2_22Eff_vsSub"));
  er2_11Eff_vsSub    = loadH1(inputFile,  createName(bn,"er2_11Eff_vsSub"));
  er2_12Eff_vsSub    = loadH1(inputFile,  createName(bn,"er2_12Eff_vsSub"));
  er2_22Eff_vsSub    = loadH1(inputFile,  createName(bn,"er2_22Eff_vsSub"));
  enudyn_12Eff_vsSub = loadH1(inputFile,  createName(bn,"enudyn_12Eff_vsSub"));

  ef1_1Ratio_vsSub      = loadH1(inputFile,  createName(bn,"ef1_1Ratio_vsSub"));
  ef1_2Ratio_vsSub      = loadH1(inputFile,  createName(bn,"ef1_2Ratio_vsSub"));
  ef2_11Ratio_vsSub     = loadH1(inputFile,  createName(bn,"ef2_11Ratio_vsSub"));
  ef2_12Ratio_vsSub     = loadH1(inputFile,  createName(bn,"ef2_12Ratio_vsSub"));
  ef2_22Ratio_vsSub     = loadH1(inputFile,  createName(bn,"ef2_22Ratio_vsSub"));
  er2_11Ratio_vsSub     = loadH1(inputFile,  createName(bn,"er2_11Ratio_vsSub"));
  er2_12Ratio_vsSub     = loadH1(inputFile,  createName(bn,"er2_12Ratio_vsSub"));
  er2_22Ratio_vsSub     = loadH1(inputFile,  createName(bn,"er2_22Ratio_vsSub"));
  enudyn_12Ratio_vsSub  = loadH1(inputFile,  createName(bn,"enudyn_12Ratio_vsSub"));
  if (reportEnd(__FUNCTION__))
    ;
}

void StatStudyHistograms::exportHistograms(TFile & outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  outputFile->cd();

  f1_1->Write();
  f1_2->Write();
  f2_11->Write();
  f2_12->Write();
  f2_22->Write();
  r2_11->Write();
  r2_12->Write();
  r2_22->Write();
  nudyn_12->Write();

  f1_1Eff->Write();
  f1_2Eff->Write();
  f2_11Eff->Write();
  f2_12Eff->Write();
  f2_22Eff->Write();
  r2_11Eff->Write();
  r2_12Eff->Write();
  r2_22Eff->Write();
  nudyn_12Eff->Write();

  f1_1Ratio->Write();
  f1_2Ratio->Write();
  f2_11Ratio->Write();
  f2_12Ratio->Write();
  f2_22Ratio->Write();
  r2_11Ratio->Write();
  r2_12Ratio->Write();
  r2_22Ratio->Write();
  nudyn_12Ratio->Write();

  f1_1_vsSub->Write();
  f1_2_vsSub->Write();
  f2_11_vsSub->Write();
  f2_12_vsSub->Write();
  f2_22_vsSub->Write();
  r2_11_vsSub->Write();
  r2_12_vsSub->Write();
  r2_22_vsSub->Write();
  nudyn_12_vsSub->Write();

  f1_1Eff_vsSub->Write();
  f1_2Eff_vsSub->Write();
  f2_11Eff_vsSub->Write();
  f2_12Eff_vsSub->Write();
  f2_22Eff_vsSub->Write();
  r2_11Eff_vsSub->Write();
  r2_12Eff_vsSub->Write();
  r2_22Eff_vsSub->Write();
  nudyn_12Eff_vsSub->Write();

  f1_1Ratio_vsSub->Write();
   f1_2Ratio_vsSub->Write();
   f2_11Ratio_vsSub->Write();
   f2_12Ratio_vsSub->Write();
   f2_22Ratio_vsSub->Write();
   r2_11Ratio_vsSub->Write();
   r2_12Ratio_vsSub->Write();
   r2_22Ratio_vsSub->Write();
   nudyn_12Ratio_vsSub->Write();

  f1_1RatioOptB_vsSub->Write();
  f1_2RatioOptB_vsSub->Write();
  f2_11RatioOptB_vsSub->Write();
  f2_12RatioOptB_vsSub->Write();
  f2_22RatioOptB_vsSub->Write();
  r2_11RatioOptB_vsSub->Write();
  r2_12RatioOptB_vsSub->Write();
  r2_22RatioOptB_vsSub->Write();
  nudyn_12RatioOptB_vsSub->Write();

  ef1_1_vsSub->Write();
  ef1_2_vsSub->Write();
  ef2_11_vsSub->Write();
  ef2_12_vsSub->Write();
  ef2_22_vsSub->Write();
  er2_11_vsSub->Write();
  er2_12_vsSub->Write();
  er2_22_vsSub->Write();
  enudyn_12_vsSub->Write();

  ef1_1Eff_vsSub->Write();
  ef1_2Eff_vsSub->Write();
  ef2_11Eff_vsSub->Write();
  ef2_12Eff_vsSub->Write();
  ef2_22Eff_vsSub->Write();
  er2_11Eff_vsSub->Write();
  er2_12Eff_vsSub->Write();
  er2_22Eff_vsSub->Write();
  enudyn_12Eff_vsSub->Write();

  ef1_1Ratio_vsSub->Write();
  ef1_2Ratio_vsSub->Write();
  ef2_11Ratio_vsSub->Write();
  ef2_12Ratio_vsSub->Write();
  ef2_22Ratio_vsSub->Write();
  er2_11Ratio_vsSub->Write();
  er2_12Ratio_vsSub->Write();
  er2_22Ratio_vsSub->Write();
  enudyn_12Ratio_vsSub->Write();
  if (reportEnd(__FUNCTION__))
    ;
}

void StatStudyHistograms::fill(double sub, double nPlus, double nMinus, double nPlusEff,  double nMinusEff)
{
  double nPlusPlus       = nPlus*(nPlus-1);
  double nMinusMinus    = nMinus*(nMinus-1);
  double nPlusMinus     = nPlus*nMinus;
  double nPlusPlusEff   = nPlusEff*(nPlusEff-1);
  double nMinusMinusEff = nMinusEff*(nMinusEff-1);
  double nPlusMinusEff  = nPlusEff*nMinusEff;

  f1_1_vsSub->Fill(sub, nPlus);
  f1_2_vsSub->Fill(sub, nMinus);
  f2_11_vsSub->Fill(sub, nPlusPlus);
  f2_12_vsSub->Fill(sub, nPlusMinus);
  f2_22_vsSub->Fill(sub, nMinusMinus);
  f1_1Eff_vsSub->Fill(sub, nPlusEff);
  f1_2Eff_vsSub->Fill(sub, nMinusEff);
  f2_11Eff_vsSub->Fill(sub, nPlusPlusEff);
  f2_12Eff_vsSub->Fill(sub, nPlusMinusEff);
  f2_22Eff_vsSub->Fill(sub, nMinusMinusEff);

  f1_1->Fill(nPlus);
  f1_2->Fill(nMinus);
  f2_11->Fill(nPlusPlus);
  f2_12->Fill(nPlusMinus);
  f2_22->Fill(nMinusMinus);

  f1_1Eff->Fill(nPlusEff);
  f1_2Eff->Fill(nMinusEff);
  f2_11Eff->Fill(nPlusPlusEff);
  f2_12Eff->Fill(nPlusMinusEff);
  f2_22Eff->Fill(nMinusMinusEff);
}

void StatStudyHistograms::computeDerivedHistograms()
{
  calculateR2(f1_1_vsSub,     f2_11_vsSub,  r2_11_vsSub);
  calculateR2(f1_1_vsSub,     f1_2_vsSub,   f2_12_vsSub, r2_12_vsSub);
  calculateR2(f1_2_vsSub,     f2_22_vsSub,  r2_22_vsSub);
  calculateNuDyn(r2_11_vsSub, r2_12_vsSub,  r2_22_vsSub,   nudyn_12_vsSub);

  calculateR2(f1_1Eff_vsSub,     f2_11Eff_vsSub,  r2_11Eff_vsSub);
  calculateR2(f1_1Eff_vsSub,     f1_2Eff_vsSub,   f2_12Eff_vsSub, r2_12Eff_vsSub);
  calculateR2(f1_2Eff_vsSub,     f2_22Eff_vsSub,  r2_22Eff_vsSub);
  calculateNuDyn(r2_11Eff_vsSub, r2_12Eff_vsSub,  r2_22Eff_vsSub,  nudyn_12Eff_vsSub);

  calculateRatio(f1_1Eff_vsSub, f1_1_vsSub,  f1_1Ratio_vsSub,   f1_1RatioOptB_vsSub);
  calculateRatio(f1_2Eff_vsSub, f1_2_vsSub,  f1_2Ratio_vsSub,   f1_2RatioOptB_vsSub);
  calculateRatio(f2_11Eff_vsSub,f2_11_vsSub, f2_11Ratio_vsSub,  f2_11RatioOptB_vsSub);
  calculateRatio(f2_12Eff_vsSub,f2_12_vsSub, f2_12Ratio_vsSub,  f2_12RatioOptB_vsSub);
  calculateRatio(f2_22Eff_vsSub,f2_22_vsSub, f2_22Ratio_vsSub,  f2_22RatioOptB_vsSub);
  calculateRatio(r2_11Eff_vsSub,r2_11_vsSub, r2_11Ratio_vsSub,  r2_11RatioOptB_vsSub);
  calculateRatio(r2_12Eff_vsSub,r2_12_vsSub, r2_12Ratio_vsSub,  r2_12RatioOptB_vsSub);
  calculateRatio(r2_22Eff_vsSub,r2_22_vsSub, r2_12Ratio_vsSub,  r2_12RatioOptB_vsSub);
  calculateRatio(nudyn_12Eff_vsSub, nudyn_12_vsSub, nudyn_12Ratio_vsSub, nudyn_12RatioOptB_vsSub);

  fillHist(r2_11_vsSub, r2_11, nSubSamples);
  fillHist(r2_12_vsSub, r2_12, nSubSamples);
  fillHist(r2_22_vsSub, r2_22, nSubSamples);
  fillHist(nudyn_12_vsSub, nudyn_12, nSubSamples);

  fillHist(r2_11Eff_vsSub, r2_11Eff, nSubSamples);
  fillHist(r2_12Eff_vsSub, r2_12Eff, nSubSamples);
  fillHist(r2_22Eff_vsSub, r2_22Eff, nSubSamples);
  fillHist(nudyn_12Eff_vsSub, nudyn_12Eff, nSubSamples);

  fillHist(f1_1Ratio_vsSub,   f1_1Ratio, nSubSamples);
  fillHist(f1_2Ratio_vsSub,   f1_2Ratio, nSubSamples);
  fillHist(f2_11Ratio_vsSub,  f2_11Ratio, nSubSamples);
  fillHist(f2_12Ratio_vsSub,  f2_12Ratio, nSubSamples);
  fillHist(f2_22Ratio_vsSub,  f2_22Ratio, nSubSamples);
  fillHist(r2_11Ratio_vsSub,  r2_11Ratio, nSubSamples);
  fillHist(r2_12Ratio_vsSub,  r2_12Ratio, nSubSamples);
  fillHist(r2_22Ratio_vsSub,  r2_22Ratio, nSubSamples);
  fillHist(nudyn_12Ratio_vsSub, nudyn_12Ratio, nSubSamples);

  fillMeanErrStd(f1_1_vsSub, ef1_1_vsSub, nSubSamples);
  fillMeanErrStd(f1_2_vsSub, ef1_2_vsSub, nSubSamples);
  fillMeanErrStd(f2_11_vsSub, ef2_11_vsSub, nSubSamples);
  fillMeanErrStd(f2_22_vsSub, ef2_22_vsSub, nSubSamples);
  fillMeanErrStd(f2_12_vsSub, ef2_12_vsSub, nSubSamples);
  fillMeanErrStd(r2_11_vsSub, er2_11_vsSub, nSubSamples);
  fillMeanErrStd(r2_22_vsSub, er2_22_vsSub, nSubSamples);
  fillMeanErrStd(r2_12_vsSub, er2_12_vsSub, nSubSamples);
  fillMeanErrStd(nudyn_12_vsSub, enudyn_12_vsSub, nSubSamples);

  fillMeanErrStd(f1_1Eff_vsSub, ef1_1Eff_vsSub, nSubSamples);
  fillMeanErrStd(f1_2Eff_vsSub, ef1_2Eff_vsSub, nSubSamples);
  fillMeanErrStd(f2_11Eff_vsSub, ef2_11Eff_vsSub, nSubSamples);
  fillMeanErrStd(f2_22Eff_vsSub, ef2_22Eff_vsSub, nSubSamples);
  fillMeanErrStd(f2_12Eff_vsSub, ef2_12Eff_vsSub, nSubSamples);
  fillMeanErrStd(r2_11Eff_vsSub, er2_11Eff_vsSub, nSubSamples);
  fillMeanErrStd(r2_22Eff_vsSub, er2_22Eff_vsSub, nSubSamples);
  fillMeanErrStd(r2_12Eff_vsSub, er2_12Eff_vsSub, nSubSamples);
  fillMeanErrStd(nudyn_12Eff_vsSub, enudyn_12Eff_vsSub, nSubSamples);

  fillMeanErrStd(f1_1Ratio_vsSub, ef1_1Ratio_vsSub, nSubSamples);
  fillMeanErrStd(f1_2Ratio_vsSub, ef1_2Ratio_vsSub, nSubSamples);
  fillMeanErrStd(f2_11Ratio_vsSub, ef2_11Ratio_vsSub, nSubSamples);
  fillMeanErrStd(f2_22Ratio_vsSub, ef2_22Ratio_vsSub, nSubSamples);
  fillMeanErrStd(f2_12Ratio_vsSub, ef2_12Ratio_vsSub, nSubSamples);
  fillMeanErrStd(r2_11Ratio_vsSub, er2_11Ratio_vsSub, nSubSamples);
  fillMeanErrStd(r2_22Ratio_vsSub, er2_22Ratio_vsSub, nSubSamples);
  fillMeanErrStd(r2_12Ratio_vsSub, er2_12Ratio_vsSub, nSubSamples);
  fillMeanErrStd(nudyn_12Ratio_vsSub, enudyn_12Ratio_vsSub, nSubSamples);
}

void StatStudyHistograms::HistogramsScale(double scalingFactor1,double scalingFactor2)
{
  f1_1->Scale(scalingFactor1);
  f1_2->Scale(scalingFactor1);
  f2_11->Scale(scalingFactor1);
  f2_12->Scale(scalingFactor1);
  f2_22->Scale(scalingFactor1);
  r2_11->Scale(scalingFactor2);
  r2_12->Scale(scalingFactor2);
  r2_22->Scale(scalingFactor2);
  nudyn_12->Scale(scalingFactor2);

  f1_1Eff->Scale(scalingFactor1);
  f1_2Eff->Scale(scalingFactor1);
  f2_11Eff->Scale(scalingFactor1);
  f2_12Eff->Scale(scalingFactor1);
  f2_22Eff->Scale(scalingFactor1);
  r2_11Eff->Scale(scalingFactor2);
  r2_12Eff->Scale(scalingFactor2);
  r2_22Eff->Scale(scalingFactor2);
  nudyn_12Eff->Scale(scalingFactor2);

  f1_1Ratio->Scale(scalingFactor2);
  f1_2Ratio->Scale(scalingFactor2);
  f2_11Ratio->Scale(scalingFactor2);
  f2_12Ratio->Scale(scalingFactor2);
  f2_22Ratio->Scale(scalingFactor2);
  r2_11Ratio->Scale(scalingFactor2);
  r2_12Ratio->Scale(scalingFactor2);
  r2_22Ratio->Scale(scalingFactor2);
  nudyn_12Ratio->Scale(scalingFactor2);
}


// r = f1/f2
 void StatStudyHistograms::calculateRatio(double f1,double ef1,double f2,double ef2,double & r,double & er)
 {
   double ref1, ref2, rer;
   if (f2!=0)
     {
     ref1   = ef1/f1;
     ref2   = ef2/f2;
     rer    = ref1*ref1 + ref2*ref2;
     r = f1/f2;
     er = r*sqrt(rer);
     }
   else
     {
     r = 0.0;
     er = 0.0;
     }
 }


 // calculation carried out assuming no correlation between the different factors..
 // r = f2/f1/f1
 void StatStudyHistograms::calculateR2(double f1,double ef1,double f2,double ef2,double & r2,double & er2)
 {
   // calculate ratio and error for r2 = f2/f1/f1
   double ref1, ref2, rer2;
   if (f1>0)
     {
     ref1 = ef1/f1;
     ref2   = ef2/f2;
     rer2   = 4*ref1*ref1 + ref2*ref2;
     r2 = f2/(f1*f1) - 1.0;
     er2 = r2*sqrt(rer2);
     }
   else
     {
     r2 = 0.0;
     er2 = 0.0;
     }
 }

 // calculation carried out assuming no correlation between the different factors..
 void StatStudyHistograms::calculateR2(double f1_1,double ef1_1,double f1_2,double ef1_2,double f2,double ef2,double & r2,double & er2)
 {
   // calculate ratio and error for r2 = f2/f1_1/f1_2

   double ref1_1, ref1_2, ref2, rer2;
   if (f1_1>0 & f1_2>0)
     {
     ref1_1 = ef1_1/f1_1;
     ref1_2 = ef1_2/f1_2;
     ref2   = ef2/f2;
     rer2   = ref1_1*ref1_1 + ref1_2*ref1_2 + ref2*ref2;
     r2 = f2/(f1_1*f1_2) - 1.0;
     er2 = r2*sqrt(rer2);
     }
   else
     {
     r2 = 0.0;
     er2 = 0.0;
     }
 }

 void StatStudyHistograms::calculateNudyn(double r2_11,double er2_11,double r2_12,double er2_12,double r2_22,double er2_22,double & nudyn,double & enudyn)
 {
   nudyn = r2_11 + r2_22 - 2.0*r2_12;
   enudyn = sqrt(er2_11*er2_11 + er2_22*er2_22 + 4.0*er2_12*er2_12);
 }


 void StatStudyHistograms::calculateR2(TH1* h1, TH1* h2, TH1* hr)
 {
   double v1, ev1, v2, ev2, r, er;
   int nBins = h1->GetNbinsX();
   for (int iBin = 1; iBin <=nBins; iBin++)
     {
     v1   = h1->GetBinContent(iBin);
     ev1  = h1->GetBinError(iBin);
     v2   = h2->GetBinContent(iBin);
     ev2  = h2->GetBinError(iBin);
     calculateR2(v1,ev1,v2,ev2,r,er);
     hr->SetBinContent(iBin, r);
     hr->SetBinError(iBin,   er);
     }
 }

 void StatStudyHistograms::calculateR2(TH1* h1_1, TH1* h1_2, TH1* h2_12, TH1* hr)
 {
   double v1_1, ev1_1, v1_2, ev1_2, v2, ev2, r, er;
   int nBins = h1_1->GetNbinsX();
   for (int iBin = 1; iBin <=nBins; iBin++)
     {
     v1_1   = h1_1->GetBinContent(iBin);
     ev1_1  = h1_1->GetBinError(iBin);
     v1_2   = h1_2->GetBinContent(iBin);
     ev1_2  = h1_2->GetBinError(iBin);
     v2     = h2_12->GetBinContent(iBin);
     ev2    = h2_12->GetBinError(iBin);
     calculateR2(v1_1,ev1_1,v1_2,ev1_2,v2,ev2,r,er);
     hr->SetBinContent(iBin, r);
     hr->SetBinError(iBin,   er);
     }
 }

 void StatStudyHistograms::calculateNuDyn(TH1* r2_11, TH1* r2_12, TH1* r2_22, TH1* nudyn)
 {
   double v2_11, ev2_11, v2_12, ev2_12, v2_22, ev2_22, nu, enu;
   int nBins = r2_11->GetNbinsX();
   for (int iBin = 1; iBin <=nBins; iBin++)
     {
     v2_11   = r2_11->GetBinContent(iBin);
     ev2_11  = r2_11->GetBinError(iBin);
     v2_12   = r2_12->GetBinContent(iBin);
     ev2_12  = r2_12->GetBinError(iBin);
     v2_22   = r2_22->GetBinContent(iBin);
     ev2_22  = r2_22->GetBinError(iBin);
     calculateNudyn(v2_11,ev2_11,v2_12,ev2_12,v2_22,ev2_22,nu,enu);
     nudyn->SetBinContent(iBin, nu);
     nudyn->SetBinError(iBin,   enu);
     }
 }
 // Calculate r = h2/h1
 void StatStudyHistograms::calculateRatio(TH1 * h1, TH1 * h2, TH1 * ratio, TH1 * ratioOptB)
 {
 ratio->Divide(h1, h2, 1.0, 1.0, "");
 if (ratioOptB) ratioOptB->Divide(h1, h2, 1.0, 1.0, "B");

//   double v1, ev1, v2, ev2, r, er;
//   int nBins = h1->GetNbinsX();
//   for (int iBin = 1; iBin <=nBins; iBin++)
//     {
//     v1   = h1->GetBinContent(iBin);
//     ev1  = h1->GetBinError(iBin);
//     v2   = h2->GetBinContent(iBin);
//     ev2  = h2->GetBinError(iBin);
//     calculateRatio(v1,ev1,v2,ev2,r,er);
//
//     //cout << ratio->GetName() << " iBin:" << iBin << "  r:" << r << "   er:" << er << endl;
//     ratio->SetBinContent(iBin, r);
//     ratio->SetBinError  (iBin, er);
//     }
 }

 void StatStudyHistograms::calculateMeanAndStd(TH1 * h, int nBins, double &mean, double &typicalError, double &eMean, double &std)
 {
   double m1  = 0;
   double e   = 0;
   double em1 = 0;
   double v, ev;
   // calculate mean value of the nBins
   for (int iBin=1; iBin<=nBins; iBin++)
     {
     v  = h->GetBinContent(iBin);
     ev = h->GetBinError(iBin);
     m1 += v;
     e  += ev;
     em1 += ev*ev;
     }
   mean = m1/double(nBins);
   typicalError = e/double(nBins);
   eMean = sqrt(em1)/double(nBins);

   double m2  = 0;
   for (int iBin=1; iBin<=nBins; iBin++)
     {
     v  = h->GetBinContent(iBin)-mean;
     m2 += v*v;
     }
   m2 = m2/double(nBins-1);
   std = sqrt(m2);
 }


 void StatStudyHistograms::fillMeanErrStd(TH1 * h, TH1 * eh,int n)
 {
   double mean, typicalError, errorOnMean, std;

   for (int i=2; i<=n; i++)
     {
     calculateMeanAndStd(h, i, mean,  typicalError,  errorOnMean, std);
     eh->SetBinContent(i,std/fabs(mean)); eh->SetBinError(i,0);
     }

   cout << "         histo:" << h->GetName() << endl;
   cout << "          mean:" << mean << endl;
   cout << "  typicalError:" << typicalError << endl;
   cout << "   errorOnMean:" << errorOnMean << endl;
   cout << "           std:" << std << endl;
   h->SetBinContent(n+5,mean); h->SetBinError(n+5,typicalError);
   h->SetBinContent(n+10,mean); h->SetBinError(n+10,errorOnMean);
   h->SetBinContent(n+15,mean); h->SetBinError(n+15,std);
 }

 void StatStudyHistograms::fillHist(TH1 * source, TH1 * target, int n)
 {
   for (int i=1; i<=n; i++)
     {
     double v  = source->GetBinContent(i);
     target->Fill(v);
     }
 }
