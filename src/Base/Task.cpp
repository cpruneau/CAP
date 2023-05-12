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
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TParameter.h"
#include "Task.hpp"

using namespace CAP;

ClassImp(Task);

Task::Task()
:
ConfigurationManager     (),
IdentifiedObject         (),
//MessageLogger            (Info),
timer                    (),
histogramManager         (),
parent                   (nullptr),
histosCreate             (false),
histosReset              (false),
histosClear              (false),
histosScale              (false),
histosPlot               (false),
histosPrint              (false),
histosForceRewrite       (false),
histosImport             (false),
histosImportPath         (""),
histosImportFile         (""),
histosExport             (false),
histosExportAsRoot       (false),
histosExportAsText       (false),
histosExportPartial      (false),
histosExportPartialCount (false),
histosExportMaxPerPartial(false),
histosExportPath         (""),
histosExportFile         (""),
taskExecutedTotal        (0),
taskExecuted             (0),
subTasks                 ()
{
  setClassName("Task");
  setInstanceName(getName());
}

Task::Task(const String & _name,
           const Configuration & _configuration)
:
ConfigurationManager     (_configuration),
IdentifiedObject         (_name,_name,"1.0",_name,_name,0),
timer                    (),
histogramManager         (),
parent                   (nullptr),
histosCreate             (false),
histosReset              (false),
histosClear              (false),
histosScale              (false),
histosPlot               (false),
histosPrint              (false),
histosForceRewrite       (false),
histosImport             (false),
histosImportPath         (""),
histosImportFile         (""),
histosExport             (false),
histosExportAsRoot       (false),
histosExportAsText       (false),
histosExportPartial      (false),
histosExportPartialCount (false),
histosExportMaxPerPartial(false),
histosExportPath         (""),
histosExportFile         (""),
taskExecutedTotal        (0),
taskExecuted             (0),
subTasks                 ()
{
  setClassName("Task");
  setInstanceName(getName());
}



void Task::setDefaultConfiguration()
{
//  String fullTaskPath = getFullTaskPath();
//  String taskName     = getName();
//  String configName   = fullTaskPath;
//  configName += taskName;
  setConfigurationPath(getFullTaskPath());
//  String path = getConfigurationPath();
//  cout << "==================================================" << endl;
//  cout << "==================================================" << endl;
//  cout << "==================================================" << endl;
//  cout << "=== Task::setDefaultConfiguration() ==============" << endl;
//  cout << " fullTaskPath...........: " << fullTaskPath << endl;
//  cout << " taskName...............: " << taskName << endl;
//  cout << " configName requested...: " << configName << endl;
//  cout << " configName set.........: " << path << endl;
//  cout << "==================================================" << endl;
//  cout << "==================================================" << endl;
//  cout << "==================================================" << endl;
//


  String none("none");
  String debug("Debug");
  addParameter("Severity",                      debug);
  addParameter("HistogramsCreate",              histosCreate);
  addParameter("HistogramsReset",               histosReset);
  addParameter("HistogramsClear",               histosClear);
  addParameter("HistogramsScale",               histosScale);
  addParameter("HistogramsPlot",                histosPlot);
  addParameter("HistogramsPrint",               histosPrint);
  addParameter("HistogramsForceRewrite",        histosForceRewrite);
  addParameter("HistogramsImport",              histosImport);
  addParameter("HistogramsImportPath",          histosImportPath);
  addParameter("HistogramsImportFile",          histosImportFile);
  addParameter("HistogramsExport",              histosExport);
  addParameter("HistogramsExportAsRoot",        histosExportAsRoot);
  addParameter("HistogramsExportAsText",        histosExportAsText);
  addParameter("HistogramsExportPartial",       histosExportPartial);
  addParameter("HistogramsExportPartialCount",  histosExportPartialCount);
  addParameter("HistogramsExportMaxPerPartial", histosExportMaxPerPartial);
  addParameter("HistogramsExportPath",          histosExportPath);
  addParameter("HistogramsExportFile",          histosExportFile);
}

