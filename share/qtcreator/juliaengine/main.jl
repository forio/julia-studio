
require("network.jl")
require("event.jl")
require("consolelogic.jl")
require("sandbox.jl")

__Sandbox.AddModule(__Event)
__Sandbox.AddModule(__ConsoleLogic)
__Sandbox.AddModule(__Network)

__Sandbox.Init()

__Sandbox.Run( __Event.EventLoop )

