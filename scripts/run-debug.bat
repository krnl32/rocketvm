@echo off
set EXE_NAME=rocketvm.exe
pushd %~dp0\..\build\rocketvm\Debug
if exist %EXE_NAME% (
    %EXE_NAME%
) else (
    echo Debug executable "%EXE_NAME%" not found!
)
popd
