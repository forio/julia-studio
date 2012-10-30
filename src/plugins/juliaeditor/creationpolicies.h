// CreationPolicies.hpp : This file contains creation policies to be used with
//                        singletons.

#ifndef CREATIONPOLICIES_H
#define CREATIONPOLICIES_H

namespace JuliaPlugin {

template < typename Type >
class CreateUsingNew
{
public:
  static Type* Create ()
  {
    return new Type;
  }

  static void Destroy ( Type* instance )
  {
    delete instance;
  }

};

// *******

template < typename Type >
class CreateStatic
{
public:
  static Type* Create ()
  {
    static Type instance;
    return &instance;
  }

  static void Destroy ( Type* instance )
  {
    // static instance is static
  }

};

// *******

template < typename Type >
class CreateUsingMalloc
{
public:
  static Type* Create ()
  {
    void* ptr = malloc( sizeof( Type ) );
    return new (ptr) Type;
  }

  static void Destroy ( Type* instance )
  {
    instance->~Type();
    free( instance );
  }

};

}


#endif // CREATIONPOLICIES_H
