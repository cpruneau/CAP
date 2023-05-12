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

#ifndef _TH2_VECTOR_3D_H_
#define _TH2_VECTOR_3D_H_

#include <TString.h>
#include "IdentifiedObject.hpp"
#include "Factory.hpp"

namespace CAP
{

class VectorField : public IdentifiedObject
{
public:
  // OPERATORS
  double& operator()(unsigned int i, unsigned int j, unsigned int k);

  // CLASS FUNCTIONS
  VectorField();

  VectorField(const TString & aName, double constValue);

  VectorField(const TString & aName,
              double aXmin, double aXampx, unsigned int aXpts,
              double aYmin, double aYampx, unsigned int aYpts,
              double aZmin, double aZampx, unsigned int aZpts,
              double initValue=0);

  VectorField(const VectorField & field);

  virtual ~VectorField();

  void setValue(double value);
  void setValue(const VectorField & field);
  void setValue(const TString & aName,
                double aXmin, double aXampx, unsigned int aXpts,
                double aYmin, double aYampx, unsigned int aYpts,
                double aZmin, double aZampx, unsigned int aZpts,
                double initValue=0);


  double getValue() const;
  double getValueAt(unsigned int iX, unsigned int iY, unsigned int iZ) const;
  double getValueAt(double aX, double aY, double aZ) const;
  double      getXMin() const;
  double      getXMax() const;
  int         getXPts() const;
  double      getYMin() const;
  double      getYMax() const;
  int         getYPts() const;
  double      getZMin() const;
  double      getZMax() const;
  int         getZPts() const;

  double	interpolate(double aX, double aY, double aZ) const;
  VectorField*	DerivativeX(const char* aName);
  VectorField*	DerivativeY(const char* aName);
  VectorField*	DerivativeZ(const char* aName);

protected:

  void    initialize(unsigned int nX, unsigned int nY, unsigned int nZ, double initialValue=0);
  void    reset(double value = 0);
  void    clear();

  double	interpolate1D(double aX)  const;
  double	interpolate2D(double aX, double aY)  const;
  double	interpolate3D(double aX, double aY, double aZ) const;

  inline int	  initDerivative(int aIdx, double aAMin, double aAampx, int aAPts);
  inline double derivative(double aFin, double aFi, double aFip);

  double constValue;

  double mXmin;
  double mXampx;
  unsigned int mXpts;
  double mDi;

  double mYmin;
  double mYampx;
  unsigned int mYpts;
  double mDj;

  double mZmin;
  double mZampx;
  unsigned int mZpts;
  double mDk;

  double***  field;


  // used by initDerivative() and derivative()
  static double mXin, mXi0, mXip, mX0, mXd;

// CAP Implementing VectorField to ampnage the Vectorfield used
  static int factorySize;
  static Factory<VectorField> * factory;

public:

  static Factory<VectorField> * getFactory();
  static void resetFactory();

};

}

#endif

/*! @file VectorField.h
 * @brief Definition of VectorField class.
 */
/*! @class VectorField
 * @brief Class representing a three dimensional vector field.
 *
 * The table of numbers can be interpolated with a cubic method. Class also provides a simple calculation of the
 * derivative along all three axes.
 *
 * @fn VectorField::VectorField()
 * @brief Default constructor.
 *
 * @fn VectorField::VectorField(const char* aName, double aXmin, double aXampx, int aXpts, double aYmin, double aYampx, int aYpts, double aZmin, double aZampx, int aZpts)
 * @brief Initiates a three dimensional vector field of a given name and limiting range and number of points for each axis.
 * @param [in] aName name of the vector field
 * @param [in] aXmin lower limit of X-axis
 * @param [in] aXampx higher limit of X-axis
 * @param [in] aXpts number of points in X direction
 * @param [in] aYmin lower limit of Y-axis
 * @param [in] aYampx higher limit of Y-axis
 * @param [in] aYpts number of points in Y direction
 * @param [in] aZmin lower limit of Z-axis
 * @param [in] aZampx higher limit of Z-axis
 * @param [in] aZpts number of points in Z direction
 *
 * @fn VectorField::VectorField(const VectorField& aVector)
 * @brief Copying constructor.
 * @param [in] aVector referance to VectorField
 *
 * @fn VectorField::~VectorField()
 * @brief Destructor.
 *
 * @fn double& VectorField::operator()(unsigned int i, unsigned int j, unsigned int k)
 * @brief Operator (i,j,k) providing access to VectorField array element
 * @param [in] i array index for the x-coordinate
 * @param [in] j array index for the y-coordinate
 * @param [in] k array index for the z-coordinate
 *
 * @fn const char* VectorField::getName() const
 * @brief Returns the vector field name
 *
 * @fn double VectorField::getXMin() const
 * @brief Returns the X-axis lower limit
 *
 * @fn double VectorField::getXMax() const
 * @brief Returns the X-axis higher limit
 *
 * @fn double VectorField::getXPts() const
 * @brief Returns the number of X-axis points
 *
 * @fn double VectorField::getYMin() const
 * @brief Returns the Y-axis lower limit
 *
 * @fn double VectorField::getYMax() const
 * @brief Returns the Y-axis higher limit
 *
 * @fn double VectorField::getYPts() const
 * @brief Returns the number of Y-axis points
 *
 * @fn double VectorField::getZMin() const
 * @brief Returns the Z-axis lower limit
 *
 * @fn double VectorField::getZMax() const
 * @brief Returns the Z-axis higher limit
 *
 * @fn double VectorField::getZPts() const
 * @brief Returns the number of Z-axis points
 *
 * @fn double VectorField::interpolate(double aX, double aY, double aZ)
 * @brief Returns an interpolated value of the vector field.
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn VectorField* VectorField::DerivativeX(const char* aName)
 * @brief Calculates the derivative of the vector field along the X-axis
 * @param [in] aName name of the derived vector field
 * @retval VectorField pointer to a created vector field
 *
 * @fn VectorField* VectorField::DerivativeY(const char* aName)
 * @brief Calculates the derivative of the vector field along the Y-axis
 * @param [in] aName name of the derived vector field
 * @retval VectorField pointer to a created vector field
 *
 * @fn VectorField* VectorField::DerivativeZ(const char* aName)
 * @brief Calculates the derivative of the vector field along the Z-axis
 * @param [in] aName name of the derived vector field
 * @retval VectorField pointer to a created vector field
 */
