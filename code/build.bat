@echo off

ctime -begin build\sinus.ctm

REM COMMON COMPILER FLAGS
REM -Od optimizer debug (-O2 fast code for release)
REM -MTd package debug C runtime library into the executable (lose the d for release)
REM -nologo turn off compiler logo
REM -fp:fast optimize floating-point code for speed at the expense of accuracy and correctness
REM -fp:except- disable reliable floating-point exception model
REM -Gm- enables minimal rebuild
REM -GR- disable runtime type info
REM -EHa- turn off exception handling
REM -Zo generates enhanced debugging information for optimized code
REM -Oi allow compiler intrinsics
REM -WX treat warnings as errors
REM -W4 warning level 4
REM -FC display full path of source code files passed to cl.exe in diagnostic text
REM -Z7 generates C 7.0-compatible debugging information
REM -wd4201 unnamed structs
REM -wd4100 unused parameter
REM -wd4189 unused variable
REM -wd4505 unreferenced local function
REM -wd4127 conditional expression is constant
REM -Fm generate symbol map file

REM COMMON LINKER FLAGS
REM -incremental:no turn off incremental build
REM -opt:ref don't put unused things in the executable
REM -subsystem:windows,5.1 disable console

set CommonCompilerFlags=-Od -MTd -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -FC -Z7
set CommonCompilerFlags=-wd4201 -wd4100 -wd4189 -wd4505 -wd4127 %CommonCompilerFlags%
set CommonCompilerFlags=-D_CRT_SECURE_NO_WARNINGS -DSINUS_DEBUG=1 -DSINUS_RELEASE=1 -DSINUS_WIN32=1 %CommonCompilerFlags%
set CommonLinkerFlags=-incremental:no -opt:ref user32.lib gdi32.lib opengl32.lib

IF NOT EXIST build mkdir build
pushd build

del *.pdb > NUL 2> NUL

REM 64-bit builds

REM game build
echo WAITING FOR PDB > lock.tmp
cl %CommonCompilerFlags% ..\code\game\game.cpp -Fmgame.map -LD /link -incremental:no -opt:ref -PDB:game_%random%.pdb -EXPORT:GameUpdateAndRender
del lock.tmp

REM platform build
cl %CommonCompilerFlags% ..\code\platform\windows\main.cpp -Fmmain.map /link %CommonLinkerFlags%

REM asset processor build
cl %CommonCompilerFlags% ..\code\tools\assetprocessor.cpp -Fmassetprocessor.map /link %CommonLinkerFlags%

SET /A errno=%ERRORLEVEL%
popd
ctime -end build\sinus.ctm %errno%
EXIT /B %errno%
