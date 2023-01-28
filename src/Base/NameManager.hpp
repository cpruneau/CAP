/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/
#ifndef CAP__NameManager
#define CAP__NameManager
#include "Aliases.hpp"

namespace CAP
{
const TString createName(const TString & s0,
                         const TString & s1,
                         int option=0);

const TString createName(const TString & s0,
                         const TString & s1,
                         const TString & s2,
                         int option=0);

const TString createName(const TString & s0,
                         const TString & s1,
                         const TString & s2,
                         const TString & s3,
                         int option=0);

const TString createName(const TString & s0,
                         const TString & s1,
                         const TString & s2,
                         const TString & s3,
                         const TString & s4,
                         int option=0);

const TString createName(const TString & s0,
                         const TString & s1,
                         const TString & s2,
                         const TString & s3,
                         const TString & s4,
                         const TString & s5,
                         int option=0);

const TString createName(const TString & s0,
                         const TString & s1,
                         const TString & s2,
                         const TString & s3,
                         const TString & s4,
                         const TString & s5,
                         const TString & s6,
                         int option=0);

const TString createName(VectorString & tokens,
                         int option=0);



const TString createNameI(const TString & s0,
                          const TString & s1,
                          int i1);

const TString createNameII(const TString & s0,
                           const TString & s1,
                           int i1,
                           int i2);

const TString createNameIII(const TString & s0,
                            const TString & s1,
                            int i1,
                            int i2,
                            int i3);
const TString createNameIIII(const TString & s0,
                             const TString & s1,
                             int i1,
                             int i2,
                             int i3,
                             int i4);

const TString createNameIS(const TString & s0,
                           int i1,
                           const TString & suffix);

const TString createNameIIS(const TString & s0,
                            int i1,
                            int i2,
                            const TString & suffix);

const TString createNameIIIS(const TString & s0,
                             int i1,
                             int i2,
                             int i3,
                             const TString & suffix);
const TString createNameIIIIS(const TString & s0,
                              int i1,
                              int i2,
                              int i3,
                              int i4,
                              const TString & suffix);



} // namespace CAP

#endif /* CAP__NameManager */
