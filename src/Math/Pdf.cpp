#include "MathBase.hpp"
#include "TString.h"
#include <Math/SpecFuncMathCore.h>
#include <Math/PdfFuncMathCore.h>
#include <Math/ProbFuncMathCore.h>

namespace CAP
{

namespace Math
{

//!
//! Calculates the binomial coefficient n over k.
//!
double  binomial(int  n,int  k)  throw (MathException)
{
  if (n<0 || k<0 || n<k) throw MathException("n<0 || k<0 || n<k","binomial(int n,int k) ");
  if (k==0 || n==k) return 1;
  int  k1 = minimum(k,n-k);
  int  k2 = n-k1;
  double  fact=k2+1;
  for (double  i=k1;i>1.;--i) fact *= (k2+i)/i;
  return fact;
}

//!
//! Suppose an event occurs with probability _p_ per trial
//! Then the probability P of its occurring _k_ or more times
//! in _n_ trials is termed a cumulative binomial probability
//! the formula is:
//! ~~~ {cpp}
//! P = sum_from_j=k_to_n(binomial (n, j)**power (p, j)*power (1-p, n-j)
//! ~~~
//! For _n_ larger than 12 betaIncomplete is a much better way
//! to evaluate the sum than would be the straightforward sum calculation
//! for _n_ smaller than 12 either method is acceptable ("Numerical Recipes")
///
//! \author Anna Kreshuk
//!
double  binomialI(double  p, int  n, int  k)
{
  if(k <= 0) return 1.0;
  if(k > n) return 0.0;
  if(k == n) return power(p,n);
  return betaIncomplete(p, double (k), double (n-k+1));
}

//!
//!
//! Probability density function of the binomial distribution.
//!
//! \f[ p(k) = \frac{n!}{k! (n-k)!} p^k (1-p)^{n-k} \f]
//!
//! for \f$ 0 \leq k \leq n \f$. For detailed description see
//! <A HREF="http://mathworld.wolfram.com/BinomialDistribution.html">
//! Mathworld</A>.
//!
double binomialPdf(unsigned int k, double p, unsigned int n) throw (MathException)
{
  if (k > n) throw MathException("k>n","binomialPdf()");
  double coeff = lgamma(n+1) - lgamma(k+1) - lgamma(n-k+1);
  return std::exp(coeff + k * std::log(p) + (n - k) * log1p(-p));
}

double binomialCdf(unsigned int k, double p, unsigned int n) throw (MathException)
{
  if ( k >= n) return 1.0;
  double a = (double) k + 1.0;
  double b = (double) n - k;
  return ROOT::Math::beta_cdf_c(p, a, b);
}

double binomialCdfC(unsigned int k, double p, unsigned int n) throw (MathException)
{
  if ( k >= n)   return 0;
  double a = (double) k + 1.0;
  double b = (double) n - k;
  return ROOT::Math::beta_cdf(p, a, b);
}


//!
//!Probability density function of the negative binomial distribution.
//!
//! \f[ p(k) = \frac{(k+n-1)!}{k! (n-1)!} p^{n} (1-p)^{k} \f]
//!
//! For detailed description see
//! <A HREF="http://mathworld.wolfram.com/NegativeBinomialDistribution.html">
//! Mathworld</A> (where \f$k \to x\f$ and \f$n \to r\f$).
//! The distribution in <A HREF="http://en.wikipedia.org/wiki/Negative_binomial_distribution">
//! Wikipedia</A> is defined with a \f$p\f$ corresponding to \f$1-p\f$ in this case.
//!
double negativeBinomialPdf(unsigned int k, double p, double n)  throw (MathException)
{
  if (n < 0)  throw MathException("n<0","negativeBinomialPdf()");
  if (p < 0 || p > 1.0)  throw MathException("p < 0 || p > 1.0","negativeBinomialPdf()");
  double coeff = lgamma(k+n) - lgamma(k+1.0) - lgamma(n);
  return std::exp(coeff + n * std::log(p) + double(k) * log1p(-p));
}

double negativeBinomialCdf(unsigned int k, double p, double n)  throw (MathException)
{
  if (n < 0)  throw MathException("n<0","negativeBinomialCdf()");
  if (p < 0 || p > 1.0)  throw MathException("p < 0 || p > 1.0","negativeBinomialCdf()");
  return ROOT::Math::beta_cdf(p, n, k+1.0);
}


double negativeBinomialCdfC(unsigned int k, double p, double n)  throw (MathException)
{
  if (n < 0)  throw MathException("n<0","negativeBinomialCdf()");
  if (p < 0 || p > 1.0)  throw MathException("p < 0 || p > 1.0","negativeBinomialCdf()");
  return ROOT::Math::beta_cdf_c(p, n, k+1.0);
}


//!
//! Calculates the incomplete beta-function.
//!
double  betaIncomplete(double  x, double  a, double  b)
{
  return inc_beta(x, a, b);
}

//!
//! Continued fraction evaluation by modified Lentz's method
//! used in calculation of incomplete beta function.
//!
double  betaCf(double  x, double  a, double  b) throw (MathException)
{
  int     itmax = 500;
  double  eps = 3.e-14;
  double  fpmin = 1.e-30;

  int  m, m2;
  double  aa, c, d, del, qab, qam, qap;
  double  h;
  qab = a+b;
  qap = a+1.0;
  qam = a-1.0;
  c = 1.0;
  d = 1.0 - qab*x/qap;
  if (absolute(d)<fpmin) d=fpmin;
  d=1.0/d;
  h=d;
  for (m=1; m<=itmax; m++) {
    m2=m*2;
    aa = m*(b-m)*x/((qam+ m2)*(a+m2));
    d = 1.0 +aa*d;
    if(absolute(d)<fpmin) d = fpmin;
    c = 1 +aa/c;
    if (absolute(c)<fpmin) c = fpmin;
    d=1.0/d;
    h*=d*c;
    aa = -(a+m)*(qab +m)*x/((a+m2)*(qap+m2));
    d=1.0+aa*d;
    if(absolute(d)<fpmin) d = fpmin;
    c = 1.0 +aa/c;
    if (absolute(c)<fpmin) c = fpmin;
    d=1.0/d;
    del = d*c;
    h*=del;
    if (absolute(del-1)<=eps) break;
  }
  if (m>itmax) throw MathException("a or b too big, or itmax too small","betaCf()");
  }
  return h;
}

//!
//! Computes the probability density function of the beta distribution
//! (the distribution function is computed in betaDistI).
//! The first argument is the point, where the function will be
//! computed, second and third are the function parameters.
//! Since the beta distribution is bounded on both sides, it's often
//! used to represent processes with natural lower and upper limits.
//! https://en.wikipedia.org/wiki/Beta_distribution
//!
double  betaPdf(double  x, double  p, double  q)  throw (MathException)
{
  if ((x<0) || (x>1) || (p<=0) || (q<=0)) throw MathException("(x<0) || (x>1) || (p<=0) || (q<=0)","betaDist()");
  double  beta = beta(p,q);
  return power(x, p-1)*power(1-x, q-1)/beta;
}

//!
//! Probability density function of the beta distribution.
//! \f[ p(x) = \frac{\gamma (a + b) } {\gamma(a)\gamma(b) } x ^{a-1} (1 - x)^{b-1} \f]
//!
//! for \f$0 \leq x \leq 1 \f$. For detailed description see
//!  <A HREF="http://mathworld.wolfram.com/BetaDistribution.html">
//!  Mathworld</A>.
//!
//double betaPdf(double x, double a, double b)
//{
//  if (x < 0 || x > 1.0) return 0;
//  if (x == 0 )
//    {
//    if (a < 1) return  std::numeric_limits<double>::infinity();
//    else if (a > 1) return  0;
//    else if ( a == 1) return b; // to avoid a nan from log(0)*0
//    }
//  if (x == 1 )
//    {
//    if (b < 1) return  std::numeric_limits<double>::infinity();
//    else if (b > 1) return  0;
//    else if ( b == 1) return a; // to avoid a nan from log(0)*0
//    }
//  return std::exp( lgamma(a + b) - lgamma(a) - lgamma(b) +
//                  std::log(x) * (a -1.) + log1p(-x ) * (b - 1.) );
//}



double betaCdf(double x, double a, double b )  throw (MathException)
{
  return betaIncomplete(x, a, b);
}


double betaCdfC(double x, double a, double b) throw (MathException)
{
  // use the fact that I(x,a,b) = 1. - I(1-x,b,a)
  return betaIncomplete(1-x, b, a);
}

//!
//! Computation of Gamma(z) for all z.
//!
double  Gamma(double  z)
{
  return tgamma(z);
}

//!
//! Computation of the normalized lower incomplete gamma function P(a,x) as defined in the
//! Handbook of Mathematical Functions by Abramowitz and Stegun, formula 6.5.1 on page 260 .
//! Its normalization is such that gamma(a,+infinity) = 1 .
///
//!  \f[
//!  P(a, x) = \frac{1}{\Gamma(a)} \int_{0}^{x} t^{a-1} e^{-t} dt
//!  \f]
///
//! \author NvE 14-nov-1998 UU-SAP Utrecht
//!
double  gamma(double  a,double  x)
{
  return gammaIncomplete(a, x);
}

//!
//! Computation of ln[Gamma(z)] for all z.
//!
double  lnGamma(double  z)
{
  return lgamma(z);
}

static double kBig    =  4.503599627370496e15;
static double kBiginv =  2.22044604925031308085e-16;
static double LS2PI   =  0.91893853320467274178;

double gammaIncomplete(double a, double x) throw (MathException)
{
  double ans, ax, c, r;

  // LM: for negative values returns 1.0 instead of zero
  // This is correct if a is a negative integer since Gamma(-n) = +/- inf
  if (a <= 0)  return 1.0;
  if (x <= 0)  return 0.0;
  if( (x>1.0) && (x>a)) return( 1.0 - gammaIncompleteC(a,x) );

  /* Compute  x**a * exp(-x) / gamma(a)  */
  ax = a * std::log(x) - x - lgam(a);
  if( ax < -kMAXLOG ) return( 0.0 );
  ax = std::exp(ax);
  /* power series */
  r   = a;
  c   = 1.0;
  ans = 1.0;
  do
    {
    r += 1.0;
    c *= x/r;
    ans += c;
    }
  while( c/ans > kMACHEP );
  return( ans * ax/a );
}

double gammaIncompleteC(double a, double x) throw (MathException)
{
  double ans, ax, c, yc, r, t, y, z;
  double pk, pkm1, pkm2, qk, qkm1, qkm2;
  // LM: for negative values returns 0.0
  // This is correct if a is a negative integer since Gamma(-n) = +/- inf
  if (a <= 0)  return 0.0;
  if (x <= 0) return 1.0;
  if( (x < 1.0) || (x < a) )  return( 1.0 - gammaIncomplete(a,x) );
  ax = a * std::log(x) - x - lgam(a);
  if( ax < -kMAXLOG )
    return( 0.0 );

  ax = std::exp(ax);

  /* continued fraction */
  y = 1.0 - a;
  z = x + y + 1.0;
  c = 0.0;
  pkm2 = 1.0;
  qkm2 = x;
  pkm1 = x + 1.0;
  qkm1 = z * x;
  ans = pkm1/qkm1;

  do
    {
    c += 1.0;
    y += 1.0;
    z += 2.0;
    yc = y * c;
    pk = pkm1 * z  -  pkm2 * yc;
    qk = qkm1 * z  -  qkm2 * yc;
    if(qk)
      {
      r = pk/qk;
      t = std::abs( (ans - r)/r );
      ans = r;
      }
    else
      t = 1.0;
    pkm2 = pkm1;
    pkm1 = pk;
    qkm2 = qkm1;
    qkm1 = qk;
    if( std::abs(pk) > kBig )
      {
      pkm2 *= kBiginv;
      pkm1 *= kBiginv;
      qkm2 *= kBiginv;
      qkm1 *= kBiginv;
      }
    }
  while( t > kMACHEP );

  return( ans * ax );
}



static double A[] = {
  8.11614167470508450300E-4,
  -5.95061904284301438324E-4,
  7.93650340457716943945E-4,
  -2.77777777730099687205E-3,
  8.33333333333331927722E-2
};

static double B[] = {
  -1.37825152569120859100E3,
  -3.88016315134637840924E4,
  -3.31612992738871184744E5,
  -1.16237097492762307383E6,
  -1.72173700820839662146E6,
  -8.53555664245765465627E5
};

static double C[] = {
  /* 1.00000000000000000000E0, */
  -3.51815701436523470549E2,
  -1.70642106651881159223E4,
  -2.20528590553854454839E5,
  -1.13933444367982507207E6,
  -2.53252307177582951285E6,
  -2.01889141433532773231E6
};

//!
//! Logarithm of gamma function */
//! A[]: Stirling's formula expansion of log gamma
//! B[], C[]: log gamma function between 2 and 3
//!
double lnGamma(double x )
{
  double p, q, u, w, z;
  int i;
  int sgngam = 1;
  if (x >= std::numeric_limits<double>::infinity()) return(std::numeric_limits<double>::infinity());
  if( x < -34.0 )
    {
    q = -x;
    w = lnGamma(q);
    p = std::floor(q);
    if( p==q ) return (std::numeric_limits<double>::infinity());
    i = (int) p;
    if( (i & 1) == 0 )
      sgngam = -1;
    else
      sgngam = 1;
    z = q - p;
    if( z > 0.5 )
      {
      p += 1.0;
      z = p - q;
      }
    z = q * std::sin( pi() * z );
    if( z == 0 )
      return (std::numeric_limits<double>::infinity());
    z = std::log(pi()) - std::log( z ) - w;
    return( z );
    }
  if( x < 13.0 )
    {
    z = 1.0;
    p = 0.0;
    u = x;
    while( u >= 3.0 )
      {
      p -= 1.0;
      u = x + p;
      z *= u;
      }
    while( u < 2.0 )
      {
      if( u == 0 )
        return (std::numeric_limits<double>::infinity());
      z /= u;
      p += 1.0;
      u = x + p;
      }
    if( z < 0.0 )
      {
      sgngam = -1;
      z = -z;
      }
    else
      sgngam = 1;
    if( u == 2.0 )
      return( std::log(z) );
    p -= 2.0;
    x = x + p;
    p = x * Polynomialeval(x, B, 5 ) / Polynomial1eval( x, C, 6);
    return( std::log(z) + p );
    }

  if( x > kMAXLGM )
    return( sgngam * std::numeric_limits<double>::infinity() );

  q = ( x - 0.5 ) * std::log(x) - x + LS2PI;
  if( x > 1.0e8 )
    return( q );

  p = 1.0/(x*x);
  if( x >= 1000.0 )
    q += ((   7.9365079365079365079365e-4 * p
           - 2.7777777777777777777778e-3) *p
          + 0.0833333333333333333333) / x;
  else
    q += Polynomialeval( p, A, 4 ) / x;
  return( q );
}

static double P[] = {
  1.60119522476751861407E-4,
  1.19135147006586384913E-3,
  1.04213797561761569935E-2,
  4.76367800457137231464E-2,
  2.07448227648435975150E-1,
  4.94214826801497100753E-1,
  9.99999999999999996796E-1
};
static double Q[] = {
  -2.31581873324120129819E-5,
  5.39605580493303397842E-4,
  -4.45641913851797240494E-3,
  1.18139785222060435552E-2,
  3.58236398605498653373E-2,
  -2.34591795718243348568E-1,
  7.14304917030273074085E-2,
  1.00000000000000000320E0
};

/* Stirling's formula for the gamma function */
static double STIR[5] = {
  7.87311395793093628397E-4,
  -2.29549961613378126380E-4,
  -2.68132617805781232825E-3,
  3.47222221605458667310E-3,
  8.33333333333482257126E-2,
};

//!
//! Stirling formula for the gamma function
//!
static double gammaStirling(double x)  throw (MathException)
{
  double y, w, v;
  w = 1.0/x;
  w = 1.0 + w * Polynomialeval( w, STIR, 4 );
  y = exp(x);
  if( x > kMAXSTIR )
    { /* Avoid overflow in pow() */
      v = pow( x, 0.5 * x - 0.25 );
      y = v * (v / y);
    }
  else
    {
    y = pow( x, x - 0.5 ) / y;
    }
  y = SQTPI * y * w;
  return( y );
}



//!
//! Computes the density function of gamma distribution at point x.
//!
//! \param[in] x       evaluation point
//! \param[in] gamma   shape parameter
//! \param[in] mu      location parameter
//! \param[in] beta    scale parameter
//!
//! The gamma density function (PDF) is computed in terms of share parameter alpha and rate parameter beta.
//!See e.g., https://en.wikipedia.org/wiki/Gamma_distribution
//!
double gammaPdf(double x, double alpha, double beta)    throw (MathException)
{
  if (alpha<=0.0 || beta<=0.0) throw MathException("gammaPdf() alpha<=0.0 || beta<=0.0");
  double arg =  alpha*std::log(beta) + (alpha-1)*std::log(x) - beta*x - lnGamma(alpha);
  return exp(arg);
}

double gammaCdf(double x, double alpha, double beta)   throw (MathException)
{
  return gammaIncomplete(alpha, beta*x);
}

double gammaCdfC(double x, double alpha, double theta, double x0)   throw (MathException)
{
  return 1.0-gammaIncomplete(alpha, beta*x);
}

//!
//! Computation of the incomplete gamma function P(a,x)
//! via its continued fraction representation.
///
//! \author NvE 14-nov-1998 UU-SAP Utrecht
//!
double  gamCf(double  a,double  x)
{
  int  itmax    = 100;      // Maximum number of iterations
  double  eps   = 3.e-14;   // Relative accuracy
  double  fpmin = 1.e-30;   // Smallest double  value allowed here

  if (a <= 0 || x <= 0) return 0;

  double  gln = lnGamma(a);
  double  b   = x+1-a;
  double  c   = 1/fpmin;
  double  d   = 1/b;
  double  h   = d;
  double  an,del;
  for (int  i=1; i<=itmax; i++) {
    an = double (-i)*(double (i)-a);
    b += 2;
    d  = an*d+b;
    if (absolute(d) < fpmin) d = fpmin;
    c = b+an/c;
    if (absolute(c) < fpmin) c = fpmin;
    d   = 1/d;
    del = d*c;
    h   = h*del;
    if (absolute(del-1) < eps) break;
    //if (i==itmax) std::cout << "*gamCf(a,x)* a too large or itmax too small" << std::endl;
  }
  double  v = exp(-x+a*log(x)-gln)*h;
  return (1-v);
}

//!
//! Computation of the incomplete gamma function P(a,x)
//! via its series representation.
///
//! \author NvE 14-nov-1998 UU-SAP Utrecht
//!
double  GamSer(double  a,double  x)
{
  int  itmax  = 100;    // Maximum number of iterations
  double  eps = 3.e-14; // Relative accuracy

  if (a <= 0 || x <= 0) return 0;

  double  gln = lnGamma(a);
  double  ap  = a;
  double  sum = 1/a;
  double  del = sum;
  for (int  n=1; n<=itmax; n++) {
    ap  += 1;
    del  = del*x/ap;
    sum += del;
    if (absolute(del) < absolute(sum*eps)) break;
    //if (n==itmax) std::cout << "*GamSer(a,x)* a too large or itmax too small" << std::endl;
  }
  double  v = sum*exp(-x+a*log(x)-gln);
  return v;
}



double uniformPdf(double x, double a, double b, double x0)  throw (MathException)
{
  if ((b-a)<=0.0) throw MathException("uniformPdf() (b-a)<=0.0");
  double arg = x - x0;
  if      (arg <  a)   return 0.0;
  else if (arg >= b)   return 0.0;
  else   return 1.0/(b-a);
}


double uniformCdf(double x, double a, double b, double x0)  throw (MathException)
{
  if ((b-a)<=0.0) throw MathException("uniformCdf() (b-a)<=0.0");
  double arg = x - x0;
  if      (arg <  a)   return 0.0;
  else if (arg >= b)   return 1.0;
  else   return (arg-a)/(b-a);
}

double uniformCdfC(double x, double a, double b, double x0)  throw (MathException)
{
  if ((b-a)<=0.0) throw MathException("uniformCdfC() (b-a)<=0.0");
  double arg = x - x0;
  if      (arg <  a)   return 1.0;
  else if (arg >= b)   return 0.0;
  else return (b-arg)/(b-a);
}

//!
//! Probability density function (PDF) of the exponential distribution.
//!
//! \f[ p(x) = \lambda e^{-\lambda x} \f]
//!
//! for x>0. For detailed description see
//! <A HREF="http://mathworld.wolfram.com/ExponentialDistribution.html">
//! Mathworld</A>.
//!
double exponentialPdf(double x, double lambda, double x0 = 0)   throw (MathException)
{
  double arg = x-x0;
  if (arg<0)  throw MathException("exponentialPdf() called with (x-x0) < 0.");
  return lambda * std::exp(-lambda*arg);
}

//!
//! Cumulative  density function (CDF) of the exponential distribution.
//!
double exponentialCdf(double x, double lambda, double x0 = 0)   throw (MathException)
{
  double arg = x-x0;
  if (arg<0)  throw MathException("exponentialPdf() called with (x-x0) < 0.");
  return 1.0 - std::exp(-lambda*arg);
}

//!
//! Complement of the cumulative  density function (CDF) of the exponential distribution.
//!
double exponentialCdfC(double x, double lambda, double x0 = 0)   throw (MathException)
{
  double arg = x-x0;
  if (arg<0)  throw MathException("exponentialPdf() called with (x-x0) < 0.");
  return std::exp(-lambda*arg);
}

//!
//! Returns the inverse error function.
//! x must be  <-1<x<1
//!
double  erfInverse(double  x) throw (MathException)
{
  int     kMaxit    = 50;
  double  kEps   = 1e-14;
  double  kConst = 0.8862269254527579;     // sqrt(pi)/2.0

  if(absolute(x) <= kEps) return kConst*x;

  // Newton iterations
  double  erfi, derfi, y0,y1,dy0,dy1;
  if(absolute(x) < 1.0) {
    erfi  = kConst*absolute(x);
    y0    = erf(0.9*erfi);
    derfi = 0.1*erfi;
    for (int  iter=0; iter<kMaxit; iter++) {
      y1  = 1. - erfc(erfi);
      dy1 = absolute(x) - y1;
      if (absolute(dy1) < kEps)  {if (x < 0) return -erfi; else return erfi;}
      dy0    = y1 - y0;
      derfi *= dy1/dy0;
      y0     = y1;
      erfi  += derfi;
      if(absolute(derfi/erfi) < kEps) {if (x < 0) return -erfi; else return erfi;}
    }
  }
  throw MathException("erfInverse() Did not converge");
}

//!
//! Returns the inverse of the complementary error function.
//! x must be  0<x<2
//! implement using  the quantile of the normal distribution
//! instead of erfInverse for better numerical precision for large x
//!
double  erfcInverse(double  x)  throw (MathException)
{
  // erfc-1(x) = - 1/sqrt(2) * normal_quantile( 0.5 * x)
  return - 0.70710678118654752440 * normQuantile( 0.5 * x);
}

//!
//! Calculates the value of the standard normal PDF
//!
double  standardNormalPdf(double x)
  {
  if (x < -39.0 || x > 39.0) return 0.0;
  return exp(-0.5*x*x)/sqrtTwoPi();
  }

//!
//! Computation of the CDF of the standard normal  PDF
//! i.e.,  (1/sqrt(2pi)) Integral(exp(-t^2/2))dt between -infinity and x.
///
double standardNormalCdf(double  x)
{
  if (x < -1.0)
    return 0.5*erfc(-x);
  else
    return 0.5*(1.0+erf(x));
}

double standardNormalCdfC(double x);
{
  if (x > 1.)
    return 0.5*erfc(x);
  else
    return 0.5*(1.-erf(x));
}

//!
//! Calculates the value of a normal (gaussian) function with mean and sigma at the give value of x.
//! If norm=true (default is false) the result is divided
//! by sqrt(2*Pi)*sigma.
//!
double  normalPdf(double  x, double  mean, double  sigma) throw (MathException)
{
  if (sigma <= 0) throw MathException("normalPdf() sigma <= 0");
  return standardNormalPdf( (x-mean)/sigma );
}

//!
//! Computation of the CDF of the normal  PDF
//! i.e.,  (1/sqrt(2pi) sigma) Integral(exp(- (x-mean)^2/(2sigma^2))dx between -infinity and x.
///
double normalCdf(double  x) throw (MathException)
{
  if (sigma <= 0) throw MathException("normalCdf() sigma <= 0");
  return standardNormalCdf( (x-mean)/sigma );
}

//!
//! Computation of the complement of the  CDF of the normal  PDF
//! i.e.,  (1/sqrt(2pi) sigma) Integral(exp(- (x-mean)^2/(2sigma^2))dx between -infinity and x.
///
double normalCdf(double  x) throw (MathException)
{
  if (sigma <= 0) throw MathException("normalCdf() sigma <= 0");
  return standardNormalCdfC( (x-mean)/sigma );
}

//!
//! Computes quantiles for standard normal distribution N(0, 1)
//! at probability p
///
//! ALGORITHM AS241  APPL. STATIST. (1988) VOL. 37, NO. 3, 477-484.
//!
double  normQuantile(double  p)  throw (MathException)
{
  if ((p<=0)||(p>=1)) throw MathException("normQuantile() probability outside (0, 1)");
  double   a0 = 3.3871328727963666080e0;
  double   a1 = 1.3314166789178437745e+2;
  double   a2 = 1.9715909503065514427e+3;
  double   a3 = 1.3731693765509461125e+4;
  double   a4 = 4.5921953931549871457e+4;
  double   a5 = 6.7265770927008700853e+4;
  double   a6 = 3.3430575583588128105e+4;
  double   a7 = 2.5090809287301226727e+3;
  double   b1 = 4.2313330701600911252e+1;
  double   b2 = 6.8718700749205790830e+2;
  double   b3 = 5.3941960214247511077e+3;
  double   b4 = 2.1213794301586595867e+4;
  double   b5 = 3.9307895800092710610e+4;
  double   b6 = 2.8729085735721942674e+4;
  double   b7 = 5.2264952788528545610e+3;
  double   c0 = 1.42343711074968357734e0;
  double   c1 = 4.63033784615654529590e0;
  double   c2 = 5.76949722146069140550e0;
  double   c3 = 3.64784832476320460504e0;
  double   c4 = 1.27045825245236838258e0;
  double   c5 = 2.41780725177450611770e-1;
  double   c6 = 2.27238449892691845833e-2;
  double   c7 = 7.74545014278341407640e-4;
  double   d1 = 2.05319162663775882187e0;
  double   d2 = 1.67638483018380384940e0;
  double   d3 = 6.89767334985100004550e-1;
  double   d4 = 1.48103976427480074590e-1;
  double   d5 = 1.51986665636164571966e-2;
  double   d6 = 5.47593808499534494600e-4;
  double   d7 = 1.05075007164441684324e-9;
  double   e0 = 6.65790464350110377720e0;
  double   e1 = 5.46378491116411436990e0;
  double   e2 = 1.78482653991729133580e0;
  double   e3 = 2.96560571828504891230e-1;
  double   e4 = 2.65321895265761230930e-2;
  double   e5 = 1.24266094738807843860e-3;
  double   e6 = 2.71155556874348757815e-5;
  double   e7 = 2.01033439929228813265e-7;
  double   f1 = 5.99832206555887937690e-1;
  double   f2 = 1.36929880922735805310e-1;
  double   f3 = 1.48753612908506148525e-2;
  double   f4 = 7.86869131145613259100e-4;
  double   f5 = 1.84631831751005468180e-5;
  double   f6 = 1.42151175831644588870e-7;
  double   f7 = 2.04426310338993978564e-15;

  double  split1 = 0.425;
  double  split2=5.;
  double  konst1=0.180625;
  double  konst2=1.6;

  double  q, r, quantile;
  q=p-0.5;
  if (absolute(q)<split1) {
    r=konst1-q*q;
    quantile = q* (((((((a7 * r + a6) * r + a5) * r + a4) * r + a3)
                     * r + a2) * r + a1) * r + a0) /
    (((((((b7 * r + b6) * r + b5) * r + b4) * r + b3)
       * r + b2) * r + b1) * r + 1.);
  } else {
    if(q<0) r=p;
    else    r=1-p;
    //error case
    if (r<=0)
      quantile=0;
    else {
      r=sqrt(-log(r));
      if (r<=split2) {
        r=r-konst2;
        quantile=(((((((c7 * r + c6) * r + c5) * r + c4) * r + c3)
                    * r + c2) * r + c1) * r + c0) /
        (((((((d7 * r + d6) * r + d5) * r + d4) * r + d3)
           * r + d2) * r + d1) * r + 1);
      } else{
        r=r-split2;
        quantile=(((((((e7 * r + e6) * r + e5) * r + e4) * r + e3)
                    * r + e2) * r + e1) * r + e0) /
        (((((((f7 * r + f6) * r + f5) * r + f4) * r + f3)
           * r + f2) * r + f1) * r + 1);
      }
      if (q<0) quantile=-quantile;
    }
  }
  return quantile;
}


//!
//! Computes the density of logNormalPdf distribution at point x.
//! Variable X has logNormalPdf distribution if Y=Ln(X) has normal distribution
///
//! \param[in] x      is the evaluation point
//! \param[in] sigma  is the shape parameter
//! \param[in] theta  is the location parameter
//! \param[in] m      is the scale parameter
///
//! The formula was taken from "Engineering Statistics Handbook" on site
//! http://www.itl.nist.gov/div898/handbook/eda/section3/eda3669.htm
//!
double logNormalPdf(double  x, double  sigma, double  theta, double  m)  throw (MathException)
{
  if ((x<theta) || (sigma<=0) || (m<=0)) throw MathException("logNormalPdf() illegal parameter values");
  return logNormalPdf_pdf(x, log(m), sigma, theta);
}

double logNormalCdf(double x, double sigma, double theta=0, double m=1) throw (MathException)
{
  double z = (std::log((x-x0))-m)/(s*sqrtTwo());
  if (z < -1.)
    return 0.5*erfc(-z);
  else
    return 0.5*(1.0 + erf(z));
}

double logNormalCdfC(double x, double sigma, double theta=0, double m=1) throw (MathException)
{
  double z = (std::log((x-x0))-m)/(s*sqrtTwo());
  if (z > 1.)  return 0.5*erfc(z);
  else         return 0.5*(1.0 - erf(z));
}

//!
//! Probability density function (PDF) of Breit-Wigner distribution,.
//!
//! \f[ p(x) = \frac{1}{\pi} \frac{\frac{1}{2} \gamma}{x^2 + (\frac{1}{2} \gamma)^2} \f]
//! \param[in] x0 : location parameter
//! \param[in] gamma : scale parameter  specifies the full-width at half-maximum (FWHM)
//!
double breitWignerPdf(double x, double gamma=1, double x0=0)
{
  double gammaHalf = gamma/2.0;
  double arg = x-x0;
  return gammaHalf/(pi() * (arg*arg + gammaHalf*gammaHalf));
}

//!
//! Cumulative density function (CDF) of Breit-Wigner distribution
//! \param[in] x0 : location parameter
//! \param[in] gamma : scale parameter  specifies the full-width at half-maximum (FWHM)
//!
double breitWignerCdf(double x, double gamma=1, double x0=0)
{
  return 0.5 + std::atan(2.0 * (x-x0) / gamma) / pi();
}

//!
//! Complement of the cumulative density function (CDFC) of Breit-Wigner distribution,.
//! \param[in] x0 : location parameter
//! \param[in] gamma : scale parameter  specifies the full-width at half-maximum (FWHM)
//!
double breitwignerCdfC(double x, double gamma=1, double x0=0)
{
  return 0.5 - std::atan(2.0 * (x-x0) / gamma) / pi();
}

//!
//! Calculates a Relativistic Breit Wigner function with median and gamma.
// \f$ BW(E) = \frac{2\sqrt{2}}{\pi}\frac{M^{2}\gamma\sqrt{M^{2} + \gamma^{2}}}{\left(\sqrt{M^{2}+M\sqrt{M^{2} + \gamma^{2}}}\right)\left(\left(E^{2} - M^{2}\right)^{2} + M^{2}\gamma^{2}\right)} \f$
//! \param[in] x
//! \param[in] median
//! \param[in] gamma
double  breitWignerRelativisticPdf(double  x, double  median, double  gamma)
{
  double  mm = median*median;
  double  gg = gamma*gamma;
  double  mg = median*gamma;
  double  xxMinusmm = x*x - mm;
  double  y = sqrt(mm * (mm + gg));
  double  k = (0.90031631615710606*mg*y)/(sqrt(mm+y)); //2*sqrt(2)/pi = 0.90031631615710606
  double  bw = k/(xxMinusmm*xxMinusmm + mg*mg);
  return bw;
}

//!
//! Probability density function of the \f$\chi^2\f$ distribution with \f$r\f$
//! degrees of freedom.
//!
//! \f[ p_r(x) = \frac{1}{\gamma(r/2) 2^{r/2}} x^{r/2-1} e^{-x/2} \f]
//!
//! for \f$x \geq 0\f$. For detailed description see
//! <A HREF="http://mathworld.wolfram.com/Chi-SquaredDistribution.html">
//! Mathworld</A>.
//!
//! \param[in] x : chi2
//! \param[in] x : dof
//! \param[in] x : location parameter
//!
double chiSquarePdf(double x, double r, double x0)   throw (MathException)
{
  double arg = x - x0;
  if (arg<0)  throw MathException("chiSquarePdf() (x-x0) <  0");
  double a = r/2 -1.;
  if (arg==0 && a == 0) return 0.5;
  return std::exp(a*std::log(arg/2) - arg/2 - lgamma(r/2) )/2;
}

//!
//!Cumulative chiSquare probability density functions
//!
//! \param[in] x : chi2
//! \param[in] x : dof
//! \param[in] x : location parameter
//!
double chiSquarCdf(double x, double ndf, double x0) throw (MathException)
{
  double arg = x - x0;
  if (arg<0)  throw MathException("chiSquarePdf() (x-x0) <  0");
  return gammaIncomplete ( 0.5*ndf, 0.5*arg);
}

//!
//! Computation of the probability density of a given chi-squared (chi2)
//! for a specific  number of degrees of freedom (ndf).
///
//! Calculations are based on the incomplete gamma function P(a,x),
//! where a=ndf/2 and x=chi2/2. P(a,x) represents the probability that the observed Chi-squared
//! for a correct model should be less than the value chi2.
///
//! The returned probability corresponds to 1-P(a,x),
//! which denotes the probability that an observed Chi-squared exceeds
//! the value chi2 by chance, even for a correct model.
///
//! \author NvE 14-nov-1998 UU-SAP Utrecht
//!
double  chiSquareCdfC(double x, double ndf, double x0) throw (MathException)
{
  double arg = x - x0;
  if (arg<0)    throw MathException("chiSquarePdf() (x-x0) <  0");
  if (ndf <= 0) throw MathException("chiSquareCdfC() ndf <= 0");
  if (arg == 0) return 1;
  return gammaIncomplete_c( 0.5*ndf, 0.5*chi2 );

}

//!
//! Evaluate the quantiles of the chi-squared probability distribution function.
//! Algorithm AS 91   Appl. Statist. (1975) Vol.24, P.35
//! .
//! Incorporates the suggested changes in AS R85 (vol.40(1), pp.233-5, 1991)
///
//! \param[in] p     the probability value, at which the quantile is computed
//! \param[in] ndf   number of degrees of freedom
///
//! \author Anna Kreshuk
//!
double  chiSquareQuantile(double  p, double  ndf) throw (MathException)
{
  double  c[]={0, 0.01, 0.222222, 0.32, 0.4, 1.24, 2.2,
    4.67, 6.66, 6.73, 13.32, 60.0, 70.0,
    84.0, 105.0, 120.0, 127.0, 140.0, 175.0,
    210.0, 252.0, 264.0, 294.0, 346.0, 420.0,
    462.0, 606.0, 672.0, 707.0, 735.0, 889.0,
    932.0, 966.0, 1141.0, 1182.0, 1278.0, 1740.0,
    2520.0, 5040.0};
  double  e = 5e-7;
  double  aa = 0.6931471806;
  int  maxit = 20;
  double  ch, p1, p2, q, t, a, b, x;
  double  s1, s2, s3, s4, s5, s6;

  if (ndf <= 0)  throw MathException("chiSquareQuantile() ndf <= 0");
  double  g = lnGamma(0.5*ndf);
  double  xx = 0.5 * ndf;
  double  cp = xx - 1;
  if (ndf >= log(p)*(-c[5]))
    {
    //starting approximation for ndf less than or equal to 0.32
    if (ndf > c[3]) {
      x = normQuantile(p);
      //starting approximation using Wilson and Hilferty estimate
      p1=c[2]/ndf;
      ch = ndf*power((x*sqrt(p1) + 1 - p1), 3);
      if (ch > c[6]*ndf + 6)
        ch = -2 * (log(1-p) - cp * log(0.5 * ch) + g);
    } else {
      ch = c[4];
      a = log(1-p);
      do{
        q = ch;
        p1 = 1 + ch * (c[7]+ch);
        p2 = ch * (c[9] + ch * (c[8] + ch));
        t = -0.5 + (c[7] + 2 * ch) / p1 - (c[9] + ch * (c[10] + 3 * ch)) / p2;
        ch = ch - (1 - exp(a + g + 0.5 * ch + cp * aa) *p2 / p1) / t;
      }while (absolute(q/ch - 1) > c[1]);
    }
  } else {
    ch = power((p * xx * exp(g + xx * aa)),(1./xx));
    if (ch < e) return ch;
  }
  //call to algorithm AS 239 and calculation of seven term  Taylor series
  for (int  i=0; i<maxit; i++){
    q = ch;
    p1 = 0.5 * ch;
    p2 = p - gamma(xx, p1);

    t = p2 * exp(xx * aa + g + p1 - cp * log(ch));
    b = t / ch;
    a = 0.5 * t - b * cp;
    s1 = (c[19] + a * (c[17] + a * (c[14] + a * (c[13] + a * (c[12] +c[11] * a))))) / c[24];
    s2 = (c[24] + a * (c[29] + a * (c[32] + a * (c[33] + c[35] * a)))) / c[37];
    s3 = (c[19] + a * (c[25] + a * (c[28] + c[31] * a))) / c[37];
    s4 = (c[20] + a * (c[27] + c[34] * a) + cp * (c[22] + a * (c[30] + c[36] * a))) / c[38];
    s5 = (c[13] + c[21] * a + cp * (c[18] + c[26] * a)) / c[37];
    s6 = (c[15] + cp * (c[23] + c[16] * cp)) / c[38];
    ch = ch + t * (1 + 0.5 * t * s1 - b * cp * (s1 - b * (s2 - b * (s3 - b * (s4 - b * (s5 - b * s6))))));
    if (absolute(q / ch - 1) > e) break;
  }
  return ch;
}

//!
//! Landau PDF.
//! Algorithm from CERNLIB G110 denlan
//! Same algorithm is used in GSL
//!
//! \param[in] x    value where the PDF is evaluated
//! \param[in] xi   width parameter
//! \param[in] x0   optional  location  parameter
//!
double landauPdf(double x, double xi, double x0) throw (MathException)
{
  static double p1[5] = {0.4259894875,-0.1249762550, 0.03984243700, -0.006298287635,   0.001511162253};
  static double q1[5] = {1.0         ,-0.3388260629, 0.09594393323, -0.01608042283,    0.003778942063};

  static double p2[5] = {0.1788541609, 0.1173957403, 0.01488850518, -0.001394989411,   0.0001283617211};
  static double q2[5] = {1.0         , 0.7428795082, 0.3153932961,   0.06694219548,    0.008790609714};

  static double p3[5] = {0.1788544503, 0.09359161662,0.006325387654, 0.00006611667319,-0.000002031049101};
  static double q3[5] = {1.0         , 0.6097809921, 0.2560616665,   0.04746722384,    0.006957301675};

  static double p4[5] = {0.9874054407, 118.6723273,  849.2794360,   -743.7792444,      427.0262186};
  static double q4[5] = {1.0         , 106.8615961,  337.6496214,    2016.712389,      1597.063511};

  static double p5[5] = {1.003675074,  167.5702434,  4789.711289,    21217.86767,     -22324.94910};
  static double q5[5] = {1.0         , 156.9424537,  3745.310488,    9834.698876,      66924.28357};

  static double p6[5] = {1.000827619,  664.9143136,  62972.92665,    475554.6998,     -5743609.109};
  static double q6[5] = {1.0         , 651.4101098,  56974.73333,    165917.4725,     -2815759.939};

  static double a1[3] = {0.04166666667,-0.01996527778, 0.02709538966};

  static double a2[2] = {-1.845568670,-4.284640743};

  if (xi <= 0) throw MathException("landauPdf() xi <= 0");
  double v = (x - x0)/xi;
  double u, ue, us, denlan;
  if (v < -5.5) {
    u   = std::exp(v+1.0);
    if (u < 1e-10) return 0.0;
    ue  = std::exp(-1/u);
    us  = std::sqrt(u);
    denlan = 0.3989422803*(ue/us)*(1+(a1[0]+(a1[1]+a1[2]*u)*u)*u);
  } else if(v < -1) {
    u   = std::exp(-v-1);
    denlan = std::exp(-u)*std::sqrt(u)*
    (p1[0]+(p1[1]+(p1[2]+(p1[3]+p1[4]*v)*v)*v)*v)/
    (q1[0]+(q1[1]+(q1[2]+(q1[3]+q1[4]*v)*v)*v)*v);
  } else if(v < 1) {
    denlan = (p2[0]+(p2[1]+(p2[2]+(p2[3]+p2[4]*v)*v)*v)*v)/
    (q2[0]+(q2[1]+(q2[2]+(q2[3]+q2[4]*v)*v)*v)*v);
  } else if(v < 5) {
    denlan = (p3[0]+(p3[1]+(p3[2]+(p3[3]+p3[4]*v)*v)*v)*v)/
    (q3[0]+(q3[1]+(q3[2]+(q3[3]+q3[4]*v)*v)*v)*v);
  } else if(v < 12) {
    u   = 1/v;
    denlan = u*u*(p4[0]+(p4[1]+(p4[2]+(p4[3]+p4[4]*u)*u)*u)*u)/
    (q4[0]+(q4[1]+(q4[2]+(q4[3]+q4[4]*u)*u)*u)*u);
  } else if(v < 50) {
    u   = 1/v;
    denlan = u*u*(p5[0]+(p5[1]+(p5[2]+(p5[3]+p5[4]*u)*u)*u)*u)/
    (q5[0]+(q5[1]+(q5[2]+(q5[3]+q5[4]*u)*u)*u)*u);
  } else if(v < 300) {
    u   = 1/v;
    denlan = u*u*(p6[0]+(p6[1]+(p6[2]+(p6[3]+p6[4]*u)*u)*u)*u)/
    (q6[0]+(q6[1]+(q6[2]+(q6[3]+q6[4]*u)*u)*u)*u);
  } else {
    u   = 1/(v-v*std::log(v)/(v+1));
    denlan = u*u*(1+(a2[0]+a2[1]*u)*u);
  }
  return denlan/xi;

}

//!
//! Landau CDF.
//! Algorithm from CERNLIB G110 denlan
//! Same algorithm is used in GSL
//!
//! \param[in] x    value where the PDF is evaluated
//! \param[in] xi   width parameter
//! \param[in] x0   optional  location  parameter
//!
double landauCdf(double x, double xi, double x0)
{
  // implementation of landau distribution (from DISLAN)
  //The algorithm was taken from the Cernlib function dislan(G110)
  //Reference: K.S.Kolbig and B.Schorr, "A program package for the Landau
  //distribution", Computer Phys.Comm., 31(1984), 97-111

  static double p1[5] = {0.2514091491e+0,-0.6250580444e-1, 0.1458381230e-1,-0.2108817737e-2, 0.7411247290e-3};
  static double q1[5] = {1.0            ,-0.5571175625e-2, 0.6225310236e-1,-0.3137378427e-2, 0.1931496439e-2};

  static double p2[4] = {0.2868328584e+0, 0.3564363231e+0, 0.1523518695e+0, 0.2251304883e-1};
  static double q2[4] = {1.0            , 0.6191136137e+0, 0.1720721448e+0, 0.2278594771e-1};

  static double p3[4] = {0.2868329066e+0, 0.3003828436e+0, 0.9950951941e-1, 0.8733827185e-2};
  static double q3[4] = {1.0            , 0.4237190502e+0, 0.1095631512e+0, 0.8693851567e-2};

  static double p4[4] = {0.1000351630e+1, 0.4503592498e+1, 0.1085883880e+2, 0.7536052269e+1};
  static double q4[4] = {1.0            , 0.5539969678e+1, 0.1933581111e+2, 0.2721321508e+2};

  static double p5[4] = {0.1000006517e+1, 0.4909414111e+2, 0.8505544753e+2, 0.1532153455e+3};
  static double q5[4] = {1.0            , 0.5009928881e+2, 0.1399819104e+3, 0.4200002909e+3};

  static double p6[4] = {0.1000000983e+1, 0.1329868456e+3, 0.9162149244e+3,-0.9605054274e+3};
  static double q6[4] = {1.0            , 0.1339887843e+3, 0.1055990413e+4, 0.5532224619e+3};

  static double a1[4] = {0              ,-0.4583333333e+0, 0.6675347222e+0,-0.1641741416e+1};
  static double a2[4] = {0              , 1.0            ,-0.4227843351e+0,-0.2043403138e+1};

  double v = (x - x0)/xi;
  double u;
  double lan;

  if (v < -5.5)
    {
    u   = std::exp(v+1);
    lan = 0.3989422803*std::exp(-1./u)*std::sqrt(u)*(1+(a1[1]+(a1[2]+a1[3]*u)*u)*u);
    }
  else if (v < -1 )
    {
    u   = std::exp(-v-1);
    lan = (std::exp(-u)/std::sqrt(u))*(p1[0]+(p1[1]+(p1[2]+(p1[3]+p1[4]*v)*v)*v)*v)/
    (q1[0]+(q1[1]+(q1[2]+(q1[3]+q1[4]*v)*v)*v)*v);
    }
  else if (v < 1)
    lan = (p2[0]+(p2[1]+(p2[2]+p2[3]*v)*v)*v)/(q2[0]+(q2[1]+(q2[2]+q2[3]*v)*v)*v);

  else if (v < 4)
    lan = (p3[0]+(p3[1]+(p3[2]+p3[3]*v)*v)*v)/(q3[0]+(q3[1]+(q3[2]+q3[3]*v)*v)*v);

  else if (v < 12)
    {
    u   = 1./v;
    lan = (p4[0]+(p4[1]+(p4[2]+p4[3]*u)*u)*u)/(q4[0]+(q4[1]+(q4[2]+q4[3]*u)*u)*u);
    }
  else if (v < 50)
    {
    u   = 1./v;
    lan = (p5[0]+(p5[1]+(p5[2]+p5[3]*u)*u)*u)/(q5[0]+(q5[1]+(q5[2]+q5[3]*u)*u)*u);
    }
  else if (v < 300)
    {
    u   = 1./v;
    lan = (p6[0]+(p6[1]+(p6[2]+p6[3]*u)*u)*u)/(q6[0]+(q6[1]+(q6[2]+q6[3]*u)*u)*u);
    }
  else
    {
    u   = 1./(v-v*std::log(v)/(v+1));
    lan = 1-(a2[1]+(a2[2]+a2[3]*u)*u)*u;
    }
  return lan;
}


double landauXm1(double x, double xi, double x0)
{
  // implementation of first moment  of the Landau distribution
  // translated from Cernlib (XM1LAN function) by Benno List

  static double p1[5] = {-0.8949374280E+0, 0.4631783434E+0,-0.4053332915E-1,
    0.1580075560E-1,-0.3423874194E-2};
  static double q1[5] = { 1.0            , 0.1002930749E+0, 0.3575271633E-1,
    -0.1915882099E-2, 0.4811072364E-4};
  static double p2[5] = {-0.8933384046E+0, 0.1161296496E+0, 0.1200082940E+0,
    0.2185699725E-1, 0.2128892058E-2};
  static double q2[5] = { 1.0            , 0.4935531886E+0, 0.1066347067E+0,
    0.1250161833E-1, 0.5494243254E-3};
  static double p3[5] = {-0.8933322067E+0, 0.2339544896E+0, 0.8257653222E-1,
    0.1411226998E-1, 0.2892240953E-3};
  static double q3[5] = { 1.0            , 0.3616538408E+0, 0.6628026743E-1,
    0.4839298984E-2, 0.5248310361E-4};
  static double p4[4] = { 0.9358419425E+0, 0.6716831438E+2,-0.6765069077E+3,
    0.9026661865E+3};
  static double q4[4] = { 1.0            , 0.7752562854E+2,-0.5637811998E+3,
    -0.5513156752E+3};
  static double p5[4] = { 0.9489335583E+0, 0.5561246706E+3, 0.3208274617E+5,
    -0.4889926524E+5};
  static double q5[4] = { 1.0            , 0.6028275940E+3, 0.3716962017E+5,
    0.3686272898E+5};
  static double a0[6] = {-0.4227843351E+0,-0.1544313298E+0, 0.4227843351E+0,
    0.3276496874E+1, 0.2043403138E+1,-0.8681296500E+1};
  static double a1[4] = { 0,              -0.4583333333E+0, 0.6675347222E+0,
    -0.1641741416E+1};
  static double a2[5] = { 0,              -0.1958333333E+1, 0.5563368056E+1,
    -0.2111352961E+2, 0.1006946266E+3};

  double v = (x-x0)/xi;
  double xm1lan;
  if (v < -4.5)
    {
    double u = std::exp(v+1);
    xm1lan   = v-u*(1+(a2[1]+(a2[2]+(a2[3]+a2[4]*u)*u)*u)*u)/
    (1+(a1[1]+(a1[2]+a1[3]*u)*u)*u);
    }
  else if (v < -2)
    {
    xm1lan   = (p1[0]+(p1[1]+(p1[2]+(p1[3]+p1[4]*v)*v)*v)*v)/
    (q1[0]+(q1[1]+(q1[2]+(q1[3]+q1[4]*v)*v)*v)*v);
    }
  else if (v < 2)
    {
    xm1lan   = (p2[0]+(p2[1]+(p2[2]+(p2[3]+p2[4]*v)*v)*v)*v)/
    (q2[0]+(q2[1]+(q2[2]+(q2[3]+q2[4]*v)*v)*v)*v);
    }
  else if (v < 10)
    {
    xm1lan   = (p3[0]+(p3[1]+(p3[2]+(p3[3]+p3[4]*v)*v)*v)*v)/
    (q3[0]+(q3[1]+(q3[2]+(q3[3]+q3[4]*v)*v)*v)*v);
    }
  else if (v < 40)
    {
    double u = 1/v;
    xm1lan   = std::log(v)*(p4[0]+(p4[1]+(p4[2]+p4[3]*u)*u)*u)/
    (q4[0]+(q4[1]+(q4[2]+q4[3]*u)*u)*u);
    }
  else if (v < 200)
    {
    double u = 1/v;
    xm1lan   = std::log(v)*(p5[0]+(p5[1]+(p5[2]+p5[3]*u)*u)*u)/
    (q5[0]+(q5[1]+(q5[2]+q5[3]*u)*u)*u);
    }
  else
    {
    double u = v-v*std::log(v)/(v+1);
    v        = 1/(u-u*(u+ std::log(u)-v)/(u+1));
    u        = -std::log(v);
    xm1lan   = (u+a0[0]+(-u+a0[1]+(a0[2]*u+a0[3]+(a0[4]*u+a0[5])*v)*v)*v)/
    (1-(1-(a0[2]+a0[4]*v)*v)*v);
    }
  return xm1lan*xi + x0;
}

double landauXm2(double x, double xi, double x0)
{
  // implementation of second moment of Landau distribution
  // translated from Cernlib (XM2LAN function) by Benno List

  static double p1[5] = { 0.1169837582E+1,-0.4834874539E+0, 0.4383774644E+0,
    0.3287175228E-2, 0.1879129206E-1};
  static double q1[5] = { 1.0            , 0.1795154326E+0, 0.4612795899E-1,
    0.2183459337E-2, 0.7226623623E-4};
  static double p2[5] = { 0.1157939823E+1,-0.3842809495E+0, 0.3317532899E+0,
    0.3547606781E-1, 0.6725645279E-2};
  static double q2[5] = { 1.0            , 0.2916824021E+0, 0.5259853480E-1,
    0.3840011061E-2, 0.9950324173E-4};
  static double p3[4] = { 0.1178191282E+1, 0.1011623342E+2,-0.1285585291E+2,
    0.3641361437E+2};
  static double q3[4] = { 1.0            , 0.8614160194E+1, 0.3118929630E+2,
    0.1514351300E+0};
  static double p4[5] = { 0.1030763698E+1, 0.1216758660E+3, 0.1637431386E+4,
    -0.2171466507E+4, 0.7010168358E+4};
  static double q4[5] = { 1.0            , 0.1022487911E+3, 0.1377646350E+4,
    0.3699184961E+4, 0.4251315610E+4};
  static double p5[4] = { 0.1010084827E+1, 0.3944224824E+3, 0.1773025353E+5,
    -0.7075963938E+5};
  static double q5[4] = { 1.0            , 0.3605950254E+3, 0.1392784158E+5,
    -0.1881680027E+5};
  static double a0[7] = {-0.2043403138E+1,-0.8455686702E+0,-0.3088626596E+0,
    0.5821346754E+1, 0.4227843351E+0, 0.6552993748E+1,
    -0.1076714945E+2};
  static double a1[4] = { 0.             ,-0.4583333333E+0, 0.6675347222E+0,
    -0.1641741416E+1};
  static double a2[4] = {-0.1958333333E+1, 0.5563368056E+1,-0.2111352961E+2,
    0.1006946266E+3};
  static double a3[4] = {-1.0            , 0.4458333333E+1,-0.2116753472E+2,
    0.1163674359E+3};

  double v    = (x-x0)/xi;
  double xm2lan;
  if (v < -4.5)
    {
    double u = std::exp(v+1);
    xm2lan   = v*v-2*u*u*
    (v/u+a2[0]*v+a3[0]+(a2[1]*v+a3[1]+(a2[2]*v+a3[2]+
                                       (a2[3]*v+a3[3])*u)*u)*u)/
    (1+(a1[1]+(a1[2]+a1[3]*u)*u)*u);
    }
  else if (v < -2)
    {
    xm2lan   = (p1[0]+(p1[1]+(p1[2]+(p1[3]+p1[4]*v)*v)*v)*v)/
    (q1[0]+(q1[1]+(q1[2]+(q1[3]+q1[4]*v)*v)*v)*v);
    }
  else if (v < 2)
    {
    xm2lan   = (p2[0]+(p2[1]+(p2[2]+(p2[3]+p2[4]*v)*v)*v)*v)/
    (q2[0]+(q2[1]+(q2[2]+(q2[3]+q2[4]*v)*v)*v)*v);
    }
  else if (v < 5)
    {
    double u = 1/v;
    xm2lan   = v*(p3[0]+(p3[1]+(p3[2]+p3[3]*u)*u)*u)/
    (q3[0]+(q3[1]+(q3[2]+q3[3]*u)*u)*u);
    }
  else if (v < 50)
    {
    double u = 1/v;
    xm2lan   = v*(p4[0]+(p4[1]+(p4[2]+(p4[3]+p4[4]*u)*u)*u)*u)/
    (q4[0]+(q4[1]+(q4[2]+(q4[3]+q4[4]*u)*u)*u)*u);
    }
  else if (v < 200)
    {
    double u = 1/v;
    xm2lan   = v*(p5[0]+(p5[1]+(p5[2]+p5[3]*u)*u)*u)/
    (q5[0]+(q5[1]+(q5[2]+q5[3]*u)*u)*u);
    }
  else
    {
    double u = v-v*std::log(v)/(v+1);
    v        = 1/(u-u*(u+log(u)-v)/(u+1));
    u        = -std::log(v);
    xm2lan   = (1/v+u*u+a0[0]+a0[1]*u+(-u*u+a0[2]*u+a0[3]+
                                       (a0[4]*u*u+a0[5]*u+a0[6])*v)*v)/(1-(1-a0[4]*v)*v);
    }
  if (x0 == 0) return xm2lan*xi*xi;
  double xm1lan = ROOT::Math::landau_xm1(x, xi, x0);
  return xm2lan*xi*xi + (2*xm1lan-x0)*x0;
}

//!
//! Computes the poisson distribution function for (x,par).
//! The poisson PDF is implemented by means of Euler's gamma-function
//! (for the factorial), so for any x integer argument it is the correct poisson distribution.
//! BUT for non-integer x values, it IS NOT equal to the poisson distribution !
//!
//! \param[in] x    value where the PDF is evaluated
//! \param[in] mean   mean of the distribution
//!
double  poissonPdf(double  x, double  mean) throw MathException
{
  if (mean < 0) throw MathException();
  if (x < 0)
    return 0;
  else if (x == 0.0 )
    return exp(-mean);
  else
    return exp( x * log(mean) - lnGamma(x + 1.) - mean);
}

//!
//! Computes the Discrete poisson distribution function for (x,par).
//! This is a discrete and a non-smooth function.
//!
double  poissonI(double  x, double  par)
{
  int  ix = int (x);
  return poisson(ix,par);
}



double poissonCdf(unsigned int n, double mu)
{
  // mu must be >= 0  . Use poisson - gamma relation
  //  Pr ( x <= n) = Pr( y >= a)   where x is poisson and y is gamma distributed ( a = n+1)
  double a = (double) n + 1.0;
  return gamma_cdf_c(mu, a, 1.0);
}


double poissonCdfC(unsigned int n, double mu)
{
  // mu must be >= 0  . Use poisson - gamma relation
  //  Pr ( x <= n) = Pr( y >= a)   where x is poisson and y is gamma distributed ( a = n+1)
  double a = (double) n + 1.0;
  return gamma_cdf(mu, a, 1.0);
}


//!
//! Computes the density function of F-distribution
//! (probability function, integral of density, is computed in fCdf).
///
//! Parameters N and M stand for degrees of freedom of chi-squares
//! mentioned above parameter F is the actual variable x of the
//! density function p(x) and the point at which the density function
//! is calculated.
///
//! ### About F distribution:
//! F-distribution arises in testing whether two random samples
//! have the same variance. It is the ratio of two chi-square
//! distributions, with N and M degrees of freedom respectively,
//! where each chi-square is first divided by it's number of degrees
//! of freedom.
//!
double  fDistributionPdf(double x, double n, double m, double x0)   throw (MathException)
{
  return fdistribution_pdf(F,N,M);
}

//!
//! Calculates the cumulative distribution function of F-distribution,
//! this function occurs in the statistical test of whether two observed
//! samples have the same variance. For this test a certain statistic F,
//! the ratio of observed dispersion of the first sample to that of the
//! second sample, is calculated. N and M stand for numbers of degrees
//! of freedom in the samples 1-fCdf() is the significance level at
//! which the hypothesis "1 has smaller variance than 2" can be rejected.
//! A small numerical value of 1 - fCdf() implies a very significant
//! rejection, in turn implying high confidence in the hypothesis
//! "1 has variance greater than 2".
//!
double  fDistributionCdf(double x, double n, double m, double x0)  throw (MathException)
{
  // f distribution  is defined only for both n and m > 0
  if (n < 0 || m < 0) throw MathException("fDistributionCdf() n < 0 || m < 0");
  double z = n*(x-x0)/(m + n*(x-x0));
  if (z > 0.9 && n > 1 && m > 1) return 1.0 - fdistributionCdfC(x,n,m,x0);
  return inc_beta(z, 0.5*n, 0.5*m);
}

double fDistributionCdfC(double x, double n, double m, double x0)   throw (MathException)
{
  // f distribution  is defined only for both n and m > 0
  if (n < 0 || m < 0) throw MathException("fDistributionCdfC() n < 0 || m < 0");
  double z = m/(m + n*(x-x0));
  if (z > 0.9 && n > 1 && m > 1) return 1.0-  fdistributionCdf(x,n,m,x0);
  // for the complement use the fact that IB(x,a,b) = 1. - IB(1-x,b,a)
  return inc_beta(m/(m + n*(x-x0)), 0.5*m, 0.5*n);
}

//!
//! Calculates the Kolmogorov distribution function,
///
//! \f[ P(z) = 2 \sum_{j=1}^{\infty} (-1)^{j-1} e^{-2 j^2 z^2}  \f]
///
//! which gives the probability that Kolmogorov's test statistic will exceed
//! the value z assuming the null hypothesis. This gives a very powerful
//! test for comparing two one-dimensional distributions.
//! see, for example, Eadie et al, "statistical Methods in experimental
//! Physics', pp 269-270).
///
//! This function returns the confidence level for the null hypothesis, where:
//!  - \f$ z = dn \sqrt{n} \f$, and
//!     - \f$ dn \f$  is the maximum deviation between a hypothetical distribution
//!           function and an experimental distribution with
//!     - \f$ n \f$  events
///
//! NOTE: To compare two experimental distributions with m and n events,
//!       use \f$ z = \sqrt{m n/(m+n)) dn} \f$
///
//! Accuracy: The function is far too accurate for any imaginable application.
//!           Probabilities less than \f$ 10^{-15} \f$ are returned as zero.
//!           However, remember that the formula is only valid for "large" n.
//!
//! Theta function inversion formula is used for z <= 1
//!
double  KolmogorovProb(double  z)
{
  double  fj[4] = {-2,-8,-18,-32}, r[4];
  const double  w = 2.50662827;
  // c1 - -pi**2/8, c2 = 9*c1, c3 = 25*c1
  const double  c1 = -1.2337005501361697;
  const double  c2 = -11.103304951225528;
  const double  c3 = -30.842513753404244;

  double  u = absolute(z);
  double  p;
  if (u < 0.2) {
    p = 1;
  } else if (u < 0.755) {
    double  v = 1./(u*u);
    p = 1 - w*(exp(c1*v) + exp(c2*v) + exp(c3*v))/u;
  } else if (u < 6.8116) {
    r[1] = 0;
    r[2] = 0;
    r[3] = 0;
    double  v = u*u;
    int  maxj = Max(1,Nint(3./u));
    for (int  j=0; j<maxj;j++) {
      r[j] = exp(fj[j]*v);
    }
    p = 2*(r[0] - r[1] +r[2] - r[3]);
  } else {
    p = 0;
  }
  return p;
}

//!
//!  Statistical test whether two one-dimensional sets of points are compatible
//!  with coming from the same parent distribution, using the Kolmogorov test.
//!  That is, it is used to compare two experimental distributions of unbinned data.
///
//! ### Input:
//!  a,b: One-dimensional arrays of length na, nb, respectively.
//!       The elements of a and b must be given in ascending order.
//!  option is a character string to specify options
//!         "D" Put out a line of "Debug" printout
//!         "M" Return the Maximum Kolmogorov distance instead of prob
///
//! ### Output:
//! The returned value prob is a calculated confidence level which gives a
//! statistical test for compatibility of a and b.
//! Values of prob close to zero are taken as indicating a small probability
//! of compatibility. For two point sets drawn randomly from the same parent
//! distribution, the value of prob should be uniformly distributed between
//! zero and one.
//!   in case of error the function return -1
//!   If the 2 sets have a different number of points, the minimum of
//!   the two sets is used.
///
//! ### Method:
//! The Kolmogorov test is used. The test statistic is the maximum deviation
//! between the two integrated distribution functions, multiplied by the
//! normalizing factor (rdmax*sqrt(na*nb/(na+nb)).
///
//!  Code adapted by Rene Brun from CERNLIB routine TKOLMO (Fred James)
//!   (W.T. Eadie, D. Drijard, F.E. James, M. Roos and B. Sadoulet,
//!      Statistical Methods in experimental Physics, (North-Holland,
//!      Amsterdam 1971) 269-271)
///
//! ### Method Improvement by Jason A Detwiler (JADetwiler@lbl.gov)
///
//!   The nuts-and-bolts of the KolmogorovTest() algorithm is a for-loop
//!   over the two sorted arrays a and b representing empirical distribution
//!   functions. The for-loop handles 3 cases: when the next points to be
//!   evaluated satisfy a>b, a<b, or a=b:
//!   For the last case, a=b, the algorithm advances each array by one index in an
//!   attempt to move through the equality. However, this is incorrect when one or
//!   the other of a or b (or both) have a repeated value, call it x. For the KS
//!   statistic to be computed properly, rdiff needs to be calculated after all of
//!   the a and b at x have been tallied (this is due to the definition of the
//!   empirical distribution function; another way to convince yourself that the
//!   old CERNLIB method is wrong is that it implies that the function defined as the
//!   difference between a and b is multi-valued at x -- besides being ugly, this
//!   would invalidate Kolmogorov's theorem).
//!   A good description of the Kolmogorov test can be found  at:
//!    http://www.itl.nist.gov/div898/handbook/eda/section3/eda35g.htm
//!
double  KolmogorovTest(int  na, const double  *a, int  nb, const double  *b, const char *option) throw (MathException)
{
//  TString opt = option;
//  opt.ToUpper();
  double  prob = -1;
  if (!a || !b || na <= 2 || nb <= 2) throw  MathException("KolmogorovTest() Sets must have more than 2 points ")
  //     Constants needed
  double  rna = na;
  double  rnb = nb;
  double  sa  = 1./rna;
  double  sb  = 1./rnb;
  double  rdiff = 0;
  double  rdmax = 0;
  int  ia = 0;
  int  ib = 0;

  //    Main loop over point sets to find max distance
  //    rdiff is the running difference, and rdmax the max.
  bool ok = false;
  for (int  i=0;i<na+nb;i++) {
    if (a[ia] < b[ib]) {
      rdiff -= sa;
      ia++;
      if (ia >= na) {ok = true; break;}
    } else if (a[ia] > b[ib]) {
      rdiff += sb;
      ib++;
      if (ib >= nb) {ok = true; break;}
    } else {
      // special cases for the ties
      double x = a[ia];
      while(ia < na && a[ia] == x) {
        rdiff -= sa;
        ia++;
      }
      while(ib < nb && b[ib] == x) {
        rdiff += sb;
        ib++;
      }
      if (ia >= na) {ok = true; break;}
      if (ib >= nb) {ok = true; break;}
    }
    rdmax = Max(rdmax,absolute(rdiff));
  }
  //    Should never terminate this loop with ok = false!

  if (ok)
    {
    rdmax = Max(rdmax,absolute(rdiff));
    double  z = rdmax * sqrt(rna*rnb/(rna+rnb));
    prob = KolmogorovProb(z);
  }
  // debug printout
//  if (opt.Contains("D")) {
//    printf(" Kolmogorov Probability = %g, Max Dist = %g\n",prob,rdmax);
//  }
//  if(opt.Contains("M")) return rdmax;
//  else                  return prob;
}

//!
//! Computes the probability density function (PDF) of the Laplace distribution
//! at point x, with location parameter alpha and shape parameter beta.
//! By default, alpha=0, beta=1.
//!
//! This distribution is known under different names, most common is
//! double exponential distribution, but it also known as
//! two-tailed exponential or the bilateral exponential distribution.
//!
double  laplacePdf(double  x, double  alpha, double  beta)  throw (MathException)
{
  if (beta<=0.0) throw MathException("laplacePdf() beta<=0.0");
  double arg = x-alpha;
  return exp(-absolute(arg/beta)) / (2.*beta);
}

//!
//! Computes the cumulative  density function (CDF) of the Laplace distribution
//! at point x, with location parameter alpha and shape parameter beta.
//! By default, alpha=0, beta=1.
//!
double  laplaceCdf(double  x, double  alpha, double  beta)  throw (MathException)
{
  if (beta<=0.0) throw MathException("laplaceCdf() beta<=0.0");
  double arg = x-alpha;
  if (arg<=0.0)
    return 0.5*exp(arg/beta);
  else
    return 1.0 - 0.5*exp(-arg/beta);
}

//!
//! Computes the complement of the cumulative  density function (CDFC) of the Laplace distribution
//! at point x, with location parameter alpha and shape parameter beta.
//! By default, alpha=0, beta=1.
//!
double  laplaceCdfC(double  x, double  alpha, double  beta)  throw (MathException)
{
  if (beta<=0.0) throw MathException("laplaceCdfC() beta<=0.0");
  double arg = x-alpha;
  if (arg<=0.0)
    return 1.0 -0.5*exp(arg/beta);
  else
    return 0.5*exp(-arg/beta);
}

double laplaceQuantile(double F, double alpha=0, double beta=1)  throw (MathException);
{
  if (F<=0.0 || F>1.0) throw MathException("laplaceQuantile() F<=0.0  || F>1.0");
  if (beta<=0.0) throw MathException("laplaceQuantile() beta<=0.0");
  if (F<=0.5)
    return alpha + beta*std::log(2.0*F);
  else
    return alpha - beta*std::log(2.0 - 2.0*F);
}

//!
//! Computes density function for Student's t- distribution
//! (the probability function (integral of density) is computed in studentCdf).
///
//! First parameter stands for x - the actual variable of the
//! density function p(x) and the point at which the density is calculated.
//! Second parameter stands for number of degrees of freedom.
///
//! About studentPdf distribution:
//! Student's t-distribution is used for many significance tests, for example,
//! for the Student's t-tests for the statistical significance of difference
//! between two sample means and for confidence intervals for the difference
//! between two population means.
///
//! Example: suppose we have a random sample of size n drawn from normal
//! distribution with mean Mu and st.deviation Sigma. Then the variable
///
//!   t = (sample_mean - Mu)/(sample_deviation / sqrt(n))
///
//! has Student's t-distribution with n-1 degrees of freedom.
///
//! NOTE that this function's second argument is number of degrees of freedom,
//! not the sample size.
///
//! As the number of degrees of freedom grows, t-distribution approaches
//! Normal(0,1) distribution.
//!
double studentPdf(double  T, double  ndf)   throw (MathException);
{
  if (ndf < 1) throw MathException("studentPdf() ndf < 1");
  double  r   = ndf;
  double  rh  = 0.5*r;
  double  rh1 = rh + 0.5;
  double  denom = sqrt(r*Pi())*gamma(rh)*power(1+T*T/r, rh1);
  return gamma(rh1)/denom;
}

//!
//! Calculates the cumulative distribution function of Student's
//! t-distribution second parameter stands for number of degrees of freedom,
//! not for the number of samples
//! if x has Student's t-distribution, the function returns the probability of
//! x being less than T.
//!
double studentCdf(double x, double r, double x0)  throw (MathException)
{
  double p    = x-x0;
  double sign = (p>0) ? 1.0 : -1.0;
  return 0.5 + 0.5*betaIncomplete(p*p/(r + p*p), 0.5, 0.5*r )*sign;
}

double studentCdfC(double x, double r, double x0)  throw (MathException)
{
  double p    = x-x0;
  double sign = (p>0) ? 1.0 : -1.0;
  return 0.5 + 0.5*betaIncomplete(p*p/(r + p*p),  0.5, 0.5*r )*sign;
}

//!
//! Computes quantiles of the Student's t-distribution
//! 1st argument is the probability, at which the quantile is computed
//! 2nd argument - the number of degrees of freedom of the
//! studentPdf distribution
//! When the 3rd argument lower_tail is true (default)-
//! the algorithm returns such x0, that
///
//!   P(x < x0)=p
///
//! upper tail (lower_tail is false)- the algorithm returns such x0, that
///
//!   P(x > x0)=p
///
//! the algorithm was taken from:
//!   G.W.Hill, "Algorithm 396, Student's t-quantiles"
//!             "Communications of the ACM", 13(10), October 1970

double studentQuantile(double  p, double  ndf, bool lower_tail)
{
  double  quantile;
  double  temp;
  bool neg;
  double  q;
  if (ndf<1 || p>=1 || p<=0) {
    Error("studentQuantile", "illegal parameter values");
    return 0;
  }
  if ((lower_tail && p>0.5)||(!lower_tail && p<0.5)){
    neg=false;
    q=2*(lower_tail ? (1-p) : p);
  } else {
    neg=true;
    q=2*(lower_tail? p : (1-p));
  }

  if ((ndf-1)<1e-8) {
    temp=PiOver2()*q;
    quantile = Cos(temp)/Sin(temp);
  } else {
    if ((ndf-2)<1e-8) {
      quantile = sqrt(2./(q*(2-q))-2);
    } else {
      double  a=1./(ndf-0.5);
      double  b=48./(a*a);
      double  c=((20700*a/b -98)*a-16)*a+96.36;
      double  d=((94.5/(b+c)-3.)/b+1)*sqrt(a*PiOver2())*ndf;
      double  x=q*d;
      double  y=power(x, (2./ndf));
      if (y>0.05+a){
        //asymptotic inverse expansion about normal
        x=normQuantile(q*0.5);
        y=x*x;
        if (ndf<5) c+=0.3*(ndf-4.5)*(x+0.6);
        c+=(((0.05*d*x-5.)*x-7.)*x-2.)*x +b;
        y=(((((0.4*y+6.3)*y+36.)*y+94.5)/c - y-3.)/b+1)*x;
        y=a*y*y;
        if(y>0.002) y  = exp(y)-1;
        else        y += 0.5*y*y;
      } else {
        y=((1./(((ndf+6.)/(ndf*y)-0.089*d-0.822)*(ndf+2.)*3)+0.5/(ndf+4.))*y-1.)*
        (ndf+1.)/(ndf+2.)+1/y;
      }
      quantile = sqrt(ndf*y);
    }
  }
  if(neg) quantile=-quantile;
  return quantile;
}

//!
//! Returns the value of the vavilovPdf density function
///
//! \param[in] x      the point were the density function is evaluated
//! \param[in] kappa  value of kappa (distribution parameter)
//! \param[in] beta2  value of beta2 (distribution parameter)
///
//! The algorithm was taken from the CernLib function vavden(G115)
//! Reference: A.Rotondi and P.Montagna, Fast Calculation of vavilovPdf distribution
//! Nucl.Instr. and Meth. B47(1990), 215-224
///
//! Accuracy: quote from the reference above:
///
//! "The results of our code have been compared with the values of the vavilovPdf
//! density function computed numerically in an accurate way: our approximation
//! shows a difference of less than 3% around the peak of the density function, slowly
//! increasing going towards the extreme tails to the right and to the left"
///
//! Begin_Macro
//! {
//!   TCanvas *c1 = new TCanvas("c1", "c1", 700, 500);
///
//!   c1->SetGridx();
//!   c1->SetGridy();
///
//!   TF1 *vavilovPdf = new TF1("vavilovPdf", "vavilovPdf(x, [0], [1])", -3, 11);
///
//!   vavilovPdf->SetParameters(0.5, 0.);
//!   vavilovPdf->SetLineColor(2);
//!   TF1 *vavilov1 = vavilovPdf->DrawCopy("L");
//!   vavilovPdf->SetParameters(0.3, 0.);
//!   vavilovPdf->SetLineColor(3);
//!   TF1 *vavilov2 = vavilovPdf->DrawCopy("LSAME");
//!   vavilovPdf->SetParameters(0.2, 0.);
//!   vavilovPdf->SetLineColor(4);
//!   TF1 *vavilov3 = vavilovPdf->DrawCopy("LSAME");
//!   vavilovPdf->SetParameters(0.1, 0.);
//!   vavilovPdf->SetLineColor(6);
//!   TF1 *vavilov4 = vavilovPdf->DrawCopy("LSAME");
///
//!   legend = new TLegend(0.5, 0.65, 0.85, 0.85);
//!   legend->AddEntry(vavilov1, "kappa = 0.5, beta2 = 0", "L");
//!   legend->AddEntry(vavilov2, "kappa = 0.3, beta2 = 0", "L");
//!   legend->AddEntry(vavilov3, "kappa = 0.2, beta2 = 0", "L");
//!   legend->AddEntry(vavilov4, "kappa = 0.1, beta2 = 0", "L");
//!   legend->Draw();
//! }
//! End_Macro

double  vavilovPdf(double  x, double  kappa, double  beta2)
{
  double  *ac = new double [14];
  double  *hc = new double [9];

  int  itype;
  int  npt;
  vavilovSet(kappa, beta2, 0, 0, ac, hc, itype, npt);
  double  v =  vavilovDenEval(x, ac, hc, itype);
  delete [] ac;
  delete [] hc;
  return v;
}


//!
//! Internal function, called by vavilovPdf and VavilovI

void vavilovSet(double  rkappa, double  beta2, bool mode, double  *WCM, double  *AC, double  *HC, int  &itype, int  &npt)
{

  double  BKMNX1 = 0.02, BKMNY1 = 0.05, BKMNX2 = 0.12, BKMNY2 = 0.05,
  BKMNX3 = 0.22, BKMNY3 = 0.05, BKMXX1 = 0.1 , BKMXY1 = 1,
  BKMXX2 = 0.2 , BKMXY2 = 1   , BKMXX3 = 0.3 , BKMXY3 = 1;

  double  FBKX1 = 2/(BKMXX1-BKMNX1), FBKX2 = 2/(BKMXX2-BKMNX2),
  FBKX3 = 2/(BKMXX3-BKMNX3), FBKY1 = 2/(BKMXY1-BKMNY1),
  FBKY2 = 2/(BKMXY2-BKMNY2), FBKY3 = 2/(BKMXY3-BKMNY3);

  double  FNINV[] = {0, 1, 0.5, 0.33333333, 0.25, 0.2};

  double  EDGEC[]= {0, 0, 0.16666667e+0, 0.41666667e-1, 0.83333333e-2,
    0.13888889e-1, 0.69444444e-2, 0.77160493e-3};

  double  U1[] = {0, 0.25850868e+0,  0.32477982e-1, -0.59020496e-2,
    0.            , 0.24880692e-1,  0.47404356e-2,
    -0.74445130e-3,  0.73225731e-2,  0.           ,
    0.11668284e-2,  0.           , -0.15727318e-2,-0.11210142e-2};

  double  U2[] = {0, 0.43142611e+0,  0.40797543e-1, -0.91490215e-2,
    0.           ,  0.42127077e-1,  0.73167928e-2,
    -0.14026047e-2,  0.16195241e-1,  0.24714789e-2,
    0.20751278e-2,  0.           , -0.25141668e-2,-0.14064022e-2};

  double  U3[] = {0,  0.25225955e+0,  0.64820468e-1, -0.23615759e-1,
    0.           ,  0.23834176e-1,  0.21624675e-2,
    -0.26865597e-2, -0.54891384e-2,  0.39800522e-2,
    0.48447456e-2, -0.89439554e-2, -0.62756944e-2,-0.24655436e-2};

  double  U4[] = {0, 0.12593231e+1, -0.20374501e+0,  0.95055662e-1,
    -0.20771531e-1, -0.46865180e-1, -0.77222986e-2,
    0.32241039e-2,  0.89882920e-2, -0.67167236e-2,
    -0.13049241e-1,  0.18786468e-1,  0.14484097e-1};

  double  U5[] = {0, -0.24864376e-1, -0.10368495e-2,  0.14330117e-2,
    0.20052730e-3,  0.18751903e-2,  0.12668869e-2,
    0.48736023e-3,  0.34850854e-2,  0.           ,
    -0.36597173e-3,  0.19372124e-2,  0.70761825e-3, 0.46898375e-3};

  double  U6[] = {0,  0.35855696e-1, -0.27542114e-1,  0.12631023e-1,
    -0.30188807e-2, -0.84479939e-3,  0.           ,
    0.45675843e-3, -0.69836141e-2,  0.39876546e-2,
    -0.36055679e-2,  0.           ,  0.15298434e-2, 0.19247256e-2};

  double  U7[] = {0, 0.10234691e+2, -0.35619655e+1,  0.69387764e+0,
    -0.14047599e+0, -0.19952390e+1, -0.45679694e+0,
    0.           ,  0.50505298e+0};
  double  U8[] = {0,  0.21487518e+2, -0.11825253e+2,  0.43133087e+1,
    -0.14500543e+1, -0.34343169e+1, -0.11063164e+1,
    -0.21000819e+0,  0.17891643e+1, -0.89601916e+0,
    0.39120793e+0,  0.73410606e+0,  0.           ,-0.32454506e+0};

  double  V1[] = {0, 0.27827257e+0, -0.14227603e-2,  0.24848327e-2,
    0.           ,  0.45091424e-1,  0.80559636e-2,
    -0.38974523e-2,  0.           , -0.30634124e-2,
    0.75633702e-3,  0.54730726e-2,  0.19792507e-2};

  double  V2[] = {0, 0.41421789e+0, -0.30061649e-1,  0.52249697e-2,
    0.           ,  0.12693873e+0,  0.22999801e-1,
    -0.86792801e-2,  0.31875584e-1, -0.61757928e-2,
    0.           ,  0.19716857e-1,  0.32596742e-2};

  double  V3[] = {0, 0.20191056e+0, -0.46831422e-1,  0.96777473e-2,
    -0.17995317e-2,  0.53921588e-1,  0.35068740e-2,
    -0.12621494e-1, -0.54996531e-2, -0.90029985e-2,
    0.34958743e-2,  0.18513506e-1,  0.68332334e-2,-0.12940502e-2};

  double  V4[] = {0, 0.13206081e+1,  0.10036618e+0, -0.22015201e-1,
    0.61667091e-2, -0.14986093e+0, -0.12720568e-1,
    0.24972042e-1, -0.97751962e-2,  0.26087455e-1,
    -0.11399062e-1, -0.48282515e-1, -0.98552378e-2};

  double  V5[] = {0, 0.16435243e-1,  0.36051400e-1,  0.23036520e-2,
    -0.61666343e-3, -0.10775802e-1,  0.51476061e-2,
    0.56856517e-2, -0.13438433e-1,  0.           ,
    0.           , -0.25421507e-2,  0.20169108e-2,-0.15144931e-2};

  double  V6[] = {0, 0.33432405e-1,  0.60583916e-2, -0.23381379e-2,
    0.83846081e-3, -0.13346861e-1, -0.17402116e-2,
    0.21052496e-2,  0.15528195e-2,  0.21900670e-2,
    -0.13202847e-2, -0.45124157e-2, -0.15629454e-2, 0.22499176e-3};

  double  V7[] = {0, 0.54529572e+1, -0.90906096e+0,  0.86122438e-1,
    0.           , -0.12218009e+1, -0.32324120e+0,
    -0.27373591e-1,  0.12173464e+0,  0.           ,
    0.           ,  0.40917471e-1};

  double  V8[] = {0, 0.93841352e+1, -0.16276904e+1,  0.16571423e+0,
    0.           , -0.18160479e+1, -0.50919193e+0,
    -0.51384654e-1,  0.21413992e+0,  0.           ,
    0.           ,  0.66596366e-1};

  double  W1[] = {0, 0.29712951e+0,  0.97572934e-2,  0.           ,
    -0.15291686e-2,  0.35707399e-1,  0.96221631e-2,
    -0.18402821e-2, -0.49821585e-2,  0.18831112e-2,
    0.43541673e-2,  0.20301312e-2, -0.18723311e-2,-0.73403108e-3};

  double  W2[] = {0, 0.40882635e+0,  0.14474912e-1,  0.25023704e-2,
    -0.37707379e-2,  0.18719727e+0,  0.56954987e-1,
    0.           ,  0.23020158e-1,  0.50574313e-2,
    0.94550140e-2,  0.19300232e-1};

  double  W3[] = {0, 0.16861629e+0,  0.           ,  0.36317285e-2,
    -0.43657818e-2,  0.30144338e-1,  0.13891826e-1,
    -0.58030495e-2, -0.38717547e-2,  0.85359607e-2,
    0.14507659e-1,  0.82387775e-2, -0.10116105e-1,-0.55135670e-2};

  double  W4[] = {0, 0.13493891e+1, -0.26863185e-2, -0.35216040e-2,
    0.24434909e-1, -0.83447911e-1, -0.48061360e-1,
    0.76473951e-2,  0.24494430e-1, -0.16209200e-1,
    -0.37768479e-1, -0.47890063e-1,  0.17778596e-1, 0.13179324e-1};

  double  W5[] = {0,  0.10264945e+0,  0.32738857e-1,  0.           ,
    0.43608779e-2, -0.43097757e-1, -0.22647176e-2,
    0.94531290e-2, -0.12442571e-1, -0.32283517e-2,
    -0.75640352e-2, -0.88293329e-2,  0.52537299e-2, 0.13340546e-2};

  double  W6[] = {0, 0.29568177e-1, -0.16300060e-2, -0.21119745e-3,
    0.23599053e-2, -0.48515387e-2, -0.40797531e-2,
    0.40403265e-3,  0.18200105e-2, -0.14346306e-2,
    -0.39165276e-2, -0.37432073e-2,  0.19950380e-2, 0.12222675e-2};

  double  W8[] = {0,  0.66184645e+1, -0.73866379e+0,  0.44693973e-1,
    0.           , -0.14540925e+1, -0.39529833e+0,
    -0.44293243e-1,  0.88741049e-1};

  itype = 0;
  if (rkappa <0.01 || rkappa >12) {
    Error("vavilovPdf distribution", "illegal value of kappa");
    return;
  }

  double  DRK[6];
  double  DSIGM[6];
  double  ALFA[8];
  int  j;
  double  x, y, xx, yy, x2, x3, y2, y3, xy, p2, p3, q2, q3, pq;
  if (rkappa >= 0.29) {
    itype = 1;
    npt = 100;
    double  wk = 1./sqrt(rkappa);

    AC[0] = (-0.032227*beta2-0.074275)*rkappa + (0.24533*beta2+0.070152)*wk + (-0.55610*beta2-3.1579);
    AC[8] = (-0.013483*beta2-0.048801)*rkappa + (-1.6921*beta2+8.3656)*wk + (-0.73275*beta2-3.5226);
    DRK[1] = wk*wk;
    DSIGM[1] = sqrt(rkappa/(1-0.5*beta2));
    for (j=1; j<=4; j++) {
      DRK[j+1] = DRK[1]*DRK[j];
      DSIGM[j+1] = DSIGM[1]*DSIGM[j];
      ALFA[j+1] = (FNINV[j]-beta2*FNINV[j+1])*DRK[j];
    }
    HC[0]=log(rkappa)+beta2+0.42278434;
    HC[1]=DSIGM[1];
    HC[2]=ALFA[3]*DSIGM[3];
    HC[3]=(3*ALFA[2]*ALFA[2] + ALFA[4])*DSIGM[4]-3;
    HC[4]=(10*ALFA[2]*ALFA[3]+ALFA[5])*DSIGM[5]-10*HC[2];
    HC[5]=HC[2]*HC[2];
    HC[6]=HC[2]*HC[3];
    HC[7]=HC[2]*HC[5];
    for (j=2; j<=7; j++)
      HC[j]*=EDGEC[j];
    HC[8]=0.39894228*HC[1];
  }
  else if (rkappa >=0.22) {
    itype = 2;
    npt = 150;
    x = 1+(rkappa-BKMXX3)*FBKX3;
    y = 1+(sqrt(beta2)-BKMXY3)*FBKY3;
    xx = 2*x;
    yy = 2*y;
    x2 = xx*x-1;
    x3 = xx*x2-x;
    y2 = yy*y-1;
    y3 = yy*y2-y;
    xy = x*y;
    p2 = x2*y;
    p3 = x3*y;
    q2 = y2*x;
    q3 = y3*x;
    pq = x2*y2;
    AC[1] = W1[1] + W1[2]*x + W1[4]*x3 + W1[5]*y + W1[6]*y2 + W1[7]*y3 +
    W1[8]*xy + W1[9]*p2 + W1[10]*p3 + W1[11]*q2 + W1[12]*q3 + W1[13]*pq;
    AC[2] = W2[1] + W2[2]*x + W2[3]*x2 + W2[4]*x3 + W2[5]*y + W2[6]*y2 +
    W2[8]*xy + W2[9]*p2 + W2[10]*p3 + W2[11]*q2;
    AC[3] = W3[1] + W3[3]*x2 + W3[4]*x3 + W3[5]*y + W3[6]*y2 + W3[7]*y3 +
    W3[8]*xy + W3[9]*p2 + W3[10]*p3 + W3[11]*q2 + W3[12]*q3 + W3[13]*pq;
    AC[4] = W4[1] + W4[2]*x + W4[3]*x2 + W4[4]*x3 + W4[5]*y + W4[6]*y2 + W4[7]*y3 +
    W4[8]*xy + W4[9]*p2 + W4[10]*p3 + W4[11]*q2 + W4[12]*q3 + W4[13]*pq;
    AC[5] = W5[1] + W5[2]*x + W5[4]*x3 + W5[5]*y + W5[6]*y2 + W5[7]*y3 +
    W5[8]*xy + W5[9]*p2 + W5[10]*p3 + W5[11]*q2 + W5[12]*q3 + W5[13]*pq;
    AC[6] = W6[1] + W6[2]*x + W6[3]*x2 + W6[4]*x3 + W6[5]*y + W6[6]*y2 + W6[7]*y3 +
    W6[8]*xy + W6[9]*p2 + W6[10]*p3 + W6[11]*q2 + W6[12]*q3 + W6[13]*pq;
    AC[8] = W8[1] + W8[2]*x + W8[3]*x2 + W8[5]*y + W8[6]*y2 + W8[7]*y3 + W8[8]*xy;
    AC[0] = -3.05;
  } else if (rkappa >= 0.12) {
    itype = 3;
    npt = 200;
    x = 1 + (rkappa-BKMXX2)*FBKX2;
    y = 1 + (sqrt(beta2)-BKMXY2)*FBKY2;
    xx = 2*x;
    yy = 2*y;
    x2 = xx*x-1;
    x3 = xx*x2-x;
    y2 = yy*y-1;
    y3 = yy*y2-y;
    xy = x*y;
    p2 = x2*y;
    p3 = x3*y;
    q2 = y2*x;
    q3 = y3*x;
    pq = x2*y2;
    AC[1] = V1[1] + V1[2]*x + V1[3]*x2 + V1[5]*y + V1[6]*y2 + V1[7]*y3 +
    V1[9]*p2 + V1[10]*p3 + V1[11]*q2 + V1[12]*q3;
    AC[2] = V2[1] + V2[2]*x + V2[3]*x2 + V2[5]*y + V2[6]*y2 + V2[7]*y3 +
    V2[8]*xy + V2[9]*p2 + V2[11]*q2 + V2[12]*q3;
    AC[3] = V3[1] + V3[2]*x + V3[3]*x2 + V3[4]*x3 + V3[5]*y + V3[6]*y2 + V3[7]*y3 +
    V3[8]*xy + V3[9]*p2 + V3[10]*p3 + V3[11]*q2 + V3[12]*q3 + V3[13]*pq;
    AC[4] = V4[1] + V4[2]*x + V4[3]*x2 + V4[4]*x3 + V4[5]*y + V4[6]*y2 + V4[7]*y3 +
    V4[8]*xy + V4[9]*p2 + V4[10]*p3 + V4[11]*q2 + V4[12]*q3;
    AC[5] = V5[1] + V5[2]*x + V5[3]*x2 + V5[4]*x3 + V5[5]*y + V5[6]*y2 + V5[7]*y3 +
    V5[8]*xy + V5[11]*q2 + V5[12]*q3 + V5[13]*pq;
    AC[6] = V6[1] + V6[2]*x + V6[3]*x2 + V6[4]*x3 + V6[5]*y + V6[6]*y2 + V6[7]*y3 +
    V6[8]*xy + V6[9]*p2 + V6[10]*p3 + V6[11]*q2 + V6[12]*q3 + V6[13]*pq;
    AC[7] = V7[1] + V7[2]*x + V7[3]*x2 + V7[5]*y + V7[6]*y2 + V7[7]*y3 +
    V7[8]*xy + V7[11]*q2;
    AC[8] = V8[1] + V8[2]*x + V8[3]*x2 + V8[5]*y + V8[6]*y2 + V8[7]*y3 +
    V8[8]*xy + V8[11]*q2;
    AC[0] = -3.04;
  } else {
    itype = 4;
    if (rkappa >=0.02) itype = 3;
    npt = 200;
    x = 1+(rkappa-BKMXX1)*FBKX1;
    y = 1+(sqrt(beta2)-BKMXY1)*FBKY1;
    xx = 2*x;
    yy = 2*y;
    x2 = xx*x-1;
    x3 = xx*x2-x;
    y2 = yy*y-1;
    y3 = yy*y2-y;
    xy = x*y;
    p2 = x2*y;
    p3 = x3*y;
    q2 = y2*x;
    q3 = y3*x;
    pq = x2*y2;
    if (itype==3){
      AC[1] = U1[1] + U1[2]*x + U1[3]*x2 + U1[5]*y + U1[6]*y2 + U1[7]*y3 +
      U1[8]*xy + U1[10]*p3 + U1[12]*q3 + U1[13]*pq;
      AC[2] = U2[1] + U2[2]*x + U2[3]*x2 + U2[5]*y + U2[6]*y2 + U2[7]*y3 +
      U2[8]*xy + U2[9]*p2 + U2[10]*p3 + U2[12]*q3 + U2[13]*pq;
      AC[3] = U3[1] + U3[2]*x + U3[3]*x2 + U3[5]*y + U3[6]*y2 + U3[7]*y3 +
      U3[8]*xy + U3[9]*p2 + U3[10]*p3 + U3[11]*q2 + U3[12]*q3 + U3[13]*pq;
      AC[4] = U4[1] + U4[2]*x + U4[3]*x2 + U4[4]*x3 + U4[5]*y + U4[6]*y2 + U4[7]*y3 +
      U4[8]*xy + U4[9]*p2 + U4[10]*p3 + U4[11]*q2 + U4[12]*q3;
      AC[5] = U5[1] + U5[2]*x + U5[3]*x2 + U5[4]*x3 + U5[5]*y + U5[6]*y2 + U5[7]*y3 +
      U5[8]*xy + U5[10]*p3 + U5[11]*q2 + U5[12]*q3 + U5[13]*pq;
      AC[6] = U6[1] + U6[2]*x + U6[3]*x2 + U6[4]*x3 + U6[5]*y + U6[7]*y3 +
      U6[8]*xy + U6[9]*p2 + U6[10]*p3 + U6[12]*q3 + U6[13]*pq;
      AC[7] = U7[1] + U7[2]*x + U7[3]*x2 + U7[4]*x3 + U7[5]*y + U7[6]*y2 + U7[8]*xy;
    }
    AC[8] = U8[1] + U8[2]*x + U8[3]*x2 + U8[4]*x3 + U8[5]*y + U8[6]*y2 + U8[7]*y3 +
    U8[8]*xy + U8[9]*p2 + U8[10]*p3 + U8[11]*q2 + U8[13]*pq;
    AC[0] = -3.03;
  }

  AC[9] = (AC[8] - AC[0])/npt;
  AC[10] = 1./AC[9];
  if (itype == 3) {
    x = (AC[7]-AC[8])/(AC[7]*AC[8]);
    y = 1./log(AC[8]/AC[7]);
    p2 = AC[7]*AC[7];
    AC[11] = p2*(AC[1]*exp(-AC[2]*(AC[7]+AC[5]*p2)-
                                  AC[3]*exp(-AC[4]*(AC[7]+AC[6]*p2)))-0.045*y/AC[7])/(1+x*y*AC[7]);
    AC[12] = (0.045+x*AC[11])*y;
  }
  if (itype == 4) AC[13] = 0.995/landauI(AC[8]);

  if (mode==0) return;
  //
  x = AC[0];
  WCM[0] = 0;
  double  fl, fu;
  int  k;
  fl = vavilovDenEval(x, AC, HC, itype);
  for (k=1; k<=npt; k++) {
    x += AC[9];
    fu = vavilovDenEval(x, AC, HC, itype);
    WCM[k] = WCM[k-1] + fl + fu;
    fl = fu;
  }
  x = 0.5*AC[9];
  for (k=1; k<=npt; k++)
    WCM[k]*=x;
}

//!
//! Internal function, called by vavilovPdf and vavilovSet

double  vavilovDenEval(double  rlam, double  *AC, double  *HC, int  itype)
{
  double  v = 0;
  if (rlam < AC[0] || rlam > AC[8])
    return 0;
  int  k;
  double  x, fn, s;
  double  h[10];
  if (itype ==1 ) {
    fn = 1;
    x = (rlam + HC[0])*HC[1];
    h[1] = x;
    h[2] = x*x -1;
    for (k=2; k<=8; k++) {
      fn++;
      h[k+1] = x*h[k]-fn*h[k-1];
    }
    s = 1 + HC[7]*h[9];
    for (k=2; k<=6; k++)
      s+=HC[k]*h[k+1];
    v = HC[8]*exp(-0.5*x*x)*Max(s, 0.);
  }
  else if (itype == 2) {
    x = rlam*rlam;
    v = AC[1]*exp(-AC[2]*(rlam+AC[5]*x) - AC[3]*exp(-AC[4]*(rlam+AC[6]*x)));
  }
  else if (itype == 3) {
    if (rlam < AC[7]) {
      x = rlam*rlam;
      v = AC[1]*exp(-AC[2]*(rlam+AC[5]*x)-AC[3]*exp(-AC[4]*(rlam+AC[6]*x)));
    } else {
      x = 1./rlam;
      v = (AC[11]*x + AC[12])*x;
    }
  }
  else if (itype == 4) {
    v = AC[13]*landau(rlam);
  }
  return v;
}

//!
//! Computation of voigtPdf function (normalised).
//! voigtPdf is a convolution of the two functions:
//! \f[
//! gauss(xx) = \frac{1}{(\sqrt{2\pi} sigma)} e^{\frac{xx^{2}}{(2 sigma{^2})}}
//! \f]
//! and
//! \f[
//! lorentz(xx) = \frac{ \frac{1}{\pi} \frac{lg}{2} }{ (xx^{2} + \frac{lg^{2}}{4}) }
//! \f]
//! \.
///
//! The voigtPdf function is known to be the real part of Faddeeva function also
//! called complex error function [2].
///
//! The algoritm was developed by J. Humlicek [1].
//! This code is based on fortran code presented by R. J. Wells [2].
//! Translated and adapted by Miha D. Puc
///
//! To calculate the Faddeeva function with relative error less than 10^(-r).
//! r can be set by the user subject to the constraints 2 <= r <= 5.
///
//!  - [1] J. Humlicek, JQSRT, 21, 437 (1982).
//!  - [2] [R.J. Wells "Rapid Approximation to the voigtPdf/Faddeeva Function and its Derivatives" JQSRT 62 (1999), pp 29-48.](http://www-atm.physics.ox.ac.uk/user/wells/voigtPdf.html)

double  voigtPdf(double  xx, double  sigma, double  lg, int  r)
{
  if ((sigma < 0 || lg < 0) || (sigma==0 && lg==0)) {
    return 0;  // Not meant to be for those who want to be thinner than 0
  }

  if (sigma == 0) {
    return lg * 0.159154943  / (xx*xx + lg*lg /4); //pure Lorentz
  }

  if (lg == 0) {   //pure gauss
    return 0.39894228 / sigma * exp(-xx*xx / (2*sigma*sigma));
  }

  double  x, y, k;
  x = xx / sigma / 1.41421356;
  y = lg / 2 / sigma / 1.41421356;

  double  r0, r1;

  if (r < 2) r = 2;
  if (r > 5) r = 5;

  r0=1.51 * exp(1.144 * (double )r);
  r1=1.60 * exp(0.554 * (double )r);

  // Constants

  const double  rrtpi = 0.56418958;  // 1/SQRT(pi)

  double  y0, y0py0, y0q;                      // for CPF12 algorithm
  y0 = 1.5;
  y0py0 = y0 + y0;
  y0q = y0 * y0;

  double  c[6] = { 1.0117281, -0.75197147, 0.012557727, 0.010022008, -0.00024206814, 0.00000050084806};
  double  s[6] = { 1.393237, 0.23115241, -0.15535147, 0.0062183662, 0.000091908299, -0.00000062752596};
  double  t[6] = { 0.31424038, 0.94778839, 1.5976826, 2.2795071, 3.0206370, 3.8897249};

  // Local variables

  int j;                                        // Loop variables
  int rg1, rg2, rg3;                            // y polynomial flags
  double  abx, xq, yq, yrrtpi;                 // --x--, x^2, y^2, y/SQRT(pi)
  double  xlim0, xlim1, xlim2, xlim3, xlim4;   // --x-- on region boundaries
  double  a0=0, d0=0, d2=0, e0=0, e2=0, e4=0, h0=0, h2=0, h4=0, h6=0;// W4 temporary variables
  double  p0=0, p2=0, p4=0, p6=0, p8=0, z0=0, z2=0, z4=0, z6=0, z8=0;
  double  xp[6], xm[6], yp[6], ym[6];          // CPF12 temporary values
  double  mq[6], pq[6], mf[6], pf[6];
  double  d, yf, ypy0, ypy0q;

  //***** Start of executable code *****************************************

  rg1 = 1;  // Set flags
  rg2 = 1;
  rg3 = 1;
  yq = y * y;  // y^2
  yrrtpi = y * rrtpi;  // y/SQRT(pi)

  // Region boundaries when both k and L are required or when R<>4

  xlim0 = r0 - y;
  xlim1 = r1 - y;
  xlim3 = 3.097 * y - 0.45;
  xlim2 = 6.8 - y;
  xlim4 = 18.1 * y + 1.65;
  if ( y <= 1e-6 ) {                      // When y<10^-6 avoid W4 algorithm
    xlim1 = xlim0;
    xlim2 = xlim0;
  }

  abx = fabsolute(x);                                // |x|
  xq = abx * abx;                               // x^2
  if ( abx > xlim0 ) {                          // Region 0 algorithm
    k = yrrtpi / (xq + yq);
  } else if ( abx > xlim1 ) {                   // Humlicek W4 Region 1
    if ( rg1 != 0 ) {                          // First point in Region 1
      rg1 = 0;
      a0 = yq + 0.5;                          // Region 1 y-dependents
      d0 = a0*a0;
      d2 = yq + yq - 1.0;
    }
    d = rrtpi / (d0 + xq*(d2 + xq));
    k = d * y * (a0 + xq);
  } else if ( abx > xlim2 ) {                   // Humlicek W4 Region 2
    if ( rg2 != 0 ) {                          // First point in Region 2
      rg2 = 0;
      h0 = 0.5625 + yq * (4.5 + yq * (10.5 + yq * (6.0 + yq)));
      // Region 2 y-dependents
      h2 = -4.5 + yq * (9.0 + yq * ( 6.0 + yq * 4.0));
      h4 = 10.5 - yq * (6.0 - yq * 6.0);
      h6 = -6.0 + yq * 4.0;
      e0 = 1.875 + yq * (8.25 + yq * (5.5 + yq));
      e2 = 5.25 + yq * (1.0 + yq * 3.0);
      e4 = 0.75 * h6;
    }
    d = rrtpi / (h0 + xq * (h2 + xq * (h4 + xq * (h6 + xq))));
    k = d * y * (e0 + xq * (e2 + xq * (e4 + xq)));
  } else if ( abx < xlim3 ) {                   // Humlicek W4 Region 3
    if ( rg3 != 0 ) {                          // First point in Region 3
      rg3 = 0;
      z0 = 272.1014 + y * (1280.829 + y *
                           (2802.870 + y *
                            (3764.966 + y *
                             (3447.629 + y *
                              (2256.981 + y *
                               (1074.409 + y *
                                (369.1989  + y *
                                 (88.26741 + y *
                                  (13.39880 + y)
                                  ))))))));   // Region 3 y-dependents
      z2 = 211.678 + y * (902.3066 + y *
                          (1758.336 + y *
                           (2037.310 + y *
                            (1549.675 + y *
                             (793.4273 + y *
                              (266.2987 + y *
                               (53.59518 + y * 5.0)
                               ))))));
      z4 = 78.86585 + y * (308.1852 + y *
                           (497.3014 + y *
                            (479.2576 + y *
                             (269.2916 + y *
                              (80.39278 + y * 10.0)
                              ))));
      z6 = 22.03523 + y * (55.02933 + y *
                           (92.75679 + y *
                            (53.59518 + y * 10.0)
                            ));
      z8 = 1.496460 + y * (13.39880 + y * 5.0);
      p0 = 153.5168 + y * (549.3954 + y *
                           (919.4955 + y *
                            (946.8970 + y *
                             (662.8097 + y *
                              (328.2151 + y *
                               (115.3772 + y *
                                (27.93941 + y *
                                 (4.264678 + y * 0.3183291)
                                 )))))));
      p2 = -34.16955 + y * (-1.322256+ y *
                            (124.5975 + y *
                             (189.7730 + y *
                              (139.4665 + y *
                               (56.81652 + y *
                                (12.79458 + y * 1.2733163)
                                )))));
      p4 = 2.584042 + y * (10.46332 + y *
                           (24.01655 + y *
                            (29.81482 + y *
                             (12.79568 + y * 1.9099744)
                             )));
      p6 = -0.07272979 + y * (0.9377051 + y *
                              (4.266322 + y * 1.273316));
      p8 = 0.0005480304 + y * 0.3183291;
    }
    d = 1.7724538 / (z0 + xq * (z2 + xq * (z4 + xq * (z6 + xq * (z8 + xq)))));
    k = d * (p0 + xq * (p2 + xq * (p4 + xq * (p6 + xq * p8))));
  } else {                             // Humlicek CPF12 algorithm
    ypy0 = y + y0;
    ypy0q = ypy0 * ypy0;
    k = 0.0;
    for (j = 0; j <= 5; j++) {
      d = x - t[j];
      mq[j] = d * d;
      mf[j] = 1.0 / (mq[j] + ypy0q);
      xm[j] = mf[j] * d;
      ym[j] = mf[j] * ypy0;
      d = x + t[j];
      pq[j] = d * d;
      pf[j] = 1.0 / (pq[j] + ypy0q);
      xp[j] = pf[j] * d;
      yp[j] = pf[j] * ypy0;
    }
    if ( abx <= xlim4 ) {                      // Humlicek CPF12 Region I
      for (j = 0; j <= 5; j++) {
        k = k + c[j]*(ym[j]+yp[j]) - s[j]*(xm[j]-xp[j]) ;
      }
    } else {                                   // Humlicek CPF12 Region II
      yf = y + y0py0;
      for ( j = 0; j <= 5; j++) {
        k = k + (c[j] *
                 (mq[j] * mf[j] - y0 * ym[j])
                 + s[j] * yf * xm[j]) / (mq[j]+y0q)
        + (c[j] * (pq[j] * pf[j] - y0 * yp[j])
           - s[j] * yf * xp[j]) / (pq[j]+y0q);
      }
      k = y * k + exp( -xq );
    }
  }
  return k / 2.506628 / sigma; // Normalize by dividing by sqrt(2*pi)*sigma.
}



//!
///Computes sample quantiles, corresponding to the given probabilities
///
//!  \param[in] x           the data sample
//!  \param[in] n           its size
//!  \param[out] quantiles  computed quantiles are returned in there
//!  \param[in] prob        probabilities where to compute quantiles
//!  \param[in] nprob       size of prob array
//!  \param[in] isSorted    is the input array x sorted ?
//!  \param[in] index       parameter index
//!  \param[in] type        method to compute (from 1 to 9).
///
//! #### NOTE:
//!  When the input is not sorted, an array of integers of size n needs
//!  to be allocated. It can be passed by the user in parameter index,
//!  or, if not passed, it will be allocated inside the function
///
//! ### Following types are provided:
//!  - Discontinuous:
//!    - type=1 - inverse of the empirical distribution function
//!    - type=2 - like type 1, but with averaging at discontinuities
//!    - type=3 - SAS definition: nearest even order statistic
//!  - Piecewise linear continuous:
//!    - In this case, sample quantiles can be obtained by linear interpolation
//!       between the k-th order statistic and p(k).
//!     -type=4 - linear interpolation of empirical cdf, p(k)=k/n;
//!    - type=5 - a very popular definition, p(k) = (k-0.5)/n;
//!    - type=6 - used by Minitab and SPSS, p(k) = k/(n+1);
//!    - type=7 - used by S-Plus and R, p(k) = (k-1)/(n-1);
//!    - type=8 - resulting sample quantiles are approximately median unbiased
//!               regardless of the distribution of x. p(k) = (k-1/3)/(n+1/3);
//!    - type=9 - resulting sample quantiles are approximately unbiased, when
//!               the sample comes from Normal distribution. p(k)=(k-3/8)/(n+1/4);
///
//!    default type = 7
///
//! ### References:
//!  1. Hyndman, R.J and Fan, Y, (1996) "Sample quantiles in statistical packages"
//!                                     American Statistician, 50, 361-365
//!  2. R Project documentation for the function quantile of package {stats}

void quantiles(int  n, int  nprob, double  *x, double  *quantiles, double  *prob, bool isSorted, int  *index, int  type)
{

  if (type<1 || type>9){
    printf("illegal value of type\n");
    return;
  }
  int  *ind = 0;
  bool isAllocated = false;
  if (!isSorted){
    if (index) ind = index;
    else {
      ind = new int [n];
      isAllocated = true;
    }
  }

  // re-implemented by L.M (9/11/2011) to fix bug https://savannah.cern.ch/bugs/?87251
  // following now exactly R implementation (available in library/stats/R/quantile.R )
  // which follows precisely Hyndman-Fan paper
  // (older implementation had a bug for type =3)

  for (int  i=0; i<nprob; i++){

    double  nppm = 0;
    double  gamma = 0;
    int  j = 0;

    //Discontinuous functions
    // type = 1,2, or 3
    if (type < 4 ){
      if (type == 3)
        nppm = n*prob[i]-0.5;   // use m = -0.5
      else
        nppm = n*prob[i]; // use m = 0

      // be careful with machine precision
      double eps = 4 * Limits<double >::Epsilon();
      j = FloorNint(nppm + eps);

      // LM : fix for numerical problems if nppm is actually equal to j, but results different for numerical error
      // g in the paper is nppm -j
      if (type == 1)
        gamma = ( (nppm -j) > j*Limits<double >::Epsilon() ) ? 1 : 0;
      else if (type == 2)
        gamma = ( (nppm -j) > j*Limits<double >::Epsilon() ) ? 1 : 0.5;
      else if (type == 3)
        // gamma = 0 for g=0 and j even
        gamma = ( absolute(nppm -j) <= j*Limits<double >::Epsilon()   && Even(j) ) ? 0 : 1;

    }
    else {
      // for continuous quantiles  type 4 to 9)
      // define alpha and beta
      double a = 0;
      double b = 0;
      if (type == 4)       { a = 0; b = 1; }
      else if (type == 5)  { a = 0.5; b = 0.5; }
      else if (type == 6)  { a = 0.; b = 0.; }
      else if (type == 7)  { a = 1.; b = 1.; }
      else if (type == 8)  { a = 1./3.; b = a; }
      else if (type == 9)  { a = 3./8.; b = a; }

      // be careful with machine precision
      double eps  = 4 * Limits<double >::Epsilon();
      nppm = a + prob[i] * ( n + 1 -a -b);       // n * p + m
      j = FloorNint(nppm + eps);
      gamma = nppm - j;
      if (gamma < eps) gamma = 0;
    }

    // since index j starts from 1 first is j-1 and second is j
    // add protection to keep indices in range [0,n-1]
    int first  = (j > 0 && j <=n) ? j-1 : ( j <=0 ) ? 0 : n-1;
    int second = (j > 0 && j < n) ?  j  : ( j <=0 ) ? 0 : n-1;

    double  xj, xjj;
    if (isSorted){
      xj = x[first];
      xjj = x[second];
    } else {
      xj = KOrdStat(n, x, first, ind);
      xjj = KOrdStat(n, x, second, ind);
    }
    quantiles[i] = (1-gamma)*xj + gamma*xjj;
    // printf("x[j] = %12f  x[j+1] = %12f \n",xj, xjj);

  }
  if (isAllocated)
    delete [] ind;
}


//!
//! Crystal ball function
//!
//! See the definition at
//! <A HREF="http://en.wikipedia.org/wiki/Crystal_Ball_function"> Wikipedia</A>.
//!
//! It is not really a pdf since it is not normalized
//!
double crystalBallFct(double x, double alpha, double n, double sigma, double mean = 0)    throw (MathException)
{
  // evaluate the crystal ball function
  if (sigma < 0.)  throw MathException("crystalBallPdf() sigma < 0.");
  double z = (x - mean)/sigma;
  if (alpha < 0) z = -z;
  double abs_alpha = std::abs(alpha);
  if (z  > - abs_alpha) return std::exp(- 0.5 * z * z);
  //double A = std::pow(n/abs_alpha,n) * std::exp(-0.5*abs_alpha*abs_alpha);
  double nDivAlpha = n/abs_alpha;
  double AA =  std::exp(-0.5*abs_alpha*abs_alpha);
  double B = nDivAlpha -abs_alpha;
  double arg = nDivAlpha/(B-z);
  return AA * std::pow(arg,n);
}

//!
//! Crystal ball PDF
//!
//! See the definition at
//! <A HREF="http://en.wikipedia.org/wiki/Crystal_Ball_function"> Wikipedia</A>.
//!
double crystalBallPdf(double x, double alpha, double n, double sigma, double mean = 0)     throw (MathException)
{
  if (sigma < 0.)  throw MathException("crystalBallPdf() sigma < 0.");
  if ( n <= 1) return std::numeric_limits<double>::quiet_NaN();  // pdf is not normalized for n <=1
  double abs_alpha = std::abs(alpha);
  double C = n/abs_alpha * 1./(n-1.) * std::exp(-alpha*alpha/2.);
  double D = std::sqrt(M_PI/2.)*(1.+std::erf(abs_alpha/std::sqrt(2.)));
  double N = 1./(sigma*(C+D));
  return N * crystalBallPdf(x,alpha,n,sigma,mean);
}


