@echo off

set mapname=%1

rem =================================================>
rem Update Configs Here

rem example : 
rem set AIBOOTCAMP2_ENGINE_DIR=D:\AIBootcamp2\AIBootcamp2\Releases
rem set AIBOT_PATH=D:\AIBootcamp2\LesOutardes\build-solution-x64\Debug\lesoutardes.dll
rem set TEAM=LesOutardes

set AIBOOTCAMP2_ENGINE_DIR=D:\AI\AIBootcamp2-2021\Releases
set AIBOT_PATH=D:\AI\AIBootcamp\build-solution-x64\Debug\myfirstaibot.dll
set TEAM=AlexEtLuciano

rem in s, -1 = default
set ConnectToProcessDelay=-1

rem in ms, -1 = default
set InitTime=-1
set TurnTime=-1

rem <=================================================

python PlayLocalMatch.py %mapname% %AIBOOTCAMP2_ENGINE_DIR% %AIBOT_PATH% %TEAM% %ConnectToProcessDelay% %InitTime% %TurnTime%

exit /b