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
using CAP::NuDynHistos;

ClassImp(NuDynHistos);


NuDynHistos::NuDynHistos(Task * _parent,
                         const String & _name,
                         Configuration & _configuration)
:
HistogramGroup(_parent,_name,_configuration),
h_eventStreams(0),
nFilters(0),
multiplicityType(0),
pairOnly(0),
deltaRapidtyBin(),
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
  if (reportStart(__FUNCTION__))
    ;
  const String & bn  = getName();
  const String & ptn = getParentTaskName();
  const String & ppn = getParentPathName();
  Configuration & configuration = getConfiguration();
  multiplicityType = configuration.getValueInt(ppn,"multiplicityType");
  int nBins_mult   = configuration.getValueInt(ppn,"nBins_mult");
  double min_mult  = configuration.getValueInt(ppn,"Min_mult");
  double max_mult  = configuration.getValueInt(ppn,"Max_mult");
  int    nBins_rapidity = configuration.getValueInt(ppn,"nBins_rapidity");
  double min_rapidity   = configuration.getValueDouble(ppn,"Min_rapidity");
  double max_rapidity   = configuration.getValueDouble(ppn,"Max_rapidity");
  double width_rapidity = (max_rapidity-min_rapidity)/double(nBins_rapidity);

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  NuDyn:Parent Task Name....................: " << ptn << endl;
    cout << "  NuDyn:Parent Path Name....................: " << ppn << endl;
    cout << "  NuDyn:Histo Base Name.....................: " << bn << endl;
    cout << "  NuDyn:multiplicityType....................: " << multiplicityType << endl;
    cout << "  NuDyn:pairOnly............................: " << pairOnly << endl;
    cout << "  NuDyn:nBins_mult..........................: " << nBins_mult << endl;
    cout << "  NuDyn:min_mult............................: " << min_mult << endl;
    cout << "  NuDyn:max_mult............................: " << max_mult << endl;
    cout << "  NuDyn:nBins_rapidity......................: " << nBins_rapidity << endl;
    cout << "  NuDyn:min_rapidity........................: " << min_rapidity << endl;
    cout << "  NuDyn:max_rapidity........................: " << max_rapidity << endl;
    cout << "  NuDyn:width_rapidity......................: " << width_rapidity << endl;
    }

  for (int iEta=0; iEta<nBins_rapidity; iEta++)
    {
    deltaRapidtyBin.push_back(min_rapidity+double(iEta)*width_rapidity );
    }

  String suffix = "";
  String xTitle = "";
  String yTitle = "Rapidity";

  switch (multiplicityType)
    {
      case 0: suffix = "vsCent"; xTitle = "%"; break;
      case 1: suffix = "vsMult"; xTitle = "mult_{Tot}";  break;
      case 2: suffix = "vsMult"; xTitle = "mult_{acc}";  break;
    }
  
  h_eventStreams        = createHistogram(createName(bn,"NeventStreams"),10,0.0,10, "Streams","n_{Events}");
  h_eventStreams_vsMult = createHistogram(createName(bn,"NeventStreams",suffix),nBins_mult,min_mult,  max_mult, "mult",  "n_{Events}");

