#pragma once

/**
 * @file CDomain.h
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5807), University of Savoie, France
 *
 * @date 2010/07/01
 *
 * Header file for concept CDomain.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(CDomain_RECURSES)
#error Recursive header files inclusion detected in CDomain.h
#else // defined(CDomain_RECURSES)
/** Prevents recursive inclusion of headers. */
#define CDomain_RECURSES

#if !defined CDomain_h
/** Prevents repeated inclusion of headers. */
#define CDomain_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "boost/concept_check.hpp"
#include "DGtal/base/Common.h"
#include "DGtal/utils/ConceptUtils.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // class CDomain
  /**
   * Description of \b concept '\b CDomain' <p> \brief Aim: This
   * concept represents a digital domain, i.e. a non mutable subset of
   * points of the given digital space.
   * 
   * <p> Refinement of
   *
   * <p> Associated types :
   * - Domain : the type itself of the CDomain model.
   * - Space : the embedding digital space.
   * - Point : the point type of the space
   * - SizeType : the type used for counting elements of the space.
   * - Vector : the vector type of the space
   * - Predicate : the type of the predicate returning true for exactly the points of this domain.  
   * - ConstIterator : the type used for iterating/visiting the points of the domain.
   * 
   * 
   * <p> Notation
   * - \t X : A type that is a model of CDomain
   * - \t x	: Object of type X
   * - \t p	: Object of type Point
   * - \t it	: Object of type ConstIterator
   *
   * <p> Definitions
   *
   * <p> Valid expressions and semantics <br>
   * <table> <tr> <td> \b Name </td> <td> \b Expression </td>
   * <td> \b Type requirements </td> <td> \b Return type </td>
   * <td> \b Precondition </td> <td> \b Semantics </td> 
   * <td> \b Postcondition </td> <td> \b Complexity </td>
   * </tr>
   * <tr> 
   * <td> Const Iterator \c begin</td> <td> it = x.begin() </td> 
   * <td> </td> <td> const ConstIterator &</td>
   * <td> </td> <td> return the iterator pointing on the first element of the domain.</td> <td> </td> <td> O(1) </td>
   * </tr>
   * <tr> 
   * <td> Const Iterator \c end</td> <td> it = x.end() </td> 
   * <td> </td> <td> const ConstIterator &</td>
   * <td> </td> <td> return the iterator pointing after the last element of the domain.</td> <td> </td> <td> O(1) </td>
   * </tr>
   * <tr> 
   * <td> lower bound</td> <td> x.lowerBound() </td> 
   * <td> </td> <td> const Point &</td>
   * <td> </td> <td> return the infimum of all points of the domain.</td> <td> </td> <td> O(1) </td>
   * </tr>
   * <tr> 
   * <td> upper bound</td> <td> x.upperBound() </td> 
   * <td> </td> <td> const Point &</td>
   * <td> </td> <td> return the supremum of all points of the domain.</td> <td> </td> <td> O(1) </td>
   * </tr>
   * <tr> 
   * <td> inside domain test</td> <td> x.isInside( p ) </td> 
   * <td> </td> <td> \c bool </td>
   * <td> </td> <td> return 'true' whenever \c p is in the domain.</td> <td> </td> <td>  </td>
   * </tr>
   * <tr> 
   * <td> inside domain predicate object</td> <td> x.predicate() </td> 
   * <td> </td> <td> const Predicate & </td>
   * <td> </td> <td> return a reference to the predicate object equivalent to the isinside(p) test.</td> <td> </td> <td>  </td>
   * </tr>
   * </table>
   *
   * <p> Invariants <br>
   *
   * <p> Models <br>
   * HyperRectDomain 
   *
   * <p> Notes <br>
   *
   * @todo Complete domain checking.
   */
  template <typename T>
  struct CDomain
  {
    // ----------------------- Concept checks ------------------------------
  public:
    typedef typename T::Domain Domain;
    typedef typename T::Space Space;
    typedef typename T::Point Point;
    typedef typename T::Vector Vector;
    typedef typename T::SizeType SizeType;
    typedef typename T::ConstIterator ConstIterator;
    typedef typename T::Predicate Predicate;

    BOOST_CONCEPT_USAGE( CDomain )
    {
      // Domain should have a lowerBound() returning a Point.
      ConceptUtils::sameType( myP, myT.lowerBound() );
      // Domain should have an upperBound() returning a Point.
      ConceptUtils::sameType( myP, myT.upperBound() );
      // Domain should have a isInside(p) returning a bool.
      ConceptUtils::sameType( myBool, myT.isInside( myP ) );
      // Domain should have a predicate() returning a Predicate.
      ConceptUtils::sameType( myPred, myT.predicate() );
      // Domain should have a begin() returning an iterator.
      ConceptUtils::sameType( myIt, myT.begin() );
      // Domain should have a end() returning an iterator.
      ConceptUtils::sameType( myIt, myT.end() );
    }

    // ------------------------- Private Datas --------------------------------
  private:
    T myT;
    Point myP;
    Predicate myPred;
    bool myBool;
    ConstIterator myIt;

    // ------------------------- Internals ------------------------------------
  private:
    
  }; // end of concept CDomain
  
} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/kernel/domains/CDomain.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CDomain_h

#undef CDomain_RECURSES
#endif // else defined(CDomain_RECURSES)