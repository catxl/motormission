@echo off
setlocal enabledelayedexpansion
set filename=""
for %%f in (*.json) do (
  set filename=%%~nf
)
python trans.py %filename%
copy /Y "mission_"%filename%".c" ..\..\mission\
pause
