@echo off
cmake --build .\build --config Debug
if errorlevel 1 exit /b %errorlevel%
.\build\Debug\bin\Engine.exe
