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
#include "HistogramCollection.hpp"
#include "TLorentzVector.h"
#include "TKey.h"

ClassImp(Collection<TH1>);

ClassImp(HistogramCollection);

HistogramCollection::HistogramCollection(const TString & _name,
                                         LogLevel  _debugLevel)
:
Collection(_name, true, _debugLevel),
randomGenerator(new TRandom())
{

  setClassName("HistogramCollection");
  setInstanceName(_name);
}

HistogramCollection::HistogramCollection(const HistogramCollection & source)
:
Collection<TH1>(source),
randomGenerator(source.randomGenerator)
{
  for (unsigned int iObject=0; iObject<source.size(); iObject++)
    {
    TH1* h0 = (TH1*) source.objects[iObject];
    if (!ptrExist(__FUNCTION__,h0)) return;
    append( (TH1*) h0->Clone());
    }
}

//!
// operator=
// Set this collection equal to the given collection (deep copy)
// The name of this collection is set to be identical to the name
// of the given collection. One may change the collection's name with the
// setName method.
//!
HistogramCollection & HistogramCollection::operator=(const HistogramCollection & source)
{

  if (reportStart(__FUNCTION__))
    ;
  if (this!=&source)
    {
    Collection<TH1>::operator=(source);
    for (unsigned int iObject=0; iObject<source.size(); iObject++)
      {
      TH1* h0 = source.objects[iObject];
      if (!ptrExist(__FUNCTION__,h0)) return *this;
      append( (TH1*) h0->Clone());
      }
    }
  return *this;
}

HistogramCollection::~HistogramCollection()
{   }


//!
// Set Default Style for Plots
//!
void HistogramCollection::setDefaultOptions(bool color)
{

  if (reportStart(__FUNCTION__))
    ;
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Reset all histograms of this collection
//!
void HistogramCollection::reset()
{

  if (reportStart(__FUNCTION__))
    ;
  for (unsigned int iObject=0; iObject<size(); iObject++) objects[iObject]->Reset();
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Create 1D histogram
//!
TH1 * HistogramCollection::createHistogram(const TString &  name,
                                           int n, double min_x, double max_x,
                                           const TString &  title_x,
                                           const TString &  title_y)
{

  if (reportDebug(__FUNCTION__))
    cout << "Creating  1D histo " << name << " nBins:" << n << " min_x:" << min_x << " max_x:" << max_x << endl;
  TH1 * h = new TH1F(name,name,n,min_x,max_x);
  if (title_x.Sizeof()>0)  h->GetXaxis()->SetTitle(title_x);
  if (title_y.Sizeof()>0)  h->GetYaxis()->SetTitle(title_y);
  append(h);
  return h;
}


//!
//! Create 1D histogram
//!
TH1 * HistogramCollection::createHistogram(const TString &  name,
                                           int n, double * bins,
                                           const TString &  title_x,
                                           const TString &  title_y)
{

  if (reportDebug(__FUNCTION__)) cout << "Creating  1D histo " << name << " with " << n << " non uniform nBins:" << endl;
  TH1 * h = new TH1F(name,name,n,bins);
  if (title_x.Sizeof()>0)  h->GetXaxis()->SetTitle(title_x);
  if (title_y.Sizeof()>0)  h->GetYaxis()->SetTitle(title_y);
  append(h);
  return h;
}


//!
// Create 2D histogram
//!

TH2 * HistogramCollection::createHistogram(const TString &  name,
                                           int n_x, double min_x, double max_x,
                                           int n_y, double min_y, double max_y,
                                           const TString &  title_x,
                                           const TString &  title_y,
                                           const TString &  title_z )
{

  if (reportDebug(__FUNCTION__))
    cout << "Creating  2D histo " << name << " n_x:" << n_x << " min_x:" << min_x << " max_x:" << max_x << " n_y:" << n_y << " min_y:" << min_y << " max_y:" << max_y<< endl;
  TH2 * h;
  h = new TH2F(name,name,n_x,min_x,max_x,n_y,min_y,max_y);
  if (title_x.Sizeof()>0)  h->GetXaxis()->SetTitle(title_x);
  if (title_y.Sizeof()>0)  h->GetYaxis()->SetTitle(title_y);
  if (title_z.Sizeof()>0)  h->GetZaxis()->SetTitle(title_z);
  append(h);
  return h;
}

//!
// Create 2D histogram
//!
TH2 * HistogramCollection::createHistogram(const TString &  name,
                                           int n_x, double* xbins, int n_y, double min_y, double max_y,
                                           const TString &  title_x,
                                           const TString &  title_y,
                                           const TString &  title_z)

{

  if (reportDebug(__FUNCTION__))
    cout << "Creating  2D histo " << name << " with " << n_x << " vs " << n_y << " non uniform nBins:" << endl;
  TH2 * h;
  h = new TH2F(name,name,n_x,xbins,n_y,min_y,max_y);
  if (title_x.Sizeof()>0)  h->GetXaxis()->SetTitle(title_x);
  if (title_y.Sizeof()>0)  h->GetYaxis()->SetTitle(title_y);
  if (title_z.Sizeof()>0)  h->GetZaxis()->SetTitle(title_z);
  append(h);
  return h;
}


//!
// Create 3D histogram
//!
TH3 * HistogramCollection::createHistogram(const TString &  name,
                                           int n_x, double min_x, double max_x,
                                           int n_y, double min_y, double max_y,
                                           int n_z, double min_z, double max_z,
                                           const TString &  title_x,
                                           const TString &  title_y,
                                           const TString &  title_z,
                                           const TString &  title_w = "w")

{

  if (reportDebug(__FUNCTION__))
    cout << "Creating  3D histo " << name
    << " n_x:" << n_x << " min_x:" << min_x << " max_x:" << max_x << " X:" << title_x
    << " n_y:" << n_y << " min_y:" << min_y << " max_y:" << max_y << " Y:" << title_y
    << " n_z:" << n_z << " min_z:" << min_z << " max_z:" << max_z << " Z:" << title_z
    << " W:" << title_w  << endl;
  TH3 * h;
  h = new TH3F(name,name,n_x,min_x,max_x,n_y,min_y,max_y,n_z,min_z,max_z);
  if (title_x.Sizeof()>0)  h->GetXaxis()->SetTitle(title_x);
  if (title_y.Sizeof()>0)  h->GetYaxis()->SetTitle(title_y);
  if (title_z.Sizeof()>0)  h->GetZaxis()->SetTitle(title_z);
  append(h);
  return h;
}


//!
// Create 1D profile histogram
//!
TProfile * HistogramCollection::createProfile(const TString & name,
                                              int n_x,double min_x,double max_x,
                                              const TString &  title_x,
                                              const TString &  title_y)
{

  if (reportDebug(__FUNCTION__)) cout << "Creating  1D profile " << name << " n_x:" << n_x << " min_x:" << min_x << " max_x:" << max_x << endl;
  TProfile * h = new TProfile(name,name,n_x,min_x,max_x);
  if (title_x.Sizeof()>0)  h->GetXaxis()->SetTitle(title_x);
  if (title_y.Sizeof()>0)  h->GetYaxis()->SetTitle(title_y);
  append(h);
  return h;
}

//!
// Create 1D profile histogram
//!
TProfile * HistogramCollection::createProfile(const TString &  name,
                                              int n_x,  double* bins,
                                              const TString &  title_x,
                                              const TString &  title_y)
{

  if (reportDebug(__FUNCTION__))
    cout << "Creating  1D profile " << name << " w/ variabe size bins" << endl;
  TProfile * h = new TProfile(name,name,n_x,bins);
  if (title_x.Sizeof()>0)  h->GetXaxis()->SetTitle(title_x);
  if (title_y.Sizeof()>0)  h->GetYaxis()->SetTitle(title_y);
  append(h);
  return h;
}

//!
// Create 2D profile histogram
//!
TProfile2D * HistogramCollection::createProfile(const TString &  name,
                                                int n_x, double min_x, double max_x,
                                                int n_y, double min_y, double max_y,
                                                const TString &  title_x,
                                                const TString &  title_y,
                                                const TString &  title_z)
{

  if (reportDebug(__FUNCTION__))
    cout << "Creating  2D profile " << name
    << " n_x:" << n_x << " min_x:" << min_x << " max_x:" << max_x
    << " n_y:" << n_y << " min_y:" << min_y << " max_y:" << max_y
    << endl;
  TProfile2D * h = new TProfile2D(name,name,n_x,min_x,max_x,n_y,min_y,max_y);
  if (title_x.Sizeof()>0)  h->GetXaxis()->SetTitle(title_x);
  if (title_y.Sizeof()>0)  h->GetYaxis()->SetTitle(title_y);
  if (title_z.Sizeof()>0)  h->GetZaxis()->SetTitle(title_z);
  append(h);
  return h;
}

//!
// Add Histograms to an external list
//!
void HistogramCollection::addHistogramsToExtList(TList *list)
{

  if (reportDebug(__FUNCTION__))
    cout << "Adding histo to external list"  << endl;
  for (unsigned int iObject=0; iObject<size(); iObject++) list->Add(objects[iObject]);
  /* the instance stops the histograms ownership */
  setOwnership(false);
  if (reportEnd(__FUNCTION__))
    { }
}

//!
// Save Histograms to file
//!
void HistogramCollection::saveHistograms(TFile * outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, outputFile)) return;
  if (reportDebug(__FUNCTION__)) cout << "    Saving histograms to file: " << outputFile->GetName()  << endl;
  if (reportDebug(__FUNCTION__)) cout << " Number of histograms to save: " <<  size() << endl;
  outputFile->cd();
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    if (reportDebug(__FUNCTION__)) cout << " Saving iObject: " << iObject << " named: " <<  objects[iObject]->GetName() << endl;
    objects[iObject]->Write();
    }
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Scale selected histograms by the given factor
//! Set scaleAll to true to scale all histogram of this collection.
//!  Profile are not scaled here because usually, the scaling done is to account for
//!  the number of events
//!
void HistogramCollection::scale(double factor)
{
  if (reportStart(__FUNCTION__))
    { }
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1 * h = objects[iObject];
    if ( (h->IsA()==TProfile::Class()) ||  (h->IsA()==TProfile2D::Class()) ) continue;
    h->Scale(factor);
    }
  if (reportEnd(__FUNCTION__))
    { }
}

void HistogramCollection::setHistoProperties(TH1 * h, const GraphConfiguration & graphConfiguration)
{
  if (reportDebug(__FUNCTION__))
    cout << "Setting properties of histo: " << h->GetTitle() << endl;
  h->SetLineColor(graphConfiguration .getValueInt("lineColor"));
  h->SetLineStyle(graphConfiguration.getValueInt("lineStyle"));
  h->SetLineWidth(graphConfiguration.getValueInt("lineWidth"));
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.getValueDouble("nXDivisions"));
  xAxis->SetTitleSize(graphConfiguration.getValueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfiguration.getValueDouble("xTitleOffset"));
  //xAxis->SetTitle(graphConfiguration.xTitle);
  xAxis->SetLabelSize(graphConfiguration.getValueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfiguration.getValueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.getValueInt("nYDivisions"));
  yAxis->SetTitleSize(graphConfiguration.getValueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfiguration.getValueDouble("yTitleOffset"));
  yAxis->SetLabelSize(graphConfiguration.getValueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfiguration.getValueDouble("yLabelOffset"));
  //yAxis->SetTitle(graphConfiguration.yTitle);
  if (h->IsA() == TH2::Class()  || h->IsA() == TH2F::Class() || h->IsA() == TH2F::Class() )
    {
    if (reportDebug(__FUNCTION__)) cout << "Setting options as 2D histo: " << h->GetTitle() << endl;
    TAxis * zAxis = (TAxis *) h->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.getValueInt("nZDivisions"));
    zAxis->SetTitleSize(graphConfiguration.getValueDouble("zTitleSize"));
    zAxis->SetTitleOffset(graphConfiguration.getValueDouble("zTitleOffset"));
    zAxis->SetLabelSize(graphConfiguration.getValueDouble("zLabelSize"));
    zAxis->SetLabelOffset(graphConfiguration.getValueDouble("zLabelOffset"));
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void HistogramCollection::setHistoProperties(TH2 * h, const GraphConfiguration & graphConfiguration)
{
  if (reportDebug(__FUNCTION__))
    cout << endl << "Setting options of 2D histo: " << h->GetTitle() << endl;
  h->SetLineColor(graphConfiguration .getValueInt("lineColor"));
  h->SetLineStyle(graphConfiguration.getValueInt("lineStyle"));
  h->SetLineWidth(graphConfiguration.getValueInt("lineWidth"));
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.getValueDouble("nXDivisions"));
  xAxis->SetTitleSize(graphConfiguration.getValueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfiguration.getValueDouble("xTitleOffset"));
  xAxis->SetLabelSize(graphConfiguration.getValueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfiguration.getValueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.getValueInt("nYDivisions"));
  yAxis->SetTitleSize(graphConfiguration.getValueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfiguration.getValueDouble("yTitleOffset"));
  yAxis->SetLabelSize(graphConfiguration.getValueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfiguration.getValueDouble("yLabelOffset"));

  if (h->IsA() == TH2::Class() || h->IsA() == TH2F::Class() || h->IsA() == TH2D::Class())
    {
    TAxis * zAxis = (TAxis *) h->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.getValueInt("nZDivisions"));
    zAxis->SetTitleSize(graphConfiguration.getValueDouble("zTitleSize"));
    zAxis->SetTitleOffset(graphConfiguration.getValueDouble("zTitleOffset"));
    zAxis->SetLabelSize(graphConfiguration.getValueDouble("zLabelSize"));
    zAxis->SetLabelOffset(graphConfiguration.getValueDouble("zLabelOffset"));
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void HistogramCollection::setHistoProperties(TH1 * h, const GraphConfiguration & graphConfiguration, const TString & xTitle, const TString & yTitle)
{
  if (reportDebug(__FUNCTION__))
    cout << endl << "Setting options of histo: " << h->GetTitle() << endl;
  h->SetLineColor(graphConfiguration .getValueInt("lineColor"));
  h->SetLineStyle(graphConfiguration.getValueInt("lineStyle"));
  h->SetLineWidth(graphConfiguration.getValueInt("lineWidth"));
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.getValueDouble("nXDivisions"));
  xAxis->SetTitleSize(graphConfiguration.getValueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfiguration.getValueDouble("xTitleOffset"));
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(graphConfiguration.getValueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfiguration.getValueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.getValueInt("nYDivisions"));
  yAxis->SetTitleSize(graphConfiguration.getValueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfiguration.getValueDouble("yTitleOffset"));
  yAxis->SetLabelSize(graphConfiguration.getValueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfiguration.getValueDouble("yLabelOffset"));
  yAxis->SetTitle(yTitle);
  if (reportEnd(__FUNCTION__))
    ;
}

void HistogramCollection::setHistoProperties(TH2 * h, const GraphConfiguration & graphConfiguration, const TString & xTitle, const TString & yTitle, const TString & zTitle)
{
  if (reportDebug(__FUNCTION__))
    cout << endl << "Setting options of histo: " << h->GetTitle() << endl;
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.getValueDouble("nXDivisions"));
  xAxis->SetTitleSize(graphConfiguration.getValueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfiguration.getValueDouble("xTitleOffset"));
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(graphConfiguration.getValueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfiguration.getValueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.getValueInt("nYDivisions"));
  yAxis->SetTitleSize(graphConfiguration.getValueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfiguration.getValueDouble("yTitleOffset"));
  yAxis->SetLabelSize(graphConfiguration.getValueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfiguration.getValueDouble("yLabelOffset"));
  yAxis->SetTitle(yTitle);
  TAxis * zAxis = (TAxis *) h->GetZaxis();
  zAxis->SetNdivisions(graphConfiguration.getValueInt("nZDivisions"));
  zAxis->SetTitleSize(graphConfiguration.getValueDouble("zTitleSize"));
  zAxis->SetTitleOffset(graphConfiguration.getValueDouble("zTitleOffset"));
  zAxis->SetLabelSize(graphConfiguration.getValueDouble("zLabelSize"));
  zAxis->SetLabelOffset(graphConfiguration.getValueDouble("zLabelOffset"));
  zAxis->SetTitle(zTitle);
  if (reportEnd(__FUNCTION__))
    ;
}

int  HistogramCollection::loadCollection(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(inputFile)) return -1;
  TIter keyList(inputFile->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)keyList()))
    {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1")) continue;
    TH1 *h = (TH1*)key->ReadObj();
    append(h);
    }
  if (reportEnd(__FUNCTION__))
    ;
  return 0;
}


//! ==========================
// Add the histograms of the given collection to those of this collection
//! ==========================
HistogramCollection * HistogramCollection::clone() const
{
  if (reportStart(__FUNCTION__))
    ;
  HistogramCollection * newCollection = new HistogramCollection(getInstanceName(), getReportLevel());
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1* h0 = objects[iObject];
    if (!ptrExist(__FUNCTION__,h0)) return nullptr;
    TH1* h1 = (TH1*) h0->Clone();
    newCollection->append(h1);
    }
  return newCollection;
}


//! ==========================
// Add the histograms of the given collection to those of this collection
//! ==========================
void HistogramCollection::add(const HistogramCollection & c1, double a1)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!sameSizeAs(c1))
    {
    if (reportError(__FUNCTION__) )
      {
      cout << endl;
      cout << "Invalid operation on collections -- incompatible sizes:"  << endl;
      cout << "       this: " << getName()    << " contains " << size() << " histograms" << endl;
      cout << "         c1: " << c1.getName() << " contains " << c1.size() << " histograms" << endl;
      }
    return;
    }
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1* h0 = objects[iObject];
    TH1* h1 = c1.objects[iObject];
    if (!h0 || !h1 )
      {
      if (reportError(__FUNCTION__) )
        {
        cout << endl;
        cout << " Null pointer detected at iObject:" << iObject << endl;
        cout << " this: " << getName()    << " histogram ptr: " << h0 << endl;
        cout << "   c1: " << c1.getName() << " histogram ptr: " << h1  << endl;
        }
      return;
      }
    if (!sameDimensions(__FUNCTION__,h0,h1))
      {
      if (reportWarning(getClassName(), getInstanceName(), "add(..)" ) )
        {
        cout << " Incompatible histogram sizes at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram named: " << h0->GetName() << endl;
        cout << "  c1: " << c1.getName() << " histogram named: " << h1->GetName()  << endl;
        }
      return;
      }
    if (reportDebug() )
      {
      cout << "At iObject:" << iObject << " Adding histogram " << h0->GetName() << " and histogram " << h1->GetName() << endl;
      }
    h0->Add(h1,a1);
    }
}

//! ==========================
// Add the histograms of the given collections and store results in this collection
//! ==========================
void HistogramCollection::add(const HistogramCollection & c1, const HistogramCollection & c2, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!sameSizeAs(c1) || !sameSizeAs(c2))
    {
    if (reportError(__FUNCTION__) )
      {
      cout << endl;
      cout << "Invalid operation on collections -- incompatible sizes:"  << endl;
      cout << "this: " << getName()    << " contains " << size() << " histograms" << endl;
      cout << "  c1: " << c1.getName() << " contains " << c1.size() << " histograms" << endl;
      cout << "  c2: " << c2.getName() << " contains " << c2.size() << " histograms" << endl;
      }
    return;
    }
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1* h0 = objects[iObject];
    TH1* h1 = c1.objects[iObject];
    TH1* h2 = c2.objects[iObject];
    if (!h0 || !h1 || !h2)
      {
      if (reportError(__FUNCTION__) )
        {
        cout << endl;
        cout << " Null pointer detected at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram ptr: " << h0 << endl;
        cout << "  c1: " << c1.getName() << " histogram ptr: " << h1  << endl;
        cout << "  c2: " << c2.getName() << " histogram ptr: " << h2  << endl;
        }
      return;
      }
    if (!sameDimensions(__FUNCTION__,h0,h1)  || !sameDimensions(__FUNCTION__,h0,h2))
      {
      if (reportWarning() )
        {
        cout << endl;
        cout << " Incompatible histogram sizes at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram named: " << h0->GetName() << endl;
        cout << "  c1: " << c1.getName() << " histogram named: " << h1->GetName()  << endl;
        cout << "  c2: " << c2.getName() << " histogram named: " << h2->GetName()  << endl;
        }
      return;
      }
    h0->Add(h1,h2,a1,a2);
    }
}

//! ==========================
// Add the histograms of the given collections and store results in this collection
//! ==========================
void HistogramCollection::add(const HistogramCollection & c1,
                              const HistogramCollection & c2,
                              const HistogramCollection & c3,
                              double a1, double a2, double a3)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!sameSizeAs(c1) || !sameSizeAs(c2) || !sameSizeAs(c3))
    {
    if (reportError(__FUNCTION__) )
      {
      cout << endl;
      cout << "Invalid operation on collections -- incompatible sizes:"  << endl;
      cout << "this: " << getName()    << " contains " << size() << " histograms" << endl;
      cout << "  c1: " << c1.getName() << " contains " << c1.size() << " histograms" << endl;
      cout << "  c2: " << c2.getName() << " contains " << c2.size() << " histograms" << endl;
      cout << "  c3: " << c3.getName() << " contains " << c3.size() << " histograms" << endl;
      }
    return;
    }
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1* h0 = objects[iObject];
    TH1* h1 = c1.objects[iObject];
    TH1* h2 = c2.objects[iObject];
    TH1* h3 = c3.objects[iObject];
    if (!h0 || !h1 || !h2 || !h3)
      {
      if (reportError(__FUNCTION__) )
        {
        cout << endl;
        cout << " Null pointer detected at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram ptr: " << h0 << endl;
        cout << "  c1: " << c1.getName() << " histogram ptr: " << h1  << endl;
        cout << "  c2: " << c2.getName() << " histogram ptr: " << h2  << endl;
        cout << "  c3: " << c2.getName() << " histogram ptr: " << h3  << endl;
        }
      return;
      }
    if (!sameDimensions(__FUNCTION__,h0,h1)  || !sameDimensions(__FUNCTION__,h0,h2) || !sameDimensions(__FUNCTION__,h0,h3))
      {
      if (reportWarning(__FUNCTION__))
        {
        cout << endl;
        cout << " Incompatible histogram sizes at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram named: " << h0->GetName() << endl;
        cout << "  c1: " << c1.getName() << " histogram named: " << h1->GetName()  << endl;
        cout << "  c2: " << c2.getName() << " histogram named: " << h2->GetName()  << endl;
        cout << "  c3: " << c3.getName() << " histogram named: " << h3->GetName()  << endl;
        }
      return;
      }

    if (reportDebug(__FUNCTION__))
      {
      cout << "At iObject:" << iObject << " Adding histograms " << h1->GetName() << " histogram " << h2->GetName()  << " and histogram " << h3->GetName() << endl;
      }
    h0->Add(h1,h2,a1,a2);
    h0->Add(h3,a3);
    }
}

