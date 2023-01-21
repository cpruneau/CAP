// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class HadronGasGeneratorTask
 \ingroup WAC

 Class defining HadronGasGeneratorTask
 */
#include "TParameter.h"
#include "HadronGasGeneratorTask.hpp"

ClassImp(HadronGasGeneratorTask);

HadronGasGeneratorTask::HadronGasGeneratorTask(const TString & _name,
                                               Configuration & _configuration)
:
Task(_name,_configuration),
{
  appendClassName("HadronGasGeneratorTask");
}

void HadronGasGeneratorTask::setDefaultConfiguration()
{
  //Task::setDefaultConfiguration();
  TString hg = "HG"
  addParameter(""UseParticles",         false);
  addParameter(""UseEventStream0",      false);
  addParameter(""StandaloneMode",       true);
  setParameter("CreateHistograms",      true);
  setParameter("SaveHistograms",        true);
  setParameter("HistoAnalyzerName",     hg);
  setParameter("HistoBaseName",         hg);
  setParameter("DoTempDependentHistos", YES);
  setParameter("DoPtDistHistos",        YES);
  addParameter("nChemicalTemp",         1);
  addParameter("MinChemicalTemp",       150.0);
  addParameter("MaxChemicalTemp",       240.0);
  addParameter("nMuB",                  1);
  addParameter("MinMuB",                0.0);
  addParameter("MaxMuB",                0.0);
  addParameter("nMuS",                   1);
  addParameter("MinMuS",                0.0);
  addParameter("MaxMuS",                0.0);
  addParameter("Volume",                1.0);
  addParameter("nMass",                 50);
  addParameter("MinMass",               0.0);
  addParameter("MaxMass",               3.0);
  addParameter("nP",                    500);
  addParameter("MinP",                  0.0);
  addParameter("MaxP",                  5.0);
}


void HadronGasGeneratorTask::initialize()
{
  Task::initialize();
  standaloneMode = getValueBool(name,"StandaloneMode");
  minTotalMult   = getValueInt(name,"MinTotalMult");
  maxTotalMult   = getValueInt(name,"MinTotalMult");
  rangeTotalMult = maxTotalMult - minTotalMult;
  minY = getValueInt(name,"MinY");
  maxY = getValueInt(name,"MaxY");
  rangeY = maxY - minY;
  double temperature = getValueDouble(name,"chemicalTemp");
  double muB         = getValueDouble(name,"MuB");
  double muS         = getValueDouble(name,"MuS");
  
  particleTypes       = ParticleTypeCollection::getMasterParticleCollection();
  stableParticleTypes = particleTypes->extractCollection(1);
  if (reportDebug("HadronGasGeneratorTask",name,"initialize()"))
    {
    stableParticleTypes->printProperties(std::cout);
    }
 
  TString label = "T";
  label += int(1000*temperature);
  HadronGas * gas = new HadronGas(particleTypes,stableParticleTypes,getSeverity());
  gas->setName(label);
  gas->calculateAllProperties(temperature,muB,muS);
  if (reportDebug("HadronGasGeneratorTask",name,"initialize()"))
    {
    gas->printProperties(std::cout);
    }
  hadronGases.push_back(gas);

  if (reportDebug("HadronGasGeneratorTask",name,"initialize()"))
    {
    cout << "Setting up momentum generation parameters." << endl;
    }
  int generatorType = getValueInt(name,"generatorType");
  vector<double> parameters;
  parameters.clear();
  switch (generatorType)
    {
      case 0:
      case 1:
      case 2:
      parameters.push_back(temperature);
      break;

      case 3:
      case 4:
      parameters.push_back(0.001);
      parameters.push_back(10.00);
      break;

      case 5:
      case 6:
      parameters.push_back(minY);
      parameters.push_back(rangeY);
      parameters.push_back(temperature);
      break;

      case 7:
      parameters.push_back(minY);
      parameters.push_back(rangeY);
      parameters.push_back(temperature);
      parameters.push_back(1000);
      parameters.push_back(0.0);
      parameters.push_back(10.0);
      break;
    }
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "           Momentum generator type selected: " << generatorType << endl;
    cout << "   Number of momentum generation parameters: " << parameters.size() << endl;
    for (unsigned int k=0; k<parameters.size(); k++)
      {
      cout << "                            parameter index: " << k << "  value: " <<  parameters[k] << endl;
      }
    }

  unsigned int nSpecies = particleTypes->size();
  if (reportDebug(__FUNCTION__)) cout << " Setting the momentum generators for : " << nSpecies <<  " particle species."  << endl;
  for (unsigned int k=0; k<nSpecies; k++)
    {
    ParticleType * particleType = particleTypes->getParticleType(k);

    MomentumGenerator * gen = new MomentumGenerator(generatorType, particleType, parameters);
    momentumGenerators.push_back(gen);
    }

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << " Filling abundance histograms."  << endl;
    cout << "             hadronGases.size:"  << hadronGases.size() << endl;
    }

  vector<double>  & d = hadronGases[0]->particleDensities;
  int nSpeciesConsidered = d.size();
  TString histoName;
  histoName = getName();
  histoName += "_relativeAbundances";
  relativeAbundances = new TH1D(histoName,histoName,nSpeciesConsidered,0.0,double(nSpeciesConsidered));
  histoName = getName();
  histoName += "_relativeAbundancesGas";
  relativeAbundancesGas = new TH1D(histoName,histoName,nSpeciesConsidered,0.0,double(nSpeciesConsidered));

  for (unsigned int k=1; k<=d.size();k++)
  {
  relativeAbundancesGas->SetBinContent(k, d[k-1]);
  relativeAbundancesGas->SetBinError(k, 0.0);
  }
  if (reportEnd(__FUNCTION__))
    ;
}

void HadronGasGeneratorTask::createHistograms()
{
  // temperature, muB, and muS  must be input in GeV

  if (reportStart(__FUNCTION__))
    ;
  histograms.clear();
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  Severity debugLevel = getSeverityLevel();
  TString & name = getName();

  TString bn                    = configuration.getValueString(name,"HistoBaseName");
  bool    doTempDependentHistos = configuration.getValueBool(name,"DoTempDependentHistos");
  int     nChemicalTemp         = configuration.getValueInt(name,"nChemicalTemp");
  double  minChemicalTemp       = configuration.getValueDouble(name,"MinChemicalTemp");
  double  maxChemicalTemp       = configuration.getValueDouble(name,"MaxChemicalTemp");

  int     nChemicalTemp         = configuration.getValueInt(name,"nChemicalTemp");
  double  minChemicalTemp       = configuration.getValueDouble(name,"MinChemicalTemp");
  double  maxChemicalTemp       = configuration.getValueDouble(name,"MaxChemicalTemp");
  double  stepTemp              = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);

  int     nMuB                  = configuration.getValueInt(name,"nMuB");
  double  minMuB                = configuration.getValueDouble(name,"MinMuB");
  double  maxMuB                = configuration.getValueDouble(name,"MaxMuB");
  double  stepMuB               = (maxMuB - minMuB)/double(nMuB);

  int     nMuS                  = configuration.getValueInt(name,"nMuS");
  double  minMuS                = configuration.getValueDouble(name,"MinMuS");
  double  maxMuS                = configuration.getValueDouble(name,"MaxMuS");
  double  stepMuS               = (maxMuS - minMuS)/double(nMuS);

  TString histoBaseName;
  TString tempLabel;
  TString muBLabel;
  TString muSLabel;

  Histograms * histos;
  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    tempLabel = "T";
    tempLabel += 1000*(minChemicalTemp+stepTemp*double(iTemp));
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      muBLabel = "B";
      muBLabel += 1000*(minMuB+stepMuB*double(iMuB));
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        muSLabel = "S";
        muSLabel += 1000*(minMuS+stepMuS*double(iMuS));
        histoBaseName = bn;
        histoBaseName += "_";
        histoBaseName += tempLabel;
        histoBaseName += "_";
        histoBaseName += muBLabel;
        histoBaseName += "_";
        histoBaseName += muSLabel;
        if (reportDebug(__FUNCTION__)) cout << "Set:" << histoBaseName << endl;
        histos = new HadronGasHistograms(histoBaseName,configuration);
        histos->createHistograms();
        baseSingleHistograms.push_back(histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histoBaseName = bn;
    histoBaseName += "VsT";
    histos = new HadronGasVsTempHistograms(histoBaseName,configuration);
    histos->createHistograms();
    histograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void HadronGasGeneratorTask::loadHistograms(TFile * inputFile)
{
  histograms.clear();
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  Severity debugLevel = getSeverityLevel();

  TString bn                    = configuration.getValueString(name,"HistoBaseName");
  bool    doTempDependentHistos = configuration.getValueBool(name,"DoTempDependentHistos");
  int     nChemicalTemp         = configuration.getValueInt(name,"nChemicalTemp");
  double  minChemicalTemp       = configuration.getValueDouble(name,"MinChemicalTemp");
  double  maxChemicalTemp       = configuration.getValueDouble(name,"MaxChemicalTemp");

  int     nChemicalTemp         = configuration.getValueInt(name,"nChemicalTemp");
  double  minChemicalTemp       = configuration.getValueDouble(name,"MinChemicalTemp");
  double  maxChemicalTemp       = configuration.getValueDouble(name,"MaxChemicalTemp");
  double  stepTemp              = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);

  int     nMuB                  = configuration.getValueInt(name,"nMuB");
  double  minMuB                = configuration.getValueDouble(name,"MinMuB");
  double  maxMuB                = configuration.getValueDouble(name,"MaxMuB");
  double  stepMuB               = (maxMuB - minMuB)/double(nMuB);

  int     nMuS                  = configuration.getValueInt(name,"nMuS");
  double  minMuS                = configuration.getValueDouble(name,"MinMuS");
  double  maxMuS                = configuration.getValueDouble(name,"MaxMuS");
  double  stepMuS               = (maxMuS - minMuS)/double(nMuS);

  TString histoBaseName;
  TString tempLabel;
  TString muBLabel;
  TString muSLabel;

  Histograms * histos;
  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    tempLabel = "T";
    tempLabel += 1000*(minChemicalTemp+stepTemp*double(iTemp));
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      muBLabel = "B";
      muBLabel += 1000*(minMuB+stepMuB*double(iMuB));
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        muSLabel = "S";
        muSLabel += 1000*(minMuS+stepMuS*double(iMuS));
        histoBaseName = bn;
        histoBaseName += "_";
        histoBaseName += tempLabel;
        histoBaseName += "_";
        histoBaseName += muBLabel;
        histoBaseName += "_";
        histoBaseName += muSLabel;
        if (reportDebug(__FUNCTION__)) cout << "Set:" << histoBaseName << endl;
        histos = new HadronGasHistograms(histoBaseName,configuration);
        histos->loadHistograms(inputFile);
        baseSingleHistograms.push_back(histos);
        }
      }
    }
  if (doTempDependentHistos)
    {
    histoBaseName = bn;
    histoBaseName += "VsT";
    histos = new HadronGasVsTempHistograms(histoBaseName,configuration);
    histos->loadHistograms(inputFile);
    histograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}




