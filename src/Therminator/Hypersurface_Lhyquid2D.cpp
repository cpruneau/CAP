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
//#include <TMath.h>
#include "Configuration.hpp"
#include "XmlParser.hpp"
#include "XmlVectorField.hpp"
#include "Hypersurface_Lhyquid2D.hpp"

using namespace std;
using namespace TMath;
using CAP::Configuration;
using CAP::XmlParser;
using CAP::XmlVectorField;

ClassImp(Hypersurface_Lhyquid2D);

Hypersurface_Lhyquid2D::Hypersurface_Lhyquid2D(const Configuration & _requestedConfiguration,
                                               Thermodynamics * _thermodynamics )
:
Hypersurface(_requestedConfiguration,_thermodynamics),
mFluidVt(nullptr),
mFluidPhi(nullptr)
{ }

double  Hypersurface_Lhyquid2D::getHyperCubeSpatialVolume() const
{
  double xRange = mDistance->getXMax() - mDistance->getXMin();
  double yRange = mDistance->getYMax() - mDistance->getYMin();
  return xRange*yRange*spatialRapidityRange;
}

void Hypersurface_Lhyquid2D::setDefaultConfiguration()
{
  Hypersurface::setDefaultConfiguration();
  addParameter("RapPRange",         momentumRapidityRange);
  addParameter("RapSRange",         spatialRapidityRange);
}

void Hypersurface_Lhyquid2D::configure()
{
  Hypersurface::configure();
  momentumRapidityRange  = getValueDouble("RapPRange");
  spatialRapidityRange   = getValueDouble("RapSRange");
}

void Hypersurface_Lhyquid2D::printConfiguration(ostream & output)
{
  output << endl;
  output << "=================================================================" << endl;
  output << "=================================================================" << endl;
  output << "Therminator 2" << endl;
  output << "Model BWA" << endl;
  output << "Release, 2.0.3, 1 February 2011 " << endl;
  output << endl;
  output << "Authors:" << endl;
  output << "    Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)" << endl;
  output << "    Adam Kisiel         (kisiel@if.pw.edu.pl)" << endl;
  output << "    Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)" << endl;
  output << "    Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)" << endl;
  output << endl;
  output << "Hypersurface Model Parameters:" << endl;
  output << "    Severity...........: " << getSeverityName() << endl;
  output << "    DeviceName.........: " << deviceName << endl;
  output << "    CollidingSystem....: " << collidingSystem << endl;
  output << "    Lambda.............: " << lambda << endl;
  output << "    RapPRange..........: " << momentumRapidityRange << endl;
  output << "    RapSRange..........: " << spatialRapidityRange << endl;
  output << "    TauI...............: " << tauI;
  output << "    CollidingEnergy....: " << collidingEnergy;
  output << "    CentralityMin......: " << centralityMin;
  output << "    CentralityMax......: " << centralityMax;
  output << "    ImpactParameter....: " << impactParameter;
  output << "    TempI..............: " << tempI;
  output << "    Temperature........: " << thermodynamics->getTemperature() << endl; // [MeV]
  output << "    MuB................: " << thermodynamics->getMuB() << endl;// [MeV]
  output << "    MuI................: " << thermodynamics->getMuI() << endl;// [MeV]
  output << "    MuS................: " << thermodynamics->getMuS() << endl;// [MeV]
  output << "    MuC................: " << thermodynamics->getMuC() << endl;// [MeV]
  output << "=================================================================" << endl;
  output << "=================================================================" << endl;
}

void Hypersurface_Lhyquid2D::setPositionOnHypersurface(LorentzVector & position)
{
  zeta      = mDistance->getXMin() + (mDistance->getXMax() - mDistance->getXMin()) * gRandom->Rndm();
  phiS      = mDistance->getYMin() + (mDistance->getYMax() - mDistance->getYMin()) * gRandom->Rndm();
  rapidityS = spatialRapidityRange * (gRandom->Rndm() - 0.5); // * spatialRapidityRange;
  Dhs       = mDistance     ->interpolate(zeta, phiS, 0.0);
  dDdPhi    = mDistanceDPhi ->interpolate(zeta, phiS, 0.0);
  dDdZeta   = mDistanceDZeta->interpolate(zeta, phiS, 0.0);
  vT	      = mFluidVt      ->interpolate(zeta, phiS, 0.0);
  gammaT    = 1.0/sqrt(1 - vT*vT);
  phiF	    = mFluidPhi     ->interpolate(zeta, phiS, 0.0);
  Tau       = tauI + Dhs * sin(zeta);
  rho       = Dhs * cos(zeta);
  position.SetXYZT(rho*cos(phiS),rho*sin(phiS),Tau*sinh(rapidityS),Tau*cosh(rapidityS));
}

double Hypersurface_Lhyquid2D::getPdotU(double aMt, double aPt, double aPhiP, double aRapP)
{
  return gammaT * (aMt * cosh(aRapP - rapidityS) - vT * aPt * cos(phiS - aPhiP + phiF));
}