void Task::configure()
{
  if (reportStart(__FUNCTION__))
    ;

  ConfigurationManager::configure();
  histosCreate              = getValueBool("HistogramsCreate");
  histosReset               = getValueBool("HistogramsReset");
  histosClear               = getValueBool("HistogramsClear");
  histosScale               = getValueBool("HistogramsScale");
  histosPlot                = getValueBool("HistogramsPlot");
  histosPrint               = getValueBool("HistogramsPrint");
  histosForceRewrite        = getValueBool("HistogramsForceRewrite");
  histosImport              = getValueBool("HistogramsImport");
  histosImportPath          = getValueString("HistogramsImportPath");
  histosImportFile          = getValueString("HistogramsImportFile");
  histosExport              = getValueBool("HistogramsExport");
  histosExportAsRoot        = getValueBool("HistogramsExportAsRoot");
  histosExportAsText        = getValueBool("HistogramsExportAsText");
  histosExportPartial       = getValueBool("HistogramsExportPartial");
  histosExportPartialCount  = getValueInt("HistogramsExportPartialCount");
  histosExportMaxPerPartial = getValueInt("HistogramsExportMaxPerPartial");
  histosExportPath          = getValueString("HistogramsExportPath");
  histosExportFile          = getValueString("HistogramsExportFile");
  if (hasSubTasks())
    {
    for (unsigned int  iTask=0; iTask<subTasks.size(); iTask++)
      {
      subTasks[iTask]->configure();
      }
    }
  configured = true;
  if (reportEnd(__FUNCTION__))
    ;
}



//
//void Task::setSeverity()
//{
//  MessageLogger::Severity selectedLevel;
//  String  logOption = getValueString("Severity");
//  if (logOption.Contains("Debug"))        selectedLevel = MessageLogger::Debug;
//  else if (logOption.Contains("Info"))    selectedLevel = MessageLogger::Info;
//  else if (logOption.Contains("Warning")) selectedLevel = MessageLogger::Warning;
//  else selectedLevel = MessageLogger::Info;
//  setSeverityLevel(selectedLevel);
//}
//

void Task::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  initializeTaskExecuted();
  if (histosImport)  importHistograms();
  if (histosCreate)  createHistograms();
//  if (calibsImport)  importCalibrations();
//  if (calibsCreate)  createCalibrations();
  if (hasSubTasks())  initializeSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  if (reportEnd(__FUNCTION__))
    ;
}


void Task::finalize()
{
  if (reportStart(__FUNCTION__))
    ;
  if (histosExport)  exportHistograms();
  if (histosPlot)    plotHistograms();
  if (histosPrint)   printHistograms();
  if (hasSubTasks()) finalizeSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::reset()
{
  if (reportStart(__FUNCTION__))
    ;
  resetTaskExecuted();
  if (histosCreate)  resetHistograms();
  if (hasSubTasks()) resetSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::clear()
{
  if (reportStart(__FUNCTION__))
    ;
  if (histosCreate)  clearHistograms();
  if (hasSubTasks()) clearSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::printConfiguration(ostream & output)
{
  output << "============================================================" << endl;
  output << "============================================================" << endl;
  output << "Task Name : " << getName() <<  endl;
  ConfigurationManager::printConfiguration(output);
  unsigned int nSubTasks = subTasks.size();
  //if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  //for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->printConfiguration(output);
  output << "============================================================" << endl;
  output << "============================================================" << endl;
}

void Task::importHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  String importPath = getValueString("HistogramsImportPath");
  String importFile = getValueString("HistogramsImportFile");
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    printItem("HistogramsImportPath",importPath);
    printItem("HistogramsImportFile",importFile);
    cout << endl;
  }
  TFile & inputFile = openRootFile(importPath,importFile,"READ");
  loadNEventsAccepted(inputFile);
  loadNEexecutedTask(inputFile);
  importHistograms(inputFile);
  if (reportEnd(__FUNCTION__))
    ;
}


void Task::importDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  String importPath = getValueString("HistogramsImportPath");
  String importFile = getValueString("HistogramsImportFile");
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    printItem("HistogramsImportPath",importPath);
    printItem("HistogramsImportFile",importFile);
    cout << endl;
    }
  TFile & inputFile = openRootFile(importPath,importFile,"READ");
  loadNEventsAccepted(inputFile);
  loadNEexecutedTask(inputFile);
  importDerivedHistograms(inputFile);
  if (reportEnd(__FUNCTION__))
    ;
}