//! ==========================
// Add the histograms of the given collections and store results in this collection
//! ==========================
void HistogramCollection::add(const HistogramCollection & c1,
                              const HistogramCollection & c2,
                              const HistogramCollection & c3,
                              const HistogramCollection & c4,
                              double a1, double a2, double a3, double a4)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!sameSizeAs(c1) || !sameSizeAs(c2) || !sameSizeAs(c3) || !sameSizeAs(c4))
    {
    if (reportError(__FUNCTION__) )
      {
      cout << endl;
      cout << "Invalid operation on collections -- incompatible sizes:"  << endl;
      cout << "this: " << getName()    << " contains " << size() << " histograms" << endl;
      cout << "  c1: " << c1.getName() << " contains " << c1.size() << " histograms" << endl;
      cout << "  c2: " << c2.getName() << " contains " << c2.size() << " histograms" << endl;
      cout << "  c3: " << c3.getName() << " contains " << c3.size() << " histograms" << endl;
      cout << "  c4: " << c4.getName() << " contains " << c4.size() << " histograms" << endl;
      }
    return;
    }
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1* h0 = objects[iObject];
    TH1* h1 = c1.objects[iObject];
    TH1* h2 = c2.objects[iObject];
    TH1* h3 = c3.objects[iObject];
    TH1* h4 = c4.objects[iObject];
    if (!h0 || !h1 || !h2 || !h3|| !h4)
      {
      if (reportError(__FUNCTION__))
        {
        cout << endl;
        cout << " Null pointer detected at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram ptr: " << h0 << endl;
        cout << "  c1: " << c1.getName() << " histogram ptr: " << h1  << endl;
        cout << "  c2: " << c2.getName() << " histogram ptr: " << h2  << endl;
        cout << "  c3: " << c3.getName() << " histogram ptr: " << h3  << endl;
        cout << "  c4: " << c4.getName() << " histogram ptr: " << h4  << endl;
        }
      return;
      }
    if (!sameDimensions(__FUNCTION__,h0,h1)  || !sameDimensions(__FUNCTION__,h0,h2) || !sameDimensions(__FUNCTION__,h0,h3) || !sameDimensions(__FUNCTION__,h0,h4))
      {
      if (reportWarning(__FUNCTION__))
        {
        cout << endl;
        cout << " Incompatible histogram sizes at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram named: " << h0->GetName() << endl;
        cout << "  c1: " << c1.getName() << " histogram named: " << h1->GetName()  << endl;
        cout << "  c2: " << c2.getName() << " histogram named: " << h2->GetName()  << endl;
        cout << "  c3: " << c3.getName() << " histogram named: " << h3->GetName()  << endl;
        cout << "  c4: " << c4.getName() << " histogram named: " << h4->GetName()  << endl;
        }
      return;
      }
    if (reportDebug(__FUNCTION__))
      {
      cout << "At iObject:" << iObject << " Adding histogram " << h1->GetName() << " histogram " << h2->GetName()  << " histogram " << h3->GetName()  << " and histogram " << h4->GetName()  << endl;
      }
    h0->Add(h1,h2,a1,a2);
    h0->Add(h3,a3);
    h0->Add(h4,a4);
    }
}

//! ==========================
// Divide the histograms of this collection by those of the given collection
//! ==========================
void HistogramCollection::divide(const HistogramCollection & c1)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!sameSizeAs(c1))
    {
    if (reportError(__FUNCTION__) )
      {
      cout << endl;
      cout << "Invalid operation on collections -- incompatible sizes:"  << endl;
      cout << "this: " << getName()    << " contains " << size() << " histograms" << endl;
      cout << "  c1: " << c1.getName() << " contains " << c1.size() << " histograms" << endl;
      }
    return;
    }
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1* h0 = objects[iObject];
    TH1* h1 = c1.objects[iObject];
    if (!h0 || !h1)
      {
      if (reportError(__FUNCTION__) )
        {
        cout << endl;
        cout << " Null pointer detected at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram ptr: " << h0 << endl;
        cout << "  c1: " << c1.getName() << " histogram ptr: " << h1  << endl;
        }
      return;
      }
    if (!sameDimensions(__FUNCTION__,h0,h1))
      {
      if (reportWarning() )
        {
        cout << endl;
        cout << " Incompatible histogram sizes at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram named: " << h0->GetName() << endl;
        cout << "  c1: " << c1.getName() << " histogram named: " << h1->GetName()  << endl;
        }
      return;
      }
    if (reportDebug() )
      {
      cout << "At iObject:" << iObject << " divide histogram " << h0->GetName() << " by histogram " << h1->GetName() << endl;
      }
    h0->Divide(h1);
    }
}

//! ==========================
// Divide the histograms of  collection c1 by those of collection c2 and store
// the result in this collection
//! ==========================
void HistogramCollection::divide(const HistogramCollection & c1,
                                 const HistogramCollection & c2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!sameSizeAs(c1) || !sameSizeAs(c2))
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;
      cout << "Invalid operation on collections -- incompatible sizes:"  << endl;
      cout << "this: " << getName()    << " contains " << size() << " histograms" << endl;
      cout << "  c1: " << c1.getName() << " contains " << c1.size() << " histograms" << endl;
      cout << "  c2: " << c2.getName() << " contains " << c2.size() << " histograms" << endl;
      }
    return;
    }
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1* h0 = objects[iObject];
    TH1* h1 = c1.objects[iObject];
    TH1* h2 = c2.objects[iObject];
    if (!h0 || !h1 || !h2)
      {
      if (reportError(__FUNCTION__))
        {
        cout << " Null pointer detected at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram ptr: " << h0 << endl;
        cout << "  c1: " << c1.getName() << " histogram ptr: " << h1  << endl;
        cout << "  c2: " << c2.getName() << " histogram ptr: " << h2  << endl;
        }
      return;
      }
    if (!sameDimensions(__FUNCTION__,h0,h1)  || !sameDimensions(__FUNCTION__,h0,h2))
      {
      if (reportWarning() )
        {
        cout << " Incompatible histogram sizes at iObject:" << iObject << endl;
        cout << "this: " << getName()    << " histogram named: " << h0->GetName() << endl;
        cout << "  c1: " << c1.getName() << " histogram named: " << h1->GetName()  << endl;
        cout << "  c2: " << c2.getName() << " histogram named: " << h2->GetName()  << endl;
        return;
        }
      }
    h0->Divide(h1,h2);
    }
}

void HistogramCollection::differenceCollection(const HistogramCollection & collection, const HistogramCollection & refCollection, bool correlatedUncertainties)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!collection.sameSizeAs(refCollection))
    {
    if (reportError(__FUNCTION__) )
      {
      cout << "Invalid operation on collections:"  << endl;
      cout << "Attempting to calculate a difference of  collection " << collection.getName() << " containing " << collection.size() << " histograms" << endl;
      cout << "with a collection named " << refCollection.getName() << " containing " << refCollection.size() << " histograms" << endl;
      return;
      }
    }
  if (reportInfo(__FUNCTION__))
    {
    cout << "Number of objects to calculate:" <<  collection.size() << endl;
    }
  for (unsigned int iObject=0; iObject<collection.size(); iObject++)
    {
    TH1* h     = collection.objects[iObject];
    TH1* hRef  = refCollection.objects[iObject];
    if (!ptrExist(__FUNCTION__,h,hRef)) return;
    if (!sameDimensions(__FUNCTION__,h,hRef)) return;
    TH1* hDiff = (TH1*) h->Clone();
    TString name = hDiff->GetName();
    name.ReplaceAll("Reco","Ratio");
    hDiff->SetName(name);
    hDiff->SetTitle(name);

    if (reportDebug(__FUNCTION__))
      {
      cout << "At iObject:" << iObject << " Computing  difference of  histogram " << h->GetName() << " and histogram " << hRef->GetName() << endl;
      }
    differenceHistos(h,hRef,hDiff,correlatedUncertainties);
    append(hDiff);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void HistogramCollection::ratioCollection(const HistogramCollection & collection, const HistogramCollection & refCollection, bool correlatedUncertainties)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!collection.sameSizeAs(refCollection))
    {
    if (reportError(__FUNCTION__))
      {
      cout << "Invalid operation on collections:"  << endl;
      cout << "Attempting to calculate a ratio of  collection " << collection.getName() << " containing " << collection.size() << " histograms" << endl;
      cout << "with a collection named " << refCollection.getName() << " containing " << refCollection.size() << " histograms" << endl;
      return;
      }
    }
  if (reportInfo(__FUNCTION__))
    {
    cout << "Number of objects to calculate:" <<  collection.size() << endl;
    }
  for (unsigned int iObject=0; iObject<collection.size(); iObject++)
    {
    TH1* h     = collection.objects[iObject];
    TH1* hRef  = refCollection.objects[iObject];

//    if (h->IsA()==TH1::Class() || h->IsA()==TH1F::Class() || h->IsA()==TH1D::Class() || h->IsA()==TH1I::Class() )
//      {
//      cout << "h is a  1D" << endl;
//      }
//    if (h->IsA()==TH2::Class() || h->IsA()==TH2F::Class() || h->IsA()==TH2D::Class() || h->IsA()==TH2I::Class() )
//      {
//      cout << "h is a  2D" << endl;
//      }
    if (!ptrExist(__FUNCTION__,h,hRef)) return;
    if (!sameDimensions(__FUNCTION__,h,hRef)) return;
    TH1* hRatio = (TH1*)  h->Clone();
    TString name = hRatio->GetName();
    name.ReplaceAll("Reco","Ratio");
    hRatio->SetName(name);
    hRatio->SetTitle(name);

    if (reportDebug(__FUNCTION__))
      {
      cout << "At iObject:" << iObject << " Computing ratio of " << h->GetName() << " by " << hRef->GetName() << endl;
      }
    ratioHistos(h,hRef,hRatio,correlatedUncertainties);
    append(hRatio);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Calculate square difference (bin by bin) of all  histograms of the other  collection relative to the
//! reference collection and store the result in the histograms of  this collection.
//! TProfile histograms have to be handled differently
//! This function is used for sub-sample analyses..
//! double sumWeights: sum of the weights of the n-1 collection accumulated in this collection
//!     double weight: weight of the current n-th collection.
//! The means are store in the BinContent
//! The rms are store in the BinError parts of the histograms.
//!
void HistogramCollection::squareDifferenceCollection(const HistogramCollection & collection, double sumWeights, double weight, int n)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!sameSizeAs(collection))
    {
    if (reportError(__FUNCTION__) )
      {
      cout << "Invalid operation on collections:"  << endl;
      cout << "Attempting to add collection named " << getName() << " containing " << size() << " histograms" << endl;
      cout << "with collection named " << collection.getName() << " containing " << collection.size() << " histograms" << endl;
      }
    return;
    }
  for (unsigned int iObject=0; iObject<size(); iObject++)
    {
    TH1* hAvg = objects[iObject];
    TH1* h    = collection.objects[iObject];
    if (!hAvg || !h)
      {
      if (reportWarning() )
        cout << " Histogram null pointers detected at iObject:" << iObject << endl;
      continue;
      }
    if (!sameDimensions(__FUNCTION__,hAvg,h)) return;
    if (reportDebug() )
      {
      cout << "At iObject:" << iObject << " Computing square difference of  histogram " << hAvg->GetName() << " and histogram " << h->GetName() << endl;
      }
    squareDifferenceHistos(hAvg, h, sumWeights, weight, n);
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void HistogramCollection::squareDifferenceHistos(TH1 *hAvg, TH1 *h, double sumWeights, double weight, int n)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,hAvg,h)) return;
  double vAvg, evAvg, evSqAvg;
  double v,  dv;
  double sum = weight + sumWeights;
  double rwn = weight/sum;
  double rw  = sumWeights/sum;
  double sqrtN = sqrt(n);
  if (h->IsA()==TH1::Class() || h->IsA()==TH1F::Class() || h->IsA()==TH1D::Class() || h->IsA()==TH1I::Class() )
    {
    int nx = hAvg->GetNbinsX();
    for (int ix=1; ix<=nx; ix++)
      {
      vAvg    = hAvg->GetBinContent(ix);
      evAvg   = hAvg->GetBinError(ix);
      v       = h->GetBinContent(ix);
      evSqAvg = evAvg*evAvg;
      dv      = v - vAvg;
      vAvg    += rwn*dv;
      evSqAvg = rw*(evSqAvg + dv*dv*rwn);
      evAvg   = n>0 ? sqrt(evSqAvg)/sqrtN : sqrt(evSqAvg);
      hAvg->SetBinContent(ix, vAvg);
      hAvg->SetBinError(ix, evAvg);
      }
    }
  else if (h->IsA()==TH2::Class() || h->IsA()==TH2F::Class() || h->IsA()==TH2D::Class() || h->IsA()==TH2I::Class() )
    {
    int nx = hAvg->GetNbinsX();
    int ny = hAvg->GetNbinsY();
    for (int iy=1; iy<=ny; iy++)
      {
      for (int ix=1; ix<=nx; ix++)
        {
        vAvg    = hAvg->GetBinContent(ix,iy);
        evAvg   = hAvg->GetBinError(ix,iy);
        v       = h->GetBinContent(ix,iy);
        evSqAvg = evAvg*evAvg;
        dv      = v - vAvg;
        vAvg    += rwn*dv;
        evSqAvg = rw*(evSqAvg + dv*dv*rwn);
        evAvg   = n>0 ? sqrt(evSqAvg)/sqrtN : sqrt(evSqAvg);
        hAvg->SetBinContent(ix,iy, vAvg);
        hAvg->SetBinError(ix,iy,evAvg);
        }
      }
    }
  else if (h->IsA()==TH3::Class() || h->IsA()==TH3F::Class() || h->IsA()==TH3D::Class() || h->IsA()==TH3I::Class() )
    {
    int nx = hAvg->GetNbinsX();
    int ny = hAvg->GetNbinsY();
    int nz = hAvg->GetNbinsZ();
    for (int iz=1; iz<=nz; iz++)
      {
      for (int iy=1; iy<=ny; iy++)
        {
        for (int ix=1; ix<=nx; ix++)
          {
          vAvg    = hAvg->GetBinContent(ix,iy,iz);
          evAvg   = hAvg->GetBinError(ix,iy,iz);
          v       = h->GetBinContent(ix,iy,iz);
          evSqAvg = evAvg*evAvg;
          dv      = v - vAvg;
          vAvg    += rwn*dv;
          evSqAvg = rw*(evSqAvg + dv*dv*rwn);
          evAvg   = n>0 ? sqrt(evSqAvg)/sqrtN : sqrt(evSqAvg);
          hAvg->SetBinContent(ix,iy,iz, vAvg);
          hAvg->SetBinError(ix,iy,iz,evAvg);
          }
        }
      }
    }
  else if (h->IsA()==TProfile::Class() )
    {
    int nx = hAvg->GetNbinsX();
    for (int ix=1; ix<=nx; ix++)
      {
      vAvg  = hAvg->GetBinContent(ix);
      evAvg = hAvg->GetBinError(ix);
      v     = h->GetBinContent(ix);
      evSqAvg = evAvg*evAvg;
      evAvg   = n>0 ? sqrt(evSqAvg)/sqrtN : sqrt(evSqAvg);
      dv      = v - vAvg;
      vAvg    += rwn*dv;
      evSqAvg = rw*(evSqAvg + dv*dv*rwn);
      evAvg   = n>0 ? sqrt(evSqAvg)/sqrtN : sqrt(evSqAvg);
      hAvg->SetBinContent(ix, vAvg);
      hAvg->SetBinError(ix,evAvg);
      ((TProfile*)hAvg)->SetBinEntries(ix, 1.0);
      }
    }
  else if (h->IsA()==TProfile2D::Class()  )
    {
    int nx = hAvg->GetNbinsX();
    int ny = hAvg->GetNbinsY();
    for (int iy=1; iy<=ny; iy++)
      {
      for (int ix=1; ix<=nx; ix++)
        {
        vAvg  = hAvg->GetBinContent(ix,iy);
        evAvg = hAvg->GetBinError(ix,iy);
        v     = h->GetBinContent(ix,iy);
        evSqAvg = evAvg*evAvg;
        dv      = v - vAvg;
        vAvg    += rwn*dv;
        evSqAvg = rw*(evSqAvg + dv*dv*rwn);
        evAvg   = n>0 ? sqrt(evSqAvg)/sqrtN : sqrt(evSqAvg);
        hAvg->SetBinContent(ix,iy, vAvg);
        hAvg->SetBinError(ix,iy,evAvg);
        //((TProfile2D*)hAvg)->SetBinEntries(ix,iy, 1.0);
        }
      }
    }
  if (reportEnd() )
    ;
}


void HistogramCollection::differenceHistos(TH1 *h, TH1 *hRef, TH1 *hDiff, bool correlatedUncertainties)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h,hRef,hDiff)) return;
  
  double v,  ev;
  double vRef,  evRef;
  double vDiff, evDiff, evDiffSq;
  
  if (h->IsA()==TH1::Class() || h->IsA()==TH1F::Class() || h->IsA()==TH1D::Class() || h->IsA()==TH1I::Class() )
    {
    int nx = h->GetNbinsX();
    for (int ix=1; ix<=nx; ix++)
      {
      v        = h->GetBinContent(ix);
      ev       = h->GetBinError(ix);
      vRef     = hRef->GetBinContent(ix);
      evRef    = hRef->GetBinError(ix);
      vDiff    = v - vRef;
      evDiffSq = correlatedUncertainties ?  TMath::Abs(ev*ev - evRef*evRef) : ev*ev + evRef*evRef;
      evDiff   = TMath::Sqrt(evDiffSq);
      hDiff->SetBinContent(ix, vDiff);
      hDiff->SetBinError(ix, evDiff);
      }
    }
  else if (h->IsA()==TH2::Class() || h->IsA()==TH2F::Class() || h->IsA()==TH2D::Class() || h->IsA()==TH2I::Class() )
    {
    int nx = h->GetNbinsX();
    int ny = h->GetNbinsY();
    for (int iy=1; iy<=ny; iy++)
      {
      for (int ix=1; ix<=nx; ix++)
        {
        v        = h->GetBinContent(ix,iy);
        ev       = h->GetBinError(ix,iy);
        vRef     = hRef->GetBinContent(ix,iy);
        evRef    = hRef->GetBinError(ix,iy);
        vDiff    = v - vRef;
        evDiffSq = correlatedUncertainties ?  TMath::Abs(ev*ev - evRef*evRef) : ev*ev + evRef*evRef;
        evDiff   = TMath::Sqrt(evDiffSq);
        hDiff->SetBinContent(ix,iy, vDiff);
        hDiff->SetBinError(ix,iy, evDiff);
        }
      }
    }
  else if (h->IsA()==TH3::Class() || h->IsA()==TH3F::Class() || h->IsA()==TH3D::Class() || h->IsA()==TH3I::Class() )
    {
    int nx = h->GetNbinsX();
    int ny = h->GetNbinsY();
    int nz = h->GetNbinsZ();
    for (int iz=1; iz<=nz; iz++)
      {
      for (int iy=1; iy<=ny; iy++)
        {
        for (int ix=1; ix<=nx; ix++)
          {
          v        = h->GetBinContent(ix,iy,iz);
          ev       = h->GetBinError(ix,iy,iz);
          vRef     = hRef->GetBinContent(ix,iy,iz);
          evRef    = hRef->GetBinError(ix,iy,iz);
          vDiff    = v - vRef;
          evDiffSq = correlatedUncertainties ?  TMath::Abs(ev*ev - evRef*evRef) : ev*ev + evRef*evRef;
          evDiff   = TMath::Sqrt(evDiffSq);
          hDiff->SetBinContent(ix,iy,iz, vDiff);
          hDiff->SetBinError(ix,iy,iz, evDiff);
          }
        }
      }
    }
  else if (h->IsA()==TProfile::Class() )
    {
    int nx = h->GetNbinsX();
    for (int ix=1; ix<=nx; ix++)
      {
      v        = h->GetBinContent(ix);
      ev       = h->GetBinError(ix);
      vRef     = hRef->GetBinContent(ix);
      evRef    = hRef->GetBinError(ix);
      vDiff    = v - vRef;
      evDiffSq = correlatedUncertainties ?  TMath::Abs(ev*ev - evRef*evRef) : ev*ev + evRef*evRef;
      evDiff   = TMath::Sqrt(evDiffSq);
      hDiff->SetBinContent(ix, vDiff);
      hDiff->SetBinError(ix, evDiff);
      ((TProfile*)hDiff)->SetBinEntries(ix, 1.0);
      }
    }
  else if (h->IsA()==TProfile2D::Class()  )
    {
    int nx = h->GetNbinsX();
    int ny = h->GetNbinsY();
    for (int iy=1; iy<=ny; iy++)
      {
      for (int ix=1; ix<=nx; ix++)
        {
        v        = h->GetBinContent(ix,iy);
        ev       = h->GetBinError(ix,iy);
        vRef     = hRef->GetBinContent(ix,iy);
        evRef    = hRef->GetBinError(ix,iy);
        vDiff    = v - vRef;
        evDiffSq = correlatedUncertainties ?  TMath::Abs(ev*ev - evRef*evRef) : ev*ev + evRef*evRef;
        evDiff   = TMath::Sqrt(evDiffSq);
        hDiff->SetBinContent(ix,iy, vDiff);
        hDiff->SetBinError(ix,iy, evDiff);
        //((TProfile*)hDiff)->SetBinEntries(ix,iy, 1.0);
        }
      }
    }
  if (reportEnd() )
    ;
}


