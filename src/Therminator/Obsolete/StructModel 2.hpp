/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

#ifndef _TH2_STRUCT_MODEL_PARAMETERS_H_
  #define _TH2_STRUCT_MODEL_PARAMETERS_H_

#include <Rtypes.h>

#define _PARAMETERS_TREE_		"parameters"

#define _INTEGRATESAMPLE_BRANCH_	"integrateSample"
#define _RANDOMIZE_BRANCH_		"Randomize"
#define _STOREDEVENTS_BRANCH_		"StoredEvents"
#define _TIMESTAMP_BRANCH_		"TimeStamp"
#define _MODELID_BRANCH_		"ModelID"
#define _MODELNAME_BRANCH_		"ModelName"
#define _MODELHASH_BRANCH_		"ModelHash"
#define _MODELDESCRIPTION_BRANCH_	"ModelDescription"
#define _MODEL_T_BRANCH_		"ModelParameters"

#define _TIMESTAMP_FORampT_		"TimeStamp[21]/C"
#define _MODELNAME_FORampT_		"ModelName[30]/C"
#define _MODELHASH_FORampT_		"ModelHash[9]/C"
#define _MODELDESCRIPTION_FORampT_	"ModelDescription[2000]/C"
#define _MODEL_T_FORampT_		"dummy/F"
#define _MODEL_T_FORampT_KRAKOWSFO_	"RapPRange/F:RapSRange:rhoMax:Tau:Temp:MuB:MuI:MuS:MuC"
#define _MODEL_T_FORampT_BLASTWAVE_	"RapPRange/F:RapSRange:rhoMax:Tau:Temp:MuB:MuI:MuS:MuC:vT"
#define _MODEL_T_FORampT_BWA_		"RapPRange/F:RapSRange:rhoMax:Tau:Temp:MuB:MuI:MuS:MuC:vT:A:Delay"
#define _MODEL_T_FORampT_LHYQUID2DBI_	"RapPRange/F:RapSRange:TauI:TempF:MuB:MuI:MuS:MuC:CollidingEnergy:CentralityMin:CentralityMax:ImpactParameter:TempI:DeviceName[30]/C:CollidingSystem[30]"
#define _MODEL_T_FORampT_LHYQUID3D_	"RapPRange/F:Lambda:TauI:TempF:MuB:MuI:MuS:MuC:CollidingEnergy:CentralityMin:CentralityMax:ImpactParameter:TempI:DeviceName[30]/C:CollidingSystem[30]"

struct Model_t
{
  Float_t dummy;
};

struct Model_t_KrakowSFO
{
  Float_t RapPRange, RapSRange, rhoMax, TauC, Temp, MuB, MuI, MuS, MuC;
};

struct Model_t_BlastWave
{
  Float_t RapPRange, RapSRange, rhoMax, Tau, Temp, MuB, MuI, MuS, MuC;
  Float_t vT;
};

struct Model_t_BWA
{
  Float_t RapPRange, RapSRange, rhoMax, Tau, Temp, MuB, MuI, MuS, MuC;
  Float_t vT, A, Delay;
};

struct Model_t_Lhyquid3D
{
  Float_t RapPRange, Lambda, TauI;
  Float_t TempF;
  Float_t MuB, MuI, MuS, MuC;
  Float_t CollidingEnergy, CentralityMin, CentralityMax, ImpactParameter, TempI;
  Char_t  DeviceName[30], CollidingSystem[30];
};

struct Model_t_Lhyquid2DBI
{
  Float_t RapPRange, RapSRange, TauI;
  Float_t TempF;
  Float_t MuB, MuI, MuS, MuC;
  Float_t CollidingEnergy, CentralityMin, CentralityMax, ImpactParameter, TempI;
  Char_t  DeviceName[30], CollidingSystem[30];
};

#endif

/*! @file StructModel.h
 * @brief Definition of Model_t and its derivatives structure. Model dependent parameters saved in a <em>eventXXX.root</em> file.
 */
