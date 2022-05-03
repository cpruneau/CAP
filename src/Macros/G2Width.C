
#include "TString.h"
#include "TRandom.h"
#include "Plotter.hpp"
//#include "BidimGaussFitConfiguration.hpp"
//#include "BidimGaussFitResult.hpp"
//#include "BidimGaussFitter.hpp"

int G2Width()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskCollection.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"BidimGaussFitConfiguration.hpp");
  gSystem->Load(includesPath+"BidimGaussFitResult.hpp");
  gSystem->Load(includesPath+"BidimGaussFitter.hpp");
  gSystem->Load("libBase.dylib");

  TString inputPath  = "/Users/claudeapruneau/Documents/GitHub/WAC-DATA/InputFiles/PYTHIA_pp_7TeV_softOnHardOff/Pairs/ByCentrality/Sums/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/WAC-DATA/OutputFiles/";

  int nFiles = 11;
  TString ** inputFileNames = new TString*[nFiles];
  TFile   ** inputFiles     = new TFile  *[nFiles];
  TString ** histoNames     = new TString*[nFiles];
  TH2     ** h2s            = new TH2    *[nFiles];
  BidimGaussFitter           * fitter  = new BidimGaussFitter(); //*[nFiles];
  BidimGaussFitConfiguration * config  = new BidimGaussFitConfiguration();
  config->outputPath = outputPath;
  config->deltaEtaLimit = 1.3;
  config->deltaEtaOuterLimit = 1.5;

  config->fixedParameters[0] = false;   config->initialValues[0]     = 0.1; config->lowerLimit[0] = 0.02;   config->upperLimit[0] = 20.0;
  config->fixedParameters[1] = false;   config->initialValues[1]     = 0.5;  config->lowerLimit[1] = 0.1;    config->upperLimit[1] = 1.8;
  config->fixedParameters[2] = false;   config->initialValues[2]     = 0.5;  config->lowerLimit[2] = 0.1;    config->upperLimit[2] = 1.8;
  config->fixedParameters[3] = false;   config->initialValues[3]     = 1.0;  config->lowerLimit[3] = 1.00;   config->upperLimit[3] = 4.0;
  config->fixedParameters[4] = false;   config->initialValues[4]     = 1.0;  config->lowerLimit[4] = 1.00;   config->upperLimit[4] = 4.0;

  config->fixedParameters[12] = true;   config->fixedValues[12]     = 0.0;
  config->fixedParameters[13] = true;   config->fixedValues[13]     = 0.0;
  config->fixedParameters[14] = true;   config->fixedValues[14]     = 0.0;
  config->fixedParameters[15] = true;   config->fixedValues[15]     = 0.0;


  inputFileNames[0]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_0_5_V0MnGeq66Lt1000__Sum_0_9.root");
  inputFileNames[1]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_5_10_V0MnGeq53Lt66__Sum_0_9.root");
  inputFileNames[2]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_10_20_V0MnGeq38Lt53__Sum_0_9.root");
  inputFileNames[3]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_20_30_V0MnGeq28Lt38__Sum_0_9.root");
  inputFileNames[4]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_30_40_V0MnGeq20Lt28__Sum_0_9.root");
  inputFileNames[5]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_40_50_V0MnGeq16Lt20__Sum_0_9.root");
  inputFileNames[6]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_50_60_V0MnGeq12Lt16__Sum_0_9.root");
  inputFileNames[7]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_60_70_V0MnGeq10Lt12__Sum_0_9.root");
  inputFileNames[8]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_70_80_V0MnGeq7Lt10__Sum_0_9.root");
  inputFileNames[9]  = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_80_90_V0MnGeq2Lt7__Sum_0_9.root");
  inputFileNames[10] = new TString("PYTHIA_pp_7TeV_softOnHardOff_Pairs_Narrow_HPHM_90_100_V0MnGeq0Lt2__Sum_0_9.root");

