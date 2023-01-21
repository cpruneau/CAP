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

TString MessageLogger::unknownSeverityName   = "<Unknown>";
TString MessageLogger::traceSeverityName     = "<Trace>";;
TString MessageLogger::startSeverityString   = "<Start>";
TString MessageLogger::endSeverityString     = "<End>";
TString MessageLogger::debugSeverityName     = "<Debug>";
TString MessageLogger::infoSeverityName      = "<Info>";
TString MessageLogger::warningSeverityName   = "<Warning>";
TString MessageLogger::errorSeverityName     = "<Error>";
TString MessageLogger::fatalSeverityName     = "<Fatal>";

bool MessageLogger::report(Severity severity, const TString &  className, const TString &  instanceName, const TString &  fctName, ostream & output) const
{
  if (reportLevel<=severity)
    {
    switch (severity)
      {
        default:
        case Unknown:  output << traceSeverityName; break;
        case Trace:    output << traceSeverityName; break;
        case Debug:    output << debugSeverityName; break;
        case Info:     output << infoSeverityName; break;
        case Warning:  output << warningSeverityName; break;
        case Error:    output << errorSeverityName; break;
        case Fatal:    output << fatalSeverityName; break;
      }
    cout << "  " << className << "[" << instanceName << "]::" << fctName << ": ";
    return true;
    }
  else
    return false;
}
