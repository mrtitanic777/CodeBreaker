@echo off
rem build.cmd -- remake the .NET lane's test inputs and goldens (needs the .NET 9 SDK; the tests themselves do not).
rem
rem     src\dotnet\testdata\build.cmd
rem
rem Builds inputs\Sample twice (Sample.dll: AnyCPU, PE32; Sample64.dll: x64, PE32+), the oracle oracle\MetadataDump (.NET's own
rem System.Reflection.Metadata), and writes golden\<input>.metadata.txt from it. The builds happen in work\ilspy-port\testdata-build.
setlocal
set HERE=%~dp0
set W=%HERE%..\..\..\work\ilspy-port\testdata-build
if exist "%W%" rmdir /s /q "%W%"
mkdir "%W%\in32" "%W%\in64" "%W%\oracle" || exit /b 1
copy /y "%HERE%inputs\Sample\*" "%W%\in32\" >nul && copy /y "%HERE%inputs\Sample\*" "%W%\in64\" >nul || exit /b 1
copy /y "%HERE%oracle\MetadataDump\*" "%W%\oracle\" >nul || exit /b 1
dotnet build "%W%\in32\Sample.csproj" -c Release -nologo -v q -o "%W%\in32\out" || exit /b 1
dotnet build "%W%\in64\Sample.csproj" -c Release -nologo -v q -p:Platform=x64 -p:PlatformTarget=x64 -o "%W%\in64\out" || exit /b 1
dotnet build "%W%\oracle\MetadataDump.csproj" -c Release -nologo -v q -o "%W%\oracle\out" || exit /b 1
copy /y "%W%\in32\out\Sample.dll" "%HERE%inputs\Sample.dll" >nul || exit /b 1
copy /y "%W%\in64\out\Sample.dll" "%HERE%inputs\Sample64.dll" >nul || exit /b 1
if not exist "%HERE%golden" mkdir "%HERE%golden"
for %%f in (Sample Sample64) do "%W%\oracle\out\MetadataDump.exe" "%HERE%inputs\%%f.dll" > "%HERE%golden\%%f.metadata.txt" || exit /b 1
rmdir /s /q "%W%"
echo testdata rebuilt
