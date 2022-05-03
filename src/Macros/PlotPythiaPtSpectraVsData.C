
#include "TString.h"
#include "TRandom.h"
#include <vector>

vector<TString>  dataFileNames;
vector<TFile*>   inputFiles;
vector<TFile*>   eventClassNames;
vector<TString>  eventClassTitles;
vector<TString>  particleNames;
vector<TString>  particleTitles;
vector<TString>  particlePairNames;
vector<TString>  particlePairTitles;
vector<TString>  observableNames;
vector<TString>  observableTitles;
vector<TString>  observableTitlesX;
vector<TString>  observableTitlesY;

int openRootInputFiles(TString & inputPathName,
                       vector<TString>  & fileNames,
                       vector<TFile*>   & inputFiles)
{
  cout << "<I> openRootInputFiles(...) Opening files..." << endl;
  TString fileName;
  for (unsigned int iFile=0;iFile<fileNames.size();iFile++)
    {
    fileName = inputPathName+fileNames[iFile];
    TFile * file = new TFile(fileName,"READ");
    if (file)
      {
      cout << "<I> openRootInputFiles() Successfully opened: " << fileName << endl;
      inputFiles.push_back(file);
      //file->ls();
      }
    else
      {
      cout << "<E> openRootInputFiles() Unable to open: " << fileName << endl;
      return 1;
      }
    }
  return 0;
}

void resetPlotArrays()
{
  eventClassNames.clear();
  particleNames.clear();
  particleTitles.clear();
  observableNames.clear();
  observableTitles.clear();
  observableTitlesX.clear();
  observableTitlesY.clear();
}


