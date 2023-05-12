#ifndef CAP_MathBasicSearch
#define CAP_MathBasicSearch
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include "MathBasicFunctions.hpp"

using namespace std;

namespace CAP
{
namespace Math
{

long binarySearch(const vector<double> array, double value) throw (MathException);

} // namespace Math

} // namespace CAP

#endif
