//
//  BWModelEventGenerator.hpp
//  Created by Victor Gonzalez on 20190206 based in the work of Claude Pruneau and the ALICE AliPhysics SW framework
//  Copyright © 2016-2019 Claude Pruneau. All rights reserved.
//

#ifndef CAP_BWModelEventGenerator
#define CAP_BWModelEventGenerator
#include "Task.hpp"
#include "Configuration.hpp"

class BWModelEventGenerator : public Task
{
public:

  BWModelEventGenerator(const TString &       _name,
                        const Configuration & _configuration,
                        LogLevel              _selectedLevel);
  virtual ~BWModelEventGenerator();
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void reset();
  virtual void execute();

  static TF1 *GetExponentialSpectra_FN(const char *name, double mass, double temp, double xmin, double xmax);
  static double BGBlastWaveIntegrand(double *x, double *par);
  static Double_t BGBlastWavedNdPt_FN(double *x, double *par);
  static TF1 *GetBGBWSpectra_FN(const char *name, double mass, double temp, double beta_avg,
                                double n, double xmin, double xmax);

  bool   useAllKinds;
  bool   useFlow;
  bool   useBGBlastWave;
  double pionFraction;
  double kaonFraction;
  double protonFraction;

  double totalMultiplicityMean;
  double totalMultiplicitySigma;
  double netChargeMean;
  double netChargeSigma;
  
  double pt_Min;
  double pt_Max;
  double eta_Min;
  double eta_Max;
  double longitudinalFlow;

  TF1   *ptSpectraAllKinds;
  TF1   *azimuthalAngleAllKinds;
  TF1   *ptSpectraPions;
  TF1   *azimuthalAnglePions;
  TF1   *ptSpectraKaons;
  TF1   *azimuthalAngleKaons;
  TF1   *ptSpectraProtons;
  TF1   *azimuthalAngleProtons;

    ClassDef(BWModelEventGenerator,0)
};

#endif /* CAP_BWModelEventGenerator */

