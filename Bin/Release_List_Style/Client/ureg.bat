cd /d "%~dp0"

for %%i in (*.dll) do regsvr32.exe /u /s  %%i

pause>nul