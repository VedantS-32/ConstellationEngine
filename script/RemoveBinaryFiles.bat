@echo off
pushd ..\
for /d /r . %%d in (bin) do @if exist "%%d" rd /s /q "%%d"
for /d /r . %%d in (bin-int) do @if exist "%%d" rd /s /q "%%d"
popd
PAUSE