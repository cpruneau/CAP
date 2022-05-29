/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/
#ifndef CAP__Task
#define CAP__Task
#include <iostream>
#include <vector>
#include "TClass.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH3D.h"
#include "TH3F.h"
#include "TParameter.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TFile.h"
#include "TList.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TString.h"
#include "TMath.h"
#include "TPad.h"
#include "TF1.h"
#include "TF2.h"
#include "TRandom.h"
#include "TString.h"
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLegend.h"
#include "TFile.h"
#include "Configuration.hpp"
#include "MessageLogger.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "Histograms.hpp"
#include "ParticleTypeCollection.hpp"
#include "EventCountHistos.hpp"
#include "ParticleDigit.hpp"
#include "Timer.hpp"

using namespace std;

//!
//!\mainpage Correlation Analysis package
//!
//!\section Introduction
//!
//!blablablabla
//!blablablabla
//!
//!\section prelims Download, Installation/Configuration, Building, Usage
//!
//!\subsection prereqs Prerequisites
//!
//!The correlation analysis package (CAP) consists of several modules and classes designed for use within the [root] data analysis framework.
//!Depending on your needs, it may be sufficient to have a binary installation of "root", but if you require the generation of collision data with, e.g.,  the [PYTHIA] model (PYTHIA8),
//!you may need to install and build root from source. Evidently, to use such generators as Pythia8, you will additionally have to download, install, and build PYTHIA8 on your own
//!machine.
//!CAP is orgainzed into several modules that you may not readily need in your analysis. The package thus use CMake to build and maintain the libraries associated with
//!each module. Instructions to configure and compile the basic CAP package are provided in sec. XXXXX. You may optionally download the DOXYGEN documentation framework
//!if you plan to add and document modules and packages.
//!
//!## Dependencies
//!- cmake
//! + Main website: [https://cmake.org/](https://cmake.org/ "cmake" )
//! + Download : [https://cmake.org/download/](https://cmake.org/download/)
//!- root
//! + Main website : [https://root.cern/](https://root.cern/)
//! + Installing : [https://root.cern/install/](https://root.cern/install/)
//!- PYTHIA  : [https://pythia.org ](https://pythia.org  "PYTHIA")
//!- DOXYGEN : [https://www.doxygen.nl](https://www.doxygen.nl)
//!
//!
//!\subsection download Downloading CAP
//!
//!The CAP package is freely avaiable from  [https://github.com/cpruneau/CAP.git][].
//!
//!\subsection installation Installation/Configuration
//!
//!In its current version, CAP does not include nor provides for the building of binary executables (although it is easy to setup if you are proficient with CMAKE).
//!Its uses thus relies entirely on a generic version of root and root loads of relevant CAP (root compatible) libraries.
//!The building, compilation,  and use of the CAP package is determined by few environment varibles which can be efficiently setup and assigned by
//!sourcing the SETUP_CAP configuration file.
//!
//!\verbatim
//!cd SOMEPATH/CAP;
//!source Setup_CAP \endverbatim
//!
//!Prior to sourcing this setup file, however, you must edit the file to choose appropriate valeues  to the following lines that assigne environment variables
//!\verbatim
//!export ROOT_SELECTED_VERSION="/Users/YourName/rootdirectory/"
//!export CAP_LARGE_DATA="/Volumes/SomeHardDiskOfYourChoice/CAP-DATA/" \endverbatim
//!
//!
//!The Setup_CAP script will assign default values for the location in binary and library files produced when build the libraries. You can evidently modify these to your liking.
//!
//!\subsection building Building
//!
//!Assuming you have downloaded the CAP code in a directory "/MyCapPath/CAP", you can create CMAKE files to build  all CAP modules and libraries by creating a build directory, invoking CMAKE to create
//!the necessary files accorging to the following sequence of commands:
//!\verbatim
//!cd /MyCapPath/CAP
//!mkdir build
//!cd build
//!cmake ./src \endverbatim
//!
//!Once the above executes without error, you are then ready to invoke the "make" command:
//!\verbatim
//!cd /MyCapPath/CAP/build
//!make install \endverbatim
//!
//!If all required resources are in place, this should compile and produce root compatible libraries and root pct files,  and install them in the appropriate directories (folders). If this operation
//!fails, verify that the required resources (root, PYTHIA8, ..) are in their respective proper place. Additionally check that the environment variables designated by the Setup_CAP script exist and are
//!propertly pointed to by environment variables.
//!
//!\subsection usage Examples
//!
//!\verbatim
//!cd /MyCapPath/CAP/src/Macros
//!root CapBasicExample.C \endverbatim
//!
//!
//!\section model Object Model
//!
//!blablablabla
//!blablablabla
//!blablablabla
//!blablablabla
//!
//!
//!\section modules Modules and Components
//!
//!\subsection base Base
//!blablablabla
//!
//!\subsection generators Event Readers and Event Generators
//!
//!\subsubsection basicgen Basic Generators (Basicgen)
//!blablablabla
//!
//!\subsubsection collgeom Collision Geometry  Generators (CollGeom)
//!blablablabla

