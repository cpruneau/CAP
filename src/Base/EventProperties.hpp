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

#ifndef EventProperties_hpp
#define EventProperties_hpp
#include <vector>
#include "Particle.hpp"
#include "ParticleType.hpp"

using std::vector;

namespace CAP
{



class EventProperties
{
public:

  EventProperties();
  EventProperties(const EventProperties & source);
  EventProperties & operator=(const EventProperties & source);
  virtual ~EventProperties() {}
  virtual void clear();
  virtual void reset();
  virtual void printProperties(ostream & output);

  virtual void fill(vector<double> & nFiltered,  // number of particles accepted by filter #i
                    vector<double> & ptSumFiltered,  // total pT of particles accepted by filter #i
                    vector<double> & eFiltered,  // total energy of particles accepted by filter #i
                    vector<double> & qFiltered,  // total charge of particles accepted by filter #i
                    vector<double> & sFiltered,  // total strangeness of particles accepted by filter #i
                    vector<double> & bFiltered);

  virtual void fillSpherocity(vector<double> & s0,
                              vector<double> & s1);
  
  // ================================================
  // Data Members
  // ================================================
  ParticleType  * projectileType;    //! type of the projectile
  ParticleType  * targetType;        //! type of the target
  unsigned int    zProjectile;       //!< atomic number projectile
  unsigned int    aProjectile;       //!< mass number projectile
  unsigned int    nPartProjectile;   //!< number of participants  projectile
  unsigned int    zTarget;           //!< atomic number target
  unsigned int    aTarget;           //!< mass number target
  unsigned int    nPartTarget;       //!< number of participants  target
  unsigned int    nParticipantsTotal; //!< total number of participants
  unsigned int    nBinaryTotal;       //!< total number of binary collisions
  double impactParameter;             //!< nucleus-nucleus center distance in fm
  double fractionalXSection;          //!< fraction cross section value
  double refMultiplicity;       //!< nominal multiplicity in the reference range
  double other;                       //!< other value of interest
  unsigned int    particlesCounted;   //!< number of particles provided internally by event generator/reader
  unsigned int    particlesAccepted;  //!< number of particles accepted by event generator/reader's particle filter

  // From Model
  vector<double> modelParameters;

  // From GlobalAnalyzer
  int    nFilters;
  vector<double> nFiltered;  // number of particles accepted by filter #i
  vector<double> ptSumFiltered;  // total pT of particles accepted by filter #i
  vector<double> eFiltered;  // total energy of particles accepted by filter #i
  vector<double> qFiltered;  // total charge of particles accepted by filter #i
  vector<double> sFiltered;  // total strangeness of particles accepted by filter #i
  vector<double> bFiltered;  // total baryon number of particles accepted by filter #i
  
  // From TransverseSpherocityAnalyzer
  vector<double> s0Filtered;  // transverse spherocity
  vector<double> s1Filtered;  // transverse spherocity w/ unig vector

  ClassDef(EventProperties,0)

};

} // namespace CAP

#endif /* EventProperties_hpp */
