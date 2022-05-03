
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


int PlotHepData()
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

  int status = openRootInputFiles(inputPathName, dataFileNames, inputFiles);
  if (status>0) return 1;

  vector<DataGraph*> dataGraphs;

  DataGraph * g;

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

  g = DataGraph::loadGraph("PP700PtCR_0M4", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PYTHIA CR1 7.0 TeV 0<M<4", 0.0, 40.0, 1.0E-9, 1.0E4,
                             inputFiles[1], "P1_PF0nGeq0Lt4_HC_n1_ptXS", 1.0/0.05,4);
  g->setProperties(*graphConfigurations[4]);
  dataGraphs.push_back(g);

  plotter->plot("PythiaVsAlice-PP-Pt", landscapeLogY, "p_{T}(GeV)", 0.0, 100.0,  "1/(2#pi p_{T}) dN/dp_{T}",1.0E-9, 1.0E4,
                dataGraphs, 0.5, 0.6, 0.8, 0.85, 0.04);

  plotter->printAllCanvas(outputPathName, printGif, printPdf, printSvg, printC);
  return 0;
}

