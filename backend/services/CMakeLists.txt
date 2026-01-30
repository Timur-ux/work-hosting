if(BUILD_api OR BUILD_ALL)
	add_subdirectory(api)
endif()

if(BUILD_checker OR BUILD_ALL)
	add_subdirectory(checker)
endif()
