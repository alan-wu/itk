/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConstNeighborhoodIterator.h,v $
  Language:  C++
  Date:      $Date: 2008-10-07 12:04:17 $
  Version:   $Revision: 1.38 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConstNeighborhoodIterator_h
#define __itkConstNeighborhoodIterator_h

#include <vector>
#include <string.h>
#include <iostream>
#include "itkImage.h"
#include "itkIndex.h"
#include "itkOffset.h"
#include "itkSize.h"
#include "itkImageRegion.h"
#include "itkMacro.h"
#include "itkNeighborhood.h"
#include "itkImageBoundaryCondition.h"
#include "itkExceptionObject.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace itk {

/** \class ConstNeighborhoodIterator
 *
 * \brief Const version of NeighborhoodIterator, defining iteration of a local
 * N-dimensional neighborhood of pixels across an itk::Image.
 *
 * ConstNeighborhoodIterator implements the read-only methods of
 * NeighborhoodIterator.  It serves as a base class from which other iterators
 * are derived. See NeighborhoodIterator for more complete information.
 *
 *
 * \ingroup ImageIterators
 *
 * \sa Neighborhood \sa ImageIterator \sa NeighborhoodIterator
 */
template<class TImage,  class TBoundaryCondition
                       = ZeroFluxNeumannBoundaryCondition<TImage> >
