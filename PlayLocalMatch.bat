@echo off

set mapname=%1

rem =================================================>
rem Update Configs Here

set AIBOOTCAMP2_ENGINE_DIR=D:\AIBootcamp2\AIBootcamp2\Releases
set AIBOT_PATH=D:\AIBootcamp2\LesOutardes\build-solution-x64\Debug\lesoutardes.dll
set TEAM=LesOutardes

rem in s, -1 = default
set ConnectToProcessDelay=-1

rem in ms, -1 = default
set InitTime=-1
set TurnTime=-1

rem <=================================================

set hour=%time:~0,2%
if "%hour:~0,1%" == " " set hour=0%hour:~1,1%
set min=%time:~3,2%
if "%min:~0,1%" == " " set min=0%min:~1,1%
set secs=%time:~6,2%
if "%secs:~0,1%" == " " set secs=0%secs:~1,1%
set year=%date:~10,4%
set month=%date:~4,2%
set day=%date:~7,2%

set loguid=%year%%month%%day%_%hour%%min%%secs%

set replayfile=%loguid%



echo.
echo You are about to play a local match with the following parameters:
echo.
echo    aibootcamp2path       = [%AIBOOTCAMP2_ENGINE_DIR%\AIBootCamp2.exe]
echo    aibotpath             = [%AIBOT_PATH%]
echo    mapname               = [%mapname%]
echo    team                  = [%team%]
echo    replayfile            = [%replayfile%]
echo    ConnectToProcessDelay = [%ConnectToProcessDelay%]
echo    InitTime              = [%InitTime%]
echo    TurnTime              = [%TurnTime%]
echo.

pause



"%AIBOOTCAMP2_ENGINE_DIR%/AIBootCamp2.exe" -dllpath %AIBOT_PATH% -mode match -scene %mapname% -team %team% -replayfile %replayfile% -connecttoprocessdelay %ConnectToProcessDelay% -initdelay %InitTime% -turndelay %TurnTime% -quit -batchmode

echo.

if %errorlevel% EQU 0 (
  	echo ==^> Match Completed : Victory!
  	exit /b
)

if %errorlevel% EQU -1 (
  	echo ==^> Match Completed : Failure

  	python RepairReplayLog.py .\Replays\%loguid%\%loguid%.replay
  	exit /b
)

if %errorlevel% EQU -2 (
	echo ==^> Invalid Map Name [%mapname%]
	exit /b
)

exit /b