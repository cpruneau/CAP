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
#include "CollisionGeometryHistograms.hpp"
#include <vector>
using std::vector;

ClassImp(CollisionGeometryHistograms);

CollisionGeometryHistograms::CollisionGeometryHistograms(const TString       & _name,
                                                         const Configuration & _configuration,
                                                         LogLevel  _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
h_nProcessedVsB(nullptr),
h_nAcceptedVsB(nullptr),
h_nPartVsB(nullptr),
h_nBinaryVsB(nullptr),
h_nPartVsB_Prof(nullptr),
h_nPartSqVsB_Prof(nullptr),
h_nBinaryVsB_Prof(nullptr),
h_nBinarySqVsB_Prof(nullptr),
h_nPart(nullptr),
h_nBinaryVsNPart_Prof(nullptr),
h_nBinarySqVsNPart_Prof(nullptr),
h_bVsNPart_Prof(nullptr),
h_bSqVsNPart_Prof(nullptr),
h_nBinary(nullptr),
h_nPartVsNBinary_Prof(nullptr),
h_nPartSqVsNBinary_Prof(nullptr),
h_bVsNBinary_Prof(nullptr),
h_bSqVsNBinary_Prof(nullptr),
h_crossSection(nullptr),
h_nPartVsXsect_Prof(nullptr),
h_nPartSqVsXsect_Prof(nullptr),
h_nBinaryVsXsect_Prof(nullptr),
h_nBinarySqVsXsect_Prof(nullptr),
h_bVsXsect_Prof(nullptr),
h_bSqVsXsect_Prof(nullptr),
h_xyDistInteractions(nullptr),
h_xyDistNucleons(nullptr),
h_nPartRmsVsB(nullptr),
h_nPartOmegaVsB(nullptr),
h_nPartR2VsB(nullptr),
h_nBinaryRmsVsB(nullptr),
h_nBinaryOmegaVsB(nullptr),
h_nBinaryR2VsB(nullptr),
h_nBinaryRmsVsNPart(nullptr),
h_nBinaryOmegaVsNPart(nullptr),
h_nBinaryR2VsNPart(nullptr),
h_bRmsVsNPart(nullptr),
h_bOmegaVsNPart(nullptr),
h_bR2VsNPart(nullptr),

h_nPartRmsVsNBinary(nullptr),
h_nPartOmegaVsNBinary(nullptr),
h_nPartR2VsNBinary(nullptr),
h_bRmsVsNBinary(nullptr),
h_bOmegaVsNBinary(nullptr),
h_bR2VsNBinary(nullptr),

h_nPartRmsVsXsect(nullptr),
h_nPartOmegaVsXsect(nullptr),
h_nPartR2VsXsect(nullptr),

h_nBinaryRmsVsXsect(nullptr),
h_nBinaryOmegaVsXsect(nullptr),
h_nBinaryR2VsXsect(nullptr),
h_bRmsVsXsect(nullptr),
h_bOmegaVsXsect(nullptr),
h_bR2VsXsect(nullptr)
{
  // no ops
}



void CollisionGeometryHistograms::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;

  Configuration & configuration = getConfiguration();
  int nBins_b        = configuration.getValueInt("nBins_b");
  double min_b       = configuration.getValueDouble("min_b");
  double max_b       = configuration.getValueDouble("max_b");
  int nBins_nPart    = configuration.getValueInt("nBins_nPart");
  double min_nPart   = configuration.getValueDouble("min_nPart");
  double max_nPart   = configuration.getValueDouble("max_nPart");
  int nBins_nBinary  = configuration.getValueInt("nBins_nBinary");
  double min_nBinary = configuration.getValueDouble("min_nBinary");
  double max_nBinary = configuration.getValueDouble("max_nBinary");
  TString impact = "b (fm)";
  TString counts = "Counts";
  TString nParticipants  = "N_{Part}";
  TString nBin   = "N_{Bin}";

  TString bn = getHistoBaseName();
  h_nProcessedVsB     = createHistogram(makeName(bn,"nProcessedVsB"),    nBins_b, min_b,  max_b,    impact,   counts);
  h_nAcceptedVsB      = createHistogram(makeName(bn,"nAcceptedVsB"),     nBins_b, min_b,  max_b,    impact,   counts);
  h_nPartVsB          = createHistogram(makeName(bn,"nPartVsB"),         nBins_b, min_b,  max_b,    nBins_nPart,   min_nPart,   max_nPart,   impact, nParticipants, counts);
  h_nBinaryVsB        = createHistogram(makeName(bn,"nBinaryVsB"),       nBins_b, min_b,  max_b,    nBins_nBinary, min_nBinary, max_nBinary, impact, nBin,  counts);
  h_nPartVsB_Prof     = createProfile(makeName(bn,"nPartVsB_Prof"),      nBins_b, min_b,  max_b,    impact,   "<N_{Part}>");
  h_nPartSqVsB_Prof   = createProfile(makeName(bn,"nPartSqVsB_Prof"),    nBins_b, min_b,  max_b,    impact,   "<N_{Part}^{2}>");
  h_nBinaryVsB_Prof   = createProfile(makeName(bn,"nBinaryVsB_Prof"),    nBins_b, min_b,  max_b,    impact,   "<N_{Bin}>");
  h_nBinarySqVsB_Prof = createProfile(makeName(bn,"nBinarySqVsB_Prof"),  nBins_b, min_b,  max_b,    impact,   "<N_{Bin}^{2}>");

  h_nPart                 = createHistogram(makeName(bn,"nParticipants"),         nBins_nPart,  min_nPart,   max_nPart,   nParticipants, counts);
  h_nBinaryVsNPart_Prof   = createProfile(makeName(bn,"nBinaryVsNPart_Prof"),  nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "<N_{Bin}>");
  h_nBinarySqVsNPart_Prof = createProfile(makeName(bn,"nBinarySqVsNPart_Prof"),nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "<N_{Bin}^{2}>");
  h_bVsNPart_Prof         = createProfile(makeName(bn,"bVsNPart_Prof"),   nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "<b>");
  h_bSqVsNPart_Prof       = createProfile(makeName(bn,"bSqVsNPart_Proff"),nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "<b^{2}>");

  h_nBinary                = createHistogram(makeName(bn,"nBinary"),       nBins_nBinary,min_nBinary, max_nBinary, nBin,  counts);
  h_nPartVsNBinary_Prof    = createProfile(makeName(bn,"nPartVsNBinary_Prof"),  nBins_nBinary,min_nBinary, max_nBinary, nBin,  "<N_{Part}>");
  h_nPartSqVsNBinary_Prof  = createProfile(makeName(bn,"nPartSqVsNBinary_Prof"),nBins_nBinary,min_nBinary, max_nBinary, nBin,  "<N_{Part}^{2}>");
  h_bVsNBinary_Prof        = createProfile(makeName(bn,"bVsNBinary_Prof"), nBins_nBinary,min_nBinary, max_nBinary, nBin,  "<b>");
  h_bSqVsNBinary_Prof      = createProfile(makeName(bn,"bSqVsNBinary_Prof"),    nBins_nBinary,min_nBinary, max_nBinary, nBin,  "<b^{2}>");

  h_crossSection           = createHistogram(makeName(bn,"Xsect"),    100,  0.0,            100.0,          "Xsect",    counts);
  h_nPartVsXsect_Prof      = createProfile(makeName(bn,"nPartVsXsect_Prof"),     100,  0.0, 100.0,          "Xsect",    "<N_{Part}>");
  h_nPartSqVsXsect_Prof    = createProfile(makeName(bn,"nPartSqVsXsect_Prof"),   100,  0.0, 100.0,          "Xsect",    "<N_{Part}^{2}>");
  h_nBinaryVsXsect_Prof    = createProfile(makeName(bn,"nBinaryVsXsect_Prof"),   100,  0.0, 100.0,          "Xsect",    "<N_{Bin}>");
  h_nBinarySqVsXsect_Prof  = createProfile(makeName(bn,"nBinarySqVsXsect_Prof"), 100,  0.0, 100.0,          "Xsect",    "<N_{Bin}^{2}>");
  h_bVsXsect_Prof          = createProfile(makeName(bn,"bVsXsect_Prof"),         100,  0.0, 100.0,          "Xsect",    "<b>");
  h_bSqVsXsect_Prof        = createProfile(makeName(bn,"bSqVsXsect_Prof"),       100,  0.0, 100.0,          "Xsect",    "<b^{2}>");

  h_xyDistInteractions     = createHistogram(makeName(bn,"xyDistInteractions"),   400, -20.0, 20.0,   400, -20.0, 20.0, "x (fm)",  "y (fm)",   counts);
  h_xyDistNucleons         = createHistogram(makeName(bn,"xyDistNucleons"),       400, -20.0, 20.0,   400, -20.0, 20.0, "x (fm)",  "y (fm)",   counts);
  // Derived Histograms
  h_nPartRmsVsB            = createHistogram(makeName(bn,"nPartRmsVsB"),       nBins_b, min_b,  max_b,      impact,   "RMS(N_{Part})");
  h_nPartOmegaVsB          = createHistogram(makeName(bn,"nPartOmegaVsB"),     nBins_b, min_b,  max_b,      impact,   "#omega(N_{part})");
  h_nPartR2VsB             = createHistogram(makeName(bn,"nPartR2VsB"),        nBins_b, min_b,  max_b,      impact,   "R_{2}(N_{part})");
  h_nBinaryRmsVsB          = createHistogram(makeName(bn,"nBinaryRmsVsB"),     nBins_b, min_b,  max_b,      impact,   "RMS(N_{Bin})");
  h_nBinaryOmegaVsB        = createHistogram(makeName(bn,"nBinaryOmegaVsB"),   nBins_b, min_b,  max_b,      impact,   "#omega(N_{Bin})");
  h_nBinaryR2VsB           = createHistogram(makeName(bn,"nBinaryR2VsB"),      nBins_b, min_b,  max_b,      impact,   "R_{2}(N_{bin})");

  h_nBinaryRmsVsNPart      = createHistogram(makeName(bn,"nBinaryRmsVsNPart"),  nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "RMS(N_{Bin})");
  h_nBinaryOmegaVsNPart    = createHistogram(makeName(bn,"nBinaryOmegaVsNPart"),nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "#omega(N_{Bin})");
  h_nBinaryR2VsNPart       = createHistogram(makeName(bn,"nBinaryR2VsNPart"),   nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "R_{2}(N_{bin})");
  h_bRmsVsNPart            = createHistogram(makeName(bn,"bRmsVsNPart"),        nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "RMS(b)");
  h_bOmegaVsNPart          = createHistogram(makeName(bn,"bOmegaVsNPart"),      nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "#omega(b)");
  h_bR2VsNPart             = createHistogram(makeName(bn,"bR2VsNPart"),         nBins_nPart,  min_nPart,   max_nPart,   nParticipants, "R_{2}(b)");

  h_nPartRmsVsNBinary      = createHistogram(makeName(bn,"nPartRmsVsNBinary"),  nBins_nBinary,min_nBinary, max_nBinary, nBin, "RMS(N_{Bin})");
  h_nPartOmegaVsNBinary    = createHistogram(makeName(bn,"nPartOmegaVsNBinary"),nBins_nBinary,min_nBinary, max_nBinary, nBin, "#omega(N_{Bin})");
  h_nPartR2VsNBinary       = createHistogram(makeName(bn,"nPartR2VsNBinary"),   nBins_nBinary,min_nBinary, max_nBinary, nBin, "R_{2}(N_{bin})");
  h_bRmsVsNBinary          = createHistogram(makeName(bn,"bRmsVsNBinary"),      nBins_nBinary,min_nBinary, max_nBinary, nBin, "RMS(b)");
  h_bOmegaVsNBinary        = createHistogram(makeName(bn,"bOmegaVsNBinary"),    nBins_nBinary,min_nBinary, max_nBinary, nBin, "#omega(b)");
  h_bR2VsNBinary           = createHistogram(makeName(bn,"bR2VsNBinary"),       nBins_nBinary,min_nBinary, max_nBinary, nBin, "R_{2}(b)");

  h_nPartRmsVsXsect        = createHistogram(makeName(bn,"nPartRmsVsXsect"),      100,  0.0,            100.0,          "Xsect", "RMS(N_{Part})");
  h_nPartOmegaVsXsect      = createHistogram(makeName(bn,"nPartOmegaVsXsect"),    100,  0.0,            100.0,          "Xsect", "#omega(N_{Part})");
  h_nPartR2VsXsect         = createHistogram(makeName(bn,"nPartR2VsXsect"),       100,  0.0,            100.0,          "Xsect", "R_{2}(N_{Part})");

  h_nBinaryRmsVsXsect      = createHistogram(makeName(bn,"nNBinaryRmsVsXsect"),   100,  0.0,            100.0,          "Xsect", "RMS(N_{Bin})");
  h_nBinaryOmegaVsXsect    = createHistogram(makeName(bn,"nBinaryOmegaVsXsect"),  100,  0.0,            100.0,          "Xsect", "#omega(N_{Bin})");
  h_nBinaryR2VsXsect       = createHistogram(makeName(bn,"nBinaryR2VsXsect"),     100,  0.0,            100.0,          "Xsect", "R_{2}(N_{Bin})");

  h_bRmsVsXsect            = createHistogram(makeName(bn,"bRmsVsXsect"),          100,  0.0,            100.0,          "Xsect", "RMS(b)");
  h_bOmegaVsXsect          = createHistogram(makeName(bn,"bOmegaVsXsect"),        100,  0.0,            100.0,          "Xsect", "#omega(b)");
  h_bR2VsXsect             = createHistogram(makeName(bn,"bR2VsXsect"),           100,  0.0,            100.0,          "Xsect", "R_{2}(b)");

//  bValues10[0] = 0.0;
//  bValues10[1] = 3.39927;
//  bValues10[2] = 4.79929;
//  bValues10[3] = 6.79951;
//  bValues10[4] = 8.39898;
//  bValues10[5] = 9.79733;
//  bValues10[6] = 10.9965;
//  bValues10[7] = 11.9972;
//  bValues10[8] = 12.9963;
//  bValues10[9] = 13.7987;
//  bValues10[10] = 14.7986;
//  bValues10[11] = 20.0;

//  h_xyNNIntVsB         = createHistogram(bn+"xyNNIntVsB",  nBins_b,  min_b,  max_b,  40, -10.0, 10.0, 40, -10.0, 10.0, "b", "x", "y", counts,0,1);
//  h_varXVsB_Prof       = createProfile(bn+"varXVsB_Prof",  nBins_b,  min_b,  max_b,  "b",  "Var[x]", 1);
//  h_varYVsB_Prof       = createProfile(bn+"varYVsB_Prof",  nBins_b,  min_b,  max_b,  "b",  "Var[y]", 1);
//  h_covXYVsB_Prof      = createProfile(bn+"covXYVsB_Prof", nBins_b,  min_b,  max_b,  "b",  "Cov[x,y]", 1);
//  h_epsilonXVsB_Prof   = createProfile(bn+"epsilonXVsB_Prof",   nBins_b,  min_b,  max_b,  "b",  "#epsilon_{x}", 1);
//  h_epsilonYVsB_Prof   = createProfile(bn+"epsilonYVsB_Prof",   nBins_b,  min_b,  max_b,  "b",  "#epsilon_{y}", 1);
//  h_epsilonXYVsB_Prof  = createProfile(bn+"epsilonXYVsB_Prof",  nBins_b,  min_b,  max_b,  "b",  "|#epsilon|", 1);
//  h_epsilonXYVsB       = createHistogram(makeName(bn,"epsilonXYVsB"), nBins_b,  min_b,  max_b,  40, -1.0, 1.0,  40, -1.0, 1.0, "b", "#epsilon_{x}", "#epsilon_{y}", counts,0,1);;
//  h_psi2VsB_Prof       = createProfile(bn+"psi2VsB_Prof",       nBins_b,  min_b,  max_b,  "b",  "#psi_{2}", 1);
//  h_psi2VsB            = createHistogram(makeName(bn,"psi2VsB"), nBins_b,  min_b,  max_b,  40, -TMath::Pi(), TMath::Pi(), "b", "#psi_2", counts,0,1);

  if (reportEnd(__FUNCTION__))
    ;
}

