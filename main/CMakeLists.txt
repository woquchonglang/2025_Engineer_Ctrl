
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

find_include_dir(INCLUDE_DIR_LIST main)

file(GLOB_RECURSE SOURCE "hdl/*.*" "app/*.*" "algorithm/*.*" "hal/*.*" "ui/MiaoUI/*.*" "ui/lib/*.*")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

idf_component_register(
  SRCS
    ${SOURCE}
    "main.cpp"
  INCLUDE_DIRS
    ${INCLUDE_DIR_LIST}/
)
