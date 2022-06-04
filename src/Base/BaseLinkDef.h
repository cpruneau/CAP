#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CanvasCollection+;
#pragma link C++ class CanvasConfiguration+;
#pragma link C++ class EnhancedGraph+;
#pragma link C++ class DataGraph+;
//#pragma link C++ class Factory+;
#pragma link C++ class Factory<Particle>+;
#pragma link C++ class Factory<ParticleDigit>+;
#pragma link C++ class Timer+;
//#pragma link C++ class Filter+;
#pragma link C++ class Filter<Event>+;
#pragma link C++ class Filter<Particle>+;

#pragma link C++ class Configuration+;
//#pragma link C++ class Collection+;
#pragma link C++ class Collection<TF1>+;
#pragma link C++ class Collection<TH1>+;
#pragma link C++ class Collection<TCanvas>+;
#pragma link C++ class Collection<ParticleType>+;
#pragma link C++ class FunctionCollection+;
#pragma link C++ class GraphConfiguration+;
#pragma link C++ class HistogramCollection+;
#pragma link C++ class Histograms+;
#pragma link C++ class Plotter+;
#pragma link C++ class MomentumGenerator+;
#pragma link C++ class RandomGenerator+;
#pragma link C++ class SelectionGenerator+;
#pragma link C++ class ScalarIntRandomGenerator+;
#pragma link C++ class FixedScalarIntRandomGenerator+;
#pragma link C++ class ExpScalarIntRandomGenerator+;
#pragma link C++ class ScalarDoubleRandomGenerator+;
#pragma link C++ class FixedScalarDoubleRandomGenerator+;
#pragma link C++ class LinearScalarDoubleRandomGenerator+;
#pragma link C++ class ExpScalarDoubleRandomGenerator+;
#pragma link C++ class HistoScalarDoubleRandomGenerator+;
#pragma link C++ class VectorRandomGenerator+;
#pragma link C++ class Task+;
#pragma link C++ class DerivedHistogramCalculator+;
#pragma link C++ class RootTreeReader+;
#pragma link C++ class TaskIterator+;
#pragma link C++ class DerivedHistoIterator+;
#pragma link C++ class MessageLogger+;

//// #pragma link C++ class AnalysisConfiguration+;
#pragma link C++ class Event+;
#pragma link C++ class EventProperties+;
#pragma link C++ class EventCountHistos+;
#pragma link C++ class EventFilter+;
#pragma link C++ class Particle+;
#pragma link C++ class ParticleDecayMode+;
#pragma link C++ class ParticleDecayer+;
#pragma link C++ class ParticleDecayerTask+;
#pragma link C++ class ParticleDigit+;

#pragma link C++ class ParticleType+;
#pragma link C++ class ParticleTypeCollection+;
#pragma link C++ class ParticleTypeTableLoader+;
#pragma link C++ class ParticleFilter+;
#pragma link C++ class ParticlePairFilter+;


#pragma link C++ class  Nucleus+;
#pragma link C++ class  NucleusType+;

//#pragma link C++ class  BidimGaussFitResult+;
//#pragma link C++ class  BidimGaussFitConfiguration+;
//#pragma link C++ class  BidimGaussFitter+;
#endif
