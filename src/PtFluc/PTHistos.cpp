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
#include "PTHistos.hpp"
#include "TH1.h"

ClassImp(PTHistos);

PTHistos::PTHistos(const TString & name,
                   Configuration * configuration,
                   LogLevel  debugLevel,
                   int ord)
:
Histograms(name,configuration,debugLevel),
maxOrder(ord),
histoIndex(0),
size(0),
numFunc(3)
{
	// no  ops
}

PTHistos::~PTHistos()
{
	//deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void PTHistos::createHistograms()
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::createHistograms(...) started"<< endl;
//	HeavyIonConfiguration & ac = (HeavyIonConfiguration&)( *getConfiguration());
//	TString bn = getHistoBaseName();
//	TH1::SetDefaultBufferSize(ac.totEvents);
//	totEvents =ac.totEvents;
//
//	// ================================================================================
//	// Naming convention
//	// ================================================================================
//  	// S is the pT deviation moments
//  	// s are the normalized moments
//  	// s* are the moments normalized by inclusive average pT's
// 	// C is the cumulants
//	// c are the normalized cumulants
// 	// c* are the cumulants normalizd by inclusive average pT's
//
//	size = (TMath::Factorial(2 * maxOrder)) / (TMath::Factorial(maxOrder ) * TMath::Factorial(maxOrder )) - 1;
//
//	hS = new TProfile ** [numFunc];
//	hS_vsMult = new TProfile ** [numFunc];
//	hS_vsCent = new TProfile ** [numFunc];
//
//	hC = new TH1 ** [numFunc];
//	hC_vsMult = new TH1 ** [numFunc];
//	hC_vsCent = new TH1 ** [numFunc];
//
//	names = new TString* [numFunc];
//	titles = new TString *[numFunc];
//	names2 = new TString* [numFunc];
//	titles2 = new TString *[numFunc];
//
//	for(int i = 0; i < numFunc; i++)
//	{
//		hS[i] = new TProfile * [size];
//		hS_vsMult[i] = new TProfile * [size];
//		hS_vsCent[i] = new TProfile * [size];
//
//		hC[i] = new TH1 * [size];
//		hC_vsMult[i] = new TH1 * [size];
//		hC_vsCent[i] = new TH1 * [size];
//
//		names[i] = new TString [size];
//		titles[i] = new TString [size];
//		names2[i] = new TString [size];
//		titles2[i] = new TString [size];
//	}
//
//	h_counts = new TProfile*  [size];
//	h_counts_vsMult = new TProfile * [size];
//	h_counts_vsCent = new TProfile * [size];
//
//
//	orders = new int [size];
//
//	h_eventStreams   = createHistogram(bn+TString("NeventStreams"),1, 0.0, 0.0,  "mult","n_{Events}");
//	if (ac.ptCorrelatorVsMult) h_eventStreams_vsMult = createHistogram(bn+TString("NeventStreams_vsMult"),ac.nBins_mult, 0.0, 0.0,  "mult","n_{Events}");
//	if (ac.ptCorrelatorVsCent) h_eventStreams_vsCent = createHistogram(bn+TString("NeventStreams_vsCent"),ac.nBins_cent, 0.0, 0.0,  "cent","n_{Events}");
//
//	TString * baseName = new TString[2 * numFunc + 1];
//	baseName[0] = bn + "S_";
//	baseName[1] = bn + "s_";
//	baseName[2] = bn + "s*_";
//	baseName[3] = bn + "C_";
//	baseName[4] = bn + "c_" ;
//	baseName[5] = bn + "c*_";
//	baseName[6] = bn + "Counts_";
//
//	TString * baseTitle = new TString[2 * numFunc + 1];
//	baseTitle[0] = "S_{";
//	baseTitle[1] = "s_{";
//	baseTitle[2] = "s*_{";
//	baseTitle[3] = "C_{";
//	baseTitle[4] = "c_{";
//	baseTitle[5] = "c*_{";
//	baseTitle[6] = "Counts_{";
//
//	histoIndex = 0;
//	createHistogramRec(baseName, baseTitle, maxOrder - 1, 0);
//
//	reorder = new int[size];
//	int counter = 0;
//	for(int iOrd = 1; iOrd <= maxOrder; iOrd++)
//	{
//		for(int iHisto = 0; iHisto < size; iHisto++)
//		{
//			if(orders[iHisto] == iOrd)
//			{
//				reorder[iHisto] = counter;
//				counter++;
//
//			}
//		}
//	}
//
//	histoIndex = 0;
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::createHistograms(...) ended"<< endl;
//	//h_c123vsMultTest = createProfile("c123Test", ac.nBins_mult,ac.min_mult,ac.max_mult,"mult", "c123Test" );
}

/////////////////////////////////////////////////////////
// needs to be fixed
////////////////////////////////////////////////////////
void PTHistos::loadHistograms(TFile * inputFile)
{
//	if (!inputFile)
//	{
//		if (reportFatal()) cout << "-Fatal- Attempting to load NuDynHistos from an invalid file pointer" << endl;
//		return;
//	}
//	Configuration & ac = *(Configuration*) getConfiguration();
//
//	TString  bn = getHistoBaseName();
//
//	size = (TMath::Factorial(2 * maxOrder)) / (TMath::Factorial(maxOrder ) * TMath::Factorial(maxOrder )) - 1;
//
//	hS = new TProfile ** [numFunc];
//	hS_vsMult = new TProfile ** [numFunc];
//	hS_vsCent = new TProfile ** [numFunc];
//
//	hC = new TH1 ** [numFunc];
//	hC_vsMult = new TH1 ** [numFunc];
//	hC_vsCent = new TH1 ** [numFunc];
//
//	for(int i = 0; i < numFunc; i++)
//	{
//		hS[i] = new TProfile * [size];
//		hS_vsMult[i] = new TProfile * [size];
//		hS_vsCent[i] = new TProfile * [size];
//
//		hC[i] = new TH1 * [size];
//		hC_vsMult[i] = new TH1 * [size];
//		hC_vsCent[i] = new TH1 * [size];
//	}
//
//	h_counts = new TProfile*  [size];
//	h_counts_vsMult = new TProfile * [size];
//	h_counts_vsCent = new TProfile * [size];
//
//
//	orders = new int [size];
//
//	h_eventStreams   = loadH1(inputFile, bn+TString("NeventStreams"));
//	if (ac.ptCorrelatorVsMult) h_eventStreams_vsMult = loadH1(inputFile, bn+TString("NeventStreams_vsMult"));
//	if (ac.ptCorrelatorVsCent) h_eventStreams_vsCent = loadH1(inputFile, bn+TString("NeventStreams_vsCent"));
//
//	TString * baseName = new TString[numFunc + 1];
//	baseName[0] = bn + "S_";
//	baseName[1] = bn + "s_";
//	baseName[2] = bn + "s*_";
//	baseName[3] = bn + "C_";
//	baseName[4] = bn + "c_" ;
//	baseName[5] = bn + "c*_";
//	baseName[6] = bn + "Counts_";
//
//	TString * baseTitle = new TString[numFunc + 1];
//	baseTitle[0] = "S_{";
//	baseTitle[1] = "s_{";
//	baseTitle[2] = "s*_{";
//	baseTitle[3] = "C_{";
//	baseTitle[4] = "c_{";
//	baseTitle[5] = "c*_{";
//	baseTitle[6] = "Counts_{";
//
//
//	loadHistogramRec(baseName, maxOrder - 1, 0, inputFile);
//
//	histoIndex = 0;
}

