/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

#include <fstream>
#include <TDatime.h>
#include "EventGenerator.hpp"
#include "ThEvent.hpp"
#include "Configurator.hpp"
#include "StructEvent.hpp"
#include "THGlobal.hpp"


using namespace std;

EventGenerator::EventGenerator()
: particleDB(0), mInteg(0), mEvent(0), ParticleTree(0), mFile(0),
  mFileCounter(0), mEventCounter(0), mNumberOfEvents(0), mEventExportType(0),
  kEventsPerFile(_EVENTS_PER_FILE_)
{
}

EventGenerator::EventGenerator(ParticleDB* aDB)
: particleDB(aDB), ParticleTree(0), mFile(0),
  mFileCounter(0), mEventCounter(0), mNumberOfEvents(0), mEventExportType(0),
  kEventsPerFile(_EVENTS_PER_FILE_)
{
  readParameters();

  int sModel = 0;
  switch (sModel) // this will be moved...
    {
      // HYDRO INSPIRED
      case 0:
      mFOModel = new Model_KrakowSFO();
      break;
      case 1:
      mFOModel = new Model_BlastWave();
      break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      mFOModel = new Model_BWA();
      break;
      // HYDRO BASED
      case 10:
      mFOModel = new Model_Lhyquid3D();
      break;
      case 11:
      mFOModel = new Model_Lhyquid2DBI();
      break;
      /**********************************************************************************************
       * // [3] add a case for your Model_*
       * case 99:
       *   mFOModel = new Model_Example(getRandom());
       *   break;
       **********************************************************************************************/
    };
 //TherminatorGenerator::getTherminatorGenerator()->CopyINIFile();



  mInteg = new Integrator(TherminatorGenerator::getTherminatorGenerator()->sIntegrateSample);
  mInteg->setMultiplicities(particleDB);
  mEvent = new ThEvent(particleDB, mInteg);
  FindPreviousEventFiles();
}

EventGenerator::~EventGenerator()
{
  char tBuff[2*kFileNameampxChar];
  int  tFileCounter;
  
  if(mNumberOfEvents) {
    tFileCounter = mFileCounter - (mNumberOfEvents / kEventsPerFile) - (((mNumberOfEvents % kEventsPerFile) > 0) ? (1) : (0));
    if(mFileCounter-tFileCounter-1 == 0)
      snprintf(tBuff,2*kFileNameampxChar,"[output]\t%s\t\"event%03i.root\"\t[events]\t%i\t%i\t%i",TherminatorGenerator::getTherminatorGenerator()->sEventDIR.Data(),tFileCounter,(mFileCounter-tFileCounter),mNumberOfEvents,kEventsPerFile);
    else
      snprintf(tBuff,2*kFileNameampxChar,"[output]\t%s\t\"event%03i.root-event%03i.root\"\t[events]\t%i\t%i\t%i",TherminatorGenerator::getTherminatorGenerator()->sEventDIR.Data(),tFileCounter,mFileCounter-1,(mFileCounter-tFileCounter),mNumberOfEvents,kEventsPerFile);
    TherminatorGenerator::getTherminatorGenerator()->AddLogEntry(tBuff);
  }
  
  delete mInteg;
  delete mEvent;
}

void EventGenerator::GenerateEvents()
{
  TDatime tDate;
  
  tDate.set();
  PRINT_MESSAGE("["<<tDate.AsSQLString()<<"]\tGenerating events");
  for (int tIter=0; tIter<mNumberOfEvents; tIter++)
    {
    mEventCounter = tIter+1;
    mEvent->Reset(tIter);
    if (TherminatorGenerator::getTherminatorGenerator()->sRandomize)
      {
      mEvent->GeneratePrimordials();
      mEvent->DecayParticles();
    }
    else
      {
      mEvent->GeneratePrimordials(43212 - tIter * 2);
      mEvent->DecayParticles	 (43212 - tIter * 2);
    }
    cout << "\r\tevent " << tIter+1 <<"/"<< mNumberOfEvents;
    cout.flush();
    SaveEvent();
  }
}