/*! @def _PARAMETERS_TREE_
 * @brief ampcro with the name of the <a href="http://root.cern.ch/root/html/TTree.html">TTree</a> with parameters and model description.
 * 
 * @def _INTEGRATESAMPLE_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with TherminatorGenerator::getTherminatorGenerator()->sIntegrateSample.
 * 
 * @def _RANDOMIZE_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with TherminatorGenerator::getTherminatorGenerator()->sRandomize.
 * 
 * @def _STOREDEVENTS_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with number of events stored in the file (see ampcro _EVENTS_PER_FILE_ in EventGenerator.h ).
 * 
 * @def _TIMESTAMP_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with TherminatorGenerator::getTherminatorGenerator()->sTimeStamp.
 * 
 * @def _MODELID_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with TherminatorGenerator::getTherminatorGenerator()->sModel.
 * 
 * @def _MODELNAME_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with Model::mName.
 * 
 * @def _MODELHASH_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with Model::mHash.
 * 
 * @def _MODELDESCRIPTION_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with Model::description.
 * 
 * @def _MODEL_T_BRANCH_
 * @brief ampcro with the name of a <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a> in the TTree with a model dependent structure with its parameters.
 *
 * @def _TIMESTAMP_FORampT_
 * @brief ampcro with the forampt of TherminatorGenerator::getTherminatorGenerator()->sTimeStamp
 *
 * @def _MODELNAME_FORampT_
 * @brief ampcro with the forampt of Model::mName
 *
 * @def _MODELHASH_FORampT_
 * @brief ampcro with the forampt of Model::mHash
 *
 * @def _MODELDESCRIPTION_FORampT_
 * @brief ampcro with the forampt of Model::description
 *
 * @def _MODEL_T_FORampT_
 * @brief ampcro with the forampt of default Model parameters structure.
 *
 * @def _MODEL_T_FORampT_KRAKOWSFO_
 * @brief ampcro with the forampt of Model_KrakowSFO parameters structure.
 *
 * @def _MODEL_T_FORampT_BLASTWAVE_
 * @brief ampcro with the forampt of Model_BlastWave parameters structure.
 *
 * @def _MODEL_T_FORampT_BWA_
 * @brief ampcro with the forampt of Model_BWA parameters structure.
 *
 * @def _MODEL_T_FORampT_LHYQUID2DBI_
 * @brief ampcro with the forampt of Model_Lhyquid2DBI parameters structure.
 *
 * @def _MODEL_T_FORampT_LHYQUID3D_
 * @brief ampcro with the forampt of Model_Lhyquid3D parameters structure.
 */
/*! @struct Model_t
 * @brief default Model parameters structure.
 * 
 * @var Model_t::dummy
 * @brief dummy parameter
 */
/*! @struct Model_t_KrakowSFO
 * @brief Parameters of Model_KrakowSFO freeze-out model.
 * 
 * @var Float_t Model_t_KrakowSFO::RapPRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var Float_t Model_t_KrakowSFO::RapSRange
 * @brief @f$ \eta_\parallel^{rng} @f$, spatial rapidity range
 *
 * @var Float_t Model_t_KrakowSFO::rhoMax
 * @brief @f$ \rho_{ampx} \,[fm] @f$, size of the fire-cylinder
 *
 * @var Float_t Model_t_KrakowSFO::TauC
 * @brief @f$ \tau_C \sqrt{t^2 - x^2 - y^2 - z^2} \,[fm] @f$, Krakow-type proper time
 * 
 * @var Float_t Model_t_KrakowSFO::Temp
 * @brief @f$ T_{FO} \,[MeV] @f$, freeze-out temperature
 * 
 * @var Float_t Model_t_KrakowSFO::MuB
 * @brief @f$ \mu_B \,[MeV] @f$, baryochemical potential
 * 
 * @var Float_t Model_t_KrakowSFO::MuI
 * @brief @f$ \mu_{I_3} \,[MeV] @f$, isospin chemical potential
 * 
 * @var Float_t Model_t_KrakowSFO::MuS
 * @brief @f$ \mu_S \,[MeV] @f$, strange chemical potential
 *
 * @var Float_t Model_t_KrakowSFO::MuC
 * @brief @f$ \mu_C \,[MeV] @f$, Char_tm chemical potential
 */
