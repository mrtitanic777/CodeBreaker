@echo off
rem build.cmd -- remake the .NET lane's test inputs and goldens (needs the .NET 9 SDK and ilspycmd 11.0; the tests themselves need neither).
rem
rem     src\dotnet\testdata\build.cmd
rem
rem Builds inputs\Sample twice (Sample.dll: AnyCPU, PE32; Sample64.dll: x64, PE32+); the oracle oracle\MetadataDump (.NET's own
rem System.Reflection.Metadata) writes golden\<input>.metadata.txt; the installed ilspycmd 11.0 writes golden\<input>.il.txt
rem (`ilspycmd -il`); the oracle oracle\NetText writes decompiler\util\char_categories.inc and golden\numbers.txt. The builds happen in
rem work\ilspy-port\testdata-build.
setlocal
set HERE=%~dp0
set W=%HERE%..\..\..\work\ilspy-port\testdata-build
if exist "%W%" rmdir /s /q "%W%"
mkdir "%W%\in32" "%W%\in64" "%W%\oracle" "%W%\nettext" || exit /b 1
copy /y "%HERE%inputs\Sample\*" "%W%\in32\" >nul && copy /y "%HERE%inputs\Sample\*" "%W%\in64\" >nul || exit /b 1
copy /y "%HERE%oracle\MetadataDump\*" "%W%\oracle\" >nul || exit /b 1
copy /y "%HERE%oracle\NetText\*" "%W%\nettext\" >nul || exit /b 1
dotnet build "%W%\in32\Sample.csproj" -c Release -nologo -v q -o "%W%\in32\out" || exit /b 1
dotnet build "%W%\in64\Sample.csproj" -c Release -nologo -v q -p:Platform=x64 -p:PlatformTarget=x64 -o "%W%\in64\out" || exit /b 1
dotnet build "%W%\oracle\MetadataDump.csproj" -c Release -nologo -v q -o "%W%\oracle\out" || exit /b 1
dotnet build "%W%\nettext\NetText.csproj" -c Release -nologo -v q -o "%W%\nettext\out" || exit /b 1
copy /y "%W%\in32\out\Sample.dll" "%HERE%inputs\Sample.dll" >nul || exit /b 1
copy /y "%W%\in64\out\Sample.dll" "%HERE%inputs\Sample64.dll" >nul || exit /b 1
if not exist "%HERE%golden" mkdir "%HERE%golden"
for %%f in (Sample Sample64) do "%W%\oracle\out\MetadataDump.exe" "%HERE%inputs\%%f.dll" > "%HERE%golden\%%f.metadata.txt" || exit /b 1
for %%f in (Sample Sample64) do ilspycmd -il "%HERE%inputs\%%f.dll" > "%HERE%golden\%%f.il.txt" 2>nul || exit /b 1
"%W%\nettext\out\NetText.exe" categories "%HERE%..\decompiler\util\char_categories.inc" || exit /b 1
"%W%\nettext\out\NetText.exe" numbers "%HERE%golden\numbers.txt" || exit /b 1
rmdir /s /q "%W%"
echo testdata rebuilt
