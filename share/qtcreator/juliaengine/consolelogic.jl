
module ConsoleLogic

type ConsoleLogicSystem <: Juliet.System
end


### Events ###########################
function OnEvalMsg(console::ConsoleLogicSystem, code)
  event_system = Juliet.GetSystem(Event.EventSystem)

  try
    parsed_expr = parse(code)
    result = @eval $parsed_expr

    if isa(result, Nothing)
      return Event.NewEvent(event_system, "network-output", "output-eval", "")
    else
      return Event.NewEvent(event_system, "network-output", "output-eval", sprint(repl_show, result))
    end

  catch error
    return Event.NewEvent(event_system, "network-output", "output-error", sprint(Base.error_show, error))
  end
end

function OnPackageMsg(console::ConsoleLogicSystem, command, params...)
  event_system = Juliet.GetSystem(Event.EventSystem)

  if isequal(command, "available")
    try
      packages = Pkg.available()
      Event.NewEvent(event_system, "network-output", "output-package", "available", packages...)
    catch
      Event.NewEvent(event_system, "network-output", "output-package", "failed")
    end

  elseif isequal(command, "required")
    try
      package_map = Pkg.required()
      packages = Array(ASCIIString, 0)
      for p in package_map
        push!(packages, p.package)
      end

      Event.NewEvent(event_system, "network-output", "output-package", "required", packages...)
    catch
      Event.NewEvent(event_system, "network-output", "output-package", "failed")
    end

  elseif isequal(command, "add")
    try
      for pkg in params
        Pkg.add(pkg)
      end

      println()
      Event.NewEvent(event_system, "network-output", "output-package", "add")
    catch err
      println()
      Event.NewEvent(event_system, "network-output", "output-package", "add", "failed")
    end

  elseif isequal(command, "remove")
    try
      for pkg in params
        Pkg.rm(pkg)
      end
      println()
      Event.NewEvent(event_system, "network-output", "output-package", "remove")
    catch err
      Event.NewEvent(event_system, "network-output", "output-package", "remove", "failed")
    end

  elseif isequal(command, "update")
    try
      Pkg.update()
      Event.NewEvent(event_system, "network-output", "output-package", "update")
    catch err
      Event.NewEvent(event_system, "network-output", "output-package", "update", "failed")
    end
  end
end

function OnDirMessage(console::ConsoleLogicSystem, dir)
  if !isdir(dir)
    error("cannot change directory to $dir")
  end

  cd(dir)

  event_system = Juliet.GetSystem(Event.EventSystem)
  Event.NewEvent(event_system, "network-output", "output-dir", dir)
end


### Juliet interface ###########################
function Load(core::Juliet.JulietCore)
  return ConsoleLogicSystem()
end

function Init(console::ConsoleLogicSystem, core::Juliet.JulietCore)
  event_system = Juliet.GetSystem(Event.EventSystem)

  Event.RegisterHandler(event_system, "eval", (msg)->OnEvalMsg(console, msg))
  Event.RegisterHandler(event_system, "package", (msg...)->OnPackageMsg(console, msg...))
  Event.RegisterHandler(event_system, "dir", (msg)->OnDirMessage(console, msg))

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