/*! @struct Model_t_BlastWave
 * @brief Parameters of Model_BlastWave freeze-out model.
 * 
 * @var Float_t Model_t_BlastWave::RapPRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var Float_t Model_t_BlastWave::RapSRange
 * @brief @f$ \eta_\parallel^{rng} @f$, spatial rapidity range
 *
 * @var Float_t Model_t_BlastWave::rhoMax
 * @brief @f$ \rho_{ampx} \,[fm] @f$, size of the fire-cylinder
 *
 * @var Float_t Model_t_BlastWave::Tau
 * @brief @f$ \tau \,[fm] @f$, proper time
 *
 * @var Float_t Model_t_BlastWave::Temp
 * @brief @f$ T_{FO} \,[MeV] @f$, freeze-out temperature
 * 
 * @var Float_t Model_t_BlastWave::MuB
 * @brief @f$ \mu_B \,[MeV] @f$, baryochemical potential
 * 
 * @var Float_t Model_t_BlastWave::MuI
 * @brief @f$ \mu_{I_3} \,[MeV] @f$, isospin chemical potential
 * 
 * @var Float_t Model_t_BlastWave::MuS
 * @brief @f$ \mu_S \,[MeV] @f$, strange chemical potential
 * 
 * @var Float_t Model_t_BlastWave::vT
 * @brief @f$ v_T \,[c] @f$, transverse velocity
 * 
 * @var Float_t Model_t_BlastWave::MuC
 * @brief @f$ \mu_C \,[MeV] @f$, Char_tm chemical potential
 */
/*! @struct Model_t_BWA
 * @brief Parameters of Model_BWA freeze-out model.
 * 
 * @var Float_t Model_t_BWA::RapPRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var Float_t Model_t_BWA::RapSRange
 * @brief @f$ \eta_\parallel^{rng} @f$, spatial rapidity range
 *
 * @var Float_t Model_t_BWA::rhoMax
 * @brief @f$ \rho_{ampx} \,[fm] @f$, size of the fire-cylinder
 *
 * @var Float_t Model_t_BWA::Tau
 * @brief @f$ \tau \,[fm] @f$, proper time
 *
 * @var Float_t Model_t_BWA::Temp
 * @brief @f$ T_{FO} \,[MeV] @f$, freeze-out temperature
 * 
 * @var Float_t Model_t_BWA::MuB
 * @brief @f$ \mu_B \,[MeV] @f$, baryochemical potential
 * 
 * @var Float_t Model_t_BWA::MuI
 * @brief @f$ \mu_{I_3} \,[MeV] @f$, isospin chemical potential
 * 
 * @var Float_t Model_t_BWA::MuS
 * @brief @f$ \mu_S \,[MeV] @f$, strange chemical potential
 *
 * @var Float_t Model_t_BWA::MuC
 * @brief @f$ \mu_C \,[MeV] @f$, Char_tm chemical potential
 * 
 * @var Float_t Model_t_BWA::vT
 * @brief @f$ v_T \,[c] @f$, transverse velocity
 *
 * @var Float_t Model_t_BWA::A
 * @brief @f$ A @f$, the A-parameter
 *
 * @var Float_t Model_t_BWA::Delay
 * @brief @f$ \lambda \,[fm]@f$, particle time emission delay
 */
