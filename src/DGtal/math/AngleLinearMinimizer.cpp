/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file AngleLinearMinimizer.cpp
 *
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5807), University of Savoie, France
 *
 * @author (backported from ImaGene by) Bertrand Kerautret (\c kerautre@loria.fr )
 * LORIA (CNRS, UMR 7503), University of Nancy, France
 *
 * @date 2011/08/31
 *
 * Implementation of methods defined in AngleLinearMinimizer.h
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include "DGtal/math/AngleLinearMinimizer.h"
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// class AngleLinearMinimizer
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :


DGtal::AngleLinearMinimizer::~AngleLinearMinimizer()
{
  reset();
}


DGtal::AngleLinearMinimizer::AngleLinearMinimizer()
  : myValues( 0 )
{
}


void 
DGtal::AngleLinearMinimizer::reset()
{
  if ( myValues != 0 )
    {
      delete[] myValues;
      myValues = 0;
    }
  mySum = 0.0;
  myMax = 0.0;
}


void 
DGtal::AngleLinearMinimizer::init( unsigned int nbMax )
{
  reset();
  myValues = new ValueInfo[ nbMax ];
  myMaxSize = nbMax;
  mySize = nbMax;
  myIsCurveOpen = false;
}




///////////////////////////////////////////////////////////////////////////////
// ------------------------- Optimization services --------------------------

double
DGtal::AngleLinearMinimizer::getEnergy( unsigned int i1, unsigned int i2 ) const
{
  ModuloComputer<unsigned int> mc( size() );
  double E = 0.0;
  for ( unsigned int i = mc.next( i1 ); i != i2; ) 
    {
      unsigned int inext = mc.next( i );
      const ValueInfo & vi = this->ro( i );
      const ValueInfo & viprev = this->ro( mc.previous( i ) );
      double dev = AngleComputer::deviation( vi.value, viprev.value ); 
      E +=  (dev * dev) / viprev.distToNext;
      i = inext;
    }
  return E;
}



double
DGtal::AngleLinearMinimizer::getFormerEnergy( unsigned int i1, unsigned int i2 ) const
{
  ModuloComputer<unsigned int> mc( size() );
  double E = 0.0;
  for ( unsigned int i = mc.next( i1 ); i != i2; ) 
    {
      unsigned int inext = mc.next( i );
      const ValueInfo & vi = this->ro( i );
      const ValueInfo & viprev = this->ro( mc.previous( i ) );
      double dev = AngleComputer::deviation( vi.oldValue, viprev.oldValue );
      E += (dev * dev)	/ viprev.distToNext;
      i = inext;
    }
  return E;
}




std::vector<double>
DGtal::AngleLinearMinimizer::getGradient() const
{
  ModuloComputer<unsigned int> mc( size() );
  
  vector<double> grad( size() );
  for ( unsigned int i = 0; i < size(); i++ ) 
    {
      unsigned int iprev = mc.previous( i );
      unsigned int inext = mc.next( i );
      const ValueInfo & vi = this->ro( i );
      double val = vi.value;
      if ( myIsCurveOpen  && ( i == ( size() - 1 ) ) )
	{ // free extremity to the front/right.
	  const ValueInfo & viprev = this->ro( iprev );
	  double valp = viprev.value; 
	  grad[ i ] = 2.0 * AngleComputer::deviation( val, valp ) / viprev.distToNext;
	}
      else if ( myIsCurveOpen && ( i ==  0 ) )
	{ // free extremity to the back/left.
	  const ValueInfo & vinext = this->ro( inext );
	  double valn = vinext.value;
	  grad[ i ] = -2.0 * AngleComputer::deviation( valn, val ) / vi.distToNext;
	}
      else
	{ // standard case.
	  const ValueInfo & viprev = this->ro( iprev );
	  const ValueInfo & vinext = this->ro( inext );
	  double valp = viprev.value; 
	  double valn = vinext.value;
	  grad[ i ] = 2.0*( AngleComputer::deviation( val, valp ) / viprev.distToNext 
			    - AngleComputer::deviation( valn, val ) / vi.distToNext ) ;
	}
    }
  return grad;
}




