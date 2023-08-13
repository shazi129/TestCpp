set workspace=%~dp0..
set proto_exe=%workspace%\External\protobuf\lib\protoc.exe

rem "%proto_exe%" --cpp_out=%~dp0 %~dp0ProtoMessages.proto
"%proto_exe%" --cpp_out=. ProtoMessages.proto