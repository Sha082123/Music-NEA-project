# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\appMusic_NEA_MSVC_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appMusic_NEA_MSVC_autogen.dir\\ParseCache.txt"
  "appMusic_NEA_MSVC_autogen"
  )
endif()
