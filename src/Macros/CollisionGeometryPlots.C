
#include "TString.h"
#include "TRandom.h"
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

double getHistoSum(TH1 * h);

int CollisionGeometryPlots()
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
  gSystem->Load(includesPath+"RandomGenerators.hpp");
  gSystem->Load(includesPath+"RapidityGenerator.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskCollection.hpp");
  gSystem->Load(includesPath+"Property.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"AnalysisConfiguration.hpp");
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
  gSystem->Load(includesPath+"TrackAndPairConfiguration.hpp");
  gSystem->Load(includesPath+"NuDynTask.hpp");
  gSystem->Load(includesPath+"NuDynHistos.hpp");
  gSystem->Load(includesPath+"NuDynDerivedHistos.hpp");
  gSystem->Load(includesPath+"ParticleHistos.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"CollisionGeometryHistograms.hpp");
  gSystem->Load("libBase.dylib");

  HistogramCollection * histogramCollection = new HistogramCollection("Collection",200);
  histogramCollection->setDefaultOptions(1);
  CanvasCollection    * canvasCollection    = new CanvasCollection();
  CanvasConfiguration * canvasConfiguration = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  GraphConfiguration  ** graphConfigurations = new GraphConfiguration*[40];
  for (int iGraph=0;iGraph<40;iGraph++)
  {
  graphConfigurations[iGraph] = new GraphConfiguration(1,iGraph%10);
  graphConfigurations[iGraph]->markerSize = 0.5;
  graphConfigurations[iGraph]->plotOption = "E0";
  }

  TString inputPath  = "/Users/claudeapruneau/Documents/GitHub/run/GeometryStudies/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/run/GeometryStudies/";


  int nFiles            = 3;
  TString ** fileNames = new TString*[nFiles];
  TString fileName;
  fileNames[0] = new TString("CollisionGeometryPbPbWS-Nominal.root");
  fileNames[1] = new TString("CollisionGeometryPbPbWS-ConfigB.root");
  fileNames[2] = new TString("CollisionGeometryPbPbWS-ConfigC.root");
  //fileNames[2] = new TString("hardOffPYTHIA_Singles_MB.root");

  //  fileNames[1] = new TString("softOffPYTHIA_Singles_MB.root");
  //  fileNames[2] = new TString("softOffPYTHIA_Singles_MB.root");
  TFile ** inputFiles  = new TFile* [nFiles];
  for (int iFile=0;iFile<nFiles;iFile++)
  {
  fileName = inputPath+*fileNames[iFile];
  inputFiles[iFile] = new TFile(fileName,"OLD");
  if (inputFiles[iFile])
    {
    cout << "<I> CollisionGeometryPlots() Successfully opened: " << fileName << endl;
    }
  else
    {
    cout << "<E> CollisionGeometryPlots() Unable to open: " << fileName << endl;
    return 1;
    }
  }

  int nModels = nFiles;
  TString ** modelNames = new TString*[nModels];
  modelNames[0] = new TString("CollisionGeometryPbPbWS-Nominal");
  modelNames[1] = new TString("CollisionGeometryPbPbWS-ConfigB");
  modelNames[2] = new TString("CollisionGeometryPbPbWS-ConfigC");
  TString ** modelTitles = new TString*[nModels];
  modelTitles[0] = new TString("Nominal");
  modelTitles[1] = new TString("Config B");
  modelTitles[2] = new TString("Config C");

  CollisionGeometryConfiguration * ac = new CollisionGeometryConfiguration("CollisionGeometry","CollisionGeometry","1.0");
  ac->histoBaseName =  "geom";

  CollisionGeometryHistograms  ** geomHistos = new CollisionGeometryHistograms *[nModels];
  cout << "<I> CollisionGeometryPlots()  Loading histograms" << endl;
  for (int iModel=0;iModel<nModels;iModel++)
  {
  cout << "iModel:" << iModel << " Name:" <<  *modelTitles[iModel]  << endl;
  geomHistos[iModel] = new CollisionGeometryHistograms(inputFiles[iModel],"geom",ac,MessageLogger::Debug);
  }
  TString canvasNameBase = "GCS_";
  canvasNameBase += "_";


  double ss0, ss1, ss2;
  double s0, s1, s2;
  double r0, r1, r2;

  Plotter ** compPlotters = new Plotter*[40];
  TString canvasName;
  TH1 ** histograms = new TH1*[20];
  TString ** titles = new TString*[20];
  int iPlotter = 0;

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_bDistribution";
  histograms[0] = geomHistos[0]->h_b; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_b; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_b; titles[2] = modelTitles[2];
  ss0 = getHistoSum(histograms[0]);
  ss1 = getHistoSum(histograms[1]);
  ss2 = getHistoSum(histograms[2]);
  histograms[0]->Scale(1.0/ss0);
  histograms[1]->Scale(1.0/ss1);
  histograms[2]->Scale(1.0/ss2);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 24.0,
                        "Rel. Freq. (a.u.)", 1.0E-6, 0.03,
                        histograms,titles,0.25, 0.65, 0.35, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::LogY);
  canvasName = "ModelComp_nPart";
  histograms[0] = geomHistos[0]->h_nPart; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPart; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPart; titles[2] = modelTitles[2];
  s0 = getHistoSum(histograms[0]);
  s1 = getHistoSum(histograms[1]);
  s2 = getHistoSum(histograms[2]);
  histograms[0]->Scale(1.0/s0);
  histograms[1]->Scale(1.0/s1);
  histograms[2]->Scale(1.0/s2);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}", 0.0, 460.0,
                        "Rel. Freq. (a.u.)", 1.0E-6, 1.0,
                        histograms,titles,0.25, 0.25, 0.45, 0.45, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::LogY);
  canvasName = "ModelComp_nBinary";
  histograms[0] = geomHistos[0]->h_nBinary; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinary; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinary; titles[2] = modelTitles[2];
  r0 = getHistoSum(histograms[0]);
  r1 = getHistoSum(histograms[1]);
  r2 = getHistoSum(histograms[2]);
  histograms[0]->Scale(1.0/r0);
  histograms[1]->Scale(1.0/r1);
  histograms[2]->Scale(1.0/r2);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                               "N_{bin}", 0.0, 2400.0,
                               "Rel. Freq. (a.u.)", 1.0E-6, 1.0,
                        histograms,titles,0.55, 0.75, 0.8, 0.9, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartVsB_Prof";
  histograms[0] = geomHistos[0]->h_nPartVsB_Prof; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartVsB_Prof; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartVsB_Prof; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 20.0,
                        "<N_{part}>", 0.0, 450,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryVsB_Prof";
  histograms[0] = geomHistos[0]->h_nBinaryVsB_Prof; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryVsB_Prof; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryVsB_Prof; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 20.0,
                        "<N_{bin}>", 0.0, 2500.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);



  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryVsNPart_Prof";
  histograms[0] = geomHistos[0]->h_nBinaryVsNPart_Prof; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryVsNPart_Prof; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryVsNPart_Prof; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}", 0.0, 450.0,
                        "<N_{bin}>", 0.0, 2500.0,
                        histograms,titles,0.25, 0.55, 0.45, 0.75, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartRmsVsB";
  histograms[0] = geomHistos[0]->h_nPartRmsVsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartRmsVsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartRmsVsB; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 20.0,
                        "RMS[N_{part}]", 0.0, 20.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryRmsVsB";
  histograms[0] = geomHistos[0]->h_nBinaryRmsVsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryRmsVsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryRmsVsB; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 20.0,
                        "RMS[N_{bin}]", 0.0, 120.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartRmsVsXsect";
  histograms[0] = geomHistos[0]->h_nPartRmsVsXsect; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartRmsVsXsect; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartRmsVsXsect; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                               "b (fm)", 0.0, 20.0,
                               "RMS(N_{part})",  0.0, 30.0,
                               histograms,titles,0.75, 0.65, 0.85, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryRmsVsXsect";
  histograms[0] = geomHistos[0]->h_nBinaryRmsVsXsect; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryRmsVsXsect; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryRmsVsXsect; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                               "b (fm)", 0.0, 20.0,
                               "RMS(N_{bin})",  0.0, 200.0,
                               histograms,titles,0.75, 0.65, 0.85, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartRmsVsBOverlay";
  histograms[0] = geomHistos[0]->h_nPartRmsVsB;     titles[0] = new TString("fine binning");
  histograms[1] = geomHistos[0]->h_nPartRmsVsXsect; titles[1] = new TString("xsect decile");
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(2, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 24.0,
                        "RMS[N_{part}]", 0.0, 30.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryRmsVsBOverlay";
  histograms[0] = geomHistos[0]->h_nBinaryRmsVsB;     titles[0] = new TString("fine binning");
  histograms[1] = geomHistos[0]->h_nBinaryRmsVsXsect; titles[1] = new TString("xsect decile");
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(2, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 24.0,
                        "RMS[N_{Bin}]", 0.0, 200.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartOmegaVsBOverlay";
  histograms[0] = geomHistos[0]->h_nPartOmegaVsB;     titles[0] = new TString("fine binning");
  histograms[1] = geomHistos[0]->h_nPartOmegaVsXsect; titles[1] = new TString("xsect decile");
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(2, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 24.0,
                        "#omega[N_{part}]", 0.0, 5.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[iPlotter]->createLabel(2.0, 1.0, 90.0, 1, 1, 0.04, "0-5%", true);
  compPlotters[iPlotter]->createLabel(4.5, 1.0, 90.0, 1, 1, 0.04, "5-10%", true);
  compPlotters[iPlotter]->createLabel(6.0, 2.8, 90.0, 1, 1, 0.04, "10-20%", true);
  compPlotters[iPlotter]->createLabel(8.0, 2.9, 90.0, 1, 1, 0.04, "20-30%", true);
  compPlotters[iPlotter]->createLabel(9.5,  3.0, 90.0, 1, 1, 0.04, "30-40%", true);
  compPlotters[iPlotter]->createLabel(10.7, 3.4, 90.0, 1, 1, 0.04, "40-50%", true);
  compPlotters[iPlotter]->createLabel(11.8, 3.5, 90.0, 1, 1, 0.04, "50-60%", true);
  compPlotters[iPlotter]->createLabel(12.8, 4.0, 90.0, 1, 1, 0.04, "60-70%", true);
  compPlotters[iPlotter]->createLabel(14.0, 3.9, 90.0, 1, 1, 0.04, "70-80%", true);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryOmegaVsBOverlay";
  histograms[0] = geomHistos[0]->h_nBinaryOmegaVsB;     titles[0] = new TString("fine binning");
  histograms[1] = geomHistos[0]->h_nBinaryOmegaVsXsect; titles[1] = new TString("xsect decile");
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(2, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 24.0,
                        "#omega[N_{Bin}]", 0.0, 30.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[iPlotter]->createLabel(2.0, 14.0, 90.0, 1, 1, 0.04, "0-5%", true);
  compPlotters[iPlotter]->createLabel(4.5, 14.0, 90.0, 1, 1, 0.04, "5-10%", true);
  compPlotters[iPlotter]->createLabel(6.0, 22.0, 90.0, 1, 1, 0.04, "10-20%", true);
  compPlotters[iPlotter]->createLabel(8.0, 19.0, 90.0, 1, 1, 0.04, "20-30%", true);
  compPlotters[iPlotter]->createLabel(9.5, 17.0, 90.0, 1, 1, 0.04, "30-40%", true);
  compPlotters[iPlotter]->createLabel(11.0, 16.0, 90.0, 1, 1, 0.04, "40-50%", true);
  compPlotters[iPlotter]->createLabel(12.0, 12.0, 90.0, 1, 1, 0.04, "50-60%", true);
  compPlotters[iPlotter]->createLabel(13.0, 11.0, 90.0, 1, 1, 0.04, "60-70%", true);
  compPlotters[iPlotter]->createLabel(14.0, 10.0, 90.0, 1, 1, 0.04, "70-80%", true);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);






  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryRmsVsNPart";
  histograms[0] = geomHistos[0]->h_nBinaryRmsVsNPart; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryRmsVsNPart; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryRmsVsNPart; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}", 0.0, 440.0,
                        "RMS[N_{bin}]", 0.0, 120.0,
                        histograms,titles,0.25, 0.65, 0.45, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartOmegaVsB";
  histograms[0] = geomHistos[0]->h_nPartOmegaVsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartOmegaVsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartOmegaVsB; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 20.0,
                        "#omega[N_{Part}]", 0.0, 4.0,
                        histograms,titles,0.25, 0.65, 0.45, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinarytOmegaVsB";
  histograms[0] = geomHistos[0]->h_nBinaryOmegaVsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryOmegaVsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryOmegaVsB; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 20.0,
                        "#omega[N_{bin}]", 0.0, 5.0,
                        histograms,titles,0.25, 0.65, 0.45, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryOmegaVsNPart";
  histograms[0] = geomHistos[0]->h_nBinaryOmegaVsNPart; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryOmegaVsNPart; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryOmegaVsNPart; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}", 0.0, 440.0,
                        "#omega[N_{bin}]", 0.0, 15.0,
                        histograms,titles,0.25, 0.65, 0.45, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  int nCoarseBins = geomHistos[0]->h_nPartVsXsect_Prof->GetNbinsX();
  double * nPartAvg0   = new double[nCoarseBins];   double * enPartAvg0 = new double[nCoarseBins];
  double * nPartAvg1   = new double[nCoarseBins];   double * enPartAvg1 = new double[nCoarseBins];
  double * nPartAvg2   = new double[nCoarseBins];   double * enPartAvg2 = new double[nCoarseBins];
  double * nPartRms0   = new double[nCoarseBins];   double * enPartRms0 = new double[nCoarseBins];
  double * nPartRms1   = new double[nCoarseBins];   double * enPartRms1 = new double[nCoarseBins];
  double * nPartRms2   = new double[nCoarseBins];   double * enPartRms2 = new double[nCoarseBins];
  double * nPartOmega0 = new double[nCoarseBins];   double * enPartOmega0 = new double[nCoarseBins];
  double * nPartOmega1 = new double[nCoarseBins];   double * enPartOmega1 = new double[nCoarseBins];
  double * nPartOmega2 = new double[nCoarseBins];   double * enPartOmega2 = new double[nCoarseBins];

  double * nBinaryRms0   = new double[nCoarseBins];   double * enBinaryRms0 = new double[nCoarseBins];
  double * nBinaryRms1   = new double[nCoarseBins];   double * enBinaryRms1 = new double[nCoarseBins];
  double * nBinaryRms2   = new double[nCoarseBins];   double * enBinaryRms2 = new double[nCoarseBins];
  double * nBinaryOmega0 = new double[nCoarseBins];   double * enBinaryOmega0 = new double[nCoarseBins];
  double * nBinaryOmega1 = new double[nCoarseBins];   double * enBinaryOmega1 = new double[nCoarseBins];
  double * nBinaryOmega2 = new double[nCoarseBins];   double * enBinaryOmega2 = new double[nCoarseBins];


  for (int iBin=1; iBin<=nCoarseBins; iBin++)
  {
  int iBin1 = iBin-1;
  nPartAvg0[iBin1] = geomHistos[0]->h_nPartVsXsect_Prof->GetBinContent(iBin); enPartAvg0[iBin1] = geomHistos[0]->h_nPartVsXsect_Prof->GetBinError(iBin);
  nPartAvg1[iBin1] = geomHistos[1]->h_nPartVsXsect_Prof->GetBinContent(iBin); enPartAvg1[iBin1] = geomHistos[1]->h_nPartVsXsect_Prof->GetBinError(iBin);
  nPartAvg2[iBin1] = geomHistos[2]->h_nPartVsXsect_Prof->GetBinContent(iBin); enPartAvg2[iBin1] = geomHistos[2]->h_nPartVsXsect_Prof->GetBinError(iBin);

  nPartRms0[iBin1] = geomHistos[0]->h_nPartRmsVsXsect->GetBinContent(iBin); enPartRms0[iBin1] = geomHistos[0]->h_nPartRmsVsXsect->GetBinError(iBin);
  nPartRms1[iBin1] = geomHistos[1]->h_nPartRmsVsXsect->GetBinContent(iBin); enPartRms1[iBin1] = geomHistos[1]->h_nPartRmsVsXsect->GetBinError(iBin);
  nPartRms2[iBin1] = geomHistos[2]->h_nPartRmsVsXsect->GetBinContent(iBin); enPartRms2[iBin1] = geomHistos[2]->h_nPartRmsVsXsect->GetBinError(iBin);

  nPartOmega0[iBin1] = geomHistos[0]->h_nPartOmegaVsXsect->GetBinContent(iBin); enPartOmega0[iBin1] = geomHistos[0]->h_nPartOmegaVsXsect->GetBinError(iBin);
  nPartOmega1[iBin1] = geomHistos[1]->h_nPartOmegaVsXsect->GetBinContent(iBin); enPartOmega1[iBin1] = geomHistos[1]->h_nPartOmegaVsXsect->GetBinError(iBin);
  nPartOmega2[iBin1] = geomHistos[2]->h_nPartOmegaVsXsect->GetBinContent(iBin); enPartOmega2[iBin1] = geomHistos[2]->h_nPartOmegaVsXsect->GetBinError(iBin);

  nBinaryRms0[iBin1] = geomHistos[0]->h_nBinaryRmsVsXsect->GetBinContent(iBin); enBinaryRms0[iBin1] = geomHistos[0]->h_nBinaryRmsVsXsect->GetBinError(iBin);
  nBinaryRms1[iBin1] = geomHistos[1]->h_nBinaryRmsVsXsect->GetBinContent(iBin); enBinaryRms1[iBin1] = geomHistos[1]->h_nBinaryRmsVsXsect->GetBinError(iBin);
  nBinaryRms2[iBin1] = geomHistos[2]->h_nBinaryRmsVsXsect->GetBinContent(iBin); enBinaryRms2[iBin1] = geomHistos[2]->h_nBinaryRmsVsXsect->GetBinError(iBin);

  nBinaryOmega0[iBin1] = geomHistos[0]->h_nBinaryOmegaVsXsect->GetBinContent(iBin); enBinaryOmega0[iBin1] = geomHistos[0]->h_nBinaryOmegaVsXsect->GetBinError(iBin);
  nBinaryOmega1[iBin1] = geomHistos[1]->h_nBinaryOmegaVsXsect->GetBinContent(iBin); enBinaryOmega1[iBin1] = geomHistos[1]->h_nBinaryOmegaVsXsect->GetBinError(iBin);
  nBinaryOmega2[iBin1] = geomHistos[2]->h_nBinaryOmegaVsXsect->GetBinContent(iBin); enBinaryOmega2[iBin1] = geomHistos[2]->h_nBinaryOmegaVsXsect->GetBinError(iBin);

  }

  TGraphErrors * nPartRmsVsAvgNpart0 = new TGraphErrors(nCoarseBins-1, nPartAvg0, nPartRms0, enPartAvg0, enPartRms0);
  TGraphErrors * nPartRmsVsAvgNpart1 = new TGraphErrors(nCoarseBins-1, nPartAvg1, nPartRms1, enPartAvg1, enPartRms1);
  TGraphErrors * nPartRmsVsAvgNpart2 = new TGraphErrors(nCoarseBins-1, nPartAvg2, nPartRms2, enPartAvg2, enPartRms2);

  TGraphErrors * nPartOmegaVsAvgNpart0 = new TGraphErrors(nCoarseBins-1, nPartAvg0, nPartOmega0, enPartAvg0, enPartOmega0);
  TGraphErrors * nPartOmegaVsAvgNpart1 = new TGraphErrors(nCoarseBins-1, nPartAvg1, nPartOmega1, enPartAvg1, enPartOmega1);
  TGraphErrors * nPartOmegaVsAvgNpart2 = new TGraphErrors(nCoarseBins-1, nPartAvg2, nPartOmega2, enPartAvg2, enPartOmega2);

  TGraphErrors * nBinaryRmsVsAvgNpart0 = new TGraphErrors(nCoarseBins-1, nPartAvg0, nBinaryRms0, enPartAvg0, enBinaryRms0);
  TGraphErrors * nBinaryRmsVsAvgNpart1 = new TGraphErrors(nCoarseBins-1, nPartAvg1, nBinaryRms1, enPartAvg1, enBinaryRms1);
  TGraphErrors * nBinaryRmsVsAvgNpart2 = new TGraphErrors(nCoarseBins-1, nPartAvg2, nBinaryRms2, enPartAvg2, enBinaryRms2);

  TGraphErrors * nBinaryOmegaVsAvgNpart0 = new TGraphErrors(nCoarseBins-1, nPartAvg0, nBinaryOmega0, enPartAvg0, enBinaryOmega0);
  TGraphErrors * nBinaryOmegaVsAvgNpart1 = new TGraphErrors(nCoarseBins-1, nPartAvg1, nBinaryOmega1, enPartAvg1, enBinaryOmega1);
  TGraphErrors * nBinaryOmegaVsAvgNpart2 = new TGraphErrors(nCoarseBins-1, nPartAvg2, nBinaryOmega2, enPartAvg2, enBinaryOmega2);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartRmsVsAvgNPart";
  histograms[0] = new TH1D("nPartRmsVsAvgNpart0","", 100, 0.0, 440.0);
  histograms[1] = new TH1D("nPartRmsVsAvgNpart1","", 100, 0.0, 440.0);
  histograms[2] = new TH1D("nPartRmsVsAvgNpart2","", 100, 0.0, 440.0);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(3, canvasName,canvasConfiguration,graphConfigurations,
                        "<N_{part}>", 0.0, 440.0,
                        "RMS[N_{part}]", 0.0, 50.0,
                        histograms,titles,0.22, 0.55, 0.45, 0.75, 0.04);
  compPlotters[iPlotter]->setProperties(nPartRmsVsAvgNpart0, *graphConfigurations[0]);
  compPlotters[iPlotter]->setProperties(nPartRmsVsAvgNpart1, *graphConfigurations[1]);
  compPlotters[iPlotter]->setProperties(nPartRmsVsAvgNpart2, *graphConfigurations[2]);
  nPartRmsVsAvgNpart0->Draw("PE");
  nPartRmsVsAvgNpart1->Draw("PE");
  nPartRmsVsAvgNpart2->Draw("PE");
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartOmegaVsAvgNPart";
  histograms[0] = new TH1D("nPartOmegaVsAvgNpart0","", 100, 0.0, 440.0);
  histograms[1] = new TH1D("nPartOmegaVsAvgNpart1","", 100, 0.0, 440.0);
  histograms[2] = new TH1D("nPartOmegaVsAvgNpart2","", 100, 0.0, 440.0);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(3, canvasName,canvasConfiguration,graphConfigurations,
                        "<N_{part}>", 0.0, 440.0,
                        "#omega[N_{part}]", 0.0, 6.0,
                        histograms,titles,0.7, 0.65, 0.85, 0.85, 0.04);
  compPlotters[iPlotter]->setProperties(nPartOmegaVsAvgNpart0, *graphConfigurations[0]);
  compPlotters[iPlotter]->setProperties(nPartOmegaVsAvgNpart1, *graphConfigurations[1]);
  compPlotters[iPlotter]->setProperties(nPartOmegaVsAvgNpart2, *graphConfigurations[2]);
  nPartOmegaVsAvgNpart0->Draw("PE");
  nPartOmegaVsAvgNpart1->Draw("PE");
  nPartOmegaVsAvgNpart2->Draw("PE");

  double off = 0.2;
  compPlotters[iPlotter]->createLabel(nPartAvg1[0], nPartOmega1[0]+off, 90.0, 1, 1, 0.04, "0-5%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[1], nPartOmega1[1]+off, 90.0, 1, 1, 0.04, "5-10%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[2], nPartOmega1[2]+off, 90.0, 1, 1, 0.04, "10-20%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[3], nPartOmega1[3]+off, 90.0, 1, 1, 0.04, "20-30%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[4], nPartOmega1[4]+off, 90.0, 1, 1, 0.04, "30-40%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[5], nPartOmega1[5]+off, 90.0, 1, 1, 0.04, "40-50%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[6], nPartOmega1[6]+off, 90.0, 1, 1, 0.04, "50-60%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[7], nPartOmega1[7]+off, 90.0, 1, 1, 0.04, "60-70%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[8], nPartOmega1[8]+off, 90.0, 1, 1, 0.04, "70-80%", true);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryRmsVsAvgNPart";
  histograms[0] = new TH1D("nBinaryRmsVsAvgNpart0","", 100, 0.0, 440.0);
  histograms[1] = new TH1D("nBinaryRmsVsAvgNpart1","", 100, 0.0, 440.0);
  histograms[2] = new TH1D("nBinaryRmsVsAvgNpart2","", 100, 0.0, 440.0);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(3, canvasName,canvasConfiguration,graphConfigurations,
                        "<N_{part}>", 0.0, 440.0,
                        "RMS[N_{bin}]", 0.0, 250.0,
                        histograms,titles,0.22, 0.55, 0.45, 0.75, 0.04);
  compPlotters[iPlotter]->setProperties(nBinaryRmsVsAvgNpart0, *graphConfigurations[0]);
  compPlotters[iPlotter]->setProperties(nBinaryRmsVsAvgNpart1, *graphConfigurations[1]);
  compPlotters[iPlotter]->setProperties(nBinaryRmsVsAvgNpart2, *graphConfigurations[2]);
  nBinaryRmsVsAvgNpart0->Draw("PE");
  nBinaryRmsVsAvgNpart1->Draw("PE");
  nBinaryRmsVsAvgNpart2->Draw("PE");
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryOmegaVsAvgNPart";
  histograms[0] = new TH1D("nBinaryOmegaVsAvgNpart0","", 100, 0.0, 440.0);
  histograms[1] = new TH1D("nBinaryOmegaVsAvgNpart1","", 100, 0.0, 440.0);
  histograms[2] = new TH1D("nBinaryOmegaVsAvgNpart2","", 100, 0.0, 440.0);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(3, canvasName,canvasConfiguration,graphConfigurations,
                        "<N_{part}>", 0.0, 440.0,
                        "#omega[N_{bin}]", 0.0, 26.0,
                        histograms,titles,0.7, 0.65, 0.85, 0.85, 0.04);
  compPlotters[iPlotter]->setProperties(nBinaryOmegaVsAvgNpart0, *graphConfigurations[0]);
  compPlotters[iPlotter]->setProperties(nBinaryOmegaVsAvgNpart1, *graphConfigurations[1]);
  compPlotters[iPlotter]->setProperties(nBinaryOmegaVsAvgNpart2, *graphConfigurations[2]);
  nBinaryOmegaVsAvgNpart0->Draw("PE");
  nBinaryOmegaVsAvgNpart1->Draw("PE");
  nBinaryOmegaVsAvgNpart2->Draw("PE");
  off = 2.0;
  compPlotters[iPlotter]->createLabel(nPartAvg1[0], nBinaryOmega1[0]+off, 90.0, 1, 1, 0.04, "0-5%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[1], nBinaryOmega1[1]+off, 90.0, 1, 1, 0.04, "5-10%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[2], nBinaryOmega1[2]+off, 90.0, 1, 1, 0.04, "10-20%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[3], nBinaryOmega1[3]+off, 90.0, 1, 1, 0.04, "20-30%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[4], nBinaryOmega1[4]+off, 90.0, 1, 1, 0.04, "30-40%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[5], nBinaryOmega1[5]+off, 90.0, 1, 1, 0.04, "40-50%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[6], nBinaryOmega1[6]+off, 90.0, 1, 1, 0.04, "50-60%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[7], nBinaryOmega1[7]+off, 90.0, 1, 1, 0.04, "60-70%", true);
  compPlotters[iPlotter]->createLabel(nPartAvg1[8], nBinaryOmega1[8]+off, 90.0, 1, 1, 0.04, "70-80%", true);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  return -10;

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartR2VsB";
  histograms[0] = geomHistos[0]->h_nPartR2VsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartR2VsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartR2VsB; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 20.0,
                        "R_{2}[N_{part}]", -0.7, 0.4,
                        histograms,titles,0.35, 0.35, 0.5, 0.45, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinarytR2VsB";
  histograms[0] = geomHistos[0]->h_nBinaryR2VsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryR2VsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryR2VsB; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 20.0,
                        "R_{2}[N_{bin}]",  -0.7, 0.7,
                        histograms,titles,0.35, 0.65, 0.5, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

   canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryR2VsNPart";
  histograms[0] = geomHistos[0]->h_nBinaryR2VsNPart; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryR2VsNPart; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryR2VsNPart; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}", 0.0, 440.0,
                        "R_{2}[N_{bin}]",  -0.05, 0.05,
                        histograms,titles,0.35, 0.35, 0.5, 0.55, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartVsXsect_Prof";
  histograms[0] = geomHistos[0]->h_nPartVsXsect_Prof; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartVsXsect_Prof; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartVsXsect_Prof; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                               "b (fm)", 0.0, 20.0,
                               "<N_{part}>",  0.0, 440.0,
                               histograms,titles,0.65, 0.55, 0.75, 0.75, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryVsXsect_Prof";
  histograms[0] = geomHistos[0]->h_nBinaryVsXsect_Prof; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryVsXsect_Prof; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryVsXsect_Prof; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                               "b (fm)", 0.0, 20.0,
                               "<N_{bin}>",  0.0, 2400.0,
                               histograms,titles,0.65, 0.55, 0.75, 0.75, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartOmegaVsXsect";
  histograms[0] = geomHistos[0]->h_nPartOmegaVsXsect; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartOmegaVsXsect; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartOmegaVsXsect; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                               "b (fm)", 0.0, 20.0,
                               "#omega(N_{part})",  0.0, 5.0,
                               histograms,titles,0.25, 0.65, 0.45, 0.85, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryOmegaVsXsect";
  histograms[0] = geomHistos[0]->h_nBinaryOmegaVsXsect; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryOmegaVsXsect; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryOmegaVsXsect; titles[2] = modelTitles[2];
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                               "b (fm)", 0.0, 20.0,
                               "#omega(N_{bin})",  0.0, 50.0,
                               histograms,titles,0.25, 0.55, 0.45, 0.75, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

    canvasConfiguration->setAxes(CanvasConfiguration::Linear);
    canvasName = "ModelComp_nPartR2VsXsect";
    histograms[0] = geomHistos[0]->h_nPartR2VsXsect; titles[0] = modelTitles[0];
    histograms[1] = geomHistos[1]->h_nPartR2VsXsect; titles[1] = modelTitles[1];
    histograms[2] = geomHistos[2]->h_nPartR2VsXsect; titles[2] = modelTitles[2];
    compPlotters[iPlotter] = new Plotter();
    compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                                 "b (fm)", 0.0, 20.0,
                                 "R_{2}(N_{part})",  -0.5, 0.5,
                                 histograms,titles,0.25, 0.25, 0.45, 0.45, 0.04);
    compPlotters[iPlotter++]->printAllCanvas(outputPath);

    canvasConfiguration->setAxes(CanvasConfiguration::Linear);
    canvasName = "ModelComp_nBinaryR2VsXsect";
    histograms[0] = geomHistos[0]->h_nBinaryR2VsXsect; titles[0] = modelTitles[0];
    histograms[1] = geomHistos[1]->h_nBinaryR2VsXsect; titles[1] = modelTitles[1];
    histograms[2] = geomHistos[2]->h_nBinaryR2VsXsect; titles[2] = modelTitles[2];
    compPlotters[iPlotter] = new Plotter();
    compPlotters[iPlotter]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                                 "b (fm)", 0.0, 20.0,
                                 "R_{2}(N_{bin})",  -1.0, 1.0,
                                 histograms,titles,0.25, 0.25, 0.45, 0.45, 0.04);
    compPlotters[iPlotter++]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::LogY);
  canvasName = "ModelComp_nPartVsCent";
  histograms[0] = geomHistos[0]->h_nPart;         titles[0] = new TString("Inclusive");
  histograms[1] = geomHistos[0]->h_nPart_0_5;     titles[1] = new TString("0 - 5 %");
  histograms[2] = geomHistos[0]->h_nPart_5_10;    titles[2] = new TString("5 - 10 %");
  histograms[3] = geomHistos[0]->h_nPart_10_20;   titles[3] = new TString("10 - 20 %");
  histograms[4] = geomHistos[0]->h_nPart_20_30;   titles[4] = new TString("20 - 30 %");
  histograms[5] = geomHistos[0]->h_nPart_30_40;   titles[5] = new TString("30 - 40 %");
  histograms[6] = geomHistos[0]->h_nPart_40_50;   titles[6] = new TString("40 - 50 %");
  histograms[7] = geomHistos[0]->h_nPart_50_60;   titles[7] = new TString("50 - 60 %");
  histograms[8] = geomHistos[0]->h_nPart_60_70;   titles[8] = new TString("60 - 70 %");
  histograms[9] = geomHistos[0]->h_nPart_70_80;   titles[9] = new TString("70 - 80 %");
  histograms[10] = geomHistos[0]->h_nPart_80_90;   titles[10] = new TString("80 - 90 %");
  histograms[11] = geomHistos[0]->h_nPart_90_100;   titles[11] = new TString("90 - 100 %");


  histograms[1]->Scale(1.0/s0);
  histograms[2]->Scale(1.0/s0);
  histograms[3]->Scale(1.0/s0);
  histograms[4]->Scale(1.0/s0);
  histograms[5]->Scale(1.0/s0);
  histograms[6]->Scale(1.0/s0);
  histograms[7]->Scale(1.0/s0);
  histograms[8]->Scale(1.0/s0);
  histograms[9]->Scale(1.0/s0);
  histograms[10]->Scale(1.0/s0);
  histograms[11]->Scale(1.0/s0);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(12, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}", 0.0, 460.0,
                        "Rel. Freq. (a.u.)", 1.0E-6, 10.0,
                        histograms,titles,0.45, 0.62, 0.6, 0.92, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

