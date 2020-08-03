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
    (BEAN, SomeClass&, "someClassBean"),
    (BEAN_INSTANCE, CustomClass*, instance)
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
    (BEAN_INSTANCE, CustomClass*, instance)
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
 * Bean Name - Optionally indicating what name to register the instance under. If not present it will default to the instance text as the name (in the above example the bean name will be "instance")
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

The original example of

```C++
  BEANS(
    (BEAN, SomeClass&, "someClassBean"),
    (BEAN_INSTANCE, CustomClass*, instance)
  )
```
will generate code which looks as follows

```C++
protected: \
	void provideBeans() { \
		m_beanManager->registerBean<SomeClass&>("someClassBean"); \
	m_beanManager->registerBeanInstance<CustomClass*>("instance", instance); \
	} \
public: \
	static const std::vector<std::string>& getBeanNames() { \
		static std::vector<std::string> beanNames { \
			"someClassBean", \
			"instance", \
		}; \
		return beanNames; \
	}
```

Note that up to 256 beans can be registered in a single Configuration through the means indicated here. 

#### RESOURCES Macro

```C++
  RESOURCES(
    (int, intbean),
    (std::string&, "beanName", stringBean),
    (SampleClass*, sampleClassInstance)
  )
```
This Macro will shorthand the process of registering resource dependencies, as well as the static getResourceNames() member function. Much like the BEANS Marco each required resource must be contained within a set of brackets, and the values provided for each resource are as follows:

* Type - indicating what type of resource is expected to be retrieved from the BeanManager
* Bean Name - Optionally indicating the name of the bean to retrieve. If not present the bean name is derived from the variable name.
* Variable Name - name of the variable where the bean is to be stored

This will create all of the specified resources as members within the Configuration and load each of them from the BeanManager. The example will generate the following code:

```C++
private: \
	int  intbean = m_beanManager->getBean<int>("intbean"); \
	std::string&  stringBean = m_beanManager->getBean<std::string&>("beanName"); \
	SampleClass*  sampleClassInstance = m_beanManager->getBean<SampleClass*>("sampleClassInstance"); \
public: \
	static const std::vector<std::string>& getResourceNames() { \
		static std::vector<std::string> resourceNames { \
			"intbean", \
			"beanName", \
			"sampleClassInstance" \
		}; \
		return resourceNames; \
	}
```
Note that up to 256 resources can be specified in a single Configuration in the manner as shown here.

#### END_CONFIGURATION Macro

```C++
END_CONFIGURATION
```
This Macro is present to maintain the symetry of the Configuration definition, and it merely places an end to the class. Meaning that it just simply converts to 
```C++
};
```

### BeanManager

As mentioned, the BeanManager is responsible for managing the specific beans within the Context. The Configurations will pull resources from it, push beans into it, and through this allow for instances of classes or types to be easily passed around the application. To push a bean into the BeanManager it must be registered, and registering the bean can be done in one of two ways:

* Instance
* Creator

#### Instance

An instance bean is essentially a specific instance of an object or type, that is registered under a name within the BeanManager. The nature of what this truly means is dependent on what type is registered (pointer, reference, or scalar), with the result being the same as if the type was retrieve via a "get" member function. A pointer or reference will pass the pointer or reference around and thus sharing the same instance across multiple actors, whereas a scalar will be passed via copy with all of the ramafications that that bring with it. When an instance is passed, the BeanManager will not be responsible for managing any memory associated with it. Therefore the Configuration will allocated the memory should be then responsible for deallocating it upon desctruction, or some other approach taken based on what is applicable in the specific circumstance. To register an instance simply

```C++
BeanManager::registerBeanInstance<Type>("Name", someInstance);
```

Note that the compiler may be able to deduce the type on its own, however this is not a guarentee. For example when passing a scalar instance the compiler will most likley deduce a scalar type, instead of a reference that may be more desirable. For this reason the Configuration Macros do not take the change and force the type to be specified.

#### Creator

