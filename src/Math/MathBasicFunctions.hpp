#ifndef CAP_MathBasicFunctions
#define CAP_MathBasicFunctions
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include "Exceptions.hpp"

using namespace std;


//!
//! Compendium of basic and essential mathematical  functions
//!  By convention, all function names  begin with a lower case letter.

namespace CAP
{
namespace Math
{

double factorial(int  i);

//!
//! Returns the binary (base-2) logarithm of `x`.
//! 
inline double log2(double x) {  return log(x)/log(2.0); }

//inline int  isFinite(double x);
//inline int  isFinite(float x);
inline bool isNaN(double x);
inline bool isNaN(float x);

//inline double quietNaN();
//inline double signalingNaN();
//inline double infinity();

//!
//! Returns the absolute value of x
//!
template <typename T> T absolute(T x)
{
  return x<0 ? -x : x;
}

template long double absolute<long double>(long double x);
template double absolute<double>(double x);
template float  absolute<float>(float x);
template long   absolute<long>(long x);
template int    absolute<int>(int x);

template <typename T>
struct NumericalLimits {
  inline static T minimum();
  inline static T maximum();
  inline static T epsilon();
};

template<typename T>
inline T NumericalLimits<T>::minimum()
{
  return (std::numeric_limits<T>::min)();
}

////////////////////////////////////////////////////////////////////////////////
/// Returns minimum double representation.

template<typename T>
inline T NumericalLimits<T>::maximum()
{
  return (std::numeric_limits<T>::max)();  //N.B. use this signature to avoid class with macro max() on Windows
}

////////////////////////////////////////////////////////////////////////////////
/// Returns minimum double representation.

template<typename T>
inline T NumericalLimits<T>::epsilon()
{
  return std::numeric_limits<T>::epsilon();
}

//! Comparing floating points.
//! Returns `true` if the absolute difference between `af` and `bf` is less than `epsilon`.
inline bool areEqualAbs(double af, double bf, double epsilon=1.0E-25)
{
  return absolute(af-bf) < epsilon ||
  absolute(af - bf) < NumericalLimits<double>::minimum() ; //! handle 0 < 0 case
}
//! Comparing floating points.
//! Returns `true` if the relative difference between `af` and `bf` is less than `relPrec`.
inline bool areEqualRel(double af, double bf, double relPrec)
{
  return absolute(af - bf) <= 0.5 * relPrec * (absolute(af) + absolute(bf)) ||
  absolute(af - bf) < NumericalLimits<double>::minimum(); //! handle denormals
}

//! =============
//! Array Algorithms

//! Min, Max of an array
template <typename T> T minElement(long n, const T *a);
template <typename T> T maxElement(long n, const T *a);

//! Locate Min, Max element number in an array
template <typename T> long  locateMinimum(long n, const T *a);
template <typename Iterator> Iterator locateMinimum(Iterator first, Iterator last);
template <typename T> long  locateMaximum(long n, const T *a);
template <typename Iterator> Iterator locateMaximum(Iterator first, Iterator last);

//! isInside
template <typename T> bool isInside(T xp, T yp, int  np, T *x, T *y);

//! Calculate the Cross Product of two vectors
template <typename T> T *cross(const T v1[3],const T v2[3], T out[3]);

//Calculate the Normalized cross Product of two vectors
template <typename T> inline T normCross(const T v1[3],const T v2[3],T out[3]);

//! Calculate a normal vector of a plane
template <typename T> T *normalToPlane(const T v1[3],const T v2[3],const T v3[3], T normal[3]);



//!
//! Arithmetic mean
template <typename Iterator> double mean(Iterator first, Iterator last);
template <typename Iterator, typename WeightIterator> double mean(Iterator first, Iterator last, WeightIterator wfirst)   throw (MathException);
template <typename T> double mean(long n, const T *a, const double *w=0);
template <typename T> double mean(std::vector<T> values);
template <typename T> double mean(std::vector<T> values, std::vector<T> weights);

template <typename Iterator> double mean(Iterator first, Iterator last)
{
  double sum  = 0;
  double sumw = 0;
  while ( first != last )
    {
    sum  += *first;
    sumw += 1;
    first++;
    }
  return sum/sumw;
}

template <typename Iterator, typename WeightIterator>
double mean(Iterator first, Iterator last, WeightIterator w)  throw (MathException)
{
  double sum = 0;
  double sumw = 0;
  while ( first != last )
    {
    if ( *w < 0) throw MathException("negative weight","mean()");
    sum  += (*w) * (*first);
    sumw += (*w) ;
    ++w;
    ++first;
    }
  if (sumw <= 0) MathException("sum of weights == 0 ?!","mean()");
  return sum/sumw;
}

//!
//! Returns the weighted mean of an array a with length n.
//!
template <typename T>
double mean(long n, const T *a, const double *w)
{
  if (w)
    return mean(a, a+n, w);
  else
    return mean(a, a+n);
}

template <typename T> double mean(const std::vector<T> values)
{
  return mean(values.begin(), values.end());
}

template <typename T> double mean(const std::vector<T> values, const std::vector<T> weights)
{
  return mean(values.begin(), values.end(), weights.begin());
}


template <typename Iterator> double geometricMean(Iterator first, Iterator last);
template <typename T> double geometricMean(long n, const T *a);
template <typename T> double geometricMean(std::vector<T> values);

template <typename Iterator> double geometricMean(Iterator first, Iterator last)
{
  double logsum = 0.;
  long   n = 0;
  while ( first != last )
    {
    if (*first == 0) return 0.;
    double absolutea = (double) absolute(*first);
    logsum += log(absolutea);
    ++first;
    ++n;
  }
  return exp(logsum/n);
}

//!
//! Returns the geometric mean of an array a of size n.
//! \f[ geometricMean = (\prod_{i=0}^{n-1} |a[i]|)^{1/n} \f]

template <typename T> double geometricMean(long n, const T *a)
{
  return geometricMean(a, a+n);
}

//!
//! Returns the  geometric mean of the given array
//!
template <typename T> double geometricMean(const std::vector<T> & values)
{
  return geometricMean(values.begin(), values.end());
}


template <typename Iterator> double rms(Iterator first, Iterator last);
template <typename Iterator, typename WeightIterator> double rms(Iterator first, Iterator last, WeightIterator wfirst);
template <typename T> double rms(long n, const T *a, const double *w=0);
template <typename T> double rms(std::vector<T> values);
template <typename T> double rms(std::vector<T> values, std::vector<T> weights);

//! Use the two pass algorithm, which is slower (! a factor of 2) but much more
//! precise.  Since we have a vector the 2 pass algorithm is still faster than the
//! Welford algorithm.

template <typename Iterator> double rms(Iterator first, Iterator last)
{
  double n = 0;
  double tot = 0;
  double mean = mean(first,last);
  while ( first != last )
    {
    double x = double(*first);
    tot += (x - mean)*(x - mean);
    ++first;
    ++n;
    }
  double rms = (n > 1) ? sqrt(tot/(n-1)) : 0.0;
  return rms;
}

//!
//! Returns the weighted Standard Deviation of an array defined by the iterators.
//! Note that this function returns the sigma(standard deviation) and
//! not the root mean square of the array.   As in the unweighted case use the two pass algorithm.
//!
template <typename Iterator, typename WeightIterator>
double rms(Iterator first, Iterator last, WeightIterator w)
{
  double tot = 0;
  double sumw = 0;
  double sumw2 = 0;
  double mean = mean(first,last,w);
  while ( first != last ) {
    double x = double(*first);
    sumw += *w;
    sumw2 += (*w) * (*w);
    tot += (*w) * (x - mean)*(x - mean);
    ++first;
    ++w;
  }
  //! use the correction neff/(neff -1) for the unbiased formula
  double rms =  sqrt(tot * sumw/ (sumw*sumw - sumw2) );
  return rms;
}

//!
//! Returns the Standard Deviation of an array a with length n.
//! Note that this function returns the sigma(standard deviation) and
//! not the root mean square of the array.

template <typename T>
double rms(long n, const T *a, const double * w)
{
  return (w) ? rms(a, a+n, w) : rms(a, a+n);
}

template <typename T> double rms(std::vector<T> values)
{
  return rms(values.begin(), values.end());
}

template <typename T> double rms(std::vector<T> values, std::vector<T> weights)
{
  return rms(values.begin(), values.end(), weights.begin());
}

inline long double  power(long double  x, long double  y);
inline long double  power(long double  x, long y);
inline long double  power(long x,         long y);
inline double       power(double x,       double y);
inline double       power(double x,       int  y);


//!
//! determine whether the give number is finite
//!
template<typename T>
inline bool isFinite(T x)
{
  return std::isfinite(x);
}

template  bool isFinite<int>(int x);
template  bool isFinite<long>(long x);
template  bool isFinite<float>(float x);
template  bool isFinite<double>(double x);
template  bool isFinite<long double>(long double x);

//!
//! determine whether the give number is not a number NaN
//!
template<typename T>
inline bool isNaN(T x)
{
  return std::isnan(x);
}

template bool isNaN<float>(float x);
template bool isNaN<double>(double x);
template bool isNaN<long double>(long double x);


//!
//! Calculates the Normalized Cross Product of two vectors.

template <typename T> inline T normCross(const T v1[3],const T v2[3],T out[3])
{
  return Normalize(Cross(v1,v2,out));
}

//!
//! Returns minimum of array a of length n.

template <typename T>
T minElement(long n, const T *a) {
  return *std::min_element(a,a+n);
}

//!
//! Returns maximum of array a of length n.

template <typename T>
T maxElement(long n, const T *a)
{
  return *std::max_element(a,a+n);
}

template <typename T>
T minElement(std::vector<T> values)
{
  return *std::min_element(values.begin(),values.end());
}

template unsigned char minElement(std::vector<unsigned char> values);
template char          minElement(std::vector<char> values);
template unsigned int  minElement(std::vector<unsigned int> values);
template int           minElement(std::vector<int> values);
template long          minElement(std::vector<long> values);
template long long     minElement(std::vector<long long> values);
template float         minElement(std::vector<float> values);
template double        minElement(std::vector<double> values);
template long double   minElement(std::vector<long double> values);

template <typename T>
T maxElement(std::vector<T> values)
{
  return *std::max_element(values.begin(),values.end());
}

template unsigned char maxElement(std::vector<unsigned char> values);
template char          maxElement(std::vector<char> values);
template unsigned int  maxElement(std::vector<unsigned int> values);
template int           maxElement(std::vector<int> values);
template long          maxElement(std::vector<long> values);
template long long     maxElement(std::vector<long long> values);
template float         maxElement(std::vector<float> values);
template double        maxElement(std::vector<double> values);
template long double   maxElement(std::vector<long double> values);


//!
//! Returns index of array with the minimum element.
//! If more than one element is minimum returns first found.
///
//! Implement here since this one is found to be faster (mainly on 64 bit machines)
//! than stl generic implementation.
//! When performing the comparison,  the STL implementation needs to de-reference both the array iterator
//! and the iterator pointing to the resulting minimum location
//!
//! Returns index of array with the minimum element.
//! If more than one element is minimum returns first found.

template <typename Iterator>
Iterator locateMinimum(Iterator first, Iterator last)
{
  return std::min_element(first, last);
}

template <typename T>
long locateMinimum(long n, const T *a)
{
  if  (n <= 0 || !a) return -1;
  T xmin = a[0];
  long loc = 0;
  for  (long i = 1; i < n; i++) {
    if (xmin > a[i])  {
      xmin = a[i];
      loc = i;
    }
  }
  return loc;
}


template <typename T, typename Iterator>
Iterator locateMinimum(std::vector<T> values)
{
  return std::min_element(values.begin(),values.end());
}

template <typename T, typename Iterator>
Iterator locateMaximum(std::vector<T> values)
{
  return std::max_element(values.begin(),values.end());
}


//!
//! Returns index of array with the maximum element.
//! If more than one element is maximum returns first found.
///
//! Implement here since it is faster (see comment in locateMinimum function)

template <typename T>
long locateMaximum(long n, const T *a) {
  if  (n <= 0 || !a) return -1;
  T xmax = a[0];
  long loc = 0;
  for  (long i = 1; i < n; i++) {
    if (xmax < a[i])  {
      xmax = a[i];
      loc = i;
    }
  }
  return loc;
}

//!
//! Returns index of array with the maximum element.
//! If more than one element is maximum returns first found.

template <typename Iterator>
Iterator locateMaximum(Iterator first, Iterator last)
{
  return std::max_element(first, last);
}



} // namespace Math

} // namespace CAP

#endif
