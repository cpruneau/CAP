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
#ifndef CAP__EventFilterAliceV0
#define CAP__EventFilterAliceV0
#include "EventFilter.hpp"

namespace CAP
{


//!
//! Class encapsulating an event filter replicating the conditions used in ALICE to trigger on events
//!  The filtering is based on the number of particles counters and stored in the structure
//! EventProperties.nFiltered[0] and EventProperties.nFiltered[0]. The former represents the
//! multiplicity measured by the ALICE V0M detector while the latter is the multiplicity measured in
//! the TPC accpetance (nominally 0.2<pT,  |eta|<0.8). The counters are set by running the global event
//! analyser after generating or loading events.
//!
class EventFilterAliceV0 : public EventFilter
{
public:


   EventFilterAliceV0(double nMinV0,
                      double nMaxV0,
                      double nMinTPC,
                      double nMaxTPC);
  virtual ~EventFilterAliceV0();
  virtual bool accept(Event & event);

//  String getName();
//  const String  & getUserName() const { return userName; }
//  String getTitle();
//  String getLongName();
//  String getLongTitle();

  //static vector<EventFilter*> createEventFilterAliceV0Set(vector<double> limits);


protected:

//  double  nMinimumTPC;
//  double  nMaximumTPC;

  ClassDef(EventFilterAliceV0,0)
};

} // namespace CAP

#endif /* CAP__EventFilterAliceV0 */
