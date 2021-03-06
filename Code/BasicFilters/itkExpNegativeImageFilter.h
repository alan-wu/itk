/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkExpNegativeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:19 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkExpNegativeImageFilter_h
#define __itkExpNegativeImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{
  
/** \class ExpNegativeImageFilter
 * \brief Computes the function vcl_exp(-K.x) pixel-wise
 *
 * Every output pixel is equal to vcl_exp(-K.x ). where x is the intensity of the
 * homologous input pixel, and K is a user-provided constant.
 * 
 * \ingroup IntensityImageFilters  Multithreaded
 *
 */

namespace Function {  
template< class TInput, class TOutput>
class ExpNegative
{
public:
  ExpNegative() { m_Factor = 1.0; }
  ~ExpNegative() {};

  bool operator!=( const ExpNegative & other ) const
    {
    if( m_Factor != other.m_Factor )
      {
      return true;
      }
    return false;
    }
  bool operator==( const ExpNegative & other ) const
    {
    return !(*this != other);
    }
  
  inline TOutput operator()( const TInput & A ) const
    {
    return static_cast<TOutput>( vcl_exp(- m_Factor * static_cast<double>(A) ) );
    }

  void SetFactor( double factor )
    {
    m_Factor = factor;
    }
  double GetFactor() const
    {
    return m_Factor;
    }
private:
  double  m_Factor;
}; 
}
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ExpNegativeImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Function::ExpNegative< 
  typename TInputImage::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ExpNegativeImageFilter                                Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage, 
    Function::ExpNegative< typename TInputImage::PixelType, 
                           typename TOutputImage::PixelType> >  Superclass;
  typedef SmartPointer<Self>                                    Pointer;
  typedef SmartPointer<const Self>                              ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Runtime information support. */
  itkTypeMacro(ExpNegativeImageFilter, 
               UnaryFunctorImageFilter);

  void SetFactor( double factor )
    {
    if( factor == this->GetFunctor().GetFactor() ) 
      {
      return;
      }
    this->GetFunctor().SetFactor( factor );
    this->Modified();
    }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToDoubleCheck,
    (Concept::Convertible<typename TInputImage::PixelType, double>));
  itkConceptMacro(DoubleConvertibleToOutputCheck,
    (Concept::Convertible<double, typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  ExpNegativeImageFilter() {}
  virtual ~ExpNegativeImageFilter() {}

private:
  ExpNegativeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