//  histoNames[0]  = new TString("Narrow_HPHM_0_5_V0MnGeq66Lt1000_HPHMCI_G2_DetaDphi_shft");
//  histoNames[1]  = new TString("Narrow_HPHM_5_10_V0MnGeq53Lt66_HPHMCI_G2_DetaDphi_shft");
//  histoNames[2]  = new TString("Narrow_HPHM_10_20_V0MnGeq38Lt53_HPHMCI_G2_DetaDphi_shft");
//  histoNames[3]  = new TString("Narrow_HPHM_20_30_V0MnGeq28Lt38_HPHMCI_G2_DetaDphi_shft");
//  histoNames[4]  = new TString("Narrow_HPHM_30_40_V0MnGeq20Lt28_HPHMCI_G2_DetaDphi_shft");
//  histoNames[5]  = new TString("Narrow_HPHM_40_50_V0MnGeq16Lt20_HPHMCI_G2_DetaDphi_shft");
//  histoNames[6]  = new TString("Narrow_HPHM_50_60_V0MnGeq12Lt16_HPHMCI_G2_DetaDphi_shft");
//  histoNames[7]  = new TString("Narrow_HPHM_60_70_V0MnGeq10Lt12_HPHMCI_G2_DetaDphi_shft");
//  histoNames[8]  = new TString("Narrow_HPHM_70_80_V0MnGeq7Lt10_HPHMCI_G2_DetaDphi_shft");
//  histoNames[9]  = new TString("Narrow_HPHM_80_90_V0MnGeq2Lt7_HPHMCI_G2_DetaDphi_shft");
//  histoNames[10] = new TString("Narrow_HPHM_90_100_V0MnGeq0Lt2_HPHMCI_G2_DetaDphi_shft");

  histoNames[0]  = new TString("Narrow_HPHM_0_5_V0MnGeq66Lt1000_HPHMCD_G2_DetaDphi_shft");
  histoNames[1]  = new TString("Narrow_HPHM_5_10_V0MnGeq53Lt66_HPHMCD_G2_DetaDphi_shft");
  histoNames[2]  = new TString("Narrow_HPHM_10_20_V0MnGeq38Lt53_HPHMCD_G2_DetaDphi_shft");
  histoNames[3]  = new TString("Narrow_HPHM_20_30_V0MnGeq28Lt38_HPHMCD_G2_DetaDphi_shft");
  histoNames[4]  = new TString("Narrow_HPHM_30_40_V0MnGeq20Lt28_HPHMCD_G2_DetaDphi_shft");
  histoNames[5]  = new TString("Narrow_HPHM_40_50_V0MnGeq16Lt20_HPHMCD_G2_DetaDphi_shft");
  histoNames[6]  = new TString("Narrow_HPHM_50_60_V0MnGeq12Lt16_HPHMCD_G2_DetaDphi_shft");
  histoNames[7]  = new TString("Narrow_HPHM_60_70_V0MnGeq10Lt12_HPHMCD_G2_DetaDphi_shft");
  histoNames[8]  = new TString("Narrow_HPHM_70_80_V0MnGeq7Lt10_HPHMCD_G2_DetaDphi_shft");
  histoNames[9]  = new TString("Narrow_HPHM_80_90_V0MnGeq2Lt7_HPHMCD_G2_DetaDphi_shft");
  histoNames[10] = new TString("Narrow_HPHM_90_100_V0MnGeq0Lt2_HPHMCD_G2_DetaDphi_shft");


  double sigmaEta[10];
  double sigmaPhi[10];
  double sigmaEtaErr[10];
  double sigmaPhiErr[10];
  double chi2Ndf[10];

  TString fileName;
  for (int iFile=0;iFile<11;iFile++)
  {
  fileName = inputPath+*inputFileNames[iFile];
  inputFiles[iFile] = new TFile(fileName,"OLD");
  if (inputFiles[iFile])
    {
    cout << "<I> G2Width() Successfully opened: " << fileName << endl;
    }
  else
    {
    cout << "<E> G2Width() Unable to open: " << fileName << endl;
    return 1;
    }
  h2s[iFile] = (TH2*)inputFiles[iFile]->Get(*histoNames[iFile]);
  if (h2s[iFile] )
    {
    cout << "<I> G2Width() Successfully retrieved: " << *histoNames[iFile] << endl;
    }
  else
    {
    cout << "<E> G2Width() Unable to retrieve: " << *histoNames[iFile] << endl;
    return 1;
    }

  fitter->fullFit(h2s[iFile], *config);

  sigmaEta[iFile]    = fitter->fullFitResult.sigmaEta;
  sigmaEtaErr[iFile] = fitter->fullFitResult.sigmaEtaErr;
  sigmaPhi[iFile]    = fitter->fullFitResult.sigmaPhi;
  sigmaPhiErr[iFile] = fitter->fullFitResult.sigmaPhiErr;
  chi2Ndf[iFile]     = fitter->fullFitResult.chi2Ndf;
  }

  fitter->printAllCanvas(outputPath);

  cout << "================================================================================ " << endl;
  for (int iFile=0;iFile<1;iFile++)
  {
  cout << iFile
  << "  ETA: " << sigmaEta[iFile] <<  " +- " << sigmaEtaErr[iFile]
  << "  PHI: " << sigmaPhi[iFile] <<  " +- " << sigmaPhiErr[iFile]
  << "  chi2/ndf: " << chi2Ndf[iFile] << endl;
  }

  return 0;
}

