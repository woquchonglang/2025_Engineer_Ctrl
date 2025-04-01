cmake_minimum_required(VERSION 3.16)
project(sim)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)


# Find and link the system SDL3 library
find_package(SDL3 REQUIRED)
include_directories(${SDL3_INCLUDE_DIRS})

macro(FIND_INCLUDE_DIR result curdir)
  file(GLOB_RECURSE children "${curdir}/*.hpp" "${curdir}/*.h")
  set(dirlist "")
  foreach(child ${children})
    string(REGEX REPLACE "(.*)/.*" "\\1" LIB_NAME ${child})
    if(IS_DIRECTORY ${LIB_NAME})
      list(FIND dirlist ${LIB_NAME} list_index)

      if(${list_index} LESS 0)
        list(APPEND dirlist ${LIB_NAME})
      endif()
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

find_include_dir(INCLUDE_DIR_LIST lib)

file(GLOB_RECURSE SOURCE "lib/MiaoUI/*.*" "lib/u8g2/*.*")

# Create your game executable target as usual
add_executable(sim sim.c)

# Link to the actual SDL3 library.
target_link_libraries(sim PRIVATE ${SDL3_LIBRARIES})