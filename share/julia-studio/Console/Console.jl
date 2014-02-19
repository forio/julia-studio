if !haskey( Pkg.installed(), "REPLCompletions" )
  Pkg.add("REPLCompletions" )
end

require("../juliet/src/Juliet.jl")
include("ConsoleLogic.jl")

import Juliet
import Juliet.Event
import Juliet.Network
import ConsoleLogic

Juliet.add_module(ConsoleLogic)
Juliet.add_module(Event)
network_sys = Juliet.add_module(Network)

Juliet.Network.accept_one_on(network_sys, 4444)

Juliet.initialize()

Juliet.run( Event.event_loop )
