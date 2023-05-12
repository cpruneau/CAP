#ifndef URQMD_EVENT
#define URQMD_EVENT

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// UrQMD event                                                                //
//                                                                      //
// Description of the event and track parameters                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TClonesArray.h"
#include "TRefArray.h"
#include "TRef.h"
#include "TH1.h"
#include "TBits.h"
#include "TMath.h"

#define URQMDPARTICLE_ITYP_BITSHIFT 0
#define URQMDPARTICLE_ITYP_MASK 0x03FFFFFF
#define URQMDPARTICLE_ISO3_BITSHIFT 26
#define URQMDPARTICLE_ISO3_MASK 0x0000000F
#define URQMDPARTICLE_CHARGE_BITSHIFT 30
#define URQMDPARTICLE_CHARGE_MASK 0x00000003


class UrQMDParticle : public TObject {

private:

  Float_t      ft;            ///< time of the particle
  Float_t      fx;            ///< x coordinate
  Float_t      fy;            ///< y coordinate
  Float_t      fz;            ///< z coordinate
  Float_t      fE;            ///< energy of the particle
  Float_t      fPx;           ///< x component of the momentum
  Float_t      fPy;           ///< y component of the momentum
  Float_t      fPz;           ///< z component of the momentum
  Float_t      fMass;         ///< the mass of the particle
  UInt_t       fChIso3Ityp;   ///< the charge, twice the I3 isospin component and particle ID

public:
  UrQMDParticle();
  UrQMDParticle(TObjArray *values);
  virtual ~UrQMDParticle() {}

  void          Set(TObjArray *values);

  void          Clear(Option_t *option="");
  Float_t       GetPx() const { return fPx; }
  Float_t       GetPy() const { return fPy; }
  Float_t       GetPz() const { return fPz; }
  Float_t       GetPt() const { return TMath::Sqrt(fPx*fPx + fPy*fPy); }
  Float_t       GetTime() const { return ft; }
  Float_t       GetRx() const { return fx; }
  Float_t       GetRy() const { return fy; }
  Float_t       GetRz() const { return fz; }
  Float_t       GetMass() const { return fMass; }
  Float_t       GetE() const {return fE; }
  Int_t         GetCharge() const { return ((fChIso3Ityp >> URQMDPARTICLE_CHARGE_BITSHIFT) & URQMDPARTICLE_CHARGE_MASK) - 1; }
  Int_t         GetItyp() const { return ((fChIso3Ityp >> URQMDPARTICLE_ITYP_BITSHIFT) & URQMDPARTICLE_ITYP_MASK) - 9999999; }
  Int_t         Get2xI3() const { return ((fChIso3Ityp >> URQMDPARTICLE_ISO3_BITSHIFT) & URQMDPARTICLE_ISO3_MASK) - 7; }

private:
  UrQMDParticle(const UrQMDParticle& orig);
  UrQMDParticle &operator=(const UrQMDParticle &orig);

  ClassDef(UrQMDParticle,1)  //A track segment
};

class UrQMDEvent : public TObject {

private:
  Int_t                         fEvtNum;           ///< the event number
  Float_t                       fb;                ///< impact parameter
  /// the tracks array
  TClonesArray                 *fTracks;           //->

  UrQMDEvent(const UrQMDEvent&);
  UrQMDEvent &operator=(const UrQMDEvent&);

public:
  UrQMDEvent();
  virtual                      ~UrQMDEvent();
  void                          Build(Int_t evtno, Float_t b);
  void                          Clear(Option_t *option ="");
  void                          Reset(Option_t *option ="");
  void                          AddParticle(TObjArray *values);

  Int_t                         GetEventNumber() const { return fEvtNum; }
  Float_t                       GetImpactParameter() const { return fb; }
  Int_t                         GetNtrack() const { return fTracks->GetEntriesFast(); }
  TClonesArray                 *GetTracks() const {return fTracks;}

  ClassDef(UrQMDEvent,1)  //Event structure
};

#endif // #ifndef URQMD_EVENT
