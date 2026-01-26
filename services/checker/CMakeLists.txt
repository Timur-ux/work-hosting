add_executable(checker main.cpp)
target_link_libraries(checker PRIVATE libzmq-static ${CMAKE_PROJECT_NAME}_LIB)