std::vector<double>
DGtal::AngleLinearMinimizer::getFormerGradient() const
{
  ModuloComputer<unsigned int> mc( size() );

  vector<double> grad( size() );
  for ( unsigned int i = 0; i < size(); i++ ) 
    {
      unsigned int iprev = mc.previous( i );
      unsigned int inext = mc.next( i );
      const ValueInfo & vi = this->ro( i );
      double val = vi.oldValue;
      if ( myIsCurveOpen && ( i == ( size() - 1 ) ) )
	{ // free extremity to the front/right.
	  const ValueInfo & viprev = this->ro( iprev );
	  double valp = viprev.oldValue; 
	  grad[ i ] = 2.0 * AngleComputer::deviation( val, valp ) / viprev.distToNext;
	}
      else if ( myIsCurveOpen && ( i ==  0 ) )
	{ // free extremity to the back/left.
	  const ValueInfo & vinext = this->ro( inext );
	  double valn = vinext.oldValue;
	  grad[ i ] = -2.0 * AngleComputer::deviation( valn, val ) / vi.distToNext;
	}
      else
	{ // standard case.
	  const ValueInfo & viprev = this->ro( iprev );
	  const ValueInfo & vinext = this->ro( inext );
	  double valp = viprev.oldValue; 
	  double valn = vinext.oldValue;
	  grad[ i ] = 2.0*( AngleComputer::deviation( val, valp ) / viprev.distToNext 
			    - AngleComputer::deviation( valn, val ) / vi.distToNext ) ;
	}
    }
  return grad;
}

  
double 
DGtal::AngleLinearMinimizer::optimize()
{
  return optimize( 0, 0 );
}


double 
DGtal::AngleLinearMinimizer::optimize( unsigned int i1, unsigned int i2 )
{
  ASSERT( size() > 2 );
  ModuloComputer<unsigned int> mc( size() );

  unsigned int i = i1;
  // (I) first pass to work on old values.
  do 
    {
      ValueInfo & vi = this->rw( i );
      vi.oldValue = vi.value;
      // go to next.
      i = mc.next( i );
    }
  while ( i != i2 );
  this->oneStep( i1, i2 );
  
  mySum = 0.0;
  myMax = 0.0;
  i = i1;
  do 
    {
      const ValueInfo & vi = this->ro( i );
      double diff = fabs( AngleComputer::deviation( vi.value, vi.oldValue ) );
      if ( diff > myMax )
	myMax = diff;
      mySum += diff;
      i = mc.next( i );
    }
  while ( i != i2 );

  return mySum;
}


double
DGtal::AngleLinearMinimizer::lastDelta() const
{
  return max();
}


void
DGtal::AngleLinearMinimizer::oneStep( unsigned int i1, unsigned int i2 )
{
  ModuloComputer<unsigned int> mc( size() );

  double mid;
  unsigned int i = i1; 
  unsigned int iprev = mc.previous( i );
  do 
    {
      unsigned int inext = mc.next( i );
      ValueInfo & vi = this->rw( i );
      if ( myIsCurveOpen && ( i == ( size() - 1 ) ) )
	{ // free extremity to the front/right.
	  const ValueInfo & viprev = this->ro( iprev );
	  mid = viprev.oldValue; // - viprev.delta;
	}
      else if ( myIsCurveOpen && ( i ==  0 ) )
	{ // free extremity to the back/left.
	  const ValueInfo & vinext = this->ro( inext );
	  mid = vinext.oldValue; // + vi.delta;
	}
      else
	{ // standard case.
	  const ValueInfo & viprev = this->ro( iprev );
	  const ValueInfo & vinext = this->ro( inext );
	  double valp = viprev.oldValue; // - viprev.delta;
	  double valn = vinext.oldValue; // + vi.delta;

	  // old
	  double y = AngleComputer::deviation( valn, valp );
	  mid = ( viprev.distToNext * y )
	    / ( vi.distToNext + viprev.distToNext );
	  mid = AngleComputer::cast( mid + valp );

	}
      if ( AngleComputer::less( mid, vi.min ) ) mid = vi.min;
      if ( AngleComputer::less( vi.max, mid ) ) mid = vi.max;
      double mvt = AngleComputer::deviation( mid, vi.oldValue );
      vi.value = AngleComputer::cast( vi.oldValue + 0.5 * mvt );      
      // go to next.
      iprev = i;
      i = inext;
    }
  while ( i != i2 );
  

}



void
DGtal::AngleLinearMinimizerByRelaxation::oneStep( unsigned int i1, unsigned int i2 )
{
  ModuloComputer<unsigned int> mc( size() );

  double mid;
  unsigned int i = i1; 
  unsigned int iprev = mc.previous( i );
  do 
    {
      unsigned int inext = mc.next( i );
      ValueInfo & vi = this->rw( i );
      //      vi.oldValue = vi.value;
      if ( myIsCurveOpen && ( i == ( size() - 1 ) ) )
	{ // free extremity to the front/right.
	  const ValueInfo & viprev = this->ro( iprev );
	  mid = viprev.value; // - viprev.delta;
	}
      else if ( myIsCurveOpen && ( i ==  0 ) )
	{ // free extremity to the back/left.
	  const ValueInfo & vinext = this->ro( inext );
	  mid = vinext.oldValue; // + vi.delta;
	}
      else
	{ // standard case.
	  const ValueInfo & viprev = this->ro( iprev );
	  const ValueInfo & vinext = this->ro( inext );
	  double valp = viprev.value; // - viprev.delta;
	  double valn = vinext.value;
	  
	  // old
	  double y = AngleComputer::deviation( valn, valp );
	  mid = ( viprev.distToNext * y )
	    / ( vi.distToNext + viprev.distToNext );
	  mid = AngleComputer::cast( mid + valp );
	}
      if ( AngleComputer::less( mid, vi.min ) ) mid = vi.min;
      if ( AngleComputer::less( vi.max, mid ) ) mid = vi.max;
      vi.value = mid;
      iprev = i;
      i = inext;
    }
  while ( i != i2 );
}




