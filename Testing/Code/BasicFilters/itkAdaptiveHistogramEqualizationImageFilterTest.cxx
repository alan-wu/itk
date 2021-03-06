/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAdaptiveHistogramEqualizationImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2007-08-10 14:34:01 $
  Version:   $Revision: 1.5 $

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
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include "itkFilterWatcher.h"

int itkAdaptiveHistogramEqualizationImageFilterTest( int argc, char * argv[] )

{
  if( argc < 6 ) 
    { 
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile radius alpha beta" << std::endl;
    return EXIT_FAILURE;
    }

  
  typedef    float    InputPixelType;
  typedef    float    OutputPixelType;
  static const int ImageDimension=2;
  
  typedef itk::Image< InputPixelType,  ImageDimension >   InputImageType;
  typedef itk::Image< OutputPixelType, ImageDimension >   OutputImageType;
  typedef itk::ImageFileReader< InputImageType >  ReaderType;
  typedef itk::AdaptiveHistogramEqualizationImageFilter<
               InputImageType >  FilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  FilterType::ImageSizeType radius;
  radius.Fill( atoi(argv[3]) );

  FilterType::Pointer filter = FilterType::New();
  FilterWatcher watcher(filter);

  filter->SetInput( reader->GetOutput() );
  filter->SetRadius( radius );
  filter->SetAlpha( atof(argv[4]) );
  filter->SetBeta( atof(argv[5]) );

  //
  //  The output of the filter is connected here to a intensity rescaler filter
  //  and then to a writer. Invoking \code{Update()} on the writer triggers the
  //  execution of both filters.
  //

  typedef unsigned char WritePixelType;

  typedef itk::Image< WritePixelType, 2 > WriteImageType;

  typedef itk::RescaleIntensityImageFilter< 
               InputImageType, WriteImageType > RescaleFilterType;

  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetOutputMinimum(   0 );
  rescaler->SetOutputMaximum( 255 );
  rescaler->SetInput( filter->GetOutput() );
  

  typedef itk::ImageFileWriter< WriteImageType >  WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
 
  writer->SetInput( rescaler->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;

}

