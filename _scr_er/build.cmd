@echo off
set WT=C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-a3bf74e3d9d7d7547
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
cd /d C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\exact.readable
cl /nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /I %WT%\native\ported\include /I %WT%\native\typed\cpp /I %WT%\native\l2\src test.cpp %WT%\native\typed\cpp\codebreaker\exact\readable.cpp %WT%\native\l2\src\pyre.cc %WT%\native\l2\src\pystr.cc %WT%\native\l2\src\value.cc /Fe:test.exe
