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

#ifndef _TH2_EVENT_H_
  #define _TH2_EVENT_H_

#include <list>
#include <vector>
#include <TRandom2.h>
#include "ParticleDb.hpp"
#include "Integrator.hpp"
#include "Particle.hpp"

class ThEvent {
  
  public:
    ThEvent();
    ThEvent(ParticleDB* aDB, Integrator* aInteg);
    ~ThEvent();

    void		 Reset(int aEventIter=0);  
    std::vector<Particle>* getParticleList();
    Integrator*		 getIntegrator() const;
    ParticleDB*		 getParticleDB() const;
    unsigned int	 getEventID() const;

    void		 GeneratePrimordials(int aSeed=0);
    void		 DecayParticles(int aSeed=0);

  private:
    void readParameters();
    void GenerateMultiplicities();
    
    std::vector<Particle>	mParticles;
    std::vector<int>	multiplicities;
    unsigned int	mEventID;
    ParticleDB*		mPartDB;
    Integrator*		mInteg;
    int			mDistribution;	// type of multiplicity distribution: 0 = Poissonian, 1 - NegativeBinomial
};

#endif

/*! @file ThEvent.h
 * @brief Definition of ThEvent class. Generates primordial, decays resonances and stores particles.
 */
/*! @class ThEvent
 * @brief Generates primordial particles and decays resonances. Created Particle is stored in a list-type container class.
 * 
 * Primordial particles are generated with a Monte-Carlo method by a Model via Integrator class. Seed of the random number
 * generator can be set by hand or initiated with system time.
 * 
 * The <c><b>THERMINATOR 2</b></c> ampin settings file <b>therminator.ini</b> options used here:
 * <table>
 *   <tr><th>Keyword</th>			<th>Description</th></tr>
 *   <tr><td>MultiplicityDistribution</td>	<td> distribution of the primordial particle multiplicity
 * 						     <table>
 *						       <tr><td><b>"Poisson"</b></td>	<td>Poissonian type</td></tr>
 * 						       <tr><td>"NegativeBinomial"</td>	<td>Negative Binomial type</td></tr>
 * 						     </table>
 * 						</td></tr>
 * </table>
 * 
 * ThEvent randomly generates the average multiplicity of primordial particles according to the given distribution, 
 * by default with Poissonian.
 *
 * @fn ThEvent::ThEvent()
 * @brief Default constructor.
 *
 * @fn ThEvent::ThEvent(ParticleDB* aDB, Integrator* aInteg)
 * @brief Creates an ThEvent with particles from database and generated with given Model via Integrator.
 * @param [in] aDB pointer to database with ParticleType to be generated in event.
 * @param [in] aInteg pointer to Integrator that uses Model to generate particles.
 *
 * @fn ThEvent::~ThEvent()
 * @brief Destructor.
 *
 * @fn void ThEvent::Reset(int aEventIter=0)
 * @brief Resets the ThEvent. Calculate eventID (CRC-32) from the current date and event iterator.
 * @param [in] aEventIter event iterator
 * 
 * @fn std::vector<Particle>* ThEvent::getParticleList()
 * @brief Returns the pointer to Particle list created in ThEvent.
 *
 * @fn Integrator* ThEvent::getIntegrator() const
 * @brief Returns the pointer to Integrator.
 *
 * @fn ParticleDB* ThEvent::getParticleDB() const
 * @brief Returns the pointer to ParticleDB.
 *
 * @fn unsigned int ThEvent::getEventID() const;
 * @brief Returns the event ID.
 * 
 * @fn void ThEvent::GeneratePrimordials(int aSeed=0)
 * @brief Generates primordial particles and stores them in the particle list. 
 * 
 * See Integrator::GenerateParticles() for details.<br />
 * @param [in] aSeed set the random generator seed to the given number. <c>aSeed=0</c> initiates random number 
 * generator seed with system time.
 *
 * @fn void ThEvent::DecayParticles(int aSeed=0)
 * @brief Uses a ParticleDecayer object to decay all unstable particles in the Particle list.
 *
 * See ParticleDecayer::DecayParticle() for details. <br />
 * Created child particles are appended to the end of the list. Decay cascade continues until all unstable 
 * particles decay and ParticleDecayer reaches the end of the list. 
 * @param [in] aSeed set the ParticleDecayer random generator seed to the given number.
 * <c>aSeed=0</c> initiates ParticleDecayer random number generator seed with system time.
 *
 * @fn void ThEvent::Randomize()
 * @brief Initiates random number generator seed with system time.
 */