//overloaded saveHistograms to save histograms in sequence of lowest order to highest order
void PTHistos::saveHistograms(TFile * outputFile, bool saveAll)
{
//
//	if (reportDebug(__FUNCTION__)) cout << "HistogramCollection::saveHistograms(TFile * outputFile) started."  << endl;
//	outputFile->cd();
//
//	Configuration & ac = *(Configuration*) getConfiguration();
//
//	int numTypes = 1;
//	if (ac.ptCorrelatorVsMult) numTypes++;
//	if (ac.ptCorrelatorVsCent) numTypes++;
//
//	for (int k=0; k<numTypes; k++)
//	{
//		if (isSaved(options[k]) || saveAll)  getObjectAt(k)->Write();
//	}
//
//	for(int iFunc = 0; iFunc <= 2 * numFunc; iFunc++)
//	{
//		for (int i = 1; i <= maxOrder; i++)
//		{
//			for (int k=numTypes; k<getCollectionSize(); k++)
//			{
//				if(k < size * numTypes + numTypes && iFunc == 2 * numFunc)
//				{
//					int k1 = (k - numTypes);
//					int orderIndex = k1 / numTypes;
//        if ((isSaved(options[k]) || saveAll) && (orders[orderIndex] == i)) getObjectAt(k)->Write();
//				}
//				if(k >= size * numTypes + numTypes && k < size * (numFunc + 1) * numTypes + numTypes && iFunc < numFunc)
//				{
//					int k1 = k - size * numTypes - numTypes;
//					int orderIndex = (k1 )/(numTypes * (numFunc ));
//					int funcIndex = (k1 )/numTypes - ( (numFunc )) * ((k1)/(numTypes * (numFunc )));
//        if ((isSaved(options[k]) || saveAll) && (orders[orderIndex] == i) && ((funcIndex) % ( numFunc) == (iFunc % ( numFunc)))) getObjectAt(k)->Write();
//				}
//				if(k >= size * (numFunc + 1) * numTypes + numTypes && iFunc >= numFunc && iFunc < 2 * numFunc)
//				{
//					int k1 = k - size * (numFunc + 1) * numTypes - numTypes;
//					int orderIndex = (k1 )/(numTypes * (numFunc ));
//					int funcIndex = (k1 )/numTypes - ( (numFunc )) * ((k1)/(numTypes * (numFunc )));
//        if ((isSaved(options[k]) || saveAll) && (orders[orderIndex] == i) && ((funcIndex) % ( numFunc) == (iFunc - ( numFunc)))) getObjectAt(k)->Write();
//				}
//			}
//		}
//	}
//
//	if (reportDebug(__FUNCTION__)) cout << "HistogramCollection::saveHistograms(TFile * outputFile) completed."  << endl;
}


void PTHistos::fillEventHistos(double mult, double cent, double weight)
{
//	Configuration & ac1 = *(Configuration*) getConfiguration();
//	h_eventStreams->Fill(mult,weight);
//	if (ac1.ptCorrelatorVsMult) h_eventStreams_vsMult->Fill(mult,weight);
//	if (ac1.ptCorrelatorVsCent) h_eventStreams_vsCent->Fill(cent,weight);
}


// recursively create histograms for correlation functions of order 1 - maxOrder
// Note: these histograms are not in sequence of lowest order to highest order. The reording occurs when the histograms are saved with the saveHistograms function.
void PTHistos::createHistogramRec(TString * baseName, TString * baseTitle, int depth, int partIndex)
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::createHistogramsRec(...) started"<< endl;
//	Configuration & ac = *(Configuration*) getConfiguration();
//	TString *histoName = new TString[2 * numFunc +1];
//	TString *histoTitle= new TString[2 * numFunc +1];
//
//
//	for(int i = partIndex; i < maxOrder; i++)
//	{
//
//		for(int iFunc = 0; iFunc < numFunc ; iFunc++)
//		{
//			histoName[iFunc]  = baseName[iFunc] + (i + 1) ;
//			histoTitle[iFunc] = (depth == maxOrder - 1) ? baseTitle[iFunc] + (i + 1) : baseTitle[iFunc] + ", " + (i + 1);
//			histoName[iFunc + numFunc]  = baseName[iFunc + numFunc] + (i + 1);
//			histoTitle[iFunc + numFunc] = (depth == maxOrder - 1) ? baseTitle[iFunc + numFunc] + (i + 1) : baseTitle[iFunc + numFunc] + ", " + (i + 1);
//
//			names[iFunc][histoIndex] = histoName[iFunc];
//			titles[iFunc][histoIndex] = histoTitle[iFunc];
//
//			/*hS[iFunc][histoIndex] = createProfile( histoName[iFunc], 1, ac.max_mult, ac.min_mult,"mult",histoTitle[iFunc] + "}" );
//			if (ac.ptCorrelatorVsMult)	hS_vsMult[iFunc][histoIndex] = createProfile(histoName[iFunc] + "_vsMult",ac.nBins_mult, ac.max_mult, ac.min_mult, "mult", histoTitle[iFunc] + "}");
//			if (ac.ptCorrelatorVsCent)	hS_vsCent[iFunc][histoIndex] = createProfile(histoName[iFunc] + "_vsCent",ac.nBins_cent,ac.max_mult, ac.min_mult, "cent", histoTitle[iFunc] + "}");*/
//
//			names2[iFunc][histoIndex] = histoName[iFunc + numFunc];
//			titles2[iFunc][histoIndex] = histoTitle[iFunc + numFunc];
//			/*hC[iFunc][histoIndex] = createHistogram( histoName[iFunc + numFunc], 1, ac.min_mult, ac.max_mult,"mult",histoTitle[iFunc + numFunc] + "}" );
//			if (ac.ptCorrelatorVsMult)	hC_vsMult[iFunc][histoIndex] = createHistogram(histoName[iFunc + numFunc] + "_vsMult",ac.nBins_mult,ac.min_mult, ac.max_mult, "mult", histoTitle[iFunc + numFunc] + "}");
//			if (ac.ptCorrelatorVsCent)	hC_vsCent[iFunc][histoIndex] = createHistogram(histoName[iFunc + numFunc] + "_vsCent",ac.nBins_cent,ac.min_mult, ac.max_mult, "cent", histoTitle[iFunc + numFunc] + "}");*/
//		}
//
//
//		histoName[2 * numFunc]  = baseName[2 * numFunc] + (i + 1);
//		histoTitle[2 * numFunc] = (depth == maxOrder - 1) ? baseTitle[2 * numFunc] + (i + 1) : baseTitle[2 * numFunc] + ", " + (i + 1);
//
//		h_counts[histoIndex]         = createProfile(histoName[2 * numFunc], 1, ac.max_mult, ac.min_mult,"mult", histoTitle[2 * numFunc] + "}" );
//		if (ac.ptCorrelatorVsMult)	h_counts_vsMult[histoIndex]  = createProfile(histoName[2 * numFunc] + "_vsMult",ac.nBins_mult, ac.max_mult, ac.min_mult, "mult", histoTitle[2 * numFunc] + "}");
//		if (ac.ptCorrelatorVsCent)	h_counts_vsCent[histoIndex]  = createProfile(histoName[2 * numFunc] + "_vsCent",ac.nBins_cent,ac.max_cent,  ac.min_cent, "cent",histoTitle[2 * numFunc] + "}");
//
//		orders[histoIndex] = maxOrder - depth;
//
//		histoIndex++;
//
//		if(depth != 0)	createHistogramRec(histoName, histoTitle , depth - 1, i);
//	}
//
//	delete [] histoName;
//	delete [] histoTitle;
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::createHistogramsRec(...) ended"<< endl;
//	return;
}

