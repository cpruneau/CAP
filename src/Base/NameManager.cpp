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
#include "NameManager.hpp"

const CAP::String CAP::createName(const String & s0,
                                  const String & s1,
                                  int option)
{
  CAP::String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  return name;
}

const CAP::String CAP::createName(const String & s0,
                                  const String & s1,
                                  const String & s2,
                                  int option)
{
  CAP::String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  return name;
}

const CAP::String CAP::createName(const String & s0,
                                  const String & s1,
                                  const String & s2,
                                  const String & s3,
                                  int option)
{
  CAP::String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  return name;
}


const CAP::String CAP::createName(const String & s0,
                                  const String & s1,
                                  const String & s2,
                                  const String & s3,
                                  const String & s4,
                                  int option)
{
  CAP::String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  name += separator;
  name += s4;
  return name;
}

const CAP::String CAP::createName(const String & s0,
                                  const String & s1,
                                  const String & s2,
                                  const String & s3,
                                  const String & s4,
                                  const String & s5,
                                  int option)
{
  CAP::String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  name += separator;
  name += s4;
  name += separator;
  name += s5;
  return name;
}

const CAP::String CAP::createName(const String & s0,
                                  const String & s1,
                                  const String & s2,
                                  const String & s3,
                                  const String & s4,
                                  const String & s5,
                                  const String & s6,
                                  int option)
{
  CAP::String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  name += separator;
  name += s4;
  name += separator;
  name += s5;
  name += separator;
  name += s6;
  return name;
}

const CAP::String CAP::createName(CAP::VectorString & tokens,
                                  int option)
{
  CAP::String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = "";
  if (tokens.size()<1)
    name = "NoName";
  else
    {
    name = tokens[0];
    for (unsigned int iToken=1; iToken<tokens.size(); iToken++)
      {
      name += separator;
      name += tokens[iToken];
      }
    }
  return name;
}


const CAP::String CAP::createNameI(const String & s0,
                                   const String & s1,
                                   int i1)
{
  CAP::String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += i1;
  name += separator;
  return name;
}

const CAP::String CAP::createNameII(const String & s0,
                                    const String & s1,
                                    int i1,
                                    int i2)
{
  CAP::String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += i1;
  name += i2;
  name += separator;
  return name;
}

const CAP::String CAP::createNameIII(const String & s0,
                                     const String & s1,
                                     int i1,
                                     int i2,
                                     int i3)
{
  CAP::String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += i1;
  name += i2;
  name += i3;
  name += separator;
  return name;
}

const CAP::String CAP::createNameIIII(const String & s0,
                                      const String & s1,
                                      int i1,
                                      int i2,
                                      int i3,
                                      int i4)
{
  CAP::String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += i1;
  name += i2;
  name += i3;
  name += i4;
  name += separator;
  return name;
}


///

const CAP::String CAP::createNameIS(const String & s0,
                                    int i1,
                                    const String & suffix)
{
  CAP::String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += i1;
  name += separator;
  name += suffix;
  return name;
}

const CAP::String CAP::createNameIIS(const String & s0,
                                     int i1,
                                     int i2,
                                     const CAP::String & suffix)
{
  CAP::String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += i1;
  name += i2;
  name += separator;
  name += suffix;
  return name;
}

const CAP::String CAP::createNameIIIS(const String & s0,
                                      int i1,
                                      int i2,
                                      int i3,
                                      const CAP::String & suffix)
{
  CAP::String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += i1;
  name += i2;
  name += i3;
  name += separator;
  name += suffix;
  return name;
}

const CAP::String CAP::createNameIIIIS(const String & s0,
                                       int i1,
                                       int i2,
                                       int i3,
                                       int i4,
                                       const CAP::String & suffix)
{
  CAP::String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  CAP::String name = s0;
  name += separator;
  name += i1;
  name += i2;
  name += i3;
  name += i4;
  name += separator;
  name += suffix;
  return name;
}

