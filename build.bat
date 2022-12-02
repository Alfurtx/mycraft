@echo off

setlocal

set CC=cl
set CFLAGS=/W3 /Zi /nologo /Femycraft.exe /MD /MP4 -DDEBUG /EHsc /wd4244
set CFLAGS=%CFLAGS% /Iw:\mycraft\libs /Iw:\mycraft\libs\glfw\include /Iw:\mycraft\libs\glm
set CFLAGS=%CFLAGS% /Femycraft.exe
set LINKFLAGS=glfw3.lib opengl32.lib shell32.lib gdi32.lib user32.lib kernel32.lib /link
set LINKFLAGS=%LINKFLAGS% /LIBPATH:w:\mycraft\libs\glfw\lib-vc2022
set CFILES=

REM Clang setup (for compile_commands.json generation)
set clang_flags=-Iw:\mycraft\libs -Iw:\mycraft\libs\glfw\include -Iw:\mycraft\libs\glm
set clang_flags=%clang_flags% -lglfw3.lib -lopengl32.lib -lshell32.lib -lgdi32.lib -luser32.lib -lkernel32.lib
set clang_flags=%clang_flags% -Lw:\mycraft\libs\glfw\lib-vc2022
set clang_flags=%clang_flags% -MJ ..\compile_commands.json
set clang_flags=%clang_flags% -o mycraft.exe

pushd ".\code"
for /R %%c in (*.cpp) do call set CFILES=%%c %%CFILES%%
popd

pushd ".\libs\glad"
for /R %%c in (*.c) do call set CFILES=%%c %%CFILES%%
popd

if not exist "bin" mkdir bin

pushd "bin"
%CC% %CFLAGS% %CFILES% %LINKFLAGS%
REM clang %clang_flags% %CFILES%
popd

endlocal
