@ECHO Off
SET EMULATOR_DIR=..\..\Emulators
SET COMPILER_DIR=..\..\Compiler
SET JO_ENGINE_SRC_DIR=..\..\jo_engine

if "%1" == "findSources" GOTO find
IF "%1" == "run" GOTO run
IF "%1" == "cleanOnly" GOTO clean
IF NOT EXIST == "game.iso" GOTO compile

:clean
del /Q /F /S cd\0.bin > NUL
del /Q /F /S *.o > NUL
del /Q /F /S %JO_ENGINE_SRC_DIR%\*.o > NUL
del /Q /F /S *.bin > NUL
del /Q /F /S *.coff > NUL
del /Q /F /S *.elf > NUL
del /Q /F /S *.map > NUL
del /Q /F /S *.iso > NUL
del /Q /F /S *.cue > NUL
IF "%1" == "cleanOnly" GOTO end

:compile
SET PATH=%COMPILER_DIR%\WINDOWS\Other Utilities;%COMPILER_DIR%\WINDOWS\bin;%PATH%
"%COMPILER_DIR%\WINDOWS\Other Utilities\make" re
IF NOT "%1" == "alsoRun" GOTO end

:run
if "%2%" == "mednafen" GOTO mednafen
IF EXIST game.iso "%EMULATOR_DIR%\yabause\yabause.exe" -a -i game.iso
IF NOT EXIST game.iso GOTO closeError
GOTO end

:mednafen
IF EXIST game.cue "%EMULATOR_DIR%\mednafen\mednafen.exe" game.cue
IF NOT EXIST game.cue GOTO closeError
GOTO end

:closeError
ECHO "Task Fail"
GOTO end

:find
setlocal enabledelayedexpansion
SET _root=%CD%\
SET _sources=
FOR /f "tokens=*" %%G IN ('dir /s /b *.c ^| find "c" ^| find /V "Directory of"') DO (
	CALL SET _line=%%G
	CALL SET _result=%%_line:%_root%=%%
	SET _sources=!_sources! !_result!
)
echo !_sources!
setlocal disabledelayedexpansion
GOTO end

:close
ECHO "Task done"

:end