/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMedianImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2009-07-12 10:53:00 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "itkImage.h"
#include "itkRandomImageSource.h"
#include "itkMedianImageFilter.h"
#include "itkTextOutput.h"
#include "itkImageRegionIterator.h"


int itkMedianImageFilterTest(int, char* [] )
{
  // Comment the following if you want to use the itk text output window
  itk::OutputWindow::SetInstance(itk::TextOutput::New());


  typedef itk::Image<float,2> FloatImage2DType;

  itk::RandomImageSource<FloatImage2DType>::Pointer random;
  random = itk::RandomImageSource<FloatImage2DType>::New();
  random->SetMin(0.0);
  random->SetMax(1000.0);

  FloatImage2DType::SizeValueType randomSize[2];
  randomSize[0] = randomSize[1] = 8;
  random->SetSize(randomSize);
  
  FloatImage2DType::SpacingValueType spacing[2] = {0.7, 2.1};
  random->SetSpacing( spacing );

  FloatImage2DType::PointValueType origin[2] = {15, 400};
  random->SetOrigin( origin );
    
  // Create a median image
  itk::MedianImageFilter<FloatImage2DType, FloatImage2DType>::Pointer median;
  median = itk::MedianImageFilter<FloatImage2DType,FloatImage2DType>::New();
  median->SetInput(random->GetOutput());

  // define the neighborhood size used for the median filter (5x5)
  FloatImage2DType::SizeType neighRadius;
  neighRadius[0] = 1;
  neighRadius[1] = 1;
  median->SetRadius(neighRadius);

  // run the algorithm
  median->Update();

  itk::ImageRegionIterator<FloatImage2DType> it;
  it = itk::ImageRegionIterator<FloatImage2DType>(random->GetOutput(),
                               random->GetOutput()->GetBufferedRegion());
  std::cout << "Input image" << std::endl;
  unsigned int i;
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }

  std::cout << "Output image" << std::endl;
  it = itk::ImageRegionIterator<FloatImage2DType>(median->GetOutput(),
                               median->GetOutput()->GetBufferedRegion());
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }
  
  // Test the itkGetConstReferenceMacro
  const FloatImage2DType::SizeType & radius = median->GetRadius();
  std::cout << "median->GetRadius():" << radius << std::endl;


  return EXIT_SUCCESS;
}



