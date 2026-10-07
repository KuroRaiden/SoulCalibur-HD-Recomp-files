@echo off
setlocal
if not defined SDL_AUDIO_DRIVER set SDL_AUDIO_DRIVER=directsound
cd /d "%~dp0..\out\build\reference"
if not exist soulcalibur_hd.exe exit /b 1
soulcalibur_hd.exe