//////////////////////////////////////////////
// need to fix
//////////////////////////////////////////////
void PTHistos::loadHistogramRec(TString * baseName, int depth, int partIndex, TFile * inputFile)
{

//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::loadHistogramRec(...) Starting." << endl;
//	Configuration & ac = *(Configuration*) getConfiguration();
//	TString *histoName = new TString[2 * numFunc +1];
//
//
//	for(int i = partIndex; i < maxOrder; i++)
//	{
//
//		for(int iFunc = 0; iFunc < numFunc ; iFunc++)
//		{
//			histoName[iFunc]  = baseName[iFunc] + (i + 1) ;
//			histoName[iFunc + numFunc]  = baseName[iFunc + numFunc] + (i + 1);
//
//
//			hS[iFunc][histoIndex] = loadProfile(inputFile, histoName[iFunc] );
//			if (ac.ptCorrelatorVsMult)	hS_vsMult[iFunc][histoIndex] = loadProfile(inputFile, histoName[iFunc] + "_vsMult");
//			if (ac.ptCorrelatorVsCent)	hS_vsCent[iFunc][histoIndex] = loadProfile(inputFile, histoName[iFunc] + "_vsCent");
//
//
//			hC[iFunc][histoIndex] = loadProfile(inputFile, histoName[iFunc + numFunc] );
//			if (ac.ptCorrelatorVsMult)	hC_vsMult[iFunc][histoIndex] = loadProfile(inputFile, histoName[iFunc + numFunc] + "_vsMult");
//			if (ac.ptCorrelatorVsCent)	hC_vsCent[iFunc][histoIndex] = loadProfile(inputFile, histoName[iFunc + numFunc] + "_vsCent");
//		}
//
//
//		histoName[2 * numFunc]  = baseName[2 * numFunc] + (i + 1);
//
//		h_counts[histoIndex]         = loadProfile(inputFile, histoName[2 * numFunc] + "}" );
//		if (ac.ptCorrelatorVsMult)	h_counts_vsMult[histoIndex]  = loadProfile(inputFile, histoName[2 * numFunc] + "_vsMult");
//		if (ac.ptCorrelatorVsCent)	h_counts_vsCent[histoIndex]  = loadProfile(inputFile, histoName[2 * numFunc] + "_vsCent");
//
//		orders[histoIndex] = maxOrder - depth;
//
//		histoIndex++;
//
//		if(depth != 0)	loadHistogramRec(histoName , depth - 1, i, inputFile);
//	}
//
//	delete [] histoName;
//	return;
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::loadHistogramRec(...) Completed." << endl;
}

