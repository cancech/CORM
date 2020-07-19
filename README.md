# CORM
C++ Object Repository Manager

## Simple CORM

The simplest form of CORM which can be provides is by means of global functions. A configuration file has a function which provides a bean, and someone else (whether this be at the global level, or within a class) call the providing function to retrieve the bean. There are several advantages this this approach:

* Simplicity (the #defines provides are merely for convenience), and the low overheard required.
* Compile time verification to ensure that all required beans are available.
* Classes can retrieve the necessary beans themselves at instantiation time (no need to pass the beans into the class via a constructor).

There are however plenty of drawbacks to this approach:

* Instance creation and all memory management is purely within the client code. The providing function must itself track instantiations and object lifecycles for the purpose of freeing memory, and ensuring that only the appropriate instances are created (for example if a given object should be a singleton).
* If the function providing the bean is not present in the existing include heirarchy at compilation time, the function signature must be made available (as seen in SimpleCormAdder.h). While not a big problem, it does force additional maintenance work onto the developer, with the only solution being a custom code generator (which would introduce additional complexity).
* The absolute simplicity cannot provide any form of error checking outside of compilation. This means that dependency cycles can go unnoticed and introduce unexpected bugs and/or odd behaviour.

The supporting Macros are all contained within simple/SimpleCorm.h, with (simple/)SimpleCorm* within the test_source directory provided to test the functionality and show examples of usage.
