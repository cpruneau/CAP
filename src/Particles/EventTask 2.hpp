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
#ifndef CAP__EventTask
#define CAP__EventTask
#include "Task.hpp"
#include "Factory.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "ParticleType.hpp"
#include "ParticleDb.hpp"
#include "HistogramGroup.hpp"

namespace CAP
{
//class ParticleDb;

class EventTask : public Task
{
public:


protected:

  bool   eventsCreate;
  bool   eventsCreateCAP;
  bool   eventsCreateNative;
  long   eventsRequested;
  bool   eventsConvertToCAP;
  bool   eventsConvertToNative;
  bool   eventsImport;
  bool   eventsImportCAP;
  bool   eventsImportNative;
  String eventsImportTree;
  String eventsImportPath;
  String eventsImportFile;
  int    eventsImportFileMinIndex;
  int    eventsImportFileMaxIndex;
  bool   eventsExport;
  String eventsExportPath;
  String eventsExportFile;
  String eventsExportTree;
  bool   eventsExportNative;
  bool   eventsExportCAP;
  long   eventsExportMaxPerFile;
  bool   eventsUseStream0;
  bool   eventsUseStream1;
  bool   eventsUseStream2;
  bool   eventsUseStream3;
  bool   eventsAnalyze;

  bool   calibsCreate;
  bool   calibsReset;
  bool   calibsClear;
  bool   calibsForceRewrite;
  bool   calibsImport;
  String calibsImportPath;
  String calibsImportFile;
  bool   calibsExport;
  bool   calibsExportAsRoot;
  bool   calibsExportAsText;
  String calibsExportPath;
  String calibsExportFile;


  //!
  //!Particle type collection or database
  //!
  ParticleDb * particleDb;

  //!
  //! Pointer to a factory of entities of type Particle.
  //!
  Factory<Particle> *  particleFactory;
  //!
  //! Array of pointers to streams (potentially) used by this EventTask.
  //!
  vector<Event*> eventStreams;

  //!
  //! number of EventFilter objects used by this class.. This is a convenience variable used to avoid calling size() repeatedly
  //!
  int nEventFilters;

  //!
  //! number of EventFilter objects used by this class.. This is a convenience variable used to avoid calling size() repeatedly
  //!
  int nParticleFilters;

  //!
  //! Array of pointers to event filters (class EventFilter) used by this EventTask instance.
  //!
  vector<EventFilter*>     eventFilters;
  //!
  //! Array of pointers to particle filters (class ParticleFilter)  used by this EventTask.
  //!
  vector<ParticleFilter*>  particleFilters;


  //!
  //! Array of filter particles (ParticleDigits).
  //!
  //vector< vector<ParticleDigit*> > filteredParticles;


  //!
  //!Number of events accepted by event filters used by this EventTask since last reset (partial save)
  //!
  vector<long> nEventsAccepted;

  //!
  //!Total number of events accepted by event filters used by this EventTask since last reset (partial save)
  //!
  vector<long> nEventsAcceptedTotal;

  //!
  //!Number of particles accepted in the last event for each of the event filters and particle filters used by this EventTask..
  //!
  vector<long> nParticlesAcceptedEvent;

  //!
  //!Number of particles accepted per each of  event filters and particle filters used by this EventTask since last reset
  //!
  vector<long> nParticlesAccepted;

  //!
  //!Number of particles accepted per each of  event filters and particle filters used by this EventTask
  //!
  vector<long> nParticlesAcceptedTotal;

public:

  //!
  //! Default constructor. It allocates resources but DOES NOT initialize the EventTask. EventTask initialization can be performed by a call to the initializationEventTask() and/or
  //!  initialization() methods.
  //!
  EventTask();

  //!
  //! Long constructor. It allocates resources but DOES NOT initialize the EventTask. EventTask initialization can be performed by a call to the initializationEventTask() and/or
  //!  initialization() methods.
  //!
  EventTask(const String & _name,
            const Configuration & _configuration);

  //!
  //! Longer constructor. It allocates resources but DOES NOT initialize the EventTask. EventTask initialization can be performed by a call to the initializationEventTask() and/or
  //!  initialization() methods.
  //!
  EventTask(const String & _name,
            const Configuration & _configuration,
            vector<EventFilter*> &   _eventFilters,
            vector<ParticleFilter*>& _particleFilters);
  //!
  //! dtor
  virtual ~EventTask() {}
  
  //!
  //! Initialize the configuration parameter of the EventTask to their default value;
  //!
  virtual void setDefaultConfiguration();

  virtual void configure();
  //!
  //! Initialize this EventTask instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void initialize();

  virtual void initializeParticleFactory();
  virtual void initializeParticleDbLink();

//  void importParticleDbCAP() {}
//  void importParticleDbNative(){}
//  void initializeParticleDb();
//  void initializeParticleDbCAP(){}
//  void initializeParticleDbNative(){}
//  void convertParticleDbCAPToNative(){}
//  void convertParticleDbNativeToCAP(){}

  virtual void importCalibrations();
  virtual void createCalibrations();
  virtual void exportCalibrations();

  virtual void finalizeEventStreams();
  virtual void finalizeEventGenerator();
  virtual void finalizeEventReader();
  virtual void finalizeEventWriter();


  //! Finalize this Task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void finalize();

