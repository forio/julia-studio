
module Network

include ("../../../../JSON/src/JSON.jl")

importall Base
import Juliet
import Juliet.Event
#import JSON


####
type Message
    msgtype
    data
end

function Message(io::IO)
    return read_message(io)
end


####
type Connection
    id::Uint64
    port::Int16
    sock

    Connection(id, port, sock = nothing) = new(id, port, sock)
end


####
type NetworkManager <: Juliet.System
    connections::Dict{Uint64, Connection}
    prev_id
end

NetworkManager() = NetworkManager(Dict{Uint64, Connection}(), 1)


### Utility ###########################
function read_message(io::IO)
    data = JSON.parse(io)
    return Message(data["msgtype"], data["data"])
end

function write_message(io::IO, msg::Message)
    msg_buff = JSON.json(msg)

    write(io, msg_buff)
end

function next_id(manager)
    manager.prev_id = manager.prev_id + 1
end

function process_incoming(c)
    msg = Message(c.sock)
    Event.new_event(Juliet.get_system(Event.EventSystem), "networkIn", c.id, msg.msgtype, msg.data)
end

function engine_callback(manager::NetworkManager, cid, msgtype, data)
    msg = Message(msgtype, data)
    write_message(manager.connections[cid].sock, msg)
    return true
end

function accept_all_on(manager::NetworkManager, port::Int)
    @async begin
        server = listen(port)

        while true
            println(STDOUT, "PORT:$port")

            c = Connection(next_id(manager), port, accept(server))
            manager.connections[c.id] = c

            @async begin
                c.sock.line_buffered = false
                Base.start_reading(c.sock)

                try
                    while true
                        process_incoming(c)
                    end

                catch err
                    warn("while processing connection: $c, error: $err")
                end
            end
        end
    end
end

function accept_one_on(manager::NetworkManager, port::Int)
    port, server = listenany(port)

    c = Connection(next_id(manager), port)
    manager.connections[c.id] = c

    println(STDOUT, "PORT:$(c.port)")

    @async begin
        c.sock = accept(server)
       	c.sock.line_buffered = false
        Base.start_reading(c.sock)

        while true
            process_incoming(c)
        end
    end

    return c
end


### Juliet interface ###########################
function load(core::Juliet.JulietCore)
    return NetworkManager()
end

function initialize(manager::NetworkManager, core::Juliet.JulietCore)
    event_system = Juliet.get_system(Event)
    Event.register_handler(event_system, "networkOut", (cid, msgtype, data)->engine_callback(manager, cid, msgtype, data))
end

function update(manager::NetworkManager)
    #println("NetworkManager looks with distain at the requirement that she have an update method.")
end

end
