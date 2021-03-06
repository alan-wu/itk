/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: FilteringExamples4.cxx,v $
  Language:  C++
  Date:      $Date: 2009-03-16 21:52:49 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// this file defines the FilterExamples for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include "itkTestMain.h" 


void RegisterTests()
{
  REGISTER_TEST(ResampleImageFilter6Test);
  REGISTER_TEST(ResampleImageFilter7Test);
  REGISTER_TEST(ResampleVolumesToBeIsotropicTest);
  REGISTER_TEST(ScaleSpaceGenerator2DTest);
  REGISTER_TEST(SubsampleVolumeTest);
}

#undef main
#define main ResampleVolumesToBeIsotropicTest
#include "ResampleVolumesToBeIsotropic.cxx"

#undef main
#define main ScaleSpaceGenerator2DTest
#include "ScaleSpaceGenerator2D.cxx"

#undef main
#define main SubsampleVolumeTest
#include "SubsampleVolume.cxx"

#undef main
#define main ResampleImageFilter6Test
#include "ResampleImageFilter6.cxx"

#undef main
#define main ResampleImageFilter7Test
#include "ResampleImageFilter7.cxx"
