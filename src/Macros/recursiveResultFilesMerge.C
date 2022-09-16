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

void simpleRecursiveMerge(const char *filename, TString &inputFiles, int nmerge=5, bool verbose=kFALSE)
{
  /// \param filename   the name of the output file
  /// \param inputfiles a string with the list of files to merge one per line
  /// \param nmerge     number of files to merge per level

  TObjArray *arr=inputFiles.Tokenize("\n");

  Int_t depth=0;

  while (arr->GetEntries()>1){
    printf("depth: %d\n",depth);
    for (Int_t iIter=0; iIter<TMath::Ceil((Double_t)arr->GetEntries()/((Double_t)nmerge)); ++iIter){
      if (verbose) Info("simpleRecursiveMerge","Iter: %d\n",iIter);
      TFileMerger m(0);
      m.OutputFile(Form("%s.%d.%d.root",filename,depth,iIter));
      if (verbose) Info("simpleRecursiveMerge","writing output file: %s\n", Form("%s.%d.%d.root",filename,depth,iIter));
      for (Int_t ifile=iIter*nmerge; ifile<(iIter+1)*nmerge; ++ifile){
        if (!arr->At(ifile)) continue;
        if (verbose) Info("simpleRecursiveMerge","Adding file: %s\n",arr->At(ifile)->GetName());
        m.AddFile(arr->At(ifile)->GetName());
      }
      m.Merge();
    }
    delete arr;
    arr=0x0;
    TString s=gSystem->GetFromPipe(Form("ls %s.%d.[0-9]*.root",filename,depth));
    arr=s.Tokenize("\n");
    ++depth;
    if (verbose) Info("simpleRecursiveMerge","%s","\n-----------\n");
  }
  gSystem->Exec(Form("mv %s.%d.0.root %s.root",filename,depth-1,filename));
  gSystem->Exec(Form("rm %s.[0-9]*.[0-9]*.root",filename));
  
  delete arr;
}

/* We are executing at the point where the result files will be removed                           */
void recursiveResultFilesMerge(const char *filename, const char *inputfilename, int nmerge=5, bool verbose=kFALSE)
{
  /// \param filename      the name of the output file
  /// \param inputfilename the basename of the input files names
  /// \param nmerge        number of files to merge per level

  TString inputFiles = gSystem->GetFromPipe(TString::Format("find . -name \"%s_*.root\" -type f",inputfilename).Data());

  TObjArray *arr=inputFiles.Tokenize("\n");

  TString tomergefiles = "";
  for (int ifile = 0; ifile < arr->GetEntries(); ifile++) {
    tomergefiles += TString::Format("%s\n",arr->At(ifile)->GetName());
  }
  simpleRecursiveMerge(filename, tomergefiles, nmerge, verbose);

  for (int ix = 0; ix < arr->GetEntries(); ix++) {
    gSystem->Exec(Form("rm %s",arr->At(ix)->GetName()));
  }
  
  delete arr;
}


