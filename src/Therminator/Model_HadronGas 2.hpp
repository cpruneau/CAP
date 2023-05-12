/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *
 ********************************************************************************/

#ifndef _TH2_MODEL_HadronGas
#define _TH2_MODEL_HadronGas

#include "Model.hpp"
#include "Thermodynamics.hpp"

class Model_HadronGas
: public Model
{
public:
  Model_HadronGas(const Configuration & _requestedConfiguration);
  virtual ~Model_HadronGas() {}
  virtual double getIntegrand(ParticleType& aPartType);
 
protected:

  ClassDef(Model_HadronGas,0)
};

#endif

/*! @file Model_BlastWave.h
 * @brief Definition of Model_BlastWave class. Blast-Wave Model.
 */
/*! @class Model_BlastWave
 * @brief Blast-Wave freeze-out model.
 *
 * <table>
 *   <tr><th>TherminatorGenerator::getTherminatorGenerator()->sModel</th>	<th>Model::mModelName</th>	<th>Description</th></tr>
 *   <tr><td>1</td>		<td>"BlastWave"</td>		<td>Blast-Wave</td></tr>
 * </table>
 *
 * The integrand has the form
 * @f{eqnarray}{
 *   I			&=& \frac{2 s+1}{\left(2\pi\right)^3} \,d\Sigamp_\mu p^\mu \left[ \exp \left( \frac{p_\mu u^\mu - \mu_{ch}}{T}\right) \pm 1\right]^{-1} p_T \,dp_T \,d\phi_p \,dy \nonumber \\
 *   \nonumber\\ \nonumber\\
 *   d\Sigamp_\mu p^\mu	&=& m_T \cosh(\eta_\parallel - y) \,\tau \,\rho \,d\rho \,d\phi \,d\eta_\parallel \nonumber \nonumber \\
 *   p_\mu u^\mu	&=& \frac{1}{\sqrt{1-v_T^2}} \left[ m_T \cosh(\eta_\parallel - y) - v_T \,p_T \cos(\phi - \phi_p)\right] \nonumber
 * @f}
 * where (link to all variable tables)
 *
 * @fn Model_BlastWave::Model_BlastWave()
 * @brief Default constructor.
 *
 * @fn Model_BlastWave::Model_BlastWave(TRandom2* aRandom)
 * @brief Constructor that links a random number generator form Integrator.
 * @param [in] aRandom pointer to <a href="http://root.cern.ch/root/html/TRandom2.html">TRandom2</a> random number generator
 *
 * @fn Model_BlastWave::~Model_BlastWave()
 * @brief Destructor.
 *
 * @fn double Model_BlastWave::getIntegrand(ParticleType& aPartType)
 * @brief Generates random space-time coordinates and four-momentum of a particle of a given type
 * and returns the value of the integrand of the Cooper-Frye formula.
 * @param [in] aPartType pointer to the ParticleType class holding particle properties.
 *
 * @fn void Model_BlastWave::addParameterBranch(TTree* aTree)
 * @brief Function called by EventGenerator to add a new <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a>
 * structure to an opened event <a href="http://root.cern.ch/root/html/TFile.html">TFile</a>.
 *
 * This branch contains model parameters and model description.
 * @param [in] aTree pointer to the current <a href="http://root.cern.ch/root/html/TTree.html">TTree</a> object.
 *
 * @var double Model_BlastWave::momentumRapidityRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var double Model_BlastWave::spatialRapidityRange
 * @brief @f$ \eta_\parallel^{rng} @f$, spatial rapidity range
 *
 * @var double Model_BlastWave::rhoMax
 * @brief @f$ \rho_{ampx} \,[GeV^{-1}] @f$, size of the fire-cylinder
 *
 * @var double Model_BlastWave::tau
 * @brief @f$ \tau \,[GeV^{-1}] @f$, proper time
 *
 * @var double Model_BlastWave::vT
 * @brief @f$ v_T \,[c] @f$, transverse velocity
 *
 * @var Thermodynamics* Model_BlastWave::thermodynamics
 * @brief Pointer to the Thermodynamics class
 */
