# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Cat_Jack_21_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Cat_Jack_21_autogen.dir\\ParseCache.txt"
  "Cat_Jack_21_autogen"
  )
endif()
