@echo off

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --file=premake.lua vs2022
popd
pause