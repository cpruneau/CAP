#include "MathBasicGeometry.hpp"


template bool CAP::Math::isInside<double> (double  xp, double  yp, int  np, double  *x, double  *y);
template double CAP::Math::modulus<double> (const double  v[3]);
template double CAP::Math::modulusSq<double> (const double  v[3]);
template double CAP::Math::normalize<double> (double  v[3]);
template double CAP::Math::scalarProduct<double>    (const double  v1[3], const double  v2[3]);
template double *CAP::Math::crossProduct<double>    (const double  v1[3], const double  v2[3], double  out[3]);
template double CAP::Math::normCrossProduct<double> (const double  v1[3], const double  v2[3], double  out[3]);
template double *CAP::Math::normalToPlane<double>   (const double  v1[3], const double  v2[3], const double  v3[3], double normal[3]);
