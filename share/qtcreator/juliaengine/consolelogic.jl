require("pkg.jl")
require("sandbox.jl")
require("event.jl")

module __ConsoleLogic

import __Sandbox
import __Event

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
    try
      packages = Pkg.available()
      __Event.NewEvent(event_system, "network-output", "output-package", "available", packages...)
    catch
      __Event.NewEvent(event_system, "network-output", "output-package", "failed")
    end

  elseif isequal(command, "required")
    try
      package_map = Pkg.required()
      packages = Array(ASCIIString, 0)
      for p in package_map
        push!(packages, p.package)
      end

      __Event.NewEvent(event_system, "network-output", "output-package", "required", packages...)
    catch
      __Event.NewEvent(event_system, "network-output", "output-package", "failed")
    end

  elseif isequal(command, "add")
    try
      for pkg in params
        Pkg.add(pkg)
      end

      println()
      __Event.NewEvent(event_system, "network-output", "output-package", "add")
    catch err
      println()
      __Event.NewEvent(event_system, "network-output", "output-package", "add", "failed")
    end

  elseif isequal(command, "remove")
    try
      for pkg in params
        Pkg.rm(pkg)
      end
      println()
      __Event.NewEvent(event_system, "network-output", "output-package", "remove")
    catch err
      __Event.NewEvent(event_system, "network-output", "output-package", "remove", "failed")
    end

  elseif isequal(command, "update")
    try
      Pkg.update()
      __Event.NewEvent(event_system, "network-output", "output-package", "update")
    catch err
      __Event.NewEvent(event_system, "network-output", "output-package", "update", "failed")
    end
  end
end

function OnDirMessage(console::ConsoleLogicSystem, dir)
  if !isdir(dir)
    error("cannot change directory to $dir")
  end

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

end

function Update(system::ConsoleLogicSystem)
  #println("ConsoleLogic shares in NetworkManager's distain.")
end


end
