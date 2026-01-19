file(GLOB_RECURSE handlers_src LIST_DIRECTORIES FALSE ./src/handlers/*.cpp)
file(GLOB_RECURSE components_src LIST_DIRECTORIES FALSE ./src/components/*.cpp)

add_library(service_lib STATIC ${handlers_src} ${components_src})
target_include_directories(service_lib PUBLIC include)
target_link_libraries(service_lib PUBLIC userver::core userver::mongo userver::redis userver::postgresql)
target_compile_definitions(service_lib PUBLIC SERVICE_NAMESPACE=work_hosting)
