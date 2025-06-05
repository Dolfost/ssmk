
add_subdirectory(pnggen)
add_custom_target(generate_png ALL pnggen --silent
	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/projects/generated/png"
)

function(add_tests where)
	file(GLOB TESTS CONFIGURE_DEPENDS
		${where}/*.cpp 
	)
	foreach(T ${TESTS})
		cmake_path(GET T STEM TNAME)
		set(TEST "ssmk_${where}-${TNAME}")
		add_executable(${TEST}
			${T}
		)
		target_compile_definitions(${TEST}
			PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
			PRIVATE TESTFILE="${T}"
		)
		target_link_libraries(${TEST} 
			sm::ssmk
		)
		add_test(NAME ${TEST}
			COMMAND ${TEST}
		)

		if(VALGRIND_EXECUTABLE)
			set_tests_properties(${TEST} 
				PROPERTIES FIXTURES_SETUP "${TEST}"
			)
			add_test(NAME valgrind_${TNAME}
				COMMAND valgrind --leak-check=full --track-origins=yes -s
				--error-exitcode=1 $<TARGET_FILE:${TEST}>
			)
			set_tests_properties(valgrind_${TEST} 
				PROPERTIES FIXTURES_REQUIRED "${TEST}"
			)
		endif()
	endforeach()
endfunction()

add_tests(reader)
add_tests(writer)
