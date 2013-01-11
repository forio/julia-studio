require("pkg.jl")
require("sandbox.jl")
require("event.jl")

module __ConsoleLogic

import __Sandbox
import __Event
import Pkg

type ConsoleLogicSystem <: __Sandbox.System
end


### Events ###########################
function OnEvalMsg(console::ConsoleLogicSystem, code)
  event_system = __Sandbox.GetSystem(__Event.EventSystem)

  try
    expr = parse_input_line(code)
    parsed_result = isa(expr, Expr)

    if parsed_result && expr.head == :error
      return __Event.NewEvent(event_system, "network-output", "output-error", sprint(repl_show, expr.args[1]))
    end

    result = eval(expr)

    if isa(result, Nothing)
      return __Event.NewEvent(event_system, "network-output", "output-eval", "")
    else
      return __Event.NewEvent(event_system, "network-output", "output-eval", sprint(repl_show, result))
    end

  catch error
    return __Event.NewEvent(event_system, "network-output", "output-error", sprint(repl_show, error))
  end
end

function OnPackageMsg(console::ConsoleLogicSystem, command, params...)
  event_system = __Sandbox.GetSystem(__Event.EventSystem)

  if isequal(command, "available")
    #println("REQUEST FOR AVAILABLE BEING ANSWERED")
    packages = Pkg.available()
    #println(packages)
    __Event.NewEvent(event_system, "network-output", "output-package", "available", packages...)

  elseif isequal(command, "required")
    package_map = Pkg.required()
    packages = Array(ASCIIString, 0)
    for p in package_map
      push!(packages, p.package)
    end

    #println("REQUEST FOR INSTALLED BEING ANSWERED")
    #println(packages)
    __Event.NewEvent(event_system, "network-output", "output-package", "required", packages...)

  elseif isequal(command, "add")
    #println("JULIA RECIEVED ADD FOR ", params)
    for pkg in params
      Pkg.add(pkg)
    end
    println()
    __Event.NewEvent(event_system, "network-output", "output-package", "add")

  elseif isequal(command, "remove")
    #println("JULIA RECIEVED REMOVE FOR ", params)
    for pkg in params
      Pkg.rm(pkg)
    end
    println()
    __Event.NewEvent(event_system, "network-output", "output-package", "remove")

  elseif isequal(command, "update")
    #println("JULIA RECIEVED UPDATE")
    Pkg.update()
    __Event.NewEvent(event_system, "network-output", "output-package", "update")
  end
end

function OnDirMessage(console::ConsoleLogicSystem, dir)
  if !isdir(dir)
    error("cannot change directory to $dir")
  end

  #println("CHANGING DIR: $dir")
  cd(dir)

  event_system = __Sandbox.GetSystem(__Event.EventSystem)
  __Event.NewEvent(event_system, "network-output", "output-dir", dir)
end


### Sandbox interface ###########################
function Load(core::__Sandbox.SandboxCore)
  return ConsoleLogicSystem()
end

function Init(console::ConsoleLogicSystem, core::__Sandbox.SandboxCore)
  event_system = __Sandbox.GetSystem(__Event.EventSystem)

  __Event.RegisterHandler(event_system, "eval", (msg)->OnEvalMsg(console, msg))
  __Event.RegisterHandler(event_system, "package", (msg...)->OnPackageMsg(console, msg...))
  __Event.RegisterHandler(event_system, "dir", (msg)->OnDirMessage(console, msg))

  if !Pkg.is_initialized()
    println("Setting up the package manager for the first time...")
    Pkg.init()
  end
end

function Update(system::ConsoleLogicSystem)
  #println("ConsoleLogic shares in NetworkManager's distain.")
end


end