void PTHistos::fillDerivedHistos(bool *** acceptances, double * mults, double * cents, int * numParticles, double ** pT)
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::fillDerivedHistos(...) Starting." << endl;
//	auto start = chrono::high_resolution_clock::now();
//	HeavyIonConfiguration & ac = (HeavyIonConfiguration&)*getConfiguration();
//	//double max = ac.nCollisionsMax;
//	h_eventStreams->BufferEmpty();
//	ac.min_mult = h_eventStreams->GetXaxis()->GetXmin();
//	ac.max_mult = h_eventStreams->GetXaxis()->GetXmax();
//
//
//	avgpT = new double  [maxOrder];
//	calculateInclusivePtAverage(acceptances, numParticles, pT);
//
//	avgCounts = new double [size]();
//	counts = new int * [totEvents]();
//	calculateInclusiveYieldsAverage(acceptances, numParticles);
//
//	SValues = new double * [totEvents]();
//	calculateEventMoments(acceptances, numParticles, pT);
//
//	//fill SValues
//	//fill SValues normalized by counts and average pT's
//	for(int iEvent = 0; iEvent < totEvents;iEvent++)
//	{
//		for(int iHisto = 0; iHisto <size; iHisto++)
//		{
//			if(iEvent == 0)
//			{
//				for (int iFunc = 0; iFunc < numFunc; ++iFunc)
//				{
//					hS[iFunc][iHisto] = createProfile( names[iFunc][iHisto], 1, ac.min_mult , ac.max_mult ,"mult",titles[iFunc][iHisto] + "}" );
//					if (ac.ptCorrelatorVsMult)	hS_vsMult[iFunc][iHisto] = createProfile(names[iFunc][iHisto] + "_vsMult",ac.nBins_mult,ac.min_mult, ac.max_mult, "mult", titles[iFunc][iHisto] + "}");
//					if (ac.ptCorrelatorVsCent)	hS_vsCent[iFunc][iHisto] = createProfile(names[iFunc][iHisto] + "_vsCent",ac.nBins_mult,ac.min_mult, ac.max_mult , "cent", titles[iFunc][iHisto] + "}");
//				}
//			}
//			if(counts[iEvent][iHisto] != 0)
//			{
//				hS[0][iHisto]->Fill(mults[iEvent], SValues[iEvent][iHisto], 1.0);//weight is always 1.0
//				if (ac.ptCorrelatorVsMult)	hS_vsMult[0][iHisto]->Fill(mults[iEvent], SValues[iEvent][iHisto] , 1.0);
//				if (ac.ptCorrelatorVsCent)	hS_vsCent[0][iHisto]->Fill(cents[iEvent], SValues[iEvent][iHisto] , 1.0);
//
//				hS[1][iHisto]->Fill(mults[iEvent], (SValues[iEvent][iHisto] / avgCounts[iHisto]), 1.0);
//				if (ac.ptCorrelatorVsMult)  hS_vsMult[1][iHisto]->Fill(mults[iEvent], (SValues[iEvent][iHisto] / avgCounts[iHisto]), 1.0);
//				if (ac.ptCorrelatorVsCent)	hS_vsCent[1][iHisto]->Fill(cents[iEvent], (SValues[iEvent][iHisto] / avgCounts[iHisto]), 1.0);
//			}
//
//			h_counts[iHisto]->Fill(mults[iEvent], counts[iEvent][iHisto], 1.0);//weight is always 1.0
//			if (ac.ptCorrelatorVsMult)	h_counts_vsMult[iHisto]->Fill(mults[iEvent], counts[iEvent][iHisto] , 1.0);
//			if (ac.ptCorrelatorVsCent)	h_counts_vsCent[iHisto]->Fill(cents[iEvent], counts[iEvent][iHisto] , 1.0);
//		}
//		histoIndex = 0;
//		fillNormalizedPTValues(maxOrder - 1, 0, 1,  SValues[iEvent], mults[iEvent], cents[iEvent]);
//		histoIndex = 0;
//	}
//
//	TH1 *** newhCValues = new TH1**[3];
//	for(int iHisto = 0; iHisto < 3; iHisto++)
//	{
//		newhCValues[iHisto] = new TH1*[size];
//	}
//
//	calculateCumulants(hS[0], newhCValues[0], 1 ,ac.min_mult,  ac.max_mult);
//	if (ac.ptCorrelatorVsMult)	calculateCumulants(hS_vsMult[0], newhCValues[1], ac.nBins_mult,ac.min_mult,  ac.max_mult);
//	if (ac.ptCorrelatorVsCent)	calculateCumulants(hS_vsCent[0], newhCValues[2], ac.nBins_cent,ac.min_cent,  ac.max_cent);
//
//	for(int iHisto = 0; iHisto <size; iHisto++)
//	{
//		for (int i = 0; i < numFunc; ++i)
//		{
//			hC[i][iHisto] = createHistogram( names2[i][iHisto], 1, ac.min_mult, ac.max_mult,"mult",titles2[i][iHisto] + "}" );
//			if (ac.ptCorrelatorVsMult)	hC_vsMult[i][iHisto] = createHistogram(names2[i][iHisto] + "_vsMult",ac.nBins_mult,ac.min_mult , ac.max_mult , "mult", titles2[i][iHisto] + "}");
//			if (ac.ptCorrelatorVsCent)	hC_vsCent[i][iHisto] = createHistogram(names2[i][iHisto] + "_vsCent",ac.nBins_cent,ac.min_mult , ac.max_mult , "cent", titles2[i][iHisto] + "}");
//		}
//		int nBins = 1;
//		for(int iBin = 1; iBin <=nBins; iBin++)
//		{
//			if(newhCValues[0][reorder[iHisto]]->GetBinContent(iBin) != 0)
//			{
//				hC[0][iHisto]->SetBinContent(iBin, totEvents * newhCValues[0][reorder[iHisto]]->GetBinContent(iBin));
//				hC[0][iHisto]->SetBinError(iBin, totEvents * newhCValues[0][reorder[iHisto]]->GetBinError(iBin));
//
//				hC[1][iHisto]->SetBinContent(iBin,  totEvents * newhCValues[0][reorder[iHisto]]->GetBinContent(iBin)/h_counts[iHisto]->GetBinContent(iBin));
//				double err = newhCValues[0][reorder[iHisto]]->GetBinContent(iBin)/h_counts[iHisto]->GetBinContent(iBin) * ( newhCValues[0][reorder[iHisto]]->GetBinError(iBin)/newhCValues[0][reorder[iHisto]]->GetBinContent(iBin) + h_counts[iHisto]->GetBinError(iBin)/h_counts[iHisto]->GetBinContent(iBin) );
//				hC[1][iHisto]->SetBinError(iBin, totEvents * err);
//			}
//		}
//
//		if (ac.ptCorrelatorVsMult)
//		{
//			nBins =ac.nBins_mult;
//			for(int iBin = 1; iBin <=nBins; iBin++)
//			{
//				if(newhCValues[1][reorder[iHisto]]->GetBinContent(iBin) != 0)
//				{
//					hC_vsMult[0][iHisto]->SetBinContent(iBin, totEvents * newhCValues[1][reorder[iHisto]]->GetBinContent(iBin));
//					hC_vsMult[0][iHisto]->SetBinError(iBin, totEvents * newhCValues[1][reorder[iHisto]]->GetBinError(iBin));
//
//					hC_vsMult[1][iHisto]->SetBinContent(iBin,  totEvents * newhCValues[1][reorder[iHisto]]->GetBinContent(iBin)/h_counts_vsMult[iHisto]->GetBinContent(iBin));
//					double err = newhCValues[1][reorder[iHisto]]->GetBinContent(iBin)/h_counts_vsMult[iHisto]->GetBinContent(iBin) * ( newhCValues[1][reorder[iHisto]]->GetBinError(iBin)/newhCValues[1][reorder[iHisto]]->GetBinContent(iBin) + h_counts_vsMult[iHisto]->GetBinError(iBin)/h_counts_vsMult[iHisto]->GetBinContent(iBin) );
//					hC_vsMult[1][iHisto]->SetBinError(iBin, totEvents * err);
//				}
//			}
//		}
//
//		if (ac.ptCorrelatorVsCent)
//		{
//			nBins = ac.nBins_cent;
//			for(int iBin = 1; iBin <=nBins; iBin++)
//			{
//				if(newhCValues[2][reorder[iHisto]]->GetBinContent(iBin) != 0)
//				{
//					hC_vsCent[0][iHisto]->SetBinContent(iBin, totEvents * newhCValues[2][reorder[iHisto]]->GetBinContent(iBin));
//					hC_vsCent[0][iHisto]->SetBinError(iBin, totEvents * newhCValues[2][reorder[iHisto]]->GetBinError(iBin));
//
//					hC_vsCent[1][iHisto]->SetBinContent(iBin, totEvents * newhCValues[2][reorder[iHisto]]->GetBinContent(iBin)/h_counts_vsCent[iHisto]->GetBinContent(iBin));
//					double err = newhCValues[2][reorder[iHisto]]->GetBinContent(iBin)/h_counts_vsCent[iHisto]->GetBinContent(iBin) * ( newhCValues[2][reorder[iHisto]]->GetBinError(iBin)/newhCValues[2][reorder[iHisto]]->GetBinContent(iBin) + h_counts_vsCent[iHisto]->GetBinError(iBin)/h_counts_vsCent[iHisto]->GetBinContent(iBin) );
//					hC_vsCent[1][iHisto]->SetBinError(iBin, totEvents * err);
//				}
//			}
//		}
//
//	}
//
//	fillNormalizedPTValues(maxOrder - 1, 0, 1,  newhCValues, reorder, new int[3]{1,ac.nBins_mult, ac.nBins_cent});
//
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	if (reportInfo(__FUNCTION__)) cout << "Time taken to calculate derived histos: " << duration.count() << " microseconds"<< endl;
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::fillDerivedHistos(...) Completed." << endl;
//
//


}

