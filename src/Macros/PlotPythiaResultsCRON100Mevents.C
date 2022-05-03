#include "TString.h"
#include "TRandom.h"

int PlotPythiaResultsCRON100Mevents()
{
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
  gSystem->Load("libBase.dylib");

  MessageLogger::LogLevel logLevel = MessageLogger::Info;
  vector<TString>  eventClassNames;
  vector<TString>  eventClassTitles;
  vector<TString>  particleNames;
  vector<TString>  particleTitles;
  vector<TString>  particlePairNames;
  vector<TString>  particlePairTitles;
  vector<TString>  observableNames;
  vector<TString>  observableTitles;
  vector<TString>  observableTitlesX;
  vector<TString>  observableTitlesY;
  vector<double>  minX;
  vector<double>  maxX;
  vector<double>  minY;
  vector<double>  maxY;
  vector<double>  minZ;
  vector<double>  maxZ;

  vector<TH1*>     histograms;
  vector<TString>  titles;
  vector<Plotter*> plotters;
  vector<GraphConfiguration*> graphConfigurations;
  TString canvasName;

  bool color = true;
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);

  CanvasConfiguration * landscapeLinear     = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  CanvasConfiguration * landscapeLogY       = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::LogY);
  for (int iGraph=0;iGraph<40;iGraph++)
    {
    GraphConfiguration * gc = new GraphConfiguration(1,iGraph%10);
    gc->markerSize = 0.5;
    gc->plotOption = "E0";
    graphConfigurations.push_back(gc);
    }
  GraphConfiguration * gc2D = new GraphConfiguration(2);
  gc2D->zTitleOffset = 1.1;

  // TString inputPathName  = getenv("WAC_REPOSITORY_PATH");
  TString inputPathName  = getenv("WAC_OUTPUT_PATH");
  TString outputPathName = getenv("WAC_OUTPUT_PATH");

  inputPathName  = "/Users/claudeapruneau/Documents/GitHub/WAC4/data/Repository/PYTHIA/7TeV/100M-CRON/";
  outputPathName = "/Users/claudeapruneau/Documents/GitHub/WAC4/data/OutputFiles/PYTHIA/7TEV/100M-CRON/";

  TString fileName;
  vector<TString>  fileNames;
  vector<TFile*>   inputFiles;
  fileNames.push_back("PYTHIA_pp_7TeV_inelastic_P2_Sum.root");
  for (unsigned int iFile=0;iFile<fileNames.size();iFile++)
    {
    fileName = inputPathName+fileNames[iFile];
    TFile * file = new TFile(fileName,"OLD");
    if (file)
      {
      cout << "<I> ParticlePlots() Successfully opened: " << fileName << endl;
      inputFiles.push_back(file);
      //file->ls();
      }
    else
      {
      cout << "<E> ParticlePlots() Unable to open: " << fileName << endl;
      return 1;
      }
    }

  plotters.push_back(new Plotter("Plotter",logLevel));

  unsigned int iEventClass = 0;

  // =================
  // Pairs
  // ===============
  if (true)
    {
    TString name;
    eventClassNames.clear();
    eventClassTitles.clear();
    particleNames.clear();
    particleTitles.clear();
    particlePairNames.clear();
    particlePairTitles.clear();
    observableNames.clear();
    observableTitles.clear();
    observableTitlesX.clear();
    observableTitlesY.clear();
    minX.clear();
    maxX.clear();
    minY.clear();
    maxY.clear();
    minZ.clear();
    maxZ.clear();

    eventClassNames.push_back("P2_PF0nGeq0Lt5.5_");      eventClassTitles.push_back("1 -- 5");
    eventClassNames.push_back("P2_PF0nGeq5.5Lt11.5_");   eventClassTitles.push_back("6 -- 11");
    eventClassNames.push_back("P2_PF0nGeq11.5Lt18.5_");  eventClassTitles.push_back("12 -- 18");
    eventClassNames.push_back("P2_PF0nGeq18.5Lt34.5_");  eventClassTitles.push_back("18 -- 34");
    eventClassNames.push_back("P2_PF0nGeq34.5Lt200_");   eventClassTitles.push_back("34 -- 200");

    particleNames.push_back("PiP");       particleTitles.push_back("#pi^{+}");
    particleNames.push_back("PiM");       particleTitles.push_back("#pi^{-}");
    particleNames.push_back("KP");        particleTitles.push_back("K^{+}");
    particleNames.push_back("KM");        particleTitles.push_back("K^{-}");
    particleNames.push_back("PP");        particleTitles.push_back("p^{+}");
    particleNames.push_back("PM");        particleTitles.push_back("#bar{p}");

//    particlePairNames.push_back("PiPPiM");
//    particlePairNames.push_back("PiPKP");
//    particlePairNames.push_back("PiPPP");
//    particlePairNames.push_back("KPPiP");
//    particlePairNames.push_back("KPKM");
//    particlePairNames.push_back("KPPP");
//    particlePairNames.push_back("PPPiP");
//    particlePairNames.push_back("PPKP");
//    particlePairNames.push_back("PPPM");

    for (unsigned int iPart1=0; iPart1<particleNames.size(); iPart1++)
      {
      for (unsigned int iPart2=0; iPart2<particleNames.size(); iPart2++)
        {
        name =  particleNames[iPart1];
        name += particleNames[iPart2];
        particlePairNames.push_back(name);
        name =  particleTitles[iPart1];
        name += particleTitles[iPart2];
        particlePairTitles.push_back(name);
        }
      }
    //observableNames.push_back("R2_ptpt");          observableTitles.push_back("R_{2}(p_{T,1},p_{T,2})");          observableTitlesX.push_back("p_{T,1}");    observableTitlesY.push_back("p_{T,2}");        minX.push_back( 0.0);  maxX.push_back(2.0); minY.push_back(0.0);  maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
    //observableNames.push_back("R2_phiPhi");        observableTitles.push_back("R_{2}(#phi_{1},#phi_{2})");        observableTitlesX.push_back("#phi_{1}");   observableTitlesY.push_back("#phi_{2}");       minX.push_back( 0.0);  maxX.push_back(6.283); minY.push_back(0.0); maxY.push_back(6.283); minZ.push_back(5.0); maxZ.push_back(-5.0);
    //observableNames.push_back("R2_etaEta");        observableTitles.push_back("R_{2}(#eta_{1},#eta_{2})");        observableTitlesX.push_back("#eta_{1}");   observableTitlesY.push_back("#eta_{2}");       minX.push_back(-1.0);  maxX.push_back(1.0); minY.push_back(-1.0); maxY.push_back(1.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
    observableNames.push_back("R2_DetaDphi_shft"); observableTitles.push_back("R_{2}(#Delta#eta,#Delta#varphi)"); observableTitlesX.push_back("#Delta#eta"); observableTitlesY.push_back("#Delta#varphi");  minX.push_back(-2.0);  maxX.push_back(2.0); minY.push_back(-1.57); maxY.push_back(4.712); minZ.push_back(5.0); maxZ.push_back(-5.0);

    histograms.clear();

    for (unsigned int iEventClass = 0; iEventClass<eventClassNames.size();iEventClass++)
      {
      for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
        {

        for (unsigned int iPart=0; iPart<particlePairNames.size(); iPart++)
          {
          canvasName = eventClassNames[iEventClass];
          canvasName += observableNames[iObservable];
          canvasName += "_";
          canvasName += particlePairNames[iPart];
          TString histoName = eventClassNames[iEventClass];
          histoName += particlePairNames[iPart];
          histoName += "_";
          histoName += observableNames[iObservable];
          TH2 * h2 =  (TH2*) inputFiles[0]->Get(histoName);
          if (!h2)
            {
            cout << "Abort -- Histogram not found: " << histoName << endl;
            cout << "in file:" << fileNames[0] << endl;
            return 1;
            }
          plotters[0]->plot(canvasName,landscapeLinear,gc2D,
                            observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                            observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                            observableTitles[iObservable],  minZ[iObservable], maxZ[iObservable],
                            h2);
          }
        }
      }







//    observableNames.push_back("CI_R2_ptPt");          observableTitles.push_back("R_{2}^{CI}(p_{T,1},p_{T,2})"); observableTitlesX.push_back("p_{T,1}");    observableTitlesY.push_back("p_{T,2}");        minX.push_back( 0.0);  maxX.push_back(2.0); minY.push_back(0.0);  maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
//    observableNames.push_back("CD_R2_ptPt");          observableTitles.push_back("R_{2}^{CD}(p_{T,1},p_{T,2})"); observableTitlesX.push_back("p_{T,1}");    observableTitlesY.push_back("p_{T,2}");        minX.push_back( 0.0);  maxX.push_back(2.0); minY.push_back(0.0);  maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
//    observableNames.push_back("CI_R2_etaEta");        observableTitles.push_back("R_{2}^{CI}(#eta_{1},#eta_{2})"); observableTitlesX.push_back("#eta_{1}");   observableTitlesY.push_back("#eta_{2}");       minX.push_back(-2.0);  maxX.push_back(2.0); minY.push_back(-2.0); maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
//    observableNames.push_back("CD_R2_etaEta");        observableTitles.push_back("R_{2}^{CD}(#eta_{1},#eta_{2})"); observableTitlesX.push_back("#eta_{1}");   observableTitlesY.push_back("#eta_{2}");       minX.push_back(-2.0);  maxX.push_back(2.0); minY.push_back(-2.0); maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
//    observableNames.push_back("CI_R2_DetaDphi_shft"); observableTitles.push_back("R_{2}^{CI}(#Delta#eta,#Delta#varphi)");observableTitlesX.push_back("#Delta#eta"); observableTitlesY.push_back("#Delta#varphi");  minX.push_back(-4.0);  maxX.push_back(4.0); minY.push_back(-1.658); maxY.push_back(4.625); minZ.push_back(5.0); maxZ.push_back(-5.0);
//    observableNames.push_back("CD_R2_DetaDphi_shft"); observableTitles.push_back("R_{2}^{CD}(#Delta#eta,#Delta#varphi)");observableTitlesX.push_back("#Delta#eta"); observableTitlesY.push_back("#Delta#varphi");  minX.push_back(-4.0);  maxX.push_back(4.0); minY.push_back(-1.658); maxY.push_back(4.625); minZ.push_back(5.0); maxZ.push_back(-5.0);
//
//    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
//      {
//
//      for (unsigned int iPart=0; iPart<particlePairNames.size(); iPart++)
//        {
//        canvasName = eventClassNames[iEventClass];
//        canvasName += observableNames[iObservable];
//        canvasName += "_";
//        canvasName += particlePairNames[iPart];
//        TString histoName = eventClassNames[iEventClass];
//        histoName += particlePairNames[iPart];
//        histoName += "_";
//        histoName += observableNames[iObservable];
//        TH2 * h2 =  (TH2*) inputFiles[1]->Get(histoName);
//        if (!h2)
//          {
//          cout << "Abort -- Histogram not found: " << histoName << endl;
//          cout << "in file:" << fileNames[1] << endl;
//          return 1;
//          }
//        plotters[0]->plot(canvasName,landscapeLinear,gc2D,
//                          observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
//                          observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
//                          observableTitles[iObservable],  minZ[iObservable], maxZ[iObservable],
//                          h2);
//        }
//      }

    }

  plotters[0]->printAllCanvas(outputPathName);

  return 0;

}

