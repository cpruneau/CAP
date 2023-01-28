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
#include "EventFilterAliceV0.hpp"
using CAP::EventFilterAliceV0;

ClassImp(EventFilterAliceV0);

EventFilterAliceV0::EventFilterAliceV0(double nMinV0,
                                       double nMaxV0,
                                       double nMinTPC,
                                       double nMaxTPC)
:
EventFilter(ParticleFilter0,nMinV0,nMaxV0,"AliceV0"),
nMinimumTPC(nMinTPC),
nMaximumTPC(nMaxTPC)
{
  // no ops
}


EventFilterAliceV0::~EventFilterAliceV0()
{
  // no ops
}

bool EventFilterAliceV0::accept(Event & event)
{
  cout << "EventFilterAliceV0::accept(Event & event) called" << endl;

  EventProperties * ptr = event.getEventProperties();
  if (ptr)
    {
    cout << "EventFilterAliceV0::accept(Event & event) ptr appears OK" << endl;
    }
  else
    {
    cout << "EventFilterAliceV0::accept(Event & event) ptr is NOT OK" << endl;
    return false;
    }

  EventProperties & ep = *ptr;
  double nV0  = ep.nFiltered[0];
  double nTPC = 1; // ep.nFiltered[1];
  cout << "EventFilterAliceV0::accept(Event & event) all GOOD!!!!" << endl;

  return (nV0 >= minimum) && (nV0 < maximum) && (nTPC>=nMinimumTPC) && (nTPC<nMaximumTPC);
}

String EventFilterAliceV0::getName()
{
  String name = Form("Alice_V0MGeq%gLt%g_TPCGeq%gLt%g",minimum,maximum,nMinimumTPC,nMaximumTPC);
  return name;
}



String EventFilterAliceV0::getTitle()
{
  String name = Form("Alice: %g#le V0<%g; %g#le TPC<%g",minimum,maximum,nMinimumTPC,nMaximumTPC);
  return name;
}


String EventFilterAliceV0::getLongName()
{
  return getName();
}

String EventFilterAliceV0::getLongTitle()
{
  return getTitle();
}


vector<EventFilter*> EventFilterAliceV0::createEventFilterAliceV0Set(vector<double> limits)
{
  vector<EventFilter*> filters;
  unsigned int n = limits.size() - 1;
  for (unsigned int k=0; k<n; k++)
    {
    filters.push_back( new EventFilterAliceV0(limits[k], limits[k+1],1.0,1000.0) );
    }
  return filters;
}
