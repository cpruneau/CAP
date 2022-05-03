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
#include <TMath.h>
#include "ParticleFilter.hpp"

ClassImp(Filter<Particle>);

ClassImp(ParticleFilter);

ParticleFilter::ParticleFilter()
:
Filter<Particle>()
{
  // no ops
}

ParticleFilter::ParticleFilter(const ParticleFilter & otherFilter)
:
Filter<Particle>(otherFilter)
{
 // no ops
}

ParticleFilter & ParticleFilter::operator=(const ParticleFilter & otherFilter)
{
  if (this!=&otherFilter)
    {
    Filter<Particle>::operator=(otherFilter);
    }
  return *this;
}


//! Produce 6 particle filters:  pi+, K+, p, pi-, K-, pbar
vector<ParticleFilter*> ParticleFilter::createPlusMinusHadronFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                                     bool filteringOnEta, double minEta, double maxEta,
                                                                     bool filteringOnY,   double minY,   double maxY)
{
  vector<ParticleFilter*> filters;

  ParticleFilter * filter;

  filter = new ParticleFilter();
  filter->setName("PiP");
  filter->setLongName("PiP");
  filter->setTitle("#pi^{+}");
  filter->setLongTitle("#pi^{+}");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1111, 0.0, 0.0);  // pi+
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);

  filter = new ParticleFilter();
  filter->setName("KP");
  filter->setLongName("KP");
  filter->setTitle("K^{+}");
  filter->setLongTitle("K^{+}");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1121, 0.0, 0.0);  // K+
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);

  filter = new ParticleFilter();
  filter->setName("PP");
  filter->setLongName("PP");
  filter->setTitle("p");
  filter->setLongTitle("p");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1211, 0.0, 0.0);  // p+
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);

  filter = new ParticleFilter();
  filter->setName("PiM");
  filter->setLongName("PiM");
  filter->setTitle("#pi^{-}");
  filter->setLongTitle("#pi^{-}");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1113, 0.0, 0.0);  // pi-
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);

  filter = new ParticleFilter();
  filter->setName("KM");
  filter->setLongName("KM");
  filter->setTitle("K^{-}");
  filter->setLongTitle("K^{-}");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1122, 0.0, 0.0);  // K-
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);

  filter = new ParticleFilter();
  filter->setName("PM");
  filter->setLongName("PM");
  filter->setTitle("#bar{p}");
  filter->setLongTitle("#bar{p}");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1212, 0.0, 0.0);  // pi+
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);
  return filters;
}

//!
//!  Create + and - charge filters with the given kinematical parameters
//!
vector<ParticleFilter*> ParticleFilter::createChargedHadronFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                                   bool filteringOnEta, double minEta, double maxEta,
                                                                   bool filteringOnY,   double minY,   double maxY)
{
  vector<ParticleFilter*> filters;

  ParticleFilter * filter;

  filter = new ParticleFilter();
  filter->setName("HP");
  filter->setLongName("HP");
  filter->setTitle("h^{+}");
  filter->setLongTitle("h^{+}");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1103, 0.0, 0.0);   // positive low mass hadron
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);

  filter = new ParticleFilter();
  filter->setName("HM");
  filter->setLongName("HM");
  filter->setTitle("h^{-}");
  filter->setLongTitle("h^{+}");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1104, 0.0, 0.0);   // negative low mass hadron
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);
  return filters;
}

//
//vector<ParticleFilter*> ParticleFilter::createMultiplePdgFilters(bool filteringOnPt,  double minPt,  double maxPt,
//                                                                 bool filteringOnEta, double minEta, double maxEta,
//                                                                 bool filteringOnY,   double minY,   double maxY,
//                                                                 vector<int> & pdgAccepted)
//{
//  vector<ParticleFilter*> filters;
//  vector<int> pdgs;
//  for (unsigned int k=0;k<pdgAccepted.size();k++)
//    {
//    pdgs.clear();
//    pdgs.push_back( pdgAccepted[k] );
//    filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, pdgs, ParticleFilter::Live) );
//    }
//  return filters;
//}
//
//vector<ParticleFilter*> ParticleFilter::createBaryonFilters(bool filteringOnPt,  double minPt,  double maxPt,
//                                                            bool filteringOnEta, double minEta, double maxEta,
//                                                            bool filteringOnY,   double minY,   double maxY)
//{
//  vector<ParticleFilter*> filters;
//  filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, ParticleFilter::Proton,      ParticleFilter::Live) );
//  filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, ParticleFilter::ProtonBar,   ParticleFilter::Live) );
//  filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, ParticleFilter::Lambda,      ParticleFilter::Live) );
//  filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, ParticleFilter::LambdaBar,   ParticleFilter::Live) );
//  filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, ParticleFilter::XiM,         ParticleFilter::Live) );
//  filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, ParticleFilter::XiMBar,      ParticleFilter::Live) );
//  filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, ParticleFilter::OmegaM,      ParticleFilter::Live) );
//  filters.push_back( new ParticleFilter(filteringOnPt,minPt, maxPt, filteringOnEta,minEta, maxEta, filteringOnY, minY, maxY, ParticleFilter::OmegaMBar,   ParticleFilter::Live) );
//  return filters;
//}
//

