require("sandbox.jl")
require("event.jl")
require("msgtypes.jl")

module __Network

import __Sandbox
import __Event
import __MsgTypes

####
type Message
  msg_type::Uint8
  params::Array{Any, 1}

  function Message(msg_type, params...)
    return new(msg_type, [params...])
  end

  function Message(io)
    msg_type = Base.read(io, Uint8)
    num_params = Base.read(io, Uint8)

    params = Array(Any, 0)
    param_length
    param
    for i = 1 : num_params
      param_length = uint32(read(io, Uint32))
      param = ASCIIString(read(io, Uint8, param_length))

      #if endswith(param, '\0')
        param = chop(param)
      #end

      push!(params, param)
    end

    return Message(msg_type, params)
  end
end

####
type NetworkManager <: __Sandbox.System
  port::Int16
  socket
  io
end

NetworkManager() = NetworkManager(int16(0), Nothing, Nothing)


### Utility ###########################
function ReadMessage(io)
  return Message(io)
end

const __io_out_buff = PipeString()
function WriteMessage(io, msg::Message)
  total_size = sizeof(Uint8) * 2
  for param in msg.params
    write(__io_out_buff, param)
    param_str = takebuf_array(__io_out_buff)
    total_size += sizeof(Uint32)
    total_size += length(param_str)
  end

  write(io, uint32(total_size))
  write(io, uint8(msg.msg_type))
  write(io, uint8(length(msg.params)))

  for param in msg.params
    write(__io_out_buff, param)
    param_str = takebuf_array(__io_out_buff)
    write(io, uint32(length(param_str)))
    write(io, param_str)
  end
end

function ConnectionCallback(connection, manager)
  msg = Message(manager.io)
  __Event.NewEvent(__Sandbox.GetSystem(__Event.EventSystem), __MsgTypes.GetMsgType(msg.msg_type), msg.params...)
end

function EngineCallback(manager, msg_type, params...)
  msg = Message(__MsgTypes.GetMsgId(msg_type), params...)
  WriteMessage(manager.io, msg)
end

function Connect(manager::NetworkManager, port::Int16)
  #portbuff = [port]
  #manager.socket = ccall(:open_any_tcp_port, Int32, (Ptr{Int16},), portbuff)
  #if manager.socket == -1
  #  error("Could not open socket on any port")
  #end

  #manager.port = portbuff[1]
  #println("PORT:", manager.port)

  #manager.connection = ccall(:accept, Int32, (Int32, Ptr{Void}, Ptr{Void}), manager.socket, C_NULL, C_NULL)
  #manager.io = fdio(manager.connection)

  (manager.port, manager.socket) = Base.open_any_tcp_port(port, false)

  println(STDOUT,"PORT:$(manager.port)")

  manager.io = Base.wait_accept(manager.socket)
  Base.start_reading(manager.io)2

  enq_work(@task while true
                   ConnectionCallback(nothing, manager)
                 end)
  #add_fd_handler(manager.connection, (connection)->ConnectionCallback(connection, manager))

  return manager
end


### Sandbox interface ###########################
function Load(core::__Sandbox.SandboxCore)
  return NetworkManager()
end

function Init(manager::NetworkManager, core::__Sandbox.SandboxCore)
  event_system = __Sandbox.GetSystem(__Event.EventSystem)
  __Event.RegisterHandler(event_system, "network-output", (info...)->EngineCallback(manager, info...))

  Connect(manager, int16(1984))
end

function Update(manager::NetworkManager)
  #println("NetworkManager looks with distain at the requirement that she have an update method.")
end


end
