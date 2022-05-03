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
#include "EventFilter.hpp"

ClassImp(Filter<Event>);

ClassImp(EventFilter);

//!
//! CTOR
//!
EventFilter::EventFilter()
:
Filter<Event>()
{
  // no ops
}


//!
//! DTOR
//!
EventFilter::~EventFilter()
{
  // no ops
}

//!
//! accept/reject the given particle based on filter parameter
//!
bool EventFilter::accept(const Event & event)
{
  unsigned int nComponents = getNConditions();
  if (nComponents<1) return true;
  EventProperties * eventProperties = event.getEventProperties();
  if (!eventProperties)
    {
    cout << " Event does NOT have properties" << endl;
    return false;
    }
  double value;
  for (unsigned int k = 0; k<getNConditions(); k++)
    {
    Condition & condition = *(conditions[k]);
    unsigned index   = condition.filterSubtype;
    switch (condition.filterType)
      {
        case 0:
        // model parameter
        if (index>=eventProperties->modelParameters.size())
          {
          cout << "<E> EventFilter::accept(Event & event)  index>=modelParameters.size()" << endl;
          return false;
          }
        value = eventProperties->modelParameters[index]; break;
        case 1:
        // filtered n
        if (index>=eventProperties->nFiltered.size())
          {
          cout << "<E> EventFilter::accept(Event & event)  index>=eventProperties->nFiltered.size()" << endl;
          return false;
          }
        value = eventProperties->nFiltered[index]; break;
        case 2:
        // filtered energy
        if (index>=eventProperties->eFiltered.size())
          {
          cout << "<E> EventFilter::accept(Event & event)  index>=eventProperties->eFiltered.size()" << endl;
          return false;
          }
        value = eventProperties->eFiltered[index]; break;
        case 3:
        // filtered charge
        if (index>=eventProperties->qFiltered.size())
          {
          cout << "<E> EventFilter::accept(Event & event)  index>=eventProperties->qFiltered.size()" << endl;
          return false;
          }
        value = eventProperties->qFiltered[index]; break;
        case 4:
        // filtered strangeness
        if (index>=eventProperties->sFiltered.size())
          {
          cout << "<E> EventFilter::accept(Event & event)  index>=eventProperties->sFiltered.size()" << endl;
          return false;
          }
        value = eventProperties->sFiltered[index]; break;
        case 5:
        // filtered strangeness
        if (index>=eventProperties->bFiltered.size())
          {
          cout << "<E> EventFilter::accept(Event & event)  index>=eventProperties->bFiltered.size()" << endl;
          return false;
          }
        value = eventProperties->bFiltered[index]; break;
      }
    if (!condition.accept(value))  return false;
    }
  return true;
}

