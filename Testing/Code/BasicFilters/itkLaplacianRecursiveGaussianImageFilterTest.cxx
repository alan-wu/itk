/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLaplacianRecursiveGaussianImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2007-08-10 14:34:02 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkLaplacianRecursiveGaussianImageFilter.h"
#include "itkZeroCrossingBasedEdgeDetectionImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkZeroCrossingImageFilter.h"
#include "itkImage.h"

int itkLaplacianRecursiveGaussianImageFilterTest(int argc, char* argv[])
{
  if( argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImage outputImage " << std::endl;
    return -1;
    }
   
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char    CharPixelType;  //IO
  typedef double          RealPixelType;  //Operations

  const    unsigned int    Dimension = 2;

  typedef itk::Image<CharPixelType, Dimension>    CharImageType;
  typedef itk::Image<RealPixelType, Dimension>    RealImageType;
  
  typedef itk::ImageFileReader< CharImageType >  ReaderType;
  typedef itk::ImageFileWriter< CharImageType >  WriterType;

  typedef itk::CastImageFilter<CharImageType, RealImageType> CastToRealFilterType;
  typedef itk::CastImageFilter<RealImageType, CharImageType> CastToCharFilterType;

  typedef itk::RescaleIntensityImageFilter<RealImageType, RealImageType> RescaleFilter;
  
  typedef itk::LaplacianRecursiveGaussianImageFilter< 
                              RealImageType, 
                              RealImageType >    LaplacianFilter;

  typedef itk::ZeroCrossingImageFilter<
                              RealImageType, 
                              RealImageType>     ZeroCrossingFilter;

  //Setting the IO
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  CastToRealFilterType::Pointer toReal = CastToRealFilterType::New();
  CastToCharFilterType::Pointer toChar = CastToCharFilterType::New();
  RescaleFilter::Pointer rescale = RescaleFilter::New();

  //Setting the ITK pipeline filter
  
  LaplacianFilter::Pointer lapFilter = LaplacianFilter::New();
  ZeroCrossingFilter::Pointer zeroFilter = ZeroCrossingFilter::New();  
  
  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );

  //The output of an edge filter is 0 or 1
  rescale->SetOutputMinimum(   0 );
  rescale->SetOutputMaximum( 255 );

  toReal->SetInput( reader->GetOutput() );
  toChar->SetInput( rescale->GetOutput() );
  writer->SetInput( toChar->GetOutput() );

  //Edge Detection by Laplacian Image Filter:

  lapFilter->SetInput( toReal->GetOutput() );
  lapFilter->SetSigma( 2.0 );
  zeroFilter->SetInput( lapFilter->GetOutput() );
  rescale->SetInput( zeroFilter->GetOutput() );


  // Test itkGetMacro
  bool bNormalizeAcrossScale = lapFilter->GetNormalizeAcrossScale();
  std::cout << "lapFilter->GetNormalizeAcrossScale(): "
            << bNormalizeAcrossScale << std::endl;

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    { 
    std::cout << "ExceptionObject caught !" << std::endl; 
    std::cout << err << std::endl; 
    return -1;
    } 

  return EXIT_SUCCESS;



}
