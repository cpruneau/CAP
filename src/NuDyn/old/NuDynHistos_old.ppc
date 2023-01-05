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
#include "NuDynHistos.hpp"
ClassImp(NuDynHistos);


NuDynHistos::NuDynHistos(Task * _parent,
                         const TString & _name,
                         Configuration & _configuration)
:
Histograms(_parent,_name,_configuration),
h_eventStreams(0),
nFilters(0),
multiplicityType(_configuration.getValueInt(getName(),"multiplicityType")),
pairOnly(_configuration.getValueInt(getName(),"PairOnly")),
h_f1(),
h_f2(),
h_f3(),
h_f4(),
h_eventStreams_vsMult(nullptr),
h_f1_vsMult(),
h_f2_vsMult(),
h_f3_vsMult(),
h_f4_vsMult()
{
  appendClassName("NuDynHistos");
}


NuDynHistos::~NuDynHistos()
{
}

// for now use the same boundaries for eta and y histogram
void NuDynHistos::createHistograms()
{
  const TString & bn  = getName();
  const TString & ptn = getParentTaskName();
  const TString & ppn = getParentPathName();
  Configuration & configuration = getConfiguration();
  nFilters         = configuration.getValueInt(ppn,"nFilters");
  multiplicityType = configuration.getValueInt(ppn,"multiplicityType");
  pairOnly         = configuration.getValueBool(ppn,"PairOnly");
  int nBins_mult   = configuration.getValueInt(ppn,"nBins_mult");
  double min_mult  = configuration.getValueInt(ppn,"Min_mult");
  double max_mult  = configuration.getValueInt(ppn,"Max_mult");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  NuDyn:Parent Task Name....................: " << ptn << endl;
    cout << "  NuDyn:Parent Path Name....................: " << ppn << endl;
    cout << "  NuDyn:Histo Base Name.....................: " << bn << endl;
    cout << "  NuDyn:nFilters............................: " << nFilters <<  endl;
    cout << "  NuDyn:multiplicityType....................: " << multiplicityType << endl;
    cout << "  NuDyn:pairOnly............................: " << pairOnly << endl;
    cout << "  NuDyn:nBins_mult..........................: " << nBins_mult << endl;
    cout << "  NuDyn:min_mult............................: " << min_mult << endl;
    cout << "  NuDyn:max_mult............................: " << max_mult << endl;
    }

  TString suffix = "";
  TString xTitle = "";
  
  switch (multiplicityType)
    {
      case 0: suffix = "vsCent"; xTitle = "%"; break;
      case 1: suffix = "vsMult"; xTitle = "mult_{Tot}";  break;
      case 2: suffix = "vsMult"; xTitle = "mult_{acc}";  break;
    }
  
  h_eventStreams   = createHistogram(makeName(bn,"NeventStreams"),10,0.0,10, "Streams","n_{Events}");
  h_eventStreams_vsMult = createHistogram(makeName(bn,"NeventStreams")+suffix,nBins_mult,min_mult,  max_mult,  xTitle,"n_{Events}");
  for (unsigned int i1=0; i1<nFilters; i1++)
    {
    h_f1.push_back(        createProfile(makeName(bn,"f1",i1), 10,0.0,10, "Streams", makeName("f_{1}^{",i1,"}")) );
    h_f1_vsMult.push_back( createProfile(makeName(bn,"f1",i1,suffix),nBins_mult,min_mult,max_mult,xTitle, makeName("f_{1}^{",i1,"}") ) );
    for (unsigned int i2=i1; i2<nFilters; i2++)
      {
      h_f2.push_back(        createProfile(makeName(bn,"f2",i1,i2), 10,0.0,10, "Streams", makeName("f_{2}^{",i1,i2,"}")) );
      h_f2_vsMult.push_back( createProfile(makeName(bn,"f2",i1,i2,suffix),nBins_mult,min_mult,max_mult,xTitle,makeName("f_{2}^{",i1,i2,"}")) );
      if (!pairOnly)
        {
        for (unsigned int i3=i2; i3<nFilters; i3++)
          {
          h_f3.push_back(        createProfile(makeName(bn,"f3",i1,i2,i3), 10,0.0,10, "Streams", makeName("f_{3}^{",i1,i2,i3,"}")) );
          h_f3_vsMult.push_back( createProfile(makeName(bn,"f3",i1,i2,i3,suffix),nBins_mult,min_mult,max_mult,xTitle,makeName("f_{3}^{",i1,i2,i3,"}")) );
          for (unsigned int i4=i3; i4<nFilters; i4++)
            {
            h_f4.push_back(        createProfile(makeName(bn,"f4",i1,i2,i3,i4), 10,0.0,10, "Streams", makeName("f_{4}^{",i1,i2,i3,i4,"}")));
            h_f4_vsMult.push_back( createProfile(makeName(bn,"f4",i1,i2,i3,i4,suffix),nBins_mult,min_mult,max_mult,xTitle,makeName("f_{4}^{",i1,i2,i3,i4,"}")));
            }
          }
        } // !pairOnly
      }
    }
}

