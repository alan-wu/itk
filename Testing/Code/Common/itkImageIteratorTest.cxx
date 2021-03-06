/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageIteratorTest.cxx,v $
  Language:  C++
  Date:      $Date: 2008-06-04 15:13:22 $
  Version:   $Revision: 1.34 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>

#include "itkImage.h"
#include "itkVector.h"
#include "itkImageIterator.h"


// This routine is used to make sure that we call the "const" version
// of GetPixel() (via the operator[])
template <class T, unsigned int VImageDimension>
void TestConstPixelAccess(const itk::Image<T, VImageDimension> &in,
                          itk::Image<T, VImageDimension> &out)
{
  typename itk::Image<T, VImageDimension>::IndexType regionStartIndex3D = {{5, 10, 15}};
  typename itk::Image<T, VImageDimension>::IndexType regionEndIndex3D = {{8, 15, 17}};

  T vec;
  
  vec[0] = 5;
  vec[1] = 4;
  vec[2] = 3;
  vec[3] = 2;
  vec[4] = 1;

  out[regionStartIndex3D] = vec;
  out[regionEndIndex3D] = in[regionStartIndex3D];
}


int itkImageIteratorTest(int, char* [] )
{
  const unsigned int ImageDimension = 3;

  std::cout << "Creating an image" << std::endl;
  itk::Image<itk::Vector<unsigned short, 5>, ImageDimension>::Pointer
    o3 = itk::Image<itk::Vector<unsigned short, 5>, ImageDimension>::New();

  float origin3D[ImageDimension] = { 5, 2.1, 8.1};
  float spacing3D[ImageDimension] = { 1.5, 2.1, 1};

  itk::Image<itk::Vector<unsigned short, 5>, ImageDimension>::SizeType imageSize3D = {{ 20, 40, 60 }};

  itk::Image<itk::Vector<unsigned short, 5>, ImageDimension>::IndexType startIndex3D = {{5, 4, 1}};
  itk::Image<itk::Vector<unsigned short, 5>, ImageDimension>::IndexType regionStartIndex3D = {{5, 10, 12}};
  itk::Image<itk::Vector<unsigned short, 5>, ImageDimension>::IndexType regionEndIndex3D = {{8, 15, 17}};


  itk::Image<itk::Vector<unsigned short, 5>, ImageDimension>::RegionType region;
  region.SetSize(imageSize3D);
  region.SetIndex(startIndex3D);
  o3->SetRegions( region );
  o3->SetOrigin(origin3D);
  o3->SetSpacing(spacing3D);

  o3->Allocate();
  itk::Vector<unsigned short, 5> fillValue;
  fillValue.Fill(itk::NumericTraits<unsigned short>::max());
  o3->FillBuffer(fillValue);

  std::cout << "Setting/Getting a pixel" << std::endl;
  itk::Vector<unsigned short, 5> vec;
  
  vec[0] = 5;
  vec[1] = 4;
  vec[2] = 3;
  vec[3] = 2;
  vec[4] = 1;

  (*o3)[regionStartIndex3D] = vec;
  (*o3)[regionEndIndex3D] = (*o3)[regionStartIndex3D];
  TestConstPixelAccess(*o3, *o3);

  typedef itk::Vector< unsigned short, 5 >  VectorPixelType;
  typedef itk::Image< VectorPixelType, ImageDimension >  VectorImageType;
  
  typedef itk::ImageIterator<      VectorImageType >  VectorImageIterator;
  typedef itk::ImageConstIterator< VectorImageType >  VectorImageConstIterator;
  
  VectorImageIterator       itr1( o3, region );
  VectorImageConstIterator  itr2( o3, region );

  // Exercise copy constructor
  VectorImageIterator   itr3( itr1 );
  
  // Exercise assignment operator
  VectorImageIterator   itr4;
  itr4 = itr1;
  
  // Exercise operator!=
  if( itr4 != itr1 )
    {
    std::cerr << "Error in operator= or operator!=" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise operator==
  if( !( itr4 == itr1 ) )
    {
    std::cerr << "Error in operator= or operator==" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise operator<=
  if( !( itr4 <= itr1 ) )
    {
    std::cerr << "Error in operator= or operator<=" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise operator<
  if( itr4 < itr1 )
    {
    std::cerr << "Error in operator= or operator<" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise operator>=
  if( !( itr4 >= itr1 ) )
    {
    std::cerr << "Error in operator= or operator>=" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise operator>
  if( itr4 > itr1 )
    {
    std::cerr << "Error in operator= or operator>" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise GetImageIteratorDimension()
  if( itr1.GetImageIteratorDimension() != ImageDimension )
    {
    std::cerr << "Error in GetImageIteratorDimension" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise GetIndex()
  VectorImageType::IndexType index1 = itr1.GetIndex();
  if( index1 != startIndex3D )
    {
    std::cerr << "Error in GetIndex()" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise SetIndex()
  VectorImageType::IndexType index2 = index1;
  index2[0]++;
  VectorImageIterator   itr5 = itr1;
  itr5.SetIndex( index2 );
  if( itr5.GetIndex() != index2 )
    {
    std::cerr << "Error in GetIndex() and/or SetIndex()" << std::endl;
    return EXIT_FAILURE;
    }

  if( itr5.GetIndex() == itr1.GetIndex() )
    {
    std::cerr << "Error in GetIndex() and/or SetIndex()" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise GetRegion()
  VectorImageType::RegionType region1 = itr1.GetRegion();
  if( region1 != region )
    {
    std::cerr << "Error in GetRegion()" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise GetImage() non-const version
  VectorImageType * image1 = itr1.GetImage();
  if( image1 != o3.GetPointer() )
    {
    std::cerr << "Error in GetImage()" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise GetImage() const version
  const VectorImageType * image2 = itr2.GetImage();
  if( image2 != o3.GetPointer() )
    {
    std::cerr << "Error in GetImage()" << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise Get() non-const and const version
  {
  VectorPixelType vp1 = itr1.Get();
  VectorPixelType vp2 = itr2.Get();
  std::cout << "vp1: " << vp1 << std::endl;
  std::cout << "vp2: " << vp2 << std::endl;
  if( vp1 != vp2 )
    {
    std::cerr << "Error in Get()" << std::endl;
    return EXIT_FAILURE;
    }
  // verify that the value can be modified
  vp1[0]++;
  itr1.Set( vp1 );
  vp2 = itr2.Get();
  if( vp1 != vp2 )
    {
    std::cerr << "Error in Get() and/or Set()" << std::endl;
    return EXIT_FAILURE;
    }
  }

 
  // Exercise Value() const and non-const methods
  {
  VectorPixelType vp1 = itr1.Value();
  VectorPixelType vp2 = itr2.Value();
  if( vp1 != vp2 )
    {
    std::cerr << "Error in Value()" << std::endl;
    return EXIT_FAILURE;
    }
  // verify that the value can be modified
  vp1[0]++;
  itr1.Value() = vp1;
  vp2 = itr2.Value();
  if( vp1 != vp2 )
    {
    std::cerr << "Error in Get() and/or Set()" << std::endl;
    return EXIT_FAILURE;
    }
  }

  // Exercise Begin(), GoToBegin(), IsAtBegin() and IsAtEnd()
  {
  VectorImageIterator itr6 = itr1.Begin();
  itr1.GoToBegin();
  if( itr1 != itr6 )
    {
    std::cerr << "Error in Begin() and/or GoToBegin()" << std::endl;
    return EXIT_FAILURE;
    }
  if( !itr1.IsAtBegin() )
    {
    std::cerr << "Error in Begin() and/or IsAtBegin()" << std::endl;
    return EXIT_FAILURE;
    }
  if( itr1.IsAtEnd() )
    {
    std::cerr << "Error in Begin() and/or IsAtEnd()" << std::endl;
    return EXIT_FAILURE;
    }
  }

 
  // Exercise End(), GoToEnd(), IsAtBegin() and IsAtEnd()
  {
  VectorImageIterator itr7 = itr1.End();
  itr1.GoToEnd();
  if( itr1 != itr7 )
    {
    std::cerr << "Error in End() and/or GoToEnd()" << std::endl;
    return EXIT_FAILURE;
    }
  if( !itr1.IsAtEnd() )
    {
    std::cerr << "Error in End() and/or IsAtEnd()" << std::endl;
    return EXIT_FAILURE;
    }
  if( itr1.IsAtBegin() )
    {
    std::cerr << "Error in End() and/or IsAtBegin()" << std::endl;
    return EXIT_FAILURE;
    }
  }


  return EXIT_SUCCESS;
}
