### the directory name
set(directory source/APPLICATIONS/TOPP)

### list all filenames of the directory here
set(executables_list
AdditiveSeries
BaselineFilter
ConsensusID
DBExporter
DBImporter
DTAExtractor
Decharger
FalseDiscoveryRate
FeatureFinder
FeatureLinker
FileConverter
FileFilter
FileInfo
FileMerger
IDDecoyProbability
IDFilter
IDMerger
INIFileEditor
ITRAQAnalyzer
InspectAdapter
InternalCalibration
MapAligner
MapNormalizer
MascotAdapter
MascotAdapterOnline
NoiseFilter
OMSSAAdapter
PTModel
PTPredict
PeakPicker
PepNovoAdapter
RTModel
RTPredict
Resampler
#SILACAnalyzer
# PILISModel
# PILISIdentification
SequestAdapter
SpectraFilter
TOFCalibration
TOPPView
TOPPAS
TextExporter
XTandemAdapter
IDMapper
IDRTCalibration
SILACAnalyzer
PrecursorIonSelector
)

### pass source file list to the upper instance
set(TOPP_executables ${TOPP_executables} ${executables_list})

