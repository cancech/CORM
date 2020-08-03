# CORM
C++ Object Repository Manager

## Simple CORM

The simplest form of CORM which can be provides is by means of global functions. A configuration file has a function which provides a bean, and someone else (whether this be at the global level, or within a class) call the providing function to retrieve the bean. There are several advantages this this approach:

* Simplicity (the #defines provides are merely for convenience), and the low overheard required.
* Compile time verification to ensure that all required beans are available.
* Classes can retrieve the necessary beans themselves at instantiation time (no need to pass the beans into the class via a constructor).
* Extremely fast, no overhead relating to bean management.

There are however plenty of drawbacks to this approach:

* Instance creation and all memory management is purely within the client code. The providing function must itself track instantiations and object lifecycles for the purpose of freeing memory, and ensuring that only the appropriate instances are created (for example if a given object should be a singleton).
* If the function providing the bean is not present in the existing include heirarchy at compilation time, the function signature must be made available (as seen in SimpleCormAdder.h). While not a big problem, it does force additional maintenance work onto the developer, with the only solution being a custom code generator (which would introduce additional complexity).
* The absolute simplicity cannot provide any form of error checking outside of compilation. This means that dependency cycles can go unnoticed and introduce unexpected bugs and/or odd behaviour.

The supporting Macros are all contained within simple/SimpleCorm.h, with (simple/)SimpleCorm* within the test_source directory provided to test the functionality and show examples of usage.

## Runtime CORM

In order to address the limitations of the Simple CORM approach, a runtime based approach is required. This approach is loosly modelled on the approach taken by Spring for Java, with a breakdown as follows:

* Beans are tracked within a BeanManager. This associates an instance or a creator (more or this later) to a bean name, and the bean can then be extracted viait name. In this respect the BeanManager is at the core of the CORM.
* Configuration classes are to be used to facilitate access to the BeanManager. Each configuration class can pull beans from the BeanManager (resources) and provides beans back into it. Each individual configuration can only be created when all if its required resources are available within the BeanManager, as the resources are treated as private members within the Configuration class. After instantiation the Configuration must be initialized, during which time any/all beans it is to provide are created as passed into the BeanManager.
* A single BeanManager and any number of Configuration classes are then contained within a single Context. The Context is responsible for assembling the Configurations and ulitmately responsible for the lifecycle of the Configurations and beans within.

### Managing a Context

Creating a new Context is as simple as creating an instance of the Context class. The Context will create the BeanManager instance for itself, and allow the registration of Configuration classes. Once all of the desired Configurations are registered, the Context must be assembled at which time it will attempt to create and initialize all registered configurations. If there is an issue encountered during the assembly (primarily missing resource, type mismatch, or circular dependency between Configurations) an exception will be thrown indicating where and why assembling the Context failed.

```c++
corm::Context sampleContext;
sampleContext.registerConfiguration<Config1, Config2, Config3>();
sampleContext.registerConfiguration<Config4>();
sampleContext.assemble();
```
Note that the Configuration class instances are not passed into the Context (their creation would most likely fail due to missing resources at this stage), but rather their type is passed through the template. Any number of Configuration class types can be passed in to a single call to registerConfiguration (minimum one).

The lifecycle of the context is tied to the lifecycle of the Context class. Once that instance is destroyed, all of the registered Configurations will be destroyed, and any beans which are managed by the BeanManager will be destroyed as well.

### Creating a Configuration

In order for a Configuration to be usable within a Context, it must:

* Extend the corm::Configuration class.
* Provide a contructor that matches the exact signature of the base corm::Configuration contructor.
* Override the getName() static member function to return a plain text name of the Configuration class. (1)
* Override the getResourceNamess() static member function to return a complete list of names of all resources the Configuration requires. (1)
* Override the getBeanNames() static member function to return a complete list of all beans the Configration will provide. (1)

(1) Strickly speaking these are not required to compile or for core functionality, however they are required for error checking and debug.

A series of Macros are available to facility the definition and creation of Configuration classes. It is highly recommended to make use of these to simplify the process of creating new Configurations. An example of such a class is as follows.

```c++
/*
 * Create a new configuration via the CONFIGURATION macro. The parameter passed to it (ExampleConfiguration in this case)
 * is the name of this Configuration class, and what will later be registered witin a Context.
 */
CONFIGURATION(ExampleConfiguration)

  // Include dependent Configurations of this Configuration (more details below)
  DEPENDENCIES(SubConfiguration1, SubConfiguration2, SubConfiguration3)

public:
  ~ExampleConfiguration() {
    // The Configuration is responsible for managing any memory it allocates
    delete(instance);
  }

protected:

  /*
   * postInit is called during the initialization process of the Configuration. It is here that any processing
   * of resources should take place. The processing need not result in any beans, but this is where any bean
   * instances which are to be then shared are to be initialized.
   */
  void postInit() {
    instance = new CustomClass(intBean, stringBean, sampleClassInstance);  
  }
  
  // Through the BEANS Macro indicate what beans are to be provided (more details below)
  BEANS(
    (BEAN_INSTANCE, CustomClass*, "customClassInstance", instance)
  )

  // Through the RESOURCES Macro indicate what resources the configuration requires (more details below)
  RESOURCES(
    (int, intbean),
    (std::string&, stringBean),
    (SampleClass*, sampleClassInstance)
  )
  
private:
  CustomClass* instance;

END_CONFIGURATION
```

#### CONFIGURATION Macro

```C++
CONFIGURATION(ExampleConfiguration)
```
This Macro marks the start of a Configuration class. It will provide the start of the class definition, and provide an implementation of the static getName() member function. The example above will generate the following:

```C++
class ExampleConfiguration: public corm::BaseConfiguration { \
	public: \
		ConsumerManagerTestConfig(corm::BeanManager* manager): BaseConfiguration(manager) {} \
		static std::string getName() { return "ExampleConfiguration"; }
```

#### DEPENDENCIES Macro

```c++
DEPENDENCIES(SubConfiguration1, SubConfiguration2, SubConfiguration3)
```

A Configuration can have dependent Configurations, which are Configurations will be registered alongside this Configuration when it is registered in a Context. This allows for easy module management, where a root Configuration for a module can be defined and this will then list dependencies of any/all Configurations required for the module to function. In this manner the whole module can be registered with a Context by simply registering the core Configuration. For example, in the ExampleConfiguration above, when it is registered to a Context the Context will ultimately register ExampleConfiguration, as well as SubConfiguration1, SubConfiguration2, SubConfiguration3, and any dependent Configurations these configurations may have.

Note that when using this Macro at least one dependent Configuration must be present, and it will support up to 256 dependent Configurations. If more than 256 dependent Configurations are required for a single Configuration, it must be broken into nested layers of dependencies (top most Configuration can have up to 256 dependencies, each of which can have up to 256 dependencies, each of those can have an additional 356 dependencies,... ad nausea).

This Macro will ultimately provide an implementation of the static getDependentConfigurations() member function, and generate code which looks like the following:

```C++
static std::vector<corm::ConfigurationWrapperInterface*> getDependentConfigurations(corm::BeanManager* manager) { \
		std::vector<corm::ConfigurationWrapperInterface*> deps; \
		deps.push_back(new corm::ConfigurationWrapper<SubConfiguration1>(manager)); \
		deps.push_back(new corm::ConfigurationWrapper<SubConfiguration2>(manager)); \
		deps.push_back(new corm::ConfigurationWrapper<SubConfiguration3>(manager)); \
		return deps; \
}
```

#### BEANS Macro

```C++
  BEANS(
    (BEAN, SomeClass&, "someClassBean"),
    (BEAN_INSTANCE, CustomClass*, "customClassInstance", instance)
  )
 ```
 This Macro is available to shorthand the process of registering beans with the BeanManager, as well as providing an implementation for the static getBeanNames() member function. Each bean to be registered must be contained within a set of brackets, however there is some flexibility in terms of what and how the beans are registered. The first parameter for each bean is the Bean Type:
 
 * BEAN indicates a bean which is to be managed via a Bean Creator (see under BeanManager for more details)
 * BEAN_INSTANCE indicates a registering a member within the Configation to the BeanManager directly
 
 For BEAN the remaining parameters are as follows:
 
 * Type - indicating the type which is to be registered
 * Creator - Optionally indicating the type of Creator to use for the bean. If not present it will default to corm::SingletonBeanCreator.
 * Bean Name - under what name the bean should be registered
 
 For BEAN_INSTANCE the remaining parameters are as follows:
 
 * Type - indicating the type which is to be registered
 * Bean Name - Optionally indicating what name to register the instance under. If not present it will default to the member variable name
 * Instance - the specific instance which is to be registered as a bean
 
 Note that for BEAN_INSTANCE the Instance can be any instance of the desired type. This could be a reference to a member variable (as per the example), or creating the instance when registering. For example

```C++
  BEANS(
    (BEAN_INSTANCE, int, "someint", 123),
    (BEAN_INSTANCE, int, "somestring", "This is a String"),
    (BEAN_INSTANCE, SomeClass, "someclass", new SomeClass(1, 2, 3))
  )
```
are all valid, though note the possible memory leak on the someclass bean instance.
