
#include "MathBasicSortingFunctions.hpp"

namespace CAP
{
namespace Math
{

//!
//! Bubble sort variant to obtain the order of an array's elements into
//! an index in order to do more useful things than the standard built
//! in functions.
//! \param[in] Narr   number of array elements
//! \param[in] *valuesArray  is unchanged;
//! \param[in] *orderArray  is the array of indices corresponding to the descending value
//!            of valuesArray with orderArray[0] corresponding to the largest valuesArray value and
//!            orderArray[Narr] the smallest.
///
//! \author Adrian Bevan (bevan@slac.stanford.edu)
//!
void bubbleHigh(int  Narr, double  *valuesArray, int  *orderArray)
{
  if (Narr <= 0) return;
  double *localvaluesArray = new double[Narr];
  int    *localorderArray = new int[Narr];
  int iEl;
  int iEl2;

  for(iEl = 0; iEl < Narr; iEl++) {
    localvaluesArray[iEl] = valuesArray[iEl];
    localorderArray[iEl] = iEl;
  }

  for (iEl = 0; iEl < Narr; iEl++) {
    for (iEl2 = Narr-1; iEl2 > iEl; --iEl2) {
      if (localvaluesArray[iEl2-1] < localvaluesArray[iEl2]) {
        double tmp        = localvaluesArray[iEl2-1];
        localvaluesArray[iEl2-1] = localvaluesArray[iEl2];
        localvaluesArray[iEl2]   = tmp;

        int    tmp2       = localorderArray[iEl2-1];
        localorderArray[iEl2-1] = localorderArray[iEl2];
        localorderArray[iEl2]   = tmp2;
      }
    }
  }

  for (iEl = 0; iEl < Narr; iEl++) {
    orderArray[iEl] = localorderArray[iEl];
  }
  delete [] localorderArray;
  delete [] localvaluesArray;
}


//!
//! Opposite ordering of the array orderArray[] to that of bubbleHigh.
///
//! \author Adrian Bevan (bevan@slac.stanford.edu)
//!
void bubbleLow(int  Narr, double  *valuesArray, int  *orderArray)
{
  if (Narr <= 0) return;
  double *localvaluesArray = new double[Narr];
  int    *localorderArray = new int[Narr];
  int iEl;
  int iEl2;

  for (iEl = 0; iEl < Narr; iEl++) {
    localvaluesArray[iEl] = valuesArray[iEl];
    localorderArray[iEl] = iEl;
  }

  for (iEl = 0; iEl < Narr; iEl++) {
    for (iEl2 = Narr-1; iEl2 > iEl; --iEl2) {
      if (localvaluesArray[iEl2-1] > localvaluesArray[iEl2]) {
        double tmp        = localvaluesArray[iEl2-1];
        localvaluesArray[iEl2-1] = localvaluesArray[iEl2];
        localvaluesArray[iEl2]   = tmp;

        int    tmp2       = localorderArray[iEl2-1];
        localorderArray[iEl2-1] = localorderArray[iEl2];
        localorderArray[iEl2]   = tmp2;
      }
    }
  }

  for (iEl = 0; iEl < Narr; iEl++) {
    orderArray[iEl] = localorderArray[iEl];
  }
  delete [] localorderArray;
  delete [] localvaluesArray;
}

//!
//! Bubble sort variant to obtain the order of an array's elements into
//! an index in order to do more useful things than the standard built
//! in functions.
//! \param[in] valuesArray  is unchanged;
//! \param[in] orderArray  is the array of indices corresponding to the descending value
//!            of valuesArray with orderArray[0] corresponding to the largest valuesArray value and
//!            orderArray[Narr] the smallest.
//!
void bubbleHigh(vector<double> valuesArray, vector<int> orderArray)
{
  int  Narr = valuesArray.size();
  if (Narr <= 0) return;
  vector<double> localvaluesArray = valuesArray;
  vector<int>    localorderArray  = orderArray;
  int iEl;
  int iEl2;

  for(iEl = 0; iEl < Narr; iEl++)
    {
    localvaluesArray[iEl] = valuesArray[iEl];
    localorderArray[iEl] = iEl;
  }

  for (iEl = 0; iEl < Narr; iEl++) {
    for (iEl2 = Narr-1; iEl2 > iEl; --iEl2) {
      if (localvaluesArray[iEl2-1] < localvaluesArray[iEl2]) {
        double tmp        = localvaluesArray[iEl2-1];
        localvaluesArray[iEl2-1] = localvaluesArray[iEl2];
        localvaluesArray[iEl2]   = tmp;

        int    tmp2       = localorderArray[iEl2-1];
        localorderArray[iEl2-1] = localorderArray[iEl2];
        localorderArray[iEl2]   = tmp2;
      }
    }
  }
  for (iEl = 0; iEl < Narr; iEl++)
    {
    orderArray[iEl] = localorderArray[iEl];
    }
}


//!
//! Opposite ordering of the array orderArray[] to that of bubbleHigh.
//!
void bubbleLow(vector<double> valuesArray, vector<int> orderArray)
{
  int  Narr = valuesArray.size();
  if (Narr <= 0) return;
  vector<double> localvaluesArray = valuesArray;
  vector<int>    localorderArray  = orderArray;
  int iEl;
  int iEl2;

  for (iEl = 0; iEl < Narr; iEl++) {
    localvaluesArray[iEl] = valuesArray[iEl];
    localorderArray[iEl] = iEl;
  }

  for (iEl = 0; iEl < Narr; iEl++) {
    for (iEl2 = Narr-1; iEl2 > iEl; --iEl2) {
      if (localvaluesArray[iEl2-1] > localvaluesArray[iEl2]) {
        double tmp        = localvaluesArray[iEl2-1];
        localvaluesArray[iEl2-1] = localvaluesArray[iEl2];
        localvaluesArray[iEl2]   = tmp;

        int    tmp2       = localorderArray[iEl2-1];
        localorderArray[iEl2-1] = localorderArray[iEl2];
        localorderArray[iEl2]   = tmp2;
      }
    }
  }
  for (iEl = 0; iEl < Narr; iEl++) {
    orderArray[iEl] = localorderArray[iEl];
  }
}




} // namespace Math
} // namespace CAP

