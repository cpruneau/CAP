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

#ifndef _TH2_Hypersurface_Lhyquid2D_H_
#define _TH2_Hypersurface_Lhyquid2D_H_
#include "Thermodynamics.hpp"
#include "VectorField.hpp"
#include "Hypersurface.hpp"

class Hypersurface_Lhyquid2D : public Hypersurface
{
public:
  Hypersurface_Lhyquid2D(const Configuration & _requestedConfiguration,
                         Thermodynamics * _thermodynamics );
  virtual ~Hypersurface_Lhyquid2D() {}

  virtual double getDSigmaP(double aMt, double aPt, double aPhiP, double aRapP);
  virtual double getPdotU(  double aMt, double aPt, double aPhiP, double aRapP);
  virtual void   setPositionOnHypersurface(LorentzVector & position);
  virtual void   setDefaultConfiguration();
  virtual void   configure();
  virtual double getHyperCubeSpatialVolume() const;
  virtual void   printConfiguration(ostream & output);
  virtual void   readFromXmlFile(const char * inputPath,
                                 const char * inputFileName);
  virtual void   writeToXmlFile(const char * outputPath,
                                const char * outputFileName);

protected:

  // velocity on the hypersurface
  VectorField* mFluidVt;
  VectorField* mFluidPhi;

  ClassDef(Hypersurface_Lhyquid2D,0)
};

#endif

/*! @file Hypersurface_Lhyquid2D.h
 * @brief Definition of Hypersurface_Lhyquid2D class. 2+1 boost-invariant freeze-out hypersurface.
 */
/*! @class Hypersurface_Lhyquid2D
 * @brief 2+1 boost-invariant freeze-out hypersurface.
 *
 * Output form the <b>Lhyquid</b> hydrodynamic code.<br />
 * Point on the hypersurface is given by a four-vector originating from the space-time point
 * @f[ x_1^\mu = (t_1,x_1,y_1,z_1) = (\tau_i \cosh(\eta_\parallel), 0, 0, \tau_i \sinh(\eta_\parallel)) @f]
 * The four-vector's end point @f$ x_2^\mu = (t_2,x_2,y_2,z_2) @f$ is defined by function @f$ d @f$ parametrized
 * by two angles:
 * <ul>
 *   <li> @f$ \zeta \in <0, \pi/2> @f$ - angle in the @f$ \rho-\tau @f$ plane (Hypersurface_Lhyquid2D::zeta)
 *   <li> @f$ \phi \in <0, 2\pi> @f$ - azimuthal angle in @f$ x-y @f$ plane (Hypersurface_Lhyquid2D::phiS)
 * </ul>
 * The space-time coordinates are calculated in the following way:
 * @f{eqnarray}{
 *   t_2 &=& \left[ \tau_i + d(\zeta,\phi) \sin(\zeta) \right] \cosh(\eta_\parallel) \nonumber \\
 *   x_2 &=& d(\zeta,\phi) \cos(\zeta) \cos(\phi) \nonumber \\
 *   y_2 &=& d(\zeta,\phi) \cos(\zeta) \sin(\phi) \nonumber \\
 *   z_2 &=& \left[ \tau_i + d(\zeta,\phi) \sin(\zeta) \right] \sinh(\eta_\parallel) \nonumber
 * @f}
 * Other useful variables:
 * <ul>
 *   <li> @f$ \rho = d(\zeta,\phi) \cos(\zeta) @f$
 *   <li> @f$ \tau = \tau_i + d(\zeta,\phi) \sin(\zeta) @f$
 * </ul>
 *
 * @fn Hypersurface_Lhyquid2D::Hypersurface_Lhyquid2D()
 * @brief Default constructor.
 *
 * @fn Hypersurface_Lhyquid2D::~Hypersurface_Lhyquid2D()
 * @brief Destructor.
 *
 * @fn double Hypersurface_Lhyquid2D::getHyperCubeSpatialVolume()
 * @brief Returns the spatial-type volume of the Monte-Carlo hypercube @f$ V_{hc} = \eta_\parallel^{rng} (\zeta_{ampx}-\zeta_{min}) (\phi_{ampx} - \phi_{min})@f$
 *
 * @fn double Hypersurface_Lhyquid2D::getDSigmaP(double aMt, double aPt, double aPhiP, double aRapP);
 * @brief Return @f$ d\Sigamp_\mu p^\mu@f$ for given four-momentum of the particle.
 * @param [in] aMt particle transverse ampss
 * @param [in] aPt particle transverse momentum
 * @param [in] aPhiP particle azimuthal angle of the momentum
 * @param [in] aRapP particle rapidity
 *
 * @fn double Hypersurface_Lhyquid2D::getPdotU(  double aMt, double aPt, double aPhiP, double aRapP);
 * @brief Return @f$ p_\mu u^\mu@f$ for given four-momentum of the particle.
 * @param [in] aMt particle transverse ampss
 * @param [in] aPt particle transverse momentum
 * @param [in] aPhiP particle azimuthal angle of the momentum
 * @param [in] aRapP particle rapidity
 *
 * @fn void Hypersurface_Lhyquid2D::setPositionOnHypersurface(TRandom2* aRandom, double* aXt, double* aXx, double* aXy, double* aXz);
 * @brief sets a random position on the hypersurface and returns the space-time coordinates.
 * @param [in] aRandom pointer to <a href="http://root.cern.ch/root/html/TRandom2.html">TRandom2</a> random number generator
 * @param [out] aXt particle time
 * @param [out] aXx particle x-coordinate
 * @param [out] aXy particle y-coordinate
 * @param [out] aXz particle z-coordinate
 *
 * @var double Hypersurface_Lhyquid2D::momentumRapidityRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var double Hypersurface_Lhyquid2D::spatialRapidityRange
 * @brief @f$ \eta_\parallel^{rng} @f$, spatial rapidity range
 *
 * @var double Hypersurface_Lhyquid2D::tauI
 * @brief @f$ \tau_i \,[GeV^{-1}]@f$, initial proper time of the hydrodynamic evolution
 *
 * @var double Hypersurface_Lhyquid2D::collidingEnergy
 * @brief (optional) @f$ \sqrt{s_{NN}} \,[GeV] @f$, center of ampss energy
 *
 * @var double Hypersurface_Lhyquid2D::centralityMin
 * @brief (optional) @f$ c_{min} \,[\%] @f$, centrality lower limit
 *
 * @var double Hypersurface_Lhyquid2D::centralityMax
 * @brief (optional) @f$ c_{ampx} \,[\%] @f$, centrality higher limit
 *
 * @var double Hypersurface_Lhyquid2D::impactParameter
 * @brief (optional) @f$ b \,[fm] @f$, impact parameter
 *
 * @var double Hypersurface_Lhyquid2D::tempI
 * @brief (optional) @f$ T_i \,[MeV] @f$, temperature in the center of the fireball at time @f$ \tau_i @f$
 *
 * @var char Hypersurface_Lhyquid2D::deviceName[30]
 * @brief (optional) name of the experimental device e.g. SPS, RHIC, LHC, etc.
 *
 * @var char Hypersurface_Lhyquid2D::collidingSystem[30]
 * @brief (optional) colliding particles e.g. pp, dCu, AuAu, PbPb, etc.
 *
 * @var Thermodynamics* Hypersurface_Lhyquid2D::thermodynamics
 * @brief Pointer to the Thermodynamics class.
 */
