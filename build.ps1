# Ensure we always run relative to the script's own folder
Set-Location $PSScriptRoot

# Kill any lingering instance of main.exe so the linker isn't blocked
Stop-Process -Name "main" -Force -ErrorAction SilentlyContinue

$BUILD_DIR = "build"

if (-not (Test-Path $BUILD_DIR)) {
    New-Item -ItemType Directory -Path $BUILD_DIR | Out-Null
}

cd $BUILD_DIR

cmake ..
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    cd ..
    exit $LASTEXITCODE
}

cmake --build .
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!" -ForegroundColor Red
    cd ..
    exit $LASTEXITCODE
}

cd ..

if (Test-Path .\build\main.exe) {
    Write-Host "Running application..." -ForegroundColor Yellow
    .\build\main.exe
} elseif (Test-Path .\build\Debug\main.exe) {
    Write-Host "Running application..." -ForegroundColor Yellow
    .\build\Debug\main.exe
} else {
    Write-Host "Could not find main.exe to run!" -ForegroundColor Red
}

Write-Host "Build finished successfully!" -ForegroundColor Green