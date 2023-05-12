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
#include "ParticleDbParser.hpp"
#include "Configuration.hpp"
using CAP::ParticleDbParser;

ClassImp(CAP::ParticleDbParser);


namespace CAP
{

ParticleDbParser::ParticleDbParser()
:
Parser()
{ }

ParticleDbParser::ParticleDbParser(MessageLogger::Severity _severity)
:
Parser(_severity)
{ }

int  ParticleDbParser::readChunShen(ParticleDb    & _particleDb,
                      const String  & _inputPath,
                      const String  & _inputFileName,
                      const String  & _extension)
{
  ifstream * inputDbFile = openInputAsciiFile(particleDbImportPath,particleDbImportFile,".dat");
  if (!inputDbFile)
    {
    if (reportError(__FUNCTION__))
      {
      cout << "Could not open ParticleDb file:" << particleDbImportPath << particleDbImportFile << endl;
      postTaskError();
      return;
      }
    }
  ifstream & inputFile = *inputDbFile;

  string name, title;
  double mass, width, decayBranchingRatio;
  int pdgCode, gSpin, gIsospin, baryonNumber, strangeNumber, charm, bottom, charge;
  int nDecayModes, decayNpart, dummy_int;
  int decayPart[5] = {0, 0, 0, 0, 0};
  ParticleType * particleType;
  ParticleType * antiParticleType;
  String theName;
  String theAntiName;
  String theTitle;
  String theAntiTitle;

  ParticleDb * particleDb = new ParticleDb();
  ParticleDb::setDefaultParticleDb(particleDb);

  while (1)
    {
    if (reportDebug(__FUNCTION__))
      cout << "Reading in ParticleType." << endl;
    inputFile >> pdgCode;
    if (inputFile.eof())
      {
      if (reportDebug(__FUNCTION__))
        cout << "Reached EOF." << endl;
      break;
      }
    inputFile >> name;
    inputFile >> mass;
    inputFile >> width;
    inputFile >> gSpin;
    inputFile >> baryonNumber;
    inputFile >> strangeNumber;
    inputFile >> charm;
    inputFile >> bottom;
    inputFile >> gIsospin;
    inputFile >> charge;
    inputFile >> nDecayModes;
    inputFile >> title;
    theName  = name;
    theTitle = title;
    particleType= new ParticleType(pdgCode, theName, theTitle, mass, width, gSpin, baryon, strangeNumber,
                                   charm, bottom, gIsospin, charge);
    particleDb->addParticleType(particleType);

    if (particleType->isFermion())
      {
      theAntiName  = "Anti-";
      theAntiName  += theName;
      theAntiTitle = "#bar ";
      theAntiTitle += theTitle;
      antiParticleType =  new ParticleType(-pdgCode, theAntiName, theAntiTitle, mass, width, gSpin,
                                           -baryon, -strangeNumber, -charm, -bottom, gIsospin,
                                           -charge);
      particleDb->addParticleType(antiParticleType);
      }
    // read decay information
    for (int j = 0; j < nDecayModes; j++)
      {
      inputFile >> dummy_int;
      inputFile >> decayNpart;
      inputFile >> decayBranchingRatio;
      inputFile >> decayPart[0];
      inputFile >> decayPart[1];
      inputFile >> decayPart[2];
      inputFile >> decayPart[3];
      inputFile >> decayPart[4];
      decayNpart = abs(decayNpart);
      std::vector<int> children;
      std::vector<int> antiChildren;
      for (int k=0; k<decayNpart; k++)
        {
        children.push_back(decayPart[k]);
        }
      if (!particleType->isFermion())
        {
        particleType->addDecayMode(decayBranchingRatio,children);
        }
      else
        {
        for (int k=0; k<decayNpart; k++)
          {
          antiChildren.push_back(particleDb->findPdgCode(decayPart[k])->getAntiParticlePdgCode());
          }
        particleType->addDecayMode(decayBranchingRatio,children);
        antiParticleType->addDecayMode(decayBranchingRatio,antiChildren);
        }
      }
    }
  //cout << "Reading in ParticleType resonance decay table.. - 5- " << endl;
  inputFile.close();

  if (reportInfo(__FUNCTION__))
    cout << "Total number of particles read: " <<  particleDb->getNumberOfTypes() << endl;
  particleDb->resolveTypes();
  particleDb->sortByMass();
}

}