//!
//!\subsubsection hadrongas Hadron Gas (HadronGas)
//!blablablabla
//!
//!\subsubsection pythia CAPPythia
//!blablablabla
//!
//!\subsubsection ampt AMPT
//!blablablabla
//!
//!\subsubsection epos EPOS
//!blablablabla
//!\subsubsection urqmd UrQMD
//!blablablabla
//!\subsubsection blastwave Blastwave
//!blablablabla

//!
//!\subsection analyzers Analysis Modules
//!
//!CAP currently features the following analysis modules:
//!- Global : Study of global event observables
//!- Particle : Study of single particle spectra
//!- Pair: Study of differential (long range) correlation functions R2, P2, and G2
//!- Nudyn: Study of integral correlation functions, moments, factorial moments, cumulants, and factorial cumulants
//!- SubSample : Calculation of statistical errors using the sub sample method.
//!- Performance : Study of the performance of   measurements of basic observables based on fast simulators
//!- Tools: Misc. additional analysis tools included in CAP.
//!
//!Each of these modules are described in some detail in the following sub-sections.
//!
//!\subsubsection global Global -- Study of global event observables
//!
//!\subsubsection particle Particle  --  Study of single particle spectra
//!
//!\subsubsection pair Pair  --  Study of differential (long range) correlation functions R2, P2, and G2
//!
//!\subsubsection nudyn Nudyn  --  Study of integral correlation functions, moments, factorial moments, cumulants, and factorial cumulants
//!
//!\subsubsection subsample SubSample --  Calculation of statistical errors using the sub sample method.
//!
//!\subsubsection performance  Performance --  Study of the performance of   measurements of basic observables based on fast simulators
//!
//!\subsubsection tools Tools
//!
//!\subsection macros  Macros and Examples
//!
//!
//!\class Task
//!\brief Base class defining the notion of light weight tasks for the generation and analysis of data
//!
//!\details
//!# SubSampleStatCalculator
//!
//!\author C. Pruneau
//!\version 1.0
//!\date May 18, 2022
//!
//! <H1>Task</h1>
//!
//! The Task class is a base class used to define data analysis tasks and provide some of the basic resources required for the generation and analysis of data.
//!
//! The task base class provides the basic tools and functionality typically required for the analysis of simulation data as well as "real" data. This base class defines
//! basic methods needed for the analysis of data and the management of files, histograms, etc. Tasks are typically data centric and require access to one or more event
//! streams. Event streams used for i/o by a particular tasks are identified with boolean flags in the task configuration used by a speficific task instance. While most tasks
//! only required one stream (turned on with the useStream0 flag), it is also possible to use as many as 10 streams. This can be of interest, for instance, for tasks that
//! convert data or modify data (e.g., class ParticlePerformanceSimulator).
//!
//! <H2>Task/subtask(s)</H2>
//!
//! The Task class is defined to be recursive, i.e., a task can have one or many subTasks. An example of class having one or many subTasks is the TaskIterator class designed
//! control the analysis of data sets consisting of large numbers of events and requiring calls to different types of classes to carry out several types of operations. Typically
//! an analysis job using the TaskIterator class would use a task to generate or read data from file (e.g., WacPythia/PythiaEventGenerator), and one or more tasks to fill
//! histograms. To add a sub-task to a task, use the addSubTask method.
//!
//! The task class provides several methods (functions) used to carry out specific tasks, such as task initialization,, task execution,
//! task finilization, and basic operations related to histograms such as histogram creation, histogram scaling at the end of analysis (to carry normalization e.g.,
//! per event analyzed, histgram saving to file, calculations of 'derived' histograms, etc. Given a task can have one or many subTasks, a mechanism is required to
//! enable a task to automatically invoke its subTasks. Generic methods thus have two levels of invocation. For instance, to carry out the action for which a specific subclass
//! is designed, call its "execute()" method. However, if the class instance has subTasks and you wish to call the class "execute" as well as the execute methods
//! of all its subTasks, use the "executeSubTasks()" method. Similarly, methods to clear, reset, initialize, finalize tasks and their subTasks shall be called "clearSubTasks()",
//! "resetSubTasks()", etc,  in general "xxxxxTasks()".  Note, that the operation "xxxxx" is first called for the parent and next invoked for the subTasks. Actually, since
//! tasks can have subTasks recursively, it is this the "xxxxxTasks()" operation which is called on the subTasks. This method in turn calls the "xxxxxx()" on the subtask
//! and next all of its subTasks, if any, recursively.
//!
//! <H2>Operations/Methods</H2>
//!
//! Operations defined by the base class are as follows:
//! <ol>
//! <li>initialize(): A method called to initialize a task instance. In principle, one could think of initializing a task within its constructor. Within the WAC framework, however,
//! the configuration of a task is kept separate from the task itself and one can actually change the configuration of task after it has been instantiated. Since calling
//! the constructor anew is evidently not possible, the initialize() method provides the ability to actually carry out the initialization once the configuration has been
//! set, even though changes may be done after the task is instantiated. Note, however, there is not provision to automatically reinitialize a task if its configuration
//! is changed after initialization. This can be done "manually" however by calling the clear() or reset() methods of the class as appropriate.
//!
//! <li>execute(): A method called to carry out or execute the task at hand. The present version of the code does not implement a state engine that keeps track
//! of whether or not a task has been initialized or not. This falls under the responsibility of the user. Note that the TaskIterator class handles all operations sequentially
//! and gracefully thereby insuring operations are carried in "the proper order". Additionally note that if an exception or error occurs during the initialization, execution,
//! or any other stage of a task, a PostTaskStatus call is placed to identify the abnormal condition.
//!
//! <li>finalize(): A method called to finalize a task instance. The finalize() method implemented in the base class  automatically calls various functions to scale
//!histograms (per event scaling), to compute derived histogram (as needed), and save histograms to a root file.
//!
//! <li>reset():  A method called to rest the task so it can be execute anew.
//! <li>clear():  A method called to clear the task -- effectively putting the task into an "uninitialized" state.
//! <li>savePartial(): A method called to save the histogram objects handled  this task to an indexed file. The output path and name of the file are stipulated
//!by the configuration class. Partial saves, as opposed on a single "final" save, of the histogram are carried out automatically if the "partialSave" boolean
//!flag is set in the class configuration. The frequency of the partial saves is defined by the TaskIterator configuration class. The file name under which histograms are
//!saved is specified by a base file name stated in the class configuration and an partial save index automatically incremented after each partial save.
//!
//! <li>subsampleAnalysis(): A method called to carry a subsample analysis of the statistical uncertainties of all the histograms held by an instance of this class. This
//!methods uses the base name + partial index to iteratively open all the files to be included in the computation of the statistical uncertainties. Note that while root automatically
//!computes statistical errors or simple histograms, one cannot, typically, on regular error propagation techniques to compute the statistical uncertainties of complex
//!observables such as two, three, or more particle correlators. The subsample analysis technique thus provides a reasonable subtitute for this purpose. See XXXXX.
//! <li>printConfiguration(): A method to print  the configuration of  a task instance. While this method usually calls the printConfiguration() method of the configuration
//! class, it may be possible to print additional information into the output stream.
//! </ol>
//!
//! <H2>Analysis control </H2>
//! Data generation and analysis tasks rely on one or several event streams which present a task with one or several instances of the Event class (or possibly a subclass).
//! Tasks of interest typically involve the generation, transformation, or use of the event stream(s) to fill histograms. Whether feeding or retrieving events into an
//! event stream, it is useful to abstract the control of the type of events and particles produces or accepts based on generic criteria or filters. For filling histograms
//! of single particle observables or correlation observables should not depend explicitly on the type of events being read or the particles these events contain. Indeed,
//! histogram filling operations usually do not depend on details and can thus be considered "abstract" histogram filling tasks. Evidently, a particular analysis be
//! be focused on ultra peripheral or most central heavy-ion collisions or anything in between. It may also be of interest to specifically choose the type of
//! particles or what kinematic ranges are of interest. Instead of hard coding such choices, the WAC framework includes notions of event and particle filters, encapsulated
//! into the EventFilter and ParticleFilter classes, respectively. There is additionally a ParticlePairFilter class. These filter classes are designed to be very generic and
//! flexible but use a unique strategy and method to decide whether an event or a particle (or a pair of particles) should be accepted and thus used/readin/output
//! by a task. Deciding whether an event or a particle is acceptable, one simply callss the accept method of the filter:
//!
//! <pre>myEventFilter->accept(anEvent);</pre>
//!
//! or
//!
//! <pre>myParticleFilter->accept(aParticle);</pre>
//!
//! The WAC framework includes the classes EventFilter and ParticleFilter that both instrument this "accept" method. These two classes are designed to handle
//! a wide variety of conditions and types of analysis. Please refer to the documentation of these two classes for more information.
//!
//!In general, in a particular data analysis, it may be desirable to  classify the events to be analyzed in several different (partially overlapping or fully) distinct
//!categories. This is readily accomplished by instantiating several event filters as input to a task. Input event filters to a task can be accomplished at
//!Task construction time by submitting a vector of event filter or by adding filters one by one to the task using the addEventFilter method.
//!
//!Similarly, it may also be of interest to repeat the same exact type of analysis of several different particle types. This can be accomplished by instantiating several
//!particle filters and inputing them into a task. As for event filters, this can be accomplished at construction time or via the use of the addParticleFilter method.
//!
//!Once constructed and initialized with sets of event and particle filters, most task classes currently defined in the WAC framework will automatically test each
//!event and each particle considered against the several filters they own and the analysis will then be automatically repeated on types of events and for particles
//!of interest. This feature is currently implemented in the ParticleAnalyzer, ParticlePairAnalyze, NuDynAnalyzer, etc, tasks.
//!
//!Given root expects instantiated histograms to be distinct by name or put in different folders, the EventFilter and ParticleFilter classes feature filter name
//! generators that can be (and in fact used by several tasks) to speficify the name of histigrams. Typically, histogram names are build according to the following
//! pattern:
//!
//! <pre>Job Name + Task Name + Event Filter Name + Particle Filter Name + Observable Name</pre>
//!
//!
//!

