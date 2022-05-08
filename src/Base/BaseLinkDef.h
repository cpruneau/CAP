#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CanvasCollection+;
#pragma link C++ class CanvasConfiguration+;
#pragma link C++ class EnhancedGraph+;
#pragma link C++ class DataGraph+;
#pragma link C++ class Factory+;
#pragma link C++ class Factory<Particle>+;
#pragma link C++ class Factory<ParticleDigit>+;
#pragma link C++ class Timer+;
#pragma link C++ class Filter+;
#pragma link C++ class Filter<Event>+;
#pragma link C++ class Filter<Particle>+;

#pragma link C++ class Configuration+;
#pragma link C++ class Collection+;
#pragma link C++ class Collection<TF1>+;
#pragma link C++ class Collection<TH1>+;
#pragma link C++ class Collection<TCanvas>+;
#pragma link C++ class Collection<ParticleType>+;
#pragma link C++ class FunctionCollection+;
#pragma link C++ class GraphConfiguration+;
#pragma link C++ class HistogramCollection+;
#pragma link C++ class Histograms+;
#pragma link C++ class Plotter+;
#pragma link C++ class StatStudyHistograms+;
#pragma link C++ class StatStudyPlotter+;
#pragma link C++ class StatStudyModel+;

#pragma link C++ class TotalXsection+;

//#pragma link C++ class IMPairHistos+;
//#pragma link C++ class IMParticleHistos+;
//#pragma link C++ class MasterAnalysis+;
//#pragma link C++ class MultHisto+;
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
#pragma link C++ class RapidityGenerator+;
#pragma link C++ class Task+;
#pragma link C++ class RootTreeReader+;
#pragma link C++ class TaskIterator+;
#pragma link C++ class FileTaskIterator+;
#pragma link C++ class MessageLogger+;

//// #pragma link C++ class AnalysisConfiguration+;
#pragma link C++ class Event+;
#pragma link C++ class EventProperties+;
#pragma link C++ class EventCountHistos+;
#pragma link C++ class EventFilter+;
///#pragma link C++ class EventFilterAliceV0+;
#pragma link C++ class EventPlaneRandomizerTask+;
#pragma link C++ class EventVertexRandomizerTask+;
#pragma link C++ class Particle+;
#pragma link C++ class ParticleDecayMode+;
#pragma link C++ class ParticleDecayer+;
#pragma link C++ class ParticleDecayerTask+;
#pragma link C++ class ParticleDigit+;

#pragma link C++ class ParticleType+;
#pragma link C++ class ParticleTypeCollection+;
#pragma link C++ class ParticleTypeTableLoader+;
#pragma link C++ class ParticleFilter+;
#pragma link C++ class ParticleFilterEfficiency+;
///#pragma link C++ class ParticleFilterAliceV0+;
#pragma link C++ class ParticlePairFilter+;
#pragma link C++ class ParticleSmearer+;

#pragma link C++ class ParticleHistos+;
#pragma link C++ class ParticleDerivedHistos+;
#pragma link C++ class ParticleAnalyzer+;
#pragma link C++ class ParticleDerivedHistogramCalculator+;

#pragma link C++ class ParticlePairHistos+;
#pragma link C++ class ParticlePairDerivedHistos+;
#pragma link C++ class ParticlePairCombinedHistos+;
#pragma link C++ class ParticlePairAnalyzer+;
#pragma link C++ class ParticlePairDerivedHistogramCalculator+;
#pragma link C++ class BalanceFunctionCalculator+;

#pragma link C++ class ResonanceGenerator+;

#pragma link C++ class NucleonNucleonCollisionGenerator+;
#pragma link C++ class AACollisionGenerator+;
#pragma link C++ class RadialBoostHistos+;
#pragma link C++ class RadialBoostTask+;
#pragma link C++ class MomentumGenerator+;
#pragma link C++ class DetectorFastSimulatorTask;

#pragma link C++ class NuDynAnalyzer+;
#pragma link C++ class NuDynHistos+;
#pragma link C++ class NuDynDerivedHistos+;
#pragma link C++ class NuDynDerivedHistogramCalculator+;
#pragma link C++ class NuDynPlotter+;
#pragma link C++ class TransverseSpherocityHistos+;
#pragma link C++ class TransverseSpherocityAnalyzer+;
#pragma link C++ class SubSampleStatCalculator+;
#pragma link C++ class TrackAndPairEffects+;
#pragma link C++ class GeneralizedGaussianProfile+;
#pragma link C++ class  CollisionGeometry+;
#pragma link C++ class  CollisionGeometryAnalyzer+;
#pragma link C++ class  CollisionGeometryGenerator+;
#pragma link C++ class  CollisionGeometryHistograms+;
#pragma link C++ class  CollisionGeometryGradientHistograms+;
#pragma link C++ class  CollisionGeometryMoments+;
#pragma link C++ class  Nucleus+;
#pragma link C++ class  NucleusGenerator+;

#pragma link C++ class  GlobalAnalyzer+;
#pragma link C++ class  GlobalHistos+;
#pragma link C++ class  GlobalAnalyzerConfiguration+;

#pragma link C++ class  BidimGaussFitResult+;
#pragma link C++ class  BidimGaussFitConfiguration+;
#pragma link C++ class  BidimGaussFitter+;

#pragma link C++ class  PartitionCalculator+;
#endif
