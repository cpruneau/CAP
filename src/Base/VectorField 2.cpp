/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/absolute/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

#include "THGlobal.hpp"
#include "VectorField.hpp"
using CAP::VectorField;
using CAP::Factory;
// STATIC VARIABLES

double VectorField::mXin;
double VectorField::mXi0;
double VectorField::mXip;
double VectorField::mX0;
double VectorField::mXd;

// OPERATORS

double& VectorField::operator()(unsigned int iX, unsigned int iY, unsigned int iZ)
{
  if (type==0)
    return constValue;
  else if (iX>=mXpts || iY>=mYpts || iZ>=mZpts)
    {
    cout << "VectorField::operator(): Invalid input" << endl;
    exit(1);
    }
  else
    return field[iX][iY][iZ];
}

// CLASS FUNCTIONS

VectorField::VectorField()
:
IdentifiedObject("VectorField","","","","",0),
constValue(0.0),
mXmin(0.0), mXampx(1.0), mXpts(0), mDi(0.0),
mYmin(0.0), mYampx(1.0), mYpts(0), mDj(0.0),
mZmin(0.0), mZampx(1.0), mZpts(0), mDk(0.0),
field(nullptr)
{
  // no field array initialization -- this field is constant type == 0
}

VectorField::VectorField(const TString & aName, double constValue)
:
IdentifiedObject(aName,aName,"","","",0),
constValue(constValue),
mXmin(0.0), mXampx(1.0), mXpts(0), mDi(0.0),
mYmin(0.0), mYampx(1.0), mYpts(0), mDj(0.0),
mZmin(0.0), mZampx(1.0), mZpts(0), mDk(0.0),
field(nullptr)
{
  // no field array initialization -- this field is constant type == 0
}

VectorField::VectorField(const TString & aName,
                         double aXmin, double aXampx, unsigned int aXpts,
                         double aYmin, double aYampx, unsigned int aYpts,
                         double aZmin, double aZampx, unsigned int aZpts,
                         double initValue)
:
IdentifiedObject(aName,aName,"","","",1),
constValue(0.0),
mXmin(aXmin), mXampx(aXampx), mXpts(aXpts), mDi(0.0),
mYmin(aYmin), mYampx(aYampx), mYpts(aYpts), mDj(0.0),
mZmin(aZmin), mZampx(aZampx), mZpts(aZpts), mDk(0.0),
field(nullptr)
{
  if(mXpts < 1) mXpts = 1;
  if(mYpts < 1) mYpts = 1;
  if(mZpts < 1) mZpts = 1;
  if (mXampx<=mXmin || mYampx<=mYmin || mZampx<=mZmin)
    {
    cout << "VectorField::VectorField: Invalid input" << endl;
    exit(1);
    }
  mDi = (mXpts - 1) / (mXampx - mXmin);
  mDj = (mYpts - 1) / (mYampx - mYmin);
  mDk = (mZpts - 1) / (mZampx - mZmin);
  initialize(mXpts, mYpts, mYpts,initValue);
}

VectorField::VectorField(const VectorField& field)
:
IdentifiedObject(field),
constValue(field.constValue),
mXmin(field.mXmin), mXampx(field.mXampx), mXpts(field.mXpts), mDi(field.mDi),
mYmin(field.mYmin), mYampx(field.mYampx), mYpts(field.mYpts), mDj(field.mDj),
mZmin(field.mZmin), mZampx(field.mZampx), mZpts(field.mZpts), mDk(field.mDk),
field(nullptr)
{
  if (getType()==1 || mXpts>0)
    initialize(mXpts, mYpts, mYpts);
}

VectorField::~VectorField()
{
  clear();
}

void VectorField::initialize(unsigned int nX, unsigned int nY, unsigned int nZ, double initialValue)
{
  if (field) clear();
  setType(1);
  constValue = 0;
  field = new double** [nX];
  for (unsigned int i=0; i<nX; i++)
    {
    field[i] = new double* [nY];
    for (unsigned int j=0; j<nY; j++)
      {
      field[i][j] = new double [nZ];
      for (unsigned int k=0; k<nZ; k++)
        field[i][j][k] = initialValue;
      }
    }
}

void VectorField::reset(double value)
{
  if (getType()==0)
    constValue = value;
  else
    {
    for (unsigned int i=0; i<mXpts; i++)
      {
      for (unsigned int j=0; j<mYpts; j++)
        {
        for (unsigned int k=0; k<mZpts; k++)
          field[i][j][k] = value;
        }
      }
    }
}

