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

#include "PartitionCalculator.hpp"

ClassImp(PartitionCalculator);

PartitionCalculator::PartitionCalculator()
{
}

void PartitionCalculator::computePartitions(TH1* h, vector<double>& fractions, vector<double>& boundaries)
{
  unsigned int nPartitions = -1 + fractions.size();
  cout << "PartitionCalculator::computePartitions(..)" << endl;
  cout << "Computing nPartitions: " << nPartitions << endl;
  if (nPartitions<2)
    {
    cout << "Are you sure about this??? Aborting." << endl;
    return;
    }
  double sum   = 0.0;
  double count = 0.0;
  double frac, content, lowEdge, width;
  int nBins = h->GetNbinsX();
  for (int i=1; i<=nBins; i++) sum +=  h->GetBinContent(i);
  boundaries.push_back( h->GetBinLowEdge(1) );
  unsigned int iFrac = 1;
  for (int i=1; i<=nBins; i++)
  {

  lowEdge  = h->GetBinLowEdge(i);
  width    = h->GetBinWidth(i);
  content  = h->GetBinContent(i);
  count    += content;
  frac     = count/sum;
  //cout << content << "    "  << count << "     " << frac << "    " << edge << endl;
  if (frac>=fractions[iFrac])
    {
    double excessFrac = (frac - fractions[iFrac]);
    double adjustedEdge = lowEdge - excessFrac*width;
    cout << frac  << "   " << lowEdge <<  "   " << adjustedEdge << endl;
    iFrac++;
    if (iFrac==fractions.size()) break;
    boundaries.push_back( adjustedEdge );
    }
  }
  boundaries.push_back( h->GetXaxis()->GetXmax () );
  cout << "PartitionCalculator::computePartitions(..) Partitions" << endl;
  cout << "Computed nPartitions: " << -1 + boundaries.size() << endl;

  for (unsigned int k=0; k<boundaries.size()-1; k++)
    {
    cout << "  " << k << "  [" << fractions[k] << "-" <<  fractions[k+1] << "] =  ["  << boundaries[k] << "-" <<  boundaries[k+1] << "] " << endl;
    }

}

