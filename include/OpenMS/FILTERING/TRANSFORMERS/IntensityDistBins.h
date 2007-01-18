// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2007 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Andreas Bertsch $
// --------------------------------------------------------------------------
//
#ifndef OPENMS_FILTERING_TRANSFORMERS_INTENSITYDISTBINS_H
#define OPENMS_FILTERING_TRANSFORMERS_INTENSITYDISTBINS_H

#include <OpenMS/FILTERING/TRANSFORMERS/FilterFunctor.h>

#include <vector>

namespace OpenMS
{
  /**
	  @brief IntensityDistBins divides the intensity range into <i>bins</i> regions and counts the peaks that fall into each bin
	  
	  @param bins number of regions

		@ingroup SpectraFilters
  */
	class ClusterSpectrum;
	
  class IntensityDistBins : public FilterFunctor
  {
  public:

		// @name Constructors and Destructors
		// @{
    /// default constructor
    IntensityDistBins();

    /// copy constructor
    IntensityDistBins(const IntensityDistBins& source);

		// destructor
		virtual ~IntensityDistBins();
		// @}

		// @name Operators
		// @{
    /// assignment operator
    IntensityDistBins& operator = (const IntensityDistBins& source);
		// @}

		// @name Accessors
		// @{
		///
    static FilterFunctor* create() { return new IntensityDistBins(); }

		///
    std::vector<double> operator () (const ClusterSpectrum& spec);

		///
		static const String getName()
		{
			return "IntensityDistBins";
		}
		// @}
		
  };
}
#endif // OPENMS_FILTERING_TRANSFORMERS_INTENSITYDISTBINS_H
