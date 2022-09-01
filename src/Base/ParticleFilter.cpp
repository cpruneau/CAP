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


vector<ParticleFilter*> ParticleFilter::createOpenParticleFilter()
{
  vector<ParticleFilter*> filters;
  ParticleFilter * filter = new ParticleFilter();
  filter->setName("All");
  filter->setLongName("All");
  filter->setTitle("All");
  filter->setLongTitle("All");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filters.push_back(filter);
  return filters;
}

vector<ParticleFilter*> ParticleFilter::createAliceV0Filter()
{
  vector<ParticleFilter*> filters;
  ParticleFilter* filter  = new ParticleFilter();
  filter->setName("V0M");
  filter->setLongName("V0M");
  filter->setTitle("V0M");
  filter->setLongTitle("V0M");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(4, 1102, 0.0, 0.0);   // low mass charged hadron
  filter->addCondition(5, 1, 0.05,  1000.0);  // pT
  filter->addCondition(5, 7, -3.7, -1.7, 2.8, 5.1); // pseudorapidity in V0A and V0C
  filters.push_back(filter);
  return filters;
}

vector<ParticleFilter*> ParticleFilter::createNeutralParticleFilter(bool filteringOnPt,  double minPt,  double maxPt,
                                                                    bool filteringOnEta, double minEta, double maxEta,
                                                                    bool filteringOnY,   double minY,   double maxY)
{
  vector<ParticleFilter*> filters;
  ParticleFilter* filter  = new ParticleFilter();
  filter->setName("Neutral");
  filter->setLongName("Meutral");
  filter->setTitle("Q=0");
  filter->setLongTitle("Q=0");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(1, 0,  0.0, 0.0);   // neutral particles only
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);  return filters;
}

vector<ParticleFilter*> ParticleFilter::createChargedParticleFilter(bool filteringOnPt,  double minPt,  double maxPt,
                                                                     bool filteringOnEta, double minEta, double maxEta,
                                                                     bool filteringOnY,   double minY,   double maxY)
{
  vector<ParticleFilter*> filters;
  ParticleFilter* filter  = new ParticleFilter();
  filter->setName("Charged");
  filter->setLongName("Charged");
  filter->setTitle("Q!=0");
  filter->setLongTitle("Q!=0");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(1, 1,  0.0, 0.0);   // charged particles only
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);  return filters;
}

vector<ParticleFilter*> ParticleFilter::createNegativeParticleFilter(bool filteringOnPt,  double minPt,  double maxPt,
                                                                     bool filteringOnEta, double minEta, double maxEta,
                                                                     bool filteringOnY,   double minY,   double maxY)
{
  vector<ParticleFilter*> filters;
  ParticleFilter* filter  = new ParticleFilter();
  filter->setName("Negative");
  filter->setLongName("Negative");
  filter->setTitle("Q<0");
  filter->setLongTitle("Q<0");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(1, 2,  0.0, 0.0);   // negative particles only
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);  return filters;
}

vector<ParticleFilter*> ParticleFilter::createPositiveParticleFilter(bool filteringOnPt,  double minPt,  double maxPt,
                                                                     bool filteringOnEta, double minEta, double maxEta,
                                                                     bool filteringOnY,   double minY,   double maxY)
{
  vector<ParticleFilter*> filters;
  ParticleFilter* filter  = new ParticleFilter();
  filter->setName("Positive");
  filter->setLongName("Positive");
  filter->setTitle("Q>0");
  filter->setLongTitle("Q>0");
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(1, 3,  0.0, 0.0);   // negative particles only
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  filters.push_back(filter);  return filters;
}

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



ParticleFilter *  ParticleFilter::createHadronFilter(int pdg, const TString & name, const TString  & title,
                                                     bool filteringOnPt,  double minPt,  double maxPt,
                                                     bool filteringOnEta, double minEta, double maxEta,
                                                     bool filteringOnY,   double minY,   double maxY)
{
  ParticleFilter * filter = new ParticleFilter();
  filter->setName(name);
  filter->setLongName(name);
  filter->setTitle(title);
  filter->setLongTitle(title);
  filter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  filter->addCondition(2, 0,  double(pdg), double(pdg));   // positive low mass hadron
  if (filteringOnPt)   filter->addCondition(5, 1, minPt,  maxPt);
  if (filteringOnEta)  filter->addCondition(5, 7, minEta, maxEta);
  if (filteringOnY)    filter->addCondition(5, 8, minY,   maxY);
  return filter;
}

