@echo off
setlocal

set "QT_BIN=C:\Qt\6.8.3\mingw_64\bin"
set "EXE=%~dp0build\OvercookedQt.exe"

if not exist "%EXE%" (
    echo [ERROR] Cannot find "%EXE%"
    exit /b 1
)

if not exist "%QT_BIN%\windeployqt.exe" (
    echo [ERROR] Cannot find "%QT_BIN%\windeployqt.exe"
    exit /b 1
)

"%QT_BIN%\windeployqt.exe" "%EXE%"

if errorlevel 1 (
    echo [ERROR] windeployqt failed.
    exit /b 1
)

echo [OK] Qt runtime deployed to build folder.
endlocal
