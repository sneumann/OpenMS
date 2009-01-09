// -*- mode: C++; tab-width: 2; -*-
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
// $Maintainer: Alexandra Scherbart $
// --------------------------------------------------------------------------
//

#ifndef OPENMS_ANALYSIS_PIP_PEAKINTENSITYPREDICTOR_H
#define OPENMS_ANALYSIS_PIP_PEAKINTENSITYPREDICTOR_H

#include <OpenMS/CHEMISTRY/AASequence.h>
#include <OpenMS/ANALYSIS/PIP/LocalLinearMap.h>
	
namespace OpenMS
{
	
	/**
		@brief Predict peak heights of peptides based on Local Linear %Map model
		
		This class can be used for predictions of peptide peak heights 
		(referred to as intensities) from a peptide sequence 
		by a Local Linear %Map (LLM) model. 
		A general introduction to the Peak Intensity Predictor (PIP)
		can be found in the <A HREF="tutorial_pip.html">PIP Tutorial</A>.
		
		The predictor performs only on the peptides sequences as an AASequence representation. Every sequence is 
		transformed to an 18 dimensional data vector representing certain 
		chemical characteristics and is loaded into the trained LocalLinearMap model to
		find the predicted peptides peak intensity.
		
		Every predictor object calls the appropriate %LocalLinearMap model, transforms
		the given sequences and creates a vector space in which the %LocalLinearMap
		performs. 
		
		@ingroup Analysis 
  */
	class PeakIntensityPredictor
	{
		
		public:

			///Constructors and Destructors
			//@{
			/// default constructor
			PeakIntensityPredictor();
			/// destructor
			virtual ~PeakIntensityPredictor();
			//@}
		
			///Returns predicted peak heights (intensites) of a single peptide
			DoubleReal predict(const AASequence& sequence);

			/**
	      @brief Returns predicted peak heights (intensites) of a single peptide
	
	      Some additional information is returned in @p add_info :
	      - 0: x coordinates of associated cluster (first column)
	      - 1: y coordinates of associated cluster (2nd column)
	      - 2: error (RMSE) of the peptide to the associated next prototype (cluster center)
    	*/
			DoubleReal predict(const AASequence& sequence, std::vector<DoubleReal>& add_info);

			///Returns predicted peak heights (intensites) of several peptides
			std::vector<DoubleReal> predict(const std::vector<AASequence>& sequences);

			/**
	      @brief Returns predicted peak heights (intensites) of several peptides
	
	      Some additional information foreach peptide is returned in @p add_info .
	      For each peptide a row with the following components is returned:
	      - 0: x coordinates of associated cluster (first column)
	      - 1: y coordinates of associated cluster (2nd column)
	      - 2: error (RMSE) of the peptide to the associated next prototype (cluster center)
    	*/
			std::vector<DoubleReal> predict(const std::vector<AASequence>& sequences, std::vector<std::vector<DoubleReal> >& add_info);

		private:
			
			/// calculate and return predicted value based on given LocalLinearMap model for corresponding aaindex variables
			DoubleReal map_(const std::vector<DoubleReal>& data);
			/// find winning prototype
			UInt findWinner_(const std::vector<DoubleReal>& data);
			/// calculate assignments of peptides to cluster and the corresponding error
			std::vector<DoubleReal> calculateAddInfo_(const std::vector<DoubleReal>& data);

			/// Local Linear %Map model
			LocalLinearMap llm_;
			
			/// copy constructor not impemented => private
			PeakIntensityPredictor(const PeakIntensityPredictor& llmModel);
			/// assignment operator not impemented => private
			PeakIntensityPredictor& operator = (const PeakIntensityPredictor& peakIntensityPredictor);
			/// equality operator not impemented => private
			bool operator == (const PeakIntensityPredictor& peakIntensityPredictor) const;
			
		};

} // namespace OpenMS

#endif // OPENMS_ANALYSIS_PIP_PEAKINTENSITYPREDICTOR_H





