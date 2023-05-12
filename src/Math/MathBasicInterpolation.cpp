#include "MathBasicFunctions.hpp"
#include "MathBasicInterpolation.hpp"
#include "MathBasicSearch.hpp"

namespace CAP
{
namespace Math
{

void interpolation1DLlinear(double* Tb_x, double* Tb_y, double* xval, double* yval, int nTblength)
{
  double Xmin = Tb_x[0];
  double Xmax = Tb_x[nTblength-1];
  double dX = Tb_x[1] - Xmin;
  double x1, y1, y2;
  double val_x, result;
  int idx;
  val_x = *xval;
  if(val_x >= Xmax)
    {
    idx = nTblength - 2;
    }
  else
    {
    if(val_x < Xmin)
      idx = 0;
    else
      idx = (int)((val_x - Xmin)/dX);
    }
  x1 = Tb_x[idx];
  y1 = Tb_y[idx];
  y2 = Tb_y[idx+1];

  result = y1 + (y2 - y1)*(val_x - x1)/dX;
  *yval = result;
  return;
}

//
//double interpolation2D_bilinear(Table2D* tb_ptr, double varX, double varY, int Iwarning)
//{
//  double varX_min = tb_ptr->getVarXmin();
//  double varY_min = tb_ptr->getVarYmin();
//  double dX = tb_ptr->getdvarX();
//  double dY = tb_ptr->getdvarY();
//  int tb_sizeX = tb_ptr->getTbsizeX();
//  int tb_sizeY = tb_ptr->getTbsizeY();
//
//  int idx_X = (int)floor((varX-varX_min)/dX);
//  int idx_Y = (int)floor((varY-varY_min)/dY);
//
//  //if it is out the table, doing linear extrapolation
//  if (idx_X<0 || idx_X>=tb_sizeX-1)
//    {
//    if(Iwarning == 1)
//      {
//      cout<<"interpolation2D_bilinear: varX out of bounds!"<<endl;
//      cout<<"varX= " << varX<<"idx_X= "<<idx_X<<endl;
//      }
//    if(idx_X < 0) idx_X = 0;
//    if(idx_X >= tb_sizeX-1) idx_X = tb_sizeX - 1;
//    }
//
//  if (idx_Y<0 || idx_Y>=tb_sizeY-1)
//    {
//    if(Iwarning == 1)
//      {
//      cout<<"interpolation2D_bilinear: varY out of bounds!"<<endl;
//      cout<<"varY= " << varY<<"idx_Y= "<<idx_Y<<endl;
//      }
//    if(idx_Y < 0) idx_Y = 0;
//    if(idx_Y >= tb_sizeY-1) idx_Y = tb_sizeY - 2;
//    }
//
//  double f00 = tb_ptr->getTbdata(idx_X, idx_Y);
//  double f10 = tb_ptr->getTbdata(idx_X+1, idx_Y);
//  double f01 = tb_ptr->getTbdata(idx_X, idx_Y+1);
//  double f11 = tb_ptr->getTbdata(idx_X+1, idx_Y+1);
//  double ddx = varX - (varX_min + idx_X*dX);
//  double ddy = varY - (varY_min + idx_Y*dY);
//
//  double result;
//  result = (f00*(dX-ddx)*(dY-ddy) + f01*(dX-ddx)*ddy
//            + f10*ddx*(dY-ddy) + f11*ddx*ddy)/dX/dY;
//  return(result);
//}


double interpolationCubicDirect(const vector<double> &x, const vector<double> &y, double x0)
// Returns the interpreted value of y=y(x) at x=x0 using cubic polynomial interpolation method.
// -- x,y: the independent and dependent double x0ables; x is assumed to be equal spaced and increasing
// -- x0: where the interpolation should be performed
{
  long size = x.size();
  if (size<=1)
    throw MathException("x.size()<=1","interpolationCubicDirect");
  double dx = x[1]-x[0]; // increment in x

  // if close to left end:
  if (Math::absolute(x0-x[0])<dx*1e-30) return y[0];

  // find x's integer index
  long idx = floor((x0-x[0])/dx);

  if (idx<0 || idx>=size-1)
    throw MathException("x0 out of bounds","interpolationCubicDirect");

  if (idx==0)
    {
    // use quadratic interpolation at left end
    double A0 = y[0], A1 = y[1], A2 = y[2], deltaX = x0 - x[0]; // deltaX is the increment of x0 compared to the closest lattice point
    return (A0-2.0*A1+A2)/(2.0*dx*dx)*deltaX*deltaX - (3.0*A0-4.0*A1+A2)/(2.0*dx)*deltaX + A0;
    }
  else if (idx==size-2)
    {
    // use quadratic interpolation at right end
    double A0 = y[size-3], A1 = y[size-2], A2 = y[size-1], deltaX = x0 - (x[0] + (idx-1)*dx);
    return (A0-2.0*A1+A2)/(2.0*dx*dx)*deltaX*deltaX - (3.0*A0-4.0*A1+A2)/(2.0*dx)*deltaX + A0;
    }
  else
    {
    // use cubic interpolation
    double A0 = y[idx-1], A1 = y[idx], A2 = y[idx+1], A3 = y[idx+2], deltaX = x0 - (x[0] + idx*dx);
    //cout << A0 << "  " << A1 << "  " << A2 << "  " << A3 << endl;
    return (-A0+3.0*A1-3.0*A2+A3)/(6.0*dx*dx*dx)*deltaX*deltaX*deltaX
    + (A0-2.0*A1+A2)/(2.0*dx*dx)*deltaX*deltaX
    - (2.0*A0+3.0*A1-6.0*A2+A3)/(6.0*dx)*deltaX
    + A1;
    }

}


double interpolationLinearDirect(const vector<double> &x, const vector<double> &y, double x0)
// Returns the interpreted value of y=y(x) at x=x0 using linear interpolation method.
// -- x,y: the independent and dependent double x0ables; x is assumed to be equal spaced and increasing
// -- x0: where the interpolation should be performed
{
  long size = x.size();
  if (size<=1) throw MathException("x.size()<=1","interpolationLinearDirect");
  double dx = x[1]-x[0]; // increment in x
  // if close to left end:
  if (absolute(x0-x[0])<dx*1e-30) return y[0];

  // find x's integer index
  long idx = floor((x0-x[0])/dx);

  if (idx<0 || idx>=size-1)  throw MathException("idx<0 || idx>=size-1","interpolationLinearDirect");
  return y[idx] + (y[idx+1]-y[idx])/dx*(x0-x[idx]);

}

double interpolationNearestDirect(const vector<double> &x, const vector<double> &y, double x0) 
// Returns the interpreted value of y=y(x) at x=x0 using nearest interpolation method.
// -- x,y: the independent and dependent double x0ables; x is assumed to be equal spaced and increasing
// -- x0: where the interpolation should be performed
{
  long size = x.size();
  if (size==1) throw  MathException("x.size()<=1","interpolationNearestDirect");
  double dx = x[1]-x[0]; // increment in x
  // if close to left end:
  if (absolute(x0-x[0])<dx*1e-30) return y[0];
  // find x's integer index
  long idx = floor((x0-x[0])/dx);
  if (idx<0 || idx>=size-1) throw MathException("idx<0 || idx>=size-1","interpolationNearestDirect");
  return x0-x[idx]>dx/2 ? y[idx+1] : y[idx];
}


double interpolationCubicMono(const vector<double> &x, const vector<double> &y, double xx)
// Returns the interpreted value of y=y(x) at x=x0 using cubic polynomial interpolation method.
// -- x,y: the independent and dependent double x0ables; x is *NOT* assumed to be equal spaced but it has to be increasing
// -- xx: where the interpolation should be performed
{
  long size = x.size();
  if (size<=1) throw MathException("x.size()<=1","interpolationCubicMono()");
  // if close to left end:
  if (absolute(xx-x[0])<(x[1]-x[0])*1e-30) return y[0];
  // find x's integer index
  long idx = binarySearch(x, xx);
  if (idx<0 || idx>=size-1)  throw MathException("idx<0 || idx>=size-1","interpolationCubicMono()");
  if (idx==0)
    {
    // use linear interpolation at the left end
    return y[0] + ( y[1]-y[0] )/( x[1]-x[0] )*( xx-x[0] );
    }
  else if (idx==size-2)
    {
    // use linear interpolation at the right end
    return y[size-2] + ( y[size-1]-y[size-2] )/( x[size-1]-x[size-2] )*( xx-x[size-2] );
    }
  else
    {
    // use cubic interpolation
    long double y0 = y[idx-1], y1 = y[idx], y2 = y[idx+1], y3 = y[idx+2];
    long double y01=y0-y1, y02=y0-y2, y03=y0-y3, y12=y1-y2, y13=y1-y3, y23=y2-y3;
    long double x0 = x[idx-1], x1 = x[idx], x2 = x[idx+1], x3 = x[idx+2];
    long double x01=x0-x1, x02=x0-x2, x03=x0-x3, x12=x1-x2, x13=x1-x3, x23=x2-x3;
    long double x0s=x0*x0, x1s=x1*x1, x2s=x2*x2, x3s=x3*x3;
    long double denominator = x01*x02*x12*x03*x13*x23;
    long double C0, C1, C2, C3;
    C0 = (x0*x02*x2*x03*x23*x3*y1
          + x1*x1s*(x0*x03*x3*y2+x2s*(-x3*y0+x0*y3)+x2*(x3s*y0-x0s*y3))
          + x1*(x0s*x03*x3s*y2+x2*x2s*(-x3s*y0+x0s*y3)+x2s*(x3*x3s*y0-x0*x0s*y3))
          + x1s*(x0*x3*(-x0s+x3s)*y2+x2*x2s*(x3*y0-x0*y3)+x2*(-x3*x3s*y0+x0*x0s*y3))
          )/denominator;
    C1 = (x0s*x03*x3s*y12
          + x2*x2s*(x3s*y01+x0s*y13)
          + x1s*(x3*x3s*y02+x0*x0s*y23-x2*x2s*y03)
          + x2s*(-x3*x3s*y01-x0*x0s*y13)
          + x1*x1s*(-x3s*y02+x2s*y03-x0s*y23)
          )/denominator;
    C2 = (-x0*x3*(x0s-x3s)*y12
          + x2*(x3*x3s*y01+x0*x0s*y13)
          + x1*x1s*(x3*y02+x0*y23-x2*y03)
          + x2*x2s*(-x3*y01-x0*y13)
          + x1*(-x3*x3s*y02+x2*x2s*y03-x0*x0s*y23)
          )/denominator;
    C3 = (x0*x03*x3*y12
          + x2s*(x3*y01+x0*y13)
          + x1*(x3s*y02+x0s*y23-x2s*y03)
          + x2*(-x3s*y01-x0s*y13)
          + x1s*(-x3*y02+x2*y03-x0*y23)
          )/denominator;
    /*    cout  << x0s*x03*x3s*y12 << "  "
     <<  x2*x2s*(x3s*y01+x0s*y13) << "   "
     <<  x1s*(x3*x3s*y02+x0*x0s*y23-x2*x2s*y03) << "  "
     <<  x2s*(-x3*x3s*y01-x0*x0s*y13) << "  "
     <<  x1*x1s*(-x3s*y02+x2s*y03-x0s*y23) << endl;
     cout << denominator << endl;

     cout << x0 << " " << x1 << "  " << x2 << "  " << x3 << endl;
     cout << y0 << " " << y1 << "  " << y2 << "  " << y3 << endl;
     cout << C0 << "  " << C1 << "  " << C2 << "  " << C3 << endl;*/
    return C0 + C1*xx + C2*xx*xx + C3*xx*xx*xx;
    }
}

double interpolationLinearMono(const vector<double> &x, const vector<double> &y, double xx)
// Returns the interpreted value of y=y(x) at x=x0 using linear interpolation method.
// -- x,y: the independent and dependent double x0ables; x is *NOT* assumed to be equal spaced but it has to be increasing
// -- xx: where the interpolation should be performed
{
  long size = x.size();
  if (size<=1) throw MathException("x.size()<=1","interpolationLinearMono()");
  // if close to left end:
  if (absolute(xx-x[0])<(x[1]-x[0])*1e-30) return y[0];
  // find x's integer index
  long idx = binarySearch(x, xx);
  if (idx<0 || idx>=size-1)  throw MathException("idx<0 || idx>=size-1","interpolationLinearMono()");
  return y[idx] + ( y[idx+1]-y[idx] )/( x[idx+1]-x[idx] )*( xx-x[idx] );
}

double interpolationNearestMono(const vector<double> &x, const vector<double> &y, double xx)
// Returns the interpreted value of y=y(x) at x=x0 using nearest interpolation method.
// -- x,y: the independent and dependent double x0ables; x is *NOT* assumed to be equal spaced but it has to be increasing
// -- xx: where the interpolation should be performed
{
  long size = x.size();
  if (size<=1) throw MathException("x.size()<=1","interpolationNearestMono()");
  // if close to left end:
  if (absolute(xx-x[0])<(x[1]-x[0])*1e-30) return y[0];
  // find x's integer index
  long idx = binarySearch(x, xx);
  if (idx<0 || idx>=size-1) throw MathException("idx<0 || idx>=size-1","interpolationNearestMono()");
  return xx-x[idx] > x[idx+1]-xx ? y[idx+1] : y[idx];
}

} // namespace Math

} // namespace CAP

