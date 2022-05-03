
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


int PlotHGPtSpectraVsData()
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

  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/publishedData/Alice/PbPb/JHEP11-2018-013/HEPData-ins1657384-v1.root");  // 9
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/publishedData/Alice/ProtonProton/Eur.Phys.J.C.73-2013-2662/Eur.Phys.J.C.73-2013-P2662_Table1.root");

  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//HG/CorrectStatIsotropic/HG_Isotropic_200MeV_P1ND.root");
  dataFileNames.push_back("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//HG/CorrectStatIsotropic/HG_Isotropic_200MeV_P1WD.root");


  int status = openRootInputFiles(inputPathName, dataFileNames, inputFiles);
  if (status>0) return 1;

  vector<DataGraph*> dataGraphs;

  DataGraph * g;

  //!
  //! Data from  HG No boost Tchem=200MeV Tkin=170
  //!

  double maxPt = 3.0;
  g = DataGraph::loadGraph("HG200MeVPionPlusPrimary", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "#pi^{+} only", 0.0, maxPt, 1.0E-5, 1.0E5,
                           inputFiles[2], "P1ND_MB_PDG211_n1_ptXS", 50.0,4);
  g->setProperties(*graphConfigurations[0]);
  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("HG200MeVPionPlusSecondary", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "#pi^{+} w/ Feedown", 0.0, maxPt, 1.0E-9, 1.0E4,
                           inputFiles[3], "P1WD_MB_PDG211_n1_ptXS", 50.0,4);
  g->setProperties(*graphConfigurations[1]);
  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("ppPt", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "pp 7 TeV h^{#pm} 1", 0.0, maxPt, 1.0E-6, 1.0E4,
                           inputFiles[1], "Table 1","Hist1D_y1","Hist1D_y1_e1","Hist1D_y1_e2","Graph1D_y1");
  if (!g) return 1;
  g->setProperties(*graphConfigurations[2]);
  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("PbPb2760PtC1", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "PbPb 2.76 TeV h^{#pm} 1", 0.0, maxPt, 1.0E-6, 1.0E4,
                           inputFiles[0], "Table 1","Hist1D_y1","Hist1D_y1_e1","Hist1D_y1_e2","Graph1D_y1");
  if (!g) return 1;
  g->setProperties(*graphConfigurations[3]);
  dataGraphs.push_back(g);

//  plotter->plot("PythiaVsXsect-PP-Pt-BS-HighPt", landscapeLogY, "p_{T}(GeV)", 0.0, maxPt,  "1/(2#pi p_{T}) dN/dp_{T}",1.0E-9, 1.0E4,
//                dataGraphs, 0.25, 0.25, 0.55, 0.45, 0.04);
  plotter->plot("PythiaVsXsect-PP-Pt-BS-LowPt", landscapeLogY, "p_{T}(GeV)", 0.0, maxPt,  "1/(2#pi p_{T}) dN/dp_{T}",1.0E-9, 1.0E4,
                dataGraphs, 0.25, 0.25, 0.45, 0.45, 0.04);
  dataGraphs.clear();


  plotter->printAllCanvas(outputPathName, printGif, printPdf, printSvg, printC);
  return 0;
}

