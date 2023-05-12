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
#include "CalibrationManager.hpp"
using CAP::CalibrationManager;
using namespace std;

ClassImp(CalibrationManager);

CalibrationManager:: CalibrationManager(const String & _name,
                                        const Configuration & _configuration)
:
EventTask(_name,_configuration),
calibsCreate             (false),
calibsReset              (false),
calibsClear              (false),
calibsForceRewrite       (false),
calibsImport             (false),
calibsImportPath         (""),
calibsImportFile         (""),
calibsExport             (false),
calibsExportAsRoot       (false),
calibsExportAsText       (false),
calibsExportPath         (""),
calibsExportFile         ("")
{
  appendClassName("CalibrationManager");
}

void CalibrationManager::setDefaultConfiguration()
{
  addParameter("CalibrationsCreate",          calibsCreate);
  addParameter("CalibrationsReset",           calibsReset);
  addParameter("CalibrationsClear",           calibsClear);
  addParameter("CalibrationsForceRewrite",    calibsForceRewrite);
  addParameter("CalibrationsImport",          calibsImport);
  addParameter("CalibrationsImportPath",      calibsImportPath);
  addParameter("CalibrationsImportFile",      calibsImportFile);
  addParameter("CalibrationsExport",          calibsExport);
  addParameter("CalibrationsExportAsRoot",    calibsExportAsRoot);
  addParameter("CalibrationsExportAsText",    calibsExportAsText);
  addParameter("CalibrationsExportPath",      calibsExportPath);
  addParameter("CalibrationsExportFile",      calibsExportFile);

}

void CalibrationManager::configure
{
  EventTask::configure();
  calibsCreate       = getValueBool("CalibrationsCreate");
  calibsReset        = getValueBool("CalibrationsReset");
  calibsClear        = getValueBool("CalibrationsClear");
  calibsForceRewrite = getValueBool("CalibrationsForceRewrite");
  calibsImport       = getValueBool("CalibrationsImport");
  calibsImportPath   = getValueString("CalibrationsImportPath");
  calibsImportFile   = getValueString("CalibrationsImportFile");
  calibsExport       = getValueBool("CalibrationsExport");
  calibsExportAsRoot = getValueBool("CalibrationsExportAsRoot");
  calibsExportAsText = getValueBool("CalibrationsExportAsText");
  calibsExportPath   = getValueString("CalibrationsExportPath");
  calibsExportFile   = getValueString("CalibrationsExportFile");
}

void CalibrationManager::execute()
{
  if (reportStart(__FUNCTION__))
;
  incrementTaskExecuted();
  if (calibsImport)  importCalibration();
  if (calibsExport)  exportCalibration();
  if (reportEnd(__FUNCTION__))
;
}

void CalibrationManager::importCalibration()
{
}

void CalibrationManager::exportCalibration()
{
}