void VectorField::clear()
{
  if (field)
    {
    for (unsigned int i=0; i<mXpts; i++)
      {
      for (unsigned int j=0; j<mYpts; j++)
        delete[] field[i][j];
      delete[] field[i];
      }
    delete[] field;
    }
}


void VectorField::setValue(double value)
{
  setType(0);
  constValue = value;
}

void VectorField::setValue(const VectorField & field)
{
  setType(1);
  constValue = 0.0;
  mXmin = field.mXmin; mXampx = field.mXampx; mXpts = field.mXpts; mDi = field.mDi;
  mYmin = field.mYmin; mYampx = field.mYampx; mYpts = field.mYpts; mDj = field.mDj;
  mZmin = field.mZmin; mZampx = field.mZampx; mZpts = field.mZpts; mDk = field.mDk;
  initialize(mXpts,mYpts,mZpts);
}

void VectorField::setValue(const TString & aName,
                           double aXmin, double aXampx, unsigned int aXpts,
                           double aYmin, double aYampx, unsigned int aYpts,
                           double aZmin, double aZampx, unsigned int aZpts,
                           double initValue)
{
  setType(1);
  setName(aName);
  setTitle(aName);
  constValue = 0.0;
  mXmin = aXmin; mXampx = aXampx; mXpts = aXpts; mDi = 0.0;
  mYmin = aYmin; mYampx = aYampx; mYpts = aYpts; mDj = 0.0;
  mZmin = aZmin; mZampx = aZampx; mZpts = aZpts; mDk = 0.0;
  if(mXpts < 1) mXpts = 1;
  if(mYpts < 1) mYpts = 1;
  if(mZpts < 1) mZpts = 1;
  if (mXampx<=mXmin || mYampx<=mYmin || mZampx<=mZmin)
    {
    cout << "VectorField::setValue() -- Invalid input" << endl;
    exit(1);
    }
  mDi = (mXpts - 1) / (mXampx - mXmin);
  mDj = (mYpts - 1) / (mYampx - mYmin);
  mDk = (mZpts - 1) / (mZampx - mZmin);
  initialize(mXpts, mYpts, mYpts,initValue);
}

double VectorField::getValue() const
{
  return constValue;
}

double VectorField::getValueAt(unsigned int iX, unsigned int iY, unsigned int iZ) const
{
  if (type==0)
    return constValue;
  else if (iX>=mXpts || iY>=mYpts || iZ>=mZpts)
    return -1.0E50;
  else
    return field[iX][iY][iZ];
}

double VectorField::getValueAt(double aX, double aY, double aZ) const
{
  if (type==0)
    return constValue;
  else
    return interpolate(aX,aY,aZ);
}

double	VectorField::getXMin() const	{ return mXmin; }
double	VectorField::getXMax() const	{ return mXampx; }
int		  VectorField::getXPts() const	{ return mXpts; }
double	VectorField::getYMin() const	{ return mYmin; }
double  VectorField::getYMax() const	{ return mYampx; }
int		  VectorField::getYPts() const	{ return mYpts; }
double	VectorField::getZMin() const	{ return mZmin; }
double	VectorField::getZMax() const	{ return mZampx; }
int		  VectorField::getZPts() const	{ return mZpts; }

double VectorField::interpolate(double aX, double aY, double aZ) const
{
  if(mZpts>1)
    return interpolate3D(aX, aY, aZ);
  else if (mYpts>1)
    return interpolate2D(aX, aY);
  else if (mXpts>1)
    return interpolate1D(aX);
  else
    return field[0][0][0];
}

VectorField* VectorField::DerivativeX(const char* aName)
{
  unsigned int ti;
  VectorField* tVec;

  mXd = (mXampx - mXmin) / (mXpts - 1);
  tVec = new VectorField(*this);
  tVec->setName(aName);
  for (unsigned int i=0; i<mXpts; i++)
    {
    ti = initDerivative(i, mXmin, mXampx, mXpts);
    for (unsigned int j=0; j<mYpts; j++)
      for (unsigned int k=0; k<mZpts; k++)
	(*tVec)(i,j,k) = derivative(field[ti-1][j][k], field[ti][j][k], field[ti+1][j][k]);
  }
  return tVec;
}

VectorField* VectorField::DerivativeY(const char* aName)
{
  unsigned int tj;
  VectorField* tVec;

  mXd = (mYampx - mYmin) / (mYpts - 1);
  tVec = new VectorField(*this);
  tVec->setName(aName);
  for (unsigned int i=0; i<mXpts; i++)
    for (unsigned int j=0; j<mYpts; j++) {
      tj = initDerivative(j, mYmin, mYampx, mYpts);
      for (unsigned int k=0; k<mZpts; k++)
	(*tVec)(i,j,k) = derivative(field[i][tj-1][k], field[i][tj][k], field[i][tj+1][k]);
    }
  return tVec;
}

