#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>

template <typename T>
class Singleton {
public:
    // Deleted copy constructor and assignment operator to prevent copying
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // Static method to get the single instance of the class
    static T& getInstance() {
        static T instance;  // Guaranteed to be destroyed, instantiated on first use
        return instance;
    }

protected:
    // Protected constructor and destructor to prevent direct instantiation
    Singleton() {}
    virtual ~Singleton() {}

    // Friend declaration to allow access to the protected constructor
    friend T;
};

#endif