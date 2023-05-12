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
#include "ModelManager.hpp"
using CAP::ModelManager;
using namespace std;

ClassImp(ModelManager);

ModelManager:: ModelManager(const String & _name,
                                        const Configuration & _configuration)
:
EventTask(_name,_configuration),
modelTypeId              (0),
modelTypeName            (""),
modelIniImport           (false),
modelIniImportPath       (""),
modelIniImportFile       (""),
modelIniExport           (false),
modelIniExportPath       (""),
modelIniExportFile       (""),
modelXmlImport           (false),
modelXmlImportPath       (""),
modelXmlImportFile       (""),
modelXmlExport           (false),
modelXmlExportPath       (""),
modelXmlExportFile       (""),
{
  appendClassName("ModelManager");
}

void ModelManager::setDefaultConfiguration()
{
  addParameter("ModelTypeId",       modelTypeId);
  addParameter("ModelTypeName",     modelTypeName);
  addParameter("ModelIniImport",    modelIniImport);
  addParameter("ModelIniImportPath",modelIniImportPath);
  addParameter("ModelIniImportFile",modelIniImportFile);
  addParameter("ModelIniExport",    modelIniExport );
  addParameter("ModelIniExportPath",modelIniExportPath);
  addParameter("ModelIniExportFile",modelIniExportFile);
  addParameter("ModelXmlImport",    modelXmlImport);
  addParameter("ModelXmlImportPath",modelXmlImportPath);
  addParameter("ModelXmlImportFile",modelXmlImportFile);
  addParameter("ModelXmlExport",    modelXmlExport);
  addParameter("ModelXmlExportPath",modelXmlExportPath);
  addParameter("ModelXmlExportFile",modelXmlExportFile);
}

void ModelManager::configure()
{
  EventTask::configure();
  modelTypeId              = getValueInt("ModelTypeId"                  );
  modelTypeName            = getValueString("ModelTypeName"             );
  modelIniImport           = getValueBool("ModelIniImport"              );
  modelIniImportPath       = getValueString("ModelIniImportPath"        );
  modelIniImportFile       = getValueString("ModelIniImportFile"        );
  modelIniExport           = getValueBool("ModelIniExport"              );
  modelIniExportPath       = getValueString("ModelIniExportPath"        );
  modelIniExportFile       = getValueString("ModelIniExportFile"        );
  modelXmlImport           = getValueBool("ModelXmlImport"              );
  modelXmlImportPath       = getValueString("ModelXmlImportPath"        );
  modelXmlImportFile       = getValueString("ModelXmlImportFile"        );
  modelXmlExport           = getValueBool("ModelXmlExport"              );
  modelXmlExportPath       = getValueString("ModelXmlExportPath"        );
  modelXmlExportFile       = getValueString("ModelXmlExportFile"        );
  setSeverity();
}

void ModelManager::execute()
{
  if (reportStart(__FUNCTION__))
;
  incrementTaskExecuted();
  if (modelIniImport)  importModel();
  if (modelIniExport)  exportModel();
  if (reportEnd(__FUNCTION__))
;
}

void ModelManager::importModel()
{
}

void ModelManager::exportModel()
{
}