void HistogramCollection::ratioHistos(TH1 *h, TH1 *hRef, TH1 *hRatio, bool correlatedUncertainties)
{

  if (!ptrExist(__FUNCTION__,h,hRef,hRatio)) return;
  
  double v,  ev, rev;
  double vRef,  evRef, revRef;
  double vRatio, evRatio, revRatio, revRatioSq;

  if (h->IsA()==TH1::Class() || h->IsA()==TH1F::Class() || h->IsA()==TH1D::Class() || h->IsA()==TH1I::Class() )
    {
    int nx = h->GetNbinsX();
    for (int ix=1; ix<=nx; ix++)
      {
      v        = h->GetBinContent(ix);
      ev       = h->GetBinError(ix);
      rev      = (v!=0) ? ev/v : 0.0;
      vRef     = hRef->GetBinContent(ix);
      evRef    = hRef->GetBinError(ix);
      revRef   = (vRef!=0) ? evRef/vRef : 0.0;
      if (vRef)
        {
        vRatio     = v/vRef;
        revRatioSq = correlatedUncertainties ? TMath::Abs(rev*rev - revRef*revRef) : rev*rev + revRef*revRef;
        revRatio   = TMath::Sqrt(revRatioSq);
        evRatio    = revRatio*vRatio;
        hRatio->SetBinContent(ix, vRatio);
        hRatio->SetBinError(ix, evRatio);
        }
      }
    }
  else if (h->IsA()==TH2::Class() || h->IsA()==TH2F::Class() || h->IsA()==TH2D::Class() || h->IsA()==TH2I::Class() )
    {
    int nx = h->GetNbinsX();
    int ny = h->GetNbinsY();
    for (int iy=1; iy<=ny; iy++)
      {
      for (int ix=1; ix<=nx; ix++)
        {
        v        = h->GetBinContent(ix,iy);
        ev       = h->GetBinError(ix,iy);
        rev      = (v!=0) ? ev/v : 0.0;
        vRef     = hRef->GetBinContent(ix,iy);
        evRef    = hRef->GetBinError(ix,iy);
        revRef   = (vRef!=0) ? evRef/vRef : 0.0;
        if (vRef)
          {
          vRatio     = v/vRef;
          revRatioSq = correlatedUncertainties ? TMath::Abs(rev*rev - revRef*revRef) : rev*rev + revRef*revRef;
          revRatio   = TMath::Sqrt(revRatioSq);
          evRatio    = revRatio*vRatio;
          hRatio->SetBinContent(ix,iy, vRatio);
          hRatio->SetBinError(ix,iy, evRatio);
          }
        }
      }
    }
  else if (h->IsA()==TH3::Class() || h->IsA()==TH3F::Class() || h->IsA()==TH3D::Class() || h->IsA()==TH3I::Class() )
    {
    int nx = h->GetNbinsX();
    int ny = h->GetNbinsY();
    int nz = h->GetNbinsZ();
    for (int iz=1; iz<=nz; iz++)
      {
      for (int iy=1; iy<=ny; iy++)
        {
        for (int ix=1; ix<=nx; ix++)
          {
          v        = h->GetBinContent(ix,iy,iz);
          ev       = h->GetBinError(ix,iy,iz);
          rev      = (v!=0) ? ev/v : 0.0;
          vRef     = hRef->GetBinContent(ix,iy,iz);
          evRef    = hRef->GetBinError(ix,iy,iz);
          revRef   = (vRef!=0) ? evRef/vRef : 0.0;
          if (vRef)
            {
            vRatio     = v/vRef;
            revRatioSq = correlatedUncertainties ? TMath::Abs(rev*rev - revRef*revRef) : rev*rev + revRef*revRef;
            revRatio   = TMath::Sqrt(revRatioSq);
            evRatio    = revRatio*vRatio;
            hRatio->SetBinContent(ix,iy,iz, vRatio);
            hRatio->SetBinError(ix,iy,iz, evRatio);
            }
          }
        }
      }
    }
  else if (h->IsA()==TProfile::Class() )
    {
    int nx = h->GetNbinsX();
    for (int ix=1; ix<=nx; ix++)
      {
      v        = h->GetBinContent(ix);
      ev       = h->GetBinError(ix);
      rev      = (v!=0) ? ev/v : 0.0;
      vRef     = hRef->GetBinContent(ix);
      evRef    = hRef->GetBinError(ix);
      revRef   = (vRef!=0) ? evRef/vRef : 0.0;
      if (vRef)
        {
        vRatio     = v/vRef;
        revRatioSq = correlatedUncertainties ? TMath::Abs(rev*rev - revRef*revRef) : rev*rev + revRef*revRef;
        revRatio   = TMath::Sqrt(revRatioSq);
        evRatio    = revRatio*vRatio;
        hRatio->SetBinContent(ix, vRatio);
        hRatio->SetBinError(ix, evRatio);
        ((TProfile*)hRatio)->SetBinEntries(ix, 1.0);
        }
      }
    }
  else if (h->IsA()==TProfile2D::Class()  )
    {
    int nx = h->GetNbinsX();
    int ny = h->GetNbinsY();
    for (int iy=1; iy<=ny; iy++)
      {
      for (int ix=1; ix<=nx; ix++)
        {
        v        = h->GetBinContent(ix,iy);
        ev       = h->GetBinError(ix,iy);
        rev      = (v!=0) ? ev/v : 0.0;
        vRef     = hRef->GetBinContent(ix,iy);
        evRef    = hRef->GetBinError(ix,iy);
        revRef   = (vRef!=0) ? evRef/vRef : 0.0;
        if (vRef)
          {
          vRatio     = v/vRef;
          revRatioSq = correlatedUncertainties ? TMath::Abs(rev*rev - revRef*revRef) : rev*rev + revRef*revRef;
          revRatio   = TMath::Sqrt(revRatioSq);
          evRatio    = revRatio*vRatio;
          hRatio->SetBinContent(ix, vRatio);
          hRatio->SetBinError(ix, evRatio);
          //((TProfile*)hRatio)->SetBinEntries(ix,iy, 1.0);
          }
        }
      }
    }
}


//Calculate External Product h_1 x h_2 and store into n1n1_12
//Verify if dimensions are compatible, aborts if they are not.
//The output is a 1D histogram packed according to the size of h_1 and h_2.
double HistogramCollection::calculateN1N1(const TH1 * h_1, const TH1 * h_2, TH1 * h_12, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_1,h_2,h_12)) return -1.0;
  int n1  = h_1->GetNbinsX();
  int n2  = h_2->GetNbinsX();
  int n3  = h_12->GetNbinsX();
  if (n3 != n1*n2 )
    {
    if (reportDebug(__FUNCTION__))
      {
      cout << "Incompatible histogram dimensions" << endl;
      cout << "H1: " << h_1->GetName()  << " nBins:" << n1 << endl;
      cout << "H2: " << h_2->GetName()  << " nBins:" << n2 << endl;
      cout << "H3: " << h_12->GetName() << " nBins:" << n3  << endl;
      }
    return -1;
    }
  double v1,ev1,v2,ev2,v,ev, r1,r2;
  double sum = 0.;
  double norm = 0.;
  int i3 = 1;
  for (int i1=1; i1<=n1;++i1)
    {
    v1  = a1*h_1->GetBinContent(i1);
    ev1 = a1*h_1->GetBinError(i1);
    for (int i2=1;i2<=n2;++i2)
      {
      v2  = a2*h_2->GetBinContent(i2);
      ev2 = a2*h_2->GetBinError(i2);
      v = v1*v2;
      if (v>0)
        {
        r1 = ev1/v1; r2 = ev2/v2;
        ev = v*sqrt(r1*r1+r2*r2);
        }
      else
        ev = 0.;
      h_12->SetBinContent(i3,v);
      h_12->SetBinError(i3,ev);
      sum  += v;
      norm += 1.;
      ++i3;
      }
    }
  //return average across bins
  return sum/norm;
}



//Calculate External Product n1_1 x n1_2 and store into n1n1_12
double HistogramCollection::calculateN1N1_H1H1H2(const TH1 * h_1, const TH1 * h_2, TH2 * h_12, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_1,h_2,h_12)) return -1.0;
  int n1   = h_1->GetNbinsX();
  int n2   = h_2->GetNbinsX();
  int n3x  = h_12->GetNbinsX();
  int n3y  = h_12->GetNbinsY();

  if (n3x!=n1 || n3y!=n2)
    {
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << "Incompatible histo dimensions" << endl;
      cout << "H1: " << h_1->GetName()    << " nBins:" << n1 << endl;
      cout << "H2: " << h_2->GetName()    << " nBins:" << n2 << endl;
      cout << "H3: " << h_12->GetName()   << " nBins_x:" << n3x << " nBins_y:" << n3y << endl;
      return -1.0;
      }
    }
  double v1,ev1,v2,ev2,v,ev, r1,r2;
  double sum  = 0.;
  double norm = 0.;
  for (int i1=1; i1<=n1; ++i1)
    {
    v1  = a1*h_1->GetBinContent(i1);
    ev1 = a1*h_1->GetBinError(i1);
    for (int i2=1; i2<=n2; ++i2)
      {
      v2  = a2*h_2->GetBinContent(i2);
      ev2 = a2*h_2->GetBinError(i2);
      v = v1*v2;
      if (v>0)
        {
        r1 = ev1/v1;
        r2 = ev2/v2;
        ev = v*sqrt(r1*r1+r2*r2);
        }
      else
        ev = 0.;
      h_12->SetBinContent(i1,i2,v);
      h_12->SetBinError(i1,i2,ev);
      sum  += v;
      norm += 1.;
      }
    }
  //return average across bins
  return sum/norm;
}

//Calculate External Product n1_1 x n1_2 and store into n1n1_12
double HistogramCollection::calculateN1N1_H2H2H2(const TH2 *h_1, const TH2 * h_2, TH2 * h_12, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_1,h_2,h_12)) return -1.0;
  int n1x = h_1->GetNbinsX();
  int n1y = h_1->GetNbinsY();
  int n2x = h_2->GetNbinsX();
  int n2y = h_2->GetNbinsY();
  int n3x = h_12->GetNbinsX();
  int n3y = h_12->GetNbinsY();
  if (n3x!=(n1x*n1y) || n3y!=(n2x*n2y) )
    {
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << "Incompatible histo dimensions" << endl;
      cout << "H1: " << h_1->GetName()  << " nBins_x:" << n1x << " nBins_y:" << n1y << endl;
      cout << "H2: " << h_2->GetName()  << " nBins_x:" << n2x << " nBins_y:" << n2y << endl;
      cout << "H3: " << h_12->GetName() << " nBins_x:" << n3x << " nBins_y:" << n3y << endl;
      return -1.;
      }
    }
  double v1,ev1,v2,ev2,v,ev, r1,r2;
  double sum  = 0.;
  double norm = 0.;
  for (int i1x=0;i1x<n1x;++i1x)
    {
    for (int i1y=0;i1y<n1y;++i1y)
      {
      v1  = a1*h_1->GetBinContent(i1x+1,i1y+1);
      ev1 = a1*h_1->GetBinError(i1x+1,i1y+1);
      for (int i2x=0;i2x<n2x;++i2x)
        {
        for (int i2y=0;i2y<n2y;++i2y)
          {
          v2  = a2*h_2->GetBinContent(i2x+1,i2y+1);
          ev2 = a2*h_2->GetBinError(i2x+1,i2y+1);
          v = v1*v2;
          if (v>0)
            {
            r1 = ev1/v1;
            r2 = ev2/v2;
            ev = v*sqrt(r1*r1+r2*r2);
            }
          else
            ev = 0.;
          int i3x = i1x*n1y+i1y+1;
          int i3y = i2x*n2y+i2y+1;
          h_12->SetBinContent(i3x,i3y,v);
          h_12->SetBinError(i3x,i3y,ev);
          sum += v;
          norm = 1.;
          }
        }
      }
    }
  //return average across bins
  return sum/norm;
}


//External Product N1N1N1
double HistogramCollection::calculateN1N1N1(const TH1 * h_1, const TH1 * h_2, const TH1 * h_3, TH1 * h_123)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_1,h_2,h_3,h_123)) return -1.0;
  int n = h_1->GetNbinsX();
  double v1,ev1,v2,ev2,v3,ev3,v,ev,r1,r2,r3;
  double sum = 0.;
  int i4 = 1;
  for (int i1=1;i1<=n;++i1)
    {
    v1  = h_1->GetBinContent(i1);
    ev1 = h_1->GetBinError(i1);
    for (int i2=1;i2<=n;++i2)
      {
      v2  = h_2->GetBinContent(i2);
      ev2 = h_2->GetBinError(i2);
      for (int i3=1;i3<=n;++i3)
        {
        v3  = h_3->GetBinContent(i3);
        ev3 = h_3->GetBinError(i3);
        v = v1*v2*v3;
        if (v>0)
          {
          r1 = ev1/v1;
          r2 = ev2/v2;
          r3 = ev3/v3;
          ev = v*sqrt(r1*r1+r2*r2+r3*r3);
          }
        else
          ev = 0.;
        h_123->SetBinContent(i4,v);
        h_123->SetBinError(i4,ev);
        sum += v;
        ++i4;
        }
      }
    }
  //return average across bins
  return sum/n/n/n;
}


//External Product N1N1N1
double HistogramCollection::calculateN1N1N1(const TH1 * h_1, const TH1 * h_2, const TH1 * h_3, TH3 * h_123)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_1,h_2,h_3,h_123)) return -1.0;
  int n = h_1->GetNbinsX();
  double v1,ev1,v2,ev2,v3,ev3,v,ev,r1,r2,r3;
  double sum = 0.;
  for (int i1=1;i1<=n;++i1)
    {
    v1  = h_1->GetBinContent(i1);
    ev1 = h_1->GetBinError(i1);
    for (int i2=1;i2<=n;++i2)
      {
      v2  = h_2->GetBinContent(i2);
      ev2 = h_2->GetBinError(i2);
      for (int i3=1;i3<=n;++i3)
        {
        v3  = h_3->GetBinContent(i3);
        ev3 = h_3->GetBinError(i3);
        v = v1*v2*v3;
        if (v>0)
          {
          r1 = ev1/v1;
          r2 = ev2/v2; r3 = ev3/v3;
          ev = v*sqrt(r1*r1+r2*r2+r3*r3);
          }
        else
          ev = 0.;
        h_123->SetBinContent(i1,i2,i3,v);
        h_123->SetBinError(i1,i2,i3,ev);
        sum += v;
        }
      }
    }
  //return average across bins
  return sum/n/n/n;
}


void HistogramCollection::calculateN2N1(const TH2 * s2, const TH1* s1, TH2 * target, int single)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,s2,s1,target)) return;
  int n = s2->GetNbinsX();
  double v1,v2,v3,ev1,ev2,ev3,v,ev,r1,r2;
  int dPhi_xy;
  int dPhi_xz;
  int dPhi_yz;
  int dPhi12;
  int dPhi13;
  for (int i_x=0;i_x<n; ++i_x)
    {
    int i_x1 = i_x+1;
    for (int i_y=0;i_y<n; ++i_y)
      {
      int i_y1 = i_y+1;
      dPhi_xy = i_x-i_y; if (dPhi_xy<0) dPhi_xy += n; dPhi_xy+=1;
      for (int iz=0;iz<n; ++iz)
        {
        int iz1 = iz+1;
        dPhi_xz = i_x-iz; if (dPhi_xz<0) dPhi_xz += n; dPhi_xz+=1;
        dPhi_yz = i_y-iz; if (dPhi_yz<0) dPhi_yz += n; dPhi_yz+=1;
        v1   = s2->GetBinContent(i_x1,i_y1);
        ev1  = s2->GetBinError(i_x1,i_y1);
        v2   = s1->GetBinContent(iz1);
        ev2  = s1->GetBinError(iz1);
        switch (single)
          {
            case 3: dPhi12 = dPhi_xy; dPhi13 = dPhi_xz; break;
            case 2: dPhi13 = dPhi_xy; dPhi12 = dPhi_xz; break;
            case 1: dPhi12 = dPhi_xz; dPhi13 = dPhi_yz; break;
            default: return;//error("calculateN2N1()") << "invalid argument"<< endl; return;
          }
        v3   = target->GetBinContent(dPhi12,dPhi13);
        ev3  = target->GetBinError(dPhi12,dPhi13);
        v = v1*v2;
        if (v>0)
          {
          r1 = ev1/v1;
          r2 = ev2/v2;
          ev = v*v*(r1*r1+r2*r2);
          }
        else
          ev = 0.;
        v = v+v3;
        ev = sqrt(ev3*ev3+ev);
        target->SetBinContent(dPhi12,dPhi13,v);
        target->SetBinError(dPhi12,dPhi13,ev);
        }
      }
    }
}


//!
//! Fold s2 and s1 into target
//!
void HistogramCollection::calculateN2N1x(const TH2 * s2, const TH1* s1, TH3 * target, int single)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,s2,s1,target)) return;
  int n = s2->GetNbinsX();
  double v1,v2,v3,ev1,ev2,ev3,v,ev,r1,r2;
  for (int i_x=0;i_x<n; ++i_x)
    {
    int i_x1 = i_x+1;
    for (int i_y=0;i_y<n; ++i_y)
      {
      int i_y1 = i_y+1;
      for (int iz=0;iz<n; ++iz)
        {
        int iz1 = iz+1;
        v1   = s2->GetBinContent(i_x1,i_y1);
        ev1  = s2->GetBinError(i_x1,i_y1);
        v2   = s1->GetBinContent(iz1);
        ev2  = s1->GetBinError(iz1);

        v = v1*v2;
        if (v>0)
          {
          r1 = ev1/v1;
          r2 = ev2/v2;
          ev = v*v*(r1*r1+r2*r2);
          }
        else
          ev = 0.;

        switch (single)
          {
            case 3:
            v3   = target->GetBinContent(i_x1,i_y1,iz1);
            ev3  = target->GetBinError(i_x1,i_y1,iz1);
            break;
            case 2:
            v3   = target->GetBinContent(i_x1,iz1,i_y1);
            ev3  = target->GetBinError(i_x1,iz1,i_y1);
            break;
            case 1:
            v3   = target->GetBinContent(iz1,i_x1,i_y1);
            ev3  = target->GetBinError(iz1,i_x1,i_y1);
            break;
            default:  //error("calculateN2N1") << "-E- calculateN2N1() invalid argument"<< endl;
            return;
          }

        v = v+v3;
        ev = sqrt(ev3*ev3+ev);
        switch (single)
          {
            case 3:
            target->SetBinContent(i_x1,i_y1,iz1,v);
            target->SetBinError(i_x1,i_y1,iz1,ev);
            break;
            case 2:
            target->SetBinContent(i_x1,iz1,i_y1,v);
            target->SetBinError(i_x1,iz1,i_y1,ev);
            break;
            case 1:
            target->SetBinContent(iz1,i_x1,i_y1,v);
            target->SetBinError(iz1,i_x1,i_y1,ev);
            break;
            default: if (reportDebug(__FUNCTION__)) cout << endl << "invalid argument"<< endl; return;
          }
        }
      }
    }
}


//!
//! Check whether the given histograms have the same dimension and size.
//!
int   HistogramCollection::getDimension(const TH1* h) const
{
  if (!ptrExist(__FUNCTION__,h)) return -1;
  int nDim = 0;
  if (h->IsA()==TH1::Class() || h->IsA()==TH1F::Class() || h->IsA()==TH1D::Class() || h->IsA()==TH1I::Class() )
    {
    nDim = 1;
    }
  else if (h->IsA()==TH2::Class() || h->IsA()==TH2F::Class() || h->IsA()==TH2D::Class() || h->IsA()==TH2I::Class() )
    {
    nDim = 2;
    }
  else if (h->IsA()==TH3::Class() || h->IsA()==TH3F::Class() || h->IsA()==TH3D::Class() || h->IsA()==TH3I::Class() )
    {
    nDim = 3;
    }
  return nDim;
}


//!
//! Check whether the given histograms have the same dimension and size.
//!
bool  HistogramCollection::sameDimensions(const TString & caller, const TH1* h1, const TH1* h2)  const
{
  if (!ptrExist(caller,h1,h2)) return false;
  int nDim1 =  getDimension(h1);
  int nDim2 =  getDimension(h2);
  if (nDim1 != nDim2)
    {
    if (reportError(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nDim:" << nDim1 << endl;
      cout << "h2: " << h2->GetName() << " nDim:" << nDim2 << endl;
      }
    return false;
    }
  if (h1->GetNbinsX()!=h2->GetNbinsX())
    {
    if (reportError(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << endl;
      }
    return false;
    }
  if (nDim1>1 && (h1->GetNbinsY()!=h2->GetNbinsY()))
    {
    if (reportError(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << " ny:" << h1->GetNbinsY() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << " ny:" << h2->GetNbinsY() << endl;
      }
    return false;
    }
  if (nDim1 > 2 && (h1->GetNbinsZ() != h2->GetNbinsZ() ))
    {
    if (reportError(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << " ny:" << h1->GetNbinsY() << " nz:" << h1->GetNbinsZ() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << " ny:" << h2->GetNbinsY() << " nz:" << h2->GetNbinsZ() << endl;
      }
    return false;
    }
  return true;
}


//!
//! Check whether the given histograms have the same dimension and size.
//!
bool  HistogramCollection::sameDimensions(const TString & caller, const TH1* h1, const TH1* h2, const TH1* h3)  const
{
  if (!ptrExist(caller,h1,h2,h3)) return false;
  int nDim1 =  getDimension(h1);
  int nDim2 =  getDimension(h2);
  int nDim3 =  getDimension(h3);
  if ( (nDim1 != nDim2) ||  (nDim1 != nDim3) )
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nDim:" << nDim1 << endl;
      cout << "h2: " << h2->GetName() << " nDim:" << nDim2 << endl;
      cout << "h3: " << h3->GetName() << " nDim:" << nDim3 << endl;
      }
    return false;
    }
  if ( (h1->GetNbinsX()!=h2->GetNbinsX()) || (h1->GetNbinsX()!=h3->GetNbinsX()) )
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << endl;
      }
    return false;
    }
  if (nDim1>1 && ( (h1->GetNbinsY()!=h2->GetNbinsY()) || (h1->GetNbinsY()!=h3->GetNbinsY()) ) )
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << " ny:" << h1->GetNbinsY() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << " ny:" << h2->GetNbinsY() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << " ny:" << h3->GetNbinsY() << endl;
      }
    return false;
    }
  if (nDim1 > 2 && ( (h1->GetNbinsZ()!=h2->GetNbinsZ()) || (h1->GetNbinsZ()!=h3->GetNbinsZ()) ) )
    {
    if (reportDebug(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << " ny:" << h1->GetNbinsY() << " nz:" << h1->GetNbinsZ() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << " ny:" << h2->GetNbinsY() << " nz:" << h2->GetNbinsZ() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << " ny:" << h3->GetNbinsY() << " nz:" << h3->GetNbinsZ() << endl;
      }
    return false;
    }
  return true;
}


//!
//! Check whether the given histograms have the same dimension and size.
//!
bool  HistogramCollection::sameDimensions(const TString & caller, const TH1* h1, const TH1* h2, const TH1* h3, const TH1* h4) const
{
  if (!ptrExist(caller,h1,h2,h3,h4)) return false;
  int nDim1 =  getDimension(h1);
  int nDim2 =  getDimension(h2);
  int nDim3 =  getDimension(h3);
  int nDim4 =  getDimension(h4);
  if ( (nDim1 != nDim2) ||  (nDim1 != nDim3) ||  (nDim1 != nDim4) )
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nDim:" << nDim1 << endl;
      cout << "h2: " << h2->GetName() << " nDim:" << nDim2 << endl;
      cout << "h3: " << h3->GetName() << " nDim:" << nDim3 << endl;
      cout << "h4: " << h4->GetName() << " nDim:" << nDim4 << endl;
      }
    return false;
    }
  if ( (h1->GetNbinsX()!=h2->GetNbinsX()) || (h1->GetNbinsX()!=h3->GetNbinsX()) || (h1->GetNbinsX()!=h4->GetNbinsX()) )
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << endl;
      cout << "h4: " << h4->GetName() << " nx:" << h4->GetNbinsX() << endl;
      }
    return false;
    }
  if (nDim1>1 && ( (h1->GetNbinsY()!=h2->GetNbinsY()) || (h1->GetNbinsY()!=h3->GetNbinsY()) || (h1->GetNbinsY()!=h4->GetNbinsY()) ) )
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << " ny:" << h1->GetNbinsY() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << " ny:" << h2->GetNbinsY() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << " ny:" << h3->GetNbinsY() << endl;
      cout << "h4: " << h4->GetName() << " nx:" << h4->GetNbinsX() << " ny:" << h4->GetNbinsY() << endl;
      }
    return false;
    }
  if (nDim1 > 2 && ( (h1->GetNbinsZ()!=h2->GetNbinsZ()) || (h1->GetNbinsZ()!=h3->GetNbinsZ()) || (h1->GetNbinsZ()!=h4->GetNbinsZ()) ) )
    {
    if (reportDebug(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << " ny:" << h1->GetNbinsY() << " nz:" << h1->GetNbinsZ() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << " ny:" << h2->GetNbinsY() << " nz:" << h2->GetNbinsZ() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << " ny:" << h3->GetNbinsY() << " nz:" << h3->GetNbinsZ() << endl;
      cout << "h4: " << h4->GetName() << " nx:" << h4->GetNbinsX() << " ny:" << h4->GetNbinsY() << " nz:" << h4->GetNbinsZ() << endl;
      }
    return false;
    }
  return true;
}

