#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>

/*
 * Base class for defining static, but not really static, classes.
 */
template <typename T>
class Singleton {
public:

    /*
     * Deleted copy constructor and assignment operator to prevent copying.
     */
    Singleton(const Singleton&) = delete;

    /*
     * Deleted copy constructor and assignment operator to prevent copying.
     */
    Singleton& operator=(const Singleton&) = delete;

    /*
     * Static method to get the single instance of the class.
     * Call as;
     * ```cpp
     * const auto mySingleton = &SomeSingletonClass::getInstance();
     * ```
     */
    static T& getInstance() {
        static T instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

protected:

    /*
     * Protected constructor and destructor to prevent direct instantiation.
     */
    Singleton() {}
    
    /*
     * Protected constructor and destructor to prevent direct instantiation.
     */
    virtual ~Singleton() {}

    /*
     * Friend declaration to allow access to the protected constructor.
     */
    friend T;
};

#endif