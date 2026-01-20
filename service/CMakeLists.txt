file(GLOB_RECURSE service_schemas ${CMAKE_CURRENT_LIST_DIR}/schemas/*.yaml)
userver_target_generate_chaotic(
		service-chgen
    # Generate serializers for responses
    GENERATE_SERIALIZERS
    # Map '/components/schemas/*' JSONSchema types to C++ types in 'samples::STEM' namespace
		LAYOUT "/components/schemas/([^/]*)/={stem}::{0}"
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/include
    SCHEMAS ${service_schemas}
    RELATIVE_TO ${CMAKE_CURRENT_SOURCE_DIR}
)


file(GLOB_RECURSE handlers_src LIST_DIRECTORIES FALSE ./src/handlers/*.cpp)
file(GLOB_RECURSE components_src LIST_DIRECTORIES FALSE ./src/components/*.cpp)

add_library(service_lib STATIC ${handlers_src} ${components_src})
target_include_directories(service_lib PUBLIC include PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(service_lib PUBLIC userver::core userver::postgresql service-chgen ${CMAKE_PROJECT_NAME}_LIB)
target_compile_definitions(service_lib PUBLIC SERVICE_NAMESPACE=work_hosting)