//________________________________________________________________________
void NuDynHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  const TString & bn  = getName();
  const TString & ptn = getParentTaskName();
  const TString & ppn = getParentPathName();
  Configuration & configuration = getConfiguration();
  nFilters         = configuration.getValueInt(ppn,"nFilters");
  multiplicityType = configuration.getValueInt(ppn,"multiplicityType");
  pairOnly         = configuration.getValueBool(ppn,"PairOnly");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  NuDyn:Parent Task Name....................: " << ptn << endl;
    cout << "  NuDyn:Parent Path Name....................: " << ppn << endl;
    cout << "  NuDyn:Histo Base Name.....................: " << bn << endl;
    cout << "  NuDyn:nFilters............................: " << nFilters <<  endl;
    cout << "  NuDyn:multiplicityType....................: " << multiplicityType << endl;
    cout << "  NuDyn:pairOnly............................: " << pairOnly << endl;
    }

  TString suffix = "";
  switch ( multiplicityType )
    {
      case 0: suffix = "vsCent"; break;
      case 1: suffix = "vsMult"; break;
      case 2: suffix = "vsMult"; break;
    }
  h_eventStreams        = loadH1(inputFile,makeName(bn,"NeventStreams"));
  h_eventStreams_vsMult = loadH1(inputFile,makeName(bn,"NeventStreams")+suffix);
  
  for (unsigned int i1=0; i1<nFilters; i1++)
    {
    h_f1.push_back(loadProfile(inputFile,makeName(bn+"f1",i1)));
    h_f1_vsMult.push_back(loadProfile(inputFile,makeName(bn,"f1",i1,suffix)));
    for (unsigned int i2=i1; i2<nFilters; i2++)
      {
      h_f2.push_back(loadProfile(inputFile,makeName(bn+"f2",i1,i2)));
      h_f2_vsMult.push_back(loadProfile(inputFile,makeName(bn,"f2",i1,i2,suffix)));
      if (!pairOnly)
        {
        for (unsigned int i3=i2; i3<nFilters; i3++)
          {
          h_f3.push_back(loadProfile(inputFile,makeName(bn+"f3",i1,i2,i3)));
          h_f3_vsMult.push_back(loadProfile(inputFile,makeName(bn,"f3",i1,i2,i3,suffix)));
          for (unsigned int i4=i3; i4<nFilters; i4++)
            {
            h_f4.push_back(loadProfile(inputFile,makeName(bn,"f4",i1,i2,i3,i4)));
            h_f4_vsMult.push_back(loadProfile(inputFile,makeName(bn,"f4",i1,i2,i3,i4,suffix)));
            }
          }
        } // !pairOnly
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void NuDynHistos::fill(double mult, vector<double> & nAccepted,  double weight)
{
  h_eventStreams->Fill(mult, weight);
  h_eventStreams_vsMult->Fill(mult, weight);
  double fill;
  
  int k1 = 0;
  int k2 = 0;
  int k3 = 0;
  int k4 = 0;
  for (unsigned int i1=0; i1<nFilters; i1++)
    {
    fill = nAccepted[i1];
    h_f1[k1]->Fill(mult,fill,weight);
    h_f1_vsMult[k1]->Fill(mult,fill,weight);
    for (unsigned int i2=i1; i2<nFilters; i2++)
      {
      fill = nAccepted[i1]*(nAccepted[i2]-((i2==i1)?1:0));
      h_f2[k2]->Fill(mult,fill,weight);
      h_f2_vsMult[k2]->Fill(mult,fill,weight);
      if (!pairOnly)
        {
        for (unsigned int i3=i2; i3<nFilters; i3++)
          {
          fill = nAccepted[i1]
          * (nAccepted[i2]-((i2==i1)?1:0))
          * (nAccepted[i3]-((i3==i1)?1:0)-((i3==i2)?1:0));
          h_f3[k3]->Fill(mult,fill,weight);
          h_f3_vsMult[k3]->Fill(mult,fill,weight);
          k3++;
          for (unsigned int i4=i3; i4<nFilters; i4++)
            {
            fill = nAccepted[i1]
            * (nAccepted[i2]-((i2==i1)?1:0))
            * (nAccepted[i3]-((i3==i1)?1:0)-((i3==i2)?1:0))
            * (nAccepted[i4]-((i4==i1)?1:0)-((i4==i2)?1:0)-((i4==i3)?1:0)) ;
            h_f4[k4]->Fill(mult,fill,weight);
            h_f4_vsMult[k4]->Fill(mult,fill,weight);
            k4++;
            }
          }
        }
      k2++;
      }
    k1++;
    }
}

