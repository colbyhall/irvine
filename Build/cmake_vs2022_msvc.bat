@echo off
cmake -S . -B VS2022_MSVC -G "Visual Studio 17 2022" -A x64 %*
echo Open VS2022_MSVC\Irvine.sln to build the project.