void Task::resetHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  histogramManager.reset();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::clearHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  histogramManager.clear();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::scaleHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::plotHistograms()
{

}

void Task::printHistograms()
{

}

void Task::exportHistograms(TFile & outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  outputFile.cd();
  writeNEventsAccepted(outputFile);
  writeNEexecutedTask(outputFile);
  histogramManager.save(outputFile);
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::exportHistograms(ofstream & outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  histogramManager.save(outputFile);
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::exportHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  String histosExportPath     = getValueString("HistogramsExportPath");
  String histosExportFile     = getValueString("HistogramsExportFile");
  if (histosExportPath.Contains("null") || histosExportPath.Contains("none")) histosExportPath = "";
  if (histosExportFile.Contains("null") || histosExportFile.Contains("none")) histosExportFile = getName();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("HistogramsExportPath",histosExportPath);
    printItem("HistogramsExportFile",histosExportFile);
    cout << endl;
    }
  gSystem->mkdir(histosExportPath,1);

//  if (histosExportAsRoot)
//  {
  String option = "NEW";
  if (histosForceRewrite) option = "RECREATE";
  TFile & outputFile = openRootFile(histosExportPath,histosExportFile,option);
  exportHistograms(outputFile);
  outputFile.Close();
//  }
//  if (histosExportAsText)
//  {
//    ofstream * outputFile;
//    if (histosForceRewrite)
//      outputFile = openOutputAsciiFile(histosExportPath,histosExportFile,".dat","RECREATE");
//    else
//      outputFile = openOutputAsciiFile(histosExportPath,histosExportFile,".dat","NEW");
//    if (!outputFile)
//    {
//      postTaskError();
//      return;
//    }
//    exportHistograms(*outputFile);
//    outputFile->close();
//  }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::writeNEexecutedTask(TFile & outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  String name = "taskExecuted";
  writeParameter(outputFile,name,taskExecuted);
  if (reportEnd(__FUNCTION__))
    ;
}

long Task::loadNEexecutedTask(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  String name = "taskExecuted";
  taskExecuted = readParameter(inputFile,name);
  if (reportEnd(__FUNCTION__))
    ;
  return taskExecuted;
}

void Task::writeParameter(TFile & outputFile, const String & parameterName, long value)
{
  if (reportStart(__FUNCTION__))
    ;
  outputFile.cd();
  TParameter<Long64_t>(parameterName,value,'+').Write();
}

long Task::readParameter(TFile & inputFile, const String & parameterName)
{
  if (reportStart(__FUNCTION__))
    ;
  TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile.Get(parameterName);
  if (!par)
  {
    if (reportError(__FUNCTION__)) cout << "Parameter not found:" <<  parameterName << endl;
    postTaskError();
    return 1.0;
  }
  double value = par->GetVal();
  delete par;
  if (reportDebug(__FUNCTION__)) cout << "Parameter named " << parameterName << " has value : " << value << endl;
  return value;
}