class ITK_EXPORT ConstNeighborhoodIterator
  :  public Neighborhood<ITK_TYPENAME TImage::InternalPixelType *,
                         ::itk::GetImageDimension<TImage>::ImageDimension>
{
public:
  /** Extract image type information. */
  typedef typename TImage::InternalPixelType InternalPixelType;
  typedef typename TImage::PixelType         PixelType;
    
  /** Save the image dimension. */
  itkStaticConstMacro(Dimension, unsigned int, TImage::ImageDimension);
  
  /** Standard class typedefs. */
  typedef ConstNeighborhoodIterator               Self;
  typedef Neighborhood<InternalPixelType *, 
            itkGetStaticConstMacro(Dimension)>    Superclass;

  /** Inherit typedefs from superclass */
  typedef typename Superclass::OffsetType      OffsetType;
  typedef typename OffsetType::OffsetValueType OffsetValueType;
  typedef typename Superclass::RadiusType      RadiusType;  
  typedef typename Superclass::SizeType        SizeType;
  typedef typename Superclass::SizeValueType   SizeValueType;
  typedef typename Superclass::Iterator        Iterator;
  typedef typename Superclass::ConstIterator   ConstIterator;
  
  /** Typedef support for common objects */
  typedef TImage                                   ImageType;
  typedef typename TImage::RegionType              RegionType;
  typedef Index<itkGetStaticConstMacro(Dimension)> IndexType;
  typedef typename IndexType::IndexValueType       IndexValueType;
  typedef Neighborhood<PixelType, itkGetStaticConstMacro(Dimension)> 
                                                   NeighborhoodType;

  /** Typedef for the functor used to access neighborhoods of pixel pointers.
   * This is obtained as a trait from the image and is different for Image
   * and VectorImage. */
  typedef typename ImageType::NeighborhoodAccessorFunctorType 
                                              NeighborhoodAccessorFunctorType;

  /** Typedef for boundary condition type. */
  typedef TBoundaryCondition BoundaryConditionType;
  
  /** Typedef for generic boundary condition pointer */
  typedef ImageBoundaryCondition<ImageType> *ImageBoundaryConditionPointerType;
  typedef ImageBoundaryCondition<ImageType> const *
                                        ImageBoundaryConditionConstPointerType;

  /** Default constructor */
  ConstNeighborhoodIterator();

  /** Virtual destructor */
  virtual ~ConstNeighborhoodIterator() {}

  /** Copy constructor */
  ConstNeighborhoodIterator( const ConstNeighborhoodIterator & );

  /** Constructor which establishes the region size, neighborhood, and image
   * over which to walk. */
  ConstNeighborhoodIterator(const SizeType &radius,
                       const ImageType * ptr,
                       const RegionType &region)
    {
    this->Initialize(radius, ptr, region);
    for (unsigned int i=0; i < Dimension; i++)
      { m_InBounds[i] = false; }
    this->ResetBoundaryCondition();
    m_NeighborhoodAccessorFunctor = ptr->GetNeighborhoodAccessor();
    m_NeighborhoodAccessorFunctor.SetBegin( ptr->GetBufferPointer() );
    }

  /** Assignment operator */
  Self &operator=(const Self& orig);

  /** Standard itk print method */
  virtual void PrintSelf(std::ostream &, Indent) const;

  /** Computes the internal, N-d offset of a pixel array position n from 
   * (0,0, ..., 0) in the "upper-left" corner of the neighborhood. */
  OffsetType ComputeInternalIndex(unsigned int n) const;

  /** Returns the array of upper loop bounds used during iteration. */
  IndexType GetBound() const
    {    return m_Bound;   }

  /** Returns the loop bound used to define the edge of a single
   * dimension in the itk::Image region. */
  long GetBound(unsigned int n) const
    {    return m_Bound[n];  }
  
  /** Returns the pointer to the center pixel of the neighborhood. */
  const InternalPixelType *GetCenterPointer() const
    {    return (this->operator[]((this->Size())>>1));  }
  
  /** Returns the pixel referenced at the center of the 
   *  ConstNeighborhoodIterator. */
  PixelType GetCenterPixel() const
    {return m_NeighborhoodAccessorFunctor.Get( this->GetCenterPointer() );}

  /** Returns a smartpointer to the image on which this iterator operates. */
  const ImageType * GetImagePointer(void) const
    { return m_ConstImage; }
 
  /** Returns the N-dimensional index of the iterator's position in
   * the image. */
  virtual IndexType GetIndex(void) const
    { return m_Loop;  }
  
  /** Virtual function that "dereferences" a ConstNeighborhoodIterator,
   * returning a Neighborhood of pixel values. */
  virtual NeighborhoodType GetNeighborhood() const;

  /** Returns the pixel value located at a linear array location i. */
  virtual PixelType GetPixel(const unsigned i) const
    { 
    if( !m_NeedToUseBoundaryCondition )
      {
      return ( m_NeighborhoodAccessorFunctor.Get( this->operator[]( i ) ) );
      }
    bool inbounds; 
    return this->GetPixel( i, inbounds ); 
    }

  /** Return the pixel value located at a linear array location i.
   * Sets "IsInBounds" to true if the location is inside the
   * image and the pixel value returned is an actual pixel in the
   * image. Sets "IsInBounds" to false if the location is outside the
   * image and the pixel value returned is a boundary condition. */
  virtual PixelType GetPixel(const unsigned i, bool& IsInBounds) const;

  /** Returns the pixel value located at the itk::Offset o from the center of
      the neighborhood. */
  virtual PixelType GetPixel(const OffsetType &o) const
    { 
    bool inbounds; 
    return (this->GetPixel(this->GetNeighborhoodIndex(o), inbounds)); 
    }

  /** Returns the pixel value located at the itk::Offset o from the center of
   * the neighborhood. Sets "IsInBounds" to true if the offset is inside the
   * image and the pixel value returned is an actual pixel in the
   * image. Sets "IsInBounds" to false if the offset is outside the
   * image and the pixel value returned is a boundary condition. */
  virtual PixelType GetPixel(const OffsetType &o,
                             bool& IsInBounds) const
    {return (this->GetPixel(this->GetNeighborhoodIndex(o), IsInBounds)); }
  
  /** Returns the pixel value located i pixels distant from the neighborhood 
   *  center in the positive specified ``axis'' direction. No bounds checking 
   *  is done on the size of the neighborhood. */
  virtual PixelType GetNext(const unsigned axis, const unsigned i) const
    { return (this->GetPixel(this->GetCenterNeighborhoodIndex()
                           + (i * this->GetStride(axis)))); }

  /** Returns the pixel value located one pixel distant from the neighborhood
   *  center in the specifed positive axis direction. No bounds checking is 
   *  done on the size of the neighborhood. */
  virtual PixelType GetNext(const unsigned axis) const
    { return (this->GetPixel(this->GetCenterNeighborhoodIndex()
                           + this->GetStride(axis))); }

  /** Returns the pixel value located i pixels distant from the neighborhood 
   *  center in the negative specified ``axis'' direction. No bounds checking 
   *  is done on the size of the neighborhood. */
  virtual PixelType GetPrevious(const unsigned axis, const unsigned i) const
    { return (this->GetPixel(this->GetCenterNeighborhoodIndex()
                           - (i * this->GetStride(axis)))); }
  
  /** Returns the pixel value located one pixel distant from the neighborhood 
   *  center in the specifed negative axis direction. No bounds checking is 
   *  done on the size of the neighborhood. */
  virtual PixelType GetPrevious(const unsigned axis) const
    { return (this->GetPixel(this->GetCenterNeighborhoodIndex()
                           - this->GetStride(axis))); } 
  
  /** Returns the image index for neighbor pixel at offset o from the center of
      the neighborhood. */
  virtual IndexType GetIndex(const OffsetType &o) const
  { return (this->GetIndex() + o); }

  /** Returns the image index for neighbor pixel at index i in the
      neighborhood. */
  virtual IndexType GetIndex(const unsigned i) const
  { return (this->GetIndex() + this->GetOffset(i)); }
  
  /**  Returns the region of iteration. */
  RegionType GetRegion() const
    { return m_Region; }
  
  /** Returns the N-dimensional starting index of the iterator's position on
   * the image. */
  IndexType GetBeginIndex() const
    { return m_BeginIndex; }

  /** Returns a bounding box for the region spanned by this neighborhood
      represented by an itk::ImageRegion */
  RegionType GetBoundingBoxAsImageRegion() const;
  
  /** Returns the offsets used to wrap across dimensional boundaries. */
  OffsetType GetWrapOffset() const
    {  return m_WrapOffset;  }

  /** Returns the internal offset associated with wrapping around a single
   * dimension's region boundary in the itk::Image.  An offset for each
   * dimension is necessary to shift pointers when wrapping around region
   * edges because region memory is not necessarily contiguous within the
   * buffer. */
  OffsetValueType GetWrapOffset(unsigned int n) const
    {    return m_WrapOffset[n];   }

  /** Virtual method for rewinding the iterator to its beginning pixel.
   * This is useful for writing functions which take neighborhood iterators
   * of arbitrary type and must use virtual functions. */
  virtual void GoToBegin();
  
  /** Virtual method for sending the iterator to one past the last pixel in its
   * region. */
  virtual void GoToEnd();
  
  /** Initializes the iterator to walk a particular image and a particular
   * region of that image. */
  virtual void Initialize(const SizeType &radius, const ImageType *ptr,
                          const RegionType &region);

  /** Virtual method for determining whether the iterator is at the
   * beginning of its iteration region. */
  virtual bool IsAtBegin() const
    {    return ( this->GetCenterPointer() == m_Begin );   }
  
  /** Virtual method for determining whether the iterator has reached the
   * end of its iteration region. */
  virtual bool IsAtEnd() const
    {
    if ( this->GetCenterPointer() > m_End )
      {
      ExceptionObject e(__FILE__, __LINE__);
      OStringStream msg;
      msg << "In method IsAtEnd, CenterPointer = " << this->GetCenterPointer()
          << " is greater than End = " << m_End
          << std::endl
          << "  " << *this;
      e.SetDescription(msg.str().c_str());
      throw e;
      }
    return ( this->GetCenterPointer() == m_End );
    }
  
  /** Increments the pointers in the ConstNeighborhoodIterator,
   * wraps across boundaries automatically, accounting for
   * the disparity in the buffer size and the region size of the
   * image. */
  Self &operator++();
  
  /** Decrements the pointers in the ConstNeighborhoodIterator,
   * wraps across boundaries automatically, accounting for
   * the disparity in the buffer size and the region size of the
   * image. */
  Self &operator--();  
 
  /** Returns a boolean == comparison of the memory addresses of the center
   * elements of two ConstNeighborhoodIterators of like pixel type and
   * dimensionality.  The radii of the iterators are ignored. */
  bool operator==(const Self &it) const 
    {   return  it.GetCenterPointer() == this->GetCenterPointer();   }
  
  /** Returns a boolean != comparison of the memory addresses of the center
   * elements of two ConstNeighborhoodIterators of like pixel type and
   * dimensionality.  The radii of the iterators are ignored. */
  bool operator!=(const Self &it) const
    {    return  it.GetCenterPointer() != this->GetCenterPointer();  }
  
  /** Returns a boolean < comparison of the memory addresses of the center
   * elements of two ConstNeighborhoodIterators of like pixel type and
   * dimensionality.  The radii of the iterators are ignored. */
  bool operator<(const Self &it) const
    {  return  this->GetCenterPointer() < it.GetCenterPointer();  }

  /** Returns a boolean < comparison of the memory addresses of the center
   * elements of two ConstNeighborhoodIterators of like pixel type and
   * dimensionality.  The radii of the iterators are ignored. */
  bool operator<=(const Self &it) const
    {    return  this->GetCenterPointer() <= it.GetCenterPointer();  }
  
  /** Returns a boolean > comparison of the memory addresses of the center
   * elements of two ConstNeighborhoodIterators of like pixel type and
   * dimensionality.  The radii of the iterators are ignored. */
  bool operator>(const Self &it) const
    {    return  this->GetCenterPointer() > it.GetCenterPointer();  }

  /** Returns a boolean >= comparison of the memory addresses of the center
   * elements of two ConstNeighborhoodIterators of like pixel type and
   * dimensionality.  The radii of the iterators are ignored. */
  bool operator>=(const Self &it) const
    {    return  this->GetCenterPointer() >= it.GetCenterPointer();  }

  /** This method positions the iterator at an indexed location in the
   * image. SetLocation should _NOT_ be used to update the position of the
   * iterator during iteration, only for initializing it to a position
   * prior to iteration.  This method is not optimized for speed. */
  void SetLocation( const IndexType& position )
    {
    this->SetLoop(position);
    this->SetPixelPointers(position);
    }
  

  /** Addition of an itk::Offset.  Note that this method does not do any bounds
   * checking.  Adding an offset that moves the iterator out of its assigned
   * region will produce undefined results. */
  Self &operator+=(const OffsetType &);

  /** Subtraction of an itk::Offset. Note that this method does not do any 
   *  bounds checking.  Subtracting an offset that moves the iterator out 
   * of its assigned region will produce undefined results. */
  Self &operator-=(const OffsetType &);

  /** Distance between two iterators */
  OffsetType operator-(const Self& b)
    {  return m_Loop - b.m_Loop;  }

  /** Returns false if the iterator overlaps region boundaries, true
   * otherwise.  Also updates an internal boolean array indicating
   * which of the iterator's faces are out of bounds. */
  bool InBounds() const;

  /** Allows a user to override the internal boundary condition. Care should
   * be taken to ensure that the overriding boundary condition is a persistent
   * object during the time it is referenced.  The overriding condition
   * can be of a different type than the default type as long as it is
   * a subclass of ImageBoundaryCondition. */
  virtual void OverrideBoundaryCondition(const 
                                         ImageBoundaryConditionPointerType i)
    { m_BoundaryCondition = i; }

  /** Resets the boundary condition to the internal, default conditions
   * specified by the template parameter. */
  virtual void ResetBoundaryCondition()
    { m_BoundaryCondition = &m_InternalBoundaryCondition;  }

  /** Sets the internal, default boundary condition. */
  void SetBoundaryCondition( const TBoundaryCondition &c )
  { m_InternalBoundaryCondition = c; }

  /** */
  const BoundaryConditionType *GetBoundaryCondition() const
  { return dynamic_cast<BoundaryConditionType *>(m_BoundaryCondition); }

  /** */
  void NeedToUseBoundaryConditionOn()
    {
    this->SetNeedToUseBoundaryCondition(true);
    }
  void NeedToUseBoundaryConditionOff()
    {
    this->SetNeedToUseBoundaryCondition(false);
    }
  void SetNeedToUseBoundaryCondition(bool b)
    {
    m_NeedToUseBoundaryCondition = b;
    }
  bool GetNeedToUseBoundaryCondition() const
    {
    return m_NeedToUseBoundaryCondition;
    }
  
protected:
  
  /** Default method for setting the coordinate location of the iterator.
   * Loop indicies correspond to the actual Image region index. */
  virtual void SetLoop( const IndexType& p )
    {  m_Loop = p; m_IsInBoundsValid = false;}
  
  /** Virtual method for setting internal loop boundaries.  This
   * method must be defined in each subclass because
   * each subclass may handle loop boundaries differently. */
  virtual void SetBound(const SizeType &);

  /** Default method for setting the values of the internal pointers
   * to itk::Image memory buffer locations.  This method should
   * generally only be called when the iterator is initialized.
   * \sa SetLocation */
  virtual void SetPixelPointers(const IndexType &);

  /** Default method for setting the index of the first pixel in the
   * iteration region. */
  virtual void SetBeginIndex( const IndexType& start)
    {  m_BeginIndex = start;  }

  /** Default method for setting the index of the first pixel in the
   * iteration region. */
  virtual void SetEndIndex();
  
  /** The starting index for iteration within the itk::Image region
   * on which this ConstNeighborhoodIterator is defined. */
  IndexType m_BeginIndex;

  /** An array of upper looping boundaries used during iteration. */
  IndexType m_Bound;

  /** A pointer to the first pixel in the iteration region. */
  const InternalPixelType *m_Begin;
  
  /** The image on which iteration is defined. */
  typename ImageType::ConstWeakPointer m_ConstImage;

  /** A pointer to one past the last pixel in the iteration region. */
  const InternalPixelType *m_End;

  /** The end index for iteration within the itk::Image region
   * on which this ConstNeighborhoodIterator is defined. */
  IndexType m_EndIndex;

  /** Array of loop counters used during iteration. */
  IndexType m_Loop;
 
  /** The region over which iteration is defined. */
  RegionType m_Region;

  /** The internal array of offsets that provide support for regions of 
   *  interest.
   *  An offset for each dimension is necessary to shift pointers when wrapping
   *  around region edges because region memory is not necessarily contiguous
   *  within the buffer. */
  OffsetType m_WrapOffset;

  /** Pointer to the actual boundary condition that will be used.
   * By default this points to m_BoundaryCondition, but
   * OverrideBoundaryCondition allows a user to point this variable an external
   * boundary condition.  */
  ImageBoundaryConditionPointerType m_BoundaryCondition;

  /** Denotes which of the iterators dimensional sides spill outside
   * region of interest boundaries. */
  mutable bool m_InBounds[Dimension];

  /** Denotes if iterator is entirely within bounds */
  mutable bool m_IsInBounds;
  
  /** Is the m_InBounds and m_IsInBounds variables up to date? Set to
   * false whenever the iterator is repositioned.  Set to true within
   * InBounds(). */
  mutable bool m_IsInBoundsValid;
  
  /** Lower threshold of in-bounds loop counter values. */
  IndexType m_InnerBoundsLow;
  
  /** Upper threshold of in-bounds loop counter values. */
  IndexType m_InnerBoundsHigh;

  /** Default boundary condition. */
  TBoundaryCondition m_InternalBoundaryCondition;

  /** Does the specified region need to worry about boundary conditions? */
  bool m_NeedToUseBoundaryCondition;

  /** Functor type used to access neighborhoods of pixel pointers */
  NeighborhoodAccessorFunctorType m_NeighborhoodAccessorFunctor;

};

