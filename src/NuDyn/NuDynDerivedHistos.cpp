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
#include "NuDynDerivedHistos.hpp"
using CAP::NuDynDerivedHistos;

ClassImp(NuDynDerivedHistos);

NuDynDerivedHistos::NuDynDerivedHistos(Task * _parent,
                                       const String & _name,
                                       Configuration & _configuration)
:
HistogramGroup(_parent,_name,_configuration),
h_F2(),
h_F3(),
h_F4(),
h_R2(),
h_R3(),
h_R4(),
h_nudyn(),
h_F2_vsMult(),
h_F3_vsMult(),
h_F4_vsMult(),
h_R2_vsMult(),
h_R3_vsMult(),
h_R4_vsMult(),
h_nudyn_vsMult()
{

}

// for now use the same boundaries for eta and y histogram
void NuDynDerivedHistos::createHistograms()
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

  int    nBins_rapidity = configuration.getValueInt(ppn,"nBins_rapidity");
  double min_rapidity   = configuration.getValueDouble(ppn,"Min_rapidity");
  double max_rapidity   = configuration.getValueDouble(ppn,"Max_rapidity");
  double width_rapidity = (max_rapidity-min_rapidity)/double(nBins_rapidity);


  TString suffix = "";
  TString xTitle = "";
  
  switch (multiplicityType)
    {
      case 0: suffix = "_vsCent"; xTitle = "%"; break;
      case 1: suffix = "_vsMult"; xTitle = "mult_{Tot}";  break;
      case 2: suffix = "_vsMult"; xTitle = "mult_{acc}";  break;
    }

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  NuDyn:Parent Task Name....................: " << ptn << endl;
    cout << "  NuDyn:Parent Path Name....................: " << ppn << endl;
    cout << "  NuDyn:Histo Base Name.....................: " << bn << endl;
    cout << "  NuDyn:nFilters............................: " << nFilters <<  endl;
    cout << "  NuDyn:multiplicityType....................: " << multiplicityType << endl;
    cout << "  NuDyn:pairOnly............................: " << pairOnly << endl;
    cout << "  NuDyn:nBins_rapidity......................: " << nBins_rapidity << endl;
    cout << "  NuDyn:min_rapidity........................: " << min_rapidity << endl;
    cout << "  NuDyn:max_rapidity........................: " << max_rapidity << endl;
    cout << "  NuDyn:width_rapidity......................: " << width_rapidity << endl;
    }

  String name;
  String title;

  for (int i1=0; i1<nFilters; i1++)
    {
    for (int i2=i1; i2<nFilters; i2++)
      {
      name = bn; name += "_"; name += "F2"; name += "_"; name += i1; name += i2;
      title = "F_{2}^{"; title += i1; title += i2; title += "}";
      h_F2.push_back(createHistogram(name, 1,min_mult,max_mult,xTitle, title));
      name = bn; name += "_"; name += "R2"; name += "_"; name += i1; name += i2;
      title = "R_{2}^{"; title += i1; title += i2; title += "}";
      h_R2.push_back(createHistogram(name, 1,min_mult,max_mult,xTitle,title));
      name = bn; name += "_"; name += "nudyn"; name += "_"; name += i1; name += i2;
      title = "#nu_{#rm dyn}^{"; title += i1; title += i2; title += "}";
      h_nudyn.push_back(createHistogram(name, 1,min_mult,max_mult,xTitle, title));
      name = bn; name += "_"; name += "F2"; name += "_"; name += i1; name += i2;
      title = "F_{2}^{"; title += i1; title += i2; title += "}";
      h_F2_vsMult.push_back(createHistogram(name,nBins_mult,min_mult,  max_mult, xTitle,title));
      name = bn; name += "_"; name += "R2"; name += "_"; name += i1; name += i2;
      title = "R_{2}^{"; title += i1; title += i2; title += "}";
      h_R2_vsMult.push_back(createHistogram(name,nBins_mult,min_mult,  max_mult, xTitle,title));
      name = bn; name += "_"; name += "nudyn"; name += "_"; name += i1; name += i2;
      title = "#nu_{#rm dyn}^{"; title += i1; title += i2; title += "}";
      h_nudyn_vsMult.push_back(createHistogram(name,nBins_mult,min_mult,max_mult,xTitle,title));
       
      if (pairOnly) continue;
      for (int i3=i2; i3<nFilters; i3++)
        {
        name = bn; name += "_"; name += "F3"; name += "_"; name += i1; name += i2; name += i3;
        title = "F_{3}^{"; title += i1; title += i2; title += i3; title += "}";
        h_F3.push_back(createHistogram(name, 1,min_mult,max_mult,xTitle,title));
        name = bn; name += "_"; name += "R3"; name += "_"; name += i1; name += i2; name += i3;
        title = "R_{3}^{"; title += i1; title += i2; title += i3;  title += "}";
        h_R3.push_back(createHistogram(name, 1,min_mult,max_mult,xTitle,title));
        name = bn; name += "_"; name += "F3"; name += "_"; name += i1; name += i2; name += i3;
        title = "F_{3}^{"; title += i1; title += i2; title += i3;  title += "}";
        h_F3_vsMult.push_back( createHistogram(name,nBins_mult,min_mult,  max_mult,xTitle,title));
        name = bn; name += "_"; name += "R3"; name += "_"; name += i1; name += i2; name += i3;
        title = "R_{3}^{"; title += i1; title += i2; title += i3;  title += "}";
        h_R3_vsMult.push_back( createHistogram(name,nBins_mult,min_mult,  max_mult,xTitle,title));
        
        for (int i4=i3; i4<nFilters; i4++)
          {
          name = bn; name += "_"; name += "F4"; name += "_"; name += i1; name += i2; name += i3;name += i4;
          title = "F_{4}^{"; title += i1; title += i2; title += i3; title += i4; title += "}";
          h_F4.push_back( createHistogram(name, 1,min_mult,max_mult,xTitle,title));
          name = bn; name += "_"; name += "R4"; name += "_"; name += i1; name += i2; name += i3;name += i4;
          title = "R_{4}^{"; title += i1; title += i2; title += i3; title += i4; title += "}";
          h_R4.push_back( createHistogram(name, 1,min_mult,max_mult,xTitle,title));
          name = bn; name += "_"; name += "F4"; name += "_"; name += i1; name += i2; name += i3;name += i4;
          title = "F_{4}^{"; title += i1; title += i2; title += i3; title += i4; title += "}";
          h_F4_vsMult.push_back( createHistogram(name,nBins_mult,min_mult,  max_mult,xTitle,title));
          name = bn; name += "_"; name += "R4"; name += "_"; name += i1; name += i2; name += i3;name += i4;
          title = "R_{4}^{"; title += i1; title += i2; title += i3; title += i4; title += "}";
          h_R4_vsMult.push_back( createHistogram(name,nBins_mult,min_mult,  max_mult,xTitle,title));
          }
        }
      }
    }
}

