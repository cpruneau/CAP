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
#include "Timer.hpp"

using namespace CAP;



ClassImp(Timer);

Timer::Timer()
:
startTime(),
stopTime(),
totalDuration(),
hours(0),
minutes(0),
seconds(0)
{
  start();
}

Timer::~Timer()
{
  // no ops.
}

void Timer::start()
{
  startTime = high_resolution_clock::now();
  oldStop   = startTime;
}
void Timer::stop()
{
  stopTime = high_resolution_clock::now();
  intervalDuration = chrono::duration_cast<chrono::seconds>(stopTime - oldStop);
  totalDuration    = chrono::duration_cast<chrono::seconds>(stopTime - startTime);
  days     = (int)(totalDuration.count()/(24*3600));
  hours    = (int)(totalDuration.count()/3600);
  minutes  = (int)((totalDuration.count() - 3600 * hours)/60);
  seconds  = totalDuration.count() - 60 * minutes - 3600 * hours;
  deltaDays    = (int)(intervalDuration.count()/(24*3600));
  deltaHours   = (int)(intervalDuration.count()/(3600));;
  deltaMinutes = (int)((intervalDuration.count() - 3600 * deltaHours)/60);
  deltaSeconds = intervalDuration.count() - 60*deltaMinutes - 3600*deltaHours - 24*3600*deltaDays;
  oldStop  = stopTime;
}

void Timer::print(ostream & os)
{
  os << "             Time since start : " << days << " days, "<< hours << " hours, " << minutes << " minutes, " << seconds << " seconds." << endl;
  os << "                Time interval : " << deltaDays << " days, "<< deltaHours << " hours, " << deltaMinutes << " minutes, " << deltaSeconds << " seconds." << endl;
}