////////////////////////////////////////////
//fill  S or C Values normalized by overall event average transverse momenta
/////////////////////////////////////////////
void PTHistos::fillNormalizedPTValues( int depth, int partIndex, double product, double * values, double  mult, double  cent)
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::fillNormalizedPTValues(...) Starting." << endl;
//	Configuration & ac = *(Configuration*)  getConfiguration();
//
//	for(int i = partIndex; i < maxOrder; i++)
//	{
//		double newProduct = product * avgpT[i];
//
//		if(values[histoIndex] != 0)
//		{
//			hS[2][histoIndex]->Fill(mult, (values[histoIndex] / newProduct), 1.0);
//			if (ac.ptCorrelatorVsMult)	hS_vsMult[2][histoIndex]->Fill(mult, (values[histoIndex] / newProduct), 1.0);
//			if (ac.ptCorrelatorVsCent)	hS_vsCent[2][histoIndex]->Fill(cent, (values[histoIndex] / newProduct), 1.0);
//		}
//		if(histoIndex != size - 1)	histoIndex++;
//
//		if(depth != 0)	fillNormalizedPTValues(depth - 1, i, newProduct, values, mult, cent);
//	}
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::fillNormalizedPTValues(...) Completed." << endl;
}
//
void PTHistos::fillNormalizedPTValues( int depth, int partIndex, double product, TH1 *** values, int* reorder, int*  nBin)
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::fillNormalizedPTValues(...) Starting." << endl;
//	Configuration & ac = *(Configuration*) getConfiguration();
//
//	for(int i = partIndex; i < maxOrder; i++)
//	{
//
//
//		double newProduct = product * avgpT[i];
//
//		for(int iBin = 1; iBin <=nBin[0]; iBin++)
//		{
//			if(values[0][reorder[histoIndex]]->GetBinContent(iBin) != 0)
//			{
//				hC[2][histoIndex]->SetBinContent(iBin, totEvents * values[0][reorder[histoIndex]]->GetBinContent(iBin) / newProduct);
//				double err = values[0][reorder[histoIndex]]->GetBinContent(iBin)/newProduct * ( values[0][reorder[histoIndex]]->GetBinError(iBin)/values[0][reorder[histoIndex]]->GetBinContent(iBin));
//				hC[2][histoIndex]->SetBinError(iBin, totEvents * err);
//			}
//		}
//		if (ac.ptCorrelatorVsMult)
//		{
//			for(int iBin = 1; iBin <=nBin[1]; iBin++)
//			{
//				if(values[1][reorder[histoIndex]]->GetBinContent(iBin) != 0)
//				{
//					hC_vsMult[2][histoIndex]->SetBinContent(iBin, totEvents * values[1][reorder[histoIndex]]->GetBinContent(iBin) / newProduct);
//					double err = values[1][reorder[histoIndex]]->GetBinContent(iBin)/newProduct * ( values[1][reorder[histoIndex]]->GetBinError(iBin)/values[1][reorder[histoIndex]]->GetBinContent(iBin));
//					hC_vsMult[2][histoIndex]->SetBinError(iBin, totEvents * err);
//				}
//			}
//		}
//		if (ac.ptCorrelatorVsCent)
//		{
//			for(int iBin = 1; iBin <=nBin[2]; iBin++)
//			{
//				if(values[2][reorder[histoIndex]]->GetBinContent(iBin) != 0)
//				{
//					hC_vsCent[2][histoIndex]->SetBinContent(iBin, totEvents * values[2][reorder[histoIndex]]->GetBinContent(iBin) / newProduct);
//					double err = values[1][reorder[histoIndex]]->GetBinContent(iBin)/newProduct * ( values[2][reorder[histoIndex]]->GetBinError(iBin)/values[2][reorder[histoIndex]]->GetBinContent(iBin));
//					hC_vsCent[2][histoIndex]->SetBinError(iBin, totEvents * err);
//				}
//			}
//		}
//
//		if(histoIndex != size - 1)	histoIndex++;
//
//		if(depth != 0)	fillNormalizedPTValues(depth - 1, i, newProduct, values, reorder, nBin);
//	}
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::fillNormalizedPTValues(...) Completed." << endl;
}

///////////////////////////////////////////
//calculate the cumulants of the moments
//////////////////////////////////////////
void PTHistos::calculateCumulants(TProfile ** SHistos, TH1 **CHistos, int nBins, double min, double max)
{	
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calculateCumulants(...) Starting." << endl;
//
//	//HeavyIonConfiguration & ac = (HeavyIonConfiguration&)( *getConfiguration());
//
//	TProfile ** newSHistos = new TProfile *[size];
//	int counter = 0;
//	for(int iOrd = 1; iOrd <= maxOrder; iOrd++)
//	{
//		for(int iHisto = 0; iHisto < size; iHisto++)
//		{
//			if(orders[iHisto] == iOrd)
//			{
//				newSHistos[counter] = SHistos[iHisto];
//				counter++;
//
//			}
//		}
//	}
//
//	int maxSize = TMath::Factorial( maxOrder);
//	double * used = new double[maxSize];
//	for(int iBin = 1; iBin <=nBins; iBin++)
//	{
//		for(int iHisto = 0; iHisto < size; iHisto++)
//		{
//			double sum = 0;
//			double absESq = 0;
//
//			int len = 0;
//			int * ind = convert(iHisto, len);
//
//			int curInd = 0;
//			int * set = new int[len];
//			for(int i = 0; i < len; i++)
//			{
//				set[i] = i+1;
//			}
//
//			calcRecSum( CHistos, iBin, absESq, 0, ind, set, len, set, len, 1, used, curInd, 1, sum );
//
//			TString name = nBins * size + iHisto;
//
//			if(iBin == 1) CHistos[iHisto] = new TH1F(name, name, nBins, min, max);
//
//			double content = newSHistos[iHisto]->GetBinContent(iBin) - sum;
//			double error = sqrt(newSHistos[iHisto]->GetBinError(iBin) * newSHistos[iHisto]->GetBinError(iBin) + absESq);
//
//			if(newSHistos[iHisto]->GetBinContent(iBin) != 0)
//			{
//				CHistos[iHisto]->SetBinContent(iBin, content);
//				CHistos[iHisto]->SetBinError(iBin, error);
//			}
//			//if (abs(CHistos[iHisto]->GetBinContent(iBin)) < pow(10, -15)) CHistos[iHisto]->SetBinContent(iBin, 0); // doubles are accurate to 15 decimal places
//
//			//ind is now uneccesary
//			delete[] ind;
//
//		}
//	}
//
//	return;
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calculateCumulants(...) Completed." << endl;

}

