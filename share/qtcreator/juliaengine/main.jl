
require("network.jl")
require("event.jl")
require("consolelogic.jl")
require("sandbox.jl")

__Sandbox.AddModule(__Event)
__Sandbox.AddModule(__Network)
__Sandbox.AddModule(__ConsoleLogic)

__Sandbox.Init()

#Event.NewEvent(Sandbox.GetSystem(Event), "network-output", "boo", 1, 2, 3)

#println("running!!!")
__Sandbox.Run( __Event.EventLoop )

