set(service_ns work_hosting)

userver_add_sql_library(
    api_sql
    NAMESPACE
    ${service_ns}
    SOURCE_DIR
    queries
    OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    SQL_FILES
    *.sql
)

userver_embed_file(api_config NAME static_config_yaml FILEPATH ${CMAKE_CURRENT_SOURCE_DIR}/static_config.yaml)
file(GLOB_RECURSE api_schemas ${CMAKE_CURRENT_LIST_DIR}/schemas/*.yaml)
userver_target_generate_chaotic(
		api-chgen
    # Generate serializers for responses
    GENERATE_SERIALIZERS
    # Map '/components/schemas/*' JSONSchema types to C++ types in 'samples::STEM' namespace
		LAYOUT "/components/schemas/([^/]*)/={stem}::{0}"
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/include
    SCHEMAS ${api_schemas}
    RELATIVE_TO ${CMAKE_CURRENT_SOURCE_DIR}
)


file(GLOB_RECURSE service_src LIST_DIRECTORIES FALSE ./src/*.cpp)

add_library(api_lib STATIC ${service_src})
target_include_directories(api_lib PUBLIC include PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(api_lib PUBLIC userver::core userver::postgresql api_sql api-chgen ${CMAKE_PROJECT_NAME}_LIB cppzmq-static)
target_compile_definitions(api_lib PUBLIC SERVICE_NAMESPACE=${service_ns})

add_executable(api)
target_sources(api PUBLIC main.cpp)
target_link_libraries(api PRIVATE api_lib api_config)
target_include_directories(api PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
