
vector<TString> listFilesInDir(const TString & pathname,
                               const TString & ext,
                               bool prependPath=true,
                               bool verbose=false,
                               int  maximumDepth=1,
                               int  currentDepth=0)
{
  TString dirname = pathname;
  int depth = currentDepth;
  if (!dirname.EndsWith("/")) dirname += "/";
  if (verbose) cout << " Searching: " << dirname << endl;
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  vector<TString>  fileNames;
  vector<TString>  subdirs;

  if (files)
    {
    TSystemFile *file;
    TString fname;
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
  if (verbose) cout << " Number of subdir found: " << nSubdirs << endl;
  ++depth;

  if (nSubdirs>0 && depth<=maximumDepth)
    {

    for (int iDir=0; iDir<nSubdirs; iDir++)
      {
      vector<TString> additionalFiles;
      TString  subdirname = dirname;
      subdirname += "/";
      subdirname += subdirs[iDir];
      subdirname += "/";
      additionalFiles = listFilesInDir(subdirname,ext,prependPath,verbose,maximumDepth,depth);
      int nAdditionalfiles = additionalFiles.size();
      for (int iFile=0;iFile<nAdditionalfiles;iFile++)
        fileNames.push_back(additionalFiles[iFile]);
      }
    }
  if (verbose) cout << " Number of files  found: " << fileNames.size() << endl;
  return fileNames;
}

void wtf()
{
  TString dirname = "/Volumes/ClaudeDisc4/"; //Simulations"; //PythiaRopes/";
  TString ext = "root";
  int depth = 4;
  vector<TString>  fileNames;

  fileNames = listFilesInDir(dirname,ext,false,true,depth);

  cout << endl;
  cout << endl;
  cout << endl;
  for (int iFile=0;iFile<fileNames.size();iFile++)
    cout << fileNames[iFile] << endl;

//  for (int k=0; k<=depth; k++)
//    {
//    TSystemDirectory dir(dirname, dirname);
//    TList *files = dir.GetListOfFiles();
//
//    if (files)
//      {
//      TSystemFile *file;
//      TString fname;
//      TIter next(files);
//      while ((file=(TSystemFile*)next()))
//        {
//        fname = file->GetName();
//        if (file->IsDirectory() )
//          {
//          subdirs.push_back(fname);
//          }
//        if (!file->IsDirectory() && fname.EndsWith(ext))
//          {
//          fileNames.push_back(fname);
//          cout << fname.Data() << endl;
//          //cout << fname << endl;
//          }
//        }
//      }
//    dirname += "*/";
//    }



//  TFile  * fg = new TFile("/Volumes/ClaudeDisc4/OutputFiles/RhoDecayTest2/RhoDecay/RhoDecay_Pair_Gen_Derived.root");
//  TFile  * fr = new TFile("/Volumes/ClaudeDisc4/OutputFiles/RhoDecayTest2/RhoDecay/RhoDecay_Pair_Reco_Derived.root");
//
//  TH2F * r2Gen  = (TH2F*) fg->Get("Pair_Gen_All_HP_HM_R2_DetaDphi_shft");
//  TH2F * r2Reco = (TH2F*) fr->Get("Pair_Reco_All_HP_HM_R2_DetaDphi_shft");
//
//  TCanvas * c1 = new TCanvas();
//  if (r2Gen)
//    {
//    r2Gen->GetXaxis()->SetRangeUser(-1.5,1.5);
//    r2Gen->Draw("SURF3");
//    }
//  else
//    {
//    cout << "no can do" << endl;
//    }
//
//  TCanvas * c2 = new TCanvas();
//  if (r2Reco)
//    {
//    r2Reco->GetXaxis()->SetRangeUser(-1.5,1.5);
//    r2Reco->Draw("SURF3");
//    }
//  else
//    {
//    cout << "no can do" << endl;
//    }
//  TCanvas * c3 = new TCanvas();
//  TH2F * diff = new TH2F(*r2Gen);
//  diff->Add(r2Reco,-1.0);
//  diff->Draw("SURF3");
//
//  TCanvas * c4 = new TCanvas();
//  TH2F * ratio = new TH2F(*r2Gen);
//  ratio->Divide(r2Gen,r2Reco,1.0,1.0);
//  ratio->Draw("SURF3");


  //
}