//  h_f1.push_back(        createProfile(createName(bn,"f1_0"), 10,0.0,10, "Streams", "f_{1}^{0}")  );
//  h_f1.push_back(        createProfile(createName(bn,"f1_1"), 10,0.0,10, "Streams", "f_{1}^{1}")  );
  h_f1_vsMult.push_back( createProfile(createName(bn,"f1_0",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{1}^{0}")  );
  h_f1_vsMult.push_back( createProfile(createName(bn,"f1_1",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{1}^{1}")  );

//  h_f2.push_back(        createProfile(createName(bn,"f2_00"), 10,0.0,10, "Streams", "f_{2}^{00}") );
//  h_f2.push_back(        createProfile(createName(bn,"f2_01"), 10,0.0,10, "Streams", "f_{2}^{01}") );
//  h_f2.push_back(        createProfile(createName(bn,"f2_11"), 10,0.0,10, "Streams", "f_{2}^{11}") );
  h_f2_vsMult.push_back( createProfile(createName(bn,"f2_00",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{2}^{00}")  );
  h_f2_vsMult.push_back( createProfile(createName(bn,"f2_01",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{2}^{01}")  );
  h_f2_vsMult.push_back( createProfile(createName(bn,"f2_11",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{2}^{11}")  );

//  h_f3.push_back(        createProfile(createName(bn,"f3_000"), 10,0.0,10, "Streams", "f_{3}^{000}") );
//  h_f3.push_back(        createProfile(createName(bn,"f3_001"), 10,0.0,10, "Streams", "f_{3}^{001}") );
//  h_f3.push_back(        createProfile(createName(bn,"f3_011"), 10,0.0,10, "Streams", "f_{3}^{011}") );
//  h_f3.push_back(        createProfile(createName(bn,"f3_111"), 10,0.0,10, "Streams", "f_{3}^{111}") );
  h_f3_vsMult.push_back( createProfile(createName(bn,"f3_000",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{3}^{000}")  );
  h_f3_vsMult.push_back( createProfile(createName(bn,"f3_001",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{3}^{001}")  );
  h_f3_vsMult.push_back( createProfile(createName(bn,"f3_011",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{3}^{011}")  );
  h_f3_vsMult.push_back( createProfile(createName(bn,"f3_111",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{3}^{111}")  );

//  h_f4.push_back(        createProfile(createName(bn,"f4_0000"), 10,0.0,10, "Streams", "f_{4}^{0000}") );
//  h_f4.push_back(        createProfile(createName(bn,"f4_0001"), 10,0.0,10, "Streams", "f_{4}^{0001}") );
//  h_f4.push_back(        createProfile(createName(bn,"f4_0011"), 10,0.0,10, "Streams", "f_{4}^{0011}") );
//  h_f4.push_back(        createProfile(createName(bn,"f4_0111"), 10,0.0,10, "Streams", "f_{4}^{0111}") );
//  h_f4.push_back(        createProfile(createName(bn,"f4_1111"), 10,0.0,10, "Streams", "f_{4}^{1111}") );
  h_f4_vsMult.push_back( createProfile(createName(bn,"f4_0000",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{4}^{0000}") );
  h_f4_vsMult.push_back( createProfile(createName(bn,"f4_0001",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{4}^{0001}") );
  h_f4_vsMult.push_back( createProfile(createName(bn,"f4_0011",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{4}^{0011}") );
  h_f4_vsMult.push_back( createProfile(createName(bn,"f4_0111",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{4}^{0111}") );
  h_f4_vsMult.push_back( createProfile(createName(bn,"f4_1111",suffix),nBins_mult,min_mult,max_mult,nBins_rapidity,min_rapidity,max_rapidity,xTitle, yTitle,"f_{4}^{1111}") );

  if (reportEnd(__FUNCTION__))
    ;
}

//________________________________________________________________________
void NuDynHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;

  const String & bn  = getName();
  const String & ptn = getParentTaskName();
  const String & ppn = getParentPathName();
  Configuration & configuration = getConfiguration();
  multiplicityType = configuration.getValueInt(ppn,"multiplicityType");
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

  String suffix = "";
  String xTitle = "";

  switch (multiplicityType)
    {
      case 0: suffix = "vsCent"; xTitle = "%"; break;
      case 1: suffix = "vsMult"; xTitle = "mult_{Tot}";  break;
      case 2: suffix = "vsMult"; xTitle = "mult_{acc}";  break;
    }

  h_eventStreams        = loadProfile2D(inputFile, createName(bn,"NeventStreams"));
  h_eventStreams_vsMult = loadProfile2D(inputFile, createName(bn,"NeventStreams"));

//  h_f1.push_back(        loadProfile2D(inputFile,createName(bn,"f1_0"));
//  h_f1.push_back(        loadProfile2D(inputFile,createName(bn,"f1_1"));
  h_f1_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f1_0",suffix)));
  h_f1_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f1_1",suffix)));

//  h_f2.push_back(        loadProfile2D(inputFile,createName(bn,"f2_00")));
//  h_f2.push_back(        loadProfile2D(inputFile,createName(bn,"f2_01")));
//  h_f2.push_back(        loadProfile2D(inputFile,createName(bn,"f2_11")));
  h_f2_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f2_00",suffix)));
  h_f2_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f2_01",suffix)));
  h_f2_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f2_11",suffix)));

//  h_f3.push_back(        loadProfile2D(inputFile,createName(bn,"f3_000")));
//  h_f3.push_back(        loadProfile2D(inputFile,createName(bn,"f3_001")));
//  h_f3.push_back(        loadProfile2D(inputFile,createName(bn,"f3_011")));
//  h_f3.push_back(        loadProfile2D(inputFile,createName(bn,"f3_111")));
  h_f3_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f3_000",suffix)));
  h_f3_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f3_001",suffix)));
  h_f3_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f3_011",suffix)));
  h_f3_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f3_111",suffix)));

//  h_f4.push_back(        loadProfile2D(inputFile,createName(bn,"f4_0000")));
//  h_f4.push_back(        loadProfile2D(inputFile,createName(bn,"f4_0001")));
//  h_f4.push_back(        loadProfile2D(inputFile,createName(bn,"f4_0011")));
//  h_f4.push_back(        loadProfile2D(inputFile,createName(bn,"f4_0111")));
//  h_f4.push_back(        loadProfile2D(inputFile,createName(bn,"f4_1111")));
  h_f4_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f4_0000",suffix)));
  h_f4_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f4_0001",suffix)));
  h_f4_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f4_0011",suffix)));
  h_f4_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f4_0111",suffix)));
  h_f4_vsMult.push_back( loadProfile2D(inputFile,createName(bn,"f4_1111",suffix)));

  if (reportEnd(__FUNCTION__))
    ;
}


void NuDynHistos::fill(double mult, vector<double> & nAccepted0, vector<double> & nAccepted1,  double weight)
{
  h_eventStreams->Fill(mult);
  h_eventStreams_vsMult->Fill(mult);
  double fill;

  double n1_0, n1_1;
  double n2_00, n2_01, n2_11;
  double n3_000, n3_001, n3_011, n3_111;
  double n4_0000, n4_0001, n4_0011, n4_0111, n4_1111;
  double deltaY;

  for (int iY=0; iY<nAccepted0.size(); iY++)
    {
    if (iY>0)
      {
      nAccepted0[iY] += nAccepted0[iY-1];
      nAccepted1[iY] += nAccepted1[iY-1];
      }
    n1_0    = nAccepted0[iY];
    n1_1    = nAccepted1[iY];
    n2_00   = n1_0 * (n1_0 - 1);
    n2_01   = n1_0 * n1_1;
    n2_11   = n1_1 * (n1_1 - 1);
    n3_000  = n1_0 * (n1_0 - 1) * (n1_0 - 2);
    n3_001  = n1_0 * (n1_0 - 1) * n1_1;
    n3_011  = n1_0 * n1_1 * (n1_1 - 1);
    n3_111  = n1_1 * (n1_1 - 1) * (n1_1 - 2);
    n4_0000  = n1_0 * (n1_0 - 1) * (n1_0 - 2) * (n1_0 - 3);
    n4_0001  = n1_0 * (n1_0 - 1) * (n1_0 - 2) * n1_1;
    n4_0011  = n1_0 * (n1_0 - 1) * n1_1 * (n1_1 - 1);
    n4_0111  = n1_0 * n1_1 * (n1_1 - 1) * (n1_1 - 2);
    n4_1111  = n1_1 * (n1_1 - 1) * (n1_1 - 2) * (n1_1 - 3);
    deltaY = deltaRapidtyBin[iY];
    h_f1[0]->Fill(mult,deltaY,n1_0);
    h_f1[1]->Fill(mult,deltaY,n1_1);

    h_f2[0]->Fill(mult,deltaY,n2_00);
    h_f2[1]->Fill(mult,deltaY,n2_01);
    h_f2[2]->Fill(mult,deltaY,n2_11);

    h_f3[0]->Fill(mult,deltaY,n3_000);
    h_f3[1]->Fill(mult,deltaY,n3_001);
    h_f3[2]->Fill(mult,deltaY,n3_011);
    h_f3[3]->Fill(mult,deltaY,n3_111);

    h_f4[0]->Fill(mult,deltaY,n4_0000);
    h_f4[1]->Fill(mult,deltaY,n4_0001);
    h_f4[2]->Fill(mult,deltaY,n4_0011);
    h_f4[3]->Fill(mult,deltaY,n4_0111);
    h_f4[4]->Fill(mult,deltaY,n4_1111);
    }
}

