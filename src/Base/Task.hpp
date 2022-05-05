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
#include "ParticleSmearer.hpp"
#include "Histograms.hpp"
#include "ParticleTypeCollection.hpp"
#include "EventCountHistos.hpp"
#include "ParticleDigit.hpp"
#include "Timer.hpp"

using namespace std;

//!
//! <H1>Task</h1>
//!
//! The Task class is a base class used to define data analysis tasks and provide some of the basic resources required for the generation and analysis of data.
//!
//! The task base class provides the basic tools and functionality typically required for the analysis of simulation data as well as "real" data. This base class defines
//! basic methods needed for the analysis of data and the management of files, histograms, etc. Tasks are typically data centric and require access to one or more event
//! streams. Event streams used for i/o by a particular tasks are identified with boolean flags in the task configuration used by a speficific task instance. While most tasks
//! only required one stream (turned on with the useStream0 flag), it is also possible to use as many as 10 streams. This can be of interest, for instance, for tasks that
//! convert data or modify data (e.g., class ParticleSmearer).
//!
//! <H2>Task/subtask(s)</H2>
//!
//! The Task class is defined to be recursive, i.e., a task can have one or many subtasks. An example of class having one or many subtasks is the TaskIterator class designed
//! control the analysis of data sets consisting of large numbers of events and requiring calls to different types of classes to carry out several types of operations. Typically
//! an analysis job using the TaskIterator class would use a task to generate or read data from file (e.g., WacPythia/PythiaEventGenerator), and one or more tasks to fill
//! histograms. To add a sub-task to a task, use the addSubtask method.
//!
//! The task class provides several methods (functions) used to carry out specific tasks, such as task initialization,, task execution,
//! task finilization, and basic operations related to histograms such as histogram creation, histogram scaling at the end of analysis (to carry normalization e.g.,
//! per event analyzed, histgram saving to file, calculations of 'derived' histograms, etc. Given a task can have one or many subtasks, a mechanism is required to
//! enable a task to automatically invoke its subtasks. Generic methods thus have two levels of invocation. For instance, to carry out the action for which a specific subclass
//! is designed, call its "execute()" method. However, if the class instance has subtasks and you wish to call the class "execute" as well as the execute methods
//! of all its subtasks, use the "executeTasks()" method. Similarly, methods to clear, reset, initialize, finalize tasks and their subtasks shall be called "clearTasks()",
//! "resetTasks()", etc,  in general "xxxxxTasks()".  Note, that the operation "xxxxx" is first called for the parent and next invoked for the subtasks. Actually, since
//! tasks can have subtasks recursively, it is this the "xxxxxTasks()" operation which is called on the subtasks. This method in turn calls the "xxxxxx()" on the subtask
//! and next all of its subtasks, if any, recursively.
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
  //! Pointer to a random number generator of the class TRandom. This variable is set to "gRandom" by default and thus does not need
  //!   to be explictly by the user/developer.
  //!
  TRandom * taskRandomGenerator;
  //!
  //! Pointer to a factory of entities of type Particle.
  //!
  Factory<Particle> *      particleFactory;
  //!
  //! Array of pointers to streams (potentially) used by this task.
  //!
  vector<Event*>           eventStreams;
  //!
  //! Array of pointers to event filters (class EventFilter) used by this task instance.
  //!
  vector<EventFilter*>     eventFilters;
  //!
  //! Array of pointers to particle filters (class ParticleFilter)  used by this task.
  //!
  vector<ParticleFilter*>  particleFilters;
  //!
  //! Array of pointers to particle smearers (class ParticleSmearer)  used by this task.
  //!
  vector<ParticleSmearer*> particleSmearers;
  //!
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
  vector<double>           nFilteredEventsAccepted;
  //!
  //! Number of events processed by this task instance.
  //!
  long nEventProcessed;
  //!
  //! Number of events accepted for analysis by this task instance. Note that this number is actually the sum of the number of events accepted
  //! by the event filters used by this task.
  //!
  long nEventAccepted;

  //!
  //! Number of particles counted by this task instance in a given event.
  //!
  unsigned int particlesCounted;    // read or generated multiplicity (before filter cut)

  //!
  //! Number of particles accepted by particle filters associated with this task instance in a given event.
  //!
  unsigned int particlesAccepted;   // accepted multiplicity by one or many particle filters.




  //!
  //! Index to be tagged to the name of histogram files produced by this task on output. These tags are used to keep track of the sub-samples of events used
  //! in a particular data analysis. The files can then be read in by SubSampleStatCalculator task to compute estimate of the statitical errors of all histograms
  //! of all histogram group owned/managed by this task.
  //!
  int  subSampleIndex;
  //!
  //! Array of pointers to subtasks called by this task instance, once per event analyzed (or iteration generated by TaskIterator task). If this instance carries out
  //! initialize, finalize, execute type operations, these are performed BEFORE the corresponding operations by the subtasks.
  //!
  vector<Task*> subtasks;
  //!
  //! Pointer to a singleton of the class ParticleTypeCollection possibly used by this task instance for particle type identification and classification purposes.
  //!
  ParticleTypeCollection * masterCollection;


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
  //! Longer constructor. It allocates resources but DOES NOT initialize the task. Task initialization can be performed by a call to the initializationTask() and/or
  //!  initialization() methods.
  //!
  Task(const TString &           _name,
       const Configuration  &    _configuration,
       vector<EventFilter*> &    _eventFilters,
       vector<ParticleFilter*>&  _particleFilters,
       vector<ParticleSmearer*>& _particleSmearer,
       MessageLogger::LogLevel   _reportLevel);

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
  //! Method called for the initialization of this task instance and  its subtasks. This method first calls the iniatilize() method implemented by this class or  one of its derived classes.
  //! It next calls the initializeTasks() method of each  of the  subtasks of this instance, if any.
  //!
  void initializeTasks();
  //!
  //! Method called to execute this task instance and  its subtasks. This method first calls the execute() method implemented by this class or  one of its derived classes.
  //! It next calls the executeTasks() method of each  of the  subtasks of this instance, if any.
  //!
  void executeTasks();
  //!
  //! Method called to finalize this task instance and  its subtasks. This method first calls the finalize() method implemented by this class or  one of its derived classes.
  //! It next calls the finalizeTasks() method of each  of the  subtasks of this instance, if any.
  //!
  void finalizeTasks();
  //!
  //! Method called to rest this task instance and its subtasks. This method first calls the reset() method implemented by this class or  one of its derived classes.
  //! It next calls the resetTasks() method of each  of the  subtasks of this instance, if any. See the class documentation for the definition of task "reset".
  //!
  void resetTasks();
  //!
  //! Method called to clear  this task instance and its subtasks. This method first calls the clear() method implemented by this class or  one of its derived classes.
  //! It next calls the resetTasks() method of each  of the  subtasks of this instance, if any. See the class documentation for the definition of task "clear".
  //!
  void clearTasks();

  //!
  //! Method called to save the histogram objects handled  this task instance and its subtasks. This method first calls the savePartial() method implemented by this class or  one of its derived classes.
  //! It next calls the savePartialTasks() method of each  of the  subtasks of this instance, if any. See the class documentation to figure out how to generate partial saves.
  //!
  void savePartialTasks();


  //!
  //! Carry out a sub-sample analysis to compute the statistical uncertainties of the histograms of this task and its subtasks.
  //!
  void subsampleAnalysisTasks();


  //!
  //! Method called to print  the configuration of  this task instance and its subtasks. This method first calls the printConfiguration() method implemented by this class or  one of its derived classes.
  //! It next calls the printConfigurationTasks() method of each  of the  subtasks of this instance, if any. The output is sent to the ostream identified by the argument "output".
  //!
  void printConfigurationTasks(ostream & output);
  //!
  //! Returns a boolean (true) is this task instance has subtasks.
  //!
  bool hasSubtasks()  const
  {
  return subtasks.size()>0;
  }
  //!
  //! Returns the number of subtasks held by this task instance.
  //!
  unsigned int getNSubtasks() const
  {
  return subtasks.size();
  }
  //!
  //! Returns a pointer to the subtask at the given index. The current implementation of the code does not checl whether the index might be larger
  //! than the actual number of subtasks held by this instance. As such, a call with a value of index in excess of the number of subtasks will yield
  //!  an exception (segmentation fault).
  //!
  Task * getSubtaskAt(unsigned int index)
  {
  return subtasks[index];
  }
  //!
  //! Adds the given taks a subtask of this task instance.
  //!
  Task *  addSubtask(Task * task);
  //!
  //! Initialize this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void initialize();
  //!
  //! Execute this task instance. This base class implementation only increments the counters nEventProcessed and nEventAccepted. Implement this method in
  //! derived class to achieve the actual task of interest. Note: You can increase the counters nEventProcessed and nEventAccepted indiscrimately
  //! by a call to Task::execute(). However, in general, you may wish to count all events looked and only count as accepted those that passed one of the event
  //! filters used by this task. To achive this, use calls to the methods incrementEventProcessed() and incrementEventAccepted() rather than explicitly incrementing
  //! the variable nEventProcessed and nEventAccepted. These are currently protected  but may defined as private in a future release.
  //!
  virtual void execute()
  {
    ++nEventProcessed;
    ++nEventAccepted;
  }
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
  //! Print event statistics on standard output.
  //!
  virtual void printEventCount() const;
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
  //! what you are doing...
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
  //! Carry out a sub-sample analysis to compute the statistical uncertainties of the histograms of this task and its subtasks.
  //!
  virtual void subsampleAnalysis();



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
  inline void setName(const TString & name)
  {
    taskName = name;
  }

  //!
  //! Return a pointer to the random generator used by this task instance.
  //!
  inline TRandom * getRandomGenerator()
  {
  return taskRandomGenerator;
  }

  //!
  //! Increment the number of event processed by this task instance.
  //!
  inline void incrementEventProcessed()
  {
    ++nEventProcessed;
  }

  //!
  //! Increment the number of event accepted (and used, say, to increment/fill histograms) by this task instance.
  //!
  inline void incrementEventAccepted(unsigned int eventFilterIndex)
  {
    ++nEventAccepted;
    if (nFilteredEventsAccepted.size()>0) nFilteredEventsAccepted[eventFilterIndex] += 1;
  }

  //!
  //! Return the number of event processed by this task instance.
  //!
  inline long getNEventProcessed() const
  {
  return nEventProcessed;
  }

  //!
  //! Return the number of event accepted (and used, say, to increment/fill histograms) by this task instance.
  //!
  inline long getNEventAccepted() const
  {
  return nEventAccepted;
  }

  //!
  //! Return the current value of the file (and event subset) index.
  //!
  inline long getSubSampleIndex() const
  {
  return subSampleIndex;
  }

  //!
  //! Save the given (long) value with the given name in the given output file.
  //!
  void writeParameter(TFile * outputFile, const TString parameterName, long value);

  //!
  //! Save the number of event processed by this task instance to the given TFile output.
  //!
  void saveNEventProcessed(TFile * outputFile);
  //!
  //! Save the number of event accepted by this task instance to the given TFile output.
  //!
  void saveNEventAccepted(TFile * outputFile);

  //!
  //! Save the number of event accepted by each event filter own by this task instance to the given TFile output.
  //!
  void saveNFilteredEventsAccepted(TFile * outputFile, vector<unsigned int> & nFilteredEventsAccepted);

  //!
  //! Load the number of event processed by a previous  task instance from the given TFile.
  //!
  void loadNEventProcessed(TFile * inputFile);

  //!
  //! Load the number of event accepted by a previous   task instance from the given TFile.
  //!
  void loadNEventAccepted(TFile * inputFile);
  //!
  //! Load the number of event accepted for each event filter by a previous   task instance from the given TFile.
  //!
  void loadNFilteredEventsAccepted(TFile * inputFile, vector<unsigned int> & nFilteredEventsAccepted);

  //!
  //! Reset to zero the particle counters associated with this task instance
  //!
  inline void resetParticleCounters()
  {
  particlesCounted  = 0;
  particlesAccepted = 0;
  }

  //!
  //! Increment by one unit the number of particles "seen" by this task in a given event.
  //!
  inline void incrementParticlesCounted()      { particlesCounted++; }

  //!
  //! Increment by one unit the number of particles accepted by this task in a given event.
  //!
  inline void incrementParticlesAccepted()     { particlesAccepted++; }

  //!
  //! Return the number of particles "seen" by this task in a given event.
  //!
  inline unsigned int getNParticlesCounted() const   { return particlesCounted; }

  //!
  //! Return the number of particles accepted by this task in a given event.
  //!
  inline unsigned int getNParticlesAccepted() const  { return particlesAccepted; }

  //!
  //! Set the random number generator used by this task instance.
  //!
  void setRandomGenerator(TRandom * randomGenerator);

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

  
  //!
  //! Clear the particle smearer used by this task instance.
  //!
  void clearParticleSmearer()
  {
  particleSmearers.clear();
  }

  //!
  //! Set particle  smearers  used by this task instance.
  //!
  void setParticleSmearers(vector<ParticleSmearer*> _particleSmearers)
  {
  particleSmearers = _particleSmearers;
  }

  //!
  //! Add a particle  smearer to those  used by this task instance.
  //!
  void addParticleSmearer(ParticleSmearer*  _particleSmearer)
  {
  particleSmearers.push_back(_particleSmearer);
  }

  //!
  //! Return particle  smearers   used by this task instance.
  //!
  vector<ParticleSmearer*> getParticleSmearer()
  {
   return particleSmearers;
  }

  inline unsigned int getNHistograms() const              { return histograms.size();   }
  inline unsigned int getNBaseSingleHistograms() const    { return baseSingleHistograms.size();   }
  inline unsigned int getNBasePairHistograms() const      { return basePairHistograms.size();     }
  inline unsigned int getNDerivedSingleHistograms() const { return derivedSingleHistograms.size();  }
  inline unsigned int getNDerivedPairHistograms() const   { return derivedPairHistograms.size();  }
  inline unsigned int getNCombinedHistograms() const      { return combinedHistograms.size(); }

  //vector<Histograms*>       histograms;

  inline vector<Histograms*> & getBaseSingleHistograms()    { return baseSingleHistograms;    }
  inline vector<Histograms*> & getBasePairHistograms()      { return basePairHistograms;      }
  inline vector<Histograms*> & getDerivedSingleHistograms() { return derivedSingleHistograms; }
  inline vector<Histograms*> & getDerivedPairHistograms()   { return derivedPairHistograms;   }
  inline vector<Histograms*> & getCombinedPairHistograms()  { return combinedHistograms;      }

  const TString createHistogramName(const TString & baseName,
                                    const TString & evtFilterName,
                                    const TString & partFilterName,
                                    const TString & observableName="",
                                    const TString & suffix="");
  
  const TString createPairHistogramName(const TString & baseName,
                                        const TString & evtFilterName,
                                        const TString & partFilterName1,
                                        const TString & partFilterName2,
                                        const TString & observableName="",
                                        const TString & suffix="");


  void setHistogramFileNames(const TString inputName, const TString outputName)
  {
  configuration.setParameter("fileFromParent", true);
  configuration.setParameter("histogramInputFileName", inputName);
  configuration.setParameter("histogramOuputFileName", outputName);
  }
  
  static vector<TString> listFilesInDir(const TString & dirname,
                                        const TString ext=".root");
  static vector<TString> listFilesInDir(const TString & pathName,
                                        vector<TString> includePatterns,
                                        vector<TString> excludePatterns);

  //!
  //!Get selected files in the given directory. The selection is made by means of two sets of configuration parameters
  //!of the form IncludePattern# and ExcludePattern# (where # represents a number between 0 and 20. These include
  //!and exclude patterns are used to assemble two lists, vector<TString> includePatterns and vector<TString> excludePatterns)
  //!which are then used to identify all file names of the given folder that match the includePatterns but exclude the excludePatterns. 
  //!
  vector<TString> getSelectedFileNamesFrom(const TString & folder);
  
  static TString removeRootExtension(const TString fileName);

  ClassDef(Task,0)
};

#endif /* CAP__Task */
