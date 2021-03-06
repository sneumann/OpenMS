from Peak1D cimport *
from libcpp.vector cimport vector as libcpp_vector

# this class has addons, see the ./addons folder

cdef extern from "<OpenMS/FORMAT/PeakTypeEstimator.h>" namespace "OpenMS":

    cdef cppclass PeakTypeEstimator:
        PeakTypeEstimator()

        # wrpped in ../addons/PeakTypeEstimator.pyx:
        int estimateType(libcpp_vector[Peak1D].iterator,
                         libcpp_vector[Peak1D].iterator) # wrap-ignore
