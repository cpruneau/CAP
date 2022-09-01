#include <TString.h>

#include<iostream>
#include<TObjString.h>
#include "UrQMDEvent.h"


ClassImp(UrQMDEvent)
ClassImp(UrQMDParticle)

using std::cout;
using std::endl;

////////////////////////////////////////////////////////////////////////////////
/// Create an Event object.
/// When the constructor is invoked for the first time, the class static
/// variable fgTracks is 0 and the TClonesArray fgTracks is created.

UrQMDEvent::UrQMDEvent() :
fEvtNum(-1),
fb(-1.0),
fTracks(NULL)
{
  fTracks = new TClonesArray("UrQMDParticle", 1000);
}

////////////////////////////////////////////////////////////////////////////////

UrQMDEvent::~UrQMDEvent()
{
   Clear();
   Reset();
}

////////////////////////////////////////////////////////////////////////////////

void UrQMDEvent::Build(Int_t evtno, Float_t b) {

  fEvtNum = evtno;
  fb = b;

  //Save current Object count
  Int_t ObjectNumber = TProcessID::GetObjectCount();

  Clear();

  //Restore Object count
  //To save space in the table keeping track of all referenced objects
  //we assume that our events do not address each other. We reset the
  //object count to what it was at the beginning of the event.
  TProcessID::SetObjectCount(ObjectNumber);
}

////////////////////////////////////////////////////////////////////////////////
/// Add a new particle to the list of tracks for this event.
void UrQMDEvent::AddParticle(TObjArray *values)
{
  //Save current Object count
  Int_t ObjectNumber = TProcessID::GetObjectCount();

  new ((*fTracks)[fTracks->GetEntriesFast()]) UrQMDParticle(values);
  //To save space in the table keeping track of all referenced objects
  //we assume that our events do not address each other. We reset the
  //object count to what it was at the beginning of the event.
  TProcessID::SetObjectCount(ObjectNumber);
}

////////////////////////////////////////////////////////////////////////////////

void UrQMDEvent::Clear(Option_t * /*option*/)
{
   fTracks->Clear("C"); //will also call Track::Clear
}

////////////////////////////////////////////////////////////////////////////////
/// Static function to reset all static objects for this event
///   fgTracks->Delete(option);

void UrQMDEvent::Reset(Option_t * /*option*/)
{
   delete fTracks;
   fTracks = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// Create a track object.

UrQMDParticle::UrQMDParticle() : TObject(),
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


UrQMDParticle::UrQMDParticle(TObjArray *values) : TObject()
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


void UrQMDParticle::Clear(Option_t * /*option*/)
{
   TObject::Clear();
}

////////////////////////////////////////////////////////////////////////////////
/// Set the values of the Track data members.

void UrQMDParticle::Set(TObjArray *values)
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

