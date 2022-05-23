#include "TString.h"
#include "TRandom.h"
void loadBase(const TString & includeBasePath);

int CalculateDerivedHistogram()
{

  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);

  std::cout << "==================================================================================" << std::endl;
  std::cout << "CalculateDerivedHistogram()" << endl;
  std::cout << "==================================================================================" << std::endl;

  TString modelLabel;
  TString systemLabel;
  TString energyLabel;
  TString extraLabel;

  TString globalLabel     = "G";
  TString spherocityLabel = "S";
  TString partLabel       = "Part";
  TString pairLabel       = "Pair";
  TString nuDynLabel      = "NuDyn";
  TString derivedLabel         = "Derived";
  TString sumLabel             = "Sum";
  TString balFctLabel          = "BalFct";
  TString rootExt              = ".root";

  TString histoOutputDataName;
  TString histoOutputAnalyzerName;
  TString histoBaseName;

  MessageLogger::LogLevel  infoLevel  = MessageLogger::Info;
  MessageLogger::LogLevel  debugLevel = MessageLogger::Debug;
  MessageLogger::LogLevel  selectedLevel = debugLevel; //infoLevel;
  bool YES = true;
  bool NO  = false;

  TString histoInputPath       = "/Volumes/ClaudeDisc4/OutputFiles/quickTest2/RhoDecay/";
  TString histoInputFileName   = "RhoDecay";
  TString histoOutputPath      = "/Volumes/ClaudeDisc4/OutputFiles/quickTest2/RhoDecay/";
  TString histoOutputFileName  = "RhoDecay";
  TString histoBaseName        = "RhoDecay";


  bool forceHistogramsRewrite  =  YES;
  int  eventSelection      = 0;
  int  particleSelection   = 3;
  int  observableSelection = 1;
  int  defaultGroupSize    = 10;

  bool sumBasicHistograms       = NO;
  bool sumDerivedHistograms     = NO;
  bool sumBalFctHistograms      = NO;
  bool computeDerivedHistograms = YES;
  bool computeBalFct            = NO;
  bool doGlobals                = NO;
  bool doParticles              = YES;
  bool doPairs                  = NO;
  bool doNuDyns                 = NO;


  Configuration partConfig("Part Spectra Configuration");
  partConfig.addParameter( "forceHistogramsRewrite", forceHistogramsRewrite);
  partConfig.addParameter( "histoInputPath",         histoInputPath      );
  partConfig.addParameter( "histoInputFileName",     histoInputFileName+partLabel   );
  partConfig.addParameter( "histoOutputPath",        histoOutputPath     );
  partConfig.addParameter( "histoOutputFileName",    histoOutputFileName+partLabel );
  partConfig.addParameter( "histoBaseName",          partLabel    );
  
  partConfig.addParameter("nBins_n1",  100);
  partConfig.addParameter("min_n1",    0.0);
  partConfig.addParameter("max_n1",  100.0);

  partConfig.addParameter("nBins_eTot",  100);
  partConfig.addParameter("min_eTot",    0.0);
  partConfig.addParameter("max_eTot",  100.0);

  partConfig.addParameter("nBins_pt",  1000);
  partConfig.addParameter("min_pt",    0.0);
  partConfig.addParameter("max_pt",  100.0);

  partConfig.addParameter("nBins_eta",  80);
  partConfig.addParameter("min_eta",   -4.0);
  partConfig.addParameter("max_eta",    4.0);
 
  partConfig.addParameter("nBins_phi",  72);
  partConfig.addParameter("min_phi",    0.0);
  partConfig.addParameter("max_phi",    TMath::TwoPi());

  partConfig.addParameter("fillEta",  YES);
  partConfig.addParameter("fillY",    NO);
  partConfig.addParameter("fillP2",   NO);
  if (selectedLevel == debugLevel) partConfig.printConfiguration(cout);
  
  
  Configuration pairConfig("Pair Correlation Configuration");
  pairConfig.addParameter("useEventStream0",         NO);
  pairConfig.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
  pairConfig.addParameter("createHistograms",        YES);
  pairConfig.addParameter("saveHistograms",          YES);
  pairConfig.addParameter("loadHistograms",          YES);
  
  pairConfig.addParameter( "histoInputPath",      histoInputPath      );
  pairConfig.addParameter( "histoInputFileName",  histoInputFileName+pairLabel   );
  pairConfig.addParameter( "histoOutputPath",     histoOutputPath     );
  pairConfig.addParameter( "histoOutputFileName", histoOutputFileName+pairLabel  );
  pairConfig.addParameter( "histoBaseName",       pairLabel           );
  
  pairConfig.addParameter("nBins_n1",       100);
  pairConfig.addParameter("min_n1",         0.0);
  pairConfig.addParameter("max_n1",       100.0);
  pairConfig.addParameter("nBins_n2",       100);
  pairConfig.addParameter("min_n2",         0.0);
  pairConfig.addParameter("max_n2",       400.0);
  pairConfig.addParameter("nBins_pt",        20);
  pairConfig.addParameter("min_pt",        0.00);
  pairConfig.addParameter("max_pt",         2.0);
  pairConfig.addParameter("nBins_eta",       40);
  pairConfig.addParameter("min_eta",       -2.0); // 1;
  pairConfig.addParameter("max_eta",        2.0);  // 1;
  pairConfig.addParameter("nBins_phi",       72);
  pairConfig.addParameter("min_phi",        0.0);
  pairConfig.addParameter("max_phi",TMath::TwoPi());
  
  pairConfig.addParameter("nBins_Dphi",         72);
  pairConfig.addParameter("min_Dphi",          0.0);
  pairConfig.addParameter("max_Dphi",TMath::TwoPi());
  pairConfig.addParameter("width_Dphi",TMath::TwoPi());
  
  pairConfig.addParameter("nBins_Dphi_shft",    72);
  pairConfig.addParameter("min_Dphi_shft",     0.0);
  pairConfig.addParameter("max_Dphi_shft",     0.0);
  
  pairConfig.addParameter("nBins_Deta",         79);
  pairConfig.addParameter("min_Deta",         -2.0);
  pairConfig.addParameter("max_Deta",          2.0);
  pairConfig.addParameter("width_Deta",   4.0/79.0);
  
  pairConfig.addParameter("nBins_Dy",           79);
  pairConfig.addParameter("min_Dy",           -2.0);
  pairConfig.addParameter("max_Dy",            2.0);
  pairConfig.addParameter("width_Dy",     4.0/79.0);
  if (selectedLevel == debugLevel) pairConfig.printConfiguration(cout);

  Configuration nuDynConfig("Nu Dyn Configuration");

  vector<EventFilter*> eventFilters;
  vector<ParticleFilter*>  particleFilters;

  EventFilter * openEventFilter  = new EventFilter();
  openEventFilter->setName("All");
  openEventFilter->setLongName("All");
  EventFilter * aliceMinBiasEventFilter  = new EventFilter();
  aliceMinBiasEventFilter->setName("V0MB");
  aliceMinBiasEventFilter->setLongName("V0MinBias");
  aliceMinBiasEventFilter->addCondition(1, 0, 1.0, 1.0E10); // v0 multiplicity
  aliceMinBiasEventFilter->addCondition(1, 1, 1.0, 1.0E10); // TPC multiplicity
  switch (eventSelection)
    {
      default:
      case 0: eventFilters.push_back( openEventFilter ); break;
      case 1: eventFilters.push_back( aliceMinBiasEventFilter );break;
    }

  ParticleFilter* openParticleFilter     = new ParticleFilter(); // no conditions -- accepts all
  openParticleFilter->setName("OpenFilter_");
  openParticleFilter->setLongName("OpenFilter_");
  openParticleFilter->setTitle("Open");
  openParticleFilter->setLongTitle("Open");

  ParticleFilter* aliceV0ParticleFilter  = new ParticleFilter();
  aliceV0ParticleFilter->setName("V0M");
  aliceV0ParticleFilter->setLongName("HM");
  aliceV0ParticleFilter->setTitle("V0M");
  aliceV0ParticleFilter->setLongTitle("V0M");
  aliceV0ParticleFilter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  aliceV0ParticleFilter->addCondition(4, 1103, 0.0, 0.0);   // low mass charged hadron
  aliceV0ParticleFilter->addCondition(5, 1, 0.05,  1000.0);  // pT
  aliceV0ParticleFilter->addCondition(5, 7, -3.7, -1.7, 2.8, 5.1); // pseudorapidity in V0A and V0C

  ParticleFilter* aliceTpcParticleFilter = new ParticleFilter();
  aliceTpcParticleFilter->setName("TpcAll");
  aliceTpcParticleFilter->setLongName("TpcAll");
  aliceTpcParticleFilter->setTitle("TpcAll");
  aliceTpcParticleFilter->setLongTitle("TpcAll");
  aliceTpcParticleFilter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  aliceTpcParticleFilter->addCondition(4, 1103, 0.0, 0.0);   // low mass charged hadron
  aliceTpcParticleFilter->addCondition(5, 1, 0.15,  100.0);  // pT
  aliceTpcParticleFilter->addCondition(5, 7, -1.0, 1.0); // pseudorapidity in  TPC

  switch (particleSelection)
    {
      default:
      case 0: particleFilters.push_back( openParticleFilter ); break;
      case 1: particleFilters.push_back( aliceV0ParticleFilter ); break;
      case 2: particleFilters.push_back( aliceTpcParticleFilter);  break;
      case 3: particleFilters = ParticleFilter::createChargedHadronFilters(YES,0.2, 10.0, YES,-6.0, 6.0, NO, 10.0, -10.0);  break;
      case 4: particleFilters = ParticleFilter::createChargedHadronFilters(YES,0.2, 2.0, YES,-1.0, 1.0, NO, 10.0, -10.0);   break;
      case 5: particleFilters = ParticleFilter::createPlusMinusHadronFilters(YES,0.2, 2.0, YES,-2.0, 2.0, NO, 10.0, -10.0); break;
    }

  cout << "           histoInputPath : " << histoInputPath << endl;
  cout << "             baseFileName : " << histoBaseName << endl;
  cout << "              globalLabel : " << globalLabel << endl;
  cout << "                partLabel : " << partLabel << endl;
  cout << "               nuDynLabel : " << nuDynLabel << endl;
  cout << "             derivedLabel : " << derivedLabel << endl;
  cout << "                 sumLabel : " << sumLabel << endl;
  cout << "              balFctLabel : " << balFctLabel << endl;
  cout << "           eventSelection : " << eventSelection << endl;
  cout << "        particleSelection : " << particleSelection << endl;
  cout << "      observableSelection : " << observableSelection << endl;
  for (unsigned int k=0; k<particleFilters.size(); k++)
    {
    cout << " particle filter k:" << k << " Name: " << particleFilters[k]->getName() << endl;
    }
  
  

  if (computeDerivedHistograms)
    {
     if (doParticles)
      {
      Configuration fileIteratorConfig("ParticleDerivedHistogramIterator Configuration");
      fileIteratorConfig.addParameter("histoInputPath",        histoInputPath      );
      fileIteratorConfig.addParameter("histoOutputPath",       histoOutputPath     );
      fileIteratorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
      fileIteratorConfig.addParameter("IncludedPattern0",rootExt);
      fileIteratorConfig.addParameter("IncludedPattern1",histoBaseName+partLabel);
      fileIteratorConfig.addParameter("ExcludedPattern0",derivedLabel);
      fileIteratorConfig.addParameter("ExcludedPattern1",sumLabel);
      fileIteratorConfig.addParameter("ExcludedPattern2",balFctLabel);
      fileIteratorConfig.addParameter("appendedString",  derivedLabel);
      FileTaskIterator * iterator = new FileTaskIterator("ParticleDerivedHistogramIterator",fileIteratorConfig,selectedLevel);
      iterator->addSubtask(new ParticleDerivedHistogramCalculator(partLabel, partConfig, eventFilters, particleFilters, selectedLevel));
      iterator->execute();
      delete iterator;
      }
    
    if (doPairs)
      {
      Configuration fileIteratorConfig("PairDerivedHistogramIterator Configuration");
      fileIteratorConfig.addParameter("histoInputPath",        histoInputPath      );
      fileIteratorConfig.addParameter("histoOutputPath",       histoOutputPath     );
      fileIteratorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
      fileIteratorConfig.addParameter("IncludedPattern0",rootExt);
      fileIteratorConfig.addParameter("IncludedPattern1",histoBaseName+pairLabel);
      fileIteratorConfig.addParameter("ExcludedPattern0",derivedLabel);
      fileIteratorConfig.addParameter("ExcludedPattern1",sumLabel);
      fileIteratorConfig.addParameter("ExcludedPattern2",balFctLabel);
      fileIteratorConfig.addParameter("appendedString",  derivedLabel);
      FileTaskIterator * iterator = new FileTaskIterator("ParticleDerivedHistogramIterator",fileIteratorConfig,selectedLevel);
      iterator->addSubtask(new ParticleDerivedHistogramCalculator(partLabel, partConfig, eventFilters, particleFilters, selectedLevel));
      iiterator->execute();
      delete iterator;
      }
    
    if (doNuDyns)
      {
      Configuration fileIteratorConfig("NuDynDerivedHistogramIterator Configuration");
      fileIteratorConfig.addParameter("histoInputPath",        histoInputPath      );
      fileIteratorConfig.addParameter("histoOutputPath",       histoOutputPath     );
      fileIteratorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
      fileIteratorConfig.addParameter("IncludedPattern0",rootExt);
      fileIteratorConfig.addParameter("IncludedPattern1",histoBaseName+nuDynLabel);
      fileIteratorConfig.addParameter("ExcludedPattern0",derivedLabel);
      fileIteratorConfig.addParameter("ExcludedPattern1",sumLabel);
      fileIteratorConfig.addParameter("ExcludedPattern2",balFctLabel);
      fileIteratorConfig.addParameter("appendedString",  derivedLabel);
      FileTaskIterator * iterator = new FileTaskIterator("NuDynDerivedHistogramIterator",fileIteratorConfig,selectedLevel);
      iterator->addSubtask(new NuDynDerivedHistogramCalculator(nuDynLabel, nuDynConfig, eventFilters, particleFilters, selectedLevel));
      iterator->execute();
      delete iterator;
      }
    }


