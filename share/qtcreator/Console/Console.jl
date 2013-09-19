
require("../Juliet.jl/src/Juliet.jl")
include("ConsoleLogic.jl")

import Juliet
import Juliet.Event
import Juliet.Network
import ConsoleLogic

#Juliet.add_module(ConsoleLogic)
Juliet.add_module(Event)
network_sys = Juliet.add_module(Network)

Network.accept_all_on(getwork_sys, 4444)

Juliet.initialize()

Juliet.run( Event.event_loop )