//!
//! Check whether the given histograms have the same dimension and size.
//!
bool  HistogramCollection::sameDimensions(const TString & caller, const TH1* h1, const TH1* h2, const TH1* h3, const TH1* h4, const TH1* h5)  const
{
  if (!ptrExist(caller,h1,h2,h3,h4,h5)) return false;
  int nDim1 =  getDimension(h1);
  int nDim2 =  getDimension(h2);
  int nDim3 =  getDimension(h3);
  int nDim4 =  getDimension(h4);
  int nDim5 =  getDimension(h5);
  if ( (nDim1 != nDim2) ||  (nDim1 != nDim3) ||  (nDim1 != nDim4) ||  (nDim1 != nDim5) )
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nDim:" << nDim1 << endl;
      cout << "h2: " << h2->GetName() << " nDim:" << nDim2 << endl;
      cout << "h3: " << h3->GetName() << " nDim:" << nDim3 << endl;
      cout << "h4: " << h4->GetName() << " nDim:" << nDim4 << endl;
      cout << "h5: " << h5->GetName() << " nDim:" << nDim5 << endl;
      }
    return false;
    }
  if ( (h1->GetNbinsX()!=h2->GetNbinsX()) || (h1->GetNbinsX()!=h3->GetNbinsX()) || (h1->GetNbinsX()!=h4->GetNbinsX())  || (h1->GetNbinsX()!=h5->GetNbinsX()))
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << endl;
      cout << "h4: " << h4->GetName() << " nx:" << h4->GetNbinsX() << endl;
      cout << "h5: " << h5->GetName() << " nx:" << h5->GetNbinsX() << endl;
      }
    return false;
    }
  if (nDim1>1 && ( (h1->GetNbinsY()!=h2->GetNbinsY()) || (h1->GetNbinsY()!=h3->GetNbinsY()) || (h1->GetNbinsY()!=h4->GetNbinsY())  || (h1->GetNbinsY()!=h5->GetNbinsY()) ) )
    {
    if (reportWarning(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << " ny:" << h1->GetNbinsY() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << " ny:" << h2->GetNbinsY() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << " ny:" << h3->GetNbinsY() << endl;
      cout << "h4: " << h4->GetName() << " nx:" << h4->GetNbinsX() << " ny:" << h4->GetNbinsY() << endl;
      cout << "h5: " << h5->GetName() << " nx:" << h5->GetNbinsX() << " ny:" << h5->GetNbinsY() << endl;
      }
    return false;
    }
  if (nDim1 > 2 && ( (h1->GetNbinsZ()!=h2->GetNbinsZ()) || (h1->GetNbinsZ()!=h3->GetNbinsZ()) || (h1->GetNbinsZ()!=h4->GetNbinsZ()) || (h1->GetNbinsZ()!=h5->GetNbinsZ()) ) )
    {
    if (reportDebug(caller))
      {
      cout << endl;
      cout << "h1: " << h1->GetName() << " nx:" << h1->GetNbinsX() << " ny:" << h1->GetNbinsY() << " nz:" << h1->GetNbinsZ() << endl;
      cout << "h2: " << h2->GetName() << " nx:" << h2->GetNbinsX() << " ny:" << h2->GetNbinsY() << " nz:" << h2->GetNbinsZ() << endl;
      cout << "h3: " << h3->GetName() << " nx:" << h3->GetNbinsX() << " ny:" << h3->GetNbinsY() << " nz:" << h3->GetNbinsZ() << endl;
      cout << "h4: " << h4->GetName() << " nx:" << h4->GetNbinsX() << " ny:" << h4->GetNbinsY() << " nz:" << h4->GetNbinsZ() << endl;
      cout << "h5: " << h5->GetName() << " nx:" << h5->GetNbinsX() << " ny:" << h5->GetNbinsY() << " nz:" << h5->GetNbinsZ() << endl;
      }
    return false;
    }
  return true;
}


//!
//! Calculate the G2 observable based on the given components
//!
void HistogramCollection::calculateDptDpt(const TH2 * spp, const TH2 * spn, const TH2 * snp, const TH2 * snn,
                                          const TH2 * avgp1, const TH2 * avgp2,  TH2 * s2dptdpt,  TH2 * dptdpt,
                                          bool ijNormalization, int nEta, int nPhi)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,spp,spn,snp,snn,avgp1,avgp2,s2dptdpt,dptdpt)) return;

  if (!sameDimensions(__FUNCTION__,spp,spn) || !sameDimensions(__FUNCTION__,spp,snp) || !sameDimensions(__FUNCTION__,spp,snn)) return;
  int nEtaPhi = nEta*nPhi;

  double sumPt1 = 0;
  double sumPt2 = 0;
  for (int iEta1=0;iEta1<nEta;++iEta1)
    {
    for (int iPhi1=0;iPhi1<nPhi;++iPhi1)
      {
      //k = iEta1*nPhi + iPhi1 + 1;
      sumPt1 += avgp1->GetBinContent(iEta1+1, iPhi1+1);
      sumPt2 += avgp2->GetBinContent(iEta1+1, iPhi1+1);
      }
    }
  double p1 = sumPt1/double(nEtaPhi);
  double p2 = sumPt2/double(nEtaPhi);
  double v1,v2,v3,v4,v5,v6;
  double ev4,ev5,ev6;


  for (int iEta1=0;iEta1<nEta;++iEta1)
    {
    //p1  = ptAvg1[iEta1];
    for (int iPhi1=0;iPhi1<nPhi;++iPhi1)
      {
      int k1 = 1+ iEta1*nPhi + iPhi1;
      //c1 = avgp1->GetBinContent(k1+1)/p1;
      for (int iEta2=0;iEta2<nEta;++iEta2)
        {
        //p2  = ptAvg2[iEta2];
        for (int iPhi2=0;iPhi2<nPhi;++iPhi2)
          {

          int k2 = 1+ iEta2*nPhi + iPhi2;
          v1  = spp->GetBinContent(k1,k2); //ev1 = spp->GetBinError(k1,k2);
          v2  = spn->GetBinContent(k1,k2); //ev2 = spn->GetBinError(k1,k2);
          v3  = snp->GetBinContent(k1,k2); //ev3 = snp->GetBinError(k1,k2);
          v4  = snn->GetBinContent(k1,k2); ev4 = snn->GetBinError(k1,k2);
          //c2 = avgp2->GetBinContent(k2+1)/p2;

          if (v4!=0 ) // && ev4/v4<0.5)
            {
            if (ijNormalization)
              {
              v5  = 2*(v1 - v2*p2 - p1*v3 + p1*p2*v4);
              ev5 = v5*ev4/v4;
              v6  = v5/(2*v4);
              ev6 = v6*ev4/v4;
              }
            else
              {
              v5  = v1 - v2*p2 - p1*v3 + p1*p2*v4;
              ev5 = v5*ev4/v4;
              v6  = v5/v4;
              ev6 = v6*ev4/v4;
              }
            }
          else
            {
            v5 = v6 = ev5 = ev6 = 0;
            }
          if (s2dptdpt)
            ;
          //s2dptdpt->SetBinContent(k,v5); s2dptdpt->SetBinError(k,ev5);
          //dptdpt->SetBinContent(k,  v6); dptdpt->SetBinError(k,  ev6);
          //s2dptdpt->SetBinContent(k1, k2, v5); s2dptdpt->SetBinError(k1, k2, ev5);
          dptdpt->SetBinContent(  k1, k2, v6); dptdpt->SetBinError(  k1, k2, ev6);
          }
        }
      }
    }

}

//!
//! Calculate the G2 observable based on the given components
//!
void HistogramCollection::calculateDptDpt(const TH2 * spp, const TH2 * spn, const TH2 * snp, const TH2 * snn,
                                          const TH1 * avgp1, const TH1 * avgp2,
                                          TH2 * dptdpt,
                                          bool ijNormalization, int nBins)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,spp,spn,snp,snn,avgp1,avgp2,dptdpt)) return;

  if (!sameDimensions(__FUNCTION__,spp,spn) || !sameDimensions(__FUNCTION__,spp,snp) || !sameDimensions(__FUNCTION__,spp,snn)) return;
  
  double v1,v2,v3,v4,v5,v6,p1,p2;
  double ev4,ev5,ev6;
  for (int i1=1;i1<=nBins;++i1)
    {
    p1 = avgp1->GetBinContent(i1);
    for (int i2=1;i2<=nBins;++i2)
      {
      p2 = avgp2->GetBinContent(i2);
      v1  = spp->GetBinContent(i1,i2); //ev1 = spp->GetBinError(i1,i2);
      v2  = spn->GetBinContent(i1,i2); //ev2 = spn->GetBinError(i1,i2);
      v3  = snp->GetBinContent(i1,i2); //ev3 = snp->GetBinError(i1,i2);
      v4  = snn->GetBinContent(i1,i2); ev4 = snn->GetBinError(i1,i2);
      if (v4!=0) // && ev4/v4<0.5)
        {
        if (ijNormalization)
          {
          v5  = 2*(v1 - v2*p2 - p1*v3 + p1*p2*v4);
          ev5 = v5*ev4/v4;
          v6  = v5/(2*v4);
          ev6 = v6*ev4/v4;
          }
        else
          {
          v5  = v1 - v2*p2 - p1*v3 + p1*p2*v4;
          ev5 = v5*ev4/v4;
          v6  = v5/v4;
          ev6 = v6*ev4/v4;
          }
        }
      else
        {
        v5 = v6 = ev5 = ev6 = 0;
        }
      dptdpt->SetBinContent(i1,i2, v6);
      dptdpt->SetBinError(i1,i2, ev6);
      }
    }

}


void HistogramCollection::calculateSc(const TH1 * spp, const TH1 * n1n1, const TH1 * pt1pt1, TH1 * g2, bool ijNormalization)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,spp,n1n1,pt1pt1,g2)) return;

  if (!sameDimensions(__FUNCTION__,spp,n1n1) || !sameDimensions(__FUNCTION__,spp,pt1pt1) || !sameDimensions(__FUNCTION__,spp,g2)) return;
  int n1 = spp->GetNbinsX();
  double v1,v2,v3,v4;
  double ev1,ev4;
  for (int i1=1;i1<=n1;++i1)
    {
    v1  = spp->GetBinContent(i1);    ev1 = spp->GetBinError(i1);
    v2  = n1n1->GetBinContent(i1);   //ev2 = n1n1->GetBinError(i1);
    v3  = pt1pt1->GetBinContent(i1); //ev3 = pt1pt1->GetBinError(i1);
    if (v2>0)
      {
      if (ijNormalization)
        v4  = 2*v1/v2 - v3;
      else
        v4  = v1/v2 - v3;
      ev4 = v1>0 ? v4*ev1/v1 : 0;
      }
    else
      {
      v4 = ev4 = 0;
      }
    g2->SetBinContent(i1,v4); g2->SetBinError(i1,ev4);
    }
}

void HistogramCollection::calculateG2_H2H2H2H2(const TH2 * spp, const TH2 * n1n1, const TH2 * pt1pt1, TH2 * g2, bool ijNormalization, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,spp,n1n1,pt1pt1,g2)) return;
  if (!sameDimensions(__FUNCTION__,spp,n1n1,pt1pt1,g2)) return;

  int n2x = n1n1->GetNbinsX();
  int n2y = n1n1->GetNbinsY();
  double v1,v2,v3,v4;
  double ev1,ev4;
  for (int i1=1;i1<=n2x;++i1)
    {
    for (int i2=1;i2<=n2y;++i2)
      {
      v1  = a1*spp->GetBinContent(i1,i2);       ev1 = a1*spp->GetBinError(i1,i2);
      v2  = a2*n1n1->GetBinContent(i1,i2);      //ev2 = a2*n1n1->GetBinError(i1,i2);
      v3  = pt1pt1->GetBinContent(i1,i2);       //ev3 = pt1pt1->GetBinError(i1,i2);
      if (v2>0)
        {
        if (ijNormalization)
          v4  = 2*v1/v2 - v3;
        else
          v4  = v1/v2 - v3;
        ev4 = v1>0 ? v4*ev1/v1 : 0;
        }
      else
        {
        v4 = ev4 = 0;
        }
      g2->SetBinContent(i1,i2,v4); g2->SetBinError(i1,i2,ev4);
      }
    }
}

/* calculate the balance functions components associated to the current pair */
/* independent of R2, two components are alway computed, for LS they will match but don't for US */
void HistogramCollection::calculateBf(const TH2 *n2, const TH2 *n1_1, const TH2 *n1_2, TH2 *bf_12, TH2 *bf_21)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,n2,n1_1,n1_2,bf_12,bf_21)) return;
  if (!sameDimensions(__FUNCTION__,n1_1,n1_2)) return;
  if (!sameDimensions(__FUNCTION__,n2,bf_12,bf_21)) return;
  int nbinsx = n2->GetNbinsX();
  int nbinsy = n2->GetNbinsY();

  bf_12->Reset();
  bf_21->Reset();
  double n11_inte = 0.0;
  double n11_int = n1_1->IntegralAndError(1,n1_1->GetNbinsX(),1,n1_1->GetNbinsY(),n11_inte);
  double n11_inter = n11_inte/n11_int;

  double n12_inte = 0.0;
  double n12_int = n1_2->IntegralAndError(1,n1_2->GetNbinsX(),1,n1_2->GetNbinsY(),n12_inte);
  double n12_inter = n12_inte/n12_int;

  for (int ix = 0; ix < nbinsx; ++ix)
    {
    for (int iy = 0; iy < nbinsy; ++iy)
      {
      double v = n2->GetBinContent(ix+1,iy+1);
      double ve = n2->GetBinError(ix+1,iy+1);

      double v12 = v/n12_int;
      double v12e = v12*TMath::Sqrt(ve/v*ve/v+n12_inter*n12_inter);
      double v21 = v/n11_int;
      double v21e = v21*TMath::Sqrt(ve/v*ve/v+n11_inter*n11_inter);

      bf_12->SetBinContent(ix+1,iy+1,v12);
      bf_12->SetBinError(ix+1,iy+1,v12e);
      bf_21->SetBinContent(iy+1,ix+1,v21);
      bf_21->SetBinError(iy+1,ix+1,v21e);
      }
    }
  bf_12->SetEntries(n2->GetEntries());
  bf_21->SetEntries(n2->GetEntries());
}

void HistogramCollection::calculateSean_H1H2H2H2(const TH1 * spp, const TH2 * n1n1, const TH2 * pt1pt1, TH2 * g2, bool ijNormalization, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,spp,n1n1,pt1pt1,g2)) return;
  if (!sameDimensions(__FUNCTION__,n1n1,pt1pt1,g2)) return;
  int n1  = spp->GetNbinsX();
  int n2x = n1n1->GetNbinsX();
  int n2y = n1n1->GetNbinsY();
  int n3x = pt1pt1->GetNbinsX();
  int n3y = pt1pt1->GetNbinsY();
  int n4x = g2->GetNbinsX();
  int n4y = g2->GetNbinsY();
  if (n1!=n2x*n2y || n2x!=n3x || n2x!=n4x)
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;
      cout << "Incompatible histogram sizes." << endl;
      cout << n1  << endl;
      cout << n2x << endl;
      cout << n2y << endl;
      cout << n3x << endl;
      cout << n3y << endl;
      cout << n4x << endl;
      cout << n4y << endl;
      }
    return;
    }
  int index;
  double v1,v2,v3,v4;
  double ev1, ev4;
  index = 1;
  for (int i1=1;i1<=n2x;++i1)
    {
    for (int i2=1;i2<=n2y;++i2)
      {
      v1  = a1*spp->GetBinContent(index);   ev1 = a1*spp->GetBinError(index);
      v2  = a2*n1n1->GetBinContent(i1,i2);  // ev2 = a2*n1n1->GetBinError(i1,i2);
      v3  = pt1pt1->GetBinContent(i1,i2);   // ev3 = pt1pt1->GetBinError(i1,i2);
      if (v2>0)
        {
        if (ijNormalization)
          v4  = 2*v1/v2 - v3;
        else
          v4  = v1/v2 - v3;
        ev4 = v1>0 ? v4*ev1/v1 : 0;
        }
      else
        {
        v4 = ev4 = 0;
        }
      g2->SetBinContent(i1,i2,v4); g2->SetBinError(i1,i2,ev4);
      index++;
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


int  HistogramCollection::calculateQ3DwPtPhiEta(double pt1, double phi1, double eta1,
                                                double pt2, double phi2, double eta2,
                                                double & Qlong, double & Qout, double & Qside)
{

  if (reportStart(__FUNCTION__))
    ;
  TLorentzVector p1;
  TLorentzVector p2;
  p1.SetPtEtaPhiM(pt1,eta1,phi1,0.13957);
  p2.SetPtEtaPhiM(pt2,eta2,phi2,0.13957);
  if (p1.P()>p1.E()) return 1;
  if (p2.P()>p2.E()) return 2;

  double px1   = p1.Px();
  double py1   = p1.Py();
  double pz1   = p1.Pz();
  double e1    = p1.E();

  double px2   = p2.Px();
  double py2   = p2.Py();
  double pz2   = p2.Pz();
  double e2    = p2.E();

  double pt,s,Mlong,roots;
  double ptot[4],q[4];
  //const int g[4]={1,-1,-1,-1};
  //int alpha;
  s=0.0;
  ptot[0] = e1  + e2;
  ptot[1] = px1 + px2;
  ptot[2] = py1 + py2;
  ptot[3] = pz1 + pz2;
  q[0] = e1  - e2;
  q[1] = px1 - px2;
  q[2] = py1 - py2;
  q[3] = pz1 - pz2;
  s = ptot[0]*ptot[0] - ptot[1]*ptot[1] - ptot[2]*ptot[2] - ptot[3]*ptot[3];
  pt=sqrt(ptot[1]*ptot[1]+ptot[2]*ptot[2]);
  Mlong=sqrt(s+pt*pt);
  roots=sqrt(s);

  if ((p1+p2).P()>(p1+p2).E()) return 3;

  if (pt>0)
    {
    Qside = (ptot[1]*q[2]-ptot[2]*q[1])/pt;
    Qlong = (ptot[0]*q[3]-ptot[3]*q[0])/Mlong;
    Qout  = (roots/Mlong)*(ptot[1]*q[1]+ptot[2]*q[2])/pt;
    }
  else
    {
    Qlong = q[3];
    Qside = q[2];
    Qout  = q[1];
    }

  return 0;
}

int  HistogramCollection::calculateQ3DwPtPhiY(double pt1, double phi1, double y1,
                                              double pt2, double phi2, double y2,
                                              double & Qlong, double & Qout, double & Qside)
{

  if (reportStart(__FUNCTION__))
    ;
  TLorentzVector p1;
  TLorentzVector p2;
  double mPart = 0.13957;
  double mSq   = mPart*mPart;
  double mt1   = sqrt(mSq + pt1*pt1);
  double p1z   = mt1 * sinh(y1);
  double mt2   = sqrt(mSq + pt2*pt2);
  double p2z   = mt2 * sinh(y2);
  p1.SetXYZM(pt1*cos(phi1),pt1*sin(phi1),p1z,mPart);
  p2.SetXYZM(pt2*cos(phi2),pt2*sin(phi2),p2z,mPart);

  if (p1.P()>p1.E()) return 1;
  if (p2.P()>p2.E()) return 2;

  double px1   = p1.Px();
  double py1   = p1.Py();
  double pz1   = p1.Pz();
  double e1    = p1.E();

  double px2   = p2.Px();
  double py2   = p2.Py();
  double pz2   = p2.Pz();
  double e2    = p2.E();

  double pt,s,Mlong,roots;
  double ptot[4],q[4];
  //const int g[4]={1,-1,-1,-1};
  //int alpha;
  s=0.0;
  ptot[0] = e1  + e2;
  ptot[1] = px1 + px2;
  ptot[2] = py1 + py2;
  ptot[3] = pz1 + pz2;
  q[0] = e1  - e2;
  q[1] = px1 - px2;
  q[2] = py1 - py2;
  q[3] = pz1 - pz2;
  s = ptot[0]*ptot[0] - ptot[1]*ptot[1] - ptot[2]*ptot[2] - ptot[3]*ptot[3];
  pt=sqrt(ptot[1]*ptot[1]+ptot[2]*ptot[2]);
  Mlong=sqrt(s+pt*pt);
  roots=sqrt(s);

  if ((p1+p2).P()>(p1+p2).E()) return 3;

  if (pt>0)
    {
    Qside = (ptot[1]*q[2]-ptot[2]*q[1])/pt;
    Qlong = (ptot[0]*q[3]-ptot[3]*q[0])/Mlong;
    Qout  = (roots/Mlong)*(ptot[1]*q[1]+ptot[2]*q[2])/pt;
    }
  else
    {
    Qlong = q[3];
    Qside = q[2];
    Qout  = q[1];
    }

  return 0;
}


// Calculate n1n1 Q3D based on h2(y,pt) x h2(y,pt)
void HistogramCollection::calculateN1N1H2H2_Q3D_MCY(TH2 * n1_1, TH2 * n1_2, TH3 * n1n1_Q3D, double a1 __attribute__((unused)), double a2 __attribute__((unused)))
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,n1_1,n1_2,n1n1_Q3D)) return;
  if (!sameDimensions(__FUNCTION__,n1_1,n1_2)) return;

  double Qlong, Qout, Qside;
  double pt1, phi1, y1;
  double pt2, phi2, y2;
  int nIter = 100000000;

  //double ss = 2.0/double(n1n1_Q3D->GetNbinsX());
  double avgN1 = n1_1->Integral();
  double avgN2 = n1_2->Integral();

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "   avgN1: " << avgN1 << endl;
    cout << "   avgN2: " << avgN2 << endl;
    }

  double scalingFactor = avgN1*avgN2/double(nIter);

  for (int k=0;k<nIter;k++)
    {
    n1_1->GetRandom2 (y1, pt1);
    n1_2->GetRandom2 (y2, pt2);
    phi1 = 2.0*TMath::Pi() * randomGenerator->Rndm();
    phi2 = 2.0*TMath::Pi() * randomGenerator->Rndm();
    bool status = calculateQ3DwPtPhiY(pt1, phi1, y1, pt2, phi2, y2, Qlong, Qout, Qside);
    if (status==0)
      {
      n1n1_Q3D->Fill(Qlong, Qside, Qout, 1.0);
      }
    }
  n1n1_Q3D->Scale(scalingFactor);
}

// Calculate n1n1 Q3D based on h2(eta,pt) x h2(eta,pt)
void HistogramCollection::calculateN1N1H2H2_Q3D_MCEta(TH2 * n1_1, TH2 * n1_2, TH3 * n1n1_Q3D, double a1 __attribute__((unused)), double a2 __attribute__((unused)))
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,n1_1,n1_2,n1n1_Q3D)) return;
  if (!sameDimensions(__FUNCTION__,n1_1,n1_2)) return;

  double Qlong, Qout, Qside;
  double pt1, phi1, eta1;
  double pt2, phi2, eta2;
  int nIter = 10000000;

  double avgN1 = n1_1->Integral();
  double avgN2 = n1_2->Integral();
  if (reportDebug(__FUNCTION__))
    {
    cout << "   avgN1: " << avgN1 << endl;
    cout << "   avgN2: " << avgN2 << endl;
    }
  double scalingFactor = avgN1*avgN2/double(nIter);

  for (int k=0;k<nIter;k++)
    {
    n1_1->GetRandom2 (eta1, pt1);
    n1_2->GetRandom2 (eta2, pt2);
    phi1 = 2.0*TMath::Pi() * randomGenerator->Rndm();
    phi2 = 2.0*TMath::Pi() * randomGenerator->Rndm();
    bool status = calculateQ3DwPtPhiEta(pt1, phi1, eta1, pt2, phi2, eta2, Qlong, Qout, Qside);
    if (status==0)
      {
      n1n1_Q3D->Fill(Qlong, Qside, Qout, 1.0);
      }
    }
  n1n1_Q3D->Scale(scalingFactor);
}

