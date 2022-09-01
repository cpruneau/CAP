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
#include "NuDynPlotter.hpp"
ClassImp(NuDynPlotter);

NuDynPlotter::NuDynPlotter()
:
Plotter("NuDynPlotter",Info)
{  }

NuDynPlotter::~NuDynPlotter()
{
}

void NuDynPlotter::makePlots(const TString & canvasNameBase,
                             NuDynHistos * nuDynHistos,
                             NuDynDerivedHistos * nuDynDerivedHistos,
                             CanvasConfiguration & canvasConfiguration,
                             GraphConfiguration  ** graphConfigurations)
{
  TH1 ** histograms = new TH1* [100];
  TString ** histTitles = new TString * [100];

  TString histName;
  TString histTitle;

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  //canvasConfiguration->setAxes(CanvasConfiguration::LogY);

  int nPart = 4;
  int nGraphs = 0;

  histograms[0] = nuDynHistos->h_eventStreams_vsMult;     histTitles[0] = new TString("N_{eventStreams}");
  plot(1, canvasNameBase+"eventStreams_vsMult",canvasConfiguration,graphConfigurations,
       "n", 0.0, 0.0,
       "Counts", 0.0, -1.2,
       histograms,histTitles,0.7, 0.5, 0.9, 0.9, 0.055);

  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
  {
  histTitle = "f_{1}";
  histTitle += "^{";
  histTitle += i1;
  histTitle += "}";
  cout << "Inserting: " << histTitle << endl;
  histograms[i1] = nuDynHistos->h_f1_vsMult[i1]; histTitles[i1] = new TString(histTitle);
  nGraphs++;
  }
  plot(nGraphs, canvasNameBase+"f1_vsMult",canvasConfiguration,graphConfigurations,
       "Mult", 20.0, 400.0,
       "f_{1}", 0.0, -1.2,
       histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      int index12 = nuDynHistos->index2(i1,i2);
      histTitle = "f_{2}";
      histTitle += "^{";
      histTitle += i1;
      histTitle += i2;
      histTitle += "}";
      cout << "Inserting: " << histTitle << endl;
      histograms[index12] = nuDynHistos->h_f2_vsMult[index12]; histTitles[index12] = new TString(histTitle);
      nGraphs++;
      }
    }
//  histograms[0] = nuDynHistos->h_f2_vsMult[2]; histTitles[0] = new TString("f_{2}^{00}");
//    plot(1, canvasNameBase+"f2_vsMult",canvasConfiguration,graphConfigurations,
       plot(nGraphs, canvasNameBase+"f2_vsMult",canvasConfiguration,graphConfigurations,
       "Mult", 20.0, 400.0,
       "f_{2}", 0.0, -1.2,
       histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      int index12 = nuDynHistos->index2(i1,i2);
      histTitle = "F_{2}";
      histTitle += "^{";
      histTitle += i1;
      histTitle += i2;
      histTitle += "}";
      cout << "Inserting: " << histTitle << endl;
      histograms[index12] = nuDynDerivedHistos->h_F2_vsMult[index12]; histTitles[index12] = new TString(histTitle);
      if (!histograms[index12])
        {
        cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
        return;
        }
      nGraphs++;
      }
    }
