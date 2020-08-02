#ifndef BEANCREATOR_H_
#define BEANCREATOR_H_

#include <memory>

namespace corm {

/*
 * Simple wrapper struct, which allow passing the created type from the BeanCreator
 * to the BeanProvider without the bean provider being dependent on what type of
 * value the creator is creating.
 *
 * This will wrap the value (whatever type it is) in a pointer, that can then easily
 * be passed around.
 */
template<typename T>
struct ValueWrapper {

	T value;

	ValueWrapper(T val) :
			value(val) {
	}
};

/*
 * Singleton creator for a scalar.
 *
 * Note that a scalar singleton is an oxymoron, so this is
 * not to be used (and left empty for normal usage). However, it is required to be implemented
 * when the auto bean registration is enabled (will not compile otherwise if scalars are used
 * anywhere). Therefore the implementation is provided to allow bean auto registration.
 */
template<typename T>
struct SingletonBeanCreator {
#ifdef ENABLE_BEAN_AUTOREGISTRATION
	ValueWrapper<T>* create() {
		return new ValueWrapper<T>(instance);
	}

private:
	T instance;
#endif
};

/*
 * Singleton creator for references
 */
template<typename T>
struct SingletonBeanCreator<T&> {

	ValueWrapper<T&>* create() {
		return new ValueWrapper<T&>(instance);
	}

private:
	// The managed singleton instance
	T instance = T();
};

/*
 * Singleton creator for pointers
 */
template<typename T>
struct SingletonBeanCreator<T*> {
	virtual ~SingletonBeanCreator() {
		if (instance != NULL)
			delete(instance);
	}

	virtual ValueWrapper<T*>* create() {
		// Delay creation until it is actually needed
		if (instance == NULL)
			instance = new T();

		return new ValueWrapper<T*>(instance);
	}

private:
	// The managed singleton instance
	T *instance = NULL;
};

/*
 * Creator where a new scalar instance is created each time create is called.
 * The instance is passed by copy.
 */
template<class T>
struct FactoryBeanCreator {
	ValueWrapper<T>* create() {
		return new ValueWrapper<T>(T());
	}
};

/*
 * Intentionally left unimplemented, a factory that returns references does not make sense.
 */
template<typename T>
struct FactoryBeanCreator<T&> {
};

/*
 * Creator where a new pointer instance is created each time create is called. Note that
 * the instance is created via new, therefore the client code is responsible for memory
 * management.
 */
template<typename T>
struct FactoryBeanCreator<T*> {
	ValueWrapper<T*>* create() {
		return new ValueWrapper<T*>(new T());
	}
};

/*
 * Singleton creator which wraps the singleton instance in a smart pointer. Defaults to a std::shared_ptr.
 */
template<typename T, template <typename> class Ptr = std::shared_ptr>
struct SmartSingletonBeanCreator {
	ValueWrapper<Ptr<T>>* create() {
		static Ptr<T> smrtPtr(new T());
		return new ValueWrapper<Ptr<T>>(smrtPtr);
	}
};

/*
 * Creates a new instance with each invocation, and wraps that instance in a smart pointer. Defaults to a
 * std::shared_ptr.
 */
template<typename T, template <typename> class Ptr = std::shared_ptr>
struct SmartFactoryBeanCreator {
	ValueWrapper<Ptr<T>>* create() {
		return new ValueWrapper<Ptr<T>>(Ptr<T>(new T()));
	}
};

}

#endif /* BEANCREATOR_H_ */
