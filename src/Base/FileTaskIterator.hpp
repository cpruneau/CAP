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
#ifndef CAP__FileTaskIterator
#define CAP__FileTaskIterator
#include "TaskIterator.hpp"

//!
//! Task Iterator designed to repeat the same series of tasks on many files/directories
//!
class FileTaskIterator : public TaskIterator
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  FileTaskIterator(const TString & _name,
                   const Configuration & _configuration);

  //!
  //! DTOR
  //!
  virtual ~FileTaskIterator() {}

  //!
  //! Initialize this task and all its subtasks. This method handles the retrieval and passing of the files to be analyzed by the subtasks.
  //!
  virtual void initializeSubTasks();

  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();

  //!
  //! Set the default configuration of this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Returns the number of  file  templates used by this task iterator
  //!
  inline unsigned int getNSelectedFileNames() const
  {
  return selectedFileNames.size();
  }

  //!
  //! Add a  file  templates for use by this task iterator
  //!
  void addFileNames(const TString name);

  //!
  //! Add several  file  templates for use by this task iterator
  //!
  void addFileNames(vector<TString> names);

  //!
  //! Add several  file  templates for use by this task iterator
  //!
  void addFileNames(unsigned int n, TString** names);

  //!
  //! Add several  file  templates/names for use by this task iterator
  //! must contain the baseFileName
  //! must include all of the patterns in includePatterns
  //! must exclude all of the partterns in excludePartterns
  //!
  void addFileNames(const TString pathName,
                            vector<TString> includePatterns,
                            vector<TString> excludePatterns);

  //!
  //!Get the index of the file currently being processed (in the file list submited to this task)
  //!
  inline const TString getCurrentFileIndex() const
  {
  return currentFileIndex;
  }
  
  //!
  //! Get array of file names to be processes by this iterator task.
  //!
  inline const vector<TString> & getSelectedFileNames() const
  {
  return selectedFileNames;
  }

protected:

  //!
  //! Array of file names to be processes by this iterator task.
  //!
  vector<TString> selectedFileNames;

  //!
  //! Character string or name to be added to the file name (on output) being processed.
  //!
  TString appendedString;
  
  //!
  //! Index of the file being processes.
  //!
  unsigned int currentFileIndex;

  ClassDef(FileTaskIterator,0)
};

#endif /* CAP__FileTaskIterator */