class Task : public MessageLogger
{
protected:

  Timer timer;
  
  //!
  //! Name given to this task instance
  //!
  TString taskName;
  //!
  //! Pointer to a TaskConfiguation class which defines the behavior of this instance of the class Task and/or its derived class instances.
  //!
  Configuration configuration;

  
  //!
  //! Pointer to a factory of entities of type Particle.
  //!
  Factory<Particle> *      particleFactory;
  //!
  //! Array of pointers to streams (potentially) used by this task.
  //!
  vector<Event*>           eventStreams;

  //!
  //! number of EventFilter objects used by this class.. This is a convenience variable used to avoid calling size() repeatedly
  //!
  int nEventFilters;

  //!
  //! number of EventFilter objects used by this class.. This is a convenience variable used to avoid calling size() repeatedly
  //!
  int nParticleFilters;

  //!
  //! Array of pointers to event filters (class EventFilter) used by this task instance.
  //!
  vector<EventFilter*>     eventFilters;
  //!
  //! Array of pointers to particle filters (class ParticleFilter)  used by this task.
  //!
  vector<ParticleFilter*>  particleFilters;

  //! Pointer to an object of the class EventCountHistos. An instance of the EventCountHistos is used to keep track of the number of events
  //!  accepted by this task for each of its event filters.
  //!
  EventCountHistos *       eventCountHistos;
  //!
  //! Array of pointers to Histograms objects used on input for initialization or calibration of the activity of this task instance.
  //!
  vector<Histograms*>      inputHistograms;

