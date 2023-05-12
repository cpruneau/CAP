#ifndef CAP_Pdf
#define CAP_Pdf
#include "MathConstants.hpp"
#include "MathBasicFunctions.hpp"

using namespace std;

//!
//! Compendium of basic and essential mathematical  functions
//!  By convention, all function names  begin with a lower case letter.

namespace CAP
{
namespace Math
{

#define kMACHEP  1.11022302462515654042363166809e-16
/* largest argument for TMath::Exp() */
#define kMAXLOG  709.782712893383973096206318587
/* smallest argument for TMath::Exp() without underflow */
#define kMINLOG  -708.396418532264078748994506896
/* the maximal number that pow(x,x-0.5) has no overflow */
/* we use a (very) conservative portable bound          */
#define kMAXSTIR  108.116855767857671821730036754
#define kMAXLGM 2.556348e305

double binomial(int  n,int  k) throw (MathException);
double binomialI(double p, int  n, int  k);
double binomialPdf(unsigned int k, double p, unsigned int n) throw (MathException);
double binomialCdf(unsigned int k, double p, unsigned int n) throw (MathException);
double binomialCdfC(unsigned int k, double p, unsigned int n) throw (MathException);

double negativeBinomialPdf(unsigned int k, double p, double n)  throw (MathException);
double negativeBinomialCdf(unsigned int k, double p, double n)  throw (MathException);
double negativeBinomialCdfC(unsigned int k, double p, double n)  throw (MathException);


double betaIncomplete(double  x, double  a, double  b) throw (MathException);
double betaCf(double  x, double  a, double  b)   throw (MathException);
double betaPdf(double x, double a, double b)     throw (MathException);
double betaCdf(double x, double a, double b )    throw (MathException);
double betaCdfC(double x, double a, double b )   throw (MathException);

double Gamma(double  z) throw (MathException);
double lnGamma(double  z) throw (MathException);
double gammaStirling(double  z) throw (MathException);
double gammaIncomplete(double alpha, double x) throw (MathException);
double gammaIncompleteC(double alpha, double x) throw (MathException);
double gammaPdf(double x, double alpha, double beta) throw (MathException);
double gammaCdf(double x, double alpha, double beta)   throw (MathException);
double gammaCdfC(double x, double alpha, double beta)   throw (MathException);

double uniformPdf(double x, double a=0, double b=1, double x0=0) throw (MathException);
double uniformCdf(double x, double a=0, double b=1, double x0=0) throw (MathException);
double uniformCdfC(double x, double a=0, double b=1, double x0=0) throw (MathException);

double exponentialPdf(double x, double lambda, double x0 = 0)   throw (MathException)
double exponentialCdf(double x, double lambda, double x0 = 0)   throw (MathException)
double exponentialCdfC(double x, double lambda, double x0 = 0)   throw (MathException)

double erfInverse(double x) throw (MathException);
double erfcInverse(double x) throw (MathException);
double standardNormalPdf(double x);
double standardNormalCdf(double x);
double standardNormalCdfC(double x);
double normalPdf(double x, double mean, double sigma) throw (MathException);
double normalCdf(double x, double mean, double sigma) throw (MathException);
double normalCdfC(double x, double mean, double sigma) throw (MathException);
double normQuantile(double p) throw (MathException);
using  gaussianPDF = normalPdf;

double logNormalPdf(double x, double sigma, double theta=0, double m=1) throw (MathException);
double logNormalCdf(double x, double sigma, double theta=0, double m=1) throw (MathException);
double logNormalCdfC(double x, double sigma, double theta=0, double m=1) throw (MathException);

double breitWignerPdf(double x, double mean=0, double gamma=1);
double breitWignerCdf(double x, double mean=0, double gamma=1);
double breitWignerCdfC(double x, double mean=0, double gamma=1);
double breitWignerRelativisticPdf(double x, double median=0, double gamma=1);
using  cauchyPdf  = breitWignerPdf;
using  cauchyCdf  = breitWignerCdf;
using  cauchyCdfC = breitWignerCdfC;

double chiSquarePdf(double x, double r, double x0 = 0) throw (MathException);
double chiSquareCdf(double x, double r, double x0 = 0) throw (MathException);
double chiSquareCdfC(double x, double r, double x0 = 0) throw (MathException);
double chiSquareQuantile(double p, double ndf);

double landauPdf(double x, double xi=1.0, double x0=0.0) throw (MathException);
double landauCdf(double x, double xi, double x0);
double landauXm1(double x, double xi, double x0);
double landauXm2(double x, double xi, double x0);

double poissonPdf(double x, double par) throw (MathException);
double poissonI(double x, double par)  throw (MathException);
double poissonCdf(unsigned int n, double mu) throw (MathException);
double poissonCdfC(unsigned int n, double mu) throw (MathException);

double fDistributionPdf(double x, double n, double m, double x0)  throw (MathException);
double fDistributionCdf(double x, double n, double m, double x0)  throw (MathException);
double fDistributionCdfC(double x, double n, double m, double x0)  throw (MathException);

double KolmogorovProb(double z);
double KolmogorovTest(int  na, const double *a, int  nb, const double *b, const char *option);

double laplacePdf(double x, double alpha=0, double beta=1)  throw (MathException);
double laplaceCdf(double x, double alpha=0, double beta=1)  throw (MathException);
double laplaceCdfC(double x, double alpha=0, double beta=1)  throw (MathException);
double laplaceQuantile(double F, double alpha=0, double beta=1)  throw (MathException);

double studentPdf(double T, double ndf)  throw (MathException);
double studentCdf(double x, double r, double x0)  throw (MathException);
double studentCdfC(double x, double r, double x0)  throw (MathException);
double studentQuantile(double p, double ndf, bool lower_tail=true)  throw (MathException);

double vavilovPdf(double x, double kappa, double beta2);
double vavilovDenEval(double  rlam, double  *AC, double  *HC, int  itype);

double voigtPdf(double x, double sigma, double lg, int  r = 4);


void quantiles(int  n, int  nprob, double  *x, double  *quantiles, double  *prob, bool isSorted, int  *index, int  type);

double crystalBallFct(double x, double alpha, double n, double sigma, double mean = 0) throw (MathException)
double crystalBallPdf(double x, double alpha, double n, double sigma, double mean = 0)     throw (MathException)







//!
//! Probability density function of the F-distribution.
//!
//! \f[ p_{n,m}(x) = \frac{\Gamma(\frac{n+m}{2})}{\Gamma(\frac{n}{2}) \Gamma(\frac{m}{2})} n^{n/2} m^{m/2} x^{n/2 -1} (m+nx)^{-(n+m)/2} \f]
//!
//! for x>=0. For detailed description see
//! <A HREF="http://mathworld.wolfram.com/F-Distribution.html">
//! Mathworld</A>.
//!
inline double fDistributionPdf(double x, double n, double m, double x0 = 0)   throw (MathException)
{
  // function is defined only for both n and m > 0
  if (n < 0 || m < 0)   throw MathException("fDistributionPdf() called with n < 0 || m < 0");
  if ((x-x0) < 0) return 0.0;
  return std::exp((n/2) * std::log(n) + (m/2) * std::log(m) + lgamma((n+m)/2) - lgamma(n/2) - lgamma(m/2)
                  + (n/2 -1) * std::log(x-x0) - ((n+m)/2) * std::log(m +  n*(x-x0)) );

}

//!
//! Probability density function of the gamma distribution.
//!
//! \f[ p(x) = {1 \over \Gamma(\alpha) \theta^{\alpha}} x^{\alpha-1} e^{-x/\theta} \f]
//!
//! for x>0. For detailed description see
//! <A HREF="http://mathworld.wolfram.com/GammaDistribution.html">
//! Mathworld</A>.
//!
inline double gammaPdf(double x, double alpha, double theta, double x0 = 0)  throw (MathException)
{
  // Inlined to enable clad-auto-derivation for this function.

  if ((x-x0) < 0)   throw MathException("gammaPdf() called with (x-x0) < 0");
  if ((x-x0) == 0) return (alpha==1) ? 1.0/theta : 0.0;
  return (alpha == 1) ?
  std::exp(-(x-x0)/theta)/theta :
  std::exp((alpha - 1) * std::log((x-x0)/theta) - (x-x0)/theta - lgamma(alpha))/theta;
}

//!
//! Probability density function of the normal (Gaussian) distribution.
//!
//! \f[ p(x) = {1 \over \sqrt{2 \pi \sigma^2}} e^{-x^2 / 2\sigma^2} \f]
//!
//! For detailed description see
//! <A HREF="http://mathworld.wolfram.com/NormalDistribution.html">
//! Mathworld</A>. It can also be evaluated using #normal_pdf which will
//! call the same implementation.
//!
inline double gaussianPdf(double x, double sigma = 1, double x0 = 0)
{
  double arg = (x-x0)/sigma;
  return oneOverSqrtTwoPi()*std::exp(-arg*arg/2)/absolute(sigma)));
}

