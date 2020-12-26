@ECHO Off
SET EMULATOR_DIR=..\..\Emulators
SET COMPILER_DIR=..\..\Compiler
SET JO_ENGINE_SRC_DIR=..\..\jo_engine

IF "%1" == "run" GOTO run
IF "%1" == "cleanOnly" GOTO clean
IF NOT EXIST == "game.iso" GOTO compile

:clean
del /Q /F /S cd\0.bin
del /Q /F /S *.o
del /Q /F /S %JO_ENGINE_SRC_DIR%\*.o
del /Q /F /S *.bin
del /Q /F /S *.coff
del /Q /F /S *.elf
del /Q /F /S *.map
del /Q /F /S *.iso
del /Q /F /S *.cue
IF "%1" == "cleanOnly" GOTO end

:compile
SET PATH=%COMPILER_DIR%\WINDOWS\Other Utilities;%COMPILER_DIR%\WINDOWS\bin;%PATH%
"%COMPILER_DIR%\WINDOWS\Other Utilities\make" re
IF NOT "%1" == "alsoRun" GOTO end

:run
IF EXIST game.iso "%EMULATOR_DIR%\yabause\yabause.exe" -a -i game.iso
IF NOT EXIST game.iso GOTO closeError
GOTO end

:closeError
ECHO "Task Fail"
GOTO end

:close
ECHO "Task done"

:end