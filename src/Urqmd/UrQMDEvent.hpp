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
#include "TObjString.h"
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
  ////////////////////////////////////////////////////////////////////////////////
  /// Create a track object.

  UrQMDParticle() : TObject(),
  ft(0.0),
  fx(0.0),
  fy(0.0),
  fz(0.0),
  fE(0.0),
  fPx(0.0),
  fPy(0.0),
  fPz(0.0),
  fMass(0.0),
  fChIso3Ityp(0)
  {

  }


  ////////////////////////////////////////////////////////////////////////////////
  /// Destroy it

  virtual ~UrQMDParticle() {}


  UrQMDParticle(TObjArray *values) : TObject()
  {
    Int_t ityp = 0;
    Int_t twoI3 = 0;
    Int_t charge = 0;
    Bool_t next = kTRUE;

    for (Int_t tok = 0; tok < values->GetEntriesFast(); tok++) {
      switch (tok) {
      case 0:
        /* t */
        ft = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 1:
        /* x */
        fx = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 2:
        /* y */
        fy = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 3:
        /* z */
        fz = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 4:
        /* E, ie, p0 */
        fE = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 5:
        /* px */
        fPx = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 6:
        /* py */
        fPy = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 7:
        /* pz */
        fPz = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 8:
        /* m */
        fMass = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 9:
        /* ityp, particle ID, or kind of */
        ityp = (((TObjString *) values->At(tok))->String()).Atoi();
        break;
      case 10:
        /* 2i3, twice the isospin third component */
        twoI3 = (((TObjString *) values->At(tok))->String()).Atoi();
        break;
      case 11:
        /* charge */
        charge = (((TObjString *) values->At(tok))->String()).Atoi();
        break;
      default:
        next = kFALSE;
        break;
      }

      if (next)
        continue;
      else {
        /* let's store the complex stuff */
        fChIso3Ityp = (((charge + 1) & URQMDPARTICLE_CHARGE_MASK) << URQMDPARTICLE_CHARGE_BITSHIFT) |
            (((twoI3 + 7) & URQMDPARTICLE_ISO3_MASK) << URQMDPARTICLE_ISO3_BITSHIFT ) |
            (((ityp + 9999999) & URQMDPARTICLE_ITYP_MASK) << URQMDPARTICLE_ITYP_BITSHIFT);
        break;
      }
    }
  }


  void Clear(Option_t * /*option*/)
  {
     TObject::Clear();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Set the values of the Track data members.

  void Set(TObjArray *values)
  {
    Int_t ityp = 0;
    Int_t twoI3 = 0;
    Int_t charge = 0;
    Bool_t next = kTRUE;

    for (Int_t tok = 0; tok < values->GetEntriesFast(); tok++) {
      switch (tok) {
      case 0:
        /* t */
        ft = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 1:
        /* x */
        fx = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 2:
        /* y */
        fy = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 3:
        /* z */
        fz = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 4:
        /* E, ie, p0 */
        fE = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 5:
        /* px */
        fPx = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 6:
        /* py */
        fPy = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 7:
        /* pz */
        fPz = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 8:
        /* m */
        fMass = (((TObjString *) values->At(tok))->String()).Atof();
        break;
      case 9:
        /* ityp, particle ID, or kind of */
        ityp = (((TObjString *) values->At(tok))->String()).Atoi();
        break;
      case 10:
        /* 2i3, twice the isospin third component */
        twoI3 = (((TObjString *) values->At(tok))->String()).Atoi();
        break;
      case 11:
        /* charge */
        charge = (((TObjString *) values->At(tok))->String()).Atoi();
        break;
      default:
        next = kFALSE;
        break;
      }

      if (next)
        continue;
      else {
        /* let's store the complex stuff */
        fChIso3Ityp = (((charge + 1) & URQMDPARTICLE_CHARGE_MASK) << URQMDPARTICLE_CHARGE_BITSHIFT) |
            (((twoI3 + 7) & URQMDPARTICLE_ISO3_MASK) << URQMDPARTICLE_ISO3_BITSHIFT ) |
            (((ityp + 9999999) & URQMDPARTICLE_ITYP_MASK) << URQMDPARTICLE_ITYP_BITSHIFT);
        break;
      }
    }
  }


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
  ////////////////////////////////////////////////////////////////////////////////
  /// Create an Event from I/O.
  UrQMDEvent() :
  fEvtNum(-1),
  fb(-1.0),
  fTracks(NULL)
  {
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Create an Event object.
  /// When the constructor is invoked for the first time, the class static
  /// variable fgTracks is 0 and the TClonesArray fgTracks is created.

  UrQMDEvent(const char *) :
  fEvtNum(-1),
  fb(-1.0),
  fTracks(NULL)
  {
    fTracks = new TClonesArray("UrQMDParticle", 1000);
  }

  ////////////////////////////////////////////////////////////////////////////////

  virtual ~UrQMDEvent()
  {
     Clear("");
     Reset("");
  }

  ////////////////////////////////////////////////////////////////////////////////

  void Build(Int_t evtno, Float_t b) {

    fEvtNum = evtno;
    fb = b;

    //Save current Object count
    Int_t ObjectNumber = TProcessID::GetObjectCount();

    Clear("");

    //Restore Object count
    //To save space in the table keeping track of all referenced objects
    //we assume that our events do not address each other. We reset the
    //object count to what it was at the beginning of the event.
    TProcessID::SetObjectCount(ObjectNumber);
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Add a new particle to the list of tracks for this event.
  void AddParticle(TObjArray *values)
  {
    //Save current Object count
    Int_t ObjectNumber = TProcessID::GetObjectCount();

    new ((*fTracks)[fTracks->GetEntriesFast()]) UrQMDParticle(values);
    //To save space in the table keeping track of all referenced objects
    //we assume that our events do not address each other. We reset the
    //object count to what it was at the beginning of the event.
    TProcessID::SetObjectCount(ObjectNumber);
  }

  Int_t                         GetEventNumber() const { return fEvtNum; }
  Float_t                       GetImpactParameter() const { return fb; }
  Int_t                         GetNtrack() const { return fTracks->GetEntriesFast(); }
  TClonesArray                 *GetTracks() const {return fTracks;}

  ////////////////////////////////////////////////////////////////////////////////

  void Clear(Option_t * /*option*/)
  {
     fTracks->Clear("C"); //will also call Track::Clear
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Static function to reset all static objects for this event
  ///   fgTracks->Delete(option);

  void Reset(Option_t * /*option*/)
  {
     delete fTracks;
     fTracks = NULL;
  }

  ClassDef(UrQMDEvent,1)  //Event structure
};

#endif // #ifndef URQMD_EVENT
