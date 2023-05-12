#include <TString.h>
class XmlDocument;
using String = TString;

#define  ReportStart(function)   ( getMessageLogger()->reportStart((function) )
#define  ReportEnd(function)     ( getMessageLogger()->reportEnd((function) )
#define  ReportTrace(function)   ( getMessageLogger()->reportTrace((function) )
#define  ReportDebug(function)   ( getMessageLogger()->reportDebug((function) )
#define  ReportInfo(function)    ( getMessageLogger()->reportInfo((function) )
#define  ReportWarning(function) ( getMessageLogger()->reportWarning((function) )
#define  ReportError(function)   ( getMessageLogger()->reportError((function) )
#define  ReportFatal(function)   ( getMessageLogger()->reportFatal((function) )


class MessageLogger
{
public:

  enum Severity   { Unknown, Trace, Debug, Info, Warning, Error, Fatal};

  Severity  reportLevel;      //!< Minimal error level reported by object instances inheriting from this class.
  Severity  holdReportLevel;  //!< Internal variable used internally to set/reset the nominal report level selected by the user.
  String   className;          //!< name of the class of this object instance
  String   instanceName;       //!< name given to this object instance
  String   fctName;            //!< name of the function initiating messages.

protected:

  static String unknownSeverityName;
  static String traceSeverityName;
  static String startSeverityString;
  static String endSeverityString;
  static String debugSeverityName;
  static String infoSeverityName;
  static String warningSeverityName;
  static String errorSeverityName;
  static String fatalSeverityName;

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
  inline void appendClassName(const String &  _className)
  {
  className += "/";
  className += _className;
  }

  //!
  //!Set the class name  of this object.
  //!
  inline void setClassName(const String &  _className)
  {
  className = _className;
  }

  //!
  //!Set the  name  of this object .
  //!
  inline void setInstanceName(const String &  _instanceName) { instanceName = _instanceName;  }

  //!
  //!Set the  name  of the function to report as message emitter.
  //!
  inline void setFunctionName(const String &  _fctName) { fctName = _fctName;   }

  //!
  //!Get the class name  of this object.
  //!
  inline const String &  getClassName()     const    { return className; }

  //!
  //!Get the  name  of this object.
  //!
  inline const String &  getInstanceName()  const    { return instanceName; }

  //!
  //!Get the  name  of the function reporting messages.
  //!
  inline const String &  getFunctionName() const    { return fctName; }