int PlotPythiaPtSpectraVsData()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load("libBase.dylib");

  MessageLogger::LogLevel logLevel = MessageLogger::Info;

  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 0;
  bool useColor = true;
  CanvasConfiguration * landscapeLinear     = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration * landscapeLogY       = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations = GraphConfiguration::createConfigurationPalette(10,1);

  Plotter * plotter = new Plotter("Plotter",MessageLogger::Info);
  plotter->setDefaultOptions(useColor);

  TString canvasName;
  TString fileName;
  TString inputPathName     = ""; //getenv("WAC_OUTPUT_PATH");
  TString outputPathName    = getenv("WAC_OUTPUT_PATH");
  TString hepInputPathName  = "/Users/claudeapruneau/Documents/GitHub/WAC4/publishedData/Alice/ProtonProton/Eur.Phys.J.C.73-2013-2662";

  resetPlotArrays();
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/publishedData/Alice/ProtonProton/Eur.Phys.J.C.73-2013-2662/Eur.Phys.J.C.73-2013-P2662_Table1.root");
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data/OutputFiles/PYTHIA/7TEV/V0AccTestHadronsOnlyWithColorReconnNoWeakDecay/PYTHIA_pp_7TeV_inelastic_P1_Sum.root");
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data/Repository/PYTHIA/7TeV/MinBias/PYTHIA_pp_7TeV_inelastic_P1_Sum.root");

  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/BetterTest/PYTHIA_pp_7TeV_inelastic_P1ND.root");  // 3
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/BetterTest/PYTHIA_pp_7TeV_inelastic_P1BS.root");  // 4
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/BetterTest2/PYTHIA_pp_7TeV_inelastic_P1ND.root"); // 5
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/BetterTest2/PYTHIA_pp_7TeV_inelastic_P1BS.root"); // 6
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/BetterTest3/PYTHIA_pp_7TeV_inelastic_P1ND.root"); // 7
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/BetterTest3/PYTHIA_pp_7TeV_inelastic_P1BS.root"); // 8

  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/publishedData/Alice/PbPb/JHEP11-2018-013/HEPData-ins1657384-v1.root");  // 9


  int status = openRootInputFiles(inputPathName, dataFileNames, inputFiles);
  if (status>0) return 1;

  vector<DataGraph*> dataGraphs;

  DataGraph * g;

  if (false)
    {
    //!
    //! Data from ALICE pp at 3 energies vs PYTHIA pp  at 7 TEV
    //!
    g = DataGraph::loadGraph("PP900Pt", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "ALICE 0.9 TeV", 0.0, 40.0, 1.0E-9, 1.0E3,
                             inputFiles[0], "Table 1","Hist1D_y1","Hist1D_y1_e1","Hist1D_y1_e2","Graph1D_y1");
    g->setProperties(*graphConfigurations[0]);
    dataGraphs.push_back(g);

    g = DataGraph::loadGraph("PP2760Pt", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "ALICE 2.76 TeV", 0.0, 40.0, 1.0E-9, 1.0E4,
                             inputFiles[0], "Table 1","Hist1D_y2","Hist1D_y2_e1","Hist1D_y2_e2","Graph1D_y2");
    g->setProperties(*graphConfigurations[1]);
    dataGraphs.push_back(g);

    g = DataGraph::loadGraph("PP7000Pt", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "ALICE 7.00 TeV", 0.0, 40.0, 1.0E-9, 1.0E4,
                             inputFiles[0], "Table 1","Hist1D_y3","Hist1D_y3_e1","Hist1D_y3_e2","Graph1D_y3");
    g->setProperties(*graphConfigurations[2]);
    dataGraphs.push_back(g);


    g = DataGraph::loadGraph("PP700PtCR1", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA 7.0 TeV", 0.0, 40.0, 1.0E-9, 1.0E4,
                             inputFiles[2], "P1_MB_HC_n1_ptXS", 1.0/0.05,4);
    g->setProperties(*graphConfigurations[3]);
    dataGraphs.push_back(g);

    plotter->plot("PythiaVsAlice-PP-Pt", landscapeLogY, "p_{T}(GeV)", 0.0, 100.0,  "1/(2#pi p_{T}) dN/dp_{T}",1.0E-9, 1.0E4,
                  dataGraphs, 0.5, 0.6, 0.8, 0.85, 0.04);
    dataGraphs.clear();

    //!
    //! Data from  PYTHIA pp  at 7 TEV -- 4 ranges of total V0M multiplicity
    //!
    g = DataGraph::loadGraph("PP700PtCR_0M4", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA CR1 7.0 TeV 0<M<4", 0.0, 12.0, 1.0E-9, 1.0E4,
                               inputFiles[1], "P1_PF0nGeq0Lt4_HC_n1_ptXS", 1.0/0.05,4);
    g->setProperties(*graphConfigurations[0]);
    dataGraphs.push_back(g);

    g = DataGraph::loadGraph("PP700PtCR_4M7", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA CR1 7.0 TeV 4#leq M<7", 0.0, 16.0, 1.0E-9, 1.0E4,
                               inputFiles[1], "P1_PF0nGeq4Lt7_HC_n1_ptXS", 1.0/0.05,4);
    g->setProperties(*graphConfigurations[1]);
    dataGraphs.push_back(g);

    g = DataGraph::loadGraph("PP700PtCR_7M12", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA CR1 7.0 TeV 7#leq M<12", 0.0, 16.0, 1.0E-9, 1.0E4,
                               inputFiles[1], "P1_PF0nGeq7Lt12_HC_n1_ptXS", 1.0/0.05,4);
    g->setProperties(*graphConfigurations[2]);
    dataGraphs.push_back(g);

    g = DataGraph::loadGraph("PP700PtCR_12M200", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA CR1 7.0 TeV 12#leq M<200", 0.0, 16.0, 1.0E-9, 1.0E4,
                               inputFiles[1], "P1_PF0nGeq12Lt200_HC_n1_ptXS", 1.0/0.05,4);
    g->setProperties(*graphConfigurations[4]);
    dataGraphs.push_back(g);

    g = DataGraph::loadGraph("PP700PtCR1", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA 7.0 TeV", 0.0, 40.0, 1.0E-9, 1.0E4,
                             inputFiles[2], "P1_MB_HC_n1_ptXS", 1.0,1);
    g->setProperties(*graphConfigurations[3]);
    dataGraphs.push_back(g);

    plotter->plot("PythiaVsXsect-PP-Pt", landscapeLogY, "p_{T}(GeV)", 0.0, 100.0,  "1/(2#pi p_{T}) dN/dp_{T}",1.0E-9, 1.0E4,
                  dataGraphs, 0.4, 0.6, 0.6, 0.85, 0.04);
    dataGraphs.clear();
    }


  //!
  //! Data from  boosted PYTHIA pp  at 7 TEV
  //!
