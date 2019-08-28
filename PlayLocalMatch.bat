@echo off

set mapname=%1

rem set your AIBOOTCAMP2 path here
rem example : AIBOOTCAMP2_ENGINE_DIR=D:\AIBootcamp2\AIBootcamp2\Releases
set AIBOOTCAMP2_ENGINE_DIR=

rem set your AIBot path here
rem example : D:\AIBootcamp2\LesOutardes\build-solution-x64\Debug\lesoutardes.dll
set AIBOT_PATH=

echo.
echo You are about to play a local match with the following parameters:
echo.
echo    aibootcamp2path = [%AIBOOTCAMP2_ENGINE_DIR%\AIBootCamp2.exe]
echo    aibotpath       = [%AIBOT_PATH%]
echo    mapname         = [%mapname%]
echo.

pause

rem in milliseconds
set InitTime=-1
set TurnTime=-1

"%AIBOOTCAMP2_ENGINE_DIR%/AIBootCamp2.exe" -dllpath %AIBOT_PATH% -mode match -scene %mapname% -quit -batchmode

echo.

if errorlevel 0 (
    echo ==^> Match Completed : Victory!
) else (
    echo ==^> Match Completed : Failure
)

exit /b