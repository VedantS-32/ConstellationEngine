@echo off
pushd ..\
for /r %%x in (Makefile) do @if exist "%%x" del "%%x"
popd
PAUSE