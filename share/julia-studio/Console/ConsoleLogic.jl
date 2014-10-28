
module ConsoleLogic

using Juliet
import Juliet.Event

using REPLCompletions

type ConsoleLogicSystem <: System
end

function showout( io, x )
  writemime( io, MIME"text/plain"(), x )
end

function parse_lines( str::String )
  lines = split( str, "\n")
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
      elseif ex.head === :incomplete
        continue
      end

    end

    curr_str = ""
    push!(parsed_exprs, ex)
  end

  return parsed_exprs
end

function execute_in_session( code )
    try
        parsed_exprs = parse_lines( code )
        r = nothing
        for e in parsed_exprs
            r = eval(Main, e)
        end
        return r

    catch error
        return error
    end
end

function getobjects( Objects )
  objs = Array( ASCIIString, 0 )
  for m in sort( names( Main ) )
    if isdefined( Main, m )
      Md = eval( Main, m )
      if typeof( Md ) == Module && Md != Base && Md != Core && Md != Juliet
        sm = string( m )
        for n in sort( names( Md ) )
          if isdefined( Md, n )
            v = eval( Md, n )
            t = typeof( eval( Md, n ) )
            if t != Module
              s = string( n )
              if ( s != "network_sys" )
                if !Objects && ( t == Function || t == DataType )
                  push!( objs, sm )
                  push!( objs, s )
                  push!( objs, summary( eval( Md, n ) ) )
                  push!( objs, "" )
                elseif Objects && !( t == Function || t == DataType )
                  push!( objs, sm )
                  push!( objs, s )
                  push!( objs, summary( eval( Md, n ) ) )
                  if ( t <: Number || t <: String )
                    push!( objs, repr( v ) )
                  else
                    push!( objs, "" )
                  end
                end
              end
            end
          end
        end
      end
    end
  end
  return objs
end

### Events ###########################
function on_eval_msg(console::ConsoleLogicSystem, cid, code)
  event_system = get_system(Event.EventSystem)

  if length( strip( code ) ) == 0 || lstrip( code )[1] == '#'
    Event.new_event( event_system, "networkOut", cid, "output-eval", "" )
    return
  end
  try
    result = execute_in_session( code )

    if isa( result, Nothing ) || rstrip( code )[end] == ';'
      Event.new_event( event_system, "networkOut", cid, "output-eval", "" )
    else
      Event.new_event( event_system, "networkOut", cid, "output-eval", sprint( showout, result ) )
    end

  catch error
    Event.new_event(event_system, "networkOut", cid, "output-error", string( "ERROR: ", sprint( showerror, error, catch_backtrace() ) ) )
  end
end

function on_eval_silent_msg(console::ConsoleLogicSystem, cid, code)
  event_system = get_system(Event.EventSystem)
  try
    execute_in_session( code )
    Event.new_event( event_system, "networkOut", cid, "output-eval-silent", "" )

  catch error
    Event.new_event(event_system, "networkOut", cid, "output-error", string( "ERROR: ", sprint( showerror, error, catch_backtrace() ) ) )
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

function on_complete_msg( console::ConsoleLogicSystem, cid, prefix )
  event_system = get_system(Event.EventSystem)

  result, range = completions( prefix, length( prefix ) )
  Event.new_event( event_system, "networkOut", cid, "output-complete", [ [ string( range.start ), string( length(range) ) ], result ] )
end

function on_watch_msg( console::ConsoleLogicSystem, cid, command )
  event_system = get_system(Event.EventSystem)

  objs = getobjects( isequal( command, "Objects" ) )
  unshift!( objs, command )
  
  Event.new_event( event_system, "networkOut", cid, "output-watch", objs )
end

### Juliet interface ###########################
function load(core::JulietCore)
  return ConsoleLogicSystem()
end

function handle_input( console::ConsoleLogicSystem, cid, msgtype, data )
  if msgtype == "eval"
    on_eval_msg( console, cid, data[1] )
  elseif msgtype == "eval-silent"
    on_eval_silent_msg( console, cid, data[1] )
  elseif msgtype == "dir"
    on_dir_msg( console, cid, data[1] )
  elseif msgtype == "package"
    on_pkg_msg( console, cid, data )
  elseif msgtype == "complete"
    on_complete_msg( console, cid, data[1] )
  elseif msgtype == "watch"
    on_watch_msg( console, cid, data[1] )
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
