cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(roo)

# ist(APPEND CMAKE_PREFIX_PATH $ENV{..ROOTSYS})

find_package(ROOT REQUIRED COMPONENTS RooFit, Math)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#ROOT_GENERATE_DICTIONARY(G__Event include/Event.h LINKDEF EventLinkDef.h)


# add_library(Event SHARED Event.cxx G__Event.cxx)
# target_link_libraries(Event PUBLIC ROOT)

# Create the main program using the library.
add_executable(main RooUnfoldGSOC.cxx)

target_link_libraries(main ROOT::RooFit, ROOT::Math)