//  plot(1, canvasNameBase+"F2_vsMult",canvasConfiguration,graphConfigurations,
       plot(nGraphs, canvasNameBase+"F2_vsMult",canvasConfiguration,graphConfigurations,
       "Mult", 20.0, 400.0,
       "F_{2}", 1.2, -1.2,
       histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
   for (int i1=0; i1<nPart; i1++)
     {
     for (int i2=i1; i2<nPart; i2++)
       {
       int index12 = nuDynHistos->index2(i1,i2);
       histTitle = "R_{2}";
       histTitle += "^{";
       histTitle += i1;
       histTitle += i2;
       histTitle += "}";
       cout << "Inserting: " << histTitle << endl;
       histograms[index12] = nuDynDerivedHistos->h_R2_vsMult[index12]; histTitles[index12] = new TString(histTitle);
       if (!histograms[index12])
         {
         cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
         return;
         }
       nGraphs++;
       }
     }
   plot(nGraphs, canvasNameBase+"R2_vsMult",canvasConfiguration,graphConfigurations,
   //     plot(1, canvasNameBase+"R2_vsMult",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
             "R_{2}", -0.15, 0.15,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);



//  nGraphs = 0;
//  for (int i1=0; i1<nPart; i1++)
//    {
//    for (int i2=i1; i2<nPart; i2++)
//      {
//      for (int i3=i2; i3<nPart; i3++)
//        {
//        int index123 = nuDynHistos->index3(i1,i2,i3);
//        histTitle = "f_{3}";
//        histTitle += "^{";
//        histTitle += i1;
//        histTitle += i2;
//        histTitle += i3;
//        histTitle += "}";
//        cout << "Inserting: " << histTitle << endl;
//        histograms[index123] = nuDynHistos->h_f3_vsMult[index123]; histTitles[index123] = new TString(histTitle);
//        if (!histograms[index123])
//          {
//          cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
//          return;
//          }
//        nGraphs++;
//        }
//      }
//    }
  histograms[0] = nuDynHistos->h_f3_vsMult[1]; histTitles[0] = new TString("f_{3}^{000}");

  plot(1, canvasNameBase+"f3_vsMult",canvasConfiguration,graphConfigurations,
       "Mult", 20.0, 400.0,
       "f_{3}", 0.0, -1.2,
       histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
   for (int i1=0; i1<nPart; i1++)
     {
     for (int i2=i1; i2<nPart; i2++)
       {
       for (int i3=i2; i3<nPart; i3++)
         {
         int index123 = nuDynHistos->index3(i1,i2,i3);
         histTitle = "F_{3}";
         histTitle += "^{";
         histTitle += i1;
         histTitle += i2;
         histTitle += i3;
         histTitle += "}";
         //cout << "Inserting: " << histTitle << endl;
         histograms[index123] = nuDynDerivedHistos->h_F3_vsMult[index123]; histTitles[index123] = new TString(histTitle);
         if (!histograms[index123])
           {
           cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
           return;
           }
         nGraphs++;
         }
       }
     }
   //plot(nGraphs, canvasNameBase+"F3_vsMult",canvasConfiguration,graphConfigurations,
        plot(1, canvasNameBase+"F3_vsMult",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "F_{3}", 1.2, -1.2,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
    for (int i1=0; i1<nPart; i1++)
      {
      for (int i2=i1; i2<nPart; i2++)
        {
        for (int i3=i2; i3<nPart; i3++)
          {
          int index123 = nuDynHistos->index3(i1,i2,i3);
          histTitle = "R_{3}";
          histTitle += "^{";
          histTitle += i1;
          histTitle += i2;
          histTitle += i3;
          histTitle += "}";
          cout << "Inserting: " << histTitle << endl;
          histograms[index123] = nuDynDerivedHistos->h_R3_vsMult[index123]; histTitles[index123] = new TString(histTitle);
          if (!histograms[index123])
            {
            cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
            return;
            }
          nGraphs++;
          }
        }
      }
    plot(nGraphs, canvasNameBase+"R3_vsMult",canvasConfiguration,graphConfigurations,
    //     plot(1, canvasNameBase+"R3_vsMult",canvasConfiguration,graphConfigurations,
         "Mult", 20.0, 400.0,
              "R_{3}", -0.03, 0.03,
         histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);


  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      for (int i3=i2; i3<nPart; i3++)
        {
        for (int i4=i3; i4<nPart; i4++)
          {
          int index1234 = nuDynHistos->index4(i1,i2,i3,i4);
          histTitle = "f_{4}";
          histTitle += "^{";
          histTitle += i1;
          histTitle += i2;
          histTitle += i3;
          histTitle += i4;
          histTitle += "}";
          cout << "Inserting: " << histTitle << endl;
          histograms[index1234] = nuDynHistos->h_f4_vsMult[index1234]; histTitles[index1234] = new TString(histTitle);
          if (!histograms[index1234])
          {
          cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
          return;
          }
          nGraphs++;
          }
        }
      }
    }
   //plot(nGraphs, canvasNameBase+"f4_vsMult",canvasConfiguration,graphConfigurations,
        plot(1, canvasNameBase+"f4_vsMult",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "f_{4}", 0.0, -1.2,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);


  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      for (int i3=i2; i3<nPart; i3++)
        {
        for (int i4=i3; i4<nPart; i4++)
          {
          int index1234 = nuDynHistos->index4(i1,i2,i3,i4);
          histTitle = "F_{4}";
          histTitle += "^{";
          histTitle += i1;
          histTitle += i2;
          histTitle += i3;
          histTitle += i4;
          histTitle += "}";
          cout << "Inserting: " << histTitle << endl;
          histograms[index1234] = nuDynDerivedHistos->h_F4_vsMult[index1234]; histTitles[index1234] = new TString(histTitle);
          if (!histograms[index1234])
          {
          cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
          return;
          }
          nGraphs++;
          }
        }
      }
    }
   //plot(nGraphs, canvasNameBase+"F4_vsMult",canvasConfiguration,graphConfigurations,
        plot(1, canvasNameBase+"F4_vsMult",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "F_{4}", 1.2, -1.2,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);


  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      for (int i3=i2; i3<nPart; i3++)
        {
        for (int i4=i3; i4<nPart; i4++)
          {
          int index1234 = nuDynHistos->index4(i1,i2,i3,i4);
          histTitle = "R_{4}";
          histTitle += "^{";
          histTitle += i1;
          histTitle += i2;
          histTitle += i3;
          histTitle += i4;
          histTitle += "}";
          cout << "Inserting: " << histTitle << endl;
          histograms[index1234] = nuDynDerivedHistos->h_R4_vsMult[index1234]; histTitles[index1234] = new TString(histTitle);
          if (!histograms[index1234])
          {
          cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
          return;
          }
          nGraphs++;
          }
        }
      }
    }
   plot(nGraphs, canvasNameBase+"R4_vsMult",canvasConfiguration,graphConfigurations,
//          plot(1, canvasNameBase+"R4_vsMult",canvasConfiguration,graphConfigurations,
          "Mult", 20.0, 400.0,
               "R_{4}", -0.02, 0.02,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);



}

