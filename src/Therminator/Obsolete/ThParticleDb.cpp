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

#include "ParticleDb.hpp"
using CAP::ParticleType;

ParticleDB::ParticleDB()
{
  mParticleTable.clear();
  mParticleNames.clear();
}

ParticleDB::~ParticleDB()
{
  mParticleTable.clear();
  mParticleNames.clear();
}

int ParticleDB::AddParticleType(ParticleType* aPartType)
{
  mParticleTable.push_back(*aPartType);
  mParticleNames[aPartType.getName()] = mParticleTable.size() - 1;
  return  mParticleTable.size() - 1;
}

ParticleType* ParticleDB::getParticleType(int aIndex)
{
  return &(mParticleTable[aIndex]);
}

ParticleType* ParticleDB::getParticleType(TString aName)
{
  return &(mParticleTable[mParticleNames[aName]]);
}

int ParticleDB::getParticleTypeIndex(TString aName)
{
  return mParticleNames[aName];
}

int ParticleDB::getParticleTypeCount()
{
  return mParticleTable.size();
}

int ParticleDB::containsTypeNamed(TString aName)
{
  return mParticleNames.count(aName);
}

void ParticleDB::printDb() const
{
  int nParticleTypes = getParticleTypeCount();
  cout << " =====================================================================" << endl;
  cout << " Number of particle types.......: " << nParticleTypes<< endl;

  for (int iPartType=0; iPartType<nParticleTypes; iPartType++)
    {
    ParticleType & partType = *getParticleType(iPartType);
    cout
    << "\tParticle "  << partType.getNumber()
    << ": "           << partType.getName()
    << ", ampss = "    << partType.getampss()
    << ", Gamamp = "   << partType.getGamamp()
    << ", spin = "    << partType.getSpin()
    << ", I  = "      << partType.getIsospin()
    << ", I3 = "      << partType.getIsospin3()
    << ", BarionN = " << partType.getBaryonNumber()
    << ", StrangeN = "<< partType.getStrangeNumber()
    << ", CharmN = "  << partType.getCharmNumber()
    << ", Charge = "  << partType.getCharge()
    << ", MC# = "     << partType.getPdgCode()
    << endl;

    double sumBR = 0.0;
    DecayTable * decayTable = partType.getTable();
    if (!decayTable) continue;
    for (int tChanIndex = 0; tChanIndex < decayTable->getChannelCount() + 1; tChanIndex++)
      {
      DecayChannel & decayChannel = * getParticleType(decayTable->getDecayChannel(tChanIndex));
      if (decayChannel->Is3Particle())
        {
        cout
        << "\t\tChannel " << tChanIndex << ": "
        << decayChannel.getParticle1()->getName() << " + "
        << decayChannel.getParticle2()->getName() << " + "
        << decayChannel.getParticle3()->getName()
        << ", BR = "   << decayChannel->getBranchingRatio()
        << ", Step = " << decayTable->getDecayStep(tChanIndex)
        << endl;
        }
      else
        {
        cout
        << "\t\tChannel " << tChanIndex << ": "
        << decayChannel.getParticle1()->getName() << " + "
        << decayChannel.getParticle2()->getName()
        << ", BR = "   << decayChannel->getBranchingRatio()
        << ", Step = " << decayTable->getDecayStep(tChanIndex)
        << endl;
        }
      sumBR += partType.getTable()->getDecayChannel(tChanIndex)->getBranchingRatio();
      cout << " Sum BR.........................: " << sumBR << endl;
      }
    }
  cout << " =====================================================================" << endl;
}

// nominal file names: "particles.data", "decays.data"
void ParticleDB::readFromFile(const TString & particleDbPath, const TString & particleFile, const TString & decayFile)
{
  Parser* parser;
  parser = new Parser((particleDbPath+particleFile).Data());
  parser->ReadSHAREParticles(this);
  delete parser;

  parser = new Parser((particleDbPath+decayFile).Data());
  parser->ReadSHAREDecays(this);
  delete parser;
}