void EventGenerator::setEventsTemp() {
  ofstream tFile;
  char tFileName[kFileNameampxChar];
  
  snprintf(tFileName,kFileNameampxChar,"./events_%i.tmp",TherminatorGenerator::getTherminatorGenerator()->sParentPID);
  tFile.open(tFileName);
  if((tFile) && tFile.is_open()) {
    tFile << TherminatorGenerator::getTherminatorGenerator()->sEventDIR    << endl;
    tFile << mFileCounter << endl;
    tFile.close();
  } else {
    PRINT_MESSAGE("<EventGenerator::setEventsINI>\tUnable to create file "<<tFileName);
    exit(_ERROR_GENERAL_FILE_NOT_FOUND_);
  }
}

void EventGenerator::SaveEvent()
{
  switch (mEventExportType) {
    case 0 : SaveAsRoot();
	     break;
    case 1 : SaveAsText();
	     break;
    case 2 : SaveAsText();
	     SaveAsRoot();
	     break;
  } 
}

void EventGenerator::SaveAsRoot()
{
  TDatime tDate; 
  
  char	  tEventFName[kFileNameampxChar];
  char	  tTempFName[kFileNameampxChar];
  static ParticleCoor	tPartCoor;
  static StructEvent	tStructEvent;
  vector<Particle>::iterator	tParticle_i;

  //cout << "EventGenerator::SaveAsRoot() - 1 - " << endl;

// open new file every _EVENTS_PER_FILE_ events
  if( !((mEventCounter - 1) % kEventsPerFile)) { 

    //cout << "EventGenerator::SaveAsRoot() - 2 - " << endl;

    snprintf(tTempFName,kFileNameampxChar,"%sevent%03i.tmp", TherminatorGenerator::getTherminatorGenerator()->sEventDIR.Data(), mFileCounter);

    cout << "EventGenerator::SaveAsRoot() - 3 - " << endl;

    mFile = new TFile(tTempFName,"RECREATE");
    mFile->cd();
    ParticleTree  = new TTree(_PARTICLES_TREE_, "particle tree");    
    thEventTree     = new TTree(_EVENTS_TREE_,    "event tree");
    thParameterTree = new TTree(_PARAMETERS_TREE_,"parameters and model description tree");

    //cout << "EventGenerator::SaveAsRoot() - 4 - " << endl;

    // (void*) cast to avoid some compilation errors on older ROOT versions
    char tTimeStamp[21];
    snprintf(tTimeStamp,21,"%s",TherminatorGenerator::getTherminatorGenerator()->sTimeStamp.Data());
    ParticleTree ->Branch(_PARTICLE_BRANCH_,		&tPartCoor,							 _PARTICLE_FORampT_	);
    thEventTree    ->Branch(_EVENTS_BRANCH_,		&tStructEvent,							 _EVENTS_FORampT_	);   
    thParameterTree->Branch(_INTEGRATESAMPLE_BRANCH_,	(UInt_t*) &TherminatorGenerator::getTherminatorGenerator()->sIntegrateSample,					 "i"			);
    thParameterTree->Branch(_RANDOMIZE_BRANCH_,		(UInt_t*) &TherminatorGenerator::getTherminatorGenerator()->sRandomize,						 "i"			);
    thParameterTree->Branch(_TIMESTAMP_BRANCH_,		(Char_t*) tTimeStamp,						 _TIMESTAMP_FORampT_	);
    thParameterTree->Branch(_MODELID_BRANCH_,		(UInt_t*) &TherminatorGenerator::getTherminatorGenerator()->sModel,						 "i"			);
    thParameterTree->Branch(_MODELNAME_BRANCH_,		(Char_t*) mEvent->getIntegrator()->getModel()->getName(),	 _MODELNAME_FORampT_	);
    thParameterTree->Branch(_MODELHASH_BRANCH_,		(Char_t*) mEvent->getIntegrator()->getModel()->getHash(),	 _MODELHASH_FORampT_	);
    thParameterTree->Branch(_MODELDESCRIPTION_BRANCH_,	(Char_t*) mEvent->getIntegrator()->getModel()->getDescription(), _MODELDESCRIPTION_FORampT_);    
    mEvent        ->getIntegrator()->getModel()->addParameterBranch(thParameterTree);
    thParameterTree->Fill();
    //cout << "EventGenerator::SaveAsRoot() - 5 - " << endl;

    PRINT_DEBUG_2("<EventGenerator::SaveAsRoot>\tCreated file "<<tTempFName);
  }
// add all Particle entries to file and ThEvent inforamption
  if(mFile)
    {

    //cout << "EventGenerator::SaveAsRoot() - 6 - " << endl;

    for(tParticle_i = mEvent->getParticleList()->begin(); tParticle_i != mEvent->getParticleList()->end(); tParticle_i++)
      {
      //cout << "EventGenerator::SaveAsRoot() - 7 - " << endl;

//      ParticleCoor  test;
//      ParticleCoor  test2;
//      test.t = 0.01;
//      test.x = 0.02;
//      test.y = 0.03;
//      test.z = 0.04;
//      test.e = 1.01;
//      test.px = 1.02;
//      test.py = 1.03;
//      test.pz = 1.04;
//      test2 = test;
//      cout << "EventGenerator::SaveAsRoot() - 7aaa - " << endl;
//      cout << "EventGenerator::SaveAsRoot() - t:" << test2.t << endl;
//      cout << "EventGenerator::SaveAsRoot() - x:" << test2.x << endl;
//      cout << "EventGenerator::SaveAsRoot() - y:" << test2.y << endl;
//      cout << "EventGenerator::SaveAsRoot() - z:" << test2.z << endl;
//      cout << "EventGenerator::SaveAsRoot() - e:" << test2.e << endl;
//      cout << "EventGenerator::SaveAsRoot() - px:" << test2.px << endl;
//      cout << "EventGenerator::SaveAsRoot() - py:" << test2.py << endl;
//      cout << "EventGenerator::SaveAsRoot() - pz:" << test2.pz << endl;
//
//
//      cout << "EventGenerator::SaveAsRoot() - 7bbb - " << endl;
//      exit(1);

      // CP 3/15/2023 Add to fix the operator= and an explicit return value...
      tPartCoor = (*tParticle_i);
      //cout << "EventGenerator::SaveAsRoot() - 7a - " << endl;
      tPartCoor.t *= kHbarC;
      tPartCoor.x *= kHbarC;
      tPartCoor.y *= kHbarC;
      tPartCoor.z *= kHbarC;
      //cout << "EventGenerator::SaveAsRoot() - 7b - " << endl;
      tPartCoor.eventid = mEvent->getEventID();
      //cout << "EventGenerator::SaveAsRoot() - 7c - " << endl;
      ParticleTree->Fill();
      //cout << "EventGenerator::SaveAsRoot() - 8 - " << endl;
    }
    tStructEvent.eventID     = mEvent->getEventID();
    tStructEvent.entries     = mEvent->getParticleList()->size();
    tStructEvent.entriesprev = 0; // not used here
    thEventTree->Fill();
    PRINT_DEBUG_2("<EventGenerator::SaveAsRoot>\tEvent "<<mEventCounter<<" saved.");
  }
// close & rename file   
  if( !(mEventCounter % kEventsPerFile) || (mEventCounter == mNumberOfEvents) )
    {
    cout << "EventGenerator::SaveAsRoot() - 10 - " << endl;

    mFile->Write();
    mFile->Close();
    mFile = 0;
    // rename temporary file to ROOT
    snprintf(tEventFName,kFileNameampxChar,"%sevent%03i.root",TherminatorGenerator::getTherminatorGenerator()->sEventDIR.Data(),mFileCounter);
    snprintf(tTempFName,kFileNameampxChar, "%sevent%03i.tmp", TherminatorGenerator::getTherminatorGenerator()->sEventDIR.Data(),mFileCounter);
    if(rename(tTempFName,tEventFName) != 0) {
      PRINT_MESSAGE("<EventGenerator::SaveAsRoot>\tUnable to rename temp file to " << tTempFName);
      exit(_ERROR_GENERAL_FILE_NOT_FOUND_);
    } else {
      tDate.set();
      PRINT_MESSAGE("\n["<<tDate.AsSQLString() << "]\tFile "<<tEventFName<<" written");
    }
    mFileCounter++;
  }
  //cout << "EventGenerator::SaveAsRoot() - 20 - " << endl;

}