double Hypersurface_Lhyquid2D::getDSigmaP(double aMt, double aPt, double aPhiP, double aRapP)
{
  return Dhs * (tauI + Dhs * sin(zeta))
  * (
     Dhs     * cos(zeta) * ( aMt * sin(zeta) * cosh(aRapP - rapidityS) + aPt * cos(zeta) * cos(phiS - aPhiP))
     + dDdZeta * cos(zeta) * (-aMt * cos(zeta) * cosh(aRapP - rapidityS) + aPt * sin(zeta) * cos(phiS - aPhiP))
     + dDdPhi  * aPt * sin(phiS - aPhiP)
     );
}

void   Hypersurface_Lhyquid2D::readFromXmlFile(const char * _inputPath,
                                               const char * _inputFileName)
{
  double temperature, muB, muI, muS, muC;

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Reading from xml file named.....: " << _inputFileName << endl;
    cout << "        from path named.........: " << _inputPath << endl;
    }
  XmlParser parser;
  XmlVectorField xmlDoc;
  parser.read(xmlDoc,_inputPath,_inputFileName);
  try
  {
  xmlDoc.getXmlTag("PARAMETER","name","Tau_i");
  tauI            = xmlDoc.getXmlContent().Atof() / CAP::Physics::hBarC();       // [GeV^-1]
  xmlDoc.getXmlTag("PARAMETER","name","Temperature");
  temperature     = xmlDoc.getXmlContent().Atof() * 0.001;  // [GeV]
  xmlDoc.getXmlTag("PARAMETER","name","Mu_B");
  muB             = xmlDoc.getXmlContent().Atof() * 0.001;
  xmlDoc.getXmlTag("PARAMETER","name","Mu_I");
  muI             = xmlDoc.getXmlContent().Atof() * 0.001;
  xmlDoc.getXmlTag("PARAMETER","name","Mu_S");
  muS             = xmlDoc.getXmlContent().Atof() * 0.001;
  xmlDoc.getXmlTag("PARAMETER","name","Mu_C");
  muC             = xmlDoc.getXmlContent().Atof() * 0.001;        // [GeV]
  xmlDoc.getXmlTag("PARAMETER","name","colliding_energy");
  collidingEnergy = xmlDoc.getXmlContent().Atof();    // [GeV]
  xmlDoc.getXmlTag("PARAMETER","name","centrality_min");
  centralityMin   = xmlDoc.getXmlContent().Atof();      // [%]
  xmlDoc.getXmlTag("PARAMETER","name","centrality_ampx");
  centralityMax   = xmlDoc.getXmlContent().Atof();     // [%]
  xmlDoc.getXmlTag("PARAMETER","name","impact_parameter");
  impactParameter = xmlDoc.getXmlContent().Atof();    // [fm]
  xmlDoc.getXmlTag("PARAMETER","name","temperature_at_center");
  tempI           = xmlDoc.getXmlContent().Atof();  // [MeV]
  xmlDoc.getXmlTag("PARAMETER","name","device");
  deviceName      = xmlDoc.getXmlContent().Data();
  xmlDoc.getXmlTag("PARAMETER","name","colliding_system");
  collidingSystem = xmlDoc.getXmlContent().Data();
  xmlDoc.getXmlTag("VECTOR3D", "name","Distance");
  mDistance       = xmlDoc.getXmlVectorField();      // [GeV^-1]
  xmlDoc.getXmlTag("VECTOR3D", "name","FluidVt" );
  mFluidVt        = xmlDoc.getXmlVectorField();
  xmlDoc.getXmlTag("VECTOR3D", "name","FluidPhi");
  mFluidPhi       = xmlDoc.getXmlVectorField();      // [c]

  }
  catch (int exceptionIndex)
  {
  if (reportFatal(__FUNCTION__))
    {
    cout << endl;
    cout << "Caught exception index:" << exceptionIndex << endl;
    cout << "Did not find one of the necessary parameters in the XML file." << endl;
    cout << "Aborting execution." << endl;
    }
  exit(exceptionIndex);
  }
  thermodynamics->setTemperature(temperature);  // [GeV]
  thermodynamics->setChemistry(muB,muI,muS,muC);
  // mDistance derivatives
  try
  {
  xmlDoc.getXmlTag("VECTOR3D","name","DistanceDZeta");
  mDistanceDZeta = xmlDoc.getXmlVectorField();      // [GeV^-1/rad]
  }
  catch (int exceptionIndex)
  {
  if (reportWarning(__FUNCTION__))
    {
    cout << endl;
    cout << "Caught exception index:" << exceptionIndex << endl;
    cout << "Calculating derivative Distance->DerivativeX()" << endl;
    }
  mDistanceDZeta = mDistance->DerivativeX("DistanceDZeta");
  }
  try
  {
  xmlDoc.getXmlTag("VECTOR3D","name","DistanceDPhi");
  mDistanceDPhi = xmlDoc.getXmlVectorField();      // [GeV^-1/rad]
  }
  catch (int exceptionIndex)
  {
  if (reportWarning(__FUNCTION__))
    {
    cout << endl;
    cout << "Caught exception index:" << exceptionIndex << endl;
    cout << "Calculating derivative Distance->DerivativeY()" << endl;
    }
  mDistanceDPhi = mDistance->DerivativeY("DistanceDPhi");
  }
 }

void   Hypersurface_Lhyquid2D::writeToXmlFile(const char * outputPath __attribute__((unused)),
                                              const char * outputFileName __attribute__((unused)))
{
  if (reportWarning(__FUNCTION__))
    cout << "Function not implement for this operation yet." << endl;
}