//!
//! accept/reject the given particle based on filter parameter
//!
bool ParticleFilter::accept(const Particle & particle)
{
  unsigned int nConditions = getNConditions();
  if (nConditions<1) return true;
  double value;
  bool   accepting;
  ParticleType & type = particle.getType();

  for (unsigned int k = 0; k<nConditions; k++)
    {
    Condition & condition = *(conditions[k]);
    unsigned int filterType    = condition.filterType;
    unsigned int filterSubType = condition.filterSubtype;
    switch (filterType)
      {
        case 0: // live or not to be considered at all
        switch (filterSubType)
          {
            case  0: accepting = !particle.isLive(); break;  // decayed or removed particles only
            case  1: accepting = particle.isLive(); break;   // undecayed particles only
            case  2: accepting = 1; break;                   // all
          }
        break;

        case 1: // type selection/filtering
        accepting = condition.accept(double(type.getCharge()));
        break;

        case 2: // PDG Code
        accepting = condition.accept(double(type.getPdgCode()));
        break;

        //case 3: // User Code
        //accepting = condition.accept(double(type.getUserPid()));
        //break;

        case 4: // Type selection
        switch (filterSubType)
          {
            case 0: accepting = type.isPhoton();     break;

            case 10: accepting = type.isLepton();         break;
            case 11: accepting = type.isElectron() || type.isPositron(); break;
            case 12: accepting = type.isElectron();   break;
            case 13: accepting = type.isPositron();   break;
            case 14: accepting = type.isMuon() || type.isAntiMuon();     break;
            case 15: accepting = type.isMuon();       break;
            case 16: accepting = type.isAntiMuon();   break;
            case 17: accepting = type.isTau()  || type.isAntiTau();      break;
            case 18: accepting = type.isTau();       break;
            case 19: accepting = type.isAntiTau();  break;

            case 40: accepting = type.isStrange();        break;
            case 41: accepting = type.isStrangePlus();    break;
            case 42: accepting = type.isStrangeMinus();   break;

            case 50: accepting = type.isCharm();          break;
            case 51: accepting = type.isCharmPlus();      break;
            case 52: accepting = type.isCharmMinus();     break;

            case 60: accepting = type.isBottom();         break;
            case 61: accepting = type.isBottomPlus();     break;
            case 62: accepting = type.isBottomMinus();    break;

            case 70: accepting = type.isTop();            break;

            case 1100: accepting = type.isHadron();   break; // any hadron
            case 1101: accepting = type.isHadron() && type.isCharged();   break; // any charged hadron
            case 1102: accepting =
            type.isPionP() ||
            type.isPionM() ||
            type.isKaonP() ||
            type.isKaonM() ||
            type.isProton() ||
            type.isAntiProton(); // pi+, pi-, K+, K-, p, pBar
            break; // any charge hadron
            case 1103: accepting =
            type.isPionP() ||
            type.isKaonP() ||
            type.isProton(); // pi+, K+, p
            break;
            case 1104: accepting =
            type.isPionM() ||
            type.isKaonM() ||
            type.isAntiProton(); // pi-, K-, pBar
            break;

            case 1110: accepting = type.isPion();     break;
            case 1111: accepting = type.isPionP(); break;
            case 1112: accepting = type.isPion0(); break;
            case 1113: accepting = type.isPionM(); break;

            case 1120: accepting = type.isKaon();     break;
            case 1121: accepting = type.isKaonP();    break;
            case 1122: accepting = type.isKaonM();    break;
            case 1123: accepting = type.isKaon0();    break;
            case 1124: accepting = type.isKaon0Bar(); break;
            case 1125: accepting = type.isKaon0S();   break;
            case 1126: accepting = type.isKaon0L();   break;

            case 1200: accepting = type.isBaryon();         break;
            case 1201: accepting = type.isBaryonPlus();     break;
            case 1202: accepting = type.isBaryonMinus();    break;

            case 1210: accepting = type.isProton() || type.isAntiProton();   break;
            case 1211: accepting = type.isProton();     break;
            case 1212: accepting = type.isAntiProton(); break;

            case 1220: accepting = type.isLambda() || type.isAntiLambda();   break;
            case 1221: accepting = type.isLambda();     break;
            case 1222: accepting = type.isAntiLambda(); break;

            case 1230: accepting = type.isSigmaP();       break;
            case 1231: accepting = type.isSigma0();       break;
            case 1232: accepting = type.isSigmaM();       break;
            case 1241: accepting = type.isAntiSigmaP();   break;
            case 1242: accepting = type.isAntiSigma0();   break;
            case 1243: accepting = type.isAntiSigmaM();   break;
            case 1244: accepting = type.isXi0();          break;
            case 1245: accepting = type.isXiM();          break;
            case 1246: accepting = type.isAntiXi0();      break;
            case 1247: accepting = type.isAntiXiM();      break;
            case 1248: accepting = type.isOmegaM();       break;
            case 1249: accepting = type.isAntiOmegaM();   break;
          }
        break;

        case 5: // kinematic selection/filtering
        const TLorentzVector & momentum = particle.getMomentum();
        switch (filterSubType)
          {
            case 0: value = momentum.P(); break;    // momentum
            case 1: value = momentum.Pt(); break;   // transverse momentum
            case 2: value = momentum.E(); break;    // energy
            case 3: value = momentum.Px(); break;   // p_x
            case 4: value = momentum.Py(); break;   // p_y
            case 5: value = momentum.Pz(); break;   // p_z
            case 6: value = momentum.Phi(); break;   // phi azimuth
            case 7: value = momentum.Eta(); break;   // pseudo rapidity
            case 8: value = momentum.Rapidity(); break;   // rapidity
          }
        accepting = conditions[k]->accept(value);
        break;
      }
    if (!accepting)  return false;
    }
  return true;
}


