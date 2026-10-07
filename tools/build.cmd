@echo off
setlocal
cd /d "%~dp0.."
cmake -S . -B out/build/reference -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang-cl -DREXSDK_VERSION=0.9.0.1
if errorlevel 1 exit /b 1
cmake --build out/build/reference --parallel
