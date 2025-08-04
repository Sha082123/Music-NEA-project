#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mx::mx" for configuration "Release"
set_property(TARGET mx::mx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mx::mx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/mx.lib"
  )

list(APPEND _cmake_import_check_targets mx::mx )
list(APPEND _cmake_import_check_files_for_mx::mx "${_IMPORT_PREFIX}/lib/mx.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
