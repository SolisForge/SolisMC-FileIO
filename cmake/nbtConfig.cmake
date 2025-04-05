@PACKAGE_INIT@

set(NBT_TARGETS "${CMAKE_CURRENT_LIST_DIR}/nbtTargets.cmake")
if (EXISTS ${NBT_TARGETS})
  include(${NBT_TARGETS})
endif()