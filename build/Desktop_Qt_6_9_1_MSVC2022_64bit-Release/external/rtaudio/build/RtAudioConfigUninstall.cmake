if(NOT EXISTS "D:/Comp Sci NEA/Music_NEA_MSVC/build/Desktop_Qt_6_9_1_MSVC2022_64bit-Release/external/rtaudio/build/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"D:/Comp Sci NEA/Music_NEA_MSVC/build/Desktop_Qt_6_9_1_MSVC2022_64bit-Release/external/rtaudio/build/install_manifest.txt\"")
endif(NOT EXISTS "D:/Comp Sci NEA/Music_NEA_MSVC/build/Desktop_Qt_6_9_1_MSVC2022_64bit-Release/external/rtaudio/build/install_manifest.txt")

file(READ "D:/Comp Sci NEA/Music_NEA_MSVC/build/Desktop_Qt_6_9_1_MSVC2022_64bit-Release/external/rtaudio/build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if(EXISTS "$ENV{DESTDIR}${file}")
    exec_program(
      "C:/Program Files/CMake/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
      )
    if(NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif(NOT "${rm_retval}" STREQUAL 0)
  else(EXISTS "$ENV{DESTDIR}${file}")
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif(EXISTS "$ENV{DESTDIR}${file}")
endforeach(file)