  //!
  //! Array of pointers to generic histogram objects produced on output by this task instance.
  //!
  vector<Histograms*>      histograms;

  //!
  //! Array of pointers to single particle histogram objects produced on output by this task instance.
  //!
  vector<Histograms*>      baseSingleHistograms;
  //!
  //! Array of pointers to  particle pair histogram objects produced on output by this task instance.
  //!
  vector<Histograms*>      basePairHistograms;

  //!
  //! Array of pointers to generic histogram objects produced on output by this task instance.
  //!
  vector<Histograms*>      derivedHistograms;

  //!
  //! Array of pointers to Histograms objects produced on output by this task instance as "derivatives" of those contained in the array "histograms".
  //!
  vector<Histograms*>      derivedSingleHistograms;

  //!
  //! Array of pointers to Histograms objects produced on output by this task instance as "derivatives" of those contained in the array "histograms".
  //!
  vector<Histograms*>      derivedPairHistograms;

  //!
  //! Array of pointers to Histograms objects produced on output by this task instance as "combinations" of other histogram groups.
  //!
  vector<Histograms*>      combinedHistograms;
  //!
  //! Array of  number of events accepted by each of the event filters used by this task instance.
  //!


  //!
  //! Array of filter particles (ParticleDigits).
  //!
  vector< vector<ParticleDigit*> > filteredParticles;

  //!
  //! Number of times this task was executed (excute called)
  //!
  long nTaskExecuted;

  //!
  //! Number of times this task was executed (excute called) since last reset
  //!
  long nTaskExecutedReset;

  //!
  //!Number of events accepted by event filters used by this task since last reset (partial save)
  //!
  vector<long> nEventsAcceptedReset;

  //!
  //!Total number of events accepted by event filters used by this task since last reset (partial save)
  //!
  vector<long> nEventsAcceptedTotal;

  //!
  //!Number of particles accepted in the last event for each of the event filters and particle filters used by this task..
  //!
  vector<long> nParticlesAcceptedEvent;

  //!
  //!Number of particles accepted per each of  event filters and particle filters used by this task since last reset
  //!
  vector<long> nParticlesAcceptedReset;

  //!
  //!Number of particles accepted per each of  event filters and particle filters used by this task
  //!
  vector<long> nParticlesAcceptedTotal;

  //!
  //! Array of pointers to subTasks called by this task instance, once per event analyzed (or iteration generated by TaskIterator task). If this instance carries out
  //! initialize, finalize, execute type operations, these are performed BEFORE the corresponding operations by the subTasks.
  //!
  vector<Task*> subTasks;