// Calculate n1n1 Q3D based on h2(eta,pt) x h2(eta,pt)
void HistogramCollection::calculateN1N1H2H2_Q3D(const TH2 * n1_1, const TH2 * n1_2, TH3 * n1n1_Q3D, double a1 __attribute__((unused)), double a2 __attribute__((unused)))
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,n1_1,n1_2,n1n1_Q3D)) return;
  if (!sameDimensions(__FUNCTION__,n1_1,n1_2)) return;
  int nx = n1_1->GetNbinsX();
  int ny = n1_1->GetNbinsY();
  double v1;
  double v2;
  double v3;
  double Qlong, Qout, Qside;
  double pt1, phi1, eta1;
  double pt2, phi2, eta2;

  for (int i1x=1;i1x<=nx;++i1x)
    {
    for (int i1y=1;i1y<=ny;++i1y)
      {
      for (int i2x=1;i2x<=nx;++i2x)
        {
        for (int i2y=1;i2y<=ny;++i2y)
          {
          v1  = a1*n1_1->GetBinContent(i1x,i1y); //ev1 = a1*n1_1->GetBinError(i1x,i1y,i1z);
          v2  = a2*n1_2->GetBinContent(i2x,i2y); //ev2 = a2*n1_2->GetBinError(i2x,i2y,i2z);
          if (v1>0 && v2>0)
            {
            v3  = v1*v2;
            for (int k=0;k<100000;k++)
              {
              phi1 = 2.0*TMath::Pi() * randomGenerator->Rndm();
              phi2 = 2.0*TMath::Pi() * randomGenerator->Rndm();
              eta1 = n1_1->GetXaxis()->GetBinCenter(i1x);
              pt1  = n1_1->GetZaxis()->GetBinCenter(i1y);
              eta2 = n1_2->GetXaxis()->GetBinCenter(i2x);
              pt2  = n1_2->GetZaxis()->GetBinCenter(i2y);
              bool status = calculateQ3DwPtPhiEta(pt1, phi1, eta1, pt2, phi2, eta2, Qlong, Qout, Qside);
              if (status==0)
                {
                //cout << " i1x:" << i1x << " i1y: " << i1y << " i1z: " << i1z << " i2x:" << i1x << " i2y: " << i2y << " i2z: " << i2z << " v1: " << v1 << " v2: " << v2 << endl;
                //cout << " pt1:" << pt1 << " phi1: " << phi1 << " eta1: " << eta1 << " pt2:" << pt2 << " phi2: " << phi2 << " eta2: " << eta2 << " Qlong:" << Qlong << " Qout:" << Qout << " Qside:" << Qside << " v3:" << v3 << endl;
                n1n1_Q3D->Fill(Qlong, Qside, Qout, v3);
                }
              }
            }
          }
        }
      }
    }

}

void HistogramCollection::calculateN1N1H3H3_Q3D(const TH3 * n1_1, const TH3 * n1_2, TH3 * n1n1_Q3D, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,n1_1,n1_2,n1n1_Q3D)) return;
  if (!sameDimensions(__FUNCTION__,n1_1,n1_2)) return;
  int nx = n1_1->GetNbinsX();
  int ny = n1_1->GetNbinsY();
  int nz = n1_1->GetNbinsX();

  double v1;
  double v2;
  double v3;
  double Qlong, Qout, Qside;
  double pt1, phi1, eta1;
  double pt2, phi2, eta2;

  for (int i1x=1;i1x<=nx;++i1x)
    {
    for (int i1y=1;i1y<=ny;++i1y)
      {
      for (int i1z=1;i1z<=nz;++i1z)
        {
        for (int i2x=1;i2x<=nx;++i2x)
          {
          for (int i2y=1;i2y<=ny;++i2y)
            {
            for (int i2z=1;i2z<=nz;++i2z)
              {
              v1  = a1*n1_1->GetBinContent(i1x,i1y,i1z); //ev1 = a1*n1_1->GetBinError(i1x,i1y,i1z);
              v2  = a2*n1_2->GetBinContent(i2x,i2y,i2z); //ev2 = a2*n1_2->GetBinError(i2x,i2y,i2z);
              if (v1>0 && v2>0)
                {
                //er1 = ev1/v1;
                //er2 = ev2/v2;
                //if (er1< 10.5 && er2<10.5)
                //  {
                v3  = v1*v2;
                //ev3 = v3*sqrt(er1*er1 + er2*er2);

                eta1 = n1_1->GetXaxis()->GetBinCenter(i1x);
                phi1 = n1_1->GetYaxis()->GetBinCenter(i1y);
                pt1  = n1_1->GetZaxis()->GetBinCenter(i1z);
                eta2 = n1_2->GetXaxis()->GetBinCenter(i2x);
                phi2 = n1_2->GetYaxis()->GetBinCenter(i2y);
                pt2  = n1_2->GetZaxis()->GetBinCenter(i2z);
                bool status = calculateQ3DwPtPhiEta(pt1, phi1, eta1, pt2, phi2, eta2, Qlong, Qout, Qside);
                if (status==0)
                  {
                  //cout << " i1x:" << i1x << " i1y: " << i1y << " i1z: " << i1z << " i2x:" << i1x << " i2y: " << i2y << " i2z: " << i2z << " v1: " << v1 << " v2: " << v2 << endl;
                  //cout << " pt1:" << pt1 << " phi1: " << phi1 << " eta1: " << eta1 << " pt2:" << pt2 << " phi2: " << phi2 << " eta2: " << eta2 << " Qlong:" << Qlong << " Qout:" << Qout << " Qside:" << Qside << " v3:" << v3 << endl;
                  n1n1_Q3D->Fill(Qlong, Qside, Qout, v3);
                  }
                // }
                }
              }
            }
          }
        }}}

}

void HistogramCollection::calculateR2_Q3D(const TH3 * n2_Q3D, const TH3 * n1n1_Q3D, TH3 * R2_Q3D, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,n2_Q3D,n1n1_Q3D,R2_Q3D)) return;
  if (!sameDimensions(__FUNCTION__,n2_Q3D,n1n1_Q3D)) return;
  int nx = n2_Q3D->GetNbinsX();
  int ny = n2_Q3D->GetNbinsY();
  int nz = n2_Q3D->GetNbinsX();
  double v1,ev1,er1;
  double v2,ev2,er2;
  double v3,ev3;
  for (int i1=1;i1<=nx;++i1)
    {
    for (int i2=1;i2<=ny;++i2)
      {
      for (int i3=1;i3<=nz;++i3)
        {
        v1  = a1*n2_Q3D->GetBinContent(i1,i2,i3);   ev1 = a1*n2_Q3D->GetBinError(i1,i2,i3);
        v2  = a2*n1n1_Q3D->GetBinContent(i1,i2,i3); ev2 = a2*n1n1_Q3D->GetBinError(i1,i2,i3);
        if (v1>0 && v2>0)
          {
          if (v1)
            er1 = ev1/v1;
          else
            er1 = 0;
          if (v2)
            er2 = ev2/v2;
          else
            er2 = 0;
          if (er1< 0.5 && er2<0.5)
            {
            v3  = v1/v2 - 1;
            ev3 = v3*sqrt(er1*er1 + er2*er2);
            }
          else
            {
            v3 = 0.0;
            ev3 = 0.0;
            }
          }
        else
          {
          v3 = 0.0;
          ev3 = 0.0;
          }
        R2_Q3D->SetBinContent(i1,i2,i3,v3);
        R2_Q3D->SetBinError(i1,i2,i3,ev3);
        }
      }
    }
}

// Return the average bin content of the given 1D histogram
double HistogramCollection::avgValue(TH1 * h)
{


  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return -1;

  int nx = h->GetNbinsX();
  if (nx<1)
    {
    if (reportWarning(__FUNCTION__))
      {
      cout << endl << "Given histogram has <1 bins." << endl;
      }
    return 0.0;
    }
  double nnx = nx;
  double sum = 0.0;
  for (int k=1; k<=nx; k++)
    {
    sum += h->GetBinContent(k);
    }
  return sum/nnx;
}


//!
//! Set all bins of the given histogram to the given value and error.
//!
void HistogramCollection::setHistogram(TH1 * h, double v, double ev)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return;
  int n = h->GetNbinsX();
  for (int i1=1;i1<=n;++i1)
    {
    h->SetBinContent(i1,v);
    h->SetBinError(i1,ev);
    }
}

//!
//! Set all bins of the given histogram to the given value and error.
//!
void HistogramCollection::setHistogram(TH2 * h, double v, double ev)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return;
  int n_x = h->GetNbinsX();
  int n_y = h->GetNbinsY();
  for (int i1=1;i1<=n_x;++i1)
    {
    for (int i2=1;i2<=n_y;++i2)
      {
      h->SetBinContent(i1,i2,v);
      h->SetBinError(i1,i2,ev);
      }
    }
}

//!
//! Set all bins of the given histogram to the given value and error.
//!
void HistogramCollection::setHistogram(TH3 * h, double v, double ev)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return;
  int n_x = h->GetNbinsX();
  int n_y = h->GetNbinsY();
  int n_z = h->GetNbinsZ();
  for (int i1=1;i1<=n_x;++i1)
    {
    for (int i2=1;i2<=n_y;++i2)
      {
      for (int i3=1;i3<=n_z;++i3)
        {
        h->SetBinContent(i1,i2,i3,v);
        h->SetBinError(i1,i2,i3,ev);
        }
      }
    }
}

TH1 * HistogramCollection::loadH1(TFile * inputFile,const TString & histoName)
{

  if (!ptrFileExist(inputFile)) return nullptr;
  TH1* h = (TH1*) inputFile->Get(histoName);
  if (!h)
    {
    if (reportError(__FUNCTION__)) cout << "Could not load histogram: " << histoName << endl;
    return nullptr;
    }
  else
    {
    if (reportDebug(__FUNCTION__)) cout << "Loaded histogram named: "  << histoName << endl;
    }
  append(h);
  return h;
}

///Load the given 1D histogram (name) from the given TFile
///No test is //done to verify that the file is properly opened.

TH2 * HistogramCollection::loadH2(TFile * inputFile,const TString & histoName)
{

  if (!ptrFileExist(inputFile)) return nullptr;
  TH2* h = (TH2*) inputFile->Get(histoName);
  if (!h)
    {
    if (reportError(__FUNCTION__)) cout << "Could not load histogram: " << histoName << endl;
    return nullptr;
    }
  else
    {
    if (reportDebug(__FUNCTION__)) cout << "Loaded histogram named: "  << histoName << endl;
    }
  append(h);
  return h;
}

///Load the given 3D histogram (name) from the given TFile
///No test is //done to verify that the file is properly opened.
TH3 * HistogramCollection::loadH3(TFile * inputFile, const TString & histoName)
{

  if (!ptrFileExist(inputFile)) return nullptr;
  TH3* h = (TH3*) inputFile->Get(histoName);
  if (!h)
    {
    if (reportError(__FUNCTION__)) cout << "Could not load histogram: " << histoName << endl;
    return nullptr;
    }
  else
    {
    if (reportDebug(__FUNCTION__)) cout << "Loaded histogram named: "  << histoName << endl;
    }
  append(h);
  return h;
}

///Load the given 3D histogram (name) from the given TFile
///No test is //done to verify that the file is properly opened.
TProfile * HistogramCollection::loadProfile(TFile * inputFile, const TString & histoName)
{

  if (!ptrFileExist(inputFile)) return nullptr;
  TProfile* h = (TProfile*) inputFile->Get(histoName);
  if (!h)
    {
    if (reportError(__FUNCTION__)) cout << "Could not load histogram: " << histoName << endl;
    return nullptr;
    }
  else
    {
    if (reportDebug(__FUNCTION__)) cout << "Loaded histogram named: "  << histoName << endl;
    }
  append(h);
  return h;
}

void HistogramCollection::loadHistosInList(TFile * inputFile, HistogramCollection * collection)
{

  if (!ptrFileExist(inputFile)) return;
  int nHistos = collection->getNHistograms();
  TString histoName;
  for (int iHisto=0; iHisto<nHistos; iHisto++)
    {
    histoName = collection->getObjectAt(iHisto)->GetName();
    TH1 * h = (TH1*) inputFile->Get(histoName);
    if (!h)
      {
      if (reportError(__FUNCTION__)) cout << "Could not load histogram: " << histoName << endl;
      return;
      }
    else
      {
      if (reportDebug(__FUNCTION__)) cout << "Loaded histogram named: "  << histoName << endl;
      }
    append(h);
    }
}


///Clone the given histogram, and set the clone's name to the given name.
/////throws a HistogramException if the histogram does not exist (null pointer).
TH1 * HistogramCollection::clone(const TH1 * h1, const TString & histoName)
{

  if (!ptrExist(__FUNCTION__,h1)) return nullptr;
  TH1 * h = (TH1*) h1->Clone();
  if (!h)
    {
    if (reportError(__FUNCTION__)) cout << "Could not clone histogram "<< h1->GetName() << endl;
    return nullptr;
    }
  else
    {
    if (reportDebug(__FUNCTION__)) cout << "Successfully cloned histogram named: "  << histoName << endl;
    }
  h->SetName(histoName);
  return h;
}

void HistogramCollection::findMaximum(TH1 * h, int xFirstBin, int xLastBin, int & xMaxValueBin, double & xMaxValue)
{

  if (!ptrExist(__FUNCTION__,h)) return;
  int    n   = h->GetNbinsX();
  if (xFirstBin<1) xFirstBin = 1;
  if (xLastBin>n)  xLastBin = n;
  for (int i=xFirstBin;i<=xLastBin;++i)
    {
    double v = h->GetBinContent(i);
    if (v>xMaxValue)
      {
      xMaxValueBin = i;
      xMaxValue = v;
      }
    }
  if (reportDebug(__FUNCTION__))
    cout << endl << "  xMaxValueBin: "<< xMaxValueBin
    << endl << "     xMaxValue: "<< xMaxValue<< endl;

}

void HistogramCollection::findMinimum(TH1 * h, int xFirstBin, int xLastBin, int & xMinValueBin, double  & xMinValue)
{

  if (!ptrExist(__FUNCTION__,h)) return;
  int    n   = h->GetNbinsX();
  if (xFirstBin<1) xFirstBin = 1;
  if (xLastBin>n)  xLastBin = n;
  for (int i=xFirstBin;i<=xLastBin;++i)
    {
    double v = h->GetBinContent(i);
    if (v>xMinValue)
      {
      xMinValueBin = i;
      xMinValue = v;
      }
    }
  if (reportDebug(__FUNCTION__))
    cout << endl << "  xMinValueBin: "<< xMinValueBin
    << endl << "     xMinValue: "<< xMinValue << endl;
}

void HistogramCollection::scaleByBinWidth1D(TH1 * h, double scale)
{

  if (!ptrExist(__FUNCTION__,h)) return;

  if (reportDebug(__FUNCTION__))
    cout << endl
    << "   1D histogram " << h->GetName() << " will be scaled by a common scale of " << scale
    << " and bin width " << h->GetBinWidth(1) << endl;
  int n_x = h->GetNbinsX();
  double width_x, v, ev;

  for (int i_x=1; i_x<=n_x; ++i_x)
    {
    v = h->GetBinContent(i_x);
    ev = h->GetBinError(i_x);
    width_x = h->GetBinWidth(i_x);
    h->SetBinContent(i_x,v*scale/width_x);
    h->SetBinError(i_x,ev*scale/width_x);
    }
}

void HistogramCollection::scaleByBinWidth2D(TH2 * h, double scale)
{

  if (!ptrExist(__FUNCTION__,h)) return;

  TAxis * xAxis = h->GetXaxis();
  TAxis * yAxis = h->GetYaxis();
  if (reportDebug(__FUNCTION__))
    cout << endl
    << "   2D histogram " << h->GetName() << " will be scaled by a common scale of " << scale
    << " bin width_x " << xAxis->GetBinWidth(1) << " bin width_y " << yAxis->GetBinWidth(1) << endl;

  int n_x = h->GetNbinsX();
  int n_y = h->GetNbinsY();
  double width_x,width_y, v, ev, w;
  for (int i_x=1; i_x<=n_x; ++i_x)
    {
    width_x = xAxis->GetBinWidth(i_x);
    for (int i_y=1; i_y<=n_y; ++i_y)
      {
      v  = h->GetBinContent(i_x,i_y);
      ev = h->GetBinError(i_x,i_y);
      width_y = yAxis->GetBinWidth(i_y);
      w = scale/(width_x*width_y);
      h->SetBinContent(i_x,i_y,v*w);
      h->SetBinError(i_x,i_y,ev*w);
      }
    }
}

void HistogramCollection::scaleByBinWidth3D(TH3 * h, double scale)
{

  if (!ptrExist(__FUNCTION__,h)) return;

  TAxis * xAxis = h->GetXaxis();
  TAxis * yAxis = h->GetYaxis();
  TAxis * zAxis = h->GetZaxis();
  if (reportDebug(__FUNCTION__))
    cout << endl
    << "   3D histogram " << h->GetName() << " will be scaled by a common scale of " << scale
    << " bin width_x " << xAxis->GetBinWidth(1) << " bin width_y " << yAxis->GetBinWidth(1) << " bin width_z " << zAxis->GetBinWidth(1) << endl;

  int n_x = h->GetNbinsX();
  int n_y = h->GetNbinsY();
  int n_z = h->GetNbinsZ();
  double width_x,width_y, width_z, v, ev, w;
  for (int i_x=1; i_x<=n_x; ++i_x)
    {
    width_x = xAxis->GetBinWidth(i_x);
    for (int i_y=1; i_y<=n_y; ++i_y)
      {
      width_y = yAxis->GetBinWidth(i_y);
      for (int i_z=1; i_z<=n_z; ++i_z)
        {
        width_z = zAxis->GetBinWidth(i_z);
        v  = h->GetBinContent(i_x, i_y, i_z);
        ev = h->GetBinError(  i_x, i_y, i_z);
        w = scale/(width_x*width_y*width_z);
        h->SetBinContent(i_x, i_y, i_z,  v*w);
        h->SetBinError(  i_x, i_y, i_z,  ev*w);
        }
      }
    }
}

void HistogramCollection::scaleByBinWidth(TH1 * h, double scale)
{

  if (!ptrExist(__FUNCTION__,h)) return;

  TClass * c = h->IsA();
  if (c == TProfile::Class())
    {
    //TProfile are not to be scaled
    if (reportDebug(__FUNCTION__)) cout << "1D Profile named:" << h->GetName() << " will NOT be scaled." << endl;
    }
  else if (c == TH1F::Class() || c == TH1D::Class())
    {
    if (reportDebug(__FUNCTION__)) cout << "1D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale << endl;
    scaleByBinWidth1D(h, scale);
    }
  else if (c == TH2F::Class() || c == TH2D::Class())
    {
    if (reportDebug(__FUNCTION__)) cout << "2D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale  << endl;
    TH2 * h2 = (TH2*) h;
    scaleByBinWidth2D(h2, scale);
    }
  else if (c == TH3F::Class() || c == TH3D::Class())
    {
    if (reportDebug(__FUNCTION__)) cout << "3D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale  << endl;
    TH3 * h3 = (TH3*) h;
    scaleByBinWidth3D(h3, scale);
    }
  else if (c == TProfile2D::Class())
    {
    if (reportDebug(__FUNCTION__)) cout << "2D Profile named:" << h->GetName() << " will NOT be scaled." << endl;
    }
  else
    {
    if (reportError(__FUNCTION__))  cout << h->GetName() << " of unrecognized type will NOT be scaled." << endl;
    }
}


void HistogramCollection::scaleAllHistoByBinWidth(double scale)
{

  TH1 * h;
  for (int i=0; i<getNHistograms(); ++i)
    {
    h = getObjectAt(i);
    TClass * c = h->IsA();
    if (c == TProfile::Class())
      {
      //TProfile are not to be scaled
      if (reportDebug(__FUNCTION__)) cout << "1D Profile named:" << h->GetName() << " will NOT be scaled." << endl;
      }
    else if (c == TH1F::Class() || c == TH1D::Class())
      {
      if (reportDebug(__FUNCTION__)) cout << "1D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale << endl;
      scaleByBinWidth1D(h, scale);
      }
    else if (c == TH2F::Class() || c == TH2D::Class())
      {
      if (reportDebug(__FUNCTION__)) cout << "2D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale  << endl;
      TH2 * h2 = (TH2*) h;
      scaleByBinWidth2D(h2, scale);
      }
    else if (c == TProfile2D::Class())
      {
      if (reportDebug(__FUNCTION__)) cout << "2D Profile named:" << h->GetName() << " will NOT be scaled." << endl;
      }
    }
}


void HistogramCollection::sumw2All()
{

  TH1 * h;
  for (int i=0; i<getNHistograms(); ++i)
    {
    h = getObjectAt(i);;
    TClass * c = h->IsA();
    if (c==TProfile::Class() )
      {
      if (reportDebug(__FUNCTION__)) cout << "Profile named:" << h->GetName() << " will not be sumw2" << endl;
      }
    else
      {
      if (reportDebug(__FUNCTION__)) cout << "Histo named:" << h->GetName() << " will  be sumw2" << endl;
      if (h->GetEntries()>0)
        h->Sumw2();
      }
    }
}



void HistogramCollection::unpack_vsXY_to_vsXVsY(const TH1 * source, TH2 * target)
{

  if (reportStart(__FUNCTION__))
    ;

  if (!ptrExist(__FUNCTION__,source)) return;


  // Unpack 1D source histogram into a 2D histogram
  int k = 1;
  double v;
  int n  = source->GetNbinsX();
  int n_x = target->GetNbinsX();
  int n_y = target->GetNbinsY();
  if (n!=n_x*n_y)
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;
      cout << "    unpack_vsXY_to_vsXVsY(..)  Incompatible histogram dimensions" << endl;
      cout << "    source  n:" << n << endl;
      cout << "    target n_x:" << n_x << endl;
      cout << "    target n_y:" << n_y << endl;
      }
    return;
    }

  for (int i_x=1; i_x<=n_x; ++i_x)
    {
    for (int i_y=1; i_y<=n_y; ++i_y)
      {
      v  = source->GetBinContent(k);
      //ev = source->GetBinError(k);
      target->SetBinContent(i_x,i_y,v);
      target->SetBinError(i_x,i_y,v);
      k++;
      }
    }
}