TFile &  Task::openRootFile(const String & inputPath, const String & fileName, const String & ioOption)  throw (FileException)
{
  if (reportStart(__FUNCTION__))
    ;
  String inputFileName = inputPath;
  // make sure that if an inputPath is given, it ends with '/'
  int slash = inputFileName.First('/');
  int len = inputFileName.Length();
  if (len>0 && (len-1)!=slash) inputFileName += "/";
  inputFileName += fileName;
  if (!inputFileName.EndsWith(".root")) inputFileName += ".root";
  if (reportDebug (__FUNCTION__))  cout << "Opening file: " << inputFileName << " with option: " << ioOption << endl;
  TFile * inputFile = new TFile(inputFileName,ioOption);
  if (!inputFile) throw  FileException(inputFileName,"File not found","Task::openRootFile()");
  if (!inputFile->IsOpen())  throw  FileException(inputFileName,"File not found/opened","Task::openRootFile()");
  if (reportDebug(__FUNCTION__)) cout << "File opened successfully." << endl;
  return *inputFile;
}

ifstream & Task::openInputAsciiFile(const String & inputPath, const String & fileName, const String & extension, const String & ioOption)  throw (FileException)
{
  if (reportStart(__FUNCTION__))
    ;
  String inputFileName = inputPath;
  // make sure that if an inputPath is given, it ends with '/'
  int slash = inputFileName.First('/');
  int len = inputFileName.Length();
  //  cout << slash << endl;
  //  cout << len << endl;
  if (len>0 && (len-1)!=slash) inputFileName += "/";
  inputFileName += fileName;
  if (!inputFileName.EndsWith(extension)) inputFileName += extension;
  if (reportTrace(__FUNCTION__))
    cout << "Attempting to open output file named: " << inputFileName << " in mode: " << ioOption << endl;
  ios_base::openmode mode = ios_base::in;
  if (reportDebug (__FUNCTION__))  cout << "Opening file: " << inputFileName << " with option: " << ioOption << endl;
  ifstream * inputFile = new ifstream(inputFileName.Data(),mode);
  if (!inputFile || !inputFile->is_open()) throw  FileException(inputFileName,"File not found","Task::openInputAsciiFile()");
  if (reportDebug (__FUNCTION__)) cout << "File: " << inputFileName << " successfully opened." << endl;
  return *inputFile;
}

ofstream & Task::openOutputAsciiFile(const String & outputPath, const String & fileName, const String & extension, const String & ioOption)  throw (FileException)
{
  if (reportStart(__FUNCTION__))
    ;
  String outputFileName = outputPath;
  // make sure that if an outputPath is given, it ends with '/'
  int slash = outputFileName.First('/');
  int len = outputFileName.Length();
  if (len>0 && (len-1)!=slash) outputFileName += "/";
  outputFileName += fileName;
  if (!outputFileName.EndsWith(extension)) outputFileName += extension;
  if (reportTrace(__FUNCTION__)) cout << "Attempting to open output file named: " << outputFileName << " in mode: " << ioOption << endl;
  ios_base::openmode mode = ios_base::out|ios_base::app;
  if (reportDebug (__FUNCTION__))  cout << "Opening file: " << outputFileName << " with option: " << ioOption << endl;
  ofstream * outputFile = new ofstream(outputFileName.Data(),mode);
  if (!outputFile || !outputFile->is_open())  throw  FileException(outputFileName,"File not found","Task::openOutputAsciiFile()");
  if (reportDebug (__FUNCTION__)) cout << "File: " << outputFileName << " successfully opened." << endl;
  return *outputFile;
}


