[string]$FFTW_SRC = "src\external\fftw\3.3.7"
[string]$BUILD_FOLDER = "build-fftw"
[string]$FFTW_INSTALL_PREFIX = "build\external\fftw\3.3.7\shared\mingw81_64"
[string]$CMAKE_BIN = "E:\Qt\Tools\CMake_64\bin\cmake.exe"
[string]$CMAKE_ARG = ""

[System.Console]::OutputEncoding = [System.Console]::InputEncoding = [System.Text.Encoding]::UTF8
$env:PATH+="E:\Qt\5.15.2\mingw81_64\bin;E:\Qt\Tools\mingw810_64\bin"

if ( Test-Path ${FFTW_INSTALL_PREFIX} ) { Remove-Item ${FFTW_INSTALL_PREFIX} -Recurse }
mkdir ${FFTW_INSTALL_PREFIX}

if ( Test-Path ${BUILD_FOLDER} ) { Remove-Item ${BUILD_FOLDER} -Recurse }
mkdir ${BUILD_FOLDER}

cd ${BUILD_FOLDER}

Write-Host
Write-Host "cmake fftw ..."

& ${CMAKE_BIN} `
 ..\${FFTW_SRC} `
 -G "MinGW Makefiles" `
 -DCMAKE_INSTALL_PREFIX="../${FFTW_INSTALL_PREFIX}" `
 -DCMAKE_BUILD_TYPE=Release `
 -DBUILD_SHARED_LIBS=ON

Write-Host
Write-Host "build fftw ..."

& ${CMAKE_BIN} `
 --build . `
 --target install `
 --config Release `
 --target all -- -j $(Get-WmiObject -class Win32_ComputerSystem).numberoflogicalprocessors

cd ..
