@echo off

setlocal

set CC=cl

set CFLAGS=/W2 /Zi /nologo /Femycraft.exe
set CFLAGS=%CFLAGS% /Iw:\mycraft\libs
set LINKFLAGS=/link opengl32 gdi32 user32 kernel32
set CFILES=

pushd ".\code"
for /R %%c in (*.cpp) do call set CFILES=%%c %%CFILES%%
popd

pushd ".\libs\glad"
for /R %%c in (*.c) do call set CFILES=%%c %%CFILES%%
popd

if not exist "bin" mkdir bin

pushd "bin"
%CC% %CFLAGS% %CFILES%
popd

endlocal
