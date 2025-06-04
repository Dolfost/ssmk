ECHO OFF

IF [%1] == [] ( 
	ECHO No operation specified!
	EXIT /B 1
)

REM install chocolatey
@"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"

REM figure out dependencies
choco install msys2 


REM set up env command
SET "MSYS_ENV=C:\tools\msys64\usr\bin\env.exe MSYSTEM=UCRT64 CHERE_INVOKING=1 /usr/bin/bash -lc"

ECHO -cmd- installing tools

%MSYS_ENV% "pacman -S --noconfirm mingw-w64-ucrt-x86_64-libpng mingw-w64-ucrt-x86_64-zlib mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake git make"

ECHO -cmd- done installing tools

SET "REPO=%cd%"

ECHO -cmd- running the cicd/cmake/%1%.cmake in msys
%MSYS_ENV% "cmake -P cicd/cmake/%1%.cmake"