void HadronGasGeneratorTask::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  bool    doTempDependentHistos = configuration.getValueBool(name,"DoTempDependentHistos");

  int     nChemicalTemp         = configuration.getValueInt(name,"nChemicalTemp");
  double  minChemicalTemp       = configuration.getValueDouble(name,"MinChemicalTemp");
  double  maxChemicalTemp       = configuration.getValueDouble(name,"MaxChemicalTemp");
  double  stepTemp              = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);

  int     nMuB                  = configuration.getValueInt(name,"nMuB");
  double  minMuB                = configuration.getValueDouble(name,"MinMuB");
  double  maxMuB                = configuration.getValueDouble(name,"MaxMuB");
  double  stepMuB               = (maxMuB - minMuB)/double(nMuB);

  int     nMuS                  = configuration.getValueInt(name,"nMuS");
  double  minMuS                = configuration.getValueDouble(name,"MinMuS");
  double  maxMuS                = configuration.getValueDouble(name,"MaxMuS");
  double  stepMuS               = (maxMuS - minMuS)/double(nMuS);

  for (int iTemp=0; iTemp<nChemicalTemp; iTemp++ )
    {
    double temperature = minChemicalTemp+stepTemp*double(iTemp);
    for (int iMuB=0; iMuB<nMuB; iMuB++ )
      {
      double muB = minMuB+stepMuB*double(iMuB);
      for (int iMuS=0; iMuS<nMuS; iMuS++ )
        {
        double muB = minMuB+stepMuB*double(iMuB);
        // vector<HadronGasParticle*> hadrons;
        HadronGas * hg = new HadronGas();
        hadronGasArray.push_back(hg);
        addSubtask(hg);
        hg->initialize();
        hg->execute();
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;

//
//  if (standaloneMode)
//    {
//    // In this mode, we generate one HG  event per call. One interaction vertex is
//    // inserted in the event stream and generate is called to carry out the particle generation.
//    particleFactory->reset();
//    event.reset();
//    interaction = particleFactory->getNextObject();
//    interaction->reset();
//    interaction->setType( ParticleType::getInteractionType());
//    interaction->setXYZT(0.0, 0.0, 0.0, 0.0);
//    //interaction->setRThetaPhiT(3.0, TMath::PiOver2(), TMath::TwoPi()*taskRandomGenerator->Rndm(),0.0);
//    event.add(interaction);
//    event.setNucleusA(1.0,1.0);
//    event.setNucleusB(1.0,1.0);
//    generate(interaction);
//    EventProperties & ep = * eventStreams[0]->getEventProperties();
//    ep.zProjectile       = 1;     // atomic number projectile
//    ep.aProjectile       = 1;     // mass number projectile
//    ep.nPartProjectile   = 1;     // number of participants  projectile
//    ep.zTarget           = 1;     // atomic number target
//    ep.aTarget           = 1;     // mass number target
//    ep.nPartTarget       = 1;     // number of participants  target
//    ep.nParticipantsTotal= 2;     // total number of participants
//    ep.nBinaryTotal      = 1;     // total number of binary collisions
//    ep.impactParameter   = -99999; // nucleus-nucleus center distance in fm
//    ep.fractionalXSection= -99999; // fraction cross section value
////    ep.refMultiplicity = eventStreams[0]->getNParticles();// nominal multiplicity in the reference range
////    ep.particlesCounted  = getNParticlesCounted();
////    ep.particlesAccepted = getNParticlesAccepted();
//    }
//  else
//    {
//    // In this mode, we generate several HG cells per event. Interaction vertices and their locations
//    // are assumed to be already loaded in the event stream and we produce as many HG cells as there
//    // are nucleon-nucleon interactions in the stream. Objects nucleusA and nucleusB are assumed defined by
//    // an earlier task and the structure EventProperties is assumed filled by that earlier task. We only fill the number of
//    // particle generated and accepted (in addition to storing the generated particles in the event)
//
//    // May skip this event if it does not satisfy the event cut.
//    if (!eventFilters[0]->accept(event)) return;
//    if (event.getParticleCount() < 1)
//      {
//      return;
//      }
//    vector<Particle*> interactions = event.getNucleonNucleonInteractions();
//
//    unsigned int n = interactions.size();
////    if (reportWarning("PythiaEventGenerator",name,"execute()"))
////      cout << "Size of interactions:" <<  n << endl;
//    for (unsigned int kInter=0; kInter<n; kInter++)
//      {
//      generate(interactions[kInter]);
//      }
////    EventProperties & ep = * event.getEventProperties();
////    ep.refMultiplicity = getNParticlesAccepted(); // nominal multiplicity in the reference range
////    ep.particlesCounted      = getNParticlesCounted();
////    ep.particlesAccepted     = getNParticlesAccepted();
//    }
}

void HadronGasGeneratorTask::generate(Particle * parent)
{
  
  if (reportStart(__FUNCTION__))
    ;
  Event & event = * eventStreams[0];
  int multiplicity = minTotalMult + double(rangeTotalMult)*gRandom->Rndm();
  //ParticleFilter & particleFilter = * particleFilters[0];
  Particle * particle;
  TLorentzVector parentPosition = parent->getPosition();
  for (int iPart=0; iPart< multiplicity; iPart++)
    {
    // implement basic no correlation scenario.
    //ParticleType * type = hadronGases[0]->generateRandomHadron();
    int     index  = hadronGases[0]->generateRandomHadronByIndex();
    double  rindex = index + 0.01;
    relativeAbundances->Fill(rindex);
    if (index<0)
      {
      if (reportFatal())
        {
        cout << "hadron index=" << index << endl;
        }
      exit(1);
      }
    ParticleType * type = particleTypes->getParticleType(index);
    if (type==nullptr)
      {
      if (reportError(__FUNCTION__)) cout << "HadronGas::generateRandomHadron() return null pointer. Post task error." << endl;
      postTaskError();
      exit(1);
      return;
      }
    TLorentzVector momentum;
    momentumGenerators[index]->generate(momentum);
    particle = particleFactory->getNextObject();
    particle->setParent(parent);
    particle->set(type, momentum, parentPosition,true);
    //particle->printProperties(cout);
    //incrementParticlesCounted(); // photons are NOT included in this tally
    //if (!particleFilter.accept(*particle)) continue; // no filter for now..
    event.add(particle);
    // // incrementParticlesAccepted();
    }
}