void NuDynPlotter::makeComparisonPlots(const TString & canvasNameBase,
                                       CanvasConfiguration & canvasConfiguration,
                                       int nGraphs,
                                       NuDynHistos ** nuDynHistos,
                                       NuDynDerivedHistos ** nuDynDerivedHistos,
                                       TString ** histLabels,
                                       GraphConfiguration  ** graphConfigurations)
{
  TH1 ** histograms = new TH1* [100];
  TString ** histTitles = new TString * [100];

  TString histName;
  TString histTitleBase;
  TString histTitle;

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  //canvasConfiguration->setAxes(CanvasConfiguration::LogY);
int index1234 = nuDynHistos[0]->index4(0,1,2,3);
  histTitleBase = "f_{4}";
  for (int i1=0; i1<nGraphs; i1++)
    {
    histTitle = histTitleBase;
    histTitle += *histLabels[i1];
    //cout << "Inserting: " << histTitle  << endl;
    histograms[i1] = nuDynHistos[i1]->h_f4_vsMult[index1234];
    if (!histograms[i1])
        {
        cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
        return;
        }
    histTitles[i1] = new TString(histTitle);

    }

   plot(nGraphs, canvasNameBase+"f4_vsMult",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "f_{4}", 0.0, -1.2,
        histograms,histTitles,0.25, 0.6, 0.45, 0.85, 0.055);

  histTitleBase = "F_{4}";
   for (int i1=0; i1<nGraphs; i1++)
     {
     histTitle = histTitleBase;
     histTitle += *histLabels[i1];
     cout << "Inserting: " << histTitle  << endl;
     histograms[i1] = nuDynDerivedHistos[i1]->h_F4_vsMult[index1234]; histTitles[i1] = new TString(histTitle);
     if (!histograms[i1])
       {
       cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
       return;
       }
     }
    plot(nGraphs, canvasNameBase+"F4_vsMult",canvasConfiguration,graphConfigurations,
         "Mult", 20.0, 400.0,
         "F_{4}", -50000, 50000,
         histograms,histTitles,0.25, 0.6, 0.45, 0.85, 0.055);

  histTitleBase = "R_{4}";
  for (int i1=0; i1<nGraphs; i1++)
    {
    histTitle = histTitleBase;
    histTitle += *histLabels[i1];
    cout << "Inserting: " << histTitle  << endl;
    histograms[i1] = nuDynDerivedHistos[i1]->h_R4_vsMult[index1234]; histTitles[i1] = new TString(histTitle);
    if (!histograms[i1])
      {
      cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
      return;
      }
    }
   plot(nGraphs, canvasNameBase+"R4_vsMult",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "R_{4}", -0.3, 0.3,
        histograms,histTitles,0.65, 0.6, 0.85, 0.9, 0.055);




}

void NuDynPlotter::makeNudynComparisonPlots(const TString & canvasNameBase,
                                            CanvasConfiguration & canvasConfiguration,
                                            int nGraphs,
                                            NuDynHistos ** nuDynHistos,
                                            NuDynDerivedHistos ** nuDynDerivedHistos,
                                            TString ** histLabels,
                                            GraphConfiguration  ** graphConfigurations)
{
  TH1 ** histograms = new TH1* [100];
  TString ** histTitles = new TString * [100];

  TString histName;
  TString histTitleBase;
  TString histTitle;

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  //canvasConfiguration->setAxes(CanvasConfiguration::LogY);
int index12= nuDynHistos[0]->index2(0,1);
  histTitleBase = "#nu_{Dyn} ";
  for (int i1=0; i1<nGraphs; i1++)
    {
    histTitle = histTitleBase;
    histTitle += *histLabels[i1];
    //cout << "Inserting: " << histTitle  << endl; // HMHMHMHM_nudyn_23_vsMult
    histograms[i1] = nuDynDerivedHistos[i1]->h_nudyn_vsMult[index12];
    if (!histograms[i1])
        {
        cout << "<F> Histograms not loaded!!!! ABORT!!!" << endl;
        return;
        }
    histTitles[i1] = new TString(histTitle);

    }

   plot(nGraphs, canvasNameBase+"nudyn_vsMult",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "#nu_{Dyn}", 0.0, -1.2,
        histograms,histTitles,0.25, 0.6, 0.45, 0.85, 0.055);

}