void HistogramCollection::correctMerging(TH1 * h, int nEta, int nPhi, bool reverse)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return;

  double v, ev;
  int iEta, iPhi, iDphi, iPhiA, iPhiB;
  int ij1, ij2;
  int nEtaPhi = nEta*nPhi;
  for (iEta=0; iEta<nEta; iEta++)
    {
    for (iPhi=0; iPhi<nPhi; iPhi++)
      {
      for (iDphi=1; iDphi<7; iDphi++)
        {
        iPhiA = iPhi+iDphi;  if (iPhiA>=nPhi) iPhiA -= nPhi; // Dphi12 = iPhi-iPhiA < 0
        iPhiB = iPhi-iDphi;  if (iPhiB< 0) iPhiB += nPhi; // Dphi12 = iPhi-iPhiB > 0
                                                          //Deta==0
        ij1 = nEtaPhi*(iEta*nPhi+iPhi) + iEta*nPhi+iPhiA;
        ij2 = nEtaPhi*(iEta*nPhi+iPhi) + iEta*nPhi+iPhiB;
        if (reverse)
          {
          v  = h->GetBinContent(ij1);
          ev = h->GetBinError(ij1);
          h->SetBinContent(ij2,v);
          h->SetBinError(ij2,ev);
          }
        else
          {
          v  = h->GetBinContent(ij2);
          ev = h->GetBinError(ij2);
          h->SetBinContent(ij1,v);
          h->SetBinError(ij1,ev);
          }
        if (iEta>0)   //Deta==1
          {
          ij1 = nEtaPhi*(iEta*nPhi+iPhi) + (iEta-1)*nPhi+iPhiA;
          ij2 = nEtaPhi*(iEta*nPhi+iPhi) + (iEta-1)*nPhi+iPhiB;
          if (reverse)
            {
            v  = h->GetBinContent(ij1);
            ev = h->GetBinError(ij1);
            h->SetBinContent(ij2,v);
            h->SetBinError(ij2,ev);
            }
          else
            {
            v  = h->GetBinContent(ij2);
            ev = h->GetBinError(ij2);
            h->SetBinContent(ij1,v);
            h->SetBinError(ij1,ev);
            }

          }
        if (iEta<nEta)   //Deta==1
          {
          ij1 = nEtaPhi*(iEta*nPhi+iPhi) + (iEta+1)*nPhi+iPhiA;
          ij2 = nEtaPhi*(iEta*nPhi+iPhi) + (iEta+1)*nPhi+iPhiB;
          if (reverse)
            {
            v  = h->GetBinContent(ij1);
            ev = h->GetBinError(ij1);
            h->SetBinContent(ij2,v);
            h->SetBinError(ij2,ev);
            }
          else
            {
            v  = h->GetBinContent(ij2);
            ev = h->GetBinError(ij2);
            h->SetBinContent(ij1,v);
            h->SetBinError(ij1,ev);
            }

          }

        }
      }
    }
}

//!
//! Calculate the 'triangle' normalization
//! target will be divided by bin widths and scaleFactor
//!
void HistogramCollection::calculateAvgH2H2(const TH2 * source, TH2 * target, double scaleFactor)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,source,target)) return;
  if (!sameDimensions(__FUNCTION__,source,target)) return;
  int nDeta = source->GetNbinsX();
  int nDphi = source->GetNbinsY();
  vector<double> denominator(nDeta,0.0);
  for (int iEta=0;iEta<(nDeta+1)/2; ++iEta)
    {
    denominator[iEta] = iEta+1;
    denominator[nDeta-1-iEta] = iEta+1;
    }
  double v1, ev1;
  for (int iEta=1;iEta<=nDeta; ++iEta)
    {
    for (int iPhi=1;iPhi<=nDphi; ++iPhi)
      {
      v1   = source->GetBinContent(iEta,iPhi);
      ev1  = source->GetBinError(  iEta,iPhi);
      v1   = v1/denominator[iEta-1];
      ev1  = ev1/denominator[iEta-1];
      target->SetBinContent(iEta,iPhi,v1);
      target->SetBinError  (iEta,iPhi,ev1);
      }
    }
  double wx = target->GetXaxis()->GetBinWidth(1);
  double wy = target->GetYaxis()->GetBinWidth(1);
  double factor = 1.0/(wx*wy*scaleFactor);
  target->Scale(factor);
}


///Calculate R2 = binCorrection*N2/N1/N1 - 1
void HistogramCollection::calculateR2_H1H1H1(const TH1 * n2_12, const TH1 * n1n1_12, TH1 * r2_12, bool ijNormalization, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,n2_12,n1n1_12,r2_12)) return;

  int n2_12_n_x    = n2_12->GetNbinsX();
  int n1n1_12_n_x  = n1n1_12->GetNbinsX();
  int r2_12_n_x    = r2_12->GetNbinsX();
  if (n2_12_n_x!=n1n1_12_n_x || n2_12_n_x!=r2_12_n_x )
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;cout << "Incompatible histo dimensions" << endl;
      cout << "H1: " << n2_12->GetName()   << " n_x:" << n2_12_n_x   << endl;
      cout << "H2: " << n1n1_12->GetName() << " n_x:" << n1n1_12_n_x << endl;
      cout << "H3: " << r2_12->GetName()   << " n_x:" << r2_12_n_x   << endl;
      cout << "ABORT!!!!!" << endl;
      }
    return;
    }

  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i1=1;i1<=n2_12_n_x;++i1)
    {
    v1  = a1*n2_12->GetBinContent(i1);    ev1 = a1*n2_12->GetBinError(i1);
    v2  = a2*n1n1_12->GetBinContent(i1);  ev2 = a2*n1n1_12->GetBinError(i1);
    if (v1>0 && v2>0 && ev1/v1<0.5  && ev2/v2<0.5 )
      {
      if (ijNormalization) //account for the fact only half the pairs were counted
        {
        v   = 2*v1/v2;
        if (i1<20) if (reportDebug(__FUNCTION__)) cout << "normalized v:" << v << endl;
        }
      else // all pairs counted - no need to multiply by 2
        {
        v   = v1/v2;
        if (i1<20) if (reportDebug(__FUNCTION__)) cout << "NOT normalized v:" << v << endl;
        }
      re1 = ev1/v1;
      re2 = ev2/v2;
      ev  = v*sqrt(re1*re1+re2*re2);
      v   -= 1.;
      //if (i1<20) if (reportDebug(__FUNCTION__)) cout << "-1:" << v << endl;

      }
    else
      {
      v = 0.;
      ev = 0;
      }
    r2_12->SetBinContent(i1,v); r2_12->SetBinError(i1,ev);
    }
}

//Calculate R2 = N2/N1/N1 - 1
void HistogramCollection::calculateR2_H2H2H2(const TH2 * n2_12, const TH2 * n1n1_12, TH2 * r2_12, bool ijNormalization, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;

  if (!ptrExist(__FUNCTION__,n2_12,n1n1_12,r2_12)) return;
  if (!sameDimensions(__FUNCTION__,n2_12,n1n1_12,r2_12)) return;
  int n2_12_n_x = n2_12->GetNbinsX();
  int n2_12_n_y = n2_12->GetNbinsY();

  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i_x=1;i_x<=n2_12_n_x;++i_x)
    {
    for (int i_y=1;i_y<=n2_12_n_y;++i_y)
      {
      v1  = a1*n2_12->GetBinContent(i_x,i_y);    ev1 = a1*n2_12->GetBinError(i_x,i_y);
      v2  = a2*n1n1_12->GetBinContent(i_x,i_y);  ev2 = a2*n1n1_12->GetBinError(i_x,i_y);
      if (v1>0 && v2>0) //   && ev1/v1<0.5  && ev2/v2<0.5)
        {
        if (ijNormalization) //account for the fact only half the pairs were counted
          v   = 2*v1/v2;
        else // all pairs counted - no need to multiply by 2
          v   = v1/v2;
        re1 = ev1/v1;
        re2 = ev2/v2;
        ev  = v*sqrt(re1*re1+re2*re2);
        v   -= 1.;
        }
      else
        {
        v = 0.;
        ev = 0;
        }
      r2_12->SetBinContent(i_x,i_y,v); r2_12->SetBinError(i_x,i_y,ev);
      }
    }
}

//Calculate R2 = N2/N1/N1 - 1
void HistogramCollection::calculateR2_H1H2H2(const TH1 * n2_12, const TH2 * n1n1_12, TH2 * r2_12, bool ijNormalization, double a1, double a2)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,n2_12,n1n1_12,r2_12)) return;

  int n2_12_n_x    = n2_12->GetNbinsX();
  int n1n1_12_n_x  = n1n1_12->GetNbinsX();
  int n1n1_12_n_y  = n1n1_12->GetNbinsY();
  int r2_12_n_x    = r2_12->GetNbinsX();
  int r2_12_n_y    = r2_12->GetNbinsY();
  if (n2_12_n_x!=(n1n1_12_n_x*n1n1_12_n_y) || n1n1_12_n_x!=r2_12_n_x || n1n1_12_n_y!=r2_12_n_y )
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;
      cout << "Incompatible histo dimensions" << endl;
      cout << "H1: " << n2_12->GetName()   << " n_x:" << n2_12_n_x   << endl;
      cout << "H2: " << n1n1_12->GetName() << " n_x:" << n1n1_12_n_x << " n_y:" << n1n1_12_n_y << endl;
      cout << "H3: " << r2_12->GetName()   << " n_x:" << r2_12_n_x   << " n_y:" << r2_12_n_y   << endl;
      cout << "ABORT!!!!!" << endl;
      }
    return;
    }

  double v1,ev1,v2,ev2,v,ev, re1,re2;
  int i = 1;
  for (int i_x=1;i_x<=n1n1_12_n_x;++i_x)
    {
    for (int i_y=1;i_y<=n1n1_12_n_y;++i_y)
      {
      v1  = a1*n2_12->GetBinContent(i);        ev1 = a1*n2_12->GetBinError(i);
      v2  = a2*n1n1_12->GetBinContent(i_x,i_y);  ev2 = a2*n1n1_12->GetBinError(i_x,i_y);
      if (v1>0 && v2>0  && ev1/v1<0.5  && ev2/v2<0.5)
        {
        if (ijNormalization) //account for the fact only half the pairs were counted
          v   = 2*v1/v2;
        else // all pairs counted - no need to multiply by 2
          v   = v1/v2;
        re1 = ev1/v1;
        re2 = ev2/v2;
        ev  = v*sqrt(re1*re1+re2*re2);
        v   -= 1.;
        }
      else
        {
        v = 0.;
        ev = 0;
        }
      r2_12->SetBinContent(i_x,i_y,v); r2_12->SetBinError(i_x,i_y,ev);
      i++;
      }
    }
}



void HistogramCollection::calculateR2VsM(const TProfile * h1, const TProfile * h2, const TProfile * h12, TH1 * r2VsM, TH1 * intR2, bool sameFilter)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h1,h2,h12,r2VsM,intR2)) return;

  int n1, n2, n3, n4, n5;
  n1   = h1->GetNbinsX();
  if (sameFilter) n2 = n1; else n2   = h2->GetNbinsX();
  n3   = h12->GetNbinsX();
  n4   = r2VsM->GetNbinsX();
  n5   = intR2->GetNbinsX();
  if (n1!=n2 || n1!=n3 || n1!=n4 || n5!=1)
    {
    if (reportError(__FUNCTION__)) cout << endl << "Incompatible histogram dimensions" << endl;
    return;
    }
  double n, nSum, vSum, v1,ev1,v2,v12,ev12, r, er;
  nSum = vSum = 0;
  for (int i=1;i<=n1;++i)
    {
    n    = h1->GetBinEntries(i);
    v1   = h1->GetBinContent(i);  ev1  = h1->GetBinError(i);
    if (sameFilter)
      {
      v2   = v1;  //ev2  = ev1;
      }
    else
      {
      v2   = h2->GetBinContent(i);  //ev2  = h2->GetBinError(i);
      }
    v12  = h12->GetBinContent(i); ev12 = h12->GetBinError(i);
    if (v1>0 && v2>0 && v12>0)
      {
      r = v12/(v1*v2);
      er = r*ev12/v12; // under estimate errors.
      nSum += n;
      vSum += n*r;
      }
    else
      {
      r  = 1;
      er = 0;
      }
    //if (reportDebug(__FUNCTION__)) cout << "calculateR2VsM: :" << i << " v1:" << v1 << " v2:" << v2 << " v12:" << v12 << " r:" << r << " nSum:" << nSum << " vSum:" << vSum << endl;
    r2VsM->SetBinContent(i,r);
    r2VsM->SetBinError(i,er);
    }
  if (nSum>0)
    {
    intR2->SetBinContent(1,vSum/nSum);
    intR2->SetBinError(1,0.);
    }
  else
    {
    intR2->SetBinContent(1,0.);
    intR2->SetBinError(1,0.);
    }
}

void HistogramCollection::calculateBinCorr(const TProfile * h1, const TProfile * h2, TH1 * intBinCorrVsM1, bool sameFilter)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h1,h2,intBinCorrVsM1)) return;

  int n1, n2, n3;
  n1   = h1->GetNbinsX();
  if (sameFilter) n2 = n1; else n2   = h2->GetNbinsX();
  n3   = intBinCorrVsM1->GetNbinsX();
  if (n1!=n2 || n3!=1)
    {
    if (reportError(__FUNCTION__)) cout << endl << "Incompatible histogram dimensions" << endl;
    return;
    }
  double n, nSum, sum1, sum2, sum12, v1,v2;
  sum1 = sum2 = sum12 = nSum = 0;
  for (int i=1;i<=n1;++i)
    {
    n    = h1->GetBinEntries(i);
    v1   = h1->GetBinContent(i);
    if (sameFilter) v2 = v1; else v2 = h2->GetBinContent(i);
    //if (reportDebug(__FUNCTION__)) cout << "calculateBinCorr: i:" << i << " v1:" << v1 << " v2:" << v2 << " n:" << n << endl;
    if (n>0)
      {
      nSum += n;
      sum1 += n*v1;
      sum2 += n*v2;
      sum12 += n*v1*v2;
      }
    }
  if (nSum>0 && sum1>0 && sum2>0)
    {
    sum1 /= nSum;
    sum2 /= nSum;
    sum12 /= nSum;
    intBinCorrVsM1->SetBinContent(1,sum12/(sum1*sum2));
    intBinCorrVsM1->SetBinError(1,0.);
    }
  else
    {
    intBinCorrVsM1->SetBinContent(1,1.); //no stat - so set correction factor to unity
    intBinCorrVsM1->SetBinError(1,0.);
    }

}

//!
//! Compute the average density of particles based on the given dN/dx histogram.
//! The histogram is assumed to be a yield per event per bin vs. eta
//! The average value is thus divided by the bin width to make it into a density.
//! This yields a density integrated over 2pi. So to use this number, one must also
//! divide by 2pi.
//!
void HistogramCollection::calculateAverage(TH1* h, double & avgDensity, double & eAvgDensity)
{

  if (reportStart(__FUNCTION__))
    ;
  int n = h->GetNbinsX();
  double v, ev, width, sum, esum, avgValue;
  sum  = 0.0;
  esum = 0.0;
  //  int first = 9;
  //  int last  = 32;
  int first = 1;
  int last  = n;
  int norm  = last - first + 1;
  for (int i=first; i<=last; i++)
    {
    v     = h->GetBinContent(i);
    ev    = h->GetBinError(i);
    width = h->GetBinWidth(i);
    sum  += v/width;
    esum += ev*ev/width/width;
    }
  avgValue    = sum/norm;
  avgDensity  = avgValue/TMath::TwoPi();
  eAvgDensity = sqrt(esum)/norm/TMath::TwoPi();
  if ( reportInfo("HistogramCollection",getName(),"calculateAverage(TH1* h, double & avgDensity, double & eAvgDensity"))
    {
    cout << "     Name: " << h->GetName() << "  avgValue: " << avgValue   << "  density: " << avgDensity  <<  " +- " << eAvgDensity << endl;
    }
}

void HistogramCollection::calculateIntegral(TH1 * h, double xMin, double xMax, double  & result, double & resultError, int option)
{
  if (reportStart(__FUNCTION__))
    ;
  int xFirst = h->GetXaxis()->FindBin(xMin);
  int xLast  = h->GetXaxis()->FindBin(xMax);

  double sum, sumError;
  double integral, integralError;
  sum      = h->IntegralAndError(xFirst,xLast, sumError);
  integral = h->IntegralAndError(xFirst,xLast, integralError, "WIDTH");
  switch (option)
    {
      case 0: result = sum; resultError = sumError; break;
      case 1: result = integral; resultError = integralError; break;
    }
  if ( reportInfo(__FUNCTION__))
    {
    cout << "     Name: " << h->GetName() << endl;
    cout << "      sum: " << sum      << " +- " << sumError << endl;
    cout << " integral: " << integral << " +- " << integralError << endl;
    }
}

void HistogramCollection::calculateIntegral(TH2 * h, double xMin, double xMax, double yMin, double yMax, double  & result, double & resultError, int option)
{

  if (reportStart(__FUNCTION__))
    ;
  int xFirst = h->GetXaxis()->FindBin(xMin);
  int xLast  = h->GetXaxis()->FindBin(xMax);
  int yFirst = h->GetYaxis()->FindBin(yMin);
  int yLast  = h->GetYaxis()->FindBin(yMax);

  double sum, sumError;
  double integral, integralError;
  sum      = h->IntegralAndError(xFirst,xLast, yFirst, yLast, sumError);
  integral = h->IntegralAndError(xFirst,xLast, yFirst, yLast, integralError, "WIDTH");
  switch (option)
    {
      case 0: result = sum; resultError = sumError; break;
      case 1: result = integral; resultError = integralError; break;
    }
  if ( reportInfo(__FUNCTION__))
    {
    cout << "     Name: " << h->GetName() << endl;
    cout << "      sum: " << sum      << " +- " << sumError << endl;
    cout << " integral: " << integral << " +- " << integralError << endl;
    }
}





void HistogramCollection::calculateAveragePt(const TH1 * h1, const TH1 * h2, TH1 * h3)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h1,h2,h3)) return;
  if (!sameDimensions(__FUNCTION__,h1,h2,h3)) return;

  int n1 = h1->GetNbinsX();
  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i1=1;i1<=n1;++i1)
    {
    v1  = h1->GetBinContent(i1);  ev1 = h1->GetBinError(i1);
    v2  = h2->GetBinContent(i1);  ev2 = h2->GetBinError(i1);
    if (v1>0 && v2>0)
      {
      v = v1/v2;
      re1 = ev1/v1;
      re2 = ev2/v2;
      ev = v*sqrt(re1*re1+re2*re2);
      }
    else
      {
      v = 0.;
      ev = 0;
      }
    h3->SetBinContent(i1,v); h3->SetBinError(i1,ev);
    }
}

//Calculate average pt by dividing sPt contained in h1 by n1 contained in h2. Store in h3.

void HistogramCollection::calculateAveragePtH2(const TH2 * h1, const TH2 * h2, TH2 * h3)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h1,h2,h3)) return;
  if (!sameDimensions(__FUNCTION__,h1,h2,h3)) return;


  int n1x   = h1->GetNbinsX();
  int n1y   = h1->GetNbinsY();
  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i_x=1;i_x<=n1x;++i_x)
    {
    for (int i_y=1;i_y<=n1y;++i_y)
      {
      v1  = h1->GetBinContent(i_x,i_y);  ev1 = h1->GetBinError(i_x,i_y);
      v2  = h2->GetBinContent(i_x,i_y);  ev2 = h2->GetBinError(i_x,i_y);
      if (v1>0 && v2>0)
        {
        v = v1/v2;
        re1 = ev1/v1;
        re2 = ev2/v2;
        ev = v*sqrt(re1*re1+re2*re2);
        }
      else
        {
        v = 0.;
        ev = 0;
        }
      h3->SetBinContent(i_x,i_y,v); h3->SetBinError(i_x,i_y,ev);
      }
    }
}

void HistogramCollection::calculateAveragePtH1H2(const TH1 * h1, const TH1 * h2, TH2 * h3)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h1,h2,h3)) return;
  if (!sameDimensions(__FUNCTION__,h1,h2)) return;

  //Calculate average pt by dividing sPt contained in h1 by n1 contained in h2. Store in h3.
  int n3x  = h3->GetNbinsX();
  int n3y  = h3->GetNbinsY();
  double v1,ev1,v2,ev2,v,ev, re1,re2;
  int i = 1;
  for (int i_x=1;i_x<=n3x;++i_x)
    {
    for (int i_y=1;i_y<=n3y;++i_y)
      {
      v1  = h1->GetBinContent(i);  ev1 = h1->GetBinError(i);
      v2  = h2->GetBinContent(i);  ev2 = h2->GetBinError(i);
      if (v1>0 && v2>0)
        {
        v = v1/v2;
        re1 = ev1/v1;
        re2 = ev2/v2;
        ev = v*sqrt(re1*re1+re2*re2);
        }
      else
        {
        v = 0.;
        ev = 0;
        }
      h3->SetBinContent(i_x,i_y,v); h3->SetBinError(i_x,i_y,ev);
      i++;
      }
    }
}

void HistogramCollection::calculateAveragePt(const TProfile * h1, const TProfile * h2, TH1 * h3)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h1,h2,h3)) return;
  if (!sameDimensions(__FUNCTION__,h1,h2,h3)) return;
  double v1,ev1,v2,ev2,v,ev, re1,re2;
  int n1 = h1->GetNbinsX();
  for (int i1=1;i1<=n1;++i1)
    {
    v1  = h1->GetBinContent(i1);  ev1 = h1->GetBinError(i1);
    v2  = h2->GetBinContent(i1);  ev2 = h2->GetBinError(i1);
    if (v2>0)
      {
      v = v1/v2;
      re1 = ev1/v1;
      re2 = ev2/v2;
      ev = v*sqrt(re1*re1+re2*re2);
      }
    else
      {
      v = 0.;
      ev = 0;
      }
    h3->SetBinContent(i1,v);  h3->SetBinError(i1,ev);
    }
}

void HistogramCollection::symmetrize3D(TH3* h)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return;
  int n = h->GetNbinsX();
  int nc = n/2;
  int nf = nc + 1;
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "     n  = " << n << endl;
    cout << "     nc = " << nc << endl;
    cout << "     nf = " << nf << endl;
    }

  double v1, v2, v3, v4, v5, v6, v7, v8;
  double ev1, ev2, ev3, ev4, ev5, ev6, ev7, ev8;
  double v, ev;

  for (int i1=1; i1<=nc; i1++)
    {
    for (int i2=1; i2<=nc; i2++)
      {
      for (int i3=1; i3<=nc; i3++)
        {
        v1 = h->GetBinContent(nc+i1,nc+i2,nc+i3);  ev1 = h->GetBinError(nc+i1,nc+i2,nc+i3);
        v2 = h->GetBinContent(nc+i1,nc+i2,nf-i3);  ev2 = h->GetBinError(nc+i1,nc+i2,nf-i3);
        v3 = h->GetBinContent(nc+i1,nf-i2,nc+i3);  ev3 = h->GetBinError(nc+i1,nf-i2,nc+i3);
        v4 = h->GetBinContent(nc+i1,nf-i2,nf-i3);  ev4 = h->GetBinError(nc+i1,nf-i2,nf-i3);
        v5 = h->GetBinContent(nf-i1,nc+i2,nc+i3);  ev5 = h->GetBinError(nf-i1,nc+i2,nc+i3);
        v6 = h->GetBinContent(nf-i1,nc+i2,nf-i3);  ev6 = h->GetBinError(nf-i1,nc+i2,nf-i3);
        v7 = h->GetBinContent(nf-i1,nf-i2,nc+i3);  ev7 = h->GetBinError(nf-i1,nf-i2,nc+i3);
        v8 = h->GetBinContent(nf-i1,nf-i2,nf-i3);  ev8 = h->GetBinError(nf-i1,nf-i2,nf-i3);

        v = (v1+v2+v3+v4+v5+v6+v7+v8)/8.0;
        ev = sqrt(ev1*ev1+ev2*ev2+ev3*ev3+ev4*ev4+ev5*ev5+ev6*ev6+ev7*ev7+ev8*ev8)/8.0;

        h->SetBinContent(nc+i1,nc+i2,nc+i3,v);  h->SetBinError(nc+i1,nc+i2,nc+i3,ev);
        h->SetBinContent(nc+i1,nc+i2,nf-i3,v);  h->SetBinError(nc+i1,nc+i2,nf-i3,ev);
        h->SetBinContent(nc+i1,nf-i2,nc+i3,v);  h->SetBinError(nc+i1,nf-i2,nc+i3,ev);
        h->SetBinContent(nc+i1,nf-i2,nf-i3,v);  h->SetBinError(nc+i1,nf-i2,nf-i3,ev);
        h->SetBinContent(nf-i1,nc+i2,nc+i3,v);  h->SetBinError(nf-i1,nc+i2,nc+i3,ev);
        h->SetBinContent(nf-i1,nc+i2,nf-i3,v);  h->SetBinError(nf-i1,nc+i2,nf-i3,ev);
        h->SetBinContent(nf-i1,nf-i2,nc+i3,v);  h->SetBinError(nf-i1,nf-i2,nc+i3,ev);
        h->SetBinContent(nf-i1,nf-i2,nf-i3,v);  h->SetBinError(nf-i1,nf-i2,nf-i3,ev);

        }
      }
    }
}



