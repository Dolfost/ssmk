set(CMAKE_CXX_STANDARD 17)

find_program(VALGRIND_EXECUTABLE NAMES valgrind)
if(VALGRIND_EXECUTABLE)
	message(STATUS "found valgrind: ${VALGRIND_EXECUTABLE}")
	add_definitions(-DCALGO_HAVE_VALGRIND) # will be visible in subdirs
else()
	message(STATUS "valgrind not found")
endif()

add_subdirectory(ssmk)
add_subdirectory(cli)
