/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: RelabelComponentImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2009-03-17 21:44:43 $
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

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginLatex
//
//  The \doxygen{RelabelComponentImageFilter} is commonly used for reorganizing
//  the labels in an image that has been produced as the result of a
//  segmentation method. For example, region growing, or a K-means statistical
//  classification.
//
//  \index{itk::RelabelComponentImageFilter}
//
//  Software Guide : EndLatex 


#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

//  Software Guide : BeginLatex
//
//  The header file corresponding to this filter should be included first.
//
//  \index{itk::RelabelComponentImageFilter!header}
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkRelabelComponentImageFilter.h"
// Software Guide : EndCodeSnippet


int main( int argc, char * argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile   outputImageFile" << std::endl;
    return 1;
    }


  //  Software Guide : BeginLatex
  //
  //  Then the pixel types for input and output image must be defined and, with
  //  them, the image types can be instantiated.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef   unsigned char  InputPixelType;
  typedef   unsigned char  OutputPixelType;

  typedef itk::Image< InputPixelType,  2 >   InputImageType;
  typedef itk::Image< OutputPixelType, 2 >   OutputImageType;
  // Software Guide : EndCodeSnippet


  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );

  //  Software Guide : BeginLatex
  //
  //  Using the image types it is now possible to instantiate the filter type
  //  and create the filter object. 
  //
  //  \index{itk::RelabelComponentImageFilter!instantiation}
  //  \index{itk::RelabelComponentImageFilter!New()}
  //  \index{itk::RelabelComponentImageFilter!Pointer}
  // 
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::RelabelComponentImageFilter<
               InputImageType, OutputImageType >  FilterType;

  FilterType::Pointer relabeler = FilterType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The input to the filter can be taken from any other filter, for example
  //  a reader. The output can be passed down the pipeline to other filters,
  //  for example, a writer. An update call on any downstream filter will
  //  trigger the execution of the mean filter.
  //
  //  \index{itk::RelabelComponentImageFilter!SetInput()}
  //  \index{itk::RelabelComponentImageFilter!GetOutput()}
  //
  //  Software Guide : EndLatex 


  // Software Guide : BeginCodeSnippet
  relabeler->SetInput( reader->GetOutput() );
  writer->SetInput( relabeler->GetOutput() );
  writer->Update();
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // We can now query the size of each one of the connected components, both in
  // pixel units and in physical units.
  //
  // Software Guide : EndLatex 


  // Software Guide : BeginCodeSnippet
  typedef std::vector< unsigned long > SizesInPixelsType;

  const SizesInPixelsType &  sizesInPixels = relabeler->GetSizeOfObjectsInPixels();

  SizesInPixelsType::const_iterator sizeItr = sizesInPixels.begin();
  SizesInPixelsType::const_iterator sizeEnd = sizesInPixels.end();

  std::cout << "Number of pixels per class " << std::endl;
  unsigned int kclass = 0;
  while( sizeItr != sizeEnd )
    {
    std::cout << "Class " << kclass << " = " << *sizeItr << std::endl;
    ++kclass;
    ++sizeItr;
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginCodeSnippet
  typedef std::vector< float > SizesInPhysicalUnitsType;

  const SizesInPhysicalUnitsType  sizesInUnits = relabeler->GetSizeOfObjectsInPhysicalUnits();

  SizesInPhysicalUnitsType::const_iterator physicalSizeItr = sizesInUnits.begin();
  SizesInPhysicalUnitsType::const_iterator physicalSizeEnd = sizesInUnits.end();

  std::cout << "Area in Physical Units per class " << std::endl;
  unsigned int jclass = 0;
  while( physicalSizeItr != physicalSizeEnd )
    {
    std::cout << "Class " << jclass << " = " << *physicalSizeItr << std::endl;
    ++jclass;
    ++physicalSizeItr;
    }
  // Software Guide : EndCodeSnippet

  return 0;
}