// h must be writtable otherwise this does not work..
// x axis is DeltaEta. It must have an odd number of bins
// y axis is DeltaPhi. It must have an even number of bins
void HistogramCollection::symmetrizeDeltaEtaDeltaPhi(TH2 * h, bool ijNormalization)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return;

  double v1, v2, v3, v4;
  double ev1, ev2, ev3, ev4;
  double sv, esv;
  int nEta = h->GetNbinsX(); //DeltaEta
  int nPhi = h->GetNbinsY(); //DeltaPhi
                             //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Will symmetrize histo:" << h->GetName() << endl;
                             //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) nEta:" << nEta << endl;
                             //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) nPhi:" << nPhi << endl;
  int nEtaHalf = (nEta-1)/2;
  int nPhiHalf = (nPhi-2)/2;
  int iEta, iPhi, iPhi1, iEta1;
  double * v = new double[nEta*nPhi];
  double * ev = new double[nEta*nPhi];
  //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Arrays created" << endl;
  for (int iPhi=0;iPhi<nPhi;iPhi++)
    {
    for (int iEta=0;iEta<nEta;iEta++)
      {
      iPhi1 = iPhi+1;
      iEta1 = iEta+1;
      v[ iEta+iPhi*nEta]  = h->GetBinContent(iEta1,iPhi1);
      ev[iEta+iPhi*nEta]  = h->GetBinError(  iEta1,iPhi1);
      }
    }
  //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Arrays copied" << endl;
  for (iEta=0;iEta<nEtaHalf;iEta++)
    {
    iEta1 = iEta+1;
    for (iPhi=0; iPhi<nPhiHalf;iPhi++)
      {
      iPhi1 = iPhi+1;
      v1 = v[  nEta-iEta1+(nPhi-iPhi1)*nEta];
      v2 = v[  nEta-iEta1+(     iPhi1)*nEta];
      v3 = v[        iEta+(nPhi-iPhi1)*nEta];
      v4 = v[        iEta+(     iPhi1)*nEta];
      ev1 = ev[nEta-iEta1+(nPhi-iPhi1)*nEta];
      ev2 = ev[nEta-iEta1+(     iPhi1)*nEta];
      ev3 = ev[      iEta+(nPhi-iPhi1)*nEta];
      ev4 = ev[      iEta+(     iPhi1)*nEta];
      if (ijNormalization)
        {
        sv = (v1+v2+v3+v4)/2.;
        esv = sqrt(ev1*ev1+ev2*ev2+ev3*ev3+ev4*ev4)/2.;
        }
      else
        {
        sv = (v1+v2+v3+v4)/4.;
        esv = sqrt(ev1*ev1+ev2*ev2+ev3*ev3+ev4*ev4)/4.;
        }
      h->SetBinContent( nEta-iEta, nPhi-iPhi, sv);
      h->SetBinContent( nEta-iEta,   iPhi1+1, sv);
      h->SetBinContent(     iEta1, nPhi-iPhi, sv);
      h->SetBinContent(     iEta1,   iPhi1+1, sv);
      h->SetBinError(   nEta-iEta, nPhi-iPhi, esv);
      h->SetBinError(   nEta-iEta,   iPhi1+1, esv);
      h->SetBinError(       iEta1, nPhi-iPhi, esv);
      h->SetBinError(       iEta1,   iPhi1+1, esv);
      }
    }
  //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Part 1 Done" << endl;
  iEta  = nEtaHalf;
  iEta1 = iEta+1;
  for (iPhi=0; iPhi<nPhiHalf;iPhi++) // iEta center bin
    {
    iPhi1 = iPhi+1;
    v3 = v[        iEta+(nPhi-iPhi1)*nEta];
    v4 = v[        iEta+(     iPhi1)*nEta];
    ev3 = ev[      iEta+(nPhi-iPhi1)*nEta];
    ev4 = ev[      iEta+(     iPhi1)*nEta];
    if (ijNormalization)
      {
      sv = (v3+v4);
      esv = sqrt(ev3*ev3+ev4*ev4);
      }
    else
      {
      sv = (v3+v4)/2.;
      esv = sqrt(ev3*ev3+ev4*ev4)/2.;
      }
    h->SetBinContent(     iEta1, nPhi-iPhi, sv);
    h->SetBinContent(     iEta1,   iPhi1+1, sv);
    h->SetBinError(       iEta1, nPhi-iPhi, esv);
    h->SetBinError(       iEta1,   iPhi1+1, esv);
    }
  //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Part 2 Done" << endl;

  iPhi = 0;
  iPhi1 = iPhi+1;
  for (iEta=0;iEta<nEtaHalf;iEta++)
    {
    iEta1 = iEta+1;
    v1 = v[  nEta-iEta1];
    v3 = v[        iEta];
    ev1 = ev[nEta-iEta1];
    ev3 = ev[      iEta];
    if (ijNormalization)
      {
      sv = (v1+v3);
      esv = sqrt(ev1*ev1+ev3*ev3);
      }
    else
      {
      sv = (v1+v3)/2.;
      esv = sqrt(ev1*ev1+ev3*ev3)/2.;
      }
    h->SetBinContent( nEta-iEta, 1, sv);
    h->SetBinContent(     iEta1, 1, sv);
    h->SetBinError(   nEta-iEta, 1, esv);
    h->SetBinError(       iEta1, 1, esv);

    iPhi  = nPhi/2;
    iPhi1 = iPhi+1;
    v1 = v[  nEta-iEta1 + iPhi*nEta];
    v3 = v[        iEta + iPhi*nEta];
    ev1 = ev[nEta-iEta1 + iPhi*nEta];
    ev3 = ev[      iEta + iPhi*nEta];
    if (ijNormalization)
      {
      sv = (v1+v3);
      esv = sqrt(ev1*ev1+ev3*ev3);
      }
    else
      {
      sv = (v1+v3)/2.;
      esv = sqrt(ev1*ev1+ev3*ev3)/2.;
      }
    h->SetBinContent( nEta-iEta, iPhi1, sv);
    h->SetBinContent(     iEta1, iPhi1, sv);
    h->SetBinError(   nEta-iEta, iPhi1, esv);
    h->SetBinError(       iEta1, iPhi1, esv);

    }
  //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) All Done now delete" << endl;
  delete[] v;
  //if (reportDebug(__FUNCTION__)) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Really All Done" << endl;

}

void HistogramCollection::symmetrizeXX(TH2 * h, bool ijNormalization)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return;

  double v1, v2;
  double ev1, ev2;
  double sv, esv;
  int n_x = h->GetNbinsX(); //DeltaEta
  int n_y = h->GetNbinsY(); //DeltaPhi
  int i_x, i_y, i_x1, i_y1;
  double * v  = new double[n_x*n_y];
  double * ev = new double[n_x*n_y];
  for (i_x=0; i_x<n_x; i_x++)
    {
    i_x1 = i_x+1;
    for (i_y=0; i_y<n_y; i_y++)
      {
      i_y1 = i_y+1;
      v[ i_x+i_y*n_x]  = h->GetBinContent(i_x1,i_y1);
      ev[i_x+i_y*n_x]  = h->GetBinError(  i_x1,i_y1);
      }
    }
  for (i_x=0; i_x<n_x; i_x++)
    {
    i_x1 = i_x+1;
    for (i_y=0; i_y<n_y; i_y++)
      {
      i_y1 = i_y+1;
      v1 = v[  i_x+i_y*n_x ];
      v2 = v[  i_y+i_x*n_x ];
      ev1 = ev[ i_x+i_y*n_x ];
      ev2 = ev[ i_y+i_x*n_x];
      if (ijNormalization)
        {
        sv = (v1+v2);
        esv = sqrt(ev1*ev1+ev2*ev2);
        }
      else
        {
        sv = (v1+v2)/2.;
        esv = sqrt(ev1*ev1+ev2*ev2)/2.;
        }
      h->SetBinContent( i_x1, i_y1, sv);
      h->SetBinContent( i_y1, i_x1, sv);
      h->SetBinError(   i_x1, i_y1, esv);
      h->SetBinError(   i_y1, i_x1, esv);
      }
    }
  delete[] v;
}

//  void reduce_n2xEtaPhi_n2DetaDphi(const TH1 * source, TH2 * target,int nEtaBins,int nPhiBins)
//  {
//  //if (reportDebug(__FUNCTION__)) cout << "reduce_n2xEtaPhi_n2DetaDphi() +++++++++++++ New Version  from TH1" << endl;
//  double v1,v2,ev1;
//  int dPhi,dEta, iPhi,iEta,jPhi,jEta, i, j;
//  int nBins = nEtaBins*nPhiBins;
//  int nWrk  = nPhiBins*(2*nEtaBins-1);
//  int index;
//  double * numerator    = new double[nWrk];
//  double * numeratorErr = new double[nWrk];
//  double * denominator  = new double[nWrk];
//  for (int k=0;k<nWrk;++k)
//    {
//    numerator[k]    = 0;
//    numeratorErr[k] = 0;
//    denominator[k]  = 0;
//    }
//
//  i=0;
//  for (iEta=0;iEta<nEtaBins; ++iEta)
//    {
//    for (iPhi=0;iPhi<nPhiBins; ++iPhi)
//      {
//      j=0;
//      for (jEta=0;jEta<nEtaBins; ++jEta)
//        {
//        for (jPhi=0;jPhi<nPhiBins; ++jPhi)
//          {
//          dPhi = iPhi-jPhi; if (dPhi<0) dPhi += nPhiBins; dPhi+=1;
//          dEta = iEta-jEta + nEtaBins;
//          v1   = source->GetBinContent(i*nBins+j+1);
//          ev1  = source->GetBinError(i*nBins+j+1);
//          //if (v1>-0.999999)
//          //{
//          index = (dEta-1)*nPhiBins + dPhi-1;
//          numerator[index]    += v1;
//          numeratorErr[index] += ev1*ev1;
//          denominator[index]  += 1.;
//          //}
//          ++j;
//          }
//        }
//      ++i;
//      }
//    }
//  for (dEta=0;dEta<2*nEtaBins-1;++dEta)
//    {
//    for (dPhi=0;dPhi<nPhiBins;++dPhi)
//      {
//      v1   = target->GetBinContent(dEta+1,dPhi+1);
//      ev1  = target->GetBinError(dEta+1,dPhi+1);
//      index = dEta*nPhiBins + dPhi;
//      v1    = numerator[index];
//      ev1   = numeratorErr[index];
//      v2    = denominator[index];
//      if (v2<=0) if (reportDebug(__FUNCTION__)) cout << "miserable idiot!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//      target->SetBinContent(dEta+1,dPhi+1,v1/v2);
//      target->SetBinError(  dEta+1,dPhi+1,sqrt(ev1)/v2);
//      }
//    }
//  delete [] numerator;
//  delete [] numeratorErr;
//  delete [] denominator;
//  }

void HistogramCollection::reduce_n2xEtaPhi_n2DetaDphi(const TH2 * source, TH2 * target,int nEtaBins,int nPhiBins)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,source,target)) return;

  //if (reportDebug(__FUNCTION__)) cout << "reduce_n2xEtaPhi_n2DetaDphi() ==============  New Version From TH2" << endl;
  double v1,v2,ev1;
  int dPhi,dEta, iPhi,iEta,jPhi,jEta, i, j;
  //int nBins = nEtaBins*nPhiBins;
  int nWrk  = nPhiBins*(2*nEtaBins-1);
  int index;
  double * numerator    = new double[nWrk];
  double * numeratorErr = new double[nWrk];
  double * denominator  = new double[nWrk];
  for (int k=0;k<nWrk;++k)
    {
    numerator[k]    = 0;
    numeratorErr[k] = 0;
    denominator[k]  = 0;
    }

  TString name = target->GetName();

  i=1;
  for (iEta=0;iEta<nEtaBins; ++iEta)
    {
    for (iPhi=0;iPhi<nPhiBins; ++iPhi)
      {
      j=1;
      for (jEta=0;jEta<nEtaBins; ++jEta)
        {
        for (jPhi=0;jPhi<nPhiBins; ++jPhi)
          {
          dPhi = iPhi-jPhi; if (dPhi<0) dPhi += nPhiBins; dPhi+=1;
          dEta = iEta-jEta + nEtaBins;
          v1   = source->GetBinContent(i, j);
          ev1  = source->GetBinError(  i, j);
          index = (dEta-1)*nPhiBins + dPhi-1;
          numerator[index]    += v1;
          numeratorErr[index] += ev1*ev1;
          denominator[index]  += 1.;
          //cout << " " << name << "  iEta:" << iEta << " iPhi:" << iPhi << " jEta:" << jEta << " jPhi:" << jPhi << " v1:" << v1 << " ev1:" << ev1 << endl;
          ++j;
          }
        }
      ++i;
      }
    }
  for (dEta=0;dEta<2*nEtaBins-1;++dEta)
    {
    for (dPhi=0;dPhi<nPhiBins;++dPhi)
      {
      //v1   = target->GetBinContent(dEta+1,dPhi+1);
      //ev1  = target->GetBinError(dEta+1,dPhi+1);
      index = dEta*nPhiBins + dPhi;
      v1    = numerator[index];
      ev1   = numeratorErr[index];
      v2    = denominator[index];
      if (v2<=0) if (reportDebug(__FUNCTION__)) cout << endl << "HistogramCollection::reduce_n2xEtaPhi_n2DetaDphi() Elements of denominator are negative." << endl;
      target->SetBinContent(dEta+1,dPhi+1,v1/v2);
      target->SetBinError(  dEta+1,dPhi+1,sqrt(ev1)/v2);
      }
    }

  // double a1m = target->GetBinContent(nEtaBins-1,1);
  // double a1  = target->GetBinContent(nEtaBins,1);
  // double a1p = target->GetBinContent(nEtaBins+1,1);
  // if (reportDebug(__FUNCTION__)) cout << " a1m:" << a1m << endl;
  // if (reportDebug(__FUNCTION__)) cout << " a1 :" << a1 << endl;
  // if (reportDebug(__FUNCTION__)) cout << " a1p:" << a1p << endl;
  // target->SetBinContent(nEtaBins,1,0.5*(a1m+a1p));


  delete [] numerator;
  delete [] numeratorErr;
  delete [] denominator;
}

//!
//! Calculate the external product of n_1(eta,phi) by n_2(eta,phi) and project onto n1n1_12(Deta,Dphi)
//! First create a 4D external product of h_1 and h_2 using a 1D vector
//! than project it onto DeltaEta(x-axis) DeltaPhi (y-axis)
//! Errors are neglected and must be accounted for using a sub-sample analysis.
//!
void HistogramCollection::reduce_n1EtaPhiN1EtaPhiOntoN1N1DetaDphi(const TH2 * h_1, TH2 * h_2, TH2 * h_12, int nDeta,int nDphi)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_1,h_2,h_12)) return;
  if (!sameDimensions(__FUNCTION__,h_1,h_2)) return;

  vector<double> numerator(nDeta*nDphi,0.0);
  vector<double> denominator(nDeta*nDphi,0.0);
  int nEta = h_1->GetNbinsX();
  int nPhi = h_1->GetNbinsY();
  int index;

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "         nEta:" << nEta << endl;
    cout << "         nPhi:" << nPhi << endl;
    cout << "        nDeta:" << nDeta << endl;
    cout << "        nDphi:" << nDphi << endl;
    cout << "  nDeta*nDphi:" << nDeta*nDphi << endl;
    }

  int iDeta, iDphi;
  double v1, v2, v, r;
  for (int iPhi1=1; iPhi1<=nPhi; iPhi1++)
    {
    for (int iEta1=1; iEta1<=nEta; iEta1++)
      {
      v1 = h_1->GetBinContent(iEta1,iPhi1);
      for (int iPhi2=1; iPhi2<=nPhi; iPhi2++)
        {
        for (int iEta2=1; iEta2<=nEta; iEta2++)
          {
          v2 = h_2->GetBinContent(iEta2,iPhi2);
          v  = v1 * v2;
          iDphi = iPhi1-iPhi2; if (iDphi<0) iDphi += nDphi; //dPhi+=1;
          iDeta = iEta1-iEta2 + nEta - 1;
          // if (iDphi<0 || )
          index = iDphi*nDeta + iDeta;
          if (index<0)
            {
            cout << "index<0" << endl;
            exit(1);
            }
          if (index>=nDeta*nDphi)
            {
            cout << "index>nDeta*nDphi" << endl;
            exit(1);
            }
          numerator[index]    += v;
          denominator[index]  += 1.0;
          }
        }
      }
    }
  if (reportDebug(__FUNCTION__)) cout << "Compute ratio and fill target histogram." << endl;
  double zero = 0.0;
  for (iDphi=0; iDphi<nDphi; iDphi++)
    {
    for (iDeta=0; iDeta<nDeta; iDeta++)
      {
      index = iDphi*nDeta + iDeta;
      double num = numerator[index];
      double den = denominator[index];
      //cout << " iDphi: " << iDphi << " iDeta:" << iDeta << " index:" << index << " num:" << num << " denom:" << den << endl;
      //if (den>0) r = num/den;
      if (den>0) r = num;
      else  r = zero;
      h_12->SetBinContent(iDeta+1,iDphi+1,r);
      h_12->SetBinError(iDeta+1,iDphi+1,zero);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void HistogramCollection::reduce_n2xEtaPhi_n2EtaEta(const TH1 * source, TH2 * target,int nEtaBins,int nPhiBins)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,source,target)) return;

  double v1,v2,ev1,ev2,v,ev;
  int iPhi,iEta,jPhi,jEta, i, j;
  int nBins = nEtaBins*nPhiBins;
  int nEtaEta = nEtaBins*nEtaBins;
  double * work = new double[nEtaEta];
  for (int k=0;k<nEtaEta;++k) work[k]=0;
  i=0;
  for (iEta=0;iEta<nEtaBins; ++iEta)
    {
    for (iPhi=0;iPhi<nPhiBins; ++iPhi)
      {
      j=0;
      for (jEta=0;jEta<nEtaBins; ++jEta)
        {
        for (jPhi=0;jPhi<nPhiBins; ++jPhi)
          {
          v1   = source->GetBinContent(i*nBins+j+1);
          ev1  = source->GetBinError(i*nBins+j+1);
          if (v1> -0.9999999)
            {
            v2   = target->GetBinContent(iEta+1,jEta+1);
            ev2  = target->GetBinError(iEta+1,jEta+1);
            v = v1+v2;
            ev = sqrt(ev1*ev1+ev2*ev2);
            target->SetBinContent(iEta+1,jEta+1,v);
            target->SetBinError(iEta+1,jEta+1,ev);
            work[ iEta*nEtaBins+jEta] += 1;
            }
          ++j;
          }
        }
      ++i;
      }
    }
  for (iEta=0;iEta<nEtaBins; ++iEta)
    {
    for (jEta=0;jEta<nEtaBins; ++jEta)
      {
      v1   = target->GetBinContent(iEta+1,jEta+1);
      ev1  = target->GetBinError(iEta+1,jEta+1);
      v2   = work[ iEta*nEtaBins+jEta ];
      if (v2<=0)
        if (reportFatal())
          {
          cout << endl;
          cout << endl << "Serious BUG !!!!!!!!!!!!!!!!!!!!!!!" << endl;
          delete [] work;
          return;
          }
      target->SetBinContent(iEta+1,jEta+1,v1/v2);
      target->SetBinError(iEta+1,jEta+1,ev1/v2);
      }
    }
  delete [] work;
}

void HistogramCollection::project_n2XYXY_n2XX(const TH2 * source, TH2 * target,int nXBins,int nYBins)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,source,target)) return;

  double v1,v2,ev1,ev2,v,ev;

  /* sanity checks */
  if ((source->GetNbinsX() != nXBins*nYBins)
      or (source->GetNbinsY() != nXBins*nYBins)
      or (target->GetNbinsX() != nXBins)
      or (target->GetNbinsY() != nXBins))
    {
    if (reportError(__FUNCTION__)) cout << endl << "Inconsistent indexes for histogram " << source->GetName() << endl;
    return;
    }

  target->Reset();

  int i=0;
  for (int iX=0;iX<nXBins; ++iX)
    {
    for (int iY=0;iY<nYBins; ++iY)
      {
      int j=0;
      for (int jX=0;jX<nXBins; ++jX)
        {
        for (int jY=0;jY<nYBins; ++jY)
          {
          v1   = source->GetBinContent(i+1,j+1);
          ev1  = source->GetBinError(i+1,j+1);
          v2   = target->GetBinContent(iX+1,jX+1);
          ev2  = target->GetBinError(iX+1,jX+1);
          v = v1+v2;
          ev = sqrt(ev1*ev1+ev2*ev2);
          target->SetBinContent(iX+1,jX+1,v);
          target->SetBinError(iX+1,jX+1,ev);
          ++j;
          }
        }
      ++i;
      }
    }
  target->SetEntries(source->GetEntries());
}

void HistogramCollection::project_n2XYXY_n2YY(const TH2 * source, TH2 * target,int nXBins,int nYBins)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,source,target)) return;

  double v1,v2,ev1,ev2,v,ev;

  /* sanity checks */
  if ((source->GetNbinsX() != nXBins*nYBins)
      or (source->GetNbinsY() != nXBins*nYBins)
      or (target->GetNbinsX() != nYBins)
      or (target->GetNbinsY() != nYBins)) {
    if (reportError(__FUNCTION__))  cout << endl << "Inconsistent indexes for histogram " << source->GetName() << endl;
    return;
  }

  target->Reset();

  int i=0;
  for (int iX=0;iX<nXBins; ++iX)
    {
    for (int iY=0;iY<nYBins; ++iY)
      {
      int j=0;
      for (int jX=0;jX<nXBins; ++jX)
        {
        for (int jY=0;jY<nYBins; ++jY)
          {
          v1   = source->GetBinContent(i+1,j+1);
          ev1  = source->GetBinError(i+1,j+1);
          v2   = target->GetBinContent(iY+1,jY+1);
          ev2  = target->GetBinError(iY+1,jY+1);
          v = v1+v2;
          ev = sqrt(ev1*ev1+ev2*ev2);
          target->SetBinContent(iY+1,jY+1,v);
          target->SetBinError(iY+1,jY+1,ev);
          ++j;
          }
        }
      ++i;
      }
    }
  target->SetEntries(source->GetEntries());
}

