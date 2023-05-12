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
#include "XmlVectorField.hpp"
using CAP::XmlDocument;
using CAP::XmlVectorField;
using CAP::String;

//ClassImp(CAP::XmlDocument);

XmlVectorField::XmlVectorField()
:
XmlDocument()
{   }

XmlVectorField::XmlVectorField(const String  & _inputPath,
                               const String  & _inputFileName)
:
XmlDocument(_inputPath,_inputFileName)
{   }

XmlVectorField::XmlVectorField(const String  & _inputFile)
:
XmlDocument(_inputFile)
{   }


VectorField* XmlVectorField::getXmlVectorField()
{
  VectorField* vectorField;
  TString   vName;
  TString   vAxis[3];
  double    vMin[3];
  double    vMax[3];
  int       vPts[3];

  if (!currentTag || !(currentTag->name == "VECTOR3D") )
    {
      cout << endl;
      cout << "No current tag selected or not a VECTOR3D tag." << endl;
      cout << "Abort." << endl;
      exit(1);
    }


  vName = getXmlAttribute("name"); currentTag = currentTag->child; // goto first AXIS
  // extract AXIS Tags
  for(int i=0; i<3; i++)
    {
    vAxis[i] = getXmlAttribute("name"); currentTag = currentTag->child; // goto <DETAIL name="min">
    vMin[i]  = getXmlContent().Atof(); currentTag = currentTag->next;  // goto <DETAIL name="ampx">
    vMax[i]  = getXmlContent().Atof(); currentTag = currentTag->next;  // goto <DETAIL name="pts">
    vPts[i]  = getXmlContent().Atoi(); currentTag = currentTag->father->next; // goto next AXIS or DATA
    }
  // extract ARRAY Tag -> VectorField object
  // CAP 3/18/2023
  // instead of instantiating directly, invoke a factory and set the object
  // the factory then handles the memory ampnagement.
  //  vectorField = new VectorField(vName.Data(),vMin[0],vampx[0],vPts[0],vMin[1],vampx[1],vPts[1],vMin[2],vampx[2],vPts[2]);
  Factory<VectorField> * factory = VectorField::getFactory();
  vectorField = factory->getNextObject();
  vectorField->setValue(vName.Data(),vMin[0],vMax[0],vPts[0],vMin[1],vMax[1],vPts[1],vMin[2],vMax[2],vPts[2]);

  XmlParser parser;
  std::ifstream & inputFile = parser.openInputFile(inputPath,inputFileName,".xml","");
  inputFile.seekg(currentTag->begin);
  inputFile.precision(16);
  String buffer = "";
  //char   chr;

  for(int i=0; i<vPts[0]; i++)
    for(int j=0; j<vPts[1]; j++)
      for(int k=0; k<vPts[2]; k++)
        inputFile >> (*vectorField)(i,j,k);
  inputFile.close();
//  if (reportTrace(__FUNCTION__))
//    {
//    cout
//    << "Extracted vector "
//    <<vectorField->getName()
//    <<"("
//    <<vAxis[0]<<"["<< vectorField->getXMin() << "," << vectorField->getXMax() << "], "
//    <<vAxis[1]<<"["<< vectorField->getYMin() << "," << vectorField->getYMax() << "], "
//    <<vAxis[2]<<"["<< vectorField->getZMin() << "," << vectorField->getZMax() << "])"
//    << endl;
//    }
  return vectorField;
}