  virtual void execute();

  virtual void initializeEventGenerator();
  virtual void initializeEventReader();
  virtual void initializeEventWriter();
  virtual void initializeEventStreams();
//  virtual void initializeEvent();
//  virtual void initializeEventCAP();
//  virtual void initializeEventNative();
  virtual void importEvent();
  virtual void importEventCAP();
  virtual void importEventNative();
  virtual void convertEventCAPToNative();
  virtual void convertEventNativeToCAP();
  virtual void partial(const String & outputPathBase);
  virtual void exportEvent();
  virtual void exportEventCAP();
  virtual void exportEventNative();
  virtual void resetEvent();
  virtual void resetEventCAP();
  virtual void resetEventNative();
  virtual void clearEvent();
  virtual void clearEventCAP();
  virtual void clearEventNative();
  virtual void createEvent();
  virtual void analyzeEvent();

  //!
  //! Reset this Task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void reset();

  //!
  //! Clear this Task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void clear();

  virtual void initializeNParticlesAccepted();
  virtual void incrementNParticlesAccepted(int iEventFilter=0, int iParticleFilter=0);
  virtual void resetNParticlesAcceptedEvent();
  virtual void resetNParticlesAccepted();
  virtual void clearNParticlesAccepted();
  virtual int getNParticlesAccepted(int iEventFilter=0, int iParticleFilter=0)  const;
  virtual int getNParticlesAcceptedTotal(int iEventFilter=0, int iParticleFilter=0) const;
  //!
  //! Scale (all) the histograms held in all the histogram groups owned by this Task instance. This operation is executed automatically within the finalize() method call if
  //! and only if the HistogramsScale parameter of the Configuration instance controlling this Task is set to "true".
  //!
  virtual void scaleHistograms() {} 

  inline ParticleDb * getParticleDb() const
  {
  return particleDb;
  }

  inline ParticleDb * getParticleDb()
  {
  return particleDb;
  }

  //!
  //! Returns a pointer to the event stream at the given index. No sanity check is performed in order to keep this call as efficient as possible.
  //! @param index index of the event stream to produce on output.
  //! @return pointer to the selected event stream.
  //!
  inline Event * getEventStream(unsigned int index)
  {
  return eventStreams[index];
  }


  //!
  //! Add the given event as a stream used by this Task instance.
  //! @param event stream to be added to this Task.
  //!
  void addEventStream(Event * event);

  //!
  //! Returns the number of event streams used  by this Task instance.
  //! @return number of streams associated with this Task instance.
  //!
  inline unsigned int getNEventStreams() const
  {
  return eventStreams.size();
  }

  inline void initializeNEventsAccepted()
  {
  nEventsAccepted.assign(nEventFilters,0);
  nEventsAcceptedTotal.assign(nEventFilters,0);
  }

  inline void incrementNEventsAccepted(int iEventFilter=0)
  {
  nEventsAccepted[iEventFilter]++;
  nEventsAcceptedTotal[iEventFilter]++;
  }

  inline void resetNEventsAccepted()
  {
  nEventsAccepted.assign(nEventFilters,0);
  }

  inline void clearNEventsAccepted()
  {
  nEventsAccepted.assign(nEventFilters,0);
  nEventsAcceptedTotal.assign(nEventFilters,0);
  }

  inline int getAcceptedEventCount(int iEventFilter=0) const
  {
  if (iEventFilter<0 || iEventFilter>=nEventFilters)
    return -1;
  else
    return nEventsAccepted[iEventFilter];
  }

  inline int getTotalAcceptedEventCount(int iEventFilter=0) const
  {
  if (iEventFilter<0 || iEventFilter>=nEventFilters)
    return -1;
  else
    return nEventsAcceptedTotal[iEventFilter];
  }

  void writeNEventsAccepted(TFile & outputFile);
  void loadNEventsAccepted(TFile & inputFile);

  void printEventStatistics() const;

  //!
  //! Clear the event filters used by this Task instance.
  //!
  void clearEventFilters();

  //!
  //! Set the event filters used by this Task instance.
  //!
  void setEventFilters(vector<EventFilter*> _eventFilters);

  //!
  //! Add an event filter to the event filters used by this Task instance.
  //!
  inline void addEventFilter(EventFilter* _eventFilter);

  //!
  //! Return event filters used by this Task instance.
  //!
  inline vector<EventFilter*> & getEventFilters();

  //!
  //! Return event filters used by this Task instance.
  //!
  inline const vector<EventFilter*> & getEventFilters() const;

  //!
  //! Clear the particle filters used by this Task instance.
  //!
  void clearParticleFilters();

  //!
  //! Set particle filters  used by this Task instance.
  //!
  inline void setParticleFilters(vector<ParticleFilter*> _particleFilters);

  //!
  //! Add a particle filter to the particle filters used by this Task instance.
  //!
  inline void addParticleFilter(ParticleFilter* _particleFilter);

  //!
  //! Return particle filters used by this Task instance.
  //!
  inline vector<ParticleFilter*> & getParticleFilters();

  //!
  //! Return particle filters used by this Task instance.
  //!
  inline const vector<ParticleFilter*> & getParticleFilters() const;

  ClassDef(EventTask,0)
};

//ClassImp(EventTask);

} // namespace CAP

#endif /* CAP__EventTask */