//    SubSampleStatIterator * subSampleStatIterator;
//    if (doSubsampleAnalysisBasic)
//      {
//      Configuration subSampleConfig("Event Iterator Configuration");
//      subSampleConfig.addParameter( "forceHistogramsRewrite",forceHistogramsRewrite);
//      subSampleConfig.addParameter( "appendedString",      TString("Sum_"));
//      subSampleConfig.addParameter( "IncludedPattern",     outputFileNameBase);
//      subSampleConfig.setParameter( "histoInputPath",      outputPathName);
//      subSampleConfig.setParameter( "histoOutputPath",     outputPathName);
//      subSampleConfig.addParameter( "histoOutputDataName",     histoOutputDataName);
//      subSampleConfig.addParameter( "histoOutputAnalyzerName", histoOutputAnalyzerName);
//      subSampleStatIterator = new SubSampleStatIterator("MultiTaskSubSampleIterator",subSampleConfig,infoLevel);
//
//    if (doParticles)
//      {
//      Configuration subSampleCalcularorConfig("SubSampleCalcularor-Particles Configuration");
//      subSampleCalcularorConfig.addParameter("histoInputPath",        histoInputPath      );
//      subSampleCalcularorConfig.addParameter("histoOutputPath",       histoOutputPath     );
//      subSampleCalcularorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
//      subSampleCalcularorConfig.addParameter("IncludedPattern0", rootExt);
//      subSampleCalcularorConfig.addParameter("IncludedPattern1", histoBaseName+partLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern0", derivedLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern1", sumLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern2", balFctLabel);
//      subSampleCalcularorConfig.addParameter("defaultGroupSize", 10);
//      subSampleCalcularorConfig.addParameter("appendedString",   "Sum_");
//      subSampleStatIterator->addSubTask( globalEventAna );
//
//      SubSampleStatCalculator * calculator = new SubSampleStatCalculator("SubSampleStatCalculator-Particles",subSampleCalcularorConfig,infoLevel);
//      calculator->run();
//      }
//    if (doPairs)
//      {
//      Configuration subSampleCalcularorConfig("SubSampleCalcularor-Pairs Configuration");
//      subSampleCalcularorConfig.addParameter("histoInputPath",        histoInputPath      );
//      subSampleCalcularorConfig.addParameter("histoOutputPath",       histoOutputPath     );
//      subSampleCalcularorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
//      subSampleCalcularorConfig.addParameter("IncludedPattern0", rootExt);
//      subSampleCalcularorConfig.addParameter("IncludedPattern1", histoBaseName+pairLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern0", derivedLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern1", sumLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern2", balFctLabel);
//      subSampleCalcularorConfig.addParameter("defaultGroupSize", 10);
//      subSampleCalcularorConfig.addParameter("appendedString",   "Sum_");
//      SubSampleStatCalculator * calculator = new SubSampleStatCalculator("SubSampleStatCalculator-Pairs",subSampleCalcularorConfig,infoLevel);
//      calculator->run();
//      }
//    if (doNuDyns)
//      {
//      Configuration subSampleCalcularorConfig("SubSampleCalcularor-NuDyns Configuration");
//      subSampleCalcularorConfig.addParameter("histoInputPath",        histoInputPath      );
//      subSampleCalcularorConfig.addParameter("histoOutputPath",       histoOutputPath     );
//      subSampleCalcularorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
//      subSampleCalcularorConfig.addParameter("IncludedPattern0", rootExt);
//      subSampleCalcularorConfig.addParameter("IncludedPattern1", histoBaseName+nuDynLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern2", derivedLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern0", sumLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern1", balFctLabel);
//      subSampleCalcularorConfig.addParameter("defaultGroupSize", 10);
//      subSampleCalcularorConfig.addParameter("appendedString",   "Derived_Sum_");
//      SubSampleStatCalculator * calculator = new SubSampleStatCalculator("SubSampleStatCalculator-NuDyns",subSampleCalcularorConfig,infoLevel);
//      calculator->run();
//      }
//    }
//
//  if (sumDerivedHistograms)
//    {
//    if (doParticles)
//      {
//      Configuration subSampleCalcularorConfig("SubSampleCalcularor-Particles Configuration");
//      subSampleCalcularorConfig.addParameter("histoInputPath",        histoInputPath      );
//      subSampleCalcularorConfig.addParameter("histoOutputPath",       histoOutputPath     );
//      subSampleCalcularorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
//      subSampleCalcularorConfig.addParameter("IncludedPattern0", rootExt);
//      subSampleCalcularorConfig.addParameter("IncludedPattern1", histoBaseName+partLabel);
//      subSampleCalcularorConfig.addParameter("IncludedPattern2", derivedLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern0", sumLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern1", balFctLabel);
//      subSampleCalcularorConfig.addParameter("defaultGroupSize", 10);
//      subSampleCalcularorConfig.addParameter("appendedString",   "Derived_Sum_");
//      SubSampleStatCalculator * calculator = new SubSampleStatCalculator("SubSampleStatCalculator-Particles",subSampleCalcularorConfig,infoLevel);
//      calculator->run();
//      }
//    if (doPairs)
//      {
//      Configuration subSampleCalcularorConfig("SubSampleCalcularor-Pairs Configuration");
//      subSampleCalcularorConfig.addParameter("histoInputPath",        histoInputPath      );
//      subSampleCalcularorConfig.addParameter("histoOutputPath",       histoOutputPath     );
//      subSampleCalcularorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
//      subSampleCalcularorConfig.addParameter("IncludedPattern0", rootExt);
//      subSampleCalcularorConfig.addParameter("IncludedPattern1", histoBaseName+pairLabel);
//      subSampleCalcularorConfig.addParameter("IncludedPattern2", derivedLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern0", sumLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern1", balFctLabel);
//      subSampleCalcularorConfig.addParameter("defaultGroupSize", 10);
//      subSampleCalcularorConfig.addParameter("appendedString",   "Derived_Sum_");
//      SubSampleStatCalculator * calculator = new SubSampleStatCalculator("SubSampleStatCalculator-Pairs",subSampleCalcularorConfig,infoLevel);
//      calculator->run();
//      }
//    if (doNuDyns)
//      {
//      Configuration subSampleCalcularorConfig("SubSampleCalcularor-NuDyns Configuration");
//      subSampleCalcularorConfig.addParameter("histoInputPath",        histoInputPath      );
//      subSampleCalcularorConfig.addParameter("histoOutputPath",       histoOutputPath     );
//      subSampleCalcularorConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
//      subSampleCalcularorConfig.addParameter("IncludedPattern0", rootExt);
//      subSampleCalcularorConfig.addParameter("IncludedPattern1", histoBaseName+nuDynLabel);
//      subSampleCalcularorConfig.addParameter("IncludedPattern2", derivedLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern0", sumLabel);
//      subSampleCalcularorConfig.addParameter("ExcludedPattern1", balFctLabel);
//      subSampleCalcularorConfig.addParameter("defaultGroupSize", 10);
//      subSampleCalcularorConfig.addParameter("appendedString",   "Derived_Sum_");
//      SubSampleStatCalculator * calculator = new SubSampleStatCalculator("SubSampleStatCalculator-NuDyns",subSampleCalcularorConfig,infoLevel);
//      calculator->run();
//      }
//    }
//
//  if (computeBalFct)
//    {
//    cout << "ComputeBF -- from folder:" << histoInputPath  << endl;
//
//
//    vector<TString> summedFiles = Task::listFilesInDir(histoInputPath,"Derived_Group_0_to_9.root");
//    int nSummedFiles = summedFiles.size();
//    cout << "  nSummedFiles: " << nSummedFiles << endl;
//    if (nSummedFiles<1)
//      {
//      cout << "<E> RunDerivedHistogramCalculator(...) No sum of derived files founds???" << endl;
//      return 1;
//      }
//
//
//    vector<TString>  sObservableNames;
//    vector<TString>  pObservableNames;
//    switch (observableSelection)
//      {
//        default:
//        case 0: // eta based observables, full complement
//        sObservableNames.push_back("_n1_eta");
//        sObservableNames.push_back("_n1_phi");
//        pObservableNames.push_back("_R2_ptpt");
//        pObservableNames.push_back("_R2_phiPhi");
//        pObservableNames.push_back("_R2_etaEta");
//        pObservableNames.push_back("_R2_DetaDphi_shft");
//        break;
//
//        case 1: // eta based observables, only DeltaEta vs DeltaPhi
//        sObservableNames.push_back("_n1_eta");
//        sObservableNames.push_back("_n1_phi");
//        pObservableNames.push_back("_R2_DetaDphi_shft");
//        break;
//
//        case 2: // y based observables, full complement
//        sObservableNames.push_back("_n1_y");
//        sObservableNames.push_back("_n1_phi");
//        pObservableNames.push_back("_R2_ptpt");
//        pObservableNames.push_back("_R2_phiPhi");
//        pObservableNames.push_back("_R2_yY");
//        pObservableNames.push_back("_R2_DyDphi_shft");
//        break;
//
//        case 3: // y based observables, only DeltaY vs DeltaPhi
//        sObservableNames.push_back("_n1_y");
//        sObservableNames.push_back("_n1_phi");
//        pObservableNames.push_back("_R2_DyDphi_shft");
//        break;
//
//        case 4: // eta based observables, only DeltaEta vs DeltaPhi
//        sObservableNames.push_back("_n1_eta");
//        sObservableNames.push_back("_n1_phi");
//        pObservableNames.push_back("_B2AB_DetaDphi_shft");
//        pObservableNames.push_back("_B2BA_DetaDphi_shft");
//        break;
//      }
//
//    for (unsigned int k=0; k<nSummedFiles; k++  )
//      {
//      pairConfig.setParameter( "histoInputPath",      histoInputPath    );
//      pairConfig.setParameter( "histoInputFileName",  summedFiles[k]    );
//      pairConfig.setParameter( "histoOutputPath",     histoOutputPath   );
//      pairConfig.setParameter( "histoOutputFileName", TString("test")   );
//      pairConfig.setParameter( "histoBaseName",       TString("Pair_")  );
//      BalanceFunctionCalculator * bfc = new BalanceFunctionCalculator("BFC",pairConfig,eventFilters, particleFilters, sObservableNames, pObservableNames, selectedLevel);
//      bfc->execute();
//      delete bfc;
//      }
//    }

  return 0;
}


void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load(includePath+"TaskIterator.hpp");
  gSystem->Load(includePath+"Collection.hpp");
  gSystem->Load(includePath+"CanvasCollection.hpp");
  gSystem->Load(includePath+"HistogramCollection.hpp");
  gSystem->Load(includePath+"Histograms.hpp");
  gSystem->Load(includePath+"Particle.hpp");
  gSystem->Load(includePath+"ParticleType.hpp");
  gSystem->Load(includePath+"ParticleTypeCollection.hpp");
  gSystem->Load(includePath+"ParticleDecayMode.hpp");
  gSystem->Load("libBase.dylib");
}
