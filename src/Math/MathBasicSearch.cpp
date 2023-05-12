#include "MathBasicSearch.hpp"

namespace CAP
{
namespace Math
{

//!
//! Return the index of the largest number less than 'value' in the list A
//! using binary search.
//!
long binarySearch(const vector<double> array, double value)
{
  int length = array.size();
  if (length<2) throw MathException("array.size()<2","binarySearch()");
  int idx_i, idx_f, idx;
  idx_i = 0;
  idx_f = length-1;
  if(value > array[idx_f]) throw MathException("Desired value is too large","binarySearch()");
  if(value < array[idx_i]) throw MathException("Desired value is too small","binarySearch()");
  idx = (int) floor((idx_f+idx_i)/2.);
  while((idx_f-idx_i) > 1)
    {
    if(array[idx] < value)
      idx_i = idx;
    else
      idx_f = idx;
    idx = (int) floor((idx_f+idx_i)/2.);
    }
  return(idx_i);
}


} // namespace Math

} // namespace CAP