//________________________________________________________________________
void CollisionGeometryHistograms::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  if (reportEnd(__FUNCTION__))
    ;
}

void CollisionGeometryHistograms::fill(Event & event, double weight)
{
  EventProperties & ep = *event.getEventProperties();
  double impactPar     = ep.impactParameter;
  double nParticipants = ep.nParticipantsTotal;
  double nBinary       = ep.nBinaryTotal;
  double xSect         = ep.fractionalXSection;

//  if (reportInfo("CollisionGeometryHistograms",getName(),"loadHistograms(TFile * inputFile)"))
//    {
//    cout<< " impactPar :" << impactPar << endl;
//    cout<< "     nParticipants :" << nParticipants << endl;
//    cout<< "   nBinary :" << nBinary << endl;
//    cout<< "     xSect :" << xSect << endl;
//    }

  // Impact parameter, cross-section, nParticipants, nBinaries, etc.
  h_nAcceptedVsB           ->Fill(impactPar, weight);
  h_nPartVsB               ->Fill(impactPar, nParticipants, weight);
  h_nBinaryVsB             ->Fill(impactPar, nBinary, weight);
  h_nPartVsB_Prof          ->Fill(impactPar, nParticipants, weight);
  h_nPartSqVsB_Prof        ->Fill(impactPar, nParticipants*nParticipants, weight);
  h_nBinaryVsB_Prof        ->Fill(impactPar, nBinary, weight);
  h_nBinarySqVsB_Prof      ->Fill(impactPar, nBinary*nBinary, weight);

  h_nPart                  ->Fill(nParticipants,  weight);
  h_nBinaryVsNPart_Prof    ->Fill(nParticipants,  nBinary,  weight);
  h_nBinarySqVsNPart_Prof  ->Fill(nParticipants,  nBinary*nBinary,     weight);
  h_bVsNPart_Prof          ->Fill(nParticipants,  impactPar,           weight);
  h_bSqVsNPart_Prof        ->Fill(nParticipants,  impactPar*impactPar, weight);

  h_nBinary                ->Fill(nBinary, weight);
  h_nPartVsNBinary_Prof    ->Fill(nBinary, nParticipants,    weight);
  h_nPartSqVsNBinary_Prof  ->Fill(nBinary, nParticipants*nParticipants,         weight);
  h_bVsNBinary_Prof        ->Fill(nBinary, impactPar,           weight);
  h_bSqVsNBinary_Prof      ->Fill(nBinary, impactPar*impactPar, weight);

  h_crossSection           ->Fill(xSect, weight);
  h_nPartVsXsect_Prof      ->Fill(xSect, nParticipants,           weight);
  h_nPartSqVsXsect_Prof    ->Fill(xSect, nParticipants*nParticipants,     weight);
  h_nBinaryVsXsect_Prof    ->Fill(xSect, nBinary,         weight);
  h_nBinarySqVsXsect_Prof  ->Fill(xSect, nBinary*nBinary, weight);
  h_bVsXsect_Prof          ->Fill(xSect, impactPar,           weight);
  h_bSqVsXsect_Prof        ->Fill(xSect, impactPar*impactPar, weight);

  // Geometry

  const vector<Particle*> interactions = event.getNucleonNucleonInteractions();
  for (unsigned int k=0;k<interactions.size(); k++)
    {
    const TLorentzVector & position = interactions[k]->getPosition();
    h_xyDistInteractions->Fill( position.X(), position.Y() );
    }

  Nucleus & nucleusA =  event.getNucleusA();
  Nucleus & nucleusB =  event.getNucleusB();
  vector<Particle*> nucleonsA = nucleusA.getChildren();
  vector<Particle*> nucleonsB = nucleusB.getChildren();
  for (unsigned int k=0;k<nucleonsA.size(); k++)
    {
    const TLorentzVector & position = nucleonsA[k]->getPosition();
    h_xyDistNucleons->Fill( position.X(), position.Y() );
    }
  for (unsigned int k=0;k<nucleonsB.size(); k++)
    {
    const TLorentzVector & position = nucleonsB[k]->getPosition();
    h_xyDistNucleons->Fill( position.X(), position.Y() );
    }
}