  //!
  //! Pointer to a singleton of the class ParticleTypeCollection possibly used by this task instance for particle type identification and classification purposes.
  //!
  ParticleTypeCollection * particleTypeCollection;

  int subSampleIndex;

public:

  //!
  //! Default constructor. It allocates resources but DOES NOT initialize the task. Task initialization can be performed by a call to the initializationTask() and/or
  //!  initialization() methods.
  //!
  Task();
  
  //!
  //! Long constructor. It allocates resources but DOES NOT initialize the task. Task initialization can be performed by a call to the initializationTask() and/or
  //!  initialization() methods.
  //!
  Task(const TString & _name,
       const Configuration & _configuration,
       MessageLogger::LogLevel _reportLevel);

  //!
  //! Longer constructor. It allocates resources but DOES NOT initialize the task. Task initialization can be performed by a call to the initializationTask() and/or
  //!  initialization() methods.
  //!
  Task(const TString &          _name,
       const Configuration  &   _configuration,
       vector<EventFilter*> &   _eventFilters,
       vector<ParticleFilter*>& _particleFilters,
       MessageLogger::LogLevel  _reportLevel);

  //!
  //! dtor
  virtual ~Task() {}
  
  //!
  //! Initialize the configuration parameter of the task to their default value;
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Set configuration parameters of this task to values in the given config;
  //!
  void setConfiguration(const Configuration & config);
  
  //!
  //! Method called for the initialization of this task instance and  its subTasks. This method first calls the iniatilize() method implemented by this class or  one of its derived classes.
  //! It next calls the initializeSubTasks() method of each  of the  subTasks of this instance, if any.
  //!
  void initializeSubTasks();
  //!
  //! Method called to execute this task instance and  its subTasks. This method first calls the execute() method implemented by this class or  one of its derived classes.
  //! It next calls the executeSubTasks() method of each  of the  subTasks of this instance, if any.
  //!
  void executeSubTasks();
  //!
  //! Method called to finalize this task instance and  its subTasks. This method first calls the finalize() method implemented by this class or  one of its derived classes.
  //! It next calls the finalizeSubTasks() method of each  of the  subTasks of this instance, if any.
  //!
  void finalizeSubTasks();
  //!
  //! Method called to rest this task instance and its subTasks. This method first calls the reset() method implemented by this class or  one of its derived classes.
  //! It next calls the resetSubTasks() method of each  of the  subTasks of this instance, if any. See the class documentation for the definition of task "reset".
  //!
  void resetSubTasks();
  //!
  //! Method called to clear  this task instance and its subTasks. This method first calls the clear() method implemented by this class or  one of its derived classes.
  //! It next calls the resetSubTasks() method of each  of the  subTasks of this instance, if any. See the class documentation for the definition of task "clear".
  //!
  void clearSubTasks();

  //!
  //! Method called to save the histogram objects handled  this task instance and its subTasks. This method first calls the savePartial() method implemented by this class or  one of its derived classes.
  //! It next calls the savePartialSubTasks() method of each  of the  subTasks of this instance, if any. See the class documentation to figure out how to generate partial saves.
  //!
  void savePartialSubTasks();

  //!
  //! Method called to print  the configuration of  this task instance and its subTasks. This method first calls the printConfiguration() method implemented by this class or  one of its derived classes.
  //! It next calls the printConfigurationTasks() method of each  of the  subTasks of this instance, if any. The output is sent to the ostream identified by the argument "output".
  //!
  void printConfigurationSubTasks(ostream & output);

  //!
  //! Returns a boolean (true) is this task instance has subTasks.
  //!
  bool hasSubTasks()  const
  {
  return subTasks.size()>0;
  }

  //!
  //! Returns the number of subTasks held by this task instance.
  //!
  unsigned int getNSubTasks() const
  {
  return subTasks.size();
  }

  //!
  //! Returns a pointer to the subtask at the given index. The current implementation of the code does not checl whether the index might be larger
  //! than the actual number of subTasks held by this instance. As such, a call with a value of index in excess of the number of subTasks will yield
  //!  an exception (segmentation fault).
  //!
  Task * getSubTaskAt(unsigned int index)
  {
  return subTasks[index];
  }

  //!
  //! Adds the given taks a subtask of this task instance.
  //!
  Task *  addSubTask(Task * task);

  //!
  //! Initialize this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void initialize();

  //!
  //! Execute this task instance. This base class implementation only increments the counters nEventProcessed and nEventAccepted. Implement this method in
  //! derived class to achieve the actual task of interest. Note: You can increase the counters nEventProcessed and nEventAccepted indiscrimately
  //! by a call to Task::execute(). However, in general, you may wish to count all events looked and only count as accepted those that passed one of the event
  //! filters used by this task. To achive this, use calls to the methods incrementTaskExecuted()() and incrementNEventsAccepted() rather than explicitly incrementing
  //! the variable nEventProcessed and nEventAccepted. These are currently protected  but may defined as private in a future release.
  //!
  virtual void execute();

