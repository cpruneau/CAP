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
#ifndef CAP__EventFilter
#define CAP__EventFilter
#include "Event.hpp"
#include "Filter.hpp"

class EventFilter : public Filter<Event>
{
public:

  EventFilter();
  virtual ~EventFilter();
  virtual bool accept(const Event & event);

  ClassDef(EventFilter,0)
};

#endif /* CAP__EventFilter */
