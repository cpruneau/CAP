/** Simple macro to perform recursive merging

*/
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TMath.h"
#include "TObjArray.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "TString.h"
#include "TError.h"
#endif

void recursiveScannedUrQMDMerge(const char *outdir, const char *dirpattern, const char *cent, Int_t nmerge=5, Bool_t verbose=kTRUE)
{
  /// \param inputFiles wildcard list of input files
  /// \param outputFile name of the output file
  /// \param nmerge     number of files to merge per level

  TString outputFile = TString::Format("%s/Urqmd_Histograms_%s",outdir,cent);
  TString inputFiles = gSystem->GetFromPipe(TString::Format("find %s -name \"Urqmd_Histograms_%s.root\" -type f",dirpattern,cent).Data());

  TObjArray *arr=inputFiles.Tokenize("\n");

  Int_t depth=0;

  while (arr->GetEntries()>1){
    printf("depth: %d\n",depth);
    for (Int_t iIter=0; iIter<TMath::Ceil((Double_t)arr->GetEntries()/((Double_t)nmerge)); ++iIter){
      if (verbose) Info("SimpleRecursiveMerge","Iter: %d\n",iIter);
      TFileMerger m(0);
      m.OutputFile(Form("%s.%d.%d.root",outputFile.Data(),depth,iIter));
      if (verbose) Info("SimpleRecursiveMerge","writing output file: %s\n", Form("%s.%d.%d.root",outputFile.Data(),depth,iIter));
      for (Int_t ifile=iIter*nmerge; ifile<(iIter+1)*nmerge; ++ifile){
        if (!arr->At(ifile)) continue;
        if (verbose) Info("SimpleRecursiveMerge","Adding file: %s\n",arr->At(ifile)->GetName());
        m.AddFile(arr->At(ifile)->GetName());
      }
      m.Merge();
    }
    delete arr;
    arr=0x0;
    TString s=gSystem->GetFromPipe(Form("ls %s.%d.[0-9]*.root",outputFile.Data(),depth));
    arr=s.Tokenize("\n");
    ++depth;
    if (verbose) Info("SimpleRecursiveMerge","%s","\n-----------\n");
  }
  gSystem->Exec(Form("mv %s.%d.0.root %s.root",outputFile.Data(),depth-1,outputFile.Data()));
  gSystem->Exec(Form("rm %s.[0-9]*.[0-9]*.root",outputFile.Data()));
  
  delete arr;
}


