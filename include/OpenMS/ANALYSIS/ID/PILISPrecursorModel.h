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


#ifndef OPENMS_ANALYSIS_ID_PILISPRECURSORMODEL_H
#define OPENMS_ANALYSIS_ID_PILISPRECURSORMODEL_H

#include <vector>
#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/DATASTRUCTURES/Map.h>
#include <OpenMS/DATASTRUCTURES/String.h>
#include <OpenMS/CONCEPT/Types.h>
#include <OpenMS/ANALYSIS/ID/HiddenMarkovModel.h>
#include <OpenMS/DATASTRUCTURES/DefaultParamHandler.h>

namespace OpenMS 
{
	class AASequence;
	
	/** 
	  @brief This class implements the simulation of the spectra from PILIS

		PILIS uses a HMM based structure to model the population of fragment ions
		from a peptide. The spectrum generator can be accessed via the getSpectrum
		method.
		 
		@ref PILISPrecursorModel_Parameters are explained on a separate page.

		@ingroup Analysis_ID
	*/	
	class PILISPrecursorModel : public DefaultParamHandler
	{
		friend class PILISPrecursorModelGenerator;

		public:
			
			/** @name Constructors and destructors
			*/
			//@{
			/// default constructor
			PILISPrecursorModel();

			/// copy constructor
			PILISPrecursorModel(const PILISPrecursorModel& model);
			
			/// destructor
			virtual ~PILISPrecursorModel();
			//@}

			/// assignment operator
			PILISPrecursorModel& operator = (const PILISPrecursorModel& mode);
			
			/** @name Accessors
			*/
			//@{
			/// performs a training step; needs as parameters a spectrum with annotated sequence and charge
			void train(const RichPeakSpectrum&, const AASequence& peptide, UInt charge);

			/** reads the model parameters from the given files
			    @param filename filename of the model
			*/ 
			//void readFromFile(const String& filename);

			/// 
			void getPrecursorIons(std::vector<RichPeak1D>& peaks, const AASequence& peptide, UInt charge);

			/// writes the HMM to the given file in the GraphML format. A detailed description of the GraphML format can be found under http://graphml.graphdrawing.org/
			//void writeGraphMLFile(const String& filename);

			/** writes the model parameters into the given files
			    @param filename filename of the base model
			*/			 
			//void writeToFile(const String& filename);

			/// this method evaluates the model after training; it should be called after all training steps with train
			void evaluate();
			//@}

		protected:

			/// extracts the precursor and related intensities of a training spectrum
			void getPrecursorIntensitiesFromSpectrum_(const RichPeakSpectrum& train_spec, Map<String, double>& pre_ints, double peptide_weight, UInt charge);

			/// trains precursor and related peaks
			void trainPrecursorIons_(double initial_probability, const Map<String, double>& intensities, const AASequence& peptide);
			
			/// estimates the precursor intensities 
			void getPrecursorIons_(Map<String, double>& intensities, double initial_probability, const AASequence& precursor);

			/// enables the states needed for precursor training/simulation
			void enablePrecursorIonStates_(const AASequence& peptide);

			/// precursor model used
			HiddenMarkovModel hmm_precursor_;

			void updateMembers_();
	};
}
#endif