vector<ParticleFilter*> ParticleFilter::createBaryonFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                            bool filteringOnEta, double minEta, double maxEta,
                                                            bool filteringOnY,   double minY,   double maxY)
{
  vector<ParticleFilter*> filters;

  // proton  : 2212
  // neutron : 2112
  // lambda  : 3122
  // Sigma+  : 3222
  // Sigma0  : 3212
  // Sigma-  : 3112
  // Xi0     : 3322
  // XiM     : 3312
  // Omega-  : 3334
  // ===========================

  filters.push_back(createHadronFilter(2212,"P",       "p",          filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(2112,"N",       "n",          filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3122,"Lambda0", "#Lambda^{0}",filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3222,"SigmaP",  "#Sigma^{+}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3212,"Sigma0",  "#Sigma^{0}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3112,"SigmaM",  "#Sigma^{-}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3322,"Xi0",     "#Xi^{0}",    filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3312,"XiM",     "#Xi^{-}",    filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3334,"OmegaM",  "#Omega^{-}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));

  filters.push_back(createHadronFilter(-2212,"Pbar",       "#bar{p}",          filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-2112,"Nbar",       "#bar{n}",          filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3122,"Lambda0bar", "#bar{#Lambda}^{0}",filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3222,"SigmaPbar",  "#bar{#Sigma}^{-}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3212,"Sigma0bar",  "#bar{#Sigma}^{0}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3112,"SigmaMbar",  "#bar{#Sigma}^{+}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3322,"Xi0bar",     "#bar{#Xi}^{0}",    filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3312,"XiMbar",     "#bar{#Xi}^{+}",    filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3334,"OmegaMbar",  "#bar{#Omega}^{+}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));

  return filters;
}



vector<ParticleFilter*> ParticleFilter::createStrangeHadronFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                   bool filteringOnEta, double minEta, double maxEta,
                                                   bool filteringOnY,   double minY,   double maxY)
{
  vector<ParticleFilter*> filters;

  // lambda  : 3122
  // Sigma+  : 3222
  // Sigma0  : 3212
  // Sigma-  : 3112
  // Xi0     : 3322
  // XiM     : 3312
  // Omega-  : 3334
  // ===========================

  filters.push_back(createHadronFilter(321, "KP",      "K^{+}",      filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3122,"Lambda0", "#Lambda^{0}",filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3222,"SigmaP",  "#Sigma^{+}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3212,"Sigma0",  "#Sigma^{0}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3112,"SigmaM",  "#Sigma^{-}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3322,"Xi0",     "#Xi^{0}",    filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3312,"XiM",     "#Xi^{-}",    filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(3334,"OmegaM",  "#Omega^{-}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));

  filters.push_back(createHadronFilter(-321, "KM",         "K^{-}",            filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3122,"Lambda0bar", "#bar{#Lambda}^{0}",filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3222,"SigmaPbar",  "#bar{#Sigma}^{-}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3212,"Sigma0bar",  "#bar{#Sigma}^{0}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3112,"SigmaMbar",  "#bar{#Sigma}^{+}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3322,"Xi0bar",     "#bar{#Xi}^{0}",    filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3312,"XiMbar",     "#bar{#Xi}^{+}",    filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));
  filters.push_back(createHadronFilter(-3334,"OmegaMbar",  "#bar{#Omega}^{+}", filteringOnPt,minPt,maxPt,filteringOnEta,minEta,maxEta,filteringOnY,minY,maxY));

  return filters;
}


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
    int charge = type.getCharge();
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

        case 1: // Charge, Neutral, Plus, or Minus
        switch (filterSubType)
          {
            case  0: accepting = (charge==0); break;  // accepts neutral only
            case  1: accepting = (charge!=0); break;  // accepts all charged particles
            case  2: accepting = (charge<0);  break;  // accepts -ve only
            case  3: accepting = (charge>0);  break;  // accepts +ve only
          }
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
