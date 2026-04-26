@echo off
setlocal EnableExtensions
if not defined BUILD_DIR set BUILD_DIR=build_msvc_ninja_arm64
if not defined NC_BUILD_TYPE set NC_BUILD_TYPE=Release
if not defined NC_CRYPTO_BACKEND set NC_CRYPTO_BACKEND=AUTO
if not defined NC_BUILD_SHARED set NC_BUILD_SHARED=ON
if not defined NC_BUILD_STATIC set NC_BUILD_STATIC=ON
if not defined NC_BUILD_GUI set NC_BUILD_GUI=ON
if not defined NC_ARM64_ASM_ARCH set NC_ARM64_ASM_ARCH=armv8-a+crypto
set NC_TARGET_ARCH=arm64
set NC_CMAKE_PROCESSOR=ARM64

call :detect_host_arch
set NC_IS_CROSS=0
if /I not "%NC_HOST_ARCH%"=="%NC_TARGET_ARCH%" set NC_IS_CROSS=1
if /I "%NC_HOST_ARCH%"=="arm64" (
  set NC_VCVARS_ARG=arm64
) else (
  set NC_VCVARS_ARG=amd64_arm64
)

echo NanoChronometer build target: os=windows arch=%NC_TARGET_ARCH% host_os=windows host_arch=%NC_HOST_ARCH% cross=%NC_IS_CROSS% toolchain=msvc

call :prepare_build_dir

if not defined VSCMD_VER call :load_vcvars "%NC_VCVARS_ARG%"
if not defined VSCMD_VER (
  echo error: Visual Studio build environment was not loaded. 1>&2
  exit /b 1
)

cmake -S . -B "%BUILD_DIR%" ^
  -G Ninja ^
  -DCMAKE_BUILD_TYPE=%NC_BUILD_TYPE% ^
  -DCMAKE_SYSTEM_NAME=Windows ^
  -DCMAKE_SYSTEM_PROCESSOR=%NC_CMAKE_PROCESSOR% ^
  -DNC_TARGET_ARCH=arm64 ^
  -DNC_TARGET_OS=windows ^
  -DNC_ARM64_ASM_ARCH=%NC_ARM64_ASM_ARCH% ^
  -DNC_CRYPTO_BACKEND=%NC_CRYPTO_BACKEND% ^
  -DNC_BUILD_SHARED=%NC_BUILD_SHARED% ^
  -DNC_BUILD_STATIC=%NC_BUILD_STATIC% ^
  -DNC_BUILD_GUI=%NC_BUILD_GUI%
if errorlevel 1 exit /b 1
call :write_build_stamp
cmake --build "%BUILD_DIR%" --config %NC_BUILD_TYPE%
if errorlevel 1 exit /b %errorlevel%
call :print_outputs
exit /b 0

:print_outputs
echo.
echo outputs:
if /I "%NC_BUILD_STATIC%"=="ON" (
  echo   %BUILD_DIR%\static\include\nanochrono.h
  echo   %BUILD_DIR%\static\lib\nanochrono.lib
  set NC_EXE_PACKAGE=static
) else (
  set NC_EXE_PACKAGE=dynamic
)
if /I "%NC_BUILD_SHARED%"=="ON" (
  echo   %BUILD_DIR%\dynamic\include\nanochrono.h
  echo   %BUILD_DIR%\dynamic\lib\nanochrono.dll
  echo   %BUILD_DIR%\dynamic\lib\nanochrono.lib
)
echo   %BUILD_DIR%\%NC_EXE_PACKAGE%\bin\nanochrono_cli.exe
if /I "%NC_BUILD_GUI%"=="ON" echo   %BUILD_DIR%\%NC_EXE_PACKAGE%\bin\nanochrono_gui.exe
exit /b 0

:detect_host_arch
set NC_RAW_ARCH=%PROCESSOR_ARCHITEW6432%
if not defined NC_RAW_ARCH set NC_RAW_ARCH=%PROCESSOR_ARCHITECTURE%
if /I "%NC_RAW_ARCH%"=="AMD64" set NC_HOST_ARCH=x64
if /I "%NC_RAW_ARCH%"=="ARM64" set NC_HOST_ARCH=arm64
if not defined NC_HOST_ARCH set NC_HOST_ARCH=%NC_RAW_ARCH%
exit /b 0

:load_vcvars
set NC_VCVARS_ARG=%~1
for %%E in (Community BuildTools Professional Enterprise) do (
  if exist "%ProgramFiles%\Microsoft Visual Studio\2022\%%E\VC\Auxiliary\Build\vcvarsall.bat" call "%ProgramFiles%\Microsoft Visual Studio\2022\%%E\VC\Auxiliary\Build\vcvarsall.bat" %NC_VCVARS_ARG%
  if defined VSCMD_VER exit /b 0
)
exit /b 1

:target_signature
> "%~1" (
  echo target_os=windows
  echo target_arch=%NC_TARGET_ARCH%
  echo host_os=windows
  echo host_arch=%NC_HOST_ARCH%
  echo cross=%NC_IS_CROSS%
  echo toolchain=msvc
  echo vcvars=%NC_VCVARS_ARG%
  echo cmake_processor=%NC_CMAKE_PROCESSOR%
  echo output_layout=static_dynamic_v2
  echo arm64_asm_arch=%NC_ARM64_ASM_ARCH%
)
exit /b 0

:prepare_build_dir
set NC_STAMP=%BUILD_DIR%\.nanochronometer-target
set NC_TMP_STAMP=%BUILD_DIR%\.nanochronometer-target.new
if "%NC_CLEAN%"=="1" (
  if exist "%BUILD_DIR%" echo Cleaning %BUILD_DIR% because NC_CLEAN=1
  if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
  exit /b 0
)
if exist "%BUILD_DIR%\CMakeCache.txt" if not exist "%NC_STAMP%" (
  echo Cleaning %BUILD_DIR% because it was created before target stamping; this avoids stale x64/arm64 objects.
  rmdir /s /q "%BUILD_DIR%"
  exit /b 0
)
if exist "%NC_STAMP%" (
  if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
  call :target_signature "%NC_TMP_STAMP%"
  fc /b "%NC_STAMP%" "%NC_TMP_STAMP%" >nul 2>nul
  if errorlevel 1 (
    echo Cleaning %BUILD_DIR% because the target/compiler signature changed.
    del /q "%NC_TMP_STAMP%" >nul 2>nul
    rmdir /s /q "%BUILD_DIR%"
  ) else (
    del /q "%NC_TMP_STAMP%" >nul 2>nul
  )
)
exit /b 0

:write_build_stamp
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
call :target_signature "%BUILD_DIR%\.nanochronometer-target"
exit /b 0