ifstream & Task::openInputBinaryFile(const String & inputPath, const String & fileName, const String & extension, const String & ioOption) throw (FileException)
{
  if (reportStart(__FUNCTION__))
    ;
  String inputFileName = inputPath;
  // make sure that if an inputPath is given, it ends with '/'
  int slash = inputFileName.First('/');
  int len = inputFileName.Length();
  if (len>0 && (len-1)!=slash) inputFileName += "/";
  inputFileName += fileName;
  if (!inputFileName.EndsWith(extension)) inputFileName += extension;
  if (reportTrace(__FUNCTION__))
    cout << "Attempting to open output file named: " << inputFileName << " in mode: " << ioOption << endl;

  ios_base::openmode mode = ios_base::in|std::ios::binary;
  if (reportDebug (__FUNCTION__))  cout << "Opening file: " << inputFileName << " with option: " << ioOption << endl;
  ifstream * inputFile = new ifstream(inputFileName.Data(),mode);
  if (!inputFile ||  !inputFile->is_open())  throw  FileException(inputFileName,"File not found","Task::openInputBinaryFile()");
  if (reportDebug (__FUNCTION__)) cout << "File: " << inputFileName << " successfully opened." << endl;
  return *inputFile;
}



ofstream & Task::openOutputBinaryFile(const String & outputPath, const String & fileName, const String & extension, const String & ioOption)  throw (FileException)
{
  if (reportStart(__FUNCTION__))
    ;
  String outputFileName = outputPath;
  // make sure that if an outputPath is given, it ends with '/'
  int slash = outputFileName.First('/');
  int len = outputFileName.Length();
  if (len>0 && (len-1)!=slash) outputFileName += "/";
  outputFileName += fileName;
  if (!outputFileName.EndsWith(extension)) outputFileName += extension;
  if (reportTrace(__FUNCTION__)) cout << "Attempting to open output file named: " << outputFileName << " in mode: " << ioOption << endl;
  ios_base::openmode mode = ios_base::out|std::ios::binary;
  if (reportDebug (__FUNCTION__))  cout << "Opening file: " << outputFileName << " with option: " << ioOption << endl;
  ofstream * outputFile = new ofstream(outputFileName.Data(),mode);
  if (!outputFile || !outputFile->is_open()) throw  FileException(outputFileName,"File not found","Task::openOutputBinaryFile()");
  if (reportDebug (__FUNCTION__)) cout << "File: " << outputFileName << " successfully opened." << endl;
  return *outputFile;
}

void Task::copyFile(const String & inputPath,  const String & inputFileName, const String & inExtension,
                    const String & outputPath, const String & outputFileName, const String & outExtension)
{
  ifstream & inputFile  = openInputBinaryFile(inputPath,inputFileName,inExtension,"");
  ofstream & outputFile = openOutputBinaryFile(outputPath,outputFileName,outExtension,"");
  if (reportDebug(__FUNCTION__))
    cout << "Attempting to copy file " << inputPath+inputFileName << " to " << outputPath+outputFileName << endl;
  // do a binary buffer copy
  outputFile << inputFile.rdbuf();
  inputFile.close();
  outputFile.close();
  if (reportDebug(__FUNCTION__)) cout << " Successfully completed!" << endl;
}



void Task::initializeSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->initialize();
  if (reportEnd(__FUNCTION__))
  { }
}

void Task::executeSubTasks()
{
  unsigned int nSubTasks = subTasks.size();
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->execute();
}

void Task::finalizeSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->finalize();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::resetSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->reset();
  if (reportEnd(__FUNCTION__))
  { }
}

void Task::clearSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->clear();
  if (reportEnd(__FUNCTION__))
  { }
}


Task * Task::addSubTask(Task * task)
{
  if (!task)
  {
    if (reportFatal(__FUNCTION__)) cout << "Given task pointer is null. Abort." << endl;
    postTaskFatal();
    return task;
  }
  if (task==this)
  {
    if (reportFatal(__FUNCTION__)) cout << "Given task pointer is self. Abort." << endl;
    postTaskFatal();
    return task;
  }

  subTasks.push_back( task );
  if (task->parent == nullptr)  task->setParent(this);
  if (reportInfo(__FUNCTION__)) cout << "Added task " << task->getName() << " to task " << getName() << endl;
  return task;
}

