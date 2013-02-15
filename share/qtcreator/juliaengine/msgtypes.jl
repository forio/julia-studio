
module __MsgTypes

global MSGTYPEBYID = Array(ASCIIString, 256)
global MSGIDBYTYPE = Dict{ASCIIString, Uint8}()

GetMsgType(id) = MSGTYPEBYID[id]
GetMsgId(name) = MSGIDBYTYPE[name]

function RegisterMsg(name::ASCIIString, id)
  MSGTYPEBYID[id] = name
  MSGIDBYTYPE[name] = id
end

function UnregisterMsg(name::ASCIIString)
  id = MSGIDBYTYPE[name]
  MSGNAMEBYID[name] = ""
  MSGIDBYTYPE[id] = 0
end


# These values MUST stay in sync with the c++ side
#RegisterMsg("null", 0)
RegisterMsg("eval", 1)
RegisterMsg("package", 2)
RegisterMsg("dir", 3)

RegisterMsg("output-error", 128)
RegisterMsg("output-eval", 129)
RegisterMsg("output-package", 130)
RegisterMsg("output-dir", 131)
RegisterMsg("output-plot", 132)

end
