@echo off
cmake -S . -B vs22_msvc -G "Visual Studio 17 2022" -A x64 %*
echo Open vs22_msvc\irvine.sln to build the project.