vector<String> Task::listFilesInDir(const String & pathname,
                                    const String & ext,
                                    bool prependPath,
                                    bool verbose,
                                    int  maximumDepth,
                                    int  currentDepth)
{
  String dirname = pathname;
  int depth = currentDepth;
  if (!dirname.EndsWith("/")) dirname += "/";
  if (verbose) cout << " Searching................: " << dirname << endl;
  if (verbose) cout << " maximumDepth.............: " << maximumDepth << endl;
  if (verbose) cout << " currentDepth.............: " << depth << endl;
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  vector<String>  fileNames;
  vector<String>  subdirs;

  if (files)
  {
    TSystemFile *file;
    String fname;
    TIter next(files);
    while ((file=(TSystemFile*)next()))
    {
      fname = file->GetName();
      if (file->IsDirectory() &&  !fname.BeginsWith(".") )
      {
        subdirs.push_back(fname);
      }
      else if (fname.EndsWith(ext))
      {
        if (prependPath)
          fileNames.push_back(dirname+fname);
        else
          fileNames.push_back(fname);
        //cout << fname.Data() << endl;
        if (verbose) cout << fname << endl;
      }
    }
  }
  int nSubdirs = subdirs.size();
  if (verbose) cout << " Number of subdir found...: " << nSubdirs << endl;
  ++depth;

  if (nSubdirs>0 && depth<=maximumDepth)
  {

    for (int iDir=0; iDir<nSubdirs; iDir++)
    {
      vector<String> additionalFiles;
      String  subdirname = dirname;
      subdirname += "/";
      subdirname += subdirs[iDir];
      subdirname += "/";
      additionalFiles = listFilesInDir(subdirname,ext,prependPath,verbose,maximumDepth,depth);
      int nAdditionalfiles = additionalFiles.size();
      for (int iFile=0;iFile<nAdditionalfiles;iFile++)
        fileNames.push_back(additionalFiles[iFile]);
    }
  }
  if (verbose) cout << " Number of files  found...: " << fileNames.size() << endl;
  if (verbose) cout << " Returning up one level.... " <<  endl;

  return fileNames;
}

//!
//! Get file  names at the given location that match all  patterns in includePatterns and exclude patterns in excludePatterns
//!
vector<String>  Task::listFilesInDir(const String & pathName,
                                     vector<String> includePatterns,
                                     vector<String> excludePatterns,
                                     bool prependPath,
                                     bool verbose,
                                     int  maximumDepth,
                                     int  currentDepth)
{

  if (reportStart(__FUNCTION__))
    ;
  vector<String> outputList;
  vector<String> fileList = listFilesInDir(pathName,".root",prependPath,verbose,maximumDepth,currentDepth);
  unsigned int nNames = fileList.size();
  if (reportDebug(__FUNCTION__))
  {
    cout << "      nNames:" << nNames << endl;
    cout << "   nIncludes:" << includePatterns.size() << endl;
    cout << "   nExcludes:" << excludePatterns.size() << endl;
  }
  for (unsigned int k=0; k<fileList.size(); k++)
  {
    String name = fileList[k];
    bool include = true;
    for (unsigned int kInclude=0; kInclude<includePatterns.size(); kInclude++)
    {
      String pattern = includePatterns[kInclude];
      if (!name.Contains(pattern)) { include = false; break;}
    }
    if (!include)
    {
      continue;
    }
    for (unsigned int kExclude=0; kExclude<excludePatterns.size(); kExclude++)
    {
      String pattern = excludePatterns[kExclude];
      if (name.Contains(pattern))
      {
        include = false;
        break;
      }
    }
    if (include)
    {
      if (name.Contains(".root"))
      {
        int dot = name.First('.');
        int len = name.Length();
        if (dot==len-5 )
          name.Remove(dot,len-dot);
      }
      //String check = pathName+name;
      //cout << " CHECK:::::: " << check << endl;
      outputList.push_back(name);
    }
  }
  return outputList;
}

