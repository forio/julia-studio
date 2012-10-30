namespace JuliaPlugin {

// ----------------------------------------------------------------------------
template < class Type,
           template < class > class CreationPolicy,
           template < class > class LifetimePolicy >
Type* Singleton<Type, CreationPolicy, LifetimePolicy>::GetInstance()
{
  if ( !instance )
  {
    // handle re-creation
    if ( destroyed )
    {
      // Typically OnDeadReference will simply return. If this singleton does
      // not support re-creation then it will assert.
      LifetimePolicy<Type>::OnDeadReference();
      destroyed = false;
    }

    // Create the new instance and give the LifetimePolicy a way to destroy the
    // singleton. The LifetimePolicy will decide when to destroy the singleton.
    instance = CreationPolicy<Type>::Create();
    LifetimePolicy<Type>::ScheduleDestruction( &DestroySingleton );
  }

  return instance;
}

// ----------------------------------------------------------------------------
template < class Type,
           template < class > class CreationPolicy,
           template < class > class LifetimePolicy >
void Singleton<Type, CreationPolicy, LifetimePolicy>::DestroySingleton()
{
  //ReleaseErrorIf ( destroyed, "Error: Singleton being destroyed twice!" );
  assert( !destroyed );

  CreationPolicy<Type>::Destroy( instance );
  instance = NULL;
  destroyed = true;
}

}