//! Depracated--dont use..
TH2* HistogramCollection::symmetrize(TH2* h)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h)) return nullptr;
  return h;
}

///shift the given source to the target vertically by nbins
void HistogramCollection::shiftY(const TH2 & source, TH2 & target, int nbins)
{

  if (reportStart(__FUNCTION__))
    ;

  int i_x, i_y;
  int n_x = source.GetNbinsX();
  int n_y = source.GetNbinsY();

  //shift the 1st area
  for (i_x=1;i_x<=n_x;++i_x)
    {
    for (i_y=1;i_y<=n_y-nbins;++i_y)
      {
      double v  = source.GetBinContent(i_x,i_y);
      double ev = source.GetBinError(i_x,i_y);
      target.SetBinContent(i_x,  i_y+nbins, v);
      target.SetBinError(i_x,    i_y+nbins, ev);
      }
    for (i_y=n_y-nbins+1;i_y<=n_y;++i_y)
      {
      double v  = source.GetBinContent(i_x,i_y);
      double ev = source.GetBinError(i_x,i_y);
      target.SetBinContent(i_x, i_y-(n_y-nbins),v);
      target.SetBinError(i_x,   i_y-(n_y-nbins),ev);
      }
    }
}

////////////////////////////////////////////////////////////////////////
float * HistogramCollection::getFloatArray(int size, float v)
{
  /// Allocate an array of type float with n values
  /// Initialize the array to the given value
  float * array = new float [size];
  for (int i=0;i<size;++i) array[i]=v;
  return array;
}


////////////////////////////////////////////////////////////////////////
double * HistogramCollection::getDoubleArray(int size, double v)
{
  /// Allocate an array of type double with n values
  /// Initialize the array to the given value
  double * array = new double [size];
  for (int i=0;i<size;++i) array[i]=v;
  return array;
}

////////////////////////////////////////////////////////////////////////
void HistogramCollection::resetDoubleArray(int n, double * array, double value)
{
  /// Set all n values of the given array to the given value
  double * ptr;
  double * ptrEnd = array+n;
  for (ptr=array; ptr<ptrEnd; ++ptr)
    {
    *ptr = value;
    }
}


int HistogramCollection::index2(int i1, int i2)
{
  int v;
  switch (i1)
    {
      case 0:
      switch (i2)
        {
          case 0: v = 0; break;
          case 1: v = 1; break;
          case 2: v = 2; break;
          case 3: v = 3; break;
          default: v = -1; break;
        }
      break;
      case 1:
      switch (i2)
        {
          case 1: v = 4; break;
          case 2: v = 5; break;
          case 3: v = 6; break;
          default: v = -1; break;
        }
      break;
      case 2:
      switch (i2)
        {
          case 2: v = 7; break;
          case 3: v = 8; break;
          default: v = -1; break;
        }
      break;
      case 3:
      switch (i2)
        {
          case 3: v = 9; break;
          default: v = -1; break;
        }
      break;
      default: v = -1; break;
    }
  if (v<0)
    if (reportWarning("index2(int i1, int i2)"))
      cout << "Combination i1:" << i1 << " i2:" << i2 << " is not allowed." << endl;
  return v;
}

int HistogramCollection::index3(int i1, int i2, int i3)
{
  int v;
  switch (i1)
    {
      case 0:
      switch (i2)
        {
          case 0:
          switch (i3)
            {
              case 0: v = 0; break;
              case 1: v = 1; break;
              case 2: v = 2; break;
              case 3: v = 3; break;
              default: v = -1; break;
            }
          break;
          case 1:
          switch (i3)
            {
              case 1: v = 4; break;
              case 2: v = 5; break;
              case 3: v = 6; break;
              default: v = -1; break;
            }
          break;
          case 2:
          switch (i3)
            {
              case 2: v = 7; break;
              case 3: v = 8; break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3: v = 9; break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 1:
      switch (i2)
        {
          case 1:
          switch (i3)
            {
              case 1: v = 10; break;
              case 2: v = 11; break;
              case 3: v = 12; break;
              default: v = -1; break;
            }
          break;
          case 2:
          switch (i3)
            {
              case 2: v = 13; break;
              case 3: v = 14; break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3: v = 15; break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 2:
      switch (i2)
        {
          case 2:
          switch (i3)
            {
              case 2: v = 16; break;
              case 3: v = 17; break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3: v = 18; break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 3:
      switch (i2)
        {
          case 3:
          switch (i3)
            {
              case 3: v = 19; break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      default: v = -1; break;
    }
  if (v<0)
    if (reportWarning("index2(int i1, int i2)"))
      cout << "Combination i1:" << i1 << " i2:" << i2 <<  " i3:" << i3 << " is not allowed." << endl;
  return v;
}

int HistogramCollection::index4(int i1, int i2, int i3, int i4)
{
  int v;
  switch (i1)
    {
      case 0:
      switch (i2)
        {
          case 0:
          switch (i3)
            {
              case 0:
              switch (i4)
                {
                  case 0: v = 0; break;
                  case 1: v = 1; break;
                  case 2: v = 2; break;
                  case 3: v = 3; break;
                  default: v = -1; break;
                }
              break;
              case 1:
              switch (i4)
                {
                  case 1: v = 4; break;
                  case 2: v = 5; break;
                  case 3: v = 6; break;
                  default: v = -1; break;
                }
              break;
              case 2:
              switch (i4)
                {
                  case 2: v = 7; break;
                  case 3: v = 8; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 9; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 1:
          switch (i3)
            {
              case 1:
              switch (i4)
                {
                  case 1: v = 10; break;
                  case 2: v = 11; break;
                  case 3: v = 12; break;
                  default: v = -1; break;
                }
              break;
              case 2:
              switch (i4)
                {
                  case 2: v = 13; break;
                  case 3: v = 14; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 15; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 2:
          switch (i3)
            {
              case 2:
              switch (i4)
                {
                  case 2: v = 16; break;
                  case 3: v = 17; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 18; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3:
              switch (i4)
                {
                  case 3: v = 19; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 1:
      switch (i2)
        {
          case 1:
          switch (i3)
            {
              case 1:
              switch (i4)
                {
                  case 1: v = 20; break;
                  case 2: v = 21; break;
                  case 3: v = 22; break;
                  default: v = -1; break;
                }
              break;
              case 2:
              switch (i4)
                {
                  case 2: v = 23; break;
                  case 3: v = 24; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 25; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 2:
          switch (i3)
            {
              case 2:
              switch (i4)
                {
                  case 2: v = 26; break;
                  case 3: v = 27; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 28; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3:
              switch (i4)
                {
                  case 3: v = 29; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 2:
      switch (i2)
        {
          case 2:
          switch (i3)
            {
              case 2:
              switch (i4)
                {
                  case 2: v = 30; break;
                  case 3: v = 31; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 32; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3:
              switch (i4)
                {
                  case 3: v = 33; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 3:
      switch (i2)
        {
          case 3:
          switch (i3)
            {
              case 3:
              switch (i4)
                {
                  case 3: v = 34; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      default: v = -1; break;
    }
  if (v<0) cout << "<E> HistogramCollection::index3(int i1, int i2, int i3, int i4) Combination i1:" << i1 << " i2:" << i2 <<  " i3:" << i3 <<  " i4:" << i4 << " is not allowed." << endl;
  return v;
}

void HistogramCollection::calculateF2R2(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f2_12, TH1* h_F2_12, TH1* h_R2_12)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_f1_1, h_f1_2, h_f2_12, h_F2_12, h_R2_12)) return;

  double f1_1, ef1_1, f1_2, ef1_2;
  double f2_12, ef2_12;
  double F2_12, eF2_12, R2_12, eR2_12;
  int nBins = h_f1_1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    f1_1   = h_f1_1->GetBinContent(iBin);   ef1_1 =   h_f1_1->GetBinError(iBin);
    f1_2   = h_f1_2->GetBinContent(iBin);   ef1_2  =  h_f1_2->GetBinError(iBin);
    f2_12  = h_f2_12->GetBinContent(iBin);  ef2_12 =  h_f2_12->GetBinError(iBin);
    calculateF2R2(f1_1,ef1_1,f1_2,ef1_2,f2_12,ef2_12, F2_12,eF2_12,R2_12,eR2_12);
    h_F2_12->SetBinContent(iBin, F2_12);
    h_F2_12->SetBinError(iBin,   eF2_12);
    h_R2_12->SetBinContent(iBin, R2_12);
    h_R2_12->SetBinError(iBin,   eR2_12);
    }
}

void HistogramCollection::calculateNudyn(TH1* h_R2_11, TH1* h_R2_12, TH1* h_R2_22, TH1* h_nudyn_12)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_R2_11, h_R2_12, h_R2_22, h_nudyn_12)) return;

  double R2_11, eR2_11, R2_12, eR2_12, R2_22, eR2_22, nudyn, enudyn;
  int nBins = h_R2_11->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    R2_11   = h_R2_11->GetBinContent(iBin);  eR2_11  =  h_R2_11->GetBinError(iBin);
    R2_12   = h_R2_12->GetBinContent(iBin);  eR2_12  =  h_R2_12->GetBinError(iBin);
    R2_22   = h_R2_22->GetBinContent(iBin);  eR2_22  =  h_R2_22->GetBinError(iBin);
    calculateNudyn(R2_11, eR2_11, R2_12, eR2_12, R2_22, eR2_22, nudyn, enudyn);
    h_nudyn_12->SetBinContent(iBin, nudyn);
    h_nudyn_12->SetBinError(iBin,   enudyn);
    }
}


void HistogramCollection::calculateF3R3(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f1_3,
                                        TH1* h_f2_12, TH1* h_f2_13, TH1* h_f2_23,
                                        TH1* h_f3_123,
                                        TH1* h_F3_123, TH1* h_R3_123)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_f1_1, h_f1_2, h_f1_3, h_f2_12, h_f2_13, h_f2_23, h_f3_123, h_F3_123, h_R3_123)) return;

  double f1_1, ef1_1, f1_2, ef1_2, f1_3, ef1_3;
  double f2_12, ef2_12, f2_13, ef2_13, f2_23, ef2_23;
  double f3_123, ef3_123;
  double F3_123, eF3_123, R3_123, eR3_123;
  int nBins = h_f1_1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    f1_1   = h_f1_1->GetBinContent(iBin);    ef1_1 =   h_f1_1->GetBinError(iBin);
    f1_2   = h_f1_2->GetBinContent(iBin);    ef1_2  =  h_f1_2->GetBinError(iBin);
    f1_3   = h_f1_3->GetBinContent(iBin);    ef1_3  =  h_f1_2->GetBinError(iBin);
    f2_12  = h_f2_12->GetBinContent(iBin);   ef2_12 =  h_f2_12->GetBinError(iBin);
    f2_13  = h_f2_13->GetBinContent(iBin);   ef2_13 =  h_f2_13->GetBinError(iBin);
    f2_23  = h_f2_23->GetBinContent(iBin);   ef2_23 =  h_f2_23->GetBinError(iBin);
    f3_123 = h_f3_123->GetBinContent(iBin);  ef3_123 =  h_f3_123->GetBinError(iBin);
    calculateF3R3(f1_1,ef1_1,f1_2,ef1_2,f1_3,ef1_3,
                  f2_12,ef2_12,f2_13,ef2_13,f2_23,ef2_23,
                  f3_123,ef3_123,
                  F3_123,eF3_123,R3_123,eR3_123);
    h_F3_123->SetBinContent(iBin, F3_123);
    h_F3_123->SetBinError(iBin,   eF3_123);
    h_R3_123->SetBinContent(iBin, R3_123);
    h_R3_123->SetBinError(iBin,   eR3_123);
    }
}

void HistogramCollection::calculateF4R4(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f1_3, TH1* h_f1_4,
                                        TH1* h_f2_12, TH1* h_f2_13, TH1* h_f2_14,
                                        TH1* h_f2_23, TH1* h_f2_24, TH1* h_f2_34,
                                        TH1* h_f3_123, TH1* h_f3_124, TH1* h_f3_134, TH1* h_f3_234,
                                        TH1* h_f4_1234,
                                        TH1* h_F4_1234, TH1* h_R4_1234)
{

  if (reportStart(__FUNCTION__))
    ;
  if (!ptrExist(__FUNCTION__,h_f1_1, h_f1_2, h_f1_3, h_f1_4, h_f2_12,  h_f2_13,  h_f2_14)) return;
  if (!ptrExist(__FUNCTION__,h_f2_23,h_f2_24,h_f2_34,h_f3_123,h_f3_124,h_f3_134,h_f3_234)) return;
  if (!ptrExist(__FUNCTION__,h_f4_1234,h_F4_1234,h_R4_1234)) return;

  double f1_1, ef1_1, f1_2, ef1_2, f1_3, ef1_3, f1_4, ef1_4;
  double f2_12, ef2_12, f2_13, ef2_13, f2_14, ef2_14, f2_23, ef2_23, f2_24, ef2_24, f2_34, ef2_34;
  double f3_123, ef3_123, f3_124, ef3_124, f3_134, ef3_134, f3_234, ef3_234;
  double f4_1234, ef4_1234;
  double F4_1234, eF4_1234, R4_1234, eR4_1234;
  int nBins = h_f1_1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    f1_1    = h_f1_1->GetBinContent(iBin);   ef1_1   =  h_f1_1->GetBinError(iBin);
    f1_2    = h_f1_2->GetBinContent(iBin);   ef1_2   =  h_f1_2->GetBinError(iBin);
    f1_3    = h_f1_3->GetBinContent(iBin);   ef1_3   =  h_f1_3->GetBinError(iBin);
    f1_4    = h_f1_4->GetBinContent(iBin);   ef1_4   =  h_f1_4->GetBinError(iBin);
    f2_12   = h_f2_12->GetBinContent(iBin);  ef2_12  =  h_f2_12->GetBinError(iBin);
    f2_13   = h_f2_13->GetBinContent(iBin);  ef2_13  =  h_f2_13->GetBinError(iBin);
    f2_14   = h_f2_14->GetBinContent(iBin);  ef2_14  =  h_f2_14->GetBinError(iBin);
    f2_23   = h_f2_23->GetBinContent(iBin);  ef2_23  =  h_f2_23->GetBinError(iBin);
    f2_24   = h_f2_24->GetBinContent(iBin);  ef2_24  =  h_f2_24->GetBinError(iBin);
    f2_34   = h_f2_34->GetBinContent(iBin);  ef2_34  =  h_f2_34->GetBinError(iBin);
    f3_123  = h_f3_123->GetBinContent(iBin); ef3_123 =  h_f3_123->GetBinError(iBin);
    f3_124  = h_f3_124->GetBinContent(iBin); ef3_124 =  h_f3_124->GetBinError(iBin);
    f3_134  = h_f3_134->GetBinContent(iBin); ef3_134 =  h_f3_134->GetBinError(iBin);
    f3_234  = h_f3_234->GetBinContent(iBin); ef3_234 =  h_f3_234->GetBinError(iBin);
    f4_1234 = h_f4_1234->GetBinContent(iBin);ef4_1234 =  h_f4_1234->GetBinError(iBin);
    calculateF4R4(f1_1,ef1_1,f1_2,ef1_2,f1_3,ef1_3,f1_4,ef1_4,
                  f2_12,ef2_12, f2_13,ef2_13, f2_14,ef2_14,
                  f2_23,ef2_23, f2_24,ef2_24, f2_34,ef2_34,
                  f3_123,ef3_123, f3_124,ef3_124, f3_134,ef3_134, f3_234,ef3_234,
                  f4_1234,ef4_1234,
                  F4_1234,eF4_1234,R4_1234,eR4_1234);
    h_F4_1234->SetBinContent(iBin, F4_1234);
    h_F4_1234->SetBinError(iBin,   eF4_1234);
    h_R4_1234->SetBinContent(iBin, R4_1234);
    h_R4_1234->SetBinError(iBin,   eR4_1234);
    }
}


void HistogramCollection::calculateF2R2(double f1_1,double ef1_1,double f1_2,double ef1_2, double f2_12,double ef2_12,
                                        double & F2_12,double & eF2_12, double &  R2_12,double & eR2_12)
{

  if (reportStart(__FUNCTION__))
    ;
  double ref1_1, ref1_2, ref2_12;
  if (f1_1<1E-20 || f1_2<1E-20)
    {
    F2_12  = 0.0;
    eF2_12 = 0.0;
    R2_12  = 0.0;
    eR2_12 = 0.0;
    }
  else
    {
    F2_12     = f2_12 - f1_1*f1_2;
    ref1_1    = ef1_1/f1_1;
    ref1_2    = ef1_2/f1_2;
    ref2_12   = ef2_12/f2_12;
    eF2_12    = sqrt(f1_1*f1_2*f1_1*f1_2*(ref1_1*ref1_1 + ref1_2*ref1_2) + eF2_12*eF2_12);
    R2_12     = F2_12/(f1_1*f1_2);
    eR2_12    = R2_12*sqrt(ref1_1*ref1_1 + ref1_2*ref1_2 + ref2_12*ref2_12);
    }
}


void HistogramCollection::calculateF3R3(double f1_1,double ef1_1,double f1_2,double ef1_2, double f1_3,double ef1_3,
                                        double f2_12,double ef2_12,double f2_13,double ef2_13,double f2_23,double ef2_23,
                                        double f3_123, double ef3_123,
                                        double & F3_123,double & eF3_123, double &  R3_123,double & eR3_123)
{

  if (reportStart(__FUNCTION__))
    ;
  double ref1_1, ref1_2, ref1_3, ref2_12, ref2_13, ref2_23, reF3_123;
  if (f1_1<1E-20 || f1_2<1E-20 || f1_3<1E-20)
    {
    F3_123   = 0.0;
    eF3_123  = 0.0;
    R3_123   = 0.0;
    eR3_123  = 0.0;
    }
  else
    {
    F3_123    = f3_123 - f2_12*f1_3 - f2_13*f1_2 - f2_23*f1_1 + 2.0*f1_1*f1_2*f1_3;
    ref1_1    = ef1_1/f1_1;
    ref1_2    = ef1_2/f1_2;
    ref1_3    = ef1_3/f1_3;
    ref2_12   = ef2_12/f2_12;
    ref2_13   = ef2_13/f2_13;
    ref2_23   = ef2_23/f2_23;
    eF3_123   = ef3_123*ef3_123;
    eF3_123   += f2_12*f2_12*f1_3*f1_3*(ref2_12*ref2_12 + ref1_3*ref1_3);
    eF3_123   += f2_13*f2_13*f1_2*f1_2*(ref2_13*ref2_13 + ref1_2*ref1_2);
    eF3_123   += f2_23*f2_23*f1_1*f1_1*(ref2_23*ref2_23 + ref1_1*ref1_1);
    eF3_123   += 4.0*f1_1*f1_1*f1_2*f1_2*f1_3*f1_3*sqrt(ref1_1*ref1_1+ref1_2*ref1_2+ref1_3*ref1_3);
    eF3_123   = sqrt(eF3_123);
    reF3_123   = eF3_123/F3_123;
    R3_123     = F3_123/(f1_1*f1_2*f1_3);
    eR3_123    = R3_123*reF3_123;
    }
}

void HistogramCollection::calculateF4R4(double f1_1,double ef1_1,double f1_2,double ef1_2, double f1_3,double ef1_3, double f1_4,double ef1_4,
                                        double f2_12,double ef2_12,double f2_13,double ef2_13,double f2_14,double ef2_14,double f2_23,double ef2_23,
                                        double f2_24,double ef2_24,double f2_34,double ef2_34,
                                        double f3_123, double ef3_123,double f3_124, double ef3_124, double f3_134, double ef3_134,double f3_234, double ef3_234,
                                        double f4_1234, double ef4_1234,
                                        double &F4_1234,double &eF4_1234, double &  R4_1234,double & eR4_1234)
{

  if (reportStart(__FUNCTION__))
    ;

  double ref1_1,  ref1_2,  ref1_3; //,  ref1_4;
  double ref2_12, ref2_13, ref2_14, ref2_24, ref2_34;
  double ref3_123, ref3_124, ref3_134, ref3_234;
  //double ref4_1234;
  //double reF4_1234;

  if (f1_1<1E-20 || f1_2<1E-20 || f1_3<1E-20 || f1_4<1E-20)
    {
    F4_1234   = 0.0;
    eF4_1234  = 0.0;
    R4_1234   = 0.0;
    eR4_1234  = 0.0;
    }
  else
    {

    F4_1234   = f4_1234 - f3_123*f1_4 - f3_124*f1_3 - f3_134*f1_2 - f3_234*f1_1
    - f2_12*f2_34 - f2_13*f2_24 - f2_14*f2_23
    + 4.0*(f2_12*f1_3*f1_4 + f2_13*f1_2*f1_4 + f2_14*f1_2*f1_3)
    - 6.0*f1_1*f1_2*f1_3*f1_4;
    ref1_1    = ef1_1/f1_1;
    ref1_2    = ef1_2/f1_2;
    ref1_3    = ef1_3/f1_3;
    //ref1_4    = ef1_4/f1_4;
    ref2_12   = ef2_12/f2_12;
    ref2_13   = ef2_13/f2_13;
    ref2_14   = ef2_14/f2_14;
    //ref2_23   = ef2_23/f2_23;
    ref2_24   = ef2_24/f2_24;
    ref2_34   = ef2_34/f2_34;
    ref3_123  = ef3_123/f3_123;
    ref3_124  = ef3_124/f3_124;
    ref3_134  = ef3_134/f3_134;
    ref3_234  = ef3_234/f3_234;
    //ref4_1234 = ef4_1234/f4_1234;

    eF4_1234   = ef4_1234*ef4_1234
    + f3_123*f3_123*f1_4*f1_4*(ref3_123*ref3_123 + ef1_4*ef1_4)
    + f3_124*f3_124*f1_3*f1_3*(ref3_124*ref3_124 + ref1_3*ref1_3)
    + f3_134*f3_134*f1_2*f1_2*(ref3_134*ref3_134 + ref1_2*ref1_2)
    + f3_234*f3_234*f1_1*f1_1*(ref3_234*ref3_234 + ref1_1*ref1_1)
    + 16.0*f2_12*f2_12*f2_34*f2_34*(ref2_12*ref2_12 + ef2_34*ef2_34)
    + 16.0*f2_13*f2_13*f2_24*f2_24*(ref2_13*ref2_13 + ef2_24*ef2_24)
    + 16.0*f2_14*f2_14*f2_23*f2_23*(ref2_14*ref2_14 + ef2_23*ef2_23);
    // neglect singles term for now..
    eF4_1234   = sqrt(eF4_1234);
    //reF4_1234  = 0.0; //eF4_1234/F4_1234;
    R4_1234    = F4_1234/(f1_1*f1_2*f1_3*f1_4);
    eR4_1234   = 0.0; //R4_1234*reF4_1234;
    }
}

void HistogramCollection::calculateNudyn(double r2_11,double er2_11,double r2_12,double er2_12,double r2_22,double er2_22,double & nudyn,double & enudyn)
{

  if (reportStart(__FUNCTION__))
    ;
  nudyn = r2_11 + r2_22 - 2.0*r2_12;
  enudyn = sqrt(er2_11*er2_11 + er2_22*er2_22 + 4.0*er2_12*er2_12);
}