template<class TImage>
inline ConstNeighborhoodIterator<TImage>
operator+(const ConstNeighborhoodIterator<TImage> &it,
          const typename ConstNeighborhoodIterator<TImage>
          ::OffsetType &ind)
{
  ConstNeighborhoodIterator<TImage> ret;
  ret = it;
  ret += ind;
  return ret;
}

template<class TImage>
inline ConstNeighborhoodIterator<TImage>
operator+(const typename ConstNeighborhoodIterator<TImage>
          ::OffsetType &ind,
          const ConstNeighborhoodIterator<TImage> &it)
{  return (it + ind); }

template<class TImage>
inline ConstNeighborhoodIterator<TImage>
operator-(const ConstNeighborhoodIterator<TImage> &it,
          const typename ConstNeighborhoodIterator<TImage>
          ::OffsetType &ind)
{
  ConstNeighborhoodIterator<TImage> ret;
  ret = it;
  ret -= ind;
  return ret;
}

} // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_ConstNeighborhoodIterator(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT ConstNeighborhoodIterator< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef ConstNeighborhoodIterator< ITK_TEMPLATE_2 x > \
                        ConstNeighborhoodIterator##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkConstNeighborhoodIterator+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkConstNeighborhoodIterator.txx"
#endif

/*
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkConstNeighborhoodIterator.txx"
#endif
*/
#endif
