@echo off

setlocal

set CC=cl

set CFLAGS=/W2 /Zi /nologo /Femycraft.exe /MD
set CFLAGS=%CFLAGS% /Iw:\mycraft\libs /Iw:\mycraft\libs\glfw\include
set LINKFLAGS=glfw3.lib opengl32.lib shell32.lib gdi32.lib user32.lib kernel32.lib /link
set LINKFLAGS=%LINKFLAGS% /LIBPATH:w:\mycraft\libs\glfw\lib-vc2022
set CFILES=

pushd ".\code"
for /R %%c in (*.cpp) do call set CFILES=%%c %%CFILES%%
popd

pushd ".\libs\glad"
for /R %%c in (*.c) do call set CFILES=%%c %%CFILES%%
popd

if not exist "bin" mkdir bin

pushd "bin"
%CC% %CFLAGS% %CFILES% %LINKFLAGS%
popd

endlocal
