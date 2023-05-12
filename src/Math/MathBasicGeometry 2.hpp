#ifndef CAP_BasicGeometry
#define CAP_BasicGeometry
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;


//!
//! Compendium of basic geometry and vector  functions
//! By convention, all function names  begin with a lower case letter.
//!

namespace CAP
{
namespace Math
{

template <typename T> bool isInside(T xp, T yp, int  np, T *x, T *y);
template <typename T> T modulus(const T v[3]);
template <typename T> T modulusSq(const T v[3]);
template <typename T> T normalize(T v[3]);
template <typename T> T scalarProduct(const T v1[3], const T v2[3]);
template <typename T> T *crossProduct(const T v1[3], const T v2[3], T out[3]);
template <typename T> T normCrossProduct(const T v1[3],const T v2[3],T out[3]);
template <typename T> T *normalToPlane(const T v1[3],const T v2[3],const T v3[3], T normal[3]);

template <typename T> T modulus(const T v[3])
{
  return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

template <typename T> T modulusSq(const T v[3])
{
  return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}

template <typename T> T normalize(T v[3])
{
  double norm = modulus(v);
  v[0] /= norm;
  v[1] /= norm;
  v[2] /= norm;
  return norm;
}

template <typename T> T scalarProduct(const T v1[3], const T v2[3])
{
  return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

//!
//! Calculates the Cross Product of two vectors: out = [v1 x v2]
//! v1 and v2 are unaffected.
//!
template <typename T> T * crossProduct(const T v1[3],const T v2[3], T out[3])
{
  out[0] = v1[1] * v2[2] - v1[2] * v2[1];
  out[1] = v1[2] * v2[0] - v1[0] * v2[2];
  out[2] = v1[0] * v2[1] - v1[1] * v2[0];
  return out;
}

template <typename T> T normCrossProduct(const T v1[3],const T v2[3],T out[3])
{
  return normalize(crossProduct(v1,v2,out));
}

//!
//! Calculates a normal vector of a plane.
///
//! \param[in]  p1, p2, p3    3D points belonged the plane to define it.
//! \param[out] normal        Pointer to 3D normal vector (normalized)
//!
template <typename T> T * normalToPlane(const T p1[3],const T p2[3],const T p3[3], T normal[3])
{
  T v1[3], v2[3];
  v1[0] = p2[0] - p1[0];
  v1[1] = p2[1] - p1[1];
  v1[2] = p2[2] - p1[2];
  v2[0] = p3[0] - p1[0];
  v2[1] = p3[1] - p1[1];
  v2[2] = p3[2] - p1[2];
  normCrossProduct(v1,v2,normal);
  return normal;
}

//!
//! Function which returns true if point xp,yp lies inside the
//! polygon defined by the np points in arrays x and y, false otherwise.
//! Note that the polygon may be open or closed.
//!
template <typename T> bool isInside(T xp, T yp, int  np, T *x, T *y)
{
  int  i, j = np-1;
  bool oddNodes = false;

  for (i=0; i<np; i++) {
    if ((y[i]<yp && y[j]>=yp) || (y[j]<yp && y[i]>=yp)) {
      if (x[i]+(yp-y[i])/(y[j]-y[i])*(x[j]-x[i])<xp) {
        oddNodes = !oddNodes;
      }
    }
    j=i;
  }
  return oddNodes;
}


} // namespace Math

} // namespace CAP

#endif
