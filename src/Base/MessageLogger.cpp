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
#include "MessageLogger.hpp"

ClassImp(MessageLogger);

TString MessageLogger::startString   = "<DS> ";
TString MessageLogger::endString     = "<DE> ";
TString MessageLogger::debugString   = "<D> ";
TString MessageLogger::infoString    = "<I> ";
TString MessageLogger::warningString = "<W> ";
TString MessageLogger::errorString   = "<E> ";
TString MessageLogger::fatalString   = "<F> ";
MessageLogger::TaskStatus MessageLogger::taskStatus = MessageLogger::TaskOk;

bool MessageLogger::reportDebug(const TString & fctName, ostream & output) const
{
 if (reportLevel<=Debug)
   {
   output << debugString  << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportNoOps(const TString & fctName, ostream & output) const
{
 if (reportLevel<=Debug)
   {
   output << debugString  << className << "[" << instanceName << "]::" << fctName << "  No ops." << endl;
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportStart(const TString & fctName, ostream & output) const
{
 if (reportLevel<=Debug)
   {
   output << startString  << className << "[" << instanceName << "]::" << fctName << "  Started." << endl;
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportEnd(const TString & fctName, ostream & output) const
{
 if (reportLevel<=Debug)
   {
   output << endString  << className << "[" << instanceName << "]::" << fctName << "  Completed." << endl;
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportInfo(const TString & fctName, ostream & output) const
{
 if (reportLevel<=Info)
   {
   output << infoString << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportWarning(const TString & fctName, ostream & output) const
{
  postTaskWarning();
 if (reportLevel<=Warning)
   {
   output << warningString << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportError(const TString & fctName, ostream & output) const
{
  postTaskError();
 if (reportLevel<=Error)
   {
   output << errorString << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportFatal(const TString & fctName, ostream & output) const
{
  postTaskFatal();
 if (reportLevel<=Fatal)
   {
   output << fatalString << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportDebug(const TString & className, const TString & instanceName, const TString & fctName, ostream & output) const
{
 if (reportLevel<=Debug)
   {
   output << debugString  << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportNoOps(const TString & className, const TString & instanceName, const TString & fctName, ostream & output) const
{
 if (reportLevel<=Debug)
   {
   output << debugString  << className << "[" << instanceName << "]::" << fctName << "  No ops." << endl;
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportStart(const TString & className, const TString & instanceName, const TString & fctName, ostream & output) const
{
 if (reportLevel<=Debug)
   {
   output << startString  << className << "[" << instanceName << "]::" << fctName << "  Started." << endl;
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportEnd(const TString & className, const TString & instanceName, const TString & fctName, ostream & output) const
{
 if (reportLevel<=Debug)
   {
   output << endString  << className << "[" << instanceName << "]::" << fctName << "  Completed." << endl;
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportInfo(const TString & className, const TString & instanceName, const TString & fctName, ostream & output) const
{
 if (reportLevel<=Info)
   {
   output << infoString << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportWarning(const TString & className, const TString & instanceName, const TString & fctName, ostream & output) const
{
  postTaskWarning();
 if (reportLevel<=Warning)
   {
   output << warningString << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportError(const TString & className, const TString & instanceName, const TString & fctName, ostream & output) const
{
  postTaskError();
 if (reportLevel<=Error)
   {
   output << errorString << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportFatal(const TString & className, const TString & instanceName, const TString & fctName, ostream & output) const
{
  postTaskFatal();
 if (reportLevel<=Fatal)
   {
   output << fatalString << className << "[" << instanceName << "]::" << fctName << "  ";
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportStart  (ostream & output) const
{
  if (reportLevel<=Debug)
    {
    output << startString << className << "[" << instanceName << "]::" << fctName << "  " << endl;
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportEnd    (ostream & output) const
{
  if (reportLevel<=Debug)
    {
    output << endString << className << "[" << instanceName << "]::" << fctName << "  " << endl;;
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportDebug(ostream & output) const
{
  if (reportLevel<=Debug)
    {
    output << debugString << className << "[" << instanceName << "]::" << fctName << "  ";
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportInfo(ostream & output) const
{
 
  if (reportLevel<=Info)
    {
    output << infoString << className << "[" << instanceName << "]::" << fctName << "  ";
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportWarning(ostream & output) const
{
  postTaskWarning();
  if (reportLevel<=Warning)
    {
    output << warningString << className << "[" << instanceName << "]::" << fctName << "  ";
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportError(ostream & output) const
{
  postTaskError();
  if (reportLevel<=Error)
    {
    output << errorString << className << "[" << instanceName << "]::" << fctName << "  ";
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportFatal(ostream & output) const
{
  postTaskFatal();
  if (reportLevel<=Fatal)
    {
    output << fatalString << className << "[" << instanceName << "]::" << fctName << "  ";
    return true;
    }
  else
    return false;
}


TString MessageLogger::getTaskStatusName()
{
  TString statusName;
  switch (taskStatus)
    {
      case TaskUnknown: statusName = "Unknown"; break;
      case TaskOk:      statusName = "TaskOk";  break;
      case TaskEof:     statusName = "TaskEof"; break;
      case TaskEod:     statusName = "TaskEod"; break;
      case TaskWarning: statusName = "TaskWarning"; break;
      case TaskError:   statusName = "TaskError";   break;
      case TaskFatal:   statusName = "TaskFatal";   break;
    }
  return statusName;
}

// sanity check functions

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function first argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrFileExist(const TString & caller, const TFile * f) const
{
  bool allGood = true;
  if (!f)
    {
    if (reportError(caller)) cout << endl  << "Ptr to file is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(caller))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(caller))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(caller))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(caller))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(caller))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(caller))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3,
                                   const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(caller))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(caller))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(caller))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(caller))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3,
                                   const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7,
                                   const TH1 * h8, const TH1 * h9) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(caller))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(caller))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(caller))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(caller))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h9)
    {
    if (reportError(caller))  cout << endl  << "Function 9th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3,
                                   const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7,
                                   const TH1 * h8, const TH1 * h9, const TH1 * h10) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(caller))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(caller))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(caller))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(caller))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h9)
    {
    if (reportError(caller))  cout << endl  << "Function 9th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h10)
    {
    if (reportError(caller))  cout << endl  << "Function 10th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3,
                                   const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7,
                                   const TH1 * h8, const TH1 * h9, const TH1 * h10, const TH1 * h11) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(caller))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(caller))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(caller))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(caller))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h9)
    {
    if (reportError(caller))  cout << endl  << "Function 9th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h10)
    {
    if (reportError(caller))  cout << endl  << "Function 10th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h11)
    {
    if (reportError(caller))  cout << endl  << "Function 11th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3,
                                   const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7,
                                   const TH1 * h8, const TH1 * h9, const TH1 * h10, const TH1 * h11, const TH1 * h12) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(caller))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(caller))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(caller))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(caller))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(caller))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(caller))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(caller))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(caller))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h9)
    {
    if (reportError(caller))  cout << endl  << "Function 9th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h10)
    {
    if (reportError(caller))  cout << endl  << "Function 10th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h11)
    {
    if (reportError(caller))  cout << endl  << "Function 11th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h12)
    {
    if (reportError(caller))  cout << endl  << "Function 12th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}


bool MessageLogger::ptrExist(const TH1 * h1) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function first argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrFileExist(const TFile * f) const
{
  bool allGood = true;
  if (!f)
    {
    if (reportError(__FUNCTION__)) cout << endl  << "Ptr to file is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3,
                             const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3,
                             const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7,
                             const TH1 * h8, const TH1 * h9) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h9)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 9th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3,
                             const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7,
                             const TH1 * h8, const TH1 * h9, const TH1 * h10) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h9)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 9th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h10)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 10th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3,
                             const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7,
                             const TH1 * h8, const TH1 * h9, const TH1 * h10, const TH1 * h11) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h9)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 9th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h10)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 10th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h11)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 11th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

bool MessageLogger::ptrExist(const TH1 * h1, const TH1 * h2, const TH1 * h3,
                             const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7,
                             const TH1 * h8, const TH1 * h9, const TH1 * h10, const TH1 * h11, const TH1 * h12) const
{
  bool allGood = true;
  if (!h1)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 1st argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h2)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 2nd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h3)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 3rd argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h4)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 4th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h5)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 5th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h6)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 6th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h7)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 7th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h8)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 8th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h9)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 9th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h10)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 10th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h11)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 11th argument is a null ptr" << endl;
    allGood = false;
    }
  if (!h12)
    {
    if (reportError(__FUNCTION__))  cout << endl  << "Function 12th argument is a null ptr" << endl;
    allGood = false;
    }
  return allGood;
}

