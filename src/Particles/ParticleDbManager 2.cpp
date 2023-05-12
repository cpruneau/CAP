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
#include "ParticleDbManager.hpp"
#include <string.h>
using CAP::ParticleDbManager;
using namespace std;

ClassImp(ParticleDbManager);

const double factorials[7] = {1.0, 1.0, 2.0, 6.0, 24.0, 120.0, 720.0 };


ParticleDbManager:: ParticleDbManager(const String & _name,
                                      const Configuration & _configuration)
:
EventTask(_name,_configuration),
particleDbImport(1),
particleDbImportPath(""),
particleDbImportFile("particles.data"),
particleDbImportDecaysFile("decays.data"),
particleDbImportNative(0),
particleDbImportCAP(0),
particleDbConvertCAPToNative(0),
particleDbConvertNativeToCAP(0),
particleDbExport(0),
particleDbExportPath(""),
particleDbExportFile("newParticles.data"),
particleDbExportDecaysFile("neweDecays.data"),
particleDbExportNative(0),
particleDbExportCAP(0),
particleDbs()
{
  appendClassName("ParticleDbManager");
}

void ParticleDbManager::setDefaultConfiguration()
{
  EventTask::setDefaultConfiguration();
  addParameter("ParticleDbImport",            particleDbImport);
  addParameter("ParticleDbImportPath",        particleDbImportPath);
  addParameter("ParticleDbImportFile",        particleDbImportFile);
  addParameter("ParticleDbImportDecaysFile",  particleDbImportDecaysFile);
  addParameter("ParticleDbImportNative",      particleDbImportNative);
  addParameter("ParticleDbImportCAP",         particleDbImportCAP);
  addParameter("ParticleDbConvertCAPToNative",particleDbConvertCAPToNative);
  addParameter("ParticleDbConvertNativeToCAP",particleDbConvertNativeToCAP);
  addParameter("ParticleDbExport",            particleDbExport);
  addParameter("ParticleDbExportPath",        particleDbExportPath);
  addParameter("ParticleDbExportFile",        particleDbExportFile);
  addParameter("ParticleDbExportDecaysFile",  particleDbExportDecaysFile);
  addParameter("ParticleDbExportNative",      particleDbExportNative);
  addParameter("ParticleDbExportCAP",         particleDbExportCAP);
}


void ParticleDbManager::configure()
{
  EventTask::configure();
  particleDbImport           = getValueBool(  "ParticleDbImport"            );
  particleDbImportPath       = getValueString("ParticleDbImportPath"        );
  particleDbImportFile       = getValueString("ParticleDbImportFile"        );
  particleDbImportDecaysFile = getValueString("ParticleDbImportDecaysFile"  );
  particleDbImportNative     = getValueBool(  "ParticleDbImportNative"      );
  particleDbImportCAP        = getValueBool(  "ParticleDbImportCAP"         );
  particleDbExport           = getValueBool(  "ParticleDbExport"            );
  particleDbExportPath       = getValueString("ParticleDbExportPath"        );
  particleDbExportFile       = getValueString("ParticleDbExportFile"        );
  particleDbExportDecaysFile = getValueString("ParticleDbExportDecaysFile"  );
  particleDbExportNative     = getValueBool(  "ParticleDbExportNative"      );
  particleDbExportCAP        = getValueBool(  "ParticleDbExportCAP"         );
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("ParticleDbImport"           ,particleDbImport);
    printItem("ParticleDbImportPath"       ,particleDbImportPath);
    printItem("ParticleDbImportFile"       ,particleDbImportFile);
    printItem("ParticleDbImportDecaysFile" ,particleDbImportDecaysFile);
    printItem("ParticleDbImportNative"     ,particleDbImportNative);
    printItem("ParticleDbImportCAP"        ,particleDbImportCAP);
    printItem("ParticleDbExport"           ,particleDbExport);
    printItem("ParticleDbExportPath"       ,particleDbExportPath);
    printItem("ParticleDbExportFile"       ,particleDbExportFile);
    printItem("ParticleDbExportDecaysFile" ,particleDbExportDecaysFile);
    printItem("ParticleDbExportNative"     ,particleDbExportNative);
    printItem("ParticleDbExportCAP"        ,particleDbExportCAP);
    cout << endl;
    }
  if (reportDebug(__FUNCTION__)) printConfiguration(cout);
}

