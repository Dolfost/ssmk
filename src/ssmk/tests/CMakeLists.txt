file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

add_subdirectory(pnggen)
add_custom_target(generate_png ALL pnggen
	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/projects/generated/png"
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(ssmk_${TNAME} 
		${T}
	)
	target_compile_definitions(ssmk_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
		PRIVATE TESTFILE="${T}"
	)
	target_link_libraries(ssmk_${TNAME} 
		sm::ssmk
	)
	add_test(NAME ssmk_${TNAME}
		COMMAND ssmk_${TNAME}
	)

	if(VALGRIND_EXECUTABLE)
		set_tests_properties(ssmk_${TNAME} 
			PROPERTIES FIXTURES_SETUP "ssmk_${TNAME}"
		)
		add_test(NAME ssmk_valgrind_${TNAME}
			COMMAND valgrind --leak-check=full --track-origins=yes -s
			--error-exitcode=1 $<TARGET_FILE:ssmk_${TNAME}>
		)
		set_tests_properties(ssmk_valgrind_${TNAME} 
			PROPERTIES FIXTURES_REQUIRED "ssmk_${TNAME}"
		)
	endif()
endforeach()
