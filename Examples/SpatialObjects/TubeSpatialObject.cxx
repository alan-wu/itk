/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: TubeSpatialObject.cxx,v $
  Language:  C++
  Date:      $Date: 2005-03-24 16:45:19 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

// Software Guide : BeginLatex
//
// \index{itk::TubeSpatialObject}
//
// \doxygen{TubeSpatialObject} defines an n-dimensional tube.  A tube is
// defined as a list of centerline points which have a position, a radius,
// some normals and other properties. Let's start by including the
// appropriate header file.
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkTubeSpatialObject.h"
#include "itkTubeSpatialObjectPoint.h"
// Software Guide : EndCodeSnippet

int main( int , char *[] )
{
// Software Guide : BeginLatex
//
// TubeSpatialObject is templated over the dimension of the space.  A
// TubeSpatialObject contains a list of TubeSpatialObjectPoints.
//
// First we define some type definitions and we create the tube.
//
// Software Guide : EndLatex 

  unsigned int i;

// Software Guide : BeginCodeSnippet
  typedef itk::TubeSpatialObject<3>            TubeType;
  typedef TubeType::Pointer                    TubePointer;
  typedef itk::TubeSpatialObjectPoint<3>       TubePointType;
  typedef TubePointType::CovariantVectorType   VectorType;

  TubePointer tube = TubeType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We create a point list and we set:
// \begin{enumerate}
// \item The position of each point in the local coordinate system using the 
// \code{SetPosition()} method.
// \item The radius of the tube at this position using \code{SetRadius()}.
// \item The two normals at the tube is set using \code{SetNormal1()} and 
// \code{SetNormal2()}.
// \item The color of the point is set to red in our case.
// \end{enumerate}  
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  TubeType::PointListType list;
  for( i=0; i<5; i++)
    {
    TubePointType p;
    p.SetPosition(i,i+1,i+2);
    p.SetRadius(1);
    VectorType normal1;
    VectorType normal2;
    for(unsigned int j=0;j<3;j++)
      {
      normal1[j]=j;
      normal2[j]=j*2;
      }
    
    p.SetNormal1(normal1);
    p.SetNormal2(normal2);
    p.SetColor(1,0,0,1);
  
    list.push_back(p);
    }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Next, we create the tube and set its name using \code{SetName()}. We also
// set its identification number with \code{SetId()} and, at the end, we add
// the list of points previously created.
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  tube->GetProperty()->SetName("Tube1");
  tube->SetId(1);
  tube->SetPoints(list);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The \code{GetPoints()} method return a reference to the internal list of
// points of the object.
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  TubeType::PointListType pointList = tube->GetPoints();
  std::cout << "Number of points representing the tube: ";
  std::cout << pointList.size() << std::endl;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The \code{ComputeTangentAndNormals()} function computes the normals and
// the tangent for each point using finite differences.
//
// Software Guide : EndLatex 
// Software Guide : BeginCodeSnippet
  tube->ComputeTangentAndNormals();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Then we can access the points using STL iterators.  \code{GetPosition()}
// and \code{GetColor()} functions return respectively the position and the
// color of the point. \code{GetRadius()} returns the radius at that
// point. \code{GetNormal1()} and \code{GetNormal1()} functions return a
// \doxygen{CovariantVector} and \code{GetTangent()} returns a
// \doxygen{Vector}.
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  TubeType::PointListType::const_iterator it = tube->GetPoints().begin();
  i=0;
  while(it != tube->GetPoints().end())
    {
    std::cout << std::endl;
    std::cout << "Point #" << i << std::endl;
    std::cout << "Position: " << (*it).GetPosition() << std::endl;
    std::cout << "Radius: " << (*it).GetRadius() << std::endl;
    std::cout << "Tangent: " << (*it).GetTangent() << std::endl;
    std::cout << "First Normal: " << (*it).GetNormal1() << std::endl;
    std::cout << "Second Normal: " << (*it).GetNormal2() << std::endl;
    std::cout << "Color = " << (*it).GetColor() << std::endl;
    it++;
    i++;
    }
// Software Guide : EndCodeSnippet

  return 0;
}
