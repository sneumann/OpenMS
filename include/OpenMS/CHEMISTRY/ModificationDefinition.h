// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
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

#ifndef OPENMS_CHEMISTRY_RESIDUEMODIFICATIONDEFINITION_H
#define OPENMS_CHEMISTRY_RESIDUEMODIFICATIONDEFINITION_H

#include <OpenMS/CONCEPT/Types.h>
#include <OpenMS/DATASTRUCTURES/String.h>
#include <OpenMS/CHEMISTRY/IsotopeDistribution.h>

namespace OpenMS
{
	/** @ingroup Chemistry
	
			@brief Representation of an element
	*/
	class ModificationDefinition
	{
		public:

			/** @name Constructor and Destructors
			*/
			//@{
			/// default constructor
			ModificationDefinition();

			/// copy constructor
			ModificationDefinition(const ModificationDefinition& element);

			/// detailed constructor
			ModificationDefinition(const String& name,
							const String& symbol,
							UInt atomic_number,
							DoubleReal average_weight,
							DoubleReal mono_weight,
							const IsotopeDistribution& isotopes);

			/// destructor
			virtual ~ModificationDefinition();
			//@}

			/** @name Accessors
			*/
			//@{
			/// sets unique atomic number
			void setAtomicNumber(UInt atomic_number);

			/// returns the unique atomic number
			UInt getAtomicNumber() const;
			
			/// sets the average weight of the element
			void setAverageWeight(DoubleReal weight);

			/// returns the average weight of the element
			DoubleReal getAverageWeight() const;

			/// sets the mono isotopic weight of the element
			void setMonoWeight(DoubleReal weight);

			/// returns the mono isotopic weight of the element
			DoubleReal getMonoWeight() const;

			/// sets the isotope distribution of the element
			void setIsotopeDistribution(const IsotopeDistribution& isotopes);

			/// returns the isotope distribution of the element
			const IsotopeDistribution& getIsotopeDistribution() const;

			/// set the name of the element
			void setName(const String& name);

			/// returns the name of the element
			const String& getName() const;

			/// sets symbol of the element
			void setSymbol(const String& symbol);

			/// returns symbol of the element
			const String& getSymbol() const;
			//@}

			/** @name Assignment
			*/
			//@{
			/// assignment operator
			ModificationDefinition& operator = (const ModificationDefinition& element);
			//@}

			/** @name Predicates
			*/
			//@{
			/// equality operator
			bool operator == (const ModificationDefinition& element) const;

			/// inequality operator
			bool operator != (const ModificationDefinition& element) const;
			//@}

			/// writes the element to an output stream
			friend std::ostream& operator << (std::ostream& os, const ModificationDefinition& element);

		protected:

			/// name of the element
			String name_;

			/// symbol of the element
			String symbol_;

			/// atomic number of the element
			UInt atomic_number_;

			/// average weight over all isotopes
			DoubleReal average_weight_;
	
			/// mono isotopic weight of the most frequent isotope
			DoubleReal mono_weight_;

			/// distribution of the isotopes
			IsotopeDistribution isotopes_;
	};

	std::ostream& operator << (std::ostream&, const ModificationDefinition&);

} // namespace OpenMS

#endif

