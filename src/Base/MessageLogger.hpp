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
#ifndef CAP__MessageLogger
#define CAP__MessageLogger
#include "TClass.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
using namespace std;
using std::cout;
using std::endl;

#define  ReportStart(function)   ( getMessageLogger()->reportStart((function) )
#define  ReportEnd(function)     ( getMessageLogger()->reportEnd((function) )
#define  ReportTrace(function)   ( getMessageLogger()->reportTrace((function) )
#define  ReportDebug(function)   ( getMessageLogger()->reportDebug((function) )
#define  ReportInfo(function)    ( getMessageLogger()->reportInfo((function) )
#define  ReportWarning(function) ( getMessageLogger()->reportWarning((function) )
#define  ReportError(function)   ( getMessageLogger()->reportError((function) )
#define  ReportFatal(function)   ( getMessageLogger()->reportFatal((function) )

//!
//! Message logger class used  by several  classes and subclasses of this package.
//!
class MessageLogger
{
public:

  enum Severity   { Unknown, Trace, Debug, Info, Warning, Error, Fatal};

  Severity  reportLevel;      //!< Minimal error level reported by object instances inheriting from this class.
  Severity  holdReportLevel;  //!< Internal variable used internally to set/reset the nominal report level selected by the user.
  TString   className;          //!< name of the class of this object instance
  TString   instanceName;       //!< name given to this object instance
  TString   fctName;            //!< name of the function initiating messages.

protected:

  static TString unknownSeverityName;
  static TString traceSeverityName;
  static TString startSeverityString;
  static TString endSeverityString;
  static TString debugSeverityName;
  static TString infoSeverityName;
  static TString warningSeverityName;
  static TString errorSeverityName;
  static TString fatalSeverityName;

public:

  //! CTOR
  MessageLogger(Severity selectedLevel=Info) :
  reportLevel( selectedLevel ),
  holdReportLevel(selectedLevel),
  className("MessageLogger"),
  instanceName("MessageLogger"),
  fctName("CTOR")
  {   }


  //! DTOR
  virtual ~MessageLogger()
  { }

  //!
  //! Set the desired reported level used by instances (objects) of this class or subclasses.
  //!
  void setSeverityLevel(Severity requiredLevel)
  {
    reportLevel = requiredLevel;
  }

  //!
  //! Get the desired reported level used by instances (objects) of this class or subclasses.
  //!
  Severity getSeverityLevel() const
  {
    return reportLevel;
  }

  //!
  //! Set the hold level .
  //!
  void holdSeverityLevel(Severity requiredLevel)
  {
  holdReportLevel = reportLevel;
  reportLevel     = requiredLevel;
  }

  //!
  //!Restore the nominal report level requested by the user.
  //!
  void restoreSeverityLevel()
  {
  reportLevel = holdReportLevel;
  }

  //!
  //!Append the given string to the class name sequence of this object instance.
  //!
  inline void appendClassName(const TString &  _className)
  {
  className += "/";
  className += _className;
  }

  //!
  //!Set the class name  of this object.
  //!
  inline void setClassName(const TString &  _className)
  {
  className = _className;
  }

  //!
  //!Set the  name  of this object .
  //!
  inline void setInstanceName(const TString &  _instanceName) { instanceName = _instanceName;  }

  //!
  //!Set the  name  of the function to report as message emitter.
  //!
  inline void setFunctionName(const TString &  _fctName) { fctName = _fctName;   }

  //!
  //!Get the class name  of this object.
  //!
  inline const TString &  getClassName()     const    { return className; }

  //!
  //!Get the  name  of this object.
  //!
  inline const TString &  getInstanceName()  const    { return instanceName; }

  //!
  //!Get the  name  of the function reporting messages.
  //!
  inline const TString &  getFunctionName() const    { return fctName; }

  //!
  //!Issue a debug message if the severity level is not below Debug.
  //!
  inline bool reportDebug(const TString &  fctName, ostream & output=cout) const
  {
    return report(Debug,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportTrace(const TString &  fctName, ostream & output=cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a function start message if the severity level is not below Trace.
  //!
  bool reportStart(const TString &  fctName, ostream & output=cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportEnd(const TString &  fctName, ostream & output=cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Info.
  //!
  bool reportInfo(const TString &  fctName, ostream & output=cout) const
  {
  return report(Info,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Warning.
  //!
  bool reportWarning(const TString &  fctName, ostream & output=cout) const
  {
  return report(Warning,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Error.
  //!
  bool reportError(const TString &  fctName, ostream & output=cout) const
  {
  return report(Error,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Fatal.
  //!
  bool reportFatal(const TString &  fctName, ostream & output=cout) const
  {
  return report(Fatal,className,instanceName,fctName,output);
  }

  bool report(Severity severity, const TString &  className, const TString &  instanceName, const TString &  fctName, ostream & output) const;

  ClassDef(MessageLogger,0)
};

#endif /* CAP__MessageLogger */
