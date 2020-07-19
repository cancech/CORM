/*
 * BeanCreator.h
 *
 *  Created on: Jul. 11, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef BEANCREATOR_H_
#define BEANCREATOR_H_

namespace corm {

/*
 * Bean creator where a singleton scalar instance is used. Note that in order
 * for this instance to be a true singleton it must be loaded as a reference
 * on the client side (i.e.: T& t = creator.create()).
 */
template<class T>
struct SingletonBeanCreator {

	T& create() {
		return instance;
	}

private:
	// The managed singleton instance
	T instance;
};

/*
 * Bean creator where a singleton pointer instance is used.
 */
template<class T>
struct SingletonBeanCreator<T*> {
	T* create() {
		if (instance == NULL)
			instance = new T();

		return instance;
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
	T create() {
		return T();
	}
};

/*
 * Creator where a new point instance is created each time create is called. Note that
 * the instance is created via new, therefore the client code is responsible for memory
 * management.
 */
template<class T>
struct FactoryBeanCreator<T*> {
	T* create() {
		return new T();
	}
};

}

#endif /* BEANCREATOR_H_ */
