cd /d "%~dp0"

for %%i in (*.dll) do regsvr32.exe /s  %%i