void CollisionGeometryHistograms::noFill(Event & event, double weight)
{
  EventProperties & ep = *event.getEventProperties();
  double impactPar     = ep.impactParameter;
//  double nParticipants = ep.nParticipantsTotal;
//  double nBinary       = ep.nBinaryTotal;
//  double xSect         = ep.fractionalXSection;
  h_nProcessedVsB->Fill(impactPar, weight);
}

//
//void CollisionGeometryHistograms::create Derived Histograms()
//{
//  if (reportStart(__FUNCTION__))
//    ;
////  double area    = 1.0; //h_crossSection->GetBinContent(4);
////  double ratio1  = nEvents>0      ? nEventsWColl/nEvents : 1.0e-30;
////  double eRatio1 = nEvents>0      ? eNEventWColl/nEvents : 1.0e-30;
////  double ratio2  = nEventNcollGE0 ? double(nEventNcollGE1)/double(nEventNcollGE0) : 1.0e-30;
////  double eRatio2 = nEventNcollGE0 ? sqrt(double(nEventNcollGE1))/double(nEventNcollGE0) : 1.0e-30;
////  double xSect1  = ratio1*area;
////  double eXSect1 = eRatio1*area;
////  double xSect2  = ratio2*area;
////  double eXSect2 = eRatio2*area;
//
////  if (reportInfo(__FUNCTION__))
////    {
////    cout << "              nEvents: " <<  nEvents << endl;
////    cout << "         nEventsWColl: " <<  nEventsWColl << endl;
////    cout << "            nEventsWColl/nEvents: " <<  ratio1 << " +-" << eRatio1 << endl;
////    cout << "       nEventNcollGE0: " <<  nEventNcollGE0 << endl;
////    cout << "       nEventNcollGE1: " <<  nEventNcollGE1 << endl;
////    cout << "   nEventNcollGE1/nEventNcollGE0: " <<  ratio2 << " +-" << eRatio2 << endl;
////    cout << "            xSect1 = ratio1*area: " <<  xSect1 << " +-" << eXSect1 << endl;
////    cout << "            xSect2 = ratio2*area: " <<  xSect2 << " +-" << eXSect2 << endl;
////    }
//
//  calculateRms(h_nPartVsB_Prof,       h_nPartSqVsB_Prof,       h_nPartRmsVsB,       h_nPartOmegaVsB,       h_nPartR2VsB);
//  calculateRms(h_nBinaryVsB_Prof,     h_nBinarySqVsB_Prof,     h_nBinaryRmsVsB,     h_nBinaryOmegaVsB,     h_nBinaryR2VsB);
//
//  calculateRms(h_nBinaryVsNPart_Prof, h_nBinarySqVsNPart_Prof, h_nBinaryRmsVsNPart, h_nBinaryOmegaVsNPart, h_nBinaryR2VsNPart);
//  calculateRms(h_bVsNPart_Prof,       h_bSqVsNPart_Prof,       h_bRmsVsNPart,       h_bOmegaVsNPart,       h_bR2VsNPart);
//
//  calculateRms(h_nPartVsNBinary_Prof, h_nPartSqVsNBinary_Prof, h_nPartRmsVsNBinary, h_nPartOmegaVsNBinary, h_nPartR2VsNBinary);
//  calculateRms(h_bVsNBinary_Prof,     h_bSqVsNBinary_Prof,     h_bRmsVsNBinary,     h_bOmegaVsNBinary,     h_bR2VsNBinary);
//
//  calculateRms(h_nPartVsXsect_Prof,   h_nPartSqVsXsect_Prof,   h_nPartRmsVsXsect,   h_nPartOmegaVsXsect,   h_nPartR2VsXsect);
//  calculateRms(h_nBinaryVsXsect_Prof, h_nBinarySqVsXsect_Prof, h_nBinaryRmsVsXsect, h_nBinaryOmegaVsXsect, h_nBinaryR2VsXsect);
//  calculateRms(h_bVsXsect_Prof,       h_bSqVsXsect_Prof,       h_bRmsVsXsect,       h_bOmegaVsXsect,       h_bR2VsXsect);
//
//  if (reportEnd(__FUNCTION__))
//    ;
//}