  //!
  //!Issue a debug message if the severity level is not below Debug.
  //!
  inline bool reportDebug(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Debug,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportTrace(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a function start message if the severity level is not below Trace.
  //!
  bool reportStart(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportEnd(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Info.
  //!
  bool reportInfo(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Info,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Warning.
  //!
  bool reportWarning(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Warning,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Error.
  //!
  bool reportError(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Error,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Fatal.
  //!
  bool reportFatal(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Fatal,className,instanceName,fctName,output);
  }

  bool report(Severity severity, const String &  className, const String &  instanceName, const String &  fctName, std::ostream & output) const;

 //ClassDef(MessageLogger,0)
};

String MessageLogger::unknownSeverityName   = "<Unknown>";
String MessageLogger::traceSeverityName     = "<Trace>";;
String MessageLogger::startSeverityString   = "<Start>";
String MessageLogger::endSeverityString     = "<End>";
String MessageLogger::debugSeverityName     = "<Debug>";
String MessageLogger::infoSeverityName      = "<Info>";
String MessageLogger::warningSeverityName   = "<Warning>";
String MessageLogger::errorSeverityName     = "<Error>";
String MessageLogger::fatalSeverityName     = "<Fatal>";

bool MessageLogger::report(Severity severity, const String &  className, const String &  instanceName, const String &  fctName, std::ostream & output) const
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
    std::cout << "  " << className << "[" << instanceName << "]::" << fctName << ": ";
    return true;
    }
  else
    return false;
}




class XmlDocument
{
public:
  friend class XmlParser;

  XmlDocument(const TString & _xmlFileName);
  ~XmlDocument();
  XmlDocument* getXmlTag(const char* aTagName, const char* aAttName, const char* aAttVal); // throw(int);
  TString   getXmlAttribute(const char* aAttName); // throw(int);
  TString   getXmlContent();
  //VectorField* getXmlVector3D();
  TString   getXmlTagDescription();
  void      getXmlVector3D();
  bool XmlAttributeMatch(const char* aAttName, const char* aAttVal);


  int       XmlAttributeMatch(const char* aAttName, const char* aAttVal);

  TString   fileName;
  XmlTag*   topTag;
  XmlTag*   currentTag;
};




void  XmlDocument::getXmlVector3D()
{
  ifstream  ifs;
  //VectorField* tVec;
  vector<double> tVec;
  TString   vName;
  TString   vAxis[3];
  double    vMin[3];
  double    vMax[3];
  int       vPts[3];

  //Factory<VectorField> * factory = VectorField->getFactory();

  if((currentTag) && (currentTag->name == "VECTOR3D") )
    { // make sure currentTag points to a VECTOR3D type Tag
      vName = getXmlAttribute("name"); currentTag = currentTag->child; // goto first AXIS
      // extract AXIS Tags
      for(int i=0; i<3; i++) {
        vAxis[i] = getXmlAttribute("name"); currentTag = currentTag->child; // goto <DETAIL name="min">
        vMin[i]  = getXmlContent().Atof(); currentTag = currentTag->next;  // goto <DETAIL name="max">
        vMax[i]  = getXmlContent().Atof(); currentTag = currentTag->next;  // goto <DETAIL name="pts">
        vPts[i]  = getXmlContent().Atoi(); currentTag = currentTag->father->next; // goto next AXIS or DATA
      }
      // extract ARRAY Tag -> VectorField object
      // CAP 3/18/2023
      // instead of instantiating directly, invoke a factory and set the object
      // the factory then handles the memory management.
      //  tVec = new VectorField(vName.Data(),vMin[0],vMax[0],vPts[0],vMin[1],vMax[1],vPts[1],vMin[2],vMax[2],vPts[2]);
      //tVec = factory->getNextObject();
      // tVec->setValue(vName.Data(),vMin[0],vMax[0],vPts[0],vMin[1],vMax[1],vPts[1],vMin[2],vMax[2],vPts[2]);
      cout << " VectorField" << endl;
      cout << " Name............: " << vName << endl;
      cout << " vMin[0].........: " << vMin[0] << endl;
      cout << " vMax[0].........: " << vMax[0] << endl;
      cout << " vPts[0]........: " << vPts[0] << endl;
      cout << " vMin[1]........: " << vMin[1] << endl;
      cout << " vMax[1]........: " << vMax[1] << endl;
      cout << " vPts[1]........: " << vPts[1] << endl;
      cout << " vMin[2]........: " << vMin[2] << endl;
      cout << " vMax[2]........: " << vMax[2] << endl;
      cout << " vPts[2]........: " << vPts[2] << endl;

      double aValue;

      ifs.open(fileName);
      if((ifs) && ifs.is_open()) {
        ifs.seekg(currentTag->begin);
        ifs.precision(16);
        for(int i=0; i<vPts[0]; i++)
          for(int j=0; j<vPts[1]; j++)
            for(int k=0; k<vPts[2]; k++)
//              ifs >> (*tVec)(i,j,k);
              ifs >> aValue;
        cout << " v..............: " << aValue  << endl;
        ifs.close();
//        if (reportTrace(__FUNCTION__))
//          {
//          cout
//          << "Extracted vector "
//          <<tVec->getName()
//          <<"("
//          <<vAxis[0]<<"["<< tVec->getXMin() << "," << tVec->getXMax() << "], "
//          <<vAxis[1]<<"["<< tVec->getYMin() << "," << tVec->getYMax() << "], "
//          <<vAxis[2]<<"["<< tVec->getZMin() << "," << tVec->getZMax() << "])"
//          << endl;
//          }

        //cout <<  "<XmlDocument::getXmlVector3D>\tExtracted vector "<<tVec->getName()<<"("<<vAxis[0]<<"["<<tVec->getXMin()<<","<<tVec->getXMax()<<"], "<<vAxis[1]<<"["<<tVec->getYMin()<<","<<tVec->getYMax()<<"], "<<vAxis[2]<<"["<<tVec->getZMin()<<","<<tVec->getZMax()<<"])");
        //return tVec;
      }
      else
        {
//        if (reportError(__FUNCTION__))
//          {
          cout << "File " << fileName<< " not opened." << endl;
//          }
        //cout << "<XmlDocument::getXmlVector3D>\tFile " << fileName<< " not opened.");
        exit(1); //_ERROR_GENERAL_FILE_NOT_FOUND_);
        }
    }
  else
    {
//    if (reportError(__FUNCTION__))
//      {
      cout << "Unable to extract VectorField. Use getXmlTag(\"VECTOR3D\",\"name\",\"your_vector_name\")."<< endl;
//      }
    //cout << "<XmlDocument::getXmlVector3D>\tUnable to extract vector. Use getXmlTag(\"VECTOR3D\",\"name\",\"your_vector_name\").");
    exit(1); //_ERROR_LIBRARY_VECTOR3D_NOT_FOUND_);
    }
}







int Test()
{
//  XmlParser parser;
//  XmlDocument * myLib = new XmlDocument("file.xml");
//  parser.read("file.xml",myLib);
//
//  //myLib->getXmlTag(const char* aTagName, const char* aAttName, const char* aAttVal)
//  myLib->getXmlVector3D();


  return 0;
}