using normalPdf = gaussianPdf;

//!
//!
//! Probability density function of the bi-dimensional (Gaussian) distribution.
//!
//! \f[ p(x) = {1 \over 2 \pi \sigma_x \sigma_y \sqrt{1-\rho^2}} \exp (-((x-x0)^2/\sigma_x^2 + (y-y0)^2/\sigma_y^2 - 2 \rho x y/(\sigma_x\sigma_y))/2(1-\rho^2)) \f]
//!
//! For detailed description see
//! <A HREF="http://mathworld.wolfram.com/BivariateNormalDistribution.html">
//! Mathworld</A>. It can also be evaluated using #normal_pdf which will
//! call the same implementation.
//!
//! @param x x variable
//! @param y y variable
//! @param sigmax the stdev in x
//! @param sigmay the stdev in y
//! @param rho correlation, must be between -1,1
//! @param x0 the offset in x
//! @param y0 the offset in y
//!
inline double biGaussianPdf(double x, double y, double sigmax = 1, double sigmay = 1, double rho = 0, double x0 = 0, double y0 = 0)
{
  double u = (x-x0)/sigmax;
  double v = (y-y0)/sigmay;
  double c = 1. - rho*rho;
  double z = u*u - 2.*rho*u*v + v*v;
  double norm = twoPi() * sigmax * sigmay * std::sqrt(c);
  return std::exp(-z/(2.0*c))/norm;
}

