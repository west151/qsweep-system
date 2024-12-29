[string]$FFTW_SRC = "src\external\fftw\3.3.7"
[string]$BUILD_FOLDER = "build-fftw"
[string]$FFTW_INSTALL_PREFIX = "build/external/fftw/3.3.7/shared/mingw81_64"
[string]$CMAKE_BIN = "C:\Qt\Tools\CMake_64\bin\cmake.exe"

[System.Console]::OutputEncoding = [System.Console]::InputEncoding = [System.Text.Encoding]::UTF8

if ( Test-Path ${FFTW_INSTALL_PREFIX} ) { Remove-Item ${FFTW_INSTALL_PREFIX} -Recurse }
mkdir ${FFTW_INSTALL_PREFIX}

if ( Test-Path ${BUILD_FOLDER} ) { Remove-Item ${BUILD_FOLDER} -Recurse }
mkdir ${BUILD_FOLDER}

cd ${BUILD_FOLDER}

$env:CC="C:\Qt\Tools\mingw1310_64\bin\gcc.exe"
$env:CXX="C:\Qt\Tools\mingw1310_64\bin\g++.exe"
$env:PATH="C:\Qt\6.8.0\mingw1310_64\bin;E:\Qt\Tools\mingw1310_64\bin"
$env:CMAKE_PREFIX_PATH="C:\Qt\6.8.0\mingw_64"

#Get-ChildItem Env:
(gci env:*).GetEnumerator() | Sort-Object Name | Out-String

Write-Host
Write-Host "cmake fftw ..."

& ${CMAKE_BIN} `
 ..\${FFTW_SRC} `
 -G "MinGW Makefiles" `
 -D CMAKE_MAKE_PROGRAM="C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe" `
 -D CMAKE_INSTALL_PREFIX="../${FFTW_INSTALL_PREFIX}" `
 -D CMAKE_BUILD_TYPE=Release `
 -D BUILD_SHARED_LIBS=ON `
 -D ENABLE_FLOAT=OFF `
 -D ENABLE_SSE=OFF `
 -D ENABLE_SSE2=OFF

# -D CMAKE_GENERATOR_PLATFORM=x64 `
# -D CMAKE_MAKE_PROGRAM:PATH="E:\Qt\Tools\mingw810_64\bin\mingw32-make.exe" `

Write-Host
Write-Host "build fftw ..."

& ${CMAKE_BIN} `
 --build . `
 --target install `
 --config Release `
 --target all -- -j $(Get-WmiObject -class Win32_ComputerSystem).numberoflogicalprocessors

cd ..
