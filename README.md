# CORM
C++ Object Repository Manager

A comprehensive library to facilitate the sharing of object instances (beans) across an application in an efficient and straight forward manner. All efforts have been made to minimize the amount of client code (repetitive, boiler plate, or otherwise) required in order to allow bean sharing to take place. The library provides two forms of bean sharing:

* Simple Compile Time - a series of Macros to facilitate uniform function naming (minimally supported)
* Comprehensive Runtime - full fledged support for bean sharing and management

Both can be used, however the Runtime CORM is the core feature. To learn more about CORM, and how to use it, refer to the github Wiki.

## Dependencies

The entire library is written using standard C++11, with boost::unit_test employed for the unit tests. Boost is not required to compile and run the core library (contained within the source directory), it is only required to compile and run the included unit tests (contained within the test_source directory).

## Compiling

The source is provided as a Eclipse CDT project, with four different Build Configurations provided:

* Release - no debug symbols in the compiled library
* Debug - debug symbols are included in the compiled library
* Test - build the library with the included unit tests (generates an application that can be executed to run all of the unit tests)
* Test_Autoregistration - build the library with the included unit test (as per Test), with the ENABLE_BEAN_AUTOREGISTRATION flag enabled (allow testing the auto-registration capabilities)

To allow compiling outside of Eclipse a cmake configuration is also provided. To compile using cmake the following must be performed (note that steps assume a Unix based environment.

```
cd <git_dir>
mkdir build
cd build
cmake ../
make
```

This will compile the library, as well as the included unit tests. The following flags can be used with cmake to customize the build:

* _-DCMAKE_BUILD_TYPE=[Release/Debug]_ - make the build using release or debug options
* _-DENABLE_AUTOREGISTRATION=ON_ - compile the unit tests with the Bean Autoregistration capabilities enabled