///////////////////////////////////////////////////
//used to calculate a recursive partition sum used in calculation of the cumulants
// ex: 	if the highest level cumulant corresponds to C(1,2,3), this function calculates sum = C(1, 2) * C(3) + C(1, 3) * C(2) + C(2, 3) * C(1) + C(1) * C(2) * C(3)
//	   	At the first level, it starts with subset {3}, and gets C(3) * C(1, 2) and adds it to sum and places it on the used array
//	   	It then proceeds to recurse on the proper subsets of {2, 3} and multiply these by C(3), so it gets C(1) * C(2) * C(3) and adds it to sum and places it on the used array
//	   	Going back to the top level, it goes to subset {2} and gets C(2) * C(1, 3) and adds it to sum and places it on the used array
//		It then proceeds to recurse on the proper subsets of {1, 3} and multiply these by C(2), so it gets C(1) * C(2) * C(3) and finds it on the used array so skips
//		Going back to the top level, it goes to subset {2, 3} and gets C(2, 3) * C(1) and adds it to sum and places it on the used array
//		It then proceeds to recurse on the proper subsets of {1} of which there are none
// 		so on...
//     	After this function concludes, C(1, 2, 3) = S(1, 2, 3) - sum
// note that C(1) = S(1) because sum = 0
///////////////////////////////////////////////////
void PTHistos::calcRecSum(TH1 **CHistos, int iBin, double& absESq, double curRelESq, int* iHisto, int* Subset, int len,  int * set, int lenSet, double productC, double* used, int& curInd, int productS, double& sum)
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calcRecSum(...) Starting." << endl;
//	int lenSub = 0;
//	int lenCompSub = 0;
//	if(len != 1)
//	{
//		char* bin = new char[len];
//		int* subset;
//		int * comp;
//		int * compSubset;
//
//		// loop over all possible proper subsets
//		for(int iSubset = 1; iSubset < pow(2, len) - 1; iSubset++)
//		{
//			//get the subset corresponding to iSubset, multiply it by the product(which contains cumulant terms from previous recursive levels)
//			convertToBinary(iSubset, bin, len);
//			subset = getSubset(bin, Subset, len, lenSub);
//			int * s = getSubset(bin, iHisto, len, lenSub);
//
//			int iSub = convert(s, lenSub);
//			double tempProduct = productC * CHistos[iSub]->GetBinContent(iBin);
//			double tempRelErr = curRelESq + CHistos[iSub]->GetBinError(iBin) * CHistos[iSub]->GetBinError(iBin) / CHistos[iSub]->GetBinContent(iBin) / CHistos[iSub]->GetBinContent(iBin);
//
//			//One of the terms in the cumulant corresponds to the product of the subset cumulant and the complementary subset cumulant. The remaining terms are further partitions which are counted when recursed
//			comp = getComplementarySubset(bin, Subset, len, lenCompSub);
//			compSubset = getComplementarySubset(bin, iHisto, len, lenCompSub);
//			int iComp = convert(compSubset, lenCompSub);
//
//			double tempProduct2 = tempProduct * CHistos[iComp]->GetBinContent(iBin);
//			double tempRelErr2  = tempRelErr  + CHistos[iComp]->GetBinError(iBin) * CHistos[iComp]->GetBinError(iBin) / CHistos[iComp]->GetBinContent(iBin) / CHistos[iComp]->GetBinContent(iBin);
//
//			int tempProduct3 = productS * getSubsetNumber(subset, lenSub, set, lenSet);
//			int tempProduct4 = tempProduct3 * getSubsetNumber(comp, lenCompSub, set, lenSet);
//
//			//check to make sure that no terms are counted more than once( ex. C(1) * C(2,3) can be counted for the subset {1} and {2,3} )
//			bool accept = true;
//			for(int i = 0; i < curInd; i++)
//			{
//				accept = accept && !(used[i] == tempProduct4);
//			}
//			if(accept)
//			{
//				sum += tempProduct2;
//				absESq += tempProduct2 * tempProduct2 * tempRelErr2;
//				used[curInd] = tempProduct4;
//				curInd++;
//			}
//
//			delete [] subset;
//			delete[] s;
//			//partition the complementary subset into smaller subsets and multiply them by tempProduct and add to the sum
//			calcRecSum(CHistos, absESq, tempRelErr, iBin, compSubset, comp, lenCompSub, set, lenSet, tempProduct, used, curInd, tempProduct3, sum);
//
//			delete [] comp;
//			delete[] compSubset;
//
//
//
//		}
//	}
//
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calcRecSum(...) Completed." << endl;
//	return;
	
}



void PTHistos::calculateInclusivePtAverage(bool *** acceptances, int * numParticles, double ** pT)
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calculateInclusivePtAverage(...) Starting." << endl;
//
//	for(int iFilter = 0; iFilter < maxOrder; iFilter++)
//	{
//		double sumPt = 0;
//		double totParts = 0;
//		for(int iEvent = 0; iEvent < totEvents; iEvent++)
//		{
//			for (int iParticle = 0; iParticle < numParticles[iEvent]; ++iParticle)
//			{
//				if(acceptances[iEvent][iFilter][iParticle])
//				{
//					sumPt += pT[iEvent][iParticle];
//					totParts += 1;
//				}
//			}
//		}
//		avgpT[iFilter] = sumPt/totParts;
//	}
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calculateInclusivePtAverage(...) Completed." << endl;

}

void PTHistos::calculateInclusiveYieldsAverage(bool *** acceptances, int * numParticles)
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calculateInclusiveYieldsAverage(...) Starting." << endl;
//
//	int ** tempCounts = new int *[totEvents];
//	double * tempAvgCounts = new double [size];
//	for(int iEvent = 0; iEvent < totEvents; iEvent++)
//	{
//		int *n = new int [maxOrder]();
//		int counter = 0;
//		tempCounts[iEvent] = new int [size]();
//		counts[iEvent] = new int [size];
//		for(int iFilter = 0; iFilter < maxOrder; iFilter++)
//		{
//			for(int iParticle = 0; iParticle < numParticles[iEvent]; iParticle++ )
//			{
//				if(acceptances[iEvent][iFilter][iParticle]) n[iFilter]++;
//			}
//		}
//
//		for(int iFilter1 = 0; iFilter1 < maxOrder; iFilter1++)
//		{
//			if(n[iFilter1] > 0 ) tempCounts[iEvent][counter] = n[iFilter1];
//			tempAvgCounts[counter] += tempCounts[iEvent][counter];
//			counter++;
//		}
//
//		for(int iFilter1 = 0; iFilter1 < maxOrder; iFilter1++)
//		{
//			for(int iFilter2 = iFilter1; iFilter2 < maxOrder; iFilter2++)
//			{
//				int same1 = iFilter2 == iFilter1? 1 : 0;
//				if(n[iFilter1] > 0 && (n[iFilter2] - same1) > 0) tempCounts[iEvent][counter] = n[iFilter1]*(n[iFilter2] - same1);
//				tempAvgCounts[counter] += tempCounts[iEvent][counter];
//				counter++;
//			}
//		}
//
//		for(int iFilter1 = 0; iFilter1 < maxOrder; iFilter1++)
//		{
//			for(int iFilter2 = iFilter1; iFilter2 < maxOrder; iFilter2++)
//			{
//				int same12 = iFilter2 == iFilter1? 1 : 0;
//				for(int iFilter3 = iFilter2; iFilter3 < maxOrder; iFilter3++)
//				{
//					int same13 = iFilter3 == iFilter1? 1 : 0;
//					int same23 = iFilter2 == iFilter3? 1 : 0;
//					if(n[iFilter1] > 0 && (n[iFilter2] - same12) > 0 && (n[iFilter3] - same13 - same23) > 0) tempCounts[iEvent][counter] = n[iFilter1]*(n[iFilter2] - same12)*(n[iFilter3] - same13 - same23);
//					tempAvgCounts[counter] += tempCounts[iEvent][counter];
//					counter++;
//				}
//			}
//		}
//
//		for(int iFilter1 = 0; iFilter1 < maxOrder; iFilter1++)
//		{
//			for(int iFilter2 = iFilter1; iFilter2 < maxOrder; iFilter2++)
//			{
//				for(int iFilter3 = iFilter2; iFilter3 < maxOrder; iFilter3++)
//				{
//					int same12 = iFilter2 == iFilter1? 1 : 0;
//					int same13 = iFilter3 == iFilter1? 1 : 0;
//					int same23 = iFilter2 == iFilter3? 1 : 0;
//					for(int iFilter4 = iFilter3; iFilter4 < maxOrder; iFilter4++)
//					{
//						int same14 = iFilter4 == iFilter1? 1 : 0;
//						int same24 = iFilter2 == iFilter4? 1 : 0;
//						int same34 = iFilter3 == iFilter4? 1 : 0;
//						if(n[iFilter1] > 0 && (n[iFilter2] - same12) > 0 && (n[iFilter3] - same13 - same23) > 0 && (n[iFilter4] - same14 - same24 - same34) > 0) tempCounts[iEvent][counter] = n[iFilter1]*(n[iFilter2] - same12)*(n[iFilter3] - same13 - same23)*(n[iFilter4] - same14 - same24 - same34);
//						tempAvgCounts[counter] += tempCounts[iEvent][counter];
//						counter++;
//					}
//				}
//			}
//		}
//		delete [] n;
//	}
//
//	for (int i = 0; i < size; ++i)
//	{
//		tempAvgCounts[i] /= totEvents;
//	}
//
//	for(int iEvent = 0; iEvent < totEvents; iEvent++)
//	{
//		for(int iHisto = 0; iHisto < size; iHisto++)
//		{
//			counts[iEvent][iHisto] = tempCounts[iEvent][reorder[iHisto]];
//		}
//	}
//
//	for(int iHisto = 0; iHisto < size; iHisto++)
//	{
//		avgCounts[iHisto] = tempAvgCounts[reorder[iHisto]];
//	}
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calculateInclusiveYieldsAverage(...) Completed." << endl;

}