  //!
  //! Finalize this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void finalize();

  //!
  //! Reset this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void reset();

  //!
  //! Clear this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void clear();

  //!
  //! Save histogram groups owned by this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void savePartial();

  //!
  //! Print this class instance configuration. This is typically achieved by a clas to the printProperties of the task configuration  (Configuration) instance
  //! used to control the behavior of this class.
  //!
  virtual void printConfiguration(ostream & output);

  //!
  //! Nominally creates histogram groups used by this task. The method of this based class does not perform any histogram instantiation and must be
  //! overiden in a derived class to achieve the desited behavior.
  //!
  virtual void createHistograms() {}

  //!
  //! Loads histogram groups used by this task. This method opens the root file identified in the Configuration object owned by this task. If the file exists,
  //! it calls the  loadHistograms(TFile * inputFile) to actually load the relevant groups.
  //!
  virtual void loadHistograms();

  //!
  //! Implement this method in a derived class to load from a root file the histogram groups that are need for the execution of this task.
  //!
  virtual void loadHistograms(TFile * inputFile __attribute__((unused))) {  }

  //!
  //! Calls the reset method of all the histogram groups owned by this task.
  //!
  virtual void resetHistograms();
  //!
  //! Calls the clear method of all the histogram groups owned by this task. This effectively delete the histogram groups. Only call this method if you know
  //! what you are doing..
  //!
  virtual void clearHistograms();

  //!
  //! Scale (all) the histograms held in all the histogram groups owned by this task instance. This operation is executed automatically within the finalize() method call if
  //! and only if the scaleHistograms parameter of the Configuration instance controlling this task is set to "true".
  //!
  virtual void scaleHistograms();
  //!
  //! Save this histogram in text format.
  //!
  virtual void saveHistogramsAsText();

  //!
  //! Save this histogram groups owned by this task instance to the given file.
  //!
  virtual void saveHistograms(TFile * outputFile);

  //!
  //! Save this histogram groups owned by this task instance. This method calls opens the root file specified in the configuration object and calls the saveHistogram
  //! methods of the histogram groups owned by this instance.
  //!
  virtual void saveHistograms();

  //!
  //! Add the histograms of this task to the given TList.
  //!
  virtual void addHistogramsToExtList(TList *list);

  //!
  //! Read the given named parameter from the given input file.
  //!
  virtual long readParameter(TFile * inputFile, const TString & parameterName);

  //!
  //! Open the root file named "fileName" located on the path "inputPath", using options specified by "ioOption".
  //! @param inputPath path where to find or create the file.
  //! @param fileName name of the root file to open for i/o.
  //! @param ioOption i/o options.
  //! @return Pointer to the file if successfully open or a null pointer if the file could not be opened.
  //!
  TFile * openRootFile(const TString & inputPath, const TString & fileName, const TString & ioOption);

  //!
  //! Returns a pointer to the event stream at the given index. No sanity check is performed in order to keep this call as efficient as possible.
  //! @param index index of the event stream to produce on output.
  //! @return pointer to the selected event stream.
  //!
  Event * getEventStream(unsigned int index)
  {
  return eventStreams[index];
  }

  //!
  //! Add the given event as a stream used by this task instance.
  //! @param event stream to be added to this task.
  //!
  void addEventStream(Event * event)
  {
  eventStreams.push_back(event);
  }

  //!
  //! Returns the number of event streams used  by this task instance.
  //! @return number of streams associated with this task instance.
  //!
  unsigned int getNEventStreams() const
  {
  return eventStreams.size();
  }

  //!
  //! Abstract class designed to probide a place holder for a concreate analysis class to instantiate and return  a related DerivedCalculator task.
  //!
  //! @return DerivedCalculator task.
  //!
  virtual Task * getDerivedCalculator()
  {
  return nullptr;
  }

  //!
  //! Returns a pointer to the Configuration object controlling the behavior of this task instance.
  //! @return pointer to the Configuration instance controlling this task instance.
  //!
  const Configuration & getConfiguration() const
  {
  return configuration;
  }

  Configuration & getConfiguration()
  {
  return configuration;
  }

  inline ParticleTypeCollection * getParticleTypeCollection() const
  {
  return particleTypeCollection;
  }

  inline ParticleTypeCollection * getParticleTypeCollection()
  {
  return particleTypeCollection;
  }

  //!
  //! Return the name of this task instance.
  //! @return name of this task instance.
  //!
  inline TString getName() const
  {
  return taskName;
  }

  //!
  //! Set the name of this task instance.
  //!
  inline void setName(const TString & _name)
  {
  taskName = _name;
  }

  inline void incrementTaskExecuted()
  {
  nTaskExecuted++;
  nTaskExecutedReset++;
  }