void CollisionGeometryHistograms::calculateRms(TProfile * h1, TProfile * h1Sq,   TH1* h1Rms, TH1* h1Omega, TH1* h1R2)
{
  if (reportStart("CollisionGeometryHistograms",getName(),"calculateRms(..)"))
    ;
  double v, vSq, rms, erms, omega, eomega, R2, eR2;
  int nBins = h1->GetNbinsX();
  for (int iBin=1; iBin<nBins; iBin++)
    {
    v   = h1->GetBinContent(iBin);
    vSq = h1Sq->GetBinContent(iBin);
    rms = vSq - v*v;
    if (rms>0) rms = sqrt(rms);
    erms = 0.0;

    if (v>0)
      {
      omega  = rms*rms/v;
      eomega = 0.0;
      R2 = (vSq-v)/v/v - 1.0;
      eR2 = 0;
      }
    else
      {
      omega  = 0.0;
      eomega = 0.0;
      R2     = 0.0;
      eR2    = 0;
      }

    //cout << " iBin: " << iBin << " v:" << v << " v2:" << v2 << " rms:" << rms << " omega:" << omega << " R2:" << R2 << endl;

    h1Rms->SetBinContent(iBin, rms);
    h1Rms->SetBinError  (iBin, erms);

    h1Omega->SetBinContent(iBin, omega);
    h1Omega->SetBinError(iBin, eomega);

    h1R2->SetBinContent(iBin, R2);
    h1R2->SetBinError(iBin, eR2);
    }
  if (reportEnd("CollisionGeometryHistograms",getName(),"calculateRms(..)"))
    ;
}
