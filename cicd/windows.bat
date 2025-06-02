ECHO OFF

IF [%1] == [] ECHO "No operation specified!" & EXIT 1

REM install chocolatey
@"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"

REM figure out dependencies
choco install msys2 

ECHO UCRT_1

set CHERE_INVOKING=yes
SET MSYSTEM=UCRT64

C:\tools\msys64\usr\bin\bash -lc "pacman -Syu --noconfirm"
C:\tools\msys64\usr\bin\bash -lc "pacman -S --noconfirm mingw-w64-ucrt-x86_64-libpng mingw-w64-ucrt-x86_64-zlib mingw-w64-ucrt-x86_64-gcc cmake git make"

ECHO UCRT_2

SET "SCRIPT_DIR=%~dp0"
SET "REPO=%SCRIPT_DIR%.."
REM SET CONFIGURATION_OPTIONS=-G "Unix Makefiles"

C:\tools\msys64\usr\bin\bash -lc 'cmake -P "%SCRIPT_DIR%cmake\%1.cmake"'
