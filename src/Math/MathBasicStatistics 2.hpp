#ifndef CAP_MathBasicStatistics
#define CAP_MathBasicStatistics
#include <iostream>
#include <vector>
#include <cmath>
#include "MathBasicFunctions.hpp"

using namespace std;

namespace CAP
{
namespace Math
{

//!
//! Arithmetic Mean
//! I.e., the regular mean of data sample
//!
template <typename T, typename Iterator> T arithmeticMean(Iterator first, Iterator last);
template <typename T, typename Iterator, typename WeightIterator> T arithmeticMean(Iterator first, Iterator last, WeightIterator wfirst)   throw (MathException);
template <typename T> T arithmeticMean(long n, const T *a, const double *w=0);
template <typename T> T arithmeticMean(std::vector<T> values);
template <typename T> T arithmeticMean(std::vector<T> values, std::vector<T> weights);

template <typename T, typename Iterator> T geometricMean(Iterator first, Iterator last);
template <typename T> T geometricMean(long n, const T *a);
template <typename T> T geometricMean(std::vector<T> values);

template <typename T, typename Iterator> T rms(Iterator first, Iterator last);
template <typename T, typename Iterator, typename WeightIterator> T standardDeviation(Iterator first, Iterator last, WeightIterator wfirst);
template <typename T> T standardDeviation(long n, const T *a, const double *w=0);
template <typename T> T standardDeviation(std::vector<T> values);
template <typename T> T standardDeviation(std::vector<T> values, std::vector<T> weights);

template <typename T, typename Iterator> T moments(int order, Iterator first, Iterator last);
template <typename T, typename Iterator, typename WeightIterator> T moments(int order, Iterator first, Iterator last, WeightIterator wfirst);
template <typename T> T moments(int order, long n, const T *a, const double *w=0);
template <typename T> T moments(int order,std::vector<T> values);
template <typename T> T moments(int order,std::vector<T> values, std::vector<T> weights);

template <typename T, typename Iterator> T centeredMoments(int order, Iterator first, Iterator last);
template <typename T, typename Iterator, typename WeightIterator> T centeredMoments(int order, Iterator first, Iterator last, WeightIterator wfirst);
template <typename T> T centeredMoments(int order, long n, const T *a, const double *w=0);
template <typename T> T centeredMoments(int order,std::vector<T> values);
template <typename T> T centeredMoments(int order,std::vector<T> values, std::vector<T> weights);

// -------------

template <typename T, typename Iterator> T arithmeticMean(Iterator first, Iterator last)
{
  T sum  = 0;
  T sumw = 0;
  while ( first != last )
    {
    sum  += *first;
    sumw += 1;
    first++;
    }
  return sum/sumw;
}


template <typename T, typename Iterator, typename WeightIterator> T arithmeticMean(Iterator first, Iterator last, WeightIterator wfirst)   throw (MathException)
{
  T sum    = 0;
  T sumw   = 0;
  T weight = *wfirst;
  while ( first != last )
    {
    if ( weight < 0) throw MathException("negative weight","arithmeticMean()");
    sum  += weight * (*first);
    sumw += weight;
    ++wfirst;
    ++first;
    }
  if (sumw <= 0) throw MathException("sum of weights == 0 ?!","arithmeticMean()");
  return sum/sumw;
}

//!
//! Returns the weighted arithmeticMean of an array a with length n.
//!
template <typename T> T arithmeticMean(long n, const T *a, const double *w)
{
  if (w)
    return arithmeticMean<T>(a, a+n, w);
  else
    return arithmeticMean<T>(a, a+n);
}

template <typename T> T arithmeticMean(const std::vector<T> values)
{
  return arithmeticMean<T>(values.begin(), values.end());
}

template <typename T> T arithmeticMean(const std::vector<T> values, const std::vector<T> weights)
{
  return arithmeticMean<T>(values.begin(), values.end(), weights.begin());
}

template <typename T, typename Iterator> T geometricMean(Iterator first, Iterator last)
{
  T logsum = 0.;
  long   n = 0;
  while ( first != last )
    {
    if (*first == 0) return 0.;
    double value = (double) absolute(*first);
    logsum += std::log(value);
    ++first;
    ++n;
  }
  return std::exp(logsum/n);
}

//!
//! Returns the geometric arithmeticMean of an array a of size n.
//! \f[ geometricMean = (\prod_{i=0}^{n-1} |a[i]|)^{1/n} \f]

template <typename T> T geometricMean(long n, const T *a)
{
  return geometricMean<T>(a, a+n);
}

//!
//! Returns the  geometric arithmeticMean of the given array
//!
template <typename T> T geometricMean(const std::vector<T> & values)
{
  return geometricMean<T>(values.begin(), values.end());
}

//!
//! Returns the root mean square (rms) of the data identified by the given iterators.
//! Use the two pass algorithm, which is slower (! a factor of 2) but much more
//! precise.  Since we have a vector the 2 pass algorithm is still faster than the
//! Welford algorithm.
//!
template <typename T, typename Iterator> T rms(Iterator first, Iterator last)
{
  T n = 0;
  T tot = 0;
  T x   = 0;
  T mean = arithmeticMean(first,last);
  while ( first != last )
    {
    x = T(*first);
    tot += (x - mean)*(x - mean);
    ++first;
    ++n;
    }
  double rms = (n > 1) ? sqrt(tot/(n-1)) : 0.0;
  return rms;
}

//!
//! Returns the weighted Standard Deviation of an array defined by the iterators.
//! Note that this function returns the sigma (standard deviation) and
//! not the root mean square of the array.   As in the unweighted case use the two pass algorithm.
//!
template <typename T, typename Iterator, typename WeightIterator> T standardDeviation(Iterator first, Iterator last, WeightIterator wfirst)
{
  T tot    = 0;
  T sumw   = 0;
  T sumw2  = 0;
  T x      = 0;
  T weight = 0;
  T mean   = arithmeticMean<T>(first,last,wfirst);
  while ( first != last )
    {
    x      = T(*first);
    weight = *wfirst;
    sumw   += weight;
    sumw2  += weight*weight;
    tot += weight*(x-mean)*(x-mean);
    ++first;
    ++wfirst;
  }
  //! use the correction neff/(neff -1) for the unbiased formula
  return std::sqrt(tot*sumw/(sumw*sumw-sumw2) );
}

//!
//! Returns the Standard Deviation of an array a with length n.
//! Note that this function returns the sigma(standard deviation) and
//! not the root arithmeticMean square of the array.
//!
template <typename T> T standardDeviation(long n, const T *a, const double * w)
{
  return (w) ? standardDeviation<T,T*,T*>(a, a+n, w) : rms<T,T*>(a, a+n);
}

template <typename T> T standardDeviation(std::vector<T> values)
{
  return rms<T,T*>(values.begin(), values.end());
}

template <typename T> T standardDeviation(std::vector<T> values, std::vector<T> weights)
{
  return rms<T,T*>(values.begin(), values.end(), weights.begin());
}

template <typename T, typename Iterator> T moments(int order, Iterator first, Iterator last)
{
  T n = 0;
  T tot = 0;
  T x   = 0;
  while ( first != last )
    {
    x   = T(*first);
    tot += Math::power(x, order);
    ++first;
    ++n;
    }
  return (n > 1) ? Math::power(tot/(n-1),1.0/order) : 0.0;
}

template <typename T, typename Iterator, typename WeightIterator> T moments(int order, Iterator first, Iterator last, WeightIterator wfirst)
{
  T tot    = 0;
  T sumw   = 0;
  T sumw2  = 0;
  T x      = 0;
  T weight = 0;
  while ( first != last )
    {
    x      = T(*first);
    weight = *wfirst;
    sumw   += weight;
    sumw2  += weight*weight;
    tot    += weight*Math::power(x, order);
    ++first;
    ++wfirst;
    }
  //! use the correction neff/(neff -1) for the unbiased formula
  return Math::power(tot*sumw/(sumw*sumw-sumw2),1.0/order);
}

template <typename T> T moments(int order, long n, const T *a, const double *w)
{
  return (w) ? moments<T,T*,T*>(order, a, a+n, w) : moments(order, a, a+n);
}


template <typename T> T moments(int order,std::vector<T> values)
{
  return moments<T,T*>(order, values.begin(), values.end());
}

template <typename T> T moments(int order,std::vector<T> values, std::vector<T> weights)
{
  return moments<T,T*>(order, values.begin(), values.end(), weights.begin());
}

template <typename T, typename Iterator> T centeredMoments(int order, Iterator first, Iterator last)
{
  T n      = 0;
  T tot    = 0;
  T x      = 0;
  T dx     = 0;
  T mean   = arithmeticMean(first,last);
  while ( first != last )
    {
    x   = T(*first);
    dx  = x-mean;
    tot += Math::power(dx, order);
    ++first;
    ++n;
    }
  return (n > 1) ? Math::power(tot/(n-1),1.0/order) : 0.0;
}

template <typename T, typename Iterator, typename WeightIterator> double centeredMoments(int order, Iterator first, Iterator last, WeightIterator wfirst)
{
  T tot    = 0;
  T sumw   = 0;
  T sumw2  = 0;
  T x      = 0;
  T dx     = 0;
  T mean   = arithmeticMean(first,last,wfirst);
  T weight = 0;
  while ( first != last )
    {
    x      = T(*first);
    dx     = x-mean;
    weight = *wfirst;
    sumw   += weight;
    sumw2  += weight*weight;
    tot += weight*Math::power(dx, order);
    ++first;
    ++wfirst;
    }
  //! use the correction neff/(neff -1) for the unbiased formula
  return Math::power(tot*sumw/(sumw*sumw-sumw2),1.0/order);
}

template <typename T> T centeredMoments(int order, long n, const T *a, const T *w)
{
  return (w) ? centeredMoments<T,T*,T*>(order, a, a+n, w) : moments<T,T*>(order, a, a+n);
}

template <typename T> T centeredMoments(int order,std::vector<T> values)
{
  return centeredMoments<T,T*>(order, values.begin(), values.end());
}

template <typename T> T centeredMoments(int order,std::vector<T> values, std::vector<T> weights)
{
  return centeredMoments<T,T*,T*>(order, values.begin(), values.end(), weights.begin());
}


} // namespace Math

} // namespace CAP

#endif
