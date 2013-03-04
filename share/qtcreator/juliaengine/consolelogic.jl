require("sandbox.jl")
require("event.jl")

import __Sandbox
import __Event

# TO BE REPLACED as soon as we move on from 0.1
function parse_lines(str::String, line_delim::Char)
  lines = split(str, line_delim)
  parsed_exprs = {}
  curr_str = ""

  for i = 1:length(lines)
    curr_str = string(curr_str, lines[i], "\n")
    ex = Base.parse_input_line(curr_str)

    if ex == nothing
      continue

    elseif isa(ex, Expr)

      if ex.head == :error
        return {ex}
      elseif ex.head == :continue
        continue
      end

    end

    curr_str = ""
    push!(parsed_exprs, ex)
  end

  return parsed_exprs
end

function __OnEvalMsg(__code)
  event_system = __Sandbox.GetSystem(__Event.EventSystem)

  try
    __parsed_exprs = parse_lines(__code, '\u2029')
    __result = nothing
    for __expression in __parsed_exprs
        __result = @eval $__expression
    end

    if isa(__result, Nothing)
      return __Event.NewEvent(event_system, "network-output", "output-eval", "")
    else
      return __Event.NewEvent(event_system, "network-output", "output-eval", sprint(repl_show, __result))
    end

  catch error
    return __Event.NewEvent(event_system, "network-output", "output-error", sprint(Base.error_show, error))
  end
end


module __ConsoleLogic

import __Sandbox
import __Event

require("consolelogic.jl")

### System ###########################
type ConsoleLogicSystem <: __Sandbox.System
end


### Events ###########################


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

  __Event.RegisterHandler(event_system, "eval", Main.__OnEvalMsg)
  __Event.RegisterHandler(event_system, "package", (msg...)->OnPackageMsg(console, msg...))
  __Event.RegisterHandler(event_system, "dir", (msg)->OnDirMessage(console, msg))

  if !isdir(Pkg.dir())
    println("Initializing package repo for the first time, hold on.\n")
    try
      Pkg.init()
    catch end
  end
end

function Update(system::ConsoleLogicSystem)
  #println("ConsoleLogic shares in NetworkManager's distain.")
end


end
