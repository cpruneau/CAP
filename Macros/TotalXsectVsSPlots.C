
#include "TString.h"


int TotalXsectVsSPlots()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"TotalXsection.hpp");
  gSystem->Load("libBase.dylib");

  HistogramCollection * histogramCollection = new HistogramCollection("Collection",200);
  histogramCollection->setDefaultOptions(1);
  CanvasCollection    * canvasCollection    = new CanvasCollection();
  CanvasConfiguration * canvasConfiguration = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::LogXY);
  GraphConfiguration  ** graphConfigurations = new GraphConfiguration*[40];
  for (int iGraph=0;iGraph<40;iGraph++)
  {
  graphConfigurations[iGraph] = new GraphConfiguration(1,iGraph%10);
  graphConfigurations[iGraph]->markerSize = 0.5;
  graphConfigurations[iGraph]->plotOption = "E0";
 }

  TString inputPath  = getenv("WAC_INPUT_PATH");
  TString outputPath = getenv("WAC_OUTPUT_PATH");
   ///  /Users/claudeapruneau/Documents/GitHub/run/PythiaStudies/PYTHIA_softOnHardOff_Singles_Wide_MB.root

  //canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  TString canvasName = "TotalXsetVsS";

  TotalXsection * factory;


  TF1 * fpp = factory->getFunctionTotalXsectionVsSqrtS(TotalXsection::PPCollisions,
                                                 TotalXsection::GauronModel,
                                                 1.0,
                                                 100000.0);

  TF1 * fppbar = factory->getFunctionTotalXsectionVsSqrtS(TotalXsection::PPbarCollisions,
                                                 TotalXsection::GauronModel,
                                                 1.0,
                                                 100000.0);

  TF1 * fgg = factory->getFunctionTotalXsectionVsSqrtS(TotalXsection::GammaGammaCollisions,
                                                 TotalXsection::GauronModel,
                                                 1.0,
                                                 100000.0);

  TCanvas * c = canvasCollection->createCanvas(canvasName, *canvasConfiguration);
  fpp->Draw("L");
  fpp->SetMinimum(1E-4);
  fpp->SetMaximum(1E3);

  fppbar->Draw("SAME");
  fgg->Draw("SAME");

  return 0;
}

