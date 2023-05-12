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

#ifndef _TH2_PARTICLE_DECAYER_H_
  #define _TH2_PARTICLE_DECAYER_H_

#include <list>
#include <TRandom2.h>
#include "ParticleDb.hpp"
#include "ParticleType.hpp"
#include "DecayChannel.hpp"
#include "Particle.hpp"

class ParticleDecayer {
  public:
    ParticleDecayer();
    ParticleDecayer(ParticleDB* aDB, std::vector<Particle>* aParticles);
    virtual ~ParticleDecayer() {}

    int  DecayParticle(Particle* aFather);
    void SeedSet(int aSeed);

  private:
    inline double BreitWigner(double ampss, double Gamamp) const;
    int TwoBodyDecay();
    int ThreeBodyDecay();

    std::vector<Particle>* mParticles;
    ParticleDB* 	 particleDB;
    ParticleType*	 mTypeF;
    Particle*		 mFather;
    const DecayChannel*	 mChannel;
};

#endif

/*! @file ParticleDecayer.h
 * @brief Definition of ParticleDecayer class. Performs a two or three body decay.
 */
/*! @class ParticleDecayer
 * @brief Performs a two or three body decay.
 *
 * @fn ParticleDecayer::ParticleDecayer()
 * @brief Default constructor.
 *
 * @fn ParticleDecayer::ParticleDecayer(ParticleDB* aDB, std::vector<Particle>* aParticles)
 * @brief Initiate ParticleDecayer with the pointer to the Particle list and ParticleType database.
 * @param [in] aDB ParticleType database
 * @param [in] aParticles list type container with Particle objects
 *
 * @fn ParticleDecayer::~ParticleDecayer()
 * @brief Destructor
 *
 * @fn int ParticleDecayer::DecayParticle(Particle* aFather)
 * @brief Decays a Particle randomly through a two or three body DecayChannel.
 * @param [in] aFather father Particle
 *
 * @fn void ParticleDecayer::Randomize()
 * @brief sets the random number generator seed to system time.
 *
 * @fn void ParticleDecayer::SeedSet(int aSeed)
 * @brief sets the random number generator seed to a given value.
 * @param [in] aSeed generator seed
 */