This is a more interesting means through which to manage beans. In this situation no instance is created outright, but rather a Creator is specified to indicate how the bean is to be created and managed. There are several Creators provided (see BeanCreator.h):

* corm::SingletonBeanCreator<Type> - creates a singleton instance which is provided as either a pointer or reference (note that scalar is not supported). The creator will ensure that the singleton instance is destroyed when the creator is destroyed (it will only be destroyed when the BeanManager is destroyed when the Context is destroyed)
* corm::FactoryBeanCreator<Type> - creates a new instance each time the bean is resourced, returning either a scalar or a pointer (note that reference is not supported). The creator does not track created instances, and thus will not clean up any memory upon destruction. For this reason it is recommended to use this with scalars to avoid memory leaks.
* corm::SmartSingleBeanCreator<Type, Ptr> - equivalent to the corm::SingleBeanCreator, except that the single pointer is contained within a smart pointer. The type of smart pointer can be specified via the Ptr template and it will default to std::shared_ptr.
* corm::SmartFactoryBeanCreator<Type, Ptr> - equivalent to corm::FactoryBeanCreator, except that the created instance wrapped within a smart pointer. The type of smart pointer can be specified via the Ptr template and it will default to std::shared_ptr.

The type of creator to use is specified when the bean is registered

```C++
BeanManager::registerBean<Type, Creator>();
```

Note that the for the provided creators the bean is not created until the bean is requested for the first time, meaning that if the bean is not resourced the creator will not create it.

It is possible and easy to create custom creators. There are however limitations:

* The creator must have a default constructor. No other constructor is supported or will be used by the BeanManager
* Must contain a create member function corm::ValueWrapper<Type>* create()

The ValueWrapper<Type> is employed to work around limitations of templates within C++, specifically to ensure that type references are not broken while moving through the BeanManager. For example the following is a custom creator which will create int 123 each time it is called.

```C++
struct Example123Creator {
	corm::ValueWrapper<int> create() {
		return 123;
	}
};
```

It could then be used to create a bean as simple as 

```C++
m_beanManager->registerBean<int, Example123Creator>();
```

or within a Configuration file

```C++
BEANS(
	(BEAN, int, Example123Creator, "myNewBean")
)
```

### Example

To show how this all comes together a few Configuration files and a Context will be included as a contrived example. So to start with, we need some Configurations

Configuration1: provides some ints. Note that this does not require any resources so the RESOURCES macro is absent
```C++
// Config which requires a single resource from ProviderManagerTestConfig and provides a single bean
CONFIGURATION(Configuration1)

	BEANS(
		(BEAN, int, Example123Creator, "int1"),
		(BEAN_INSTANCE, int, "int2", 456),
		(BEAN_INSTANCE, int, "int3", 789)
	)

END_CONFIGURATION
```

Configuration2: takes ints to create a vector
```C++
// Config which requires a single resource from ProviderManagerTestConfig and provides a single bean
CONFIGURATION(Configuration2)

private:
	std::vector<int> allInts;

protected:
	void postInit() {
		// Called during initialization after resource are available
		allInts.push_back(int1);
		allInts.push_back(int2);
		allInts.push_back(int3);
	}

	BEANS(
		(BEAN_INSTANCE, std::vector<int>&, allInts)
	)

	RESOURCES(
		(int, int1),
		(int, int2),
		(int, int3)
	)

END_CONFIGURATION
```

Configuration3: specifies Configuration1 and Configuation2 as dependencies, and adds some more ints to the vector
```C++
CONFIGURATION(Configuration3)

DEPENDENCIES(Configuration1, Configuation2)

protected:
	void postInit() {
		m_allInts.push_back(13579);
		m_allInts.push_back(24680);
	}

	RESOURCES(
		(std::vector<int>&, "allInts", m_allInts)
	)
	
END_CONFIGURATION
```

Create a context which loads the above three Configurations
```C++
corm::Context context;
context.registerConfiguration<Configuration3>();
// load any other Configurations
context.assemble();
```
