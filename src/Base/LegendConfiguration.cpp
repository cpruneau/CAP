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

#include "LegendConfiguration.hpp"

ClassImp(LegendConfiguration);

LegendConfiguration::LegendConfiguration()
:
legends(),
labels()
{
  setDefaultConfiguration();
}

LegendConfiguration::LegendConfiguration(double _xLeft,
                                         double _xRight,
                                         double _yLow,
                                         double _yHigh,
                                         double _textSize)
:
legends(),
labels()
{
  setDefaultConfiguration();
  addParameter("textSize",_textSize);
  addParameter("xLeft",   _xLeft);
  addParameter("xRight",  _xRight);
  addParameter("yLow",    _yLow);
  addParameter("yHigh",   _yHigh);
}

LegendConfiguration::LegendConfiguration(double _xLeft,
                                         double _xRight,
                                         double _yLow,
                                         double _yHigh,
                                         double _textSize,
                                         vector<TString> & _entries)
:
legends(),
labels()
{
  setDefaultConfiguration();
  addParameter("textSize",_textSize);
  addParameter("xLeft",   _xLeft);
  addParameter("xRight",  _xRight);
  addParameter("yLow",    _yLow);
  addParameter("yHigh",   _yHigh);
  int n = _entries.size();
  if (n>0) addParameter("useTitles",false);
  for (int k=0; k<n; k++)
    {
    legends.push_back(_entries[k]);
    }
}


void LegendConfiguration::setDefaultConfiguration()
{
  addParameter("useLegend",    false);
  addParameter("useLabels",    false);
  addParameter("drawLegend",   true);
  //addParameter("drawBorder",   false);
  addParameter("useTitles",    true);
  addParameter("nColumns",     1);
  addParameter("borderColor",  0);
  addParameter("borderSize",   0);
  addParameter("fillColor",    5);
  addParameter("fillStyle",    0);
  addParameter("textIndex",    22);
  addParameter("textColor",    1);
  addParameter("textSize",     0.05);
  addParameter("textAlign",    12);
  addParameter("xLeft",        0.1);
  addParameter("xRight",       0.2);
  addParameter("yLow",         0.1);
  addParameter("yHigh",        0.2);
  addParameter("header",       TString(""));
}

LegendConfiguration::LegendConfiguration(const LegendConfiguration & source)
:
Configuration(source),
legends(source.legends),
labels(source.labels)
{

}


LegendConfiguration & LegendConfiguration::operator=(const LegendConfiguration & source)
{
  if (this!=&source)
    {
    Configuration::operator=(source);
    legends = source.legends;
    labels  = source.labels;
    }
  return *this;
}

void LegendConfiguration::addLegend(const TString & legend)
{
  legends.push_back(legend);
}

TString LegendConfiguration::getLegendAt(int index)  const
{
  if (index<0 || index>=int(legends.size()))
    return TString("NotDefined");
  else
    return legends[index];
}

void LegendConfiguration::setLegendAt(int index,const TString & legend)
{
  if (index<0)
    return;
  else if (index < int(legends.size()))
    legends[index] = legend;
  else
    {
    for (int k=0; k<(index-int(legends.size())); k++) legends.push_back(TString(""));
    legends.push_back(legend);
    }
}


void LegendConfiguration::addLabel(const TString & text, double x, double y, double angle, int color,  double fontSize)
{
  TLatex * label = new TLatex(x,y,text);
  label->SetTextColor(color);
  label->SetTextAngle(angle);
  label->SetTextSize(fontSize);
  labels.push_back(label);
}

TLatex * LegendConfiguration::getLabelAt(int index) const
{
  if (index<0 || index>=int(labels.size()))
    {
    return nullptr;
    }
  else
    return labels[index];
}

void LegendConfiguration::drawLabels()
{
  for (int k=0; k<int(labels.size()); k++) labels[k]->Draw();
}