  inline void initializeTaskExecuted()
  {
  nTaskExecuted = 0;
  nTaskExecutedReset = 0;
  }

  inline void resetTaskExecuted()
  {
  nTaskExecutedReset = 0;
  }

  inline void clearTaskExecuted()
  {
  nTaskExecuted = 0;
  nTaskExecutedReset = 0;
  }

  inline long getNTaskExecuted()
  {
  return nTaskExecuted;
  }


  inline long getNTaskExecutedReset()
  {
  return nTaskExecutedReset;
  }

  inline void initializeNEventsAccepted()
  {
  nEventsAcceptedReset.assign(nEventFilters,0);
  nEventsAcceptedTotal.assign(nEventFilters,0);
  }

  inline void incrementNEventsAccepted(int iEventFilter=0)
  {
  nEventsAcceptedReset[iEventFilter]++;
  nEventsAcceptedTotal[iEventFilter]++;
  }

  inline void resetNEventsAccepted()
  {
  nEventsAcceptedReset.assign(nEventFilters,0);
  }

  inline void clearNEventsAccepted()
  {
  nEventsAcceptedReset.assign(nEventFilters,0);
  nEventsAcceptedTotal.assign(nEventFilters,0);
  }

  inline int getNEventsAcceptedReset(int iEventFilter=0) const
  {
  if (iEventFilter<0 || iEventFilter>=nEventFilters)
    return -1;
  else
    return nEventsAcceptedReset[iEventFilter];
  }

  inline int getNEventsAcceptedTotal(int iEventFilter=0) const
  {
  if (iEventFilter<0 || iEventFilter>=nEventFilters)
    return -1;
  else
    return nEventsAcceptedTotal[iEventFilter];
  }

  void writeNEexecutedTask(TFile * outputFile);
  long loadNEexecutedTask(TFile * inputFile);
  void writeNEventsAccepted(TFile * outputFile);
  void loadNEventsAccepted(TFile * outputFile);

  vector<long> nParticleAcceptedEvent;
  vector<long> nParticleAcceptedReset;
  vector<long> nParticleAcceptedTotal;

  inline void initializeNParticlesAccepted()
  {
  int n = nEventFilters*nParticleFilters;
  nParticleAcceptedEvent.assign(n,0);
  nParticleAcceptedReset.assign(n,0);
  nParticleAcceptedTotal.assign(n,0);
  }

  inline void incrementNParticlesAccepted(int iEventFilter=0, int iParticleFilter=0)
  {
  int index = iEventFilter*nParticleFilters+iParticleFilter;
  nParticleAcceptedEvent[index]++;
  nParticleAcceptedReset[index]++;
  nParticleAcceptedTotal[index]++;
  }

  inline void resetNParticlesAcceptedEvent()
  {
  int n = nEventFilters*nParticleFilters;
  nParticleAcceptedEvent.assign(n,0);
  }

  inline void resetNParticlesAccepted()
  {
  int n = nEventFilters*nParticleFilters;
  nParticleAcceptedEvent.assign(n,0);
  nParticleAcceptedReset.assign(n,0);
  }

  inline void clearNParticlesAccepted()
  {
  int n = nEventFilters*nParticleFilters;
  nParticleAcceptedEvent.assign(n,0);
  nParticleAcceptedReset.assign(n,0);
  nParticleAcceptedTotal.assign(n,0);
  }

  inline int getNParticlesAcceptedEvent(int iEventFilter=0, int iParticleFilter=0)  const
  {
  if (iEventFilter<0 || iEventFilter>=nEventFilters)
    return -1;
  else
    {
    if (iParticleFilter<0 || iParticleFilter>=nParticleFilters)
      return -1;
    else
      return nParticleAcceptedEvent[iEventFilter*nParticleFilters+iParticleFilter];
    }
  }

  inline int getNParticlesAcceptedReset(int iEventFilter=0, int iParticleFilter=0) const
  {
  if (iEventFilter<0 || iEventFilter>=nEventFilters)
    return -1;
  else
    {
    if (iParticleFilter<0 || iParticleFilter>=nParticleFilters)
      return -1;
    else
      return nParticleAcceptedReset[iEventFilter*nParticleFilters+iParticleFilter];
    }
  }

  inline int getNParticlesAcceptedTotal(int iEventFilter=0, int iParticleFilter=0) const
  {
  if (iEventFilter<0 || iEventFilter>=nEventFilters)
    return -1;
  else
    {
    if (iParticleFilter<0 || iParticleFilter>=nParticleFilters)
      return -1;
    else
      return nParticleAcceptedTotal[iEventFilter*nParticleFilters+iParticleFilter];
    }
  }


  void printEventStatistics() const;

  bool hasEventCountHistograms();
  void createEventCountHistograms();
  void loadEventCountHistograms(TFile * inputFile);
  void fillEventCountHistograms();
  void saveEventCountHistograms(TFile * outputFile);
  void resetEventCountHistograms();
  void clearEventCountHistograms();


