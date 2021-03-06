/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGaborImageSource.h,v $
  Language:  C++
  Date:      $Date: 2009-10-03 16:15:16 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGaborImageSource_h
#define __itkGaborImageSource_h

#include "itkImageSource.h"
#include "itkFixedArray.h"

namespace itk
{

/** \class GaborImageSource
 * \brief Generate an n-dimensional image of a Gabor filter.
 *
 * GaborImageSource generates an image of either the real 
 * (i.e. symmetric) or complex (i.e. antisymmetric) part 
 * of the Gabor filter with the orientation directed along 
 * the x-axis.  The GaborKernelFunction is used to evaluate 
 * the contribution along the x-axis whereas a non-normalized
 * 1-D Gaussian envelope provides the contribution in each of 
 * the remaining N dimensions.  Orientation can be manipulated 
 * via the Transform classes of the toolkit.  
 *
 * The output image may be of any dimension. 
 *
 * This implementation was contributed as a paper to the Insight Journal
 * http://hdl.handle.net/1926/500
 *
 * \ingroup DataSources
 */
template <typename TOutputImage>
class ITK_EXPORT GaborImageSource : public ImageSource<TOutputImage>
{
public:

  /** Standard class typedefs. */
  typedef GaborImageSource                       Self;
  typedef ImageSource<TOutputImage>              Superclass;
  typedef SmartPointer<Self>                     Pointer;
  typedef SmartPointer<const Self>               ConstPointer;

  /** Output image typedefs */
  typedef TOutputImage                            OutputImageType;
  typedef typename OutputImageType::PixelType     PixelType;
  typedef typename OutputImageType::RegionType    RegionType;
  typedef typename OutputImageType::SpacingType   SpacingType;
  typedef typename OutputImageType::PointType     PointType;
  typedef typename OutputImageType::DirectionType DirectionType;

  typedef typename RegionType::SizeType          SizeType;

  /** Run-time type information (and related methods). */
  itkTypeMacro( GaborImageSource, ImageSource );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );
  
  /** Dimensionality of the output image */
  itkStaticConstMacro( ImageDimension, unsigned int, 
                       OutputImageType::ImageDimension );

  /** Type used to store gabor parameters. */
  typedef FixedArray<double, 
    itkGetStaticConstMacro( ImageDimension )>    ArrayType;
  
  itkSetMacro( Size, SizeType );
  itkGetConstReferenceMacro( Size, SizeType );
  
  itkSetMacro( Spacing, SpacingType );
  itkGetConstReferenceMacro( Spacing, SpacingType );

  itkSetMacro( Origin, PointType );
  itkGetConstReferenceMacro( Origin, PointType );

  itkSetMacro( Direction, DirectionType );
  itkGetConstReferenceMacro( Direction, DirectionType );

  itkSetMacro( Sigma, ArrayType );
  itkGetConstReferenceMacro( Sigma, ArrayType );

  itkSetMacro( Mean, ArrayType );
  itkGetConstReferenceMacro( Mean, ArrayType );
  
  itkSetMacro( Frequency, double );
  itkGetConstReferenceMacro( Frequency, double );
  
  itkSetMacro( CalculateImaginaryPart, bool );
  itkGetConstReferenceMacro( CalculateImaginaryPart, bool );
  itkBooleanMacro( CalculateImaginaryPart );

protected:
  GaborImageSource();
  ~GaborImageSource();
  void PrintSelf(std::ostream& os, Indent indent) const;
  void GenerateData();
  virtual void GenerateOutputInformation();

private:
  GaborImageSource( const GaborImageSource& ); //purposely not implemented
  void operator=( const GaborImageSource& ); //purposely not implemented

  SizeType           m_Size;       //size of the output image
  SpacingType        m_Spacing;    //spacing
  PointType          m_Origin;     //origin
  DirectionType      m_Direction;  //direciton

  /** Parameters for the Gabor. */

  /** Evaluate using the complex part */
  bool m_CalculateImaginaryPart;

  /** Modulation frequency of the sine or cosine component */
  double m_Frequency;

  /** Evaluate using a stretched gabor filter (ensure zero dc response) */
  double m_PhaseOffset;
  
  /** The standard deviation in each direction. */
  ArrayType m_Sigma;

  /** The mean in each direction. */
  ArrayType m_Mean;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGaborImageSource.txx"
#endif

#endif
