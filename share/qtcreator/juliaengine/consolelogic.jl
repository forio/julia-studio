require("sandbox.jl")
require("event.jl")

module __ConsoleLogic

import __Sandbox
import __Event

type ConsoleLogicSystem <: __Sandbox.System
end

function ParseLines(__input)
    __lines = split(__input, '\u2029')
    __parsed_exprs = {}
    __input_so_far = ""
    __all_nothing = true

    for i = 1:length(__lines)
        __input_so_far = string(__input_so_far, __lines[i], "\n")
        __expr = Base.parse_input_line(__input_so_far)

        # if there was nothing to parse, just keep going
        if __expr == nothing
            continue
        end

        __all_nothing = false
        __expr_multitoken = isa(__expr, Expr)

        # stop now if there was a parsing error
        if __expr_multitoken && __expr.head == :error
            return [__expr.args[1]]
        end

        # if the expression was incomplete, just keep going
        if __expr_multitoken && __expr.head == :continue
            continue
        end

        # add the parsed expression to the list
        __input_so_far = ""
        __parsed_exprs = [__parsed_exprs, __expr]
    end
    return __parsed_exprs
end


### Events ###########################
function OnEvalMsg(console::ConsoleLogicSystem, code)
  event_system = __Sandbox.GetSystem(__Event.EventSystem)

  #println("code:", code)
  try
    __parsed_exprs = ParseLines(code)
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