VectorField* VectorField::DerivativeZ(const char* aName)
{
  unsigned int tk;
  VectorField* tVec;

  mXd = (mZampx - mZmin) / (mZpts - 1);
  tVec = new VectorField(*this);
  tVec->setName(aName);
  for (unsigned int i=0; i<mXpts; i++)
    for (unsigned int j=0; j<mYpts; j++)
      for (unsigned int k=0; k<mZpts; k++) {
	tk = initDerivative(k, mZmin, mZampx, mZpts);
	(*tVec)(i,j,k) = derivative(field[i][j][tk-1], field[i][j][tk], field[i][j][tk+1]);
      }
  return tVec;
}

double VectorField::interpolate1D(double aX)  const
{
  unsigned int    i, j, k;
  double ti;

  ti = (aX - mXmin) * mDi;	i = (unsigned int) ti;	if(i+1 > mXpts-1) i--;	ti -= i;
  j  = 0;
  k  = 0;
  return
    field[i  ][j  ][k  ] * (1-ti) + field[i+1][j  ][k  ] * ti;
}

double VectorField::interpolate2D(double aX, double aY) const
{
  unsigned int    i, j, k;
  double ti,tj;

  ti = (aX - mXmin) * mDi;	i = (unsigned int) ti;	if(i+1 > mXpts-1) i--;	ti -= i;
  tj = (aY - mYmin) * mDj;	j = (unsigned int) tj;	if(j+1 > mYpts-1) j--;	tj -= j;
  k  = 0;
  return
    (field[i  ][j  ][k  ] * (1-ti) + field[i+1][j  ][k  ] * ti) * (1-tj) +
    (field[i  ][j+1][k  ] * (1-ti) + field[i+1][j+1][k  ] * ti) *    tj;
}

double VectorField::interpolate3D(double aX, double aY, double aZ)  const
{
  unsigned int    i, j, k;
  double ti,tj,tk;

  ti = (aX - mXmin) * mDi;	i = (unsigned int) ti;	if(i+1 > mXpts-1) i--;	ti -= i;
  tj = (aY - mYmin) * mDj;	j = (unsigned int) tj;	if(j+1 > mYpts-1) j--;	tj -= j;
  tk = (aZ - mZmin) * mDk;	k = (unsigned int) tk;	if(k+1 > mZpts-1) k--;	tk -= k;
  return
    (
      (field[i  ][j  ][k  ] * (1-ti) + field[i+1][j  ][k  ] * ti) * (1-tj) +
      (field[i  ][j+1][k  ] * (1-ti) + field[i+1][j+1][k  ] * ti) *    tj
    ) * (1-tk) + (
      (field[i  ][j  ][k+1] * (1-ti) + field[i+1][j  ][k+1] * ti) * (1-tj) +
      (field[i  ][j+1][k+1] * (1-ti) + field[i+1][j+1][k+1] * ti) *    tj
    ) *    tk;
}

inline int VectorField::initDerivative(int aIdx, double aAMin, double aAampx, int aAPts)
{
  if(aIdx == 0) {
    mXin = aAMin;
    mXi0 = aAMin + mXd;
    mXip = aAMin + mXd * 2.0;
    mX0  = aAMin;
    return aIdx + 1;
  } else if (aIdx == aAPts - 1) {
    mXin = aAampx - mXd * 2.0;
    mXi0 = aAampx - mXd;
    mXip = aAampx;
    mX0  = aAampx;
    return aIdx - 1;
  } else {
    mXin = aAMin + mXd * (aIdx - 1);
    mXi0 = aAMin + mXd *  aIdx;
    mXip = aAMin + mXd * (aIdx + 1);
    mX0  = mXi0;
    return aIdx;
  }
}

inline double VectorField::derivative(double aFin, double aFi0, double aFip)
{
  return (
    mXin * (mXin - 2.0 * mX0) * (aFip - aFi0) +
    mXi0 * (mXi0 - 2.0 * mX0) * (aFin - aFip) +
    mXip * (mXip - 2.0 * mX0) * (aFi0 - aFin)
  ) / (2.0 * mXd * mXd * mXd);
}


int VectorField::factorySize = 40;
Factory<VectorField> * VectorField::factory = 0;
Factory<VectorField> * VectorField::getFactory()
{
  if (!factory)
    {
    factory = new Factory<VectorField>();
    factory->initialize(factorySize);
    }
  return factory;
}

void VectorField::resetFactory()
{
  factory->reset();
}




