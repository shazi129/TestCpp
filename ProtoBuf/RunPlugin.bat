set workspace=%~dp0..
set proto=%workspace%\External\protobuf\lib\protoc.exe
set plugin_exe=%workspace%\x64\Debug\TestCpp.exe
set msg_path=%workspace%\ProtoBuf

"%proto%" %msg_path%\ProtoMessages.proto -I=%msg_path% --plugin=protoc-gen-TestCpp=%plugin_exe% --TestCpp_out=./