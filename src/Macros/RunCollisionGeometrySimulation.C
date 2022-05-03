//  Created by Claude Pruneau on 6/19/2020.
//  Copyright © 2020 Claude Pruneau. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
#include "CollisionGeometry.hpp"
#include "CollisionGeometryConfiguration.hpp"
#include "CollisionGeometryGenerator.hpp"
#include "CollisionGeometryAnalyzer.hpp"
#include "TaskIterator.hpp"

int RunCollisionGeometrySimulation()
{
  // load stuff here...
  MessageLogger::LogLevel messageLevel = MessageLogger::Info;

  TaskIterator * taskIterator = new TaskIterator();
  taskIterator->setNEventRequested(10000000);
  taskIterator->setNEventReported(100000);
  taskIterator->setReportLevel(messageLevel);
  taskIterator->setNEventPartialSave(100000);
  taskIterator->setPartialSave(true);
  taskIterator->setSubsampleAnalysis(true);

  CollisionGeometryConfiguration * geometryConfiguration = new CollisionGeometryConfiguration("PYTHIACollisionGeometry","PYTHIACollisionGeometry","1.0");
  geometryConfiguration->aNucleusZ  = 82;
  geometryConfiguration->aNucleusA  = 208;
  geometryConfiguration->bNucleusZ  = 82;
  geometryConfiguration->aNucleusA  = 208;
  geometryConfiguration->inputPath  = getenv("WAC_INPUT_PATH");
  geometryConfiguration->outputPath = getenv("WAC_OUTPUT_PATH");
  geometryConfiguration->rootOuputFileName =  "CollisionGeometry";
  geometryConfiguration->histoBaseName =  "geom";
  geometryConfiguration->minB = 0.0;
  geometryConfiguration->maxB = 21.0;

  geometryConfiguration->aParA = 6.62;
  geometryConfiguration->aParB = 0.546;
  geometryConfiguration->aParC = 0.0;
  geometryConfiguration->aNR   = 11000;
  geometryConfiguration->aMinR = 0.0;
  geometryConfiguration->aMaxR = 11.0;
  geometryConfiguration->bParA = 6.62;
  geometryConfiguration->bParB = 0.546;
  geometryConfiguration->bParC = 0.0;
  geometryConfiguration->bNR   = 11000;
  geometryConfiguration->bMinR = 0.0;
  geometryConfiguration->bMaxR = 11.0;
  geometryConfiguration->nnCrossSection = 6.9;  // in fm^2 -- Config C
  geometryConfiguration->nBins_b = 120;
  geometryConfiguration->min_b   = 0.0;
  geometryConfiguration->max_b   = 24.0;
  geometryConfiguration->nBins_nPart = 100;
  geometryConfiguration->min_nPart   = 0;
  geometryConfiguration->max_nPart   = 500;
  geometryConfiguration->nBins_nBinary = 600;
  geometryConfiguration->min_nBinary   = 0;
  geometryConfiguration->max_nBinary   = 3000;
  geometryConfiguration->calculateDerivedHistograms = true;

  CollisionGeometryGenerator * collisionGeometryGenerator = new CollisionGeometryGenerator("PbPbWSGen",geometryConfiguration, messageLevel);
  CollisionGeometry * collisionGeometry = collisionGeometryGenerator->getCollisionGeometry();
  CollisionGeometryAnalyzer  * collisionGeometryAnalyzer  = new CollisionGeometryAnalyzer ("PbPbWS-ConfigC", geometryConfiguration, collisionGeometry, messageLevel);
  taskIterator->addSubtask( collisionGeometryGenerator );
  taskIterator->addSubtask( collisionGeometryAnalyzer );
  taskIterator->run();
}


// Au(197,79)+Au(197,79)
// Type             ParA     ParB   ParC
// HardSphere       6.5^3    0.0    0.0
// WoodsSaxon       6.5      0.535  0.0
// WoodsSaxonHard   6.5      0.020  0.0
// Exponential      6.5      0.0    0.0
// Gaussian         6.5^1/2  0.0    0.0
// DoubleGaussian   -        -      -



// Pb(208,82)+Pb(208,82)
// Type             ParA       ParB   ParC
// HardSphere       7.1^3      0.0    0.0
// WoodsSaxon       7.1        0.535  0.0
// WoodsSaxonHard   7.1        0.020  0.0
// Exponential      7.1        0.0    0.0
// Gaussian         7.1^1/2    0.0    0.0
// DoubleGaussian   -          -      -


