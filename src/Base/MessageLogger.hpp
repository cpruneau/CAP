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
#include <iostream>
#include "Aliases.hpp"
#include "Exceptions.hpp"

namespace CAP
{

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
  CAP::String   className;          //!< name of the class of this object instance
  CAP::String   instanceName;       //!< name given to this object instance
  CAP::String   fctName;            //!< name of the function initiating messages.

protected:

  static CAP::String unknownSeverityName;
  static CAP::String traceSeverityName;
  static CAP::String startSeverityString;
  static CAP::String endSeverityString;
  static CAP::String debugSeverityName;
  static CAP::String infoSeverityName;
  static CAP::String warningSeverityName;
  static CAP::String errorSeverityName;
  static CAP::String fatalSeverityName;

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
  //! Get the desired reported level used by instances (objects) of this class or subclasses.
  //!
  String  getSeverityName() const
  {
  String s;
  switch (reportLevel)
    {
      default:
      case Unknown:  s = traceSeverityName; break;
      case Trace:    s = traceSeverityName; break;
      case Debug:    s = debugSeverityName; break;
      case Info:     s = infoSeverityName; break;
      case Warning:  s = warningSeverityName; break;
      case Error:    s = errorSeverityName; break;
      case Fatal:    s = fatalSeverityName; break;
    }
  return s;
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
  inline void appendClassName(const CAP::String &  _className)
  {
  className += "/";
  className += _className;
  }

  //!
  //!Set the class name  of this object.
  //!
  inline void setClassName(const CAP::String &  _className)
  {
  className = _className;
  }

  //!
  //!Set the  name  of this object .
  //!
  inline void setInstanceName(const CAP::String &  _instanceName) { instanceName = _instanceName;  }

  //!
  //!Set the  name  of the function to report as message emitter.
  //!
  inline void setFunctionName(const CAP::String &  _fctName) { fctName = _fctName;   }

  //!
  //!Get the class name  of this object.
  //!
  inline const CAP::String &  getClassName()     const    { return className; }

  //!
  //!Get the  name  of this object.
  //!
  inline const CAP::String &  getInstanceName()  const    { return instanceName; }

  //!
  //!Get the  name  of the function reporting messages.
  //!
  inline const CAP::String &  getFunctionName() const    { return fctName; }

  //!
  //!Issue a debug message if the severity level is not below Debug.
  //!
  inline bool reportDebug(const CAP::String &  fctName, std::ostream & output=std::cout) const
  {
    return report(Debug,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportTrace(const CAP::String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a function start message if the severity level is not below Trace.
  //!
  bool reportStart(const CAP::String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportEnd(const CAP::String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Info.
  //!
  bool reportInfo(const CAP::String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Info,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Warning.
  //!
  bool reportWarning(const CAP::String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Warning,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Error.
  //!
  bool reportError(const CAP::String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Error,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Fatal.
  //!
  bool reportFatal(const CAP::String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Fatal,className,instanceName,fctName,output);
  }

  bool report(Severity severity, const CAP::String &  className, const CAP::String &  instanceName, const CAP::String &  fctName, std::ostream & output) const;

  ClassDef(MessageLogger,0)
};

} // namespace CAP

#endif /* CAP__MessageLogger */
