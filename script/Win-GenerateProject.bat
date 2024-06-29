@echo off
pushd ..\
call vendor\premake\bin0\premake5.exe vs2022
popd
PAUSE