vector<String> Task::getSelectedFileNamesFrom(const String & folder)
{

  if (reportStart(__FUNCTION__))
    ;
  vector<String> includePatterns;
  vector<String> excludePatterns;
  vector<String> selectedNames;
  for (int k=0; k<20; k++)
  {
    String key = "IncludedPattern"; key += k;
    String  value = getValueString(key);
    if (!value.Contains("none")) includePatterns.push_back(value);
  }
  for (int k=0; k<20; k++)
  {
    String key = "ExcludedPattern"; key += k;
    String  value = getValueString(key);
    if (!value.Contains("none")) excludePatterns.push_back(value);
  }
  return listFilesInDir(folder,includePatterns,excludePatterns);
}

String Task::removeRootExtension(const String fileName)
{
  String name = fileName;
  int dot = name.Last('.');
  int len = name.Length();
  //  cout << dot << endl;
  //  cout << len << endl;
  if (dot==len-5 ) name.Remove(dot,len-dot);
  return name;
}

Task * Task::getTaskAt(int depth)
{
  //if (depth == 0) return this;
  Task * task = this;
  Task * parentTask = task->getParent();
  for (int k=0; k<depth; k++)
  {
    task = parentTask;
    if (task==nullptr) break;
    parentTask = task->getParent();
  }
  return task;
}


const Task * Task::getTaskAt(int depth) const
{
  //if (depth == 0) return this;
  const Task * task = this;
  const Task * parentTask = task->getParent();
  //cout <<  "0 : name: " << task->getName();
  for (int k=0; k<depth; k++)
  {
    task = parentTask;
    if (task==nullptr) break;
    //cout << k <<  " : name: " << task->getName();
    parentTask = task->getParent();
  }
  return task;
}

String Task::getReverseTaskPath(int depth) const
{
  String result;
  String work;
  String colon = String(":");
  for (int k=0; k<=depth; k++)
  {
    const Task * task = getTaskAt(k);
    if (task!=nullptr)
    {
      work = task->getName();
      work += colon;
      if (result.Length()>0)
      {
        result = work + result;
      }
      else
      {
        result = work;
      }
    }
  }
  return result;
}

vector<String> Task::getTaskPathTokens() const
{
  vector<String> paths;
  int n = getNAncestors();
  for (int k=n; k>=0; k--)
  {
    const Task * task= getTaskAt(k);
    if (task==nullptr)
    {
      cout << "<F> getTaskPathTokens()  Logic error." << endl;
      exit(1);
    }
    else
    {
      paths.push_back(task->getName());
    }
  }
  return paths;
}


String Task::getTaskPath(int depth) const
{
  String path = "";
  vector<String> paths = getTaskPathTokens();
  for (int k=0; k<=depth; k++)
  {
    path += paths[k];
    path += ":";
  }
  return path;
}


String Task::getFullTaskPath() const
{
  String path;
  //cout << "getFullTaskPath() -- 1" << endl;
  int n = getNAncestors();
  //cout << "getFullTaskPath() -- 2" << endl;
  path = getReverseTaskPath(n);
  //cout << "getFullTaskPath() -- 3" << endl;
  return path;
}


int Task::getNAncestors() const
{
  int count = 0;
  if (parent==nullptr) return count;
  Task * parentTask = parent;
  while (parentTask!=nullptr)
  {
    count++;
    if (parentTask!=nullptr)
      parentTask = parentTask->getParent();
    else
      break;
  }
  return count;
}


void Task::printIntroMessage(const TString & option __attribute__ (( unused )) ) const
{
  if (reportInfo(__FUNCTION__))
    {
    cout << "No intro message available for this task. Sorry!" << endl;
    }
}

void Task::printHelp(const TString & option __attribute__ (( unused )) ) const
{
  if (reportInfo(__FUNCTION__))
    {
    cout << "No help message available for this task. Sorry!" << endl;
    }
}

void Task::printVersion(const TString & option __attribute__ (( unused )) ) const
{
  if (reportInfo(__FUNCTION__))
    {
    cout << "Version:" << getVersion() << endl;
    }
}



