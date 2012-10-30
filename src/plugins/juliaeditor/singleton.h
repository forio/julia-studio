// Singleton.hpp : This is an implementation of the "Singleton Holder" as discussed
//                 in "Modern C++ Design".

// Example Usage:
//
//  #define YourMacro Singleton< YourClass, CreationPolicy, LifetimePolicy >::GetInstance()
//

#ifndef SINGLETON_H
#define SINGLETON_H

#include "CreationPolicies.h"
#include "LifetimePolicies.h"

namespace JuliaPlugin {

template < class Type,
           template < class > class CreationPolicy = CreateUsingNew,
           template < class > class LifetimePolicy = StandardLifetime >
class Singleton
{
public:
    static Type* GetInstance ();

private:
    static void DestroySingleton ();

    static Type* instance;
    static bool destroyed;

    // Do not call
    Singleton ();
    Singleton ( Singleton const & );
    Singleton& operator = ( Singleton const & );

};


// instance pointer
template < class Type,
    template < class > class CreationPolicy,
    template < class > class LifetimePolicy >
Type* Singleton< Type,
    CreationPolicy,
    LifetimePolicy >::instance = NULL;


// destroyed bool
template < class Type,
    template < class > class CreationPolicy,
    template < class > class LifetimePolicy >
bool Singleton< Type,
    CreationPolicy,
    LifetimePolicy >::destroyed = false;

}


#include "singleton.cpp"

#endif // SINGLETON_H