//!  BetterTest   radialBoostConfig->param_a     = 0.08;

  double maxPt = 2.0;
  g = DataGraph::loadGraph("PP7000PtCR1ND", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA 7.0 Unboosted", 0.0, maxPt, 1.0E-1, 1.0E5,
                           inputFiles[3], "P1ND_MB_HC_n1_ptXS", 1.0,1);
  g->setProperties(*graphConfigurations[0]);
  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("PP7000PtCR1BS", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "Boost: #alpha=0.08", 0.0, maxPt, 1.0E-9, 1.0E4,
                           inputFiles[4], "P1BS_MB_HC_n1_ptXS", 1.0,1);
  g->setProperties(*graphConfigurations[1]);
  dataGraphs.push_back(g);

//  g = DataGraph::loadGraph("PP7000PtCR1ND2", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA 7.0 TeV ND", 0.0, maxPt, 1.0E-1, 1.0E5,
//                           inputFiles[5], "P1ND_MB_HC_n1_ptXS", 1.0,1);
//  g->setProperties(*graphConfigurations[2]);
//  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("PP7000PtCR1BS2", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "Boost: #alpha=0.4", 0.0, maxPt, 1.0E-9, 1.0E4,
                           inputFiles[6], "P1BS_MB_HC_n1_ptXS", 1.0,1);
  g->setProperties(*graphConfigurations[3]);
  dataGraphs.push_back(g);

//  g = DataGraph::loadGraph("PP7000PtCR1ND3", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA 7.0 TeV ND", 0.0, maxPt, 1.0E-1, 1.0E5,
//                           inputFiles[7], "P1ND_MB_HC_n1_ptXS", 1.0,1);
//  g->setProperties(*graphConfigurations[2]);
//  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("PP7000PtCR1BS3", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "Boost: #alpha=0.99", 0.0, maxPt, 1.0E-9, 1.0E4,
                           inputFiles[8], "P1BS_MB_HC_n1_ptXS", 1.0,1);
  g->setProperties(*graphConfigurations[4]);
  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("PbPb2760PtC1", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PbPb 2.76 TeV h^{#pm} 1", 0.0, maxPt, 1.0E-6, 1.0E4,
                           inputFiles[9], "Table 1","Hist1D_y1","Hist1D_y1_e1","Hist1D_y1_e2","Graph1D_y1");
  if (!g) return 1;
  g->setProperties(*graphConfigurations[6]);
  dataGraphs.push_back(g);

//  plotter->plot("PythiaVsXsect-PP-Pt-BS-HighPt", landscapeLogY, "p_{T}(GeV)", 0.0, maxPt,  "1/(2#pi p_{T}) dN/dp_{T}",1.0E-9, 1.0E4,
//                dataGraphs, 0.25, 0.25, 0.55, 0.45, 0.04);
  plotter->plot("PythiaVsXsect-PP-Pt-BS-LowPt", landscapeLogY, "p_{T}(GeV)", 0.0, maxPt,  "1/(2#pi p_{T}) dN/dp_{T}",1.0E-9, 1.0E4,
                dataGraphs, 0.45, 0.25, 0.65, 0.45, 0.04);
  dataGraphs.clear();


  plotter->printAllCanvas(outputPathName, printGif, printPdf, printSvg, printC);
  return 0;
}

