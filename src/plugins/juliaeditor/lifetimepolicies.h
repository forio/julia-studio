// LifetimePolicies.hpp : This file contains implementations of lifetime policies for
//                        use with singletons.

#ifndef LIFETIMEPOLICIES_H
#define LIFETIMEPOLICIES_H

#include <assert.h>
#include <stdlib.h>

namespace JuliaPlugin {

template < typename Type >
class StandardLifetime
{
public:
  typedef void (*DestructionFn) ();

  static void ScheduleDestruction ( DestructionFn fn )
  {
    std::atexit( fn );
  }

  static void OnDeadReference ()
  {
    // wow there, you said you wanted this destroyed at the end of
    // the program... did you lie to me?
    assert( "You lied to me." );
  }

};

// *******

template < typename Type >
class PhoenixLifetime
{
public:
  typedef void (*DestructionFn) ();

  static void ScheduleDestruction ( DestructionFn fn )
  {
    std::atexit( fn );
  }

  static void OnDeadReference ()
  {
    // Rise, together we rise.
  }

};

// *******

template < typename Type >
class LeakyLifetime
{
public:
  typedef void (*DestructionFn) ();

  static void ScheduleDestruction ( DestructionFn )
  {
    // leaky
  }

  static void OnDeadReference ()
  {
    assert( "Error: Leaky singleton was destroyed." );
  }

};

}

#endif // LIFETIMEPOLICIES_H