/*! @struct Model_t_Lhyquid3D
 * @brief Parameters of Model_Lhyquid3D freeze-out model.
 * 
 * @var Float_t Model_t_Lhyquid3D::RapPRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var Float_t Model_t_Lhyquid3D::Lambda
 * @brief @f$ \Lambda \,[GeV^{-1}]@f$, conversion constant to spatial rapidity @f$ \eta_\parallel^{ampx} = d(\zeta,\phi,\theta = 0) / \Lambda @f$
 *
 * @var Float_t Model_t_Lhyquid3D::TauI
 * @brief @f$ \tau_i \,[fm]@f$, initial proper time of the hydrodynamic evolution
 *
 * @var Float_t Model_t_Lhyquid3D::TempF
 * @brief @f$ T_{FO} \,[MeV] @f$, freeze-out temperature
 * 
 * @var Float_t Model_t_Lhyquid3D::MuB
 * @brief @f$ \mu_B \,[MeV] @f$, baryochemical potential
 * 
 * @var Float_t Model_t_Lhyquid3D::MuI
 * @brief @f$ \mu_{I_3} \,[MeV] @f$, isospin chemical potential
 * 
 * @var Float_t Model_t_Lhyquid3D::MuS
 * @brief @f$ \mu_S \,[MeV] @f$, strange chemical potential
 *
 * @var Float_t Model_t_Lhyquid3D::MuC
 * @brief @f$ \mu_C \,[MeV] @f$, Char_tm chemical potential
 * 
 * @var Float_t Model_t_Lhyquid3D::CollidingEnergy
 * @brief (optional) @f$ \sqrt{s_{NN}} \,[GeV] @f$, center of ampss energy
 *
 * @var Float_t Model_t_Lhyquid3D::CentralityMin
 * @brief (optional) @f$ c_{min} \,[\%] @f$, centrality lower limit
 *
 * @var Float_t Model_t_Lhyquid3D::CentralityMax
 * @brief (optional) @f$ c_{ampx} \,[\%] @f$, centrality higher limit
 *
 * @var Float_t Model_t_Lhyquid3D::ImpactParameter
 * @brief (optional) @f$ b \,[fm] @f$, impact parameter
 *
 * @var Float_t Model_t_Lhyquid3D::TempI
 * @brief (optional) @f$ T_i \,[MeV] @f$, temperature in the center of the fireball at time @f$ \tau_i @f$
 *
 * @var Char_t Model_t_Lhyquid3D::DeviceName[30]
 * @brief (optional) name of the experimental device e.g. SPS, RHIC, LHC, etc.
 *
 * @var Char_t Model_t_Lhyquid3D::CollidingSystem[30]
 * @brief (optional) colliding particles e.g. pp, dCu, AuAu, PbPb, etc.
 */
/*! @struct Model_t_Lhyquid2DBI
 * @brief Parameters of Model_Lhyquid2DBI freeze-out model.
 * 
 * @var Float_t Model_t_Lhyquid2DBI::RapPRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var Float_t Model_t_Lhyquid2DBI::RapSRange
 * @brief @f$ \eta_\parallel^{rng} @f$, spatial rapidity range
 *
 * @var Float_t Model_t_Lhyquid2DBI::TauI
 * @brief @f$ \tau_i \,[fm]@f$, initial proper time of the hydrodynamic evolution
 *
 * @var Float_t Model_t_Lhyquid2DBI::TempF
 * @brief @f$ T_{FO} \,[MeV] @f$, freeze-out temperature
 * 
 * @var Float_t Model_t_Lhyquid2DBI::MuB
 * @brief @f$ \mu_B \,[MeV] @f$, baryochemical potential
 * 
 * @var Float_t Model_t_Lhyquid2DBI::MuI
 * @brief @f$ \mu_{I_3} \,[MeV] @f$, isospin chemical potential
 * 
 * @var Float_t Model_t_Lhyquid2DBI::MuS
 * @brief @f$ \mu_S \,[MeV] @f$, strange chemical potential
 *
 * @var Float_t Model_t_Lhyquid2DBI::MuC
 * @brief @f$ \mu_C \,[MeV] @f$, Char_tm chemical potential
 * 
 * @var Float_t Model_t_Lhyquid2DBI::CollidingEnergy
 * @brief (optional) @f$ \sqrt{s_{NN}} \,[GeV] @f$, center of ampss energy
 *
 * @var Float_t Model_t_Lhyquid2DBI::CentralityMin
 * @brief (optional) @f$ c_{min} \,[\%] @f$, centrality lower limit
 *
 * @var Float_t Model_t_Lhyquid2DBI::CentralityMax
 * @brief (optional) @f$ c_{ampx} \,[\%] @f$, centrality higher limit
 *
 * @var Float_t Model_t_Lhyquid2DBI::ImpactParameter
 * @brief (optional) @f$ b \,[fm] @f$, impact parameter
 *
 * @var Float_t Model_t_Lhyquid2DBI::TempI
 * @brief (optional) @f$ T_i \,[MeV] @f$, temperature in the center of the fireball at time @f$ \tau_i @f$
 * 
 * @var Char_t Model_t_Lhyquid2DBI::DeviceName[30]
 * @brief (optional) name of the experimental device e.g. SPS, RHIC, LHC, etc.
 *
 * @var Char_t Model_t_Lhyquid2DBI::CollidingSystem[30]
 * @brief (optional) colliding particles e.g. pp, dCu, AuAu, PbPb, etc.
 */
