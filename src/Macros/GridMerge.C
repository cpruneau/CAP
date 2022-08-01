/** Simple macro to perform recursive merging of Duke models chain events 
 WARNING: removing the input files after the merging

 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TMath.h"
#include "TObjArray.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "TString.h"
#include "TError.h"
#endif

//!
//! \param filesToMerge array of file names identifying the .root files to be merge
//! \param outputFileName  path and name of the files to contain the merged files.
//!
void simpleRecursiveMerge(vector<TString> & filesToMerge,
                          const TString outputFileName="Test",
                          bool  deleteAddedFiles=kFALSE,
                          bool  verbose=kFALSE)
{
  int nFilesToMerge = filesToMerge.size();
  if (nFilesToMerge<1)
    {
    cout << "<W> WAKY, WAKY: There are no files to merge" << endl;
    return;
    }
  if (verbose) cout << "<I> n files to merge: " << nFilesToMerge << endl;
  if (verbose) cout << "<I> Output file name: " << outputFileName << endl;
  TFileMerger m(0);
  m.OutputFile(outputFileName);
  for (int k=0; k<nFilesToMerge; ++k)
    {
    TString name = filesToMerge[k];
    if (verbose) cout << "k: " << k << " name: " << name << endl;
    m.AddFile(name);
    }
  m.Merge();

  //if (deleteAddedFiles)
  //{
  //  gSystem->Exec(Form("mv %s.%d.0.root %s.root",filename,depth-1,filename));
  //  gSystem->Exec(Form("rm %s.[0-9]*.[0-9]*.root",filename));
  //  }
}


bool fileNameIncludes(const TString & name,
                      vector<TString> includePatterns)
{
  bool ok = true;
  for (unsigned int kInclude=0; kInclude<includePatterns.size(); kInclude++)
    {
    TString pattern = includePatterns[kInclude];
    if (!name.Contains(pattern))
      {
      //cout << "Does not contain pattern:" << pattern << endl;
      ok = false; break;
      }
    }
  return ok;
}

bool fileNameExcludes(const TString & name,
                      vector<TString> excludePatterns)
{
  bool ok = true;
  for (unsigned int kExclude=0; kExclude<excludePatterns.size(); kExclude++)
    {
    TString pattern = excludePatterns[kExclude];
    if (name.Contains(pattern))
      {
      //cout << "Contains pattern:" << pattern << endl;
      ok = false;
      break;
      }
    }
  return ok;
}



vector<TString> listFilesInDir(const TString & pathName,
                               vector<TString> includePatterns,
                               vector<TString> excludePatterns)
{
  TSystemDirectory dir(pathName, pathName);
  TList *files = dir.GetListOfFiles();
  vector<TString>  fileNames;

  if (files)
    {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=(TSystemFile*)next()))
      {
      fname = file->GetName();
      //cout << " Examining: " << fname << endl;
      if (!file->IsDirectory() &&
          fileNameIncludes(fname,includePatterns) &&
          fileNameExcludes(fname,excludePatterns) &&
          fname.EndsWith(".root"))
        {
        //cout << "add file to list" << endl;
        TString name = pathName;
        name += "/";
        name += fname;
        fileNames.push_back(name);
        }
      if (file->IsDirectory() && fileNameExcludes(fname,excludePatterns) && !fname.EndsWith("."))
        {
        TString subpath = pathName;
        subpath += "/";
        subpath += fname;
        subpath += "/";
        //cout << "examine subpath:" << subpath << endl;
        vector<TString>  names = listFilesInDir(subpath,includePatterns,excludePatterns);
        if (names.size()>0)
          {
          for (int k=0; k<names.size(); k++)
            {
            fileNames.push_back(names[k]);
            }
          }
        }
      }
    }
  return fileNames;
}

/* We are executing at the point where the result files will be removed                           */
//! \param filename      the name of the output file
//! \param inputfilename the basename of the input files names
//! \param nmerge        number of files to merge per level
//!
void GridMerge(const TString inputPathName,
               const TString outputPathName="./",
               const TString outputFileName="Test.root",
               const TString patternName="Pair",
               bool  deleteAddedFiles=kFALSE,
               bool  verbose=true)
{
  vector<TString> includePatterns;
  vector<TString> excludePatterns;
  includePatterns.push_back(patternName);
  excludePatterns.push_back(TString("Derived"));
  excludePatterns.push_back(TString("BalFct"));
  excludePatterns.push_back(TString(".DS_Store"));
  vector<TString> fileNames = listFilesInDir(inputPathName,includePatterns,excludePatterns);

  if (verbose)
    {
    for (int iFile=0; iFile<fileNames.size();iFile++)
      {
      cout << " iFile:" << iFile << " Name:" << fileNames[iFile] << endl;
      }
    }
  TString outputName = outputPathName;
  outputName += outputFileName;
  if (!outputName.EndsWith(".root")) outputName += ".root";
  simpleRecursiveMerge(fileNames,outputName,false,true);
}