void PTHistos::calculateEventMoments(bool *** acceptances, int * numParticles, double ** pT)
{
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calculateEventMoments(...) Starting." << endl;
//
//	double ** tempSValues = new double *[totEvents];
//	for(int iEvent = 0; iEvent < totEvents; iEvent++)
//	{
//		double *n1 = new double [maxOrder]();
//		double *n2 = new double [maxOrder]();
//		double *n3 = new double [maxOrder]();
//		double *n4 = new double [maxOrder]();
//		int counter = 0;
//		tempSValues[iEvent] = new double [size]();
//		SValues[iEvent] = new double [size];
//		for(int iFilter = 0; iFilter < maxOrder; iFilter++)
//		{
//			for(int iParticle = 0; iParticle < numParticles[iEvent]; iParticle++ )
//			{
//				if(acceptances[iEvent][iFilter][iParticle])
//				{
//					n1[iFilter]+= pT[iEvent][iParticle] - avgpT[iFilter];
//					n2[iFilter]+= (pT[iEvent][iParticle] - avgpT[iFilter]) * (pT[iEvent][iParticle] - avgpT[iFilter]);
//					n3[iFilter]+= (pT[iEvent][iParticle] - avgpT[iFilter]) * (pT[iEvent][iParticle] - avgpT[iFilter]) * (pT[iEvent][iParticle] - avgpT[iFilter]);
//					n4[iFilter]+= (pT[iEvent][iParticle] - avgpT[iFilter]) * (pT[iEvent][iParticle] - avgpT[iFilter]) * (pT[iEvent][iParticle] - avgpT[iFilter]) * (pT[iEvent][iParticle] - avgpT[iFilter]);
//				}
//			}
//		}
//
//		for(int iFilter1 = 0; iFilter1 < maxOrder; iFilter1++)
//		{
//			tempSValues[iEvent][counter] = n1[iFilter1];
//			counter++;
//		}
//
//		for(int iFilter1 = 0; iFilter1 < maxOrder; iFilter1++)
//		{
//			for(int iFilter2 = iFilter1; iFilter2 < maxOrder; iFilter2++)
//			{
//				double same12 = iFilter2 == iFilter1? n2[iFilter1] : 0;
//				tempSValues[iEvent][counter] = n1[iFilter1] * n1[iFilter2] - same12;
//				counter++;
//			}
//		}
//
//		for(int iFilter1 = 0; iFilter1 < maxOrder; iFilter1++)
//		{
//			for(int iFilter2 = iFilter1; iFilter2 < maxOrder; iFilter2++)
//			{
//				int same12 = iFilter2 == iFilter1? 1 : 0;
//				for(int iFilter3 = iFilter2; iFilter3 < maxOrder; iFilter3++)
//				{
//					if(iFilter1 == iFilter2)
//					{
//						if(iFilter1 == iFilter3) tempSValues[iEvent][counter] = n1[iFilter1] * n1[iFilter1] * n1[iFilter1] - 3 * n2[iFilter1] * n1[iFilter1] + 2 * n3[iFilter1];
//						else tempSValues[iEvent][counter] = (n1[iFilter1] * n1[iFilter1] - n2[iFilter1] ) * n1[iFilter3];
//					}
//					else
//					{
//						if(iFilter2 == iFilter3) tempSValues[iEvent][counter] = (n1[iFilter2] * n1[iFilter2] - n2[iFilter2] ) * n1[iFilter1];
//						else tempSValues[iEvent][counter] = n1[iFilter1] * n1[iFilter2] * n1[iFilter3];
//					}
//					counter++;
//				}
//			}
//		}
//
//		for(int iFilter1 = 0; iFilter1 < maxOrder; iFilter1++)
//		{
//			for(int iFilter2 = iFilter1; iFilter2 < maxOrder; iFilter2++)
//			{
//				for(int iFilter3 = iFilter2; iFilter3 < maxOrder; iFilter3++)
//				{
//					for(int iFilter4 = iFilter3; iFilter4 < maxOrder; iFilter4++)
//					{
//						if(iFilter1 == iFilter2)
//						{
//							if(iFilter1 == iFilter3)
//							{
//								if(iFilter1 == iFilter4)  tempSValues[iEvent][counter] = n1[iFilter2] * n1[iFilter1] * n1[iFilter1] * n1[iFilter1] - 6 * n1[iFilter1] * n1[iFilter1] * n2[iFilter1] + 8 * n1[iFilter1] *  n3[iFilter1] + 3 * n2[iFilter1] *  n2[iFilter1] - 6 *  n4[iFilter1];
//								else tempSValues[iEvent][counter] = (n1[iFilter1] * n1[iFilter1] * n1[iFilter1] - 3 * n2[iFilter1] * n1[iFilter1] + 2 * n3[iFilter1]) * n1[iFilter4];
//							}
//							else
//							{
//								if(iFilter3 == iFilter4) tempSValues[iEvent][counter] = (n1[iFilter1] * n1[iFilter1] - n2[iFilter1] ) * (n1[iFilter3] * n1[iFilter3] - n2[iFilter3] );
//								else tempSValues[iEvent][counter] = (n1[iFilter1] * n1[iFilter1] - n2[iFilter1] ) * n1[iFilter3] * n1[iFilter4];
//							}
//						}
//						else
//						{
//							if(iFilter2 == iFilter3)
//							{
//								if(iFilter2 == iFilter4) tempSValues[iEvent][counter] = (n1[iFilter2] * n1[iFilter2] * n1[iFilter2] - 3 * n2[iFilter2] * n1[iFilter2] + 2 * n3[iFilter2]) * n1[iFilter1];
//								else tempSValues[iEvent][counter] = (n1[iFilter2] * n1[iFilter2] - n2[iFilter2] ) * n1[iFilter1] * n1[iFilter4];
//							}
//							else
//							{
//								if(iFilter3 == iFilter4) tempSValues[iEvent][counter] = (n1[iFilter3] * n1[iFilter3] - n2[iFilter3] ) * n1[iFilter1] * n1[iFilter2];
//								else tempSValues[iEvent][counter] = n1[iFilter1] * n1[iFilter2] * n1[iFilter3] * n1[iFilter4];
//							}
//						}
//						counter++;
//					}
//				}
//			}
//		}
//		delete [] n1;
//		delete [] n2;
//		delete [] n3;
//		delete [] n4;
//	}
//
//	for(int iEvent = 0; iEvent < totEvents; iEvent++)
//	{
//		for(int iHisto = 0; iHisto < size; iHisto++)
//		{
//			SValues[iEvent][iHisto] = tempSValues[iEvent][reorder[iHisto]];
//		}
//	}
//
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::calculateEventMoments(...) Completed." << endl;
}
////////////////////////////////////////////////////////////////////////////
//Helper Functions
////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////
// convert a base maxOrder integer (represented by num) into the index of the corresponding moment in the array
// ex: for 4th order correlations, the function {1,2,3} is index 19 in the array (1, 2, 3, 4, 11, 12 ... 44, 111, ... 123)
// checked for correctness
///////////////////////////////////////
int PTHistos::convert(int * num, int len)
{
//	int convert = (TMath::Factorial( maxOrder + len - 1)) / (TMath::Factorial(maxOrder ) * TMath::Factorial(len - 1 )) - 1;
//	for(int i = 0; i < len; i++)
//	{
//		for(int j = (i == 0)? 0: num[i - 1] - 1; j < num[i] - 1; j++)
//		{
//			convert += (TMath::Factorial( maxOrder + len - j - i - 2)) / (TMath::Factorial(maxOrder - j - 1 ) * TMath::Factorial(len - i - 1 ));
//		}
//	}
  return 0;//convert;
}

