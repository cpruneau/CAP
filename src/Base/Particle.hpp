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

#ifndef CAP__Particle
#define CAP__Particle
#include "TLorentzVector.h"
#include "Factory.hpp"
#include "ParticleType.hpp"
#include "ParticleTypeCollection.hpp"

using namespace std;


// ========================================================
// Class Particle
// ========================================================
class Particle
{
public:
  
  Particle();
  virtual ~Particle();
  Particle(const Particle& other);
  Particle & operator=(const Particle & other);
  void printProperties(ostream & output);

  void clear();
  void reset();

  void setType(ParticleType * _type) { type = _type;   }
  ParticleType & getType() const     { return *type;   }

  void set(ParticleType * type,
           double p_x, double p_y, double p_z, double p_e,
           double x,   double y,   double z,   double t,
           bool live=true);

  void set(ParticleType * type,
           TLorentzVector & momentum,
           TLorentzVector & position,
           bool live=true);

  void set(ParticleType * type,
           TLorentzVector & momentum,
           vector<Particle*> parents,
           bool live=true);

  void setPidPxPyPzE(long _Pid, double p_x, double p_y, double p_z, double p_e);


  const TLorentzVector & getMomentum() const                  { return momentum;  }
  TLorentzVector & getMomentum()                        { return momentum;  }
  void setPxPyPzE(double p_x, double p_y, double p_z, double p_e)
  {  momentum.SetPxPyPzE(p_x,p_y,p_z,p_e);  }
  void setPxPyPzE(TLorentzVector & _momentum)           { momentum = _momentum;  }
  void setMomentum(TLorentzVector & _momentum)          { momentum = _momentum;  }

  TLorentzVector & getPosition()     { return position;  }
  const TLorentzVector & getPosition() const    { return position;  }
  void setXYZT(double x, double y, double z, double t)  { position.SetPxPyPzE(x,y,z,t); }
  void setXYZT(const TLorentzVector & _position)        { position = _position; }
  void setPosition(const TLorentzVector & _position)    { position = _position; }
  void setRThetaPhiT(double _r, double _theta, double _phi,double _t);
  void setRCosThetaPhiT(double _r, double _cosTheta, double _phi,double _t);
  void shift(const TLorentzVector & dX)                 { position += dX;}
  void shift(double dx, double dy, double dz, double dt)
  {
  TLorentzVector dX(dx,dy,dz,dt);
  position += dX;
  }


  void setPid(long _pid)             { pid = _pid;        }
  long getPid() const                { return pid;        }
  void setDecayed(bool decayed)      { live = !decayed;   }
  bool isDecayed() const             { return !live;      }
  bool isStable() const              { if (type) return type->isStable(); else return true; }
  void setWounded(bool wounded)      { live = !wounded;   }
  void setInteracted(bool interacted){ live = !interacted;}
  bool isWounded() const             { return !live;      }
  bool hasInteracted() const         { return !live;      }
  bool isLive() const                { return live;       }
  void setLive(bool _live)           { live =_live;       }


  void setSourceIndex(int _sourceIndex) { sourceIndex = _sourceIndex; }
  int  getSourceIndex()  const       { return sourceIndex; }

  void boost(double ax, double ay, double az);
  void boostRapidity(double boost);

  // set Particle Interaction (wounded) status

  virtual double distanceXYZSq(Particle * otherParticle) const;
  virtual double distanceXYZ(Particle * otherParticle) const;
  virtual double distanceXYSq(Particle * otherParticle) const;
  virtual double distanceXY(Particle * otherParticle) const;

  bool isPrimary() const;
  bool isSecondary() const;
  bool isParticle() const;
  bool isDecay()  const          { return type->isDecay();          }
  bool isInteraction()  const    { return type->isInteraction();    }
  bool isPPInteraction() const   { return type->isPPInteraction();  }
  bool isPNInteraction() const   { return type->isPNInteraction();  }
  bool isNNInteraction() const   { return type->isNNInteraction();  }
  bool isNucleon()  const        { return type->isNucleon();        }
  bool isProton()  const         { return type->isProton();         }
  bool isNeutron()  const        { return type->isNeutron();        }
  bool isPhoton()  const         { return type->isPhoton();         }

  bool hasParents() const
  {
  return parents.size()>0;
  }

  TString getName()
  {
  if (type)
    return type->getName();
  else
    return "UnknownType";
  }

  unsigned int getNParents() const
  {
  return parents.size();
  }

  Particle * getParentAt(unsigned int index)
  {
  if (index < parents.size())
    return parents[index];
  else
    return nullptr;
  }

  const vector<Particle*> & getParents() const
  {
    return parents;
  }

  // Particle Interaction 1->X
  void setParent(Particle * parent);
  // Particle Interaction 2->X
  void setParents(Particle * parent1, Particle * parent2);
  // Particle Interaction 3->X
  void setParents(Particle * parent1, Particle * parent2, Particle * parent3);
  // Particle Interaction n->X
  void setParents(const vector<Particle*> &  parents);

  bool hasChildren() const
  {
  return children.size()>0;
  }

  unsigned int getNChildren() const
  {
  return children.size();
  }

  Particle * getChildAt(unsigned int index)
  {
  if (index < children.size())
    return children[index];
  else
    return nullptr;
  }

  const vector<Particle*> & getChildren() const
  {
    return children;
  }

  void addChild(Particle* child);
  void addChildren(Particle* child1, Particle* child2);
  void addChildren(Particle* child1, Particle* child2, Particle* child3);
  void addChildren(const vector<Particle*> &  children);

  bool isNucleonNucleonInteraction() const;
  bool isProtonProton() const;
  bool isNeutronNeutron() const;
  bool isProtonNeutron() const;

  void setTruth(Particle * _truth)  { truth = _truth;         }
  Particle * getTruth() const       { return truth;           }
  bool hasTruth() const             { return truth!=nullptr;  }

  // ==================================================
  // Data Members
  // ==================================================
protected:
  TLorentzVector momentum;
  TLorentzVector position;
  ParticleType * type;
  vector<Particle*> parents;
  vector<Particle*> children;
  Particle * truth;
  bool live;
  long pid;  // used in some application
  int  sourceIndex;  // used in some application

public:
  int   ixEtaPhi, ixYPhi;

public:
  static int factorySize;
  static Factory<Particle> * factory;
  static Factory<Particle> * getFactory();
  static void resetFactory();

  static Particle * getProton();
  static Particle * getNeutron();

  ParticleType * protonType  = ParticleType::getProtonType();

  ClassDef(Particle,0)

};


#endif /* CAP__Particle */
