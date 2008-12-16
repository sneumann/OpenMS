### the directory name
set(directory include/OpenMS/ANALYSIS/ID)

### list all header files of the directory here
set(sources_list_h
ConsensusID.h
FalseDiscoveryRate.h
HiddenMarkovModel.h
IDConsensusFeatureMapper.h
IDDecoyProbability.h
IDFeatureMapper.h
IDSpectrumMapper.h
PILISIdentification.h
PILISModel.h
PILISModelGenerator.h
PILISNeutralLossModel.h
PILISScoring.h
ProtonDistributionModel.h
PrecursorIonSelectionPreprocessing.h
PrecursorIonSelection.h
)

### add path to the filenames
set(sources_h)
foreach(i ${sources_list_h})
	list(APPEND sources_h ${directory}/${i})
endforeach(i)

### source group definition
source_group("Header Files\\OpenMS\\ANALYSIS\\ID" FILES ${sources_h})

set(OpenMS_sources_h ${OpenMS_sources_h} ${sources_h})

