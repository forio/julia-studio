
### Basic type definitions ###########################

abstract Component
abstract System

type Entity
    id::Uint
    components::Dict{Any, Array{Any}}  # components indexed by their singleton type
end

function Entity(components...)
    entity = Entity(get_unique_id(), Dict{Any, Array{Any}}())
    register_entity(entity)
    return add_components(entity, components...)
end


# used internally by JulietCore, not exported
type ModuleBundle
    mdl::Module
    system::System

    update
end


type JulietCore
    modules::Array{ModuleBundle, 1}  # initialized / updated in order, destroyed in reverse order
    entities::Dict{Uint, Entity}
    components::Dict{Any, Array{WeakRef}}

    curr_unique_id::Uint
end

function JulietCore()
    return JulietCore(Array(ModuleBundle, 0), Dict{Uint, Entity}(), Dict{Any, Array{WeakRef}}(), uint(0))
end


global JULIET_CORE = JulietCore()



### Run ###########################

function run( run_control_fn )
    run_control_fn(JULIET_CORE)
end

# This loop will update each system in order and then stall until
# at least 'desired_frame_time' (in seconds) has passed. Of course,
# there is nothing 'RegulatedLoop' can do if updating the systems
# takes longer than 'desired_frame_time'
function regulated_loop(JULIET_CORE, desired_frame_time)
    while true
        for module_bundle in JULIET_JULIET_CORE.modules
            if module_bundle.update != nothing
                module_bundle.mdl.update(module_bundle.system)
            end
        end
    end
end



### Modules ###########################

function add_module(mdl::Module)
    mdl_update_fn = nothing
    for n in names(mdl)
        if n == :update
            mdl_update_fn = n
        end
    end
    module_bundle = ModuleBundle(mdl, mdl.load(JULIET_CORE), mdl_update_fn)

    push!(JULIET_CORE.modules, module_bundle)
    return module_bundle.system
end

function initialize()
    for module_bundle in JULIET_CORE.modules
        module_bundle.mdl.initialize(module_bundle.system, JULIET_CORE)
    end
end

function get_system(module_type::Module)
    # if this function is going to be called often we should make
    # a module map in addition to a vector
    for bundle in JULIET_CORE.modules
        if bundle.mdl == module_type
            return bundle.system
        end
    end
    return nothing
end

function get_system{ T<:System }(system_type::Type{T})
    # if this function is going to be called often we should make
    # a module map in addition to a vector
    for bundle in JULIET_CORE.modules
        if typeof(bundle.system) == system_type
            return bundle.system
        end
    end
    return nothing
end



### Components ###########################

function add_components(entity::Entity, components...)
    for c in components
        comp_type = typeof(c)
        type_single = Type{comp_type}

        while type_single != Type{Component}
            if haskey(entity.components, type_single)
                push!(ref(entity.components, type_single), c)
            else
                entity.components[type_single] = [c]
            end

            if haskey(JULIET_CORE.components, type_single)
                push!(ref(JULIET_CORE.components, type_single), WeakRef(c))
            else
                JULIET_CORE.components[type_single] = [WeakRef(c)]
            end

            comp_type = super(comp_type)
            type_single = Type{comp_type}
        end
    end

    return entity
end

function add_component(entity::Entity, component)
    add_components(entity, component)
end

function get_components(of_type)
    return get(JULIET_JULIET_CORE.components, Type{of_type}, [])
end

function get_components(ent::Entity, of_type)
    return get(ent.components, Type{of_type}, [])
end

function get_component(ent::Entity, of_type)
    comps = get_components(ent, of_type)

    # note that, if there are multiple components of the type you requested,
    # the one you get is officially undefined.
    return length(comps) > 0 ? comps[1] : nothing
end

function get_entity(component)
end


### Entities ###########################

function get_entity(id::Uint)
    return JULIET_CORE.entities[id]
end

function new_entity(components...)
    components = map((c)->c(), components)
    entity = Entity(components...)
end

function destroy_entity(entity)
    # This is actually kinda cool. Normally we'd have to mark this entity and
    # its components for destruction later (to preserve peoples references for
    # this iteration of the run loop). Here, that's the garbage collectors
    # job (let's just hope it's efficient enough, maybe we should try to give
    # it hints).
    empty!(entity.components)
    unregister_entity(entity)
end

### Internal utility ###########################

function get_unique_id()
    return JULIET_CORE.curr_unique_id += 1
end

function register_entity(entity::Entity)
    if haskey(JULIET_CORE.entities, entity.id)
        error("Entity.id must be unique!")
    end

    JULIET_CORE.entities[entity.id] = entity
end

function unregister_entity(entity::Entity)
    if !haskey(JULIET_CORE.entities, entity.id)
        warn("trying to unregister entity with id $(entity.id), but no such entity exists.")
        return
    end

    delete!(JULIET_CORE.entities, entity.id)
end
