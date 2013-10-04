
module ConsoleLogic

using Juliet
import Juliet.Event

type ConsoleLogicSystem <: System
end

function showout( io, x )
  writemime( io, MIME"text/plain"(), x )
end

function isspaced( s )
  i = 1
  spaced = true
  while spaced && i <= endof( s )
    c = s[i]
    spaced = c == ' ' || c == '\t' || c == '\n' || c=='\r'
    i += 1
  end
  return spaced
end

### Events ###########################
function on_eval_msg(console::ConsoleLogicSystem, cid, code)
  event_system = get_system(Event.EventSystem)

  if isspaced( code )
    Event.new_event( event_system, "networkOut", cid, "output-eval", "" )
    return
  end
  try
    parsed_expr = parse(code)
    result = @eval $parsed_expr

    if isa( result, Nothing ) || rstrip( code )[end] == ';'
      Event.new_event( event_system, "networkOut", cid, "output-eval", "" )
    else
      Event.new_event( event_system, "networkOut", cid, "output-eval", sprint( showout, result ) )
    end

  catch error
    Event.new_event(event_system, "networkOut", cid, "output-error", sprint( showerror, error ) )
  end
end

function on_dir_msg(console::ConsoleLogicSystem, cid, dir)
  if !isdir(dir)
    error("cannot change directory to $dir")
  end

  cd(dir)

  event_system = get_system(Event.EventSystem)
  Event.new_event( event_system, "networkOut", cid, "output-dir", dir )
end


function on_pkg_msg(console::ConsoleLogicSystem, cid, data )
  event_system = get_system(Event.EventSystem)

  command = shift!( data );
  if isequal(command, "available")
    try
      packages = Pkg.available()
      unshift!( packages, command )
      
      Event.new_event( event_system, "networkOut", cid, "output-package", packages )
    catch
      Event.new_event( event_system, "networkOut", cid, "output-package", "failed" )
    end

  elseif isequal(command, "installed")
    try
      package_map = Pkg.installed()
      packages = Array(ASCIIString, 0)
      for p in package_map
        push!(packages, p[1])
      end
      unshift!( packages, command )

      Event.new_event( event_system, "networkOut", cid, "output-package", packages )
    catch
      Event.new_event( event_system, "networkOut", cid, "output-package", "failed" )
    end

  elseif isequal(command, "add")
    try
      for pkg in data
        Pkg.add(pkg)
      end

      Event.new_event(event_system, "networkOut", cid, "output-package", "add")
    catch err

      Event.new_event(event_system, "networkOut", cid, "output-package", ["add", "failed"])
    end

  elseif isequal(command, "remove")
    try
      for pkg in data
        Pkg.rm(pkg)
      end

      Event.new_event(event_system, "networkOut", cid, "output-package", "remove")
    catch err
      Event.new_event(event_system, "networkOut", cid, "output-package", ["remove", "failed"])
    end

  elseif isequal(command, "update")

    try
      Pkg.update()

      Event.new_event(event_system, "networkOut", cid, "output-package", "update")
    catch err
      Event.new_event(event_system, "networkOut", cid, "output-package", ["update", "failed"])
    end
  end
end

### Juliet interface ###########################
function load(core::JulietCore)
  return ConsoleLogicSystem()
end

function handle_input( console::ConsoleLogicSystem, cid, msgtype, data )
  if msgtype == "eval"
    on_eval_msg( console, cid, data[1] )
  elseif msgtype == "dir"
    on_dir_msg( console, cid, data[1] )
  elseif msgtype == "package"
    on_pkg_msg( console, cid, data )
  end
end

function initialize(console::ConsoleLogicSystem, core::JulietCore)
  event_system = get_system(Event.EventSystem)

  Event.register_handler( event_system, "networkIn", ( cid, msgtype, data ) -> handle_input( console, cid, msgtype, data ) )
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