void ParticleDbManager::execute()
{
  if (reportStart(__FUNCTION__))
;
  incrementTaskExecuted();
  if (particleDbImport)  importParticleDb();
  if (particleDbExport)  initializeParticleDb();
  if (particleDbConvertCAPToNative || particleDbConvertNativeToCAP)
    {
    importParticleDb();
    initializeParticleDb();
    }
  if (reportEnd(__FUNCTION__))
;
}

void ParticleDbManager::importParticleDb()
{
  if (particleDbImportCAP || !particleDbImportNative)
    importParticleDbCAP();
  if (particleDbImportNative)
    importParticleDbNative();
}

void ParticleDbManager::importParticleDbCAP() throw (FileException)
{
  ifstream & inputFile = openInputAsciiFile(particleDbImportPath,particleDbImportFile,".dat");
  string name, title;
  double mass, width, decayBranchingRatio;
  int pdgCode, gSpin, gIsospin, baryonNumber, strangeNumber, charmNumber, bottomNumber, charge;
  int nDecayModes, decayNpart, dummy_int;
  int decayPart[5] = {0, 0, 0, 0, 0};
  int index = 0;
  ParticleType * particleType;
  ParticleType * antiParticleType;
  String theName;
  String theAntiName;
  String theTitle;
  String theAntiTitle;
  ParticleDb * particleDb = new ParticleDb();
  particleDbs.push_back(particleDb);
  ParticleDb::setDefaultParticleDb(particleDb);
  try
  {
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
    inputFile >> charmNumber;
    inputFile >> bottomNumber;
    inputFile >> gIsospin;
    inputFile >> charge;
    inputFile >> nDecayModes;
    inputFile >> title;
    theName  = name;
    theTitle = title;
    particleType= new ParticleType();
    //pdgCode, theName, theTitle, mass, width, gSpin, baryonNumber, strangeNumber,
    //                               charmNumber, bottomNumber, gIsospin, charge);
    particleType->setIndex(index); index++;
    particleDb->addParticleType(particleType);

    if (particleType->isFermion())
      {
      theAntiName  = "Anti-";
      theAntiName  += theName;
      theAntiTitle = "#bar ";
      theAntiTitle += theTitle;
      antiParticleType =  new ParticleType();
      //      -pdgCode, theAntiName, theAntiTitle, mass, width, gSpin,
      //                                           -baryonNumber, -strangeNumber, -charmNumber, -bottomNumber, gIsospin,
      //                                           -charge);
      antiParticleType->setIndex(index); index++;
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
  }
  catch (...)
  {
  throw FileException(particleDbImportPath,particleDbImportFile,"Error reading particle data base file","ParticleDbManager::importParticleDbCAP()");
  }
  inputFile.close();
  if (reportInfo(__FUNCTION__))
    cout << "Total number of particles read: " <<  particleDb->getNumberOfTypes() << endl;
  particleDb->resolveTypes();
  particleDb->sortByMass();
}

void ParticleDbManager::importParticleDbNative()  throw (FileException)
{
  ifstream & inputFile = openInputAsciiFile(particleDbImportPath,particleDbImportFile,".data");
  ParticleDb * particleDb = new ParticleDb();
  particleDbs.push_back(particleDb);
  ParticleDb::setDefaultParticleDb(particleDb);
  ParticleType * particleType;
  char   buff[300];
  char   name[20];
  double mass, width, spin, isospin, isospin3, Nq, Ns, Naq, Nas, Nc, Nac, pdgCode;
  int    index = 0;
  try
  {
  while (!inputFile.eof())
    {
    inputFile.getline(buff,300);
    if (!(*buff) || (*buff == '#'))
      {
//      cout << "Buffer:" << buff << endl;
//      cout << "Skip comment" << endl;
      continue;
      }
    istringstream * iss = new istringstream(buff);
    (*iss) >> name >> mass >> width >> spin >> isospin >> isospin3 >> Nq >> Ns >> Naq >> Nas >> Nc >> Nac >> pdgCode;
    index++;
//    if (reportDebug(__FUNCTION__))
//      {
//      cout << '\t'<<index<<" "<<name<<" "<<mass<<" "<<width<<" "<<spin<<" "<<isospin<<" "<<isospin3<<" "<<Nq<<" "<<Naq<<" "<<Ns<<" "<<Nas<<" "<<Nc<<" "<<Nac<<" "<<pdgCode << endl;
//      }
    particleType = new ParticleType();
    particleType->setIndex(index);
    particleType->setName(name);
    particleType->setTitle(name);
    particleType->setMass(mass);
    particleType->setWidth(width);
    particleType->setSpin(spin);
    particleType->setIsospin(isospin);
    particleType->setIsospin3(isospin3);
    particleType->setNumberQ(static_cast<int>  (Nq));
    particleType->setNumberAQ(static_cast<int> (Naq));
    particleType->setNumberS(static_cast<int>  (Ns));
    particleType->setNumberAS(static_cast<int> (Nas));
    particleType->setNumberC(static_cast<int>  (Nc));
    particleType->setNumberAC(static_cast<int> (Nac));
    particleType->setNumberB(static_cast<int>  (0));
    particleType->setNumberAB(static_cast<int> (0));
    particleType->setPdgCode(static_cast<int>  (pdgCode));
    particleDb->addParticleType(particleType);
    delete iss;
    }
  }
  catch (...)
  {
  throw FileException(particleDbImportPath,particleDbImportFile,"Error reading particle data base file","ParticleDbManager::importParticleDbNative()");
  }
  inputFile.close();
  ifstream & inputFileDecays = openInputAsciiFile(particleDbImportPath,particleDbImportDecaysFile,".data");
  double tBRatio, tRatio;
  int    CGcoeff; // complete branching ratio by Clebsch-Gordan coefficient: 0-no 1-yes
  while (!inputFileDecays.eof())
    {
    inputFileDecays.getline(buff,300);
    if (!(*buff) || (*buff == '#'))
      {
//      cout << "Buffer:" << buff << endl;
//      cout << "Skip comment" << endl;
      continue;
      }
    char   parentName[20], childName1[20], childName2[20], childName3[20], childName4[20];
    istringstream * isss = new istringstream(buff);
    *isss >> parentName >> childName1 >> childName2 >> childName3 >> childName4 >> tBRatio >> CGcoeff;;
    //cout << parentName << "  " << childName1 << "  " << childName2 << "  " << childName3 << "  " <<  childName4 <<"  " << tBRatio << "  " <<CGcoeff <<  endl;;
    delete isss;

    ParticleType * parentType = nullptr;
    ParticleType * childType1 = nullptr;
    ParticleType * childType2 = nullptr;
    ParticleType * childType3 = nullptr;
    ParticleType * childType4 = nullptr;
    double mass0 = 0;
    double mass1 = 0;
    double mass2 = 0;
    double mass3 = 0;
    double mass4 = 0;

    parentType = particleDb->getParticleType(parentName);
    if (!parentType)
      {
      if (reportError(__FUNCTION__))
        {
        cout << "Did not find the father particle named: " << parentName << endl;
        }
      continue;
      }
    mass0 = parentType->getMass();
    //cout << "Parent:" << parentType->getName() << endl;
    childType1 = particleDb->getParticleType(childName1);
    if (!childType1)
      {
      if (reportError(__FUNCTION__))
        {
        cout << "Did not find child 1 named: " << childName1 << endl;
        }
      continue;
      }
    mass1 = childType1->getMass();
    //cout << "Child1:" << childType1->getName() << endl;
    childType2 = particleDb->getParticleType(childName2);
    if (!childType2)
      {
      if (reportError(__FUNCTION__))
        {
        cout << "Did not find child 2 named: " << childName2 << endl;
        }
      continue;
      }
    mass2 = childType2->getMass();
    //cout << "Child2:" << childType2->getName() << endl;
    String name3(childName3);
    if (!name3.EqualTo("none"))
      {
      childType3 = particleDb->getParticleType(childName3);
      if (!childType3)
        {
        if (reportError(__FUNCTION__))
          {
          cout << "Did not find child 3 named: " << childName3 << endl;
          }
        continue;
        }
      mass3 = childType3->getMass();
      //cout << "Child3:" << childType3->getName() << endl;
      }
    String name4(childName4);
    if (!name4.EqualTo("none"))
      {
      childType4 = particleDb->getParticleType(childName4);
      if (!childType4)
        {
        if (reportError(__FUNCTION__))
          {
          cout << "Did not find child 4 named: " << childName4 << endl;
          }
        continue;
        }
      mass4 = childType4->getMass();
      //cout << "Child4:" << childType4->getName() << endl;
      }
    double massDiff = mass1+mass2+mass3+mass4-mass0;
    if (massDiff>0)
      {
      if (reportDebug(__FUNCTION__))
        {
        cout << "Mass exception for " << parentName << "->" <<  childName1 <<"+"<< childName2 <<"+"<< childName3 <<"+"<< childName4 << " with Mass Diff=" << massDiff << endl;
        }
      continue;
      }
    if (CGcoeff)
      {// complete branching ratio by Clebsch-Gordan coefficient
        double j1, m1, j2, m2, J, M, CB;
        J  = parentType->getIsospin();
        M  = parentType->getIsospin3();
        j1 = childType1->getIsospin();
        m1 = childType1->getIsospin3();
        j2 = childType2->getIsospin();
        m2 = childType2->getIsospin3();
        CB = clebschGordan(J, M, j1, m1, j2, m2);
        tRatio = CB*CB * tBRatio;
        // Multiply the Clebsh by two?
        // The same spin, ampss, strangeness (charm?)
        // and different I3?
        if (  (TMath::Abs(childType1->getSpin()  - childType2->getSpin()) < 0.01)
            && (TMath::Abs(childType1->getMass() - childType2->getMass()) < 0.01)
            && (TMath::Abs(childType1->getIsospin3()-childType2->getIsospin3())   > 0.01)
            && (childType1->getNetStrangeness()-childType2->getNetStrangeness() == 0)
            && (childType1->getNetCharm()- childType2->getNetCharm() == 0)        )
          {
          tRatio *= 2.0;
          }
      }
    else
      {
      tRatio = tBRatio;
      }
    ParticleDecayMode decayMode;
    decayMode.setBranchingRatio(tRatio);
    decayMode.addChild(childType1);
    decayMode.addChild(childType2);
    if (childType3) decayMode.addChild(childType3);
    if (childType4) decayMode.addChild(childType3);
    parentType->addDecayMode(decayMode);
    }
  if (reportDebug(__FUNCTION__))
    cout << "Total index of particles read: " <<  particleDb->getNumberOfTypes() << endl;
  inputFileDecays.close();
  particleDb->setupDecayGenerator();
  if (reportDebug(__FUNCTION__)) particleDb->printProperties(cout);
}

void ParticleDbManager::initializeParticleDb()
{
  if ((particleDbExportCAP || particleDbConvertNativeToCAP) && !particleDbImportCAP)
    initializeParticleDbCAP();
  if ((particleDbExportNative || particleDbConvertCAPToNative) && !particleDbImportNative)
    initializeParticleDbNative();
}

void ParticleDbManager::initializeParticleDbCAP()
{
  ParticleDb * particleDb = new ParticleDb();
  ParticleDb::setDefaultParticleDb(particleDb);
}

void ParticleDbManager::initializeParticleDbNative()
{

}

void ParticleDbManager::exportParticleDb()
{
  if (particleDbExportCAP || !particleDbExportNative)
    exportParticleDbCAP();
  if (particleDbExportNative)
    exportParticleDbNative();
}

void ParticleDbManager::exportParticleDbCAP()
{
  ofstream & outputFile = openOutputAsciiFile(particleDbExportPath,particleDbExportFile,".dat");
  for (unsigned int iType=0; iType<particleDb->size(); iType++)
    {
    ParticleType * type = particleDb->getParticleType(iType);
    if (type->getPdgCode()<0) continue; // not printing antiparticles
    // this code is incomplete.
    }
  outputFile.close();
  if (reportEnd(__FUNCTION__))
;
}

void ParticleDbManager::exportParticleDbNative()
{  }


void ParticleDbManager::convertParticleDbCAPToNative() {}
void ParticleDbManager::convertParticleDbNativeToCAP() {}

double ParticleDbManager::clebschGordan(double aJot,  double aEm,
                                        double aJot1, double aEm1,
                                        double aJot2, double aEm2)
{
  int mint, maxt;
  double cgc = 0.0;
  int tIter;
  double coef;

  maxt = lrint(aJot1 + aJot2 - aJot);
  mint = 0;
  if (lrint(aJot1 - aEm1)  < maxt) maxt = lrint(aJot1 - aEm1);
  if (lrint(aJot2 + aEm2)  < maxt) maxt = lrint(aJot2 + aEm2);
  if (lrint(-(aJot-aJot2+aEm1))  > mint) mint = lrint(-(aJot-aJot2+aEm1));
  if (lrint(-(aJot-aJot1-aEm2))  > mint) mint = lrint(-(aJot-aJot1-aEm2));

  //  // PRINT_DEBUG_3("\t\t\tmint " << mint << " j1 " <<  aJot1 << " m1 " << aEm1);
  //  // PRINT_DEBUG_3("\t\t\tampxt " << maxt << " j2 " <<  aJot2 << " m2 " << aEm2);

  for (tIter = mint; tIter<=maxt; tIter ++)
    {
    coef = TMath::Power(-1, tIter);
    // PRINT_DEBUG_3("\t\t\tcoef1 " << coef);
    coef *= sqrt( (2*aJot+1)
                 * factorials[lrint(aJot1+aEm1)]
                 * factorials[lrint(aJot1-aEm1)]
                 * factorials[lrint(aJot2+aEm2)]
                 * factorials[lrint(aJot2-aEm2)]
                 * factorials[lrint(aJot+aEm)]
                 * factorials[lrint(aJot-aEm)]
                 );
    // PRINT_DEBUG_3("\t\t\tcoef2 " << coef);
    coef /= factorials[tIter]
    * factorials[lrint(aJot1+aJot2-aJot-tIter)]
    * factorials[lrint(aJot1-aEm1-tIter)]
    * factorials[lrint(aJot2+aEm2-tIter)]
    * factorials[lrint(aJot-aJot2+aEm1+tIter)]
    * factorials[lrint(aJot-aJot1-aEm2+tIter)];
    // PRINT_DEBUG_3("\t\t\tcoef3 " << coef);
    cgc += coef;
    }
  cgc *= deltaJ(aJot1, aJot2, aJot);

  return cgc;
}

double ParticleDbManager::deltaJ(double aJot1, double aJot2, double aJot)
{
  double res = sqrt( 1.0
                    * factorials[lrint( aJot1 + aJot2 - aJot)]
                    * factorials[lrint( aJot1 - aJot2 + aJot)]
                    * factorials[lrint(-aJot1 + aJot2 + aJot)]
                    / factorials[lrint( aJot1 + aJot2 + aJot + 1)]
                    );
  return res;
}