double
DGtal::AngleLinearMinimizerByRelaxation::lastDelta() const
{
  return max();
}



void
DGtal::AngleLinearMinimizerByGradientDescent::oneStep( unsigned int i1, unsigned int i2 )
{
  ModuloComputer<unsigned int> mc( size() );

  vector<double> grad ( getFormerGradient() );
  double mid;
  unsigned int i = i1; 
  do 
    {
      unsigned int inext = mc.next( i );
      ValueInfo & vi = this->rw( i );
      double val = vi.oldValue;
      mid = AngleComputer::cast( val - myStep*grad[ i ] );
      if ( AngleComputer::less( mid, vi.min ) ) mid = vi.min;
      if ( AngleComputer::less( vi.max, mid ) ) mid = vi.max;
      vi.value = mid;
      // go to next.
      i = inext;
    }
  while ( i != i2 );
  double E1 = getFormerEnergy( i1, i2 );
  double E2 = getEnergy( i1, i2 );  
  cerr << "E1=" << E1 << " E2=" << E2 << " s=" << myStep << endl;
}



double
DGtal::AngleLinearMinimizerByGradientDescent::lastDelta() const
{
  vector<double> grad ( getFormerGradient() );
  double ninf = 0.0;
  for ( unsigned int i = 0; i < size(); i++ ) 
    {
      const ValueInfo & vi = this->ro( i );
      if ( vi.value != vi.oldValue )
	{
	  double n = fabs( grad[ i ] );
	  if ( n > ninf ) ninf = n;
	}
    }
  return ninf;
}




void
DGtal::AngleLinearMinimizerByAdaptiveStepGradientDescent::oneStep( unsigned int i1, unsigned int i2 )
{
  ModuloComputer<unsigned int> mc( size() );
  vector<double> grad ( getFormerGradient() );
  
  double mid;
  unsigned int i = i1; 
  do 
    {
      unsigned int inext = mc.next( i );
      ValueInfo & vi = this->rw( i );
      double val = vi.oldValue;
      mid = AngleComputer::cast( val - myStep*grad[ i ] );
      if ( AngleComputer::less( mid, vi.min ) ) mid = vi.min;
      if ( AngleComputer::less( vi.max, mid ) ) mid = vi.max;
      vi.value = mid;
      // go to next.
      i = inext;
    }
  while ( i != i2 );

  double E1 = getFormerEnergy( i1, i2 );
  double E2 = getEnergy( i1, i2 );
  cerr << "E1=" << E1 << " E2=" << E2 << " s=" << myStep << endl;
  if ( E1 <= E2 )
    {
      myStep /= 4.0;
    }
  else
    {
      /* doubling step. */
      myStep *= 2.0;
    }
}



double
DGtal::AngleLinearMinimizerByAdaptiveStepGradientDescent::lastDelta() const
{
  vector<double> grad ( getFormerGradient() );
  double ninf = 0.0;
  for ( unsigned int i = 0; i < size(); i++ ) 
    {
      const ValueInfo & vi = this->ro( i );
      if ( vi.value != vi.oldValue )
	{
	  double n = fabs( grad[ i ] );
	  if ( n > ninf ) ninf = n;
	}
    }
  return ninf;
}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param out the output stream where the object is written.
 */
void
DGtal::AngleLinearMinimizer::selfDisplay ( std::ostream & aStream ) const
{
  aStream << "[AngleLinearMinimizer::standard 0.5]";
}


/**
 * Writes/Displays the object on an output stream.
 * @param aStream the output stream where the object is written.
 */
void 
DGtal::AngleLinearMinimizerByRelaxation::selfDisplay( std::ostream & aStream ) const
{
  aStream << "[LinearMinimizer::relaxation]";
}

/**
 * Writes/Displays the object on an output stream.
 * @param aStream the output stream where the object is written.
 */
void 
DGtal::AngleLinearMinimizerByGradientDescent::selfDisplay( std::ostream& aStream ) const
{
  aStream << "[LinearMinimizer::gradient descent " << myStep << "]";
}

/**
 * Writes/Displays the object on an output stream.
 * @param aStream the output stream where the object is written.
 */
void 
DGtal::AngleLinearMinimizerByAdaptiveStepGradientDescent::selfDisplay( std::ostream& aStream ) const
{
  aStream << "[LinearMinimizer::gradient descent with adaptive step " << myStep << "]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool
DGtal::AngleLinearMinimizer::isValid() const
{
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
