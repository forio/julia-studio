
module Juliet

# types
export  System,
        Entity,
        Component,
        JulietCore,

# module interface
        add_module,
        initialize,
        get_system,

# entity interface,
        add_components,
        add_component,
        get_components,
        get_component,
        get_entity,
        new_entity,
        destroy_entity


include("juliet-engine.jl")

# bundled modules
include("modules/event/event.jl")
include("modules/network/network.jl")

end
