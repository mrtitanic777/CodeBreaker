@echo off
rem check_old.cmd <worktree root> <source files ...> -- syntax-check old-tree sources against the shims (cl /Zs, C++20)
set W=%~1
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
for /f "tokens=1,* delims= " %%a in ("%*") do set REST=%%b
cl /nologo /Zs /std:c++20 /EHsc /utf-8 /permissive- /bigobj /MT /Zc:__cplusplus /Zc:char8_t- /DNOMINMAX /DWIN32_LEAN_AND_MEAN /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4267 /wd4996 /wd4018 /wd4146 /wd5051 /wd4101 /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /I "%W%" /I "%W%\native" /I "%W%\native\third_party" /I "%W%\native\third_party\include" %REST%
exit /b %errorlevel%
