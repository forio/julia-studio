
module Event

import Juliet

####
type EventInfo
    name::String
    params::Tuple
end

####
type EventSystem <: Juliet.System
    registrar::Dict{String, Array{Any}}  # handlers indexed by event name
    pending_events::Array{EventInfo}
    event_signal::RemoteRef
end

EventSystem() = EventSystem(Dict{String, Array{Any}}(), Array(EventInfo, 0), RemoteRef())


### Interface ###########################
function register_handler(system, event_name, handler)
    if haskey(system.registrar, event_name)
        push!(ref(system.registrar, event_name), handler)
    else
        system.registrar[event_name] = [handler]
    end
end

function unregister_handler(system, event_name, handler)
    handlers = get(system.registrar, event_name, Array(Any, 0))
    for i = 1 : length(handlers)
        if handlers[i] == handler
            del(handlers, i)
            break
        end
    end
end

function new_event(system, event_name, parameters...)
    push!(system.pending_events, EventInfo(event_name, parameters))

    if !isready(system.event_signal)
        put!(system.event_signal, nothing)
    end
end


### Juliet interface ###########################
function load(core::Juliet.JulietCore)
    return EventSystem()
end

function initialize(system::EventSystem, core::Juliet.JulietCore)
end

function update(system::EventSystem)
    handle_pending_events(system)
end

# This loop will wait until the event system has something pending, then
# update each system in order (note: the event system is not necessarily
# updated first.
function event_loop(core::Juliet.JulietCore)
    event_system = Juliet.get_system(EventSystem)

    while true
        take!(event_system.event_signal)
        for module_bundle in core.modules
            #if module_bundle.update != Nothing
                module_bundle.mdl.update(module_bundle.system)
            #send
        end
    end
end


### Utility ###########################
function handle_pending_events(system::EventSystem)
    for event in system.pending_events
        handlers = get(system.registrar, event.name, Array(Any, 0))
        for handler in handlers
            handler(event.params...)
        end
    end

    if isready(system.event_signal)
        take!(system.event_signal)
    end

    # Note that more events may have been queued while we were handling
    # the origional set, however, they were appeneded to the end and thus
    # handled. If we implement priority this may break.
    empty!(system.pending_events)
end

end
