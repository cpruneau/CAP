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
#include "FileTaskIterator.hpp"
ClassImp(FileTaskIterator);

FileTaskIterator::FileTaskIterator(const TString & _name,
                                   const Configuration & _configuration,
                                   MessageLogger::LogLevel _reportLevel)
:
Task(_name,_configuration,_reportLevel),
selectedFileNames(),
appendedString("_Derived")
{
  setClassName("FileTaskIterator");
  setInstanceName("FileTaskIterator");
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

//FileTaskIterator::FileTaskIterator(vector<TString> _selectedFileNames, const TString _appendedString)
//:
//Task(),
//selectedFileNames(_selectedFileNames),
//appendedString(_appendedString)
//{
//  setClassName("FileTaskIterator");
//  setInstanceName("FileTaskIterator");
//  setDefaultConfiguration();
//}

void FileTaskIterator::setDefaultConfiguration()
{
  if (reportStart("FileTaskIterator",getName(),"setDefaultConfiguration()"))
    {
    }
  configuration.setName("FileTaskIterator Configuration");
  configuration.setParameter("useParticles",false);
  configuration.addParameter("appendedString",TString("_Derived") );

  if (reportDebug("FileTaskIterator",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}




void FileTaskIterator::run()
{
  timer.start();
  TString fct = "run()";
  unsigned int nSelectedFiles = selectedFileNames.size();

  if (nSelectedFiles==0)
    {
    // no file selected, get the list from the designated folder
    // with the included and excluded patterns.
    TString histoInputPath = configuration.getValueString("histoInputPath");
    selectedFileNames = getSelectedFileNamesFrom(histoInputPath);
    }
  nSelectedFiles = selectedFileNames.size();
  if (nSelectedFiles<1)
    {
    if (reportWarning(fct))
      {
      cout << "Attempting to execute file iterator with no selected files." << endl;
      cout << "====> Check your code! " <<  endl;
      }
    return;
    }
  if (reportInfo(fct))
    {
    cout << "Running with ..." << endl;
    cout << "            nTasks: " << getNSubtasks() << endl;
    cout << "nselectedFileNames: " << nSelectedFiles << endl << endl;
    }
  postTaskOk();

  currentFileIndex = 0;
  while (currentFileIndex<nSelectedFiles)
    {
    if (reportDebug(fct))
      {
      cout << "Processing file index:" << currentFileIndex << endl;
      cout << "                named:" << selectedFileNames[currentFileIndex] << endl;
      }
    initializeTasks();
    if (!isTaskOk())
      {
      if (reportWarning(fct))
        {
        cout << "Initialization failed for file index:" << currentFileIndex << endl;
        cout << "                               named:" << selectedFileNames[currentFileIndex] << endl;
        cout << "ABORT!" << endl << endl;
        }
      return;
      }
    executeTasks();
    if (!isTaskOk())
      {
      if (reportWarning(fct))
        {
        cout << "     Execution failed for file index:" << currentFileIndex << endl;
        cout << "                               named:" << selectedFileNames[currentFileIndex] << endl;
        cout << "ABORT!" << endl << endl;
        }
      return;
      }
    finalizeTasks();
    if (!isTaskOk())
      {
      if (reportWarning(fct))
        {
        cout << "      Finalize failed for file index:" << currentFileIndex << endl;
        cout << "                               named:" << selectedFileNames[currentFileIndex] << endl;
        cout << "ABORT!" << endl << endl;
        }
      return;
      }
    clearTasks();
    if (!isTaskOk())
      {
      if (reportWarning(fct))
        {
        cout << "      Finalize failed for file index:" << currentFileIndex << endl;
        cout << "                               named:" << selectedFileNames[currentFileIndex] << endl;
        cout << "ABORT!" << endl << endl;
        }
      return;
      }
    currentFileIndex++;
    }
  timer.stop();
  if (reportInfo(fct))
    {
    cout << endl;
    cout << "  Completed with status : " << getTaskStatusName() << endl;
    cout << "            "; timer.print(cout);
    cout << endl << endl<< endl << endl;
    }
}

//!
//! Add several  file  templates for use by this task iterator
//!
void FileTaskIterator::addFileNames(vector<TString> names)
{
  TString fct = "addFileNames(vector<TString> names)";
  if (reportStart(fct))
    { }
  if (names.size()<1)
    {
    if (reportWarning(fct))
      {
      cout << "Given vector<TString> names is empty." << endl;
      cout << "Check your code!!!!!" << endl << endl;
      }
    return;
    }
  for (unsigned int k=0; k<names.size(); k++)
    {
    if (reportInfo("FileTaskIterator",getName(),"addFilenameTemplate(const TString name)"))
      {
      cout << "Adding template:" << names[k] << endl;
      }
    selectedFileNames.push_back(names[k]);
    }
  if (reportEnd(fct))
    { }
}

//!
//! Add several  file  templates for use by this task iterator
//!
void FileTaskIterator::addFileNames(unsigned int nNames, TString** names)
{
  TString fct = "addFileNames(unsigned int n, TString** names)";
  if (reportStart(fct))
    { }
  if (nNames<1)
    {
    if (reportWarning(fct))
      {
      cout << "Given number of names is null." << endl;
      cout << "Check your code!!!!!" << endl << endl;
      }
    return;
    }
  for (unsigned int k=0; k<nNames; k++)  selectedFileNames.push_back(*(names[k]));
  if (reportEnd(fct))
    { }
}

//!
//! Add several  file  templates for use by this task iterator
//!
void FileTaskIterator::addFileNames(const TString pathName,
                                            vector<TString> includePatterns,
                                            vector<TString> excludePatterns)
{
  TString fct = "addFileNames(const TString pathName,vector<TString> includePatterns,vector<TString> excludePatterns)";
  if (reportStart(fct))
    { }
  vector<TString> fileList = listFilesInDir(pathName,".root");
  unsigned int nNames = fileList.size();
  cout << "      nNames:" << nNames << endl;
  cout << "   nIncludes:" << includePatterns.size() << endl;
  cout << "   nExcludes:" << excludePatterns.size() << endl;
  for (unsigned int k=0; k<fileList.size(); k++)
    {
    TString name = fileList[k];
    bool include = true;
    for (unsigned int kInclude=0; kInclude<includePatterns.size(); kInclude++)
      {
      TString pattern = includePatterns[kInclude];
      if (!name.Contains(pattern)) { include = false; break;}
      }
    if (!include)
      {
      continue;
      }
    for (unsigned int kExclude=0; kExclude<excludePatterns.size(); kExclude++)
      {
      TString pattern = excludePatterns[kExclude];
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
      TString check = pathName+name;
      cout << " CHECK:::::: " << check << endl;
        selectedFileNames.push_back(pathName+name);
      }
    }
  if (reportEnd(fct))
    { }
}

void FileTaskIterator::initializeTasks()
{
  if (reportStart("Task",getName(),"initializeTasks()"))
    { }
  if (!isTaskOk()) return;
  //initialize();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    {
    if (reportDebug("Task",getName(),"initializeTasks()"))
      {
      cout << "Initializing " << nSubtasks << " tasks." << endl;
      }
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      if (!isTaskOk()) break;
      if (reportDebug("Task",getName(),"initializeTasks()"))
        {
        cout << "Initializing task:" << subtasks[iTask]->getName() << endl;
        }
      TString name = removeRootExtension(selectedFileNames[currentFileIndex]);
      subtasks[iTask]->setHistogramFileNames(name,name+appendedString);
      subtasks[iTask]->initializeTasks();
      }
    }
  else
    {
    if (reportDebug("Task",getName(),"initializeTasks()"))
      {
      cout << "No subtask to initialize. " <<   endl;
      }
    }
  if (reportEnd("Task",getName(),"initializeTasks()"))
    { }
}