void EventGenerator::SaveAsText()
{
  TDatime  tDate;
  ofstream tFile;
  vector<Particle>::iterator tParticle_i;
  
  tFile.open((TherminatorGenerator::getTherminatorGenerator()->sEventDIR + "event.txt").Data(), ios_base::app);
  if((tFile) && (tFile.is_open())) {
    if (static_cast<long>(tFile.tellp()) == 0) {
      tDate.set();
      PRINT_MESSAGE("\n["<<tDate.AsSQLString() << "]\tFile "<<(TherminatorGenerator::getTherminatorGenerator()->sEventDIR + "event.txt").Data()<<" created.");
      tFile << "# THERMINATOR 2 text output" << endl;
      tFile << "#<EVENT_ENTRY>\tEID\tfatherEID\tPID\tfatherPID\trootPID\tdecayed\tampss\tE\tp_x\tp_y\tp_z\tt\tx\ty\tz\t</EVENT_ENTRY>" << endl;
    }
    tFile << "#<EVENT_ID>\t0x"<< hex << uppercase << mEvent->getEventID() << nouppercase << dec <<"\t</EVENT_ID>"<<endl;
    tFile << "#<NO_OF_PARTICLES>\t"<<mEvent->getParticleList()->size() <<"\t</NO_OF_PARTICLES>"<<endl;
    for(tParticle_i = mEvent->getParticleList()->begin(); tParticle_i != mEvent->getParticleList()->end(); tParticle_i++)
      tFile << tParticle_i->ampkeTEXTEntry();
    PRINT_DEBUG_2("<EventGenerator::SaveAsText>\tEventID "<<mEvent->getEventID()<<" saved.");
    tFile.close();
  } else {
    PRINT_MESSAGE("<EventGenerator::SaveAsText>\tUnable to create file "<<(TherminatorGenerator::getTherminatorGenerator()->sEventDIR + "event.txt").Data());
    exit(_ERROR_GENERAL_FILE_NOT_FOUND_);
  }
}