///
  canvasConfiguration->setAxes(CanvasConfiguration::LogY);
  canvasName = "ModelComp_nBinaryVsCent";
  histograms[0] = geomHistos[0]->h_nBinary;         titles[0] = new TString("Inclusive");
  histograms[1] = geomHistos[0]->h_nBinary_0_5;     titles[1] = new TString("0 - 5 %");
  histograms[2] = geomHistos[0]->h_nBinary_5_10;    titles[2] = new TString("5 - 10 %");
  histograms[3] = geomHistos[0]->h_nBinary_10_20;   titles[3] = new TString("10 - 20 %");
  histograms[4] = geomHistos[0]->h_nBinary_20_30;   titles[4] = new TString("20 - 30 %");
  histograms[5] = geomHistos[0]->h_nBinary_30_40;   titles[5] = new TString("30 - 40 %");
  histograms[6] = geomHistos[0]->h_nBinary_40_50;   titles[6] = new TString("40 - 50 %");
  histograms[7] = geomHistos[0]->h_nBinary_50_60;   titles[7] = new TString("50 - 60 %");
  histograms[8] = geomHistos[0]->h_nBinary_60_70;   titles[8] = new TString("60 - 70 %");
  histograms[9] = geomHistos[0]->h_nBinary_70_80;   titles[9] = new TString("70 - 80 %");
  histograms[10] = geomHistos[0]->h_nBinary_80_90;   titles[10] = new TString("80 - 90 %");
  histograms[11] = geomHistos[0]->h_nBinary_90_100;   titles[11] = new TString("90 - 100 %");


  histograms[1]->Scale(1.0/r0);
  histograms[2]->Scale(1.0/r0);
  histograms[3]->Scale(1.0/r0);
  histograms[4]->Scale(1.0/r0);
  histograms[5]->Scale(1.0/r0);
  histograms[6]->Scale(1.0/r0);
  histograms[7]->Scale(1.0/r0);
  histograms[8]->Scale(1.0/r0);
  histograms[9]->Scale(1.0/r0);
  histograms[10]->Scale(1.0/r0);
  histograms[11]->Scale(1.0/r0);
  compPlotters[iPlotter] = new Plotter();
  compPlotters[iPlotter]->plot(12, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{Bin}", 0.0, 2400.0,
                        "Rel. Freq. (a.u.)", 1.0E-6, 10.0,
                        histograms,titles,0.45, 0.62, 0.6, 0.92, 0.04);
  compPlotters[iPlotter++]->printAllCanvas(outputPath);

// rms vs


  return 0;
}

double getHistoSum(TH1 * h)
{
  double sum = 0.0;
  int n = h->GetNbinsX();
  for (int i=1; i<=n; i++)
  {
  double v = h->GetBinContent(i);
  if (v<=0.0)
    {
    v = 1e-20;
    h->SetBinContent(i,v);
    }
  sum += v;
  }
  return sum;
}
