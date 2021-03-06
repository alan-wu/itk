/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAffineTransformTest.cxx,v $
  Language:  C++
  Date:      $Date: 2009-04-09 09:23:18 $
  Version:   $Revision: 1.46 $

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

#include "itkAffineTransform.h"
#include "itkImage.h"
#include "vnl/vnl_vector_fixed.h"

typedef  itk::Matrix<double,2,2>   MatrixType;
typedef  itk::Vector<double,2>     VectorType;

namespace
{
  
void PrintVector( const VectorType & v )
  {
  for( unsigned int i=0; i<VectorType::Dimension; i++)
    {
    std::cout << v[i] << ", ";
    }
  std::cout << std::endl;
  }
}


int itkAffineTransformTest(int, char *[])
{


    int any = 0;       // Any errors detected in testing?

    MatrixType                   matrix2;
    MatrixType                   inverse2;
    VectorType                   vector2;

    unsigned int i, j;

    /* FIXME: This code exercises most of the methods but doesn't
       actually check that the results are correct. */

    /* Create a 2D identity transformation and show its parameters */
    typedef itk::Point<double,12> ParametersType;
    typedef itk::Matrix<double,2,12> JacobianType;
    
    typedef itk::AffineTransform<double,2> Affine2DType;
    Affine2DType::Pointer id2 = Affine2DType::New();
    matrix2 = id2->GetMatrix();
    vector2 = id2->GetOffset();
    std::cout << "Matrix from instantiating an identity transform:"
              << std::endl << matrix2;
    std::cout << "Vector from instantiating an identity transform:"
              << std::endl;
    PrintVector( vector2 );
    
    /* Create and show a simple 2D transform from given parameters */
    matrix2[0][0] = 1;
    matrix2[0][1] = 2;
    matrix2[1][0] = 3;
    matrix2[1][1] = 4;
    vector2[0] = 5;
    vector2[1] = 6;

    Affine2DType::Pointer aff2 = Affine2DType::New();
    aff2->SetMatrix( matrix2 );
    aff2->SetOffset( vector2 );
    for (i = 0; i < 2; i++)
      {
      for (j = 0; j < 2; j++)
        {
        matrix2[i][j] = 0.0;
        }
      vector2[i]    = 0.0;
      }
    std::cout << "Instantiation of a given 2D transform:" << std::endl;
    aff2->Print( std::cout );

    inverse2 = aff2->GetInverseMatrix();
    std::cout << "Inverse matrix for the given transform:"
              << std::endl << inverse2;

    /* Set parameters of a 2D transform */
    matrix2[0][0] = 6;
    matrix2[0][1] = 5;
    matrix2[1][0] = 4;
    matrix2[1][1] = 3;
    vector2[0] = 2;
    vector2[1] = 1;
    aff2->SetMatrix(matrix2);
    aff2->SetOffset(vector2);
    for (i = 0; i < 2; i++)
      {
      for (j = 0; j < 2; j++)
        {
        matrix2[i][j] = 0.0;
        }
      vector2[i]    = 0.0;
      }
    matrix2 = aff2->GetMatrix();
    vector2 = aff2->GetOffset();
    std::cout << "Setting the matrix in an existing transform:"
              << std::endl << matrix2;
    std::cout << "Setting the offset in an existing  transform:"
              << std::endl;
    PrintVector( vector2 );


    /* Try composition of two transformations */
    aff2->Compose( aff2 );
    std::cout << "Result of a composition:" << std::endl;
    aff2->Print( std::cout );

    /* Compose with a translation */
    VectorType trans;
    trans[0] = 1;
    trans[1] = 2;
    aff2->Translate(trans);
    std::cout << "Result of a translation:" << std::endl;
    aff2->Print( std::cout );

    /* Compose with an isotropic scaling */
    aff2->Scale(.3, 1);
    std::cout << "Result of isotropic scaling:" << std::endl;
    aff2->Print( std::cout );

    /* Compose with an anisotropic scaling */
    VectorType scale;
    scale[0] = .3;
    scale[1] = .2;
    aff2->Scale(scale);
    std::cout << "Result of anisotropic scaling:" << std::endl;
    aff2->Print( std::cout );

    /* Compose with a general N-D rotation */
    aff2->Rotate(0, 1, 0.57, 1);
    std::cout << "Result of general rotation:" << std::endl;
    aff2->Print( std::cout );

    /* Compose with a 2-D rotation */
    aff2->Rotate(0, 1, -0.57, 1);
    std::cout << "Result of 2-D rotation:" << std::endl;
    aff2->Print( std::cout );

    /* Compose with a shear */
    aff2->Shear(1, 0, .2);
    std::cout << "Result of shear:" << std::endl;
    aff2->Print( std::cout );

    /* Transform a point */
    itk::Point<double, 2> u2, v2;
    u2[0] = 3;
    u2[1] = 5;
    v2 = aff2->TransformPoint(u2);
    std::cout << "Transform a point:" << std::endl
              << v2[0] << " , " << v2[1] << std::endl;

    /* Back transform a point */
    //v2 = aff2->BackTransform(u2);
    //std::cout << "Back transform a point:" << std::endl
              //<< v2[0] << " , " << v2[1] << std::endl;

    /* Transform a vnl_vector */
    vnl_vector_fixed<double, 2> x2, y2;
    x2[0] = 1;
    x2[1] = 2;
    y2 = aff2->TransformVector(x2);
    std::cout << "Transform a vnl_vector:" << std::endl
              << y2[0] << " , " << y2[1] << std::endl;

    /* Back transform a vector */
    //y2 = aff2->BackTransform(x2);
    //std::cout << "Back transform a vnl_vector:" << std::endl
              //<< y2[0] << " , " << y2[1] << std::endl;

    /* Transform a vector */
    itk::Vector<double, 2> u3, v3;
    u3[0] = 3;
    u3[1] = 5;
    v3 = aff2->TransformVector(u3);
    std::cout << "Transform a vector:" << std::endl
              << v3[0] << " , " << v3[1] << std::endl;

    /* Back transform a vector */
    //v3 = aff2->BackTransform(u3);
    //std::cout << "Back transform a vector :" << std::endl
              //<< v3[0] << " , " << v3[1] << std::endl;

    /* Transform a Covariant vector */
    itk::Vector<double, 2> u4, v4;
    u4[0] = 3;
    u4[1] = 5;
    v4 = aff2->TransformVector(u4);
    std::cout << "Transform a Covariant vector:" << std::endl
              << v4[0] << " , " << v4[1] << std::endl;

    /* Back transform a vector */
    //v4 = aff2->BackTransform(u4);
    //std::cout << "Back transform a vector :" << std::endl
              //<< v4[0] << " , " << v4[1] << std::endl;



    /* Create a 3D transform and rotate in 3D */
    typedef itk::AffineTransform<double,3> Affine3DType;
    Affine3DType::Pointer aff3 = Affine3DType::New();
    itk::Vector<double,3> axis;
    axis[0] = .707;
    axis[1] = .707;
    axis[2] = .707;
    aff3->Rotate3D(axis, 1.0, 1);
    std::cout << "Create and rotate a 3D transform:" << std::endl;
    aff3->Print( std::cout );

    /* Generate inverse transform */
    Affine3DType::Pointer inv3 = Affine3DType::New();
    if(!aff3->GetInverse(inv3))
      {
      std::cout << "Cannot compute inverse transformation" << std::endl;
      return EXIT_FAILURE;
      }
    std::cout << "Create an inverse transformation:" << std::endl;
    inv3->Print( std::cout );

    Affine3DType::Pointer inv4 =
       dynamic_cast<Affine3DType*>(aff3->GetInverseTransform().GetPointer());
    if(!inv4)
      {
      std::cout << "Cannot compute inverse transformation" << std::endl;
      return EXIT_FAILURE;
      }
    std::cout << "Create an inverse transformation:" << std::endl;
    inv4->Print( std::cout );
  
    /* Test output of GetJacobian */
    Affine3DType::Pointer jaff = Affine3DType::New();
    Affine3DType::MatrixType jaffMatrix = jaff->GetMatrix();
    Affine3DType::OffsetType jaffVector = jaff->GetOffset();

    Affine3DType::InputPointType jpoint;
    jpoint[0] = 5.0;
    jpoint[1] = 10.0;
    jpoint[2] = 15.0; 
    Affine3DType::JacobianType jaffJacobian = jaff->GetJacobian( jpoint );

    std::cout << "GetJacobian: " << std::endl;
    std::cout << jaffJacobian << std::endl;
    
    /* Test SetParameters */
    Affine3DType::Pointer paff = Affine3DType::New();
    Affine3DType::ParametersType parameters1( paff->GetNumberOfParameters() );

    /* set up a 3x3 magic square matrix */
    parameters1[0] = 8;
    parameters1[1] = 1;
    parameters1[2] = 6;
    parameters1[3] = 3;
    parameters1[4] = 5;
    parameters1[5] = 7;
    parameters1[6] = 4;
    parameters1[7] = 9;
    parameters1[8] = 2;

    parameters1[9] = 5;
    parameters1[10] = 5;
    parameters1[11] = 5;
    
    paff->Print( std::cout );
    paff->SetParameters( parameters1 );
    paff->Print( std::cout );

    paff->SetIdentity();
    paff->Print( std::cout );


    {
    // Test SetParameters and GetInverse
    typedef itk::AffineTransform<double,2> TransformType;
    TransformType::Pointer transform = TransformType::New();

    TransformType::ParametersType parameters2;
    TransformType::ParametersType expectedParameters;
    expectedParameters.SetSize( transform->GetNumberOfParameters() );

    double epsilon = 1e-10;

    // check the returned parameters

    // Test 1: SetIdentity
    transform->SetIdentity();
    parameters2 = transform->GetParameters();

    expectedParameters.Fill( 0.0 );
    expectedParameters[0] = 1.0;
    expectedParameters[3] = 1.0;

    for( unsigned int k = 0; k < transform->GetNumberOfParameters(); k++ )
      {
      if( vcl_fabs( parameters2[k] - expectedParameters[k] ) > epsilon )
        {
        std::cout << "Test failed:" << std::endl;
        std::cout << "Results=" << parameters2 << std::endl;
        std::cout << "Expected=" << expectedParameters << std::endl;
        any = true;
        break;
        }
      }


    // Test 2: SetParameters
    expectedParameters.Fill( 0.0 );
    expectedParameters[0] = 2.0;
    expectedParameters[3] = 2.0;

    transform->SetParameters( expectedParameters );
    parameters2 = transform->GetParameters();

    for( unsigned int k = 0; k < transform->GetNumberOfParameters(); k++ )
      {
      if( vcl_fabs( parameters2[k] - expectedParameters[k] ) > epsilon )
        {
        std::cout << "Test failed:" << std::endl;
        std::cout << "Results=" << parameters2 << std::endl;
        std::cout << "Expected=" << expectedParameters << std::endl;
        any = true;
        break;
        }
      }


    // Test 3: GetInverse
    expectedParameters.Fill( 0.0 );
    expectedParameters[0] = 2.0;
    expectedParameters[3] = 2.0;

    transform->SetParameters( expectedParameters );

    TransformType::Pointer other = TransformType::New();
    transform->GetInverse( other );

    TransformType::Pointer otherbis = 
       dynamic_cast<TransformType*>(transform->GetInverseTransform().GetPointer());

    parameters2 = other->GetParameters();
    TransformType::ParametersType parameters2bis = otherbis->GetParameters();

    expectedParameters.Fill( 0.0 );
    expectedParameters[0] = 0.5;
    expectedParameters[3] = 0.5;
    
    other->Print( std::cout );
    otherbis->Print( std::cout );

    for( unsigned int k = 0; k < transform->GetNumberOfParameters(); k++ )
      {
      if( vcl_fabs( parameters2[k] - expectedParameters[k] ) > epsilon )
        {
        std::cout << "Test failed:" << std::endl;
        std::cout << "Results=" << parameters2 << std::endl;
        std::cout << "Expected=" << expectedParameters << std::endl;
        any = true;
        break;
        }
      }

    for( unsigned int k = 0; k < transform->GetNumberOfParameters(); k++ )
      {
      if( vcl_fabs( parameters2bis[k] - expectedParameters[k] ) > epsilon )
        {
        std::cout << "Test failed:" << std::endl;
        std::cout << "Results=" << parameters2bis << std::endl;
        std::cout << "Expected=" << expectedParameters << std::endl;
        any = true;
        break;
        }
      }


    // Try to invert a singular transform
    TransformType::Pointer singularTransform = TransformType::New();
    TransformType::Pointer singularTransformInverse = TransformType::New();
    singularTransform->Scale(0.0);
    if (!singularTransform->GetInverse(singularTransformInverse))
      {
      std::cout << "Detected an attempt to invert a singular transform as expected" << std::endl;
      }
    else
      {
      std::cout << "Failed to detect an attempt to invert a singular transform!" << std::endl;
      return EXIT_FAILURE;
      }

    TransformType::Pointer singularTransformInverse2 =
       dynamic_cast<TransformType*>(singularTransform->GetInverseTransform().GetPointer());
    if (!singularTransformInverse2)
      {
      std::cout << "Detected an attempt to invert a singular transform as expected" << std::endl;
      }
    else
      {
      std::cout << "Failed to detect an attempt to invert a singular transform!" << std::endl;
      return EXIT_FAILURE;
      }
}

    return any;
}
