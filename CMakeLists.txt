# Requires cmake v3.15 for MSVC_RUNTIME_LIBRARY
cmake_minimum_required(VERSION 3.15)
project(pdbrn LANGUAGES CXX)

# Enforce C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Create the target and add source files
add_executable(${PROJECT_NAME}
  src/main.cpp
)

# Setup static linking C++ runtime for GCC and MSVC runtime
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  # Static linking of libgcc and libstdc++ is not enough because some Linux distro like Alpine
  #   ships with an older version of libc, which cannot opt-in for static linking individually.
  target_link_options(${PROJECT_NAME} PRIVATE
    -static
  )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  set_property(TARGET ${PROJECT_NAME} PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
  )
endif()

# Enable cmake --install to copy the binary to system dir
install(
  TARGETS ${PROJECT_NAME}
)