/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOpeningByReconstructionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 18:14:36 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOpeningByReconstructionImageFilter_h
#define __itkOpeningByReconstructionImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/** \class OpeningByReconstructionImageFilter
 * \brief Opening by reconstruction of an image
 *
 * This filter preserves regions, in the foreground, that can
 * completely contain the structuring element. At the same time, this
 * filter eliminates all other regions of foreground pixels. Contrary
 * to the mophological opening, the opening by reconstruction
 * preserves the shape of the components that are not removed by
 * erosion.  The opening by reconstruction of an image "f" is defined
 * as:
 *
 *   OpeningByReconstruction(f) = DilationByRecontruction(f, Erosion(f)).
 *
 * Opening by reconstruction not only removes structures destroyed by
 * the erosion, but also levels down the contrast of the brightest
 * regions. If PreserveIntensities is on, a subsequent reconstruction
 * by dilation using a marker image that is the original image for all
 * unaffected pixels.
 *
 * Opening by reconstruction is described in Chapter 6.3.9 of Pierre
 * Soille's book "Morphological Image Analysis: Principles and
 * Applications", Second Edition, Springer, 2003.
 * 
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa GrayscaleMorphologicalOpeningImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT OpeningByReconstructionImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef OpeningByReconstructionImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>
                                             Superclass;
  typedef SmartPointer<Self>                 Pointer;
  typedef SmartPointer<const Self>           ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  
 /** Kernel typedef. */
  typedef TKernel KernelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(OpeningByReconstructionImageFilter, 
               ImageToImageFilter);

  /** Set kernel (structuring element). */
  itkSetMacro(Kernel, KernelType);
  
  /** Get the kernel (structuring element). */
  itkGetConstReferenceMacro(Kernel, KernelType);

  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);
  
  /**
   * Set/Get whether the original intensities of the image retained for
   * those pixels unaffected by the opening by reconstrcution. If Off,
   * the output pixel contrast will be reduced. */
  itkSetMacro(PreserveIntensities, bool);
  itkGetConstReferenceMacro(PreserveIntensities, bool);
  itkBooleanMacro(PreserveIntensities);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputEqualityComparableCheck,
                  (Concept::EqualityComparable<InputImagePixelType>));
  /** End concept checking */
#endif

protected:
  OpeningByReconstructionImageFilter();
  ~OpeningByReconstructionImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** OpeningByReconstructionImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** OpeningByReconstructionImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  void GenerateData();
  

private:
  OpeningByReconstructionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** kernel or structuring element to use. */
  KernelType          m_Kernel;
  bool                m_FullyConnected;
  bool                m_PreserveIntensities;
}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOpeningByReconstructionImageFilter.txx"
#endif

#endif
