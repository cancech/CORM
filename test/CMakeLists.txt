cmake_minimum_required(VERSION 3.10)
project(libcorm-test)

find_package (Boost 1.65 REQUIRED COMPONENTS unit_test_framework)
list(APPEND INCLUDE_DIRS ${BOOST_INCLUDE_DIRS})

file(GLOB_RECURSE sources_test src/*.cpp)

add_executable(CORM_Test ${sources_test})
target_include_directories(CORM_Test PUBLIC include)
target_link_libraries(CORM_Test corm)
target_link_libraries(CORM_Test
  ${Boost_LIBRARIES}
  boost_unit_test_framework
)

option(ENABLE_AUTOREGISTRATION "Enable the Bean Autoregistration flag for the tests" OFF)
if(ENABLE_AUTOREGISTRATION)
	target_compile_definitions(CORM_Test PUBLIC ENABLE_BEAN_AUTOREGISTRATION)
ENDIF(ENABLE_AUTOREGISTRATION)
