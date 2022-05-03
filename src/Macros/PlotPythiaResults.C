
#include "TString.h"
#include "TRandom.h"
//#include "ParticlePlotter.hpp"
//#include "TFile.h"
//#include "AnalysisConfiguration.hpp"
//#include "NuDynHistos.hpp"
//#include "NuDynDerivedHistos.hpp"
//#include "CanvasConfiguration.hpp"
//#include "HistogramCollection.hpp"
//#include "GraphConfiguration.hpp"
//#include "CanvasConfiguration.hpp"
//#include "TRint.h"
//R__LOAD_LIBRARY(/Users/claudeapruneau/opt/WAC/lib/libBase.dylib)


int PlotPythiaResults()
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

 // TString inputPathName  = getenv("WAC_REPOSITORY_PATH");
  TString inputPathName  = getenv("WAC_OUTPUT_PATH");
  TString outputPathName = getenv("WAC_OUTPUT_PATH");

  inputPathName  += "/Users/claudeapruneau/Documents/GitHub/WAC4/data/Repository/PYTHIA/7TeV/100M-CRON/";
  outputPathName += "/Users/claudeapruneau/Documents/GitHub/WAC4/data/OutputFiles/PYTHIA/7TEV/100M-CRON/";

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


  if (false) // this set was done with MB PYTHIA events at 7 TeV
    {
    eventClassNames.clear();
    particleNames.clear();
    particleTitles.clear();
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
    eventClassNames.push_back("P1_MB_");  eventClassTitles.push_back("MB");

    particleNames.push_back("HC");        particleTitles.push_back("h^{#pm}");
    particleNames.push_back("HP");        particleTitles.push_back("h^{+}");
    particleNames.push_back("HM");        particleTitles.push_back("h^{-}");
    particleNames.push_back("PiC");       particleTitles.push_back("#pi^{#pm}");
    particleNames.push_back("PiP");       particleTitles.push_back("#pi^{+}");
    particleNames.push_back("PiM");       particleTitles.push_back("#pi^{-}");
    particleNames.push_back("KC");        particleTitles.push_back("K^{#pm}");
    particleNames.push_back("KP");        particleTitles.push_back("K^{+}");
    particleNames.push_back("KM");        particleTitles.push_back("K^{-}");
    particleNames.push_back("PC");        particleTitles.push_back("p^{#pm}");
    particleNames.push_back("PP");        particleTitles.push_back("p^{+}");
    particleNames.push_back("PM");        particleTitles.push_back("#bar{p}");

    observableNames.push_back("n1");      observableTitles.push_back("n_{1}");                observableTitlesX.push_back("n_{1}"); observableTitlesY.push_back("Counts");               minX.push_back(0.0);  maxX.push_back(400.0); minY.push_back(1.0E-6); maxY.push_back(1.0E1);
    observableNames.push_back("n1_pt");   observableTitles.push_back("dn_{1}/dp_{T}");        observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("dn_{1}/dp_{T}");        minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_ptXS"); observableTitles.push_back("1/p_{T}dn_{1}/dp_{T}"); observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("1/p_{T}dn_{1}/dp_{T}"); minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_eta");  observableTitles.push_back("dn_{1}/d#eta");         observableTitlesX.push_back("#eta");  observableTitlesY.push_back("dn_{1}/d#eta");         minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-3); maxY.push_back(1.0E2);
    //observableNames.push_back("pt1_eta"); observableTitles.push_back("<p_{T}>");              observableTitlesX.push_back("#eta");  observableTitlesY.push_back("<p_{T}>");              minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-2); maxY.push_back(2.0);


    unsigned int iEventClass = 0;

    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = eventClassNames[iEventClass];
      canvasName += observableNames[iObservable];
      for (unsigned int iPart=0; iPart<particleNames.size(); iPart++)
        {
        TString histoName = eventClassNames[iEventClass];
        histoName += particleNames[iPart];
        histoName += "_";
        histoName += observableNames[iObservable];
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(particleTitles[iPart]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.65, 0.6, 0.75, 0.9, 0.045);
      }

    }

  if (false) // this set was done with P1_PF0nGeq1Lt1000 based on the ALICE V0M acceptance PYTHIA events at 7 TeV
    {
    inputFiles[0]->cd();
    eventClassNames.clear();
    particleNames.clear();
    particleTitles.clear();
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
    eventClassNames.push_back("P1_PF0nGeq1Lt1000_");  eventClassTitles.push_back("V0 MB");

    particleNames.push_back("HC");        particleTitles.push_back("h^{#pm}");
    particleNames.push_back("HP");        particleTitles.push_back("h^{+}");
    particleNames.push_back("HM");        particleTitles.push_back("h^{-}");
    particleNames.push_back("PiC");       particleTitles.push_back("#pi^{#pm}");
    particleNames.push_back("PiP");       particleTitles.push_back("#pi^{+}");
    particleNames.push_back("PiM");       particleTitles.push_back("#pi^{-}");
    particleNames.push_back("KC");        particleTitles.push_back("K^{#pm}");
    particleNames.push_back("KP");        particleTitles.push_back("K^{+}");
    particleNames.push_back("KM");        particleTitles.push_back("K^{-}");
    particleNames.push_back("PC");        particleTitles.push_back("p^{#pm}");
    particleNames.push_back("PP");        particleTitles.push_back("p^{+}");
    particleNames.push_back("PM");        particleTitles.push_back("#bar{p}");

    observableNames.push_back("n1");      observableTitles.push_back("n_{1}");                observableTitlesX.push_back("n_{1}"); observableTitlesY.push_back("Counts");               minX.push_back(0.0);  maxX.push_back(400.0); minY.push_back(1.0E-6); maxY.push_back(1.0E1);
    observableNames.push_back("n1_pt");   observableTitles.push_back("dn_{1}/dp_{T}");        observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("dn_{1}/dp_{T}");        minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_ptXS"); observableTitles.push_back("1/p_{T}dn_{1}/dp_{T}"); observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("1/p_{T}dn_{1}/dp_{T}"); minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_eta");  observableTitles.push_back("dn_{1}/d#eta");         observableTitlesX.push_back("#eta");  observableTitlesY.push_back("dn_{1}/d#eta");         minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-3); maxY.push_back(1.0E2);
    //observableNames.push_back("pt1_eta"); observableTitles.push_back("<p_{T}>");              observableTitlesX.push_back("#eta");  observableTitlesY.push_back("<p_{T}>");              minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-2); maxY.push_back(2.0);

    iEventClass = 0;
    //for (unsigned int iEventClass=0; iEventClass<eventClassNames.size(); iEventClass++)
      {}

    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = eventClassNames[iEventClass];
      canvasName += observableNames[iObservable];

      for (unsigned int iPart=0; iPart<particleNames.size(); iPart++)
        {
        TString histoName = eventClassNames[iEventClass];
        histoName += particleNames[iPart];
        histoName += "_";
        histoName += observableNames[iObservable];
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(particleTitles[iPart]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.65, 0.6, 0.75, 0.9, 0.045);
      }

    }

  if (false) // this set was done with P1_PF0nGeq1Lt1000 based on the ALICE V0M acceptance PYTHIA events at 7 TeV
    {
    inputFiles[0]->cd();
    eventClassNames.clear();
    particleNames.clear();
    particleTitles.clear();
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
    //eventClassNames.push_back("P1_PF0nGeq1Lt1000_");  eventClassTitles.push_back("V0 MB");
    eventClassNames.push_back("P1_PF0nGeq0Lt7_");     eventClassTitles.push_back("0 -- 7");
    eventClassNames.push_back("P1_PF0nGeq7Lt12_");    eventClassTitles.push_back("7 -- 12");
    eventClassNames.push_back("P1_PF0nGeq12Lt20_");   eventClassTitles.push_back("12 -- 20");
    eventClassNames.push_back("P1_PF0nGeq20Lt38_");   eventClassTitles.push_back("20 -- 38");
    eventClassNames.push_back("P1_PF0nGeq38Lt400_");  eventClassTitles.push_back("38 -- 400");

    particleNames.push_back("HC");        particleTitles.push_back("h^{#pm}");
//    particleNames.push_back("PiC");       particleTitles.push_back("#pi^{#pm}");
//    particleNames.push_back("KC");        particleTitles.push_back("K^{#pm}");
//    particleNames.push_back("PC");        particleTitles.push_back("p^{#pm}");

    observableNames.push_back("n1");      observableTitles.push_back("n_{1}");                observableTitlesX.push_back("n_{1}"); observableTitlesY.push_back("Counts");               minX.push_back(0.0);  maxX.push_back(400.0); minY.push_back(1.0E-6); maxY.push_back(1.0E1);
    observableNames.push_back("n1_pt");   observableTitles.push_back("dn_{1}/dp_{T}");        observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("dn_{1}/dp_{T}");        minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_ptXS"); observableTitles.push_back("1/p_{T}dn_{1}/dp_{T}"); observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("1/p_{T}dn_{1}/dp_{T}"); minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_eta");  observableTitles.push_back("dn_{1}/d#eta");         observableTitlesX.push_back("#eta");  observableTitlesY.push_back("dn_{1}/d#eta");         minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-3); maxY.push_back(1.0E2);
    //observableNames.push_back("pt1_eta"); observableTitles.push_back("<p_{T}>");              observableTitlesX.push_back("#eta");  observableTitlesY.push_back("<p_{T}>");              minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-2); maxY.push_back(2.0);

    iEventClass = 0;


    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = "V0MSlices_";
      canvasName += observableNames[iObservable];

      for (unsigned int iEventClass=0; iEventClass<eventClassNames.size(); iEventClass++)
        {
      for (unsigned int iPart=0; iPart<particleNames.size(); iPart++)
        {
        TString histoName = eventClassNames[iEventClass];
        histoName += particleNames[iPart];
        histoName += "_";
        histoName += observableNames[iObservable];
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(eventClassTitles[iEventClass]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.55, 0.6, 0.75, 0.9, 0.03);
      }

    }

  if (false) // PIONS
    {
    inputFiles[0]->cd();
    eventClassNames.clear();
    particleNames.clear();
    particleTitles.clear();
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
    //eventClassNames.push_back("P1_PF0nGeq1Lt1000_");  eventClassTitles.push_back("V0 MB");
    eventClassNames.push_back("P1_PF0nGeq0Lt7_");     eventClassTitles.push_back("0 -- 7");
    eventClassNames.push_back("P1_PF0nGeq7Lt12_");    eventClassTitles.push_back("7 -- 12");
    eventClassNames.push_back("P1_PF0nGeq12Lt20_");   eventClassTitles.push_back("12 -- 20");
    eventClassNames.push_back("P1_PF0nGeq20Lt38_");   eventClassTitles.push_back("20 -- 38");
    eventClassNames.push_back("P1_PF0nGeq38Lt400_");  eventClassTitles.push_back("38 -- 400");

//    particleNames.push_back("HC");        particleTitles.push_back("h^{#pm}");
    particleNames.push_back("PiC");       particleTitles.push_back("#pi^{#pm}");
//    particleNames.push_back("KC");        particleTitles.push_back("K^{#pm}");
//    particleNames.push_back("PC");        particleTitles.push_back("p^{#pm}");

//    observableNames.push_back("n1");      observableTitles.push_back("n_{1}");                observableTitlesX.push_back("n_{1}"); observableTitlesY.push_back("Counts");               minX.push_back(0.0);  maxX.push_back(400.0); minY.push_back(1.0E-6); maxY.push_back(1.0E1);
//    observableNames.push_back("n1_pt");   observableTitles.push_back("dn_{1}/dp_{T}");        observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("dn_{1}/dp_{T}");        minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_ptXS"); observableTitles.push_back("1/p_{T}dn_{1}/dp_{T}"); observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("1/p_{T}dn_{1}/dp_{T}"); minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
//    observableNames.push_back("n1_eta");  observableTitles.push_back("dn_{1}/d#eta");         observableTitlesX.push_back("#eta");  observableTitlesY.push_back("dn_{1}/d#eta");         minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-3); maxY.push_back(1.0E2);
    //observableNames.push_back("pt1_eta"); observableTitles.push_back("<p_{T}>");              observableTitlesX.push_back("#eta");  observableTitlesY.push_back("<p_{T}>");              minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-2); maxY.push_back(2.0);

    iEventClass = 0;


    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = "Pion_V0MSlices_";
      canvasName += observableNames[iObservable];

      for (unsigned int iEventClass=0; iEventClass<eventClassNames.size(); iEventClass++)
        {
      for (unsigned int iPart=0; iPart<particleNames.size(); iPart++)
        {
        TString histoName = eventClassNames[iEventClass];
        histoName += particleNames[iPart];
        histoName += "_";
        histoName += observableNames[iObservable];
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(eventClassTitles[iEventClass]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.55, 0.6, 0.75, 0.9, 0.03);
      }

    }

  if (false) // KAONS
    {
    inputFiles[0]->cd();
    eventClassNames.clear();
    particleNames.clear();
    particleTitles.clear();
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
    //eventClassNames.push_back("P1_PF0nGeq1Lt1000_");  eventClassTitles.push_back("V0 MB");
    eventClassNames.push_back("P1_PF0nGeq0Lt7_");     eventClassTitles.push_back("0 -- 7");
    eventClassNames.push_back("P1_PF0nGeq7Lt12_");    eventClassTitles.push_back("7 -- 12");
    eventClassNames.push_back("P1_PF0nGeq12Lt20_");   eventClassTitles.push_back("12 -- 20");
    eventClassNames.push_back("P1_PF0nGeq20Lt38_");   eventClassTitles.push_back("20 -- 38");
    eventClassNames.push_back("P1_PF0nGeq38Lt400_");  eventClassTitles.push_back("38 -- 400");

//    particleNames.push_back("HC");        particleTitles.push_back("h^{#pm}");
//    particleNames.push_back("PiC");       particleTitles.push_back("#pi^{#pm}");
    particleNames.push_back("KC");        particleTitles.push_back("K^{#pm}");
//    particleNames.push_back("PC");        particleTitles.push_back("p^{#pm}");

//    observableNames.push_back("n1");      observableTitles.push_back("n_{1}");                observableTitlesX.push_back("n_{1}"); observableTitlesY.push_back("Counts");               minX.push_back(0.0);  maxX.push_back(400.0); minY.push_back(1.0E-6); maxY.push_back(1.0E1);
//    observableNames.push_back("n1_pt");   observableTitles.push_back("dn_{1}/dp_{T}");        observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("dn_{1}/dp_{T}");        minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_ptXS"); observableTitles.push_back("1/p_{T}dn_{1}/dp_{T}"); observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("1/p_{T}dn_{1}/dp_{T}"); minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
//    observableNames.push_back("n1_eta");  observableTitles.push_back("dn_{1}/d#eta");         observableTitlesX.push_back("#eta");  observableTitlesY.push_back("dn_{1}/d#eta");         minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-3); maxY.push_back(1.0E2);
    //observableNames.push_back("pt1_eta"); observableTitles.push_back("<p_{T}>");              observableTitlesX.push_back("#eta");  observableTitlesY.push_back("<p_{T}>");              minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-2); maxY.push_back(2.0);

    iEventClass = 0;


    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = "Kaon_V0MSlices_";
      canvasName += observableNames[iObservable];

      for (unsigned int iEventClass=0; iEventClass<eventClassNames.size(); iEventClass++)
        {
      for (unsigned int iPart=0; iPart<particleNames.size(); iPart++)
        {
        TString histoName = eventClassNames[iEventClass];
        histoName += particleNames[iPart];
        histoName += "_";
        histoName += observableNames[iObservable];
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(eventClassTitles[iEventClass]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.55, 0.6, 0.75, 0.9, 0.03);
      }

    }

  if (false) // PROTONS
    {
    inputFiles[0]->cd();
    eventClassNames.clear();
    particleNames.clear();
    particleTitles.clear();
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
    //eventClassNames.push_back("P1_PF0nGeq1Lt1000_");  eventClassTitles.push_back("V0 MB");
    eventClassNames.push_back("P1_PF0nGeq0Lt7_");     eventClassTitles.push_back("0 -- 7");
    eventClassNames.push_back("P1_PF0nGeq7Lt12_");    eventClassTitles.push_back("7 -- 12");
    eventClassNames.push_back("P1_PF0nGeq12Lt20_");   eventClassTitles.push_back("12 -- 20");
    eventClassNames.push_back("P1_PF0nGeq20Lt38_");   eventClassTitles.push_back("20 -- 38");
    eventClassNames.push_back("P1_PF0nGeq38Lt400_");  eventClassTitles.push_back("38 -- 400");

//    particleNames.push_back("HC");        particleTitles.push_back("h^{#pm}");
//    particleNames.push_back("PiC");       particleTitles.push_back("#pi^{#pm}");
//    particleNames.push_back("KC");        particleTitles.push_back("K^{#pm}");
    particleNames.push_back("PC");        particleTitles.push_back("p^{#pm}");

//    observableNames.push_back("n1");      observableTitles.push_back("n_{1}");                observableTitlesX.push_back("n_{1}"); observableTitlesY.push_back("Counts");               minX.push_back(0.0);  maxX.push_back(400.0); minY.push_back(1.0E-6); maxY.push_back(1.0E1);
//    observableNames.push_back("n1_pt");   observableTitles.push_back("dn_{1}/dp_{T}");        observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("dn_{1}/dp_{T}");        minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
    observableNames.push_back("n1_ptXS"); observableTitles.push_back("1/p_{T}dn_{1}/dp_{T}"); observableTitlesX.push_back("p_{T}"); observableTitlesY.push_back("1/p_{T}dn_{1}/dp_{T}"); minX.push_back(0.0);  maxX.push_back(10.0);  minY.push_back(1.0E-7); maxY.push_back(1.0E2);
//    observableNames.push_back("n1_eta");  observableTitles.push_back("dn_{1}/d#eta");         observableTitlesX.push_back("#eta");  observableTitlesY.push_back("dn_{1}/d#eta");         minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-3); maxY.push_back(1.0E2);
    //observableNames.push_back("pt1_eta"); observableTitles.push_back("<p_{T}>");              observableTitlesX.push_back("#eta");  observableTitlesY.push_back("<p_{T}>");              minX.push_back(-2.0); maxX.push_back(2.0);   minY.push_back(1.0E-2); maxY.push_back(2.0);

    iEventClass = 0;


    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = "Proton_V0MSlices_";
      canvasName += observableNames[iObservable];

      for (unsigned int iEventClass=0; iEventClass<eventClassNames.size(); iEventClass++)
        {
      for (unsigned int iPart=0; iPart<particleNames.size(); iPart++)
        {
        TString histoName = eventClassNames[iEventClass];
        histoName += particleNames[iPart];
        histoName += "_";
        histoName += observableNames[iObservable];
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(eventClassTitles[iEventClass]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.55, 0.6, 0.75, 0.9, 0.03);
      }

    }


  // =================
  // Pairs
  // ===============
  if (false)
    {
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

  eventClassNames.push_back("P2_MB_");  eventClassTitles.push_back("MB");

  particleNames.push_back("PiP");       particleTitles.push_back("#pi^{+}");
  particleNames.push_back("PiM");       particleTitles.push_back("#pi^{-}");
  particleNames.push_back("KP");        particleTitles.push_back("K^{+}");
  particleNames.push_back("KM");        particleTitles.push_back("K^{-}");
  particleNames.push_back("PP");        particleTitles.push_back("p^{+}");
  particleNames.push_back("PM");        particleTitles.push_back("#bar{p}");

  TString name;
  particlePairNames.clear();
  particlePairTitles.clear();

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
  observableNames.push_back("R2_ptPt");          observableTitles.push_back("R_{2}(p_{T,1},p_{T,2})");          observableTitlesX.push_back("p_{T,1}");    observableTitlesY.push_back("p_{T,2}");        minX.push_back( 0.0);  maxX.push_back(2.0); minY.push_back(0.0);  maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
  observableNames.push_back("R2_etaEta");        observableTitles.push_back("R_{2}(#eta_{1},#eta_{2})");        observableTitlesX.push_back("#eta_{1}");   observableTitlesY.push_back("#eta_{2}");       minX.push_back(-2.0);  maxX.push_back(2.0); minY.push_back(-2.0); maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
  observableNames.push_back("R2_DetaDphi_shft"); observableTitles.push_back("R_{2}(#Delta#eta,#Delta#varphi)"); observableTitlesX.push_back("#Delta#eta"); observableTitlesY.push_back("#Delta#varphi");  minX.push_back(-4.0);  maxX.push_back(4.0); minY.push_back(-1.658); maxY.push_back(4.625); minZ.push_back(5.0); maxZ.push_back(-5.0);

  histograms.clear();

//  for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
//    {
//
//    for (unsigned int iPart=0; iPart<particlePairNames.size(); iPart++)
//      {
//      canvasName = eventClassNames[iEventClass];
//      canvasName += observableNames[iObservable];
//      canvasName += "_";
//      canvasName += particlePairNames[iPart];
//      TString histoName = eventClassNames[iEventClass];
//      histoName += particlePairNames[iPart];
//      histoName += "_";
//      histoName += observableNames[iObservable];
//      TH2 * h2 =  (TH2*) inputFiles[1]->Get(histoName);
//      if (!h2)
//        {
//        cout << "Abort -- Histogram not found: " << histoName << endl;
//        cout << "in file:" << fileNames[1] << endl;
//        return 1;
//        }
//      plotters[0]->plot(canvasName,landscapeLinear,gc2D,
//                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
//                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
//                        observableTitles[iObservable],  minZ[iObservable], maxZ[iObservable],
//                        h2);
//      }
//    }

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
  
  particlePairNames.push_back("PiPPiM");
  particlePairNames.push_back("PiPKP");
  particlePairNames.push_back("PiPPP");
  particlePairNames.push_back("KPPiP");
  particlePairNames.push_back("KPKM");
  particlePairNames.push_back("KPPP");
  particlePairNames.push_back("PPPiP");
  particlePairNames.push_back("PPKP");
  particlePairNames.push_back("PPPM");


  observableNames.push_back("CI_R2_ptPt");          observableTitles.push_back("R_{2}^{CI}(p_{T,1},p_{T,2})"); observableTitlesX.push_back("p_{T,1}");    observableTitlesY.push_back("p_{T,2}");        minX.push_back( 0.0);  maxX.push_back(2.0); minY.push_back(0.0);  maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
  observableNames.push_back("CD_R2_ptPt");          observableTitles.push_back("R_{2}^{CD}(p_{T,1},p_{T,2})"); observableTitlesX.push_back("p_{T,1}");    observableTitlesY.push_back("p_{T,2}");        minX.push_back( 0.0);  maxX.push_back(2.0); minY.push_back(0.0);  maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
  observableNames.push_back("CI_R2_etaEta");        observableTitles.push_back("R_{2}^{CI}(#eta_{1},#eta_{2})"); observableTitlesX.push_back("#eta_{1}");   observableTitlesY.push_back("#eta_{2}");       minX.push_back(-2.0);  maxX.push_back(2.0); minY.push_back(-2.0); maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
  observableNames.push_back("CD_R2_etaEta");        observableTitles.push_back("R_{2}^{CD}(#eta_{1},#eta_{2})"); observableTitlesX.push_back("#eta_{1}");   observableTitlesY.push_back("#eta_{2}");       minX.push_back(-2.0);  maxX.push_back(2.0); minY.push_back(-2.0); maxY.push_back(2.0); minZ.push_back(5.0); maxZ.push_back(-5.0);
  observableNames.push_back("CI_R2_DetaDphi_shft"); observableTitles.push_back("R_{2}^{CI}(#Delta#eta,#Delta#varphi)");observableTitlesX.push_back("#Delta#eta"); observableTitlesY.push_back("#Delta#varphi");  minX.push_back(-4.0);  maxX.push_back(4.0); minY.push_back(-1.658); maxY.push_back(4.625); minZ.push_back(5.0); maxZ.push_back(-5.0);
  observableNames.push_back("CD_R2_DetaDphi_shft"); observableTitles.push_back("R_{2}^{CD}(#Delta#eta,#Delta#varphi)");observableTitlesX.push_back("#Delta#eta"); observableTitlesY.push_back("#Delta#varphi");  minX.push_back(-4.0);  maxX.push_back(4.0); minY.push_back(-1.658); maxY.push_back(4.625); minZ.push_back(5.0); maxZ.push_back(-5.0);

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
      TH2 * h2 =  (TH2*) inputFiles[1]->Get(histoName);
      if (!h2)
        {
        cout << "Abort -- Histogram not found: " << histoName << endl;
        cout << "in file:" << fileNames[1] << endl;
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

  if (true)
    {
    TFile * wColor2 = new TFile("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/V0AccTestHadronsOnlyWithColorReconn2/PYTHIA_pp_7TeV_inelastic_Global.root");
    TFile * noColor1 = new TFile("/Users/claudeapruneau/Documents/GitHub/WAC4/data/OutputFiles/PYTHIA/7TEV/V0AccTestHadronsOnlyWithNoColorReconn2/PYTHIA_pp_7TeV_inelastic_Global-NoColorReconn.root");
    TFile * noColor2 = new TFile("/Users/claudeapruneau/Documents/GitHub/WAC4/data/OutputFiles/PYTHIA/7TEV/V0AccTestHadronsOnlyWithNoColorReconn2/ReconnectionOff.root");
    TFile * victor   = new TFile("/Users/claudeapruneau/Documents/GitHub/WAC4/data/OutputFiles/PYTHIA/7TEV/V0AccTestHadronsOnlyWithNoColorReconn2/PrimChMultiplicities.root");

    // this next file is in the wrong location and was generated with color reco on.
    TFile * wColor3  = new TFile("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/V0AccTestHadronsOnlyWithColorReconn/PYTHIA_pp_7TeV_inelastic_G1.root");
    TFile * wColor4  = new TFile("/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/V0AccTestHadronsOnlyWithColorReconnNoWeakDecay/PYTHIA_pp_7TeV_inelastic_G1.root");

    TH1 * hWCol2 = (TH1*) wColor2->Get("Global_MB_V0M_n");
    TH1 * hWCol3 = (TH1*) wColor3->Get("G1_PF0nGeq1Lt1000_V0M_n");
    TH1 * hWCol4 = (TH1*) wColor4->Get("G1_PF0nGeq1Lt1000_V0M_n");

    TH1 * hNoCol = (TH1*) noColor1->Get("Global_MB_V0M_n");
    TH1 * hVic   = (TH1*) victor->Get("V0M");

    TH2 * hWCol2D = (TH2*) wColor2->Get("Global_MB_V0M_n_HC_n");
    TH2 * hNoCol2D = (TH2*) noColor1->Get("Global_MB_V0M_n_HC_n");

   // hWCol2->Draw();

    TCanvas * c1 = new TCanvas();
    hWCol2D->Draw("ZCOL");
    TH1 * hWColProX = hWCol2D->ProjectionX("WColX",2,-1);

    TCanvas * c2 = new TCanvas();
    hNoCol2D->Draw("ZCOL");
    TH1 * hNoColProX = hNoCol2D->ProjectionX("NoColX",2,-1);

    TCanvas * c3 = new TCanvas();

//    hNoCol->SetLineColor(2);
//    hNoCol->SetMarkerColor(2);
//    hNoCol->SetMarkerStyle(22);
//    hNoCol->Draw();

    hVic->SetLineColor(6);
    hVic->SetMarkerColor(6);
    hVic->SetMarkerStyle(21);
    hVic->Draw();


    hNoColProX->SetLineColor(4);
    hNoColProX->SetMarkerColor(4);
    hNoColProX->SetMarkerStyle(20);
    hNoColProX->Draw("SAME");

//    hWColProX->SetLineColor(2);
//    hWColProX->SetMarkerColor(2);
//    hWColProX->SetMarkerStyle(23);
//    hWColProX->Scale(0.041/0.034);
//    hWColProX->Draw("SAME");

    hWCol3->SetLineColor(2);
    hWCol3->SetMarkerSize(1.65);
    hWCol3->SetMarkerColor(2);
    hWCol3->SetMarkerStyle(23);
    hWCol3->Draw("SAME");

    hWCol4->SetLineColor(5);
    hWCol4->SetMarkerSize(1.65);
    hWCol4->SetMarkerColor(5);
    hWCol4->SetMarkerStyle(24);
    hWCol4->Draw("SAME");


    }

  //plotters[0]->printAllCanvas(outputPathName);

  return 0;
  //  canvasConfiguration->setAxes(CanvasConfiguration::LogY);
  //  canvasName = "ModelComp_HC_MultDist";
  //  histograms[0] = particleHistos[0]->h_n1; titles[0] = modelTitles[0];
  //  histograms[1] = particleHistos[nParticleTypes]->h_n1; titles[1] = modelTitles[1];
  //  compPlotters[0] = new Plotter();
  //  compPlotters[0]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
  //       "N_{h^{#pm}}", 0.0, 200.0,
  //       "Counts", 1.0E-6, 5.0E-1,
  //       histograms,titles,0.55, 0.75, 0.8, 0.9, 0.04);
  //  compPlotters[0]->printAllCanvas(outputPath);
  //
  //
  //  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  //  canvasName = "ModelComp_HC_dndeta";
  //  histograms[0] = particleHistos[0]->h_n1_eta; titles[0] = modelTitles[0];
  //  histograms[1] = particleHistos[nParticleTypes]->h_n1_eta; titles[1] = modelTitles[1];
  //  compPlotters[1] = new Plotter();
  //  compPlotters[1]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
  //                    "#eta", -6.0, 6.0,
  //                    "dN/d#eta", 0.0, 2.0,
  //                    histograms,titles,0.55, 0.17, 0.85, 0.35, 0.04);
  //  compPlotters[1]->printAllCanvas(outputPath);
  //
  //  canvasConfiguration->setAxes(CanvasConfiguration::LogY);
  //  canvasName = "ModelComp_HC_dnpTdpT";
  //  histograms[0] = particleHistos[0]->h_n1_ptXS; titles[0] = modelTitles[0];
  //  histograms[1] = particleHistos[nParticleTypes]->h_n1_ptXS; titles[1] = modelTitles[1];
  //  compPlotters[2] = new Plotter();
  //  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
  //                    "p_{T}", 0.0, 40.0,
  //                    "dN/p_{T}dp_{T}", 1.0E-9, 3.0E1,
  //                    histograms,titles,0.55, 0.6, 0.85, 0.9, 0.055);
  //  compPlotters[2]->printAllCanvas(outputPath);
  //
  //  return 0;
}

