#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CAP::Factory<CAP::Particle>+;
#pragma link C++ class CAP::Factory<CAP::ParticleDigit>+;
#pragma link C++ class CAP::Timer+;
//#pragma link C++ class Filter+;
#pragma link C++ class CAP::Filter<CAP::Event>+;
#pragma link C++ class CAP::Filter<CAP::Particle>+;

#pragma link C++ class CAP::Configuration+;
#pragma link C++ class CAP::Collection<TH1>+;
#pragma link C++ class CAP::Collection<CAP::ParticleType>+;
#pragma link C++ class CAP::HistogramCollection+;
#pragma link C++ class CAP::HistogramGroup+;
#pragma link C++ class CAP::MomentumGenerator+;
#pragma link C++ class CAP::RandomGenerator+;
#pragma link C++ class CAP::SelectionGenerator+;
#pragma link C++ class CAP::ScalarIntRandomGenerator+;
#pragma link C++ class CAP::FixedScalarIntRandomGenerator+;
#pragma link C++ class CAP::ExpScalarIntRandomGenerator+;
#pragma link C++ class CAP::ScalarDoubleRandomGenerator+;
#pragma link C++ class CAP::FixedScalarDoubleRandomGenerator+;
#pragma link C++ class CAP::LinearScalarDoubleRandomGenerator+;
#pragma link C++ class CAP::ExpScalarDoubleRandomGenerator+;
#pragma link C++ class CAP::HistoScalarDoubleRandomGenerator+;
#pragma link C++ class CAP::VectorRandomGenerator+;
#pragma link C++ class CAP::Task+;
#pragma link C++ class CAP::DerivedHistogramCalculator+;
#pragma link C++ class CAP::RootTreeReader+;
#pragma link C++ class CAP::TaskIterator+;
#pragma link C++ class CAP::DerivedHistoIterator+;
#pragma link C++ class CAP::MessageLogger+;
#pragma link C++ class CAP::StateManager+;

//// #pragma link C++ class AnalysisConfiguration+;
#pragma link C++ class CAP::Event+;
#pragma link C++ class CAP::EventProperties+;
#pragma link C++ class CAP::EventCountHistos+;
#pragma link C++ class CAP::EventFilter+;
#pragma link C++ class CAP::Particle+;
#pragma link C++ class CAP::ParticleDecayMode+;
#pragma link C++ class CAP::ParticleDecayer+;
#pragma link C++ class CAP::ParticleDecayerTask+;
#pragma link C++ class CAP::ParticleDigit+;

#pragma link C++ class CAP::ParticleType+;
#pragma link C++ class CAP::ParticleTypeCollection+;
#pragma link C++ class CAP::ParticleTypeTableLoader+;
#pragma link C++ class CAP::ParticleFilter+;
#pragma link C++ class CAP::ParticlePairFilter+;
#pragma link C++ class  CAP::Nucleus+;
#pragma link C++ class  CAP::NucleusType+;
#endif