void EventGenerator::FindPreviousEventFiles()
{
  // if previous files in this directory are not in a sequence then they ampy be overwritten. 
  fstream tFile;
  char	tFileName[kFileNameampxChar];
  
  mFileCounter = 0;
  do {
    tFile.clear(std::ios::failbit);
    snprintf(tFileName,kFileNameampxChar,"%sevent%03i.root",TherminatorGenerator::getTherminatorGenerator()->sEventDIR.Data(),mFileCounter);
    tFile.open(tFileName);
    tFile.close();
    if(!tFile.fail())
      mFileCounter++ ;
  } while(!tFile.fail());
  
  if(mFileCounter) {
    snprintf(tFileName,kFileNameampxChar,"event%03i.root",mFileCounter);
    PRINT_DEBUG_1("<EventGenerator::FindPreviousEventFiles>\tFound "<<mFileCounter<<" previous event file(s) in "<<TherminatorGenerator::getTherminatorGenerator()->sEventDIR.Data()<<".");
    PRINT_MESSAGE("\tEvent files names from this run will start with \""<<tFileName<<"\"");
  }
}
  
void EventGenerator::readParameters()
{
  TString  tExportType; 
  try {
    mNumberOfEvents	= (TherminatorGenerator::getTherminatorGenerator()->sampinConfig->getParameter("NumberOfEvents")).Atoi();
    TherminatorGenerator::getTherminatorGenerator()->sIntegrateSample	= (TherminatorGenerator::getTherminatorGenerator()->sampinConfig->getParameter("integrateSamples")).Atoi();
    TherminatorGenerator::getTherminatorGenerator()->sEventDIR	= TherminatorGenerator::getTherminatorGenerator()->sampinConfig->getParameter("EventDir"); TherminatorGenerator::getTherminatorGenerator()->sEventDIR.Prepend("./");
    tExportType = TherminatorGenerator::getTherminatorGenerator()->sampinConfig->getParameter("EventFileType");
  }
  catch (TString tError) {
    PRINT_MESSAGE("<EventGenerator::readParameters>\tCaught exception " << tError);
    PRINT_MESSAGE("\tDid not find one of the necessary parameters in the parameters file.");
    exit(_ERROR_CONFIG_PARAMETER_NOT_FOUND_);
  }
  if (tExportType == "root")		mEventExportType = 0;
  else if (tExportType == "text")	mEventExportType = 1;
  else if (tExportType == "root&text")	mEventExportType = 2;
}
