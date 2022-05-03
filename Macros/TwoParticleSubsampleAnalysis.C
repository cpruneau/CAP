
#include "TString.h"
#include "TRandom.h"


int doPair(const TString & inputPathName,
           int nFiles,
           const TString & inputFileBaseName,
           const TString & outputPathName,
           const TString & outputFileName);


int TwoParticleSubsampleAnalysis()
{
  cout << "ParticleSubsampleAnalysis() Starting" << endl;
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"EnhancedGraph.hpp");
  gSystem->Load(includesPath+"Factory.hpp");
  gSystem->Load(includesPath+"FunctionCollection.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load(includesPath+"RandomGenerators.hpp");
  gSystem->Load(includesPath+"RapidityGenerator.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskCollection.hpp");
  gSystem->Load(includesPath+"Property.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"Event.hpp");
  gSystem->Load(includesPath+"EventFilter.hpp");
  gSystem->Load(includesPath+"EventHistos.hpp");
  gSystem->Load(includesPath+"EventLoop.hpp");
  gSystem->Load(includesPath+"GeneralizedGaussianConfiguration.hpp");
  gSystem->Load(includesPath+"GeneralizedGaussianProfile.hpp");
  gSystem->Load(includesPath+"GeneratorConfiguration.hpp");
  gSystem->Load(includesPath+"TwoPartCorrelationAnalyzer.hpp");
  gSystem->Load(includesPath+"Particle.hpp");
  gSystem->Load(includesPath+"ParticleFilter.hpp");
  gSystem->Load(includesPath+"ParticleHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairCombinedHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairDerivedHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairFilter.hpp");
  gSystem->Load(includesPath+"ParticlePairHistos.hpp");
  gSystem->Load(includesPath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libBase.dylib");

  TString inputPathName  = getenv("WAC_INPUT_PATH");
  TString outputPathName = getenv("WAC_OUTPUT_PATH");

  int nFiles;
  TString ** fileNameBase = new TString*[20];

  int selection = 1;
  switch (selection)
  {
    case 0:
    // two particle correlation in pp collisions
    // 14 TeV, specific pairs of particle species
    nFiles = 10;
    doPair(inputPathName, nFiles, TString("PYTHIA_softOnHardOff_Pairs_Narrow_HPHM_MB__"),   outputPathName, TString("PYTHIA_softOnHardOff_Pairs_Narrow_HPHM_MB__Sum0_99"));
    doPair(inputPathName, nFiles, TString("PYTHIA_softOnHardOff_Pairs_Narrow_PiPPiM_MB__"), outputPathName, TString("PYTHIA_softOnHardOff_Pairs_Narrow_PiPPiM_MB__Sum0_99"));
    doPair(inputPathName, nFiles, TString("PYTHIA_softOnHardOff_Pairs_Narrow_KPKM_MB__"),   outputPathName, TString("PYTHIA_softOnHardOff_Pairs_Narrow_KPKM_MB__Sum0_99"));
    doPair(inputPathName, nFiles, TString("PYTHIA_softOnHardOff_Pairs_Narrow_PPPM_MB__"),   outputPathName, TString("PYTHIA_softOnHardOff_Pairs_Narrow_PPPM_MB__Sum0_99"));
    doPair(inputPathName, nFiles, TString("PYTHIA_softOnHardOff_Pairs_Narrow_PiCKC_MB__"),  outputPathName, TString("PYTHIA_softOnHardOff_Pairs_Narrow_PiCKC_MB__Sum0_99"));
    doPair(inputPathName, nFiles, TString("PYTHIA_softOnHardOff_Pairs_Narrow_PiCPC_MB__"),  outputPathName, TString("PYTHIA_softOnHardOff_Pairs_Narrow_PiCPC_MB__Sum0_99"));
    doPair(inputPathName, nFiles, TString("PYTHIA_softOnHardOff_Pairs_Narrow_KCPC_MB__"),   outputPathName, TString("PYTHIA_softOnHardOff_Pairs_Narrow_KCPC_MB__Sum0_99"));
    break;

    case 1:
    // two particle correlation in pp collisions
    // 7 TeV, charged hadrons, inclusive and by x-section slice
    // Claude: Dec 18,2020.
    nFiles = 10;
    //inputPathName  =  "/Users/claudeapruneau/Documents/GitHub/WAC-DATA/InputFiles/PYTHIA_pp_7TeV_softOnHardOff/Pairs/";
    inputPathName = "/Users/claudeapruneau/Documents/GitHub/WAC-DATA/InputFiles/PYTHIA_pp_7TeV_softOnHardOff/Pairs/ByCentrality/Partials/";
    fileNameBase[0] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_0_5_V0MnGeq66Lt1000__");
    fileNameBase[1] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_5_10_V0MnGeq53Lt66__");
    fileNameBase[2] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_10_20_V0MnGeq38Lt53__");
    fileNameBase[3] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_20_30_V0MnGeq28Lt38__");
    fileNameBase[4] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_30_40_V0MnGeq20Lt28__");
    fileNameBase[5] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_40_50_V0MnGeq16Lt20__");
    fileNameBase[6] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_50_60_V0MnGeq12Lt16__");
    fileNameBase[7] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_60_70_V0MnGeq10Lt12__");
    fileNameBase[8] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_70_80_V0MnGeq7Lt10__");
    fileNameBase[9] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_80_90_V0MnGeq2Lt7__");
    fileNameBase[10] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_90_100_V0MnGeq0Lt2__");
    fileNameBase[11] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_0_100_MB__");

    //for (int iClass=0; iClass<12; iClass++)
    for (int iClass=8; iClass<9; iClass++)
    {
    doPair(inputPathName, nFiles, *fileNameBase[iClass],   outputPathName, *fileNameBase[iClass]+"Sum_0_9");
    }
  }
  return 0;
}

int doPair(const TString & inputPathName,
           int nFiles,
           const TString & inputFileBaseName,
           const TString & outputPathName,
           const TString & outputFileName)
{
  MessageLogger::LogLevel  debugLevel = MessageLogger::Info;
  TString ** fileNames = new TString* [nFiles];
  cout << "      input Path Name: " << inputPathName << endl;
  cout << " input File Base Name: " << inputFileBaseName << endl;
  cout << "     output Path Name: " << outputPathName << endl;
  cout << "     output File Name: " << outputFileName << endl;
  for (int iFile=0; iFile<nFiles; iFile++)
  {
  TString  fName = inputFileBaseName;
  fName += iFile;
  fileNames[iFile] = new TString(fName);
  cout << "   file Name [" << iFile << "]: " << *fileNames[iFile] << endl;
  }
  SubSampleStatCalculator * calculator = new SubSampleStatCalculator(inputPathName,nFiles,fileNames,outputPathName,outputFileName,debugLevel);
  calculator->execute();
  return 0;
//  delete calculator;
  //delete [] fileNames;
}
