@echo off
set EXE_NAME=rocketvm.exe
pushd %~dp0\..\build\rocketvm\Release
if exist %EXE_NAME% (
    %EXE_NAME%
) else (
    echo Release executable "%EXE_NAME%" not found!
)
popd
