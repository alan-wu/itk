/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshBoundaryEdgesMeshFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-02-25 21:17:07 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuadEdgeMeshBoundaryEdgesMeshFunction_h
#define __itkQuadEdgeMeshBoundaryEdgesMeshFunction_h

#include<itkFunctionBase.h>

namespace itk
{

/**
 * \class QuadEdgeMeshBoundaryEdgesMeshFunction
 *
 * \brief Build a list of references to edges (as \ref GeometricalQuadEdge::RawPointer)
 *        each one representing a different boundary component.
 * \note  Each resulting edge has the surface on its right and is hence
 *        ready for an walk on with the help of
 *        \ref QEPrimal::IteratorGeom::BeginGeomLnext().
 * \note  The size() of the resulting list is the number of boundary
 *        components.
 */
template< class TMesh >
class ITK_EXPORT QuadEdgeMeshBoundaryEdgesMeshFunction
   : public FunctionBase< TMesh, typename TMesh::EdgeListPointerType >
{
public:
  // Standard types
  typedef QuadEdgeMeshBoundaryEdgesMeshFunction  Self;
  typedef SmartPointer< Self >                   Pointer;
  typedef SmartPointer< const Self >             ConstPointer;
  typedef FunctionBase< TMesh,
                        typename  TMesh::EdgeListPointerType > Superclass;

  // Types in superclass:
  typedef typename Superclass::InputType  InputType;
  typedef typename Superclass::OutputType OutputType;

  // Local aliases
  typedef InputType                       MeshType;
  typedef typename MeshType::QEPrimal     QEPrimal;
  typedef typename MeshType::EdgeCellType EdgeCellType;
  typedef typename MeshType::EdgeListType EdgeListType;

  itkTypeMacro( QuadEdgeMeshBoundaryEdgesMeshFunction, FunctionBase );
  itkNewMacro( Self );

  virtual OutputType Evaluate( const InputType& mesh ) const;

protected:
  QuadEdgeMeshBoundaryEdgesMeshFunction( ) { };

private:
  QuadEdgeMeshBoundaryEdgesMeshFunction( const Self& ); //purposely not implemented 
  void operator=( const Self& );     //purposely not implemented 
};

} 

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkQuadEdgeMeshBoundaryEdgesMeshFunction.txx"
#endif

#endif 
