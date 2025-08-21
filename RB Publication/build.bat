@echo off
echo Building RB Publication...

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Navigate to build directory
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake ..

REM Build the application
echo Building application...
cmake --build . --config Release

REM Check if build was successful
if exist "rb_publication.exe" (
    echo.
    echo Build successful!
    echo.
    echo To run the application:
    echo   cd build
    echo   rb_publication.exe
    echo.
    echo Website will be available at: http://localhost:8080
    echo Admin panel: http://localhost:8080/admin
    echo.
    echo Default admin credentials:
    echo   Username: admin
    echo   Password: rbpublication123
    echo.
    pause
) else (
    echo.
    echo Build failed! Please check the error messages above.
    echo.
    echo Common solutions:
    echo   1. Make sure you have CMake installed
    echo   2. Make sure you have a C++ compiler installed
    echo   3. Make sure Crow framework is installed
    echo   4. Make sure SQLite3 development libraries are installed
    echo.
    pause
)