  //!
  //! Save the given (long) value with the given name in the given output file.
  //!
  void writeParameter(TFile * outputFile, const TString parameterName, long value);

  //!
  //! Clear the event filters used by this task instance.
  //!
  void clearEventFilters()
  {
  eventFilters.clear();
  }

  //!
  //! Set the event filters used by this task instance.
  //!
  void setEventFilters(vector<EventFilter*> _eventFilters)
  {
  eventFilters = _eventFilters;
  }

  //!
  //! Add an event filter to the event filters used by this task instance.
  //!
  void addEventFilter(EventFilter* _eventFilter)
  {
  eventFilters.push_back(_eventFilter);
  }

  //!
  //! Return event filters used by this task instance.
  //!
  vector<EventFilter*> & getEventFilters()
  {
  return eventFilters;
  }

  //!
  //! Return event filters used by this task instance.
  //!
  const vector<EventFilter*> & getEventFilters() const
  {
  return eventFilters;
  }

  //!
  //! Clear the particle filters used by this task instance.
  //!
  void clearParticleFilters()
  {
  eventFilters.clear();
  }

  //!
  //! Set particle filters  used by this task instance.
  //!
  void setParticleFilters(vector<ParticleFilter*> _particleFilters)
  {
  particleFilters = _particleFilters;
  }

  //!
  //! Add a particle filter to the particle filters used by this task instance.
  //!
  void addParticleFilter(ParticleFilter* _particleFilter)
  {
  particleFilters.push_back(_particleFilter);
  }

  //!
  //! Return particle filters used by this task instance.
  //!
  vector<ParticleFilter*> & getParticleFilters()
  {
  return particleFilters;
  }

  //!
  //! Return particle filters used by this task instance.
  //!
  const vector<ParticleFilter*> & getParticleFilters() const
  {
  return particleFilters;
  }

  inline unsigned int getNHistograms() const              { return histograms.size();   }
  inline unsigned int getNBaseSingleHistograms() const    { return baseSingleHistograms.size();   }
  inline unsigned int getNBasePairHistograms() const      { return basePairHistograms.size();     }
  inline unsigned int getNDerivedSingleHistograms() const { return derivedSingleHistograms.size();  }
  inline unsigned int getNDerivedPairHistograms() const   { return derivedPairHistograms.size();  }
  inline unsigned int getNCombinedHistograms() const      { return combinedHistograms.size(); }


  inline vector<Histograms*> & getBaseSingleHistograms()    { return baseSingleHistograms;    }
  inline vector<Histograms*> & getBasePairHistograms()      { return basePairHistograms;      }
  inline vector<Histograms*> & getDerivedSingleHistograms() { return derivedSingleHistograms; }
  inline vector<Histograms*> & getDerivedPairHistograms()   { return derivedPairHistograms;   }
  inline vector<Histograms*> & getCombinedPairHistograms()  { return combinedHistograms;      }


  const TString makeHistoName(const TString & s0,
                              const TString & s1);

  const TString makeHistoName(const TString & s0,
                              const TString & s1,
                              const TString & s2);

  const TString makeHistoName(const TString & s0,
                              const TString & s1,
                              const TString & s2,
                              const TString & s3);

  const TString makeHistoName(const TString & s0,
                              const TString & s1,
                              const TString & s2,
                              const TString & s3,
                              const TString & s4);
  
  const TString makeHistoName(const TString & s0,
                              const TString & s1,
                              const TString & s2,
                              const TString & s3,
                              const TString & s4,
                              const TString & s5);


  void setHistogramFileNames(const TString inputName, const TString outputName)
  {
  configuration.setParameter("fileFromParent", true);
  configuration.setParameter("histogramInputFileName", inputName);
  configuration.setParameter("histogramOuputFileName", outputName);
  }
  
  vector<TString> listFilesInDir(const TString & dirname,
                                 const TString ext=".root");
  vector<TString> listFilesInDir(const TString & pathName,
                                 vector<TString> includePatterns,
                                 vector<TString> excludePatterns);

  //!
  //!Get selected files in the given directory. The selection is made by means of two sets of configuration parameters
  //!of the form IncludePattern# and ExcludePattern# (where # represents a number between 0 and 20. These include
  //!and exclude patterns are used to assemble two lists, vector<TString> includePatterns and vector<TString> excludePatterns)
  //!which are then used to identify all file names of the given folder that match the includePatterns but exclude the excludePatterns.
  //!
  vector<TString> getSelectedFileNamesFrom(const TString & folder);
  

  //!
  //!Removes the ".root" ending of the given string if it has this ending.
  //!This function is used in the management of file names for subsampling analyses and
  //!the calculatioin of derived histograms.
  //!
  TString removeRootExtension(const TString fileName);



  ClassDef(Task,0)
};

#endif /* CAP__Task */
