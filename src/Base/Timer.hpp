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
#ifndef CAP__Timer
#define CAP__Timer
#include <wchar.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include "TObject.h"
using namespace std;
using namespace std::chrono;

namespace CAP
{
class Timer
{
public:

  Timer();
  virtual ~Timer();
  void start();
  void stop();
  void print(ostream & os);

  high_resolution_clock::time_point startTime;
  high_resolution_clock::time_point stopTime;
  high_resolution_clock::time_point oldStop;
  duration<double> totalDuration;
  duration<double> intervalDuration;
  int    days;
  int    hours;
  int    minutes;
  double seconds;
  int    deltaDays;
  int    deltaHours;
  int    deltaMinutes;
  double deltaSeconds;

  ClassDef(Timer,0)
};

}



#endif /* CAP__Timer */
