#include "MathBasicStatistics.hpp"

namespace CAP
{
namespace Math
{

template double CAP::Math::arithmeticMean<double,double*>(double* first, double*last);

//template <typename T, typename Iterator, typename WeightIterator> T arithmeticMean(Iterator first, Iterator last, WeightIterator wfirst)
//template <typename T> T arithmeticMean(long n, const T *a, const double *w=0);
//template <typename T> T arithmeticMean(std::vector<T> values);
//template <typename T> T arithmeticMean(std::vector<T> values, std::vector<T> weights);
//
//template <typename T, typename Iterator> T geometricMean(Iterator first, Iterator last);
//template <typename T> T geometricMean(long n, const T *a);
//template <typename T> T geometricMean(std::vector<T> values);
//
//template <typename T, typename Iterator> T rms(Iterator first, Iterator last);
//template <typename Iterator, typename WeightIterator> T standardDeviation(Iterator first, Iterator last, WeightIterator wfirst);
//template <typename T> T standardDeviation(long n, const T *a, const double *w=0);
//template <typename T> T standardDeviation(std::vector<T> values);
//template <typename T> T standardDeviation(std::vector<T> values, std::vector<T> weights);
//
//template <typename T, typename Iterator> T moments(int order, Iterator first, Iterator last);
//template <typename Iterator, typename WeightIterator> double moments(int order, Iterator first, Iterator last, WeightIterator wfirst);
//template <typename T> T moments(int order, long n, const T *a, const double *w=0);
//template <typename T> T moments(int order,std::vector<T> values);
//template <typename T> T moments(int order,std::vector<T> values, std::vector<T> weights);
//
//template <typename T, typename Iterator> T centeredMoments(int order, Iterator first, Iterator last);
//template <typename Iterator, typename WeightIterator> double centeredMoments(int order, Iterator first, Iterator last, WeightIterator wfirst);
//template <typename T> T centeredMoments(int order, long n, const T *a, const double *w=0);
//template <typename T> T centeredMoments(int order,std::vector<T> values);
//template <typename T> T centeredMoments(int order,std::vector<T> values, std::vector<T> weights);


} // namespace Math
} // namespace CAP

