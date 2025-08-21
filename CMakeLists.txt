cmake_minimum_required(VERSION 3.10)
project(RB_Publication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SQLite3
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

# Include directories
include_directories(${SQLITE3_INCLUDE_DIRS})

# Add executable
add_executable(rb_publication 
    src/main.cpp
    src/admin_panel.cpp
    src/content_manager.cpp
    src/database.cpp
)

# Link libraries
target_link_libraries(rb_publication ${SQLITE3_LIBRARIES})

# Compiler flags
target_compile_options(rb_publication PRIVATE ${SQLITE3_CFLAGS_OTHER})

# For Crow framework, we'll use header-only approach
# Make sure to include Crow headers in your source files
