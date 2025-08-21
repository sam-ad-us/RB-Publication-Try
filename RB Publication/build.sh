#!/bin/bash

echo "Building RB Publication..."

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

# Build the application
echo "Building application..."
make

# Check if build was successful
if [ -f "rb_publication" ]; then
    echo ""
    echo "Build successful!"
    echo ""
    echo "To run the application:"
    echo "  cd build"
    echo "  ./rb_publication"
    echo ""
    echo "Website will be available at: http://localhost:8080"
    echo "Admin panel: http://localhost:8080/admin"
    echo ""
    echo "Default admin credentials:"
    echo "  Username: admin"
    echo "  Password: rbpublication123"
    echo ""
    read -p "Press Enter to continue..."
else
    echo ""
    echo "Build failed! Please check the error messages above."
    echo ""
    echo "Common solutions:"
    echo "  1. Make sure you have CMake installed"
    echo "  2. Make sure you have a C++ compiler installed"
    echo "  3. Make sure Crow framework is installed"
    echo "  4. Make sure SQLite3 development libraries are installed"
    echo ""
    read -p "Press Enter to continue..."
fi
