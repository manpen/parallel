

# --------------------------------------------------------------------------------------------------
#   CMake Init
# --------------------------------------------------------------------------------------------------

# Require the minimum version of any system that we test on - which is the version used by Travis.
# Thus, we can be sure that this works (as long as they don't change it...).
cmake_minimum_required (VERSION 3.0 FATAL_ERROR)
message (STATUS "CMake version ${CMAKE_VERSION}")

set (CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set (CMAKE_DISABLE_SOURCE_CHANGES  ON)

if (NOT WIN32)
    # Fun with colors!
    string(ASCII 27 Esc)

    set(ColorBlue   "${Esc}[34m") # Build information.
    set(ColorGreen  "${Esc}[32m") # Optional additions.
    set(ColorYellow "${Esc}[33m") # Tipps for the user.
    set(ColorRed    "${Esc}[31m") # Warnings and Errors.
    set(ColorEnd    "${Esc}[0m")
endif()

# --------------------------------------------------------------------------------------------------
#   Project Definition and Requirements
# --------------------------------------------------------------------------------------------------

project ( parallel_sorter CXX C)

# set (parallel_sorter_VERSION_MAJOR 0)
# set (parallel_sorter_VERSION_MINOR 1)

# Helpful output. Version information is unfortunately only available for CMake >= 2.8.9
message (STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} at ${CMAKE_CXX_COMPILER}")
message (STATUS "C compiler  : ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION} at ${CMAKE_C_COMPILER}")

# --------------------------------------------------------------------------------------------------
#   Build Options
# --------------------------------------------------------------------------------------------------

option (BUILD_SHARED_LIB    "Build shared library" ON)
option (BUILD_STATIC_LIB    "Build static library" OFF)

option (BUILD_APPLICATIONS  "Build applications"   OFF)

option (USE_THREADS         "Use a threading library (mainly, pthreads)"      OFF)
option (USE_OPENMP          "Use OpenMP if available"                         ON)

# Define unity build option with list of valid values.
set( UNITY_BUILD FULL CACHE STRING "Build modules monolithically - trading memory for speed." )
set_property( CACHE UNITY_BUILD PROPERTY STRINGS FULL MODULES OFF )

if (BUILD_APPLICATIONS OR BUILD_PYTHON_MODULE OR BUILD_TESTS)
    # The applications, the python module and the tests all use the shared lib,
    # so we need to build it if one of those is activated.
    set (BUILD_SHARED_LIB ON)
endif()

# --------------------------------------------------------------------------------------------------
#   Build Type
# --------------------------------------------------------------------------------------------------

set (CMAKE_BUILD_TYPE DEBUG)


message (STATUS "${ColorBlue}Building ${CMAKE_BUILD_TYPE}${ColorEnd}")
message (STATUS "Unity build: ${UNITY_BUILD}")

# --------------------------------------------------------------------------------------------------
#   Compiler and Linker Options
# --------------------------------------------------------------------------------------------------

# Set the warn flags to a very high level - except unknown pragmas, as this causes needless
# warnings with OpenMP and other pragma-based techniques.
set (WARN_FLAGS              "-Wall -Wextra -Wno-unknown-pragmas -pedantic -pedantic-errors")
set (CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=c++11 ${WARN_FLAGS}")

set (CMAKE_CXX_FLAGS_DEBUG   "-O2 -g -ggdb3 -DDEBUG -D_GLIBCXX_DEBUG")
set (CMAKE_CXX_FLAGS_RELEASE "-O2       -DNDEBUG")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Misc options
# STDCC = clang++
# MPICC   = mpic++
# mpi: CCFLAGS += -DUSE_MPI
# LDFLAGS = -lm $(THREADS)

# Pre-compilation check for endianness
include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if( IS_BIG_ENDIAN )
    message(
        STATUS
        "${ColorYellow}Your system uses big endian memory, which is rare. Genesis is not tested "
        "with such systems. There might be bugs because of this!${ColorEnd}"
    )
endif()


find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")


# --------------------------------------------------------------------------------------------------
#   stxxl library
# --------------------------------------------------------------------------------------------------
# disallow in-source builds
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

# include the STXXL library
add_subdirectory(stxxl)

# apply STXXL CXXFLAGS to our configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STXXL_CXX_FLAGS}")

# add STXXL includes path
include_directories(${STXXL_INCLUDE_DIRS})

# build a program and link it with STXXL.
add_executable(parallel_testing parallel_testing.cpp)
target_link_libraries(parallel_testing ${STXXL_LIBRARIES})

set_property(TARGET parallel_testing PROPERTY CXX_STANDARD 11)
set_target_properties(parallel_testing PROPERTIES INSTALL_RPATH ${PROJECT_SOURCE_DIR}/build)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
