@echo off

setlocal

set CC=cl

set CFLAGS=/W2 /Zi /nologo /Femycraft.exe
set CFILES=

pushd ".\code"
for /R %%c in (*.cpp) do call set CFILES=%%c %%CFILES%%
popd

if not exist "bin" mkdir bin

pushd "bin"
%CC% %CFLAGS% %CFILES%
popd

endlocal
