ADD_EXECUTABLE (judy_test main.cpp)
TARGET_LINK_LIBRARIES (judy_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
ADD_TEST (NAME judy_test COMMAND judy_test)
