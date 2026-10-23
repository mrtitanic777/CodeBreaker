# Seamless switches between the ports' processes (Shared/runtime/transition/transition/transition.h): the black
# cover window, launching with a ready event, SignalReady. A runtime includes this file and adds the sources:
#   include(".../Shared/runtime/transition/transition.cmake")
#   target_sources(<runtime> PRIVATE ${X360_TRANSITION_SOURCES})
#   target_include_directories(<runtime> PRIVATE ${X360_TRANSITION_INCLUDE_DIR})
# The sources log through the runtime's "log.h" (Log), like Shared/runtime/xui and live.
set(X360_TRANSITION_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(X360_TRANSITION_SOURCES "${CMAKE_CURRENT_LIST_DIR}/transition/transition.cpp")