//////////////////////////////////////
// convert the index of the moment (represented by num) into a base maxOrder integer
// ex: for 4th order correlations, the function {1,2,3} is index 19 in the array (1, 2, 3, 4, 11, 12 ... 44, 111, ... 123)
//////////////////////////////////////
int * PTHistos::convert(int num, int & len)
{
  return 0;

//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::convert(...) started." << endl;
//	for(len = 2; len <=maxOrder + 1; len++)
//	{
//		int temp = (TMath::Factorial( maxOrder + len - 1)) / (TMath::Factorial(maxOrder ) * TMath::Factorial(len - 1 )) - 1 ;
//		if(num < temp)
//		{
//			len--;
//			num -= (TMath::Factorial( maxOrder + len - 1)) / (TMath::Factorial(maxOrder ) * TMath::Factorial(len - 1 )) - 1 ;
//			break;
//		}
//	}
//
//	int * convert = new int[len];
//
//	for(int i = 0; i <len; i++)
//	{
//		int temp = 0;
//		for(convert[i] = (i == 0)? 1: convert[i - 1] ; convert[i] <= maxOrder; convert[i]++)
//		{
//			int temp2 = (TMath::Factorial( maxOrder + len - (convert[i] - 1) - i - 2)) / (TMath::Factorial(maxOrder - (convert[i] - 1) - 1 ) * TMath::Factorial(len - i - 1 ));
//			temp += temp2;
//			if(num < temp)
//			{
//				num -=  (temp - temp2);
//				break;
//			}
//		}
//	}
//	if (reportDebug(__FUNCTION__))  cout << "PTHistos::convert(...) Completed." << endl;
//	return convert;
}

//////////////////////////////////////////////
//convert num to a binary string of length len (including leading 0's)
// checked for correctness
////////////////////////////////////////////////
void PTHistos::convertToBinary(int num, char*str, int len )
{
	for(; len > 0; len --)
	{
		str[len - 1] = ((num%2 == 1) ? '1':'0');
		num /= 2;
	}
}	


///////////////////////////////////////
// get the subset corresponding to the binary string bin, from set
// ex: the subset 001101 of [1, 1, 2, 3, 3, 3] is [2, 3, 3]
// note that because set is guaranteed to be ordered, so is subset
//////////////////////////////////////
int * PTHistos::getSubset(char* subset, int * set, int len, int& lenSub)
{
	lenSub = 0;
	for(int i = 0; i < len; i ++)
	{
		if(subset[i] == '1') lenSub++;
	}

	int * temp = new int[lenSub];

	lenSub = 0;
	for(int i = 0; i < len; i ++)
	{
		if(subset[i] == '1') 
		{
			temp[lenSub] = set[i];
			lenSub++;
		}
	}
	return temp;
}


///////////////////////////////////////
// get the complementary subset corresponding to the binary string bin, from set
// ex: the complementary subset 001101 of [1, 1, 2, 3, 3, 3] is [1, 1, 3]
// note that because set is guaranteed to be ordered, so is the complementary subset
//////////////////////////////////////
int * PTHistos::getComplementarySubset(char* subset, int * set, int len, int& lenSub)
{
	lenSub = 0;
	for(int i = 0; i < len; i ++)
	{
		if(subset[i] == '0') lenSub++;
	}

	int * temp = new int[lenSub];

	lenSub = 0;
	for(int i = 0; i < len; i ++)
	{
		if(subset[i] == '0') 
		{
			temp[lenSub] = set[i];
			lenSub++;
		}
	}
	return temp;
}

////////////////////////////////////////////////////////////
// get the subset number of subset in mainset
// assumes all elements of mainset are distinct
// checked for correctness
////////////////////////////////////////////////////////////
int PTHistos::getSubsetNumber(int * subset, int lenSub, int * mainset, int lenSet)
{
	int num = 0;
	for(;lenSub > 0; lenSub--)
	{
		for(int i = 0; i<lenSet; i++)
		{
			if(subset[lenSub - 1] == mainset[i]) num += pow(2, lenSet - i - 1);
		}
	}
	return num ;
}
