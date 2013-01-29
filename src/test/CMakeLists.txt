ADD_EXECUTABLE (judy_test main.cpp map.cpp set.cpp)
TARGET_LINK_LIBRARIES (judy_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${LJUDY})
ADD_TEST (NAME judy_test COMMAND judy_test)

CONFIGURE_FILE (config.h.in ${CMAKE_BINARY_DIR}/src/test/config.h)
INCLUDE_DIRECTORIES (${CMAKE_BINARY_DIR}/src/test)
ADD_EXECUTABLE (perf_test perf_test.cpp)
TARGET_LINK_LIBRARIES (perf_test ${LJUDY} rt)
