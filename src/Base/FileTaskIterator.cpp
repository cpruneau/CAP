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
using CAP::FileTaskIterator;

ClassImp(FileTaskIterator);

FileTaskIterator::FileTaskIterator(const String & _name,
                                   const Configuration & _configuration)
:
TaskIterator(_name,_configuration),
selectedFileNames(),
appendedString("_Derived")
{
  appendClassName("FileTaskIterator");
}

void FileTaskIterator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  addParameter("AppendedString",TString("_Derived") );
}

void FileTaskIterator::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  timer.start();
  incrementTaskExecuted();
  String fct = "execute()";
  unsigned int nSelectedFiles = selectedFileNames.size();

  if (nSelectedFiles==0)
    {
    // no file selected, get the list from the designated folder
    // with the included and excluded patterns.
    String histoInputPath = getValueString("HistogramsImportPath");
    selectedFileNames = getSelectedFileNamesFrom(histoInputPath);
    }
  nSelectedFiles = selectedFileNames.size();
  if (nSelectedFiles<1)
    {
    if (reportWarning(__FUNCTION__))
      {
      cout << "Attempting to execute file iterator with no selected files." << endl;
      cout << "====> Check your code! " <<  endl;
      }
    return;
    }
  if (reportInfo(__FUNCTION__))
    {
    cout << "Running with .." << endl;
    cout << "            nTasks: " << getNSubTasks() << endl;
    cout << "nselectedFileNames: " << nSelectedFiles << endl << endl;
    }
  postTaskOk();

  currentFileIndex = 0;
  while (currentFileIndex<nSelectedFiles)
    {
    if (reportDebug(__FUNCTION__))
      {
      cout << "Processing file index:" << currentFileIndex << endl;
      cout << "                named:" << selectedFileNames[currentFileIndex] << endl;
      }
    initializeSubTasks();
    if (!isTaskOk())
      {
      if (reportWarning(__FUNCTION__))
        {
        cout << "Initialization failed for file index:" << currentFileIndex << endl;
        cout << "                               named:" << selectedFileNames[currentFileIndex] << endl;
        cout << "ABORT!" << endl << endl;
        }
      return;
      }
    executeSubTasks();
    if (!isTaskOk())
      {
      if (reportWarning(__FUNCTION__))
        {
        cout << "     Execution failed for file index:" << currentFileIndex << endl;
        cout << "                               named:" << selectedFileNames[currentFileIndex] << endl;
        cout << "ABORT!" << endl << endl;
        }
      return;
      }
    finalizeSubTasks();
    if (!isTaskOk())
      {
      if (reportWarning(__FUNCTION__))
        {
        cout << "      Finalize failed for file index:" << currentFileIndex << endl;
        cout << "                               named:" << selectedFileNames[currentFileIndex] << endl;
        cout << "ABORT!" << endl << endl;
        }
      return;
      }
    clearSubTasks();
    if (!isTaskOk())
      {
      if (reportWarning(__FUNCTION__))
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
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  Completed with status : " << getStateName() << endl;
    cout << "            "; timer.print(cout);
    cout << endl << endl<< endl << endl;
    }
}

//!
//! Add several  file  templates for use by this task iterator
//!
void FileTaskIterator::addFileNames(VectorString  names)
{
  
  if (reportStart(__FUNCTION__))
    ;
  if (names.size()<1)
    {
    if (reportWarning(__FUNCTION__))
      {
      cout << "Given VectorString  names is empty." << endl;
      cout << "Check your code!!!!!" << endl << endl;
      }
    return;
    }
  for (unsigned int k=0; k<names.size(); k++)
    {
    if (reportInfo("FileTaskIterator",getName(),"addFilenameTemplate(const String name)"))
      {
      cout << "Adding template:" << names[k] << endl;
      }
    selectedFileNames.push_back(names[k]);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Add several  file  templates for use by this task iterator
//!
void FileTaskIterator::addFileNames(unsigned int nNames, TString** names)
{
  
  if (reportStart(__FUNCTION__))
    ;
  if (nNames<1)
    {
    if (reportWarning(__FUNCTION__))
      {
      cout << "Given number of names is null." << endl;
      cout << "Check your code!!!!!" << endl << endl;
      }
    return;
    }
  for (unsigned int k=0; k<nNames; k++)  selectedFileNames.push_back(*(names[k]));
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Add several  file  templates for use by this task iterator
//!
void FileTaskIterator::addFileNames(const String pathName,
                                            VectorString  includePatterns,
                                            VectorString  excludePatterns)
{
  
  if (reportStart(__FUNCTION__))
    ;
  VectorString  fileList = listFilesInDir(pathName,".root");
  unsigned int nNames = fileList.size();
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
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
      String check = pathName+name;
      //cout << " CHECK:::::: " << check << endl;
        selectedFileNames.push_back(pathName+name);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void FileTaskIterator::initializeSubTasks()
{
  
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubtasks = getNSubTasks();
  if (nSubtasks>0)
    {
    if (reportDebug(__FUNCTION__)) cout << "Initializing " << nSubtasks << " tasks." << endl;

    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      Task * subTask = getSubTaskAt(iTask);
      if (!isTaskOk()) break;
      if (reportDebug(__FUNCTION__))  cout << "Initializing task:" << subTask->getName() << endl;
      String name = removeRootExtension(selectedFileNames[currentFileIndex]);
      subTask->setHistogramFileNames(name,name+appendedString);
      subTask->initialize();
      }
    }
  else
    {
    if (reportDebug(__FUNCTION__)) cout << "No subtask to initialize. " <<   endl;
    }
  if (reportEnd(__FUNCTION__))
    ;
}