//!
//! Probability density function of the Landau distribution:
//! \f[ p(x) = \frac{1}{\xi} \phi (\lambda) \f]
//! with
//! \f[  \phi(\lambda) = \frac{1}{2 \pi i}\int_{c-i\infty}^{c+i\infty} e^{\lambda s + s \log{s}} ds\f]
//! where \f$\lambda = (x-x_0)/\xi\f$. For a detailed description see
//! K.S. K&ouml;lbig and B. Schorr, A program package for the Landau distribution,
//! <A HREF="http://dx.doi.org/10.1016/0010-4655(84)90085-7">Computer Phys. Comm. 31 (1984) 97-111</A>
//! <A HREF="http://dx.doi.org/10.1016/j.cpc.2008.03.002">[Erratum-ibid. 178 (2008) 972]</A>.
//! The same algorithms as in
//! <A HREF="https://cern-tex.web.cern.ch/cern-tex/shortwrupsdir/g110/top.html">
//! CERNLIB</A> (DENLAN)  is used
//!
//! @param x The argument \f$x\f$
//! @param xi The width parameter \f$\xi\f$
//! @param x0 The location parameter \f$x_0\f$
//!
double landauPdf(double x, double xi = 1, double x0 = 0);


//!
//!
//! Probability density function of the lognormal distribution.
//!
//! \f[ p(x) = {1 \over x \sqrt{2 \pi s^2} } e^{-(\ln{x} - m)^2/2 s^2} \f]
//!
//! for x>0. For detailed description see
//! <A HREF="http://mathworld.wolfram.com/LogNormalDistribution.html">
//! Mathworld</A>.
//! @param x x variable
//! @param m M = 0 for lognormal
//! @param s scale parameter (not the sigma of the distribution which is not even defined)
//! @param x0  location parameter, corresponds approximately to the most probable value. For x0 = 0, sigma = 1, the x_mpv = -0.22278
//!
inline double logNormalPdf(double x, double m, double s, double x0 = 0)  throw (MathException)
{
  // Inlined to enable clad-auto-derivation for this function.
  if ((x-x0) <= 0) MathException("logNormalPdf() (x-x0) <= 0 ")
  double arg = (std::log((x-x0)) - m)/s;
  return 1.0 / ((x-x0) * std::fabs(s) * sqrtTwoPi()) * std::exp(-(arg*arg) /2);
}


//!
//!
//! Probability density function of the Poisson distribution.
//!
//! \f[ p(n) = \frac{\mu^n}{n!} e^{- \mu} \f]
//!
//! For detailed description see
//! <A HREF="http://mathworld.wolfram.com/PoissonDistribution.html">
//! Mathworld</A>.
//!
inline double poissonPdf(unsigned int n, double mu)   throw (MathException)
{
  if (mu < 0)  throw MathException("poissonPdf() mu < 0");
  if (n > 0 && mu >= 0) return std::exp (n*std::log(mu) - lgamma(n+1) - mu);
  return std::exp(-mu);
}

//!
//!Probability density function of Student's t-distribution.
//!
//! \f[ p_{r}(x) = \frac{\Gamma(\frac{r+1}{2})}{\sqrt{r \pi}\Gamma(\frac{r}{2})} \left( 1+\frac{x^2}{r}\right)^{-(r+1)/2}  \f]
//!
//! for \f$k \geq 0\f$. For detailed description see
//! <A HREF="http://mathworld.wolfram.com/Studentst-Distribution.html">
//! Mathworld</A>.
//!
inline double tDistributionPdf(double x, double r, double x0 = 0)
{
  double arg = x-x0;
  return (std::exp (lgamma((r + 1.0)/2.0) - lgamma(r/2.0)) / std::sqrt (pi() * r))
  * std::pow((1.0 + arg*arg/r), -(r + 1.0)/2.0);
}


//!
//! Probability density function of the uniform (flat) distribution.
//!
//! \f[ p(x) = {1 \over (b-a)} \f]
//!
//! if \f$a \leq x<b\f$ and 0 otherwise. For detailed description see
//! <A HREF="http://mathworld.wolfram.com/UniformDistribution.html">
//! Mathworld</A>.
//!
inline double uniformPdf(double x, double a, double b, double x0 = 0)   throw (MathException)
{
  if (a==b) throw MathException("uniformPdf() a==b");
  double arg = x-x0;
  if (arg>=a && arg<b) return 1.0/(b - a);
  return 0.0;
}


}

}

#endif
