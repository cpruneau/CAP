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
using CAP::Filter;
using CAP::Event;
using CAP::EventFilter;

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
//! accept/reject the given Event based on filter parameter
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
        // filtered baryoness
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


vector<EventFilter*> EventFilter::createOpenEventFilter()
{
  vector<EventFilter*> filters;
  EventFilter * filter = new EventFilter();
  filter->setName("All");
  filter->setLongName("All");
  filter->setTitle("All");
  filter->setLongTitle("All");
  filters.push_back(filter);
  return filters;
}

vector<EventFilter*> EventFilter::createAliceMBEventFilter()
{
  vector<EventFilter*> filters;
  EventFilter* filter  = new EventFilter();
  filter->setName("AliceMB");
  filter->setLongName("AliceMB");
  filter->setTitle("AliceMB");
  filter->setLongTitle("AliceMB");
  filter->addCondition(1, 0, 1.0, 1.0E10); // v0 multiplicity
  filter->addCondition(1, 1, 1.0, 1.0E10); // TPC multiplicity
  filters.push_back(filter);
  return filters;
}

vector<EventFilter*> EventFilter::createImpactParameterFilters(vector<double> & bounds)
{
  vector<EventFilter*> filters;
  EventFilter* filter;
  int n = bounds.size();
  for (int k=0; k<n-1; k++)
    {
    double low  = bounds[k];
    double high = bounds[k+1];
    String name = "b";
    name += int(1000*low);
    name +="To";
    name += int(1000*high);
    String title;
    title = low;
    title += "#LT b <";
    title += high;
    filter = new EventFilter();
    filter->setName(name);
    filter->setLongName(name);
    filter->setTitle(title);
    filter->setLongTitle(title);
    filter->addCondition(0, 0, low, high); // meant to cut on b
    }
  filters.push_back(filter);
  return filters;
}

vector<EventFilter*> EventFilter::createV0MultiplicityFilters(vector<double> & bounds)
{
  vector<EventFilter*> filters;
  EventFilter* filter;
  int n = bounds.size();
  for (int k=0; k<n-1; k++)
    {
    double low  = bounds[k];
    double high = bounds[k+1];
    String name = "b";
    name += int(1000*low);
    name +="To";
    name += int(1000*high);
    String title;
    title = low;
    title += "#LT b <";
    title += high;
    filter = new EventFilter();
    filter->setName(name);
    filter->setLongName(name);
    filter->setTitle(title);
    filter->setLongTitle(title);
    filter->addCondition(1, 0, low, high); // meant to cut on V0M
    }
  filters.push_back(filter);
  return filters;
}


vector<EventFilter*> EventFilter::createTpcMultiplicityFilters(vector<double> & bounds)
{
  vector<EventFilter*> filters;
  EventFilter* filter;
  int n = bounds.size();
  for (int k=0; k<n-1; k++)
    {
    double low  = bounds[k];
    double high = bounds[k+1];
    String name = "b";
    name += int(1000*low);
    name +="To";
    name += int(1000*high);
    String title;
    title = low;
    title += "#LT b <";
    title += high;
    filter = new EventFilter();
    filter->setName(name);
    filter->setLongName(name);
    filter->setTitle(title);
    filter->setLongTitle(title);
    filter->addCondition(1, 1, low, high); // meant to cut on V0M
    }
  filters.push_back(filter);
  return filters;
}
