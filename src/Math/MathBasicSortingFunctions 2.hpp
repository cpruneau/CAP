#ifndef CAP_MathBasicSortingFunctions
#define CAP_MathBasicSortingFunctions
#include <vector>
using std::vector;

//!
//! Compendium of sorting  functions
//! By convention, all function names  begin with a lower case letter.
//!

namespace CAP
{
namespace Math
{
void bubbleHigh(int  Narr, double  *valuesArray, int  *orderArray);
void bubbleLow(int  Narr, double  *valuesArray, int  *orderArray);
void bubbleHigh(vector<double> valuesArray, vector<int> orderArray);
void bubbleLow (vector<double> valuesArray, vector<int> orderArray);
} // namespace Math

} // namespace CAP

#endif