//________________________________________________________________________
void NuDynDerivedHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  const String & bn  = getName();
  const String & ptn = getParentTaskName();
  const String & ppn = getParentPathName();  nFilters         = configuration.getValueInt(ppn,"nFilters");
  multiplicityType = configuration.getValueInt(ppn,"multiplicityType");
  pairOnly         = configuration.getValueBool(ppn,"PairOnly");

  String suffix = "";
  String xTitle = "";
  switch ( multiplicityType )
    {
      case 0: suffix = "_vsCent"; xTitle = "%"; break;
      case 1: suffix = "_vsMult"; xTitle = "mult_{Tot}";  break;
      case 2: suffix = "_vsMult"; xTitle = "mult_{acc}";  break;
    }

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

  String name;
  String title;
  for (int i1=0; i1<nFilters; i1++)
    {
    for (int i2=i1; i2<nFilters; i2++)
      {
      name = bn; name += "_"; name += "F2"; name += "_"; name += i1; name += i2;
      h_F2.push_back(loadH1(inputFile,name));
      name = bn; name += "_"; name += "R2"; name += "_"; name += i1; name += i2;
      h_R2.push_back(loadH1(inputFile,name));
      name = bn; name += "_"; name += "nudyn"; name += "_"; name += i1; name += i2;
      h_nudyn.push_back(loadH1(inputFile,name));
      name = bn; name += "_"; name += "F2"; name += "_"; name += i1; name += i2;
      h_F2_vsMult.push_back(loadH1(inputFile,name));
      name = bn; name += "_"; name += "R2"; name += "_"; name += i1; name += i2;
      h_R2_vsMult.push_back(loadH1(inputFile,name));
      name = bn; name += "_"; name += "nudyn"; name += "_"; name += i1; name += i2;
      h_nudyn_vsMult.push_back(loadH1(inputFile,name));
      if (pairOnly) continue;
      for (int i3=i2; i3<nFilters; i3++)
        {
        name = bn; name += "_"; name += "F3"; name += "_"; name += i1; name += i2; name += i3;
        h_F3.push_back(loadH1(inputFile,name));
        name = bn; name += "_"; name += "R3"; name += "_"; name += i1; name += i2; name += i3;
        h_R3.push_back(loadH1(inputFile,name));
        name = bn; name += "_"; name += "F3"; name += "_"; name += i1; name += i2; name += i3;
        h_F3_vsMult.push_back(loadH1(inputFile,name));
        name = bn; name += "_"; name += "R3"; name += "_"; name += i1; name += i2; name += i3;
        h_R3_vsMult.push_back(loadH1(inputFile,name));
        for (int i4=i3; i4<nFilters; i4++)
          {
          name = bn; name += "_"; name += "F4"; name += "_"; name += i1; name += i2; name += i3;name += i4;
          h_F4.push_back(loadH1(inputFile,name));
          name = bn; name += "_"; name += "R4"; name += "_"; name += i1; name += i2; name += i3;name += i4;
          h_R4.push_back(loadH1(inputFile,name));
          name = bn; name += "_"; name += "F4"; name += "_"; name += i1; name += i2; name += i3;name += i4;
          h_F4_vsMult.push_back(loadH1(inputFile,name));
          name = bn; name += "_"; name += "R4"; name += "_"; name += i1; name += i2; name += i3;name += i4;
          h_R4_vsMult.push_back(loadH1(inputFile,name));
          }
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void NuDynDerivedHistos::calculateDerivedHistograms(NuDynHistos* baseHistos)
{
  // h_r2_11 = <n1(n1-1)>/<n1><n1>
  // h_r2_22 = <n2(n2-1)>/<n2><n2>
  // h_r2_12 = <n1(n2-1)>/<n1><n2>
  // h_nudyn_12 = h_r2_11 + h_r2_22 -2*h_r2_12
  
  NuDynHistos & source = *baseHistos;
  int k1 = 0;
  int k2 = 0;
  int k3 = 0;
  int k4 = 0;
  for (int i1=0; i1<nFilters; i1++)
    {
    for (int i2=i1; i2<nFilters; i2++)
      {
      int index12 = index2(i1,i2);
      calculateF2R2(source.h_f1[i1],source.h_f1[i2],source.h_f2[k2], h_F2[k2], h_R2[k2]);
      calculateF2R2(source.h_f1_vsMult[i1],source.h_f1_vsMult[i2],source.h_f2_vsMult[k2], h_F2_vsMult[k2], h_R2_vsMult[k2]);
     
      if (pairOnly) continue;
      
      for (int i3=i2; i3<nFilters; i3++)
        {
        int index13 = index2(i1,i3);
        int index23 = index2(i2,i3);
        int index123 = index3(i1,i2,i3);
        calculateF3R3(source.h_f1[i1],source.h_f1[i2],source.h_f1[i3],
                      source.h_f2[index12],source.h_f2[index13],source.h_f2[index23],
                      source.h_f3[k3],
                      h_F3[k3], h_R3[k3]);
        calculateF3R3(source.h_f1_vsMult[i1],source.h_f1_vsMult[i2],source.h_f1_vsMult[i3],
                      source.h_f2_vsMult[index12],source.h_f2_vsMult[index13],source.h_f2_vsMult[index23],
                      source.h_f3_vsMult[k3],
                      h_F3_vsMult[k3], h_R3_vsMult[k3]);
        for (int i4=i3; i4<nFilters; i4++)
          {
          int index14 = index2(i1,i4);
          int index24 = index2(i2,i4);
          int index34 = index2(i3,i4);
          int index124 = index3(i1,i2,i4);
          int index134 = index3(i1,i3,i4);
          int index234 = index3(i2,i3,i4);
          calculateF4R4(source.h_f1[i1],source.h_f1[i2],source.h_f1[i3],source.h_f1[i4],
                        source.h_f2[index12],source.h_f2[index13],source.h_f2[index14],
                        source.h_f2[index23],source.h_f2[index24],source.h_f2[index34],
                        source.h_f3[index123],source.h_f3[index124],source.h_f3[index134],source.h_f3[index234],
                        source.h_f4[k4],
                        h_F4[k4], h_R4[k4]);
          calculateF4R4(source.h_f1_vsMult[i1],source.h_f1_vsMult[i2],source.h_f1_vsMult[i3],source.h_f1_vsMult[i4],
                        source.h_f2_vsMult[index12], source.h_f2_vsMult[index13], source.h_f2_vsMult[index14],
                        source.h_f2_vsMult[index23], source.h_f2_vsMult[index24], source.h_f2_vsMult[index34],
                        source.h_f3_vsMult[index123],source.h_f3_vsMult[index124],source.h_f3_vsMult[index134],source.h_f3_vsMult[index234],
                        source.h_f4_vsMult[k4],
                        h_F4_vsMult[k4], h_R4_vsMult[k4]);
          k4++;
          }
        k3++;
        }
      k2++;
      }
    k1++;
    }
}
