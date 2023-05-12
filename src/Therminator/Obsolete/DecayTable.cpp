/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

#include "DecayTable.hpp"

DecayTable::DecayTable()
{
  mDecayChannels.clear();
  mBranchingRatios.clear();
}

DecayTable::DecayTable(const DecayTable& aTable)
{
  mDecayChannels.clear();
  mBranchingRatios.clear();
  for (int tIter=0; tIter<aTable.getChannelCount(); tIter++)
    AddDecayChannel( *(aTable.getDecayChannel(tIter)) );
}

DecayTable::~DecayTable()
{
  mDecayChannels.clear();
  mBranchingRatios.clear();
}

void DecayTable::AddDecayChannel(DecayChannel aChannel)
{
  mDecayChannels.push_back(aChannel);
  RecalculateBranchingRatios();
}

void DecayTable::RecalculateBranchingRatios()
{
  float tSumRatio = 0.0;
  float tCurRatio = 0.0;

  for (int tIter=0; tIter<mDecayChannels.size(); tIter++)
    tSumRatio += mDecayChannels[tIter].getBranchingRatio();

  for (int tIter=0; tIter<mDecayChannels.size(); tIter++) {
    tCurRatio += mDecayChannels[tIter].getBranchingRatio() / tSumRatio;
    if (mBranchingRatios.size() <= tIter)
      mBranchingRatios.push_back(tCurRatio);
    else
      mBranchingRatios[tIter] = tCurRatio;
  }
}

int DecayTable::getChannelCount() const
{
  return mDecayChannels.size() - 1;
}


const DecayChannel* DecayTable::getDecayChannel(int aIndex) const
{
  return &(mDecayChannels[aIndex]);
}

float DecayTable::getDecayStep(int aIndex)
{
  return mBranchingRatios[aIndex];
}

int DecayTable::ChooseDecayChannel(double aProb)
{
  int tChanIndex = 0;
  while ((mBranchingRatios[tChanIndex] < aProb) && (tChanIndex < mDecayChannels.size()))
    tChanIndex++;

  return tChanIndex;
}

int DecayTable::ChooseDecayChannelOrNot(double aProb)
{
  float tSumRatio = 0.0;

  for (int tIter=0; tIter<mDecayChannels.size(); tIter++) {
    if ((aProb > tSumRatio) && (aProb <= tSumRatio+mDecayChannels[tIter].getBranchingRatio()))
      return tIter;
    tSumRatio += mDecayChannels[tIter].getBranchingRatio();
  }
  return -1;
}
