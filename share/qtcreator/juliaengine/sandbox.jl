
module __Sandbox

### Basic type definitions ###########################

abstract Component
abstract System

type Entity
  id::Uint
  components::Dict{Any, Array{Any}}  # components indexed by their singleton type
end

function Entity(components::Tuple)
  entity = Entity(GetUniqueId(), Dict{Any, Array{Any}}())
  RegisterEntity(entity)
  return AddComponents(entity, components)
end


# used internally by SandboxCore, not exported
type ModuleBundle
  mdl::Module
  system::System

  update
end


type SandboxCore
  modules::Array{ModuleBundle, 1}  # initialized / updated in order, destroyed in reverse order
  entities::Dict{Uint, Entity}
  components::Dict{Any, Array{WeakRef}}

  curr_unique_id::Uint
end

function SandboxCore()
  return SandboxCore(Array(ModuleBundle, 0), Dict{Uint, Entity}(), Dict{Any, Array{WeakRef}}(), uint(0))
end


global core = SandboxCore()



### Run ###########################

function Run( run_control_fn )
  run_control_fn(core)
end

# This loop will update each system in order and then stall until
# at least 'desired_frame_time' (in seconds) has passed. Of course,
# there is nothing 'RegulatedLoop' can do if updating the systems
# takes longer than 'desired_frame_time'
function RegulatedLoop(core, desired_frame_time)
  while true
    for module_bundle in core.modules
      if module_bundle.update != Nothing
        module_bundle.mdl.update(module_bundle.system)
      end
    end
  end
end



### Modules ###########################

function AddModule(mdl::Module)
  mdl_update_fn = Nothing
  for n in names(mdl)
    if n == :Update
      mdl_update_fn = n
    end
  end
  module_bundle = ModuleBundle(mdl, mdl.Load(__Sandbox.core), mdl_update_fn)

  push!(core.modules, module_bundle)
end

function Init()
  for module_bundle in core.modules
    module_bundle.mdl.Init(module_bundle.system, core)
  end
end

function GetSystem(module_type::Module)
  # if this function is going to be called often we should make
  # a module map in addition to a vector
  for bundle in core.modules
    if bundle.mdl == module_type
      return bundle.system
    end
  end
  return Nothing
end

function GetSystem{ T<:System }(system_type::Type{T})
  # if this function is going to be called often we should make
  # a module map in addition to a vector
  for bundle in core.modules
    if typeof(bundle.system) == system_type
      return bundle.system
    end
  end
  return Nothing
end



### Components ###########################

function AddComponents(entity::Entity, components...)
  for c in components
    comp_type = typeof(c)
    type_single = Type{comp_type}

    while type_single != Type{Component}
      if has(entity.components, type_single)
        push!(ref(entity.components, type_single), c)
      else
        entity.components[type_single] = [c]
      end

      if has(core.components, type_single)
        push!(ref(core.components, type_single), WeakRef(c))
      else
        core.components[type_single] = [WeakRef(c)]
      end

      comp_type = super(comp_type)
      type_single = Type{comp_type}
    end
  end

  return entity
end

function AddComponent(entity::Entity, component)
  AddComponents(entity, component)
end

function GetComponents(of_type)
  return core.components[Type{of_type}]
end

function GetComponents(ent::Entity, of_type)
  return ent.components[Type{of_type}]
end

function GetComponent(ent::Entity, of_type)
  comps = GetComponents(ent, of_type)

  # note that, if there are multiple components of the type you requested,
  # the one you get is officially undefined.
  return length(comps) > 0 ? comps[1] : Nothing
end

function GetEntity(component)

end


### Entities ###########################

function GetEntity(id::Uint)
  return core.entities[id]
end

function NewEntity(components...)
  components = map((c)->c(), components)
  entity = Entity(components)
end



### Internal utility ###########################

function GetUniqueId()
  return core.curr_unique_id += 1
end

function RegisterEntity(entity::Entity)
  if has(core.entities, entity.id)
    error("Entity.id must be unique!")
  end

  core.entities[entity.id] = entity
end

end
