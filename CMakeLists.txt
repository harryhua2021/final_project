cmake_minimum_required(VERSION 3.14)
project(ERTriageMVP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(er-triage
    src/main.cpp
    src/Patient.cpp
)

# Optional: add warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(er-triage PRIVATE -Wall -Wextra -Wpedantic)
endif()
