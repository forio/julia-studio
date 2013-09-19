
module ConsoleLogic

using Juliet
import Event

type ConsoleLogicSystem <: System
end


### Events ###########################
function on_eval_msg(console::ConsoleLogicSystem, code)
  event_system = get_system(Event.EventSystem)

  try
    parsed_expr = parse(code)
    result = @eval $parsed_expr

    if isa(result, Nothing)
      return Event.new_event(event_system, "network-output", "output-eval", "")
    else
      return Event.new_event(event_system, "network-output", "output-eval", sprint(repl_show, result))
    end

  catch error
    return Event.new_event(event_system, "network-output", "output-error", sprint(Base.error_show, error))
  end
end

function on_pkg_msg(console::ConsoleLogicSystem, command, params...)
  event_system = get_system(Event.EventSystem)

  if isequal(command, "available")
    try
      packages = Pkg.available()
      Event.new_event(event_system, "network-output", "output-package", "available", packages...)
    catch
      Event.new_event(event_system, "network-output", "output-package", "failed")
    end

  elseif isequal(command, "required")
    try
      package_map = Pkg.required()
      packages = Array(ASCIIString, 0)
      for p in package_map
        push!(packages, p.package)
      end

      Event.new_event(event_system, "network-output", "output-package", "required", packages...)
    catch
      Event.new_event(event_system, "network-output", "output-package", "failed")
    end

  elseif isequal(command, "add")
    try
      for pkg in params
        Pkg.add(pkg)
      end

      println()
      Event.new_event(event_system, "network-output", "output-package", "add")
    catch err
      println()
      Event.new_event(event_system, "network-output", "output-package", "add", "failed")
    end

  elseif isequal(command, "remove")
    try
      for pkg in params
        Pkg.rm(pkg)
      end
      println()
      Event.new_event(event_system, "network-output", "output-package", "remove")
    catch err
      Event.new_event(event_system, "network-output", "output-package", "remove", "failed")
    end

  elseif isequal(command, "update")
    try
      Pkg.update()
      Event.new_event(event_system, "network-output", "output-package", "update")
    catch err
      Event.new_event(event_system, "network-output", "output-package", "update", "failed")
    end
  end
end

function on_dir_msg(console::ConsoleLogicSystem, dir)
  if !isdir(dir)
    error("cannot change directory to $dir")
  end

  cd(dir)

  event_system = get_system(Event.EventSystem)
  Event.new_event(event_system, "network-output", "output-dir", dir)
end


### Juliet interface ###########################
function load(core::JulietCore)
  return ConsoleLogicSystem()
end

function initialize(console::ConsoleLogicSystem, core::JulietCore)
  event_system = get_system(Event.EventSystem)

  Event.RegisterHandler(event_system, "eval", (msg)->on_eval_msg(console, msg))
  Event.RegisterHandler(event_system, "package", (msg...)->on_pkg_msg(console, msg...))
  Event.RegisterHandler(event_system, "dir", (msg)->on_dir_msg(console, msg))

  if !isdir(Pkg.dir())
    println("Initializing package repo for the first time, hold on.\n")
    try
      Pkg.init()
    catch end
  end
end

function update(system::ConsoleLogicSystem)
  #println("ConsoleLogic shares in NetworkManager's distain.")
end


end
