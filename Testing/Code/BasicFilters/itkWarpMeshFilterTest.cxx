/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkWarpMeshFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2009-09-17 11:14:57 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkWarpMeshFilter.h"
#include "itkRegularSphereMeshSource.h"
#include "itkMesh.h"
#include "itkImage.h"

int itkWarpMeshFilterTest(int, char* [] ) 
{
  const unsigned int Dimension = 3;
  
  // Declare the type of the input and output mesh
  typedef itk::DefaultStaticMeshTraits<
    double, Dimension, Dimension, double, double, double> MeshTraits;
  typedef itk::Mesh<double,Dimension,MeshTraits> MeshType;


  // declare triangle mesh source
  typedef itk::RegularSphereMeshSource< MeshType >  SphereMeshSourceType;
  typedef SphereMeshSourceType::PointType PointType;
  typedef SphereMeshSourceType::VectorType VectorType;

  SphereMeshSourceType::Pointer  sphereMeshSource = SphereMeshSourceType::New();

  PointType center; 
  center[0] = 25.0;
  center[1] = 25.0;
  center[2] = 25.0;

  VectorType scale;
  scale[0] = 20.0;
  scale[1] = 20.0;
  scale[2] = 20.0;
  
  sphereMeshSource->SetCenter(center);
  sphereMeshSource->SetScale(scale);
  sphereMeshSource->SetResolution(2);


  // Declare the deformation field 
  typedef itk::Vector< double, Dimension> VectorType;
  typedef itk::Image< VectorType, Dimension > DeformationFieldType;
  
  DeformationFieldType::Pointer deformationField = DeformationFieldType::New();

  DeformationFieldType::IndexType  start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;

  DeformationFieldType::SizeType   size;
  size[0] = 25;
  size[1] = 25;
  size[2] = 25;

  DeformationFieldType::RegionType region;
  region.SetSize( size );
  region.SetIndex( start );

  deformationField->SetRegions( region );

  DeformationFieldType::PointType  origin;
  origin[0] = 0.0;
  origin[1] = 0.0;
  origin[2] = 0.0;
 
  deformationField->SetOrigin( origin );

  DeformationFieldType::SpacingType  spacing;
  spacing[0] = 2.0;
  spacing[1] = 2.0;
  spacing[2] = 2.0;
 
  deformationField->SetSpacing( spacing );

  deformationField->Allocate();

  VectorType simpleVector; // set up an uniform vector field
  simpleVector[0] = 3.0;
  simpleVector[1] = 5.0;
  simpleVector[2] = 7.0;
  
  deformationField->FillBuffer( simpleVector );

  // Declare the Warping filter
  typedef itk::WarpMeshFilter< 
            MeshType, MeshType, DeformationFieldType >
                                           WarpFilterType;
  
  WarpFilterType::Pointer warpFilter = WarpFilterType::New();

  warpFilter->SetInput( sphereMeshSource->GetOutput() );

  warpFilter->SetDeformationField( deformationField );
      
  try
    {
    warpFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception: " << excp << std::endl;
    return EXIT_FAILURE;
    }
return EXIT_SUCCESS;

  MeshType::Pointer outputMesh = warpFilter->GetOutput();
  MeshType::ConstPointer inputMesh  = warpFilter->GetInput();

  MeshType::PointsContainerPointer outPoints = outputMesh->GetPoints();
  MeshType::PointsContainerConstPointer inPoints  = inputMesh->GetPoints();

  MeshType::PointsContainer::ConstIterator  inputPoint  = inPoints->Begin();
  MeshType::PointsContainer::ConstIterator  outputPoint = outPoints->Begin();

  MeshType::PointsContainer::ConstIterator  lastInputPoint  = inPoints->End();
  MeshType::PointsContainer::ConstIterator  lastOutputPoint = outPoints->End();

  const double tolerance = 1e-8;

  while( inputPoint != lastInputPoint && outputPoint != lastOutputPoint )
    {
    for(unsigned int i=0; i<Dimension; i++)
      {
      const double distance = outputPoint.Value()[i] - inputPoint.Value()[i];
      if( vcl_fabs( distance - simpleVector[i] ) > tolerance )
        {
        std::cerr << "Filter failed" << std::endl;
        std::cerr << "Expected displacement = " << simpleVector[i] << std::endl;
        std::cerr << "but got  = " << distance << std::endl;
        return EXIT_FAILURE;
        }
      }
    ++inputPoint;
    ++outputPoint;
    }
      
  return EXIT_SUCCESS;

}
