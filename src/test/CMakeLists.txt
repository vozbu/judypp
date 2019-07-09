ADD_EXECUTABLE (judy_test main.cpp map.cpp set.cpp)
TARGET_LINK_LIBRARIES (judy_test judypp Boost::unit_test_framework)
ADD_TEST (NAME judy_test COMMAND judy_test)

ADD_EXECUTABLE (perf_test perf_test.cpp)
TARGET_LINK_LIBRARIES (perf_test judypp)
IF (TARGET GoogleSparseHash::GoogleSparseHash)
    TARGET_LINK_LIBRARIES (perf_test GoogleSparseHash::GoogleSparseHash)
ENDIF()